#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#include <sys/mailbox.h>
#include <sys/mailbox_if.h>

// Property Tag buffer must be aligned to a 16-byte boundary
static int property_tags[0x2000] __attribute__((aligned(0x10)));
static int property_tags_idx = 0;

void mailbox_if_property_init(void)
{
	property_tags[PT_SIZE] = 0xC;
	property_tags[PT_REQ_OR_RESP] = 0x0;
	property_tags_idx = 2;
	property_tags[property_tags_idx] = 0x0;
}

int mailbox_if_property_process(void)
{
	int retval;

	// Fill buffer size
	property_tags[PT_SIZE] = (property_tags_idx + 1) << 2;
	property_tags[PT_REQ_OR_RESP] = 0x0;

	// Send a mailbox request to the ARM -> VideoCore channel
	mailbox_write(MB_TAGS_ARM_TO_VC, (unsigned int)property_tags);

	// Receive a mailbox response
	retval = mailbox_read(MB_TAGS_ARM_TO_VC);

	return retval;
}

mailbox_if_property_t *mailbox_if_property_get(mailbox_if_tag_t tag)
{
	static mailbox_if_property_t prop;
	int *tag_buf = 0;

	prop.tag = tag;

	int idx;

	// Find tag
	for(idx = 2; idx < (property_tags[PT_SIZE] >> 2); idx += (property_tags[idx + 1] >> 2) + 3)
	{
		if(property_tags[idx] == tag)
		{
			tag_buf = &property_tags[idx];
			break;
		}
	}

	// Tag not found? Return null
	if(tag_buf == NULL)
		return NULL;

	// If tag is found, return the requested data
	prop.length = tag_buf[O_RESPONSE] & 0xFFFF;
	memcpy(prop.data.buf_u8, &tag_buf[O_VALUE], prop.length);

	return &prop;
}

int mailbox_if_add_tag(mailbox_if_tag_t tag, ...)
{
	va_list vl;
	va_start(vl, tag);

	property_tags[property_tags_idx++] = tag;

	switch(tag)
	{
		case TAG_VC_GET_FW_REVISION:
        case TAG_HW_GET_BOARD_MODEL:
        case TAG_HW_GET_BOARD_REVISION:
        case TAG_HW_GET_BOARD_MAC:
        case TAG_HW_GET_BOARD_SERIAL:
        case TAG_HW_GET_ARM_MEMORY:
        case TAG_HW_GET_VC_MEMORY:
        case TAG_DMA_GET_CHANNELS:
            property_tags[property_tags_idx++] = 0x8;
            property_tags[property_tags_idx++] = 0;
            property_tags_idx += 2;
            break;

        case TAG_HW_GET_CLOCKS:
        case TAG_CONFIG_GET_CMD_LINE:
            property_tags[property_tags_idx++] = 0x100;
            property_tags[property_tags_idx++] = 0;
            property_tags_idx += 0x100 >> 2;
            break;

        case TAG_FRAMEBUFFER_ALLOC:
        case TAG_CLOCK_GET_MAXRATE:
        case TAG_CLOCK_GET_MINRATE:
        case TAG_CLOCK_GET_RATE:
            property_tags[property_tags_idx++] = 0x8;
            property_tags[property_tags_idx++] = 0;
            property_tags[property_tags_idx++] = va_arg( vl, int );
            property_tags[property_tags_idx++] = 0;
            break;

        case TAG_CLOCK_SET_STATE:
            property_tags[property_tags_idx++] = 0xC;
            property_tags[property_tags_idx++] = 0;
            property_tags[property_tags_idx++] = va_arg( vl, int );
            property_tags[property_tags_idx++] = va_arg( vl, int );
            property_tags[property_tags_idx++] = va_arg( vl, int );
            break;

        case TAG_FRAMEBUFFER_GET_PHYS_SIZE:
        case TAG_FRAMEBUFFER_SET_PHYS_SIZE:
        case TAG_FRAMEBUFFER_TEST_PHYS_SIZE:
        case TAG_FRAMEBUFFER_GET_VIRT_SIZE:
        case TAG_FRAMEBUFFER_SET_VIRT_SIZE:
        case TAG_FRAMEBUFFER_TEST_VIRT_SIZE:
        case TAG_FRAMEBUFFER_GET_VIRT_OFFSET:
        case TAG_FRAMEBUFFER_SET_VIRT_OFFSET:
            property_tags[property_tags_idx++] = 8;
            property_tags[property_tags_idx++] = 0;

            if((tag == TAG_FRAMEBUFFER_SET_PHYS_SIZE) ||
               (tag == TAG_FRAMEBUFFER_SET_VIRT_SIZE) ||
               (tag == TAG_FRAMEBUFFER_SET_VIRT_OFFSET) ||
               (tag == TAG_FRAMEBUFFER_TEST_PHYS_SIZE) ||
               (tag == TAG_FRAMEBUFFER_TEST_VIRT_SIZE))
            {
                property_tags[property_tags_idx++] = va_arg(vl, int);
                property_tags[property_tags_idx++] = va_arg(vl, int);
            }
            else
            {
                property_tags_idx += 2;
            }
            break;

        case TAG_FRAMEBUFFER_GET_ALPHA_MODE:
        case TAG_FRAMEBUFFER_SET_ALPHA_MODE:
        case TAG_FRAMEBUFFER_GET_DEPTH:
        case TAG_FRAMEBUFFER_SET_DEPTH:
        case TAG_FRAMEBUFFER_GET_PIXEL_ORDER:
        case TAG_FRAMEBUFFER_SET_PIXEL_ORDER:
        case TAG_FRAMEBUFFER_GET_PITCH:
            property_tags[property_tags_idx++] = 4;
            property_tags[property_tags_idx++] = 0;

            if((tag == TAG_FRAMEBUFFER_SET_DEPTH) ||
               (tag == TAG_FRAMEBUFFER_SET_PIXEL_ORDER) ||
               (tag == TAG_FRAMEBUFFER_SET_ALPHA_MODE))
            {
                property_tags[property_tags_idx++] = va_arg(vl, int);
            }
            else
            {
                property_tags_idx += 1;
            }
            break;

        case TAG_FRAMEBUFFER_GET_OVERSCAN:
        case TAG_FRAMEBUFFER_SET_OVERSCAN:
            property_tags[property_tags_idx++] = 16;
            property_tags[property_tags_idx++] = 0;

            if((tag == TAG_FRAMEBUFFER_SET_OVERSCAN))
            {
                property_tags[property_tags_idx++] = va_arg(vl, int);
                property_tags[property_tags_idx++] = va_arg(vl, int);
                property_tags[property_tags_idx++] = va_arg(vl, int);
                property_tags[property_tags_idx++] = va_arg(vl, int);
            }
            else
            {
                property_tags_idx += 4;
            }
            break;

        default:
            /* Remove unknown tags */
            property_tags_idx--;
            break;
    }

    /* Tag list must be null terminated to end the list */
    property_tags[property_tags_idx] = 0x00;

    va_end(vl);
}
