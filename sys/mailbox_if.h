#ifndef _SYS_MAILBOX_IF_H
#define _SYS_MAILBOX_IF_H

/*
 * VideoCore IV firmware mailbox enums/structures. Provided courteously by
 * Raspberry Pi Foundation @ 
 * https://github.com/raspberrypi/firmware/wiki/Mailbox-property-interface.
 *
 * Implementations based on work courteously provided by Brian Sidebotham's
 * "ARM Bare Metal OS" tutorials.
 */

// Properties
typedef struct
{
    int tag;
    int length;

    union
    {
        int val_s32;
        unsigned char buf_u8[0x100];
        int buf_s32[64];
    } data;
} mailbox_if_property_t;

// Tags for properties
typedef enum
{
    // VideoCore
    TAG_VC_GET_FW_REVISION              = 0x00000001,

    // Hardware
    TAG_HW_GET_BOARD_MODEL              = 0x00010001,
    TAG_HW_GET_BOARD_REVISION           = 0x00010002,
    TAG_HW_GET_BOARD_MAC                = 0x00010003,
    TAG_HW_GET_BOARD_SERIAL             = 0x00010004,
    TAG_HW_GET_ARM_MEMORY               = 0x00010005,
    TAG_HW_GET_VC_MEMORY                = 0x00010006,
    TAG_HW_GET_CLOCKS                   = 0x00010007,

    // Power
    TAG_POWER_GET_STATE                 = 0x00020001,
    TAG_POWER_GET_TIMING                = 0x00020002,

    TAG_POWER_SET_STATE                 = 0x00028001,

    // Clock
    TAG_CLOCK_GET_STATE                 = 0x00030001,
    TAG_CLOCK_GET_RATE                  = 0x00030002,
    TAG_CLOCK_GET_MAXRATE               = 0x00030004,
    TAG_CLOCK_GET_MINRATE               = 0x00030007,
    TAG_CLOCK_GET_TURBO                 = 0x00030009,

    TAG_CLOCK_SET_STATE                 = 0x00038001,
    TAG_CLOCK_SET_RATE                  = 0x00038002,
    TAG_CLOCK_SET_TURBO                 = 0x00038009,

    // Voltage
    TAG_VOLT_GET_VOLTAGE                = 0x00030003,
    TAG_VOLT_GET_MAXVOLT                = 0x00030005,
    TAG_VOLT_GET_MINVOLT                = 0x00030008,

    TAG_VOLT_SET_VOLTAGE                = 0x00038003,

    // Temperature
    TAG_TEMP_GET_TEMP                   = 0x00030006,
    TAG_TEMP_GET_MAXTEMP                = 0x0003000A,

    // Memory
    TAG_MEM_MALLOC                      = 0x0003000C,
    TAG_MEM_MLOCK                       = 0x0003000D,
    TAG_MEM_MUNLOCK                     = 0x0003000E,
    TAG_MEM_MRELEASE                    = 0x0003000F,
    TAG_MEM_MEXEC                       = 0x00030010,
    TAG_MEM_GET_DISPMANX_HANDLE         = 0x00030014,
    TAG_MEM_GET_EDID_BLOCK              = 0x00030020,

    // Frame Buffer
    TAG_FRAMEBUFFER_ALLOC               = 0x00040001,
    TAG_FRAMEBUFFER_RELEASE             = 0x00048001,
    TAG_FRAMEBUFFER_BLANK               = 0x00040002,

    TAG_FRAMEBUFFER_GET_PHYS_SIZE       = 0x00040003,
    TAG_FRAMEBUFFER_GET_VIRT_SIZE       = 0x00040004,
    TAG_FRAMEBUFFER_GET_DEPTH           = 0x00040005,
    TAG_FRAMEBUFFER_GET_PIXEL_ORDER     = 0x00040006,
    TAG_FRAMEBUFFER_GET_ALPHA_MODE      = 0x00040007,
    TAG_FRAMEBUFFER_GET_PITCH           = 0x00040008,
    TAG_FRAMEBUFFER_GET_VIRT_OFFSET     = 0x00040009,
    TAG_FRAMEBUFFER_GET_OVERSCAN        = 0x0004000A,
    TAG_FRAMEBUFFER_GET_PALETTE         = 0x0004000B,

    TAG_FRAMEBUFFER_TEST_PHYS_SIZE      = 0x00044003,
    TAG_FRAMEBUFFER_TEST_VIRT_SIZE      = 0x00044004,
    TAG_FRAMEBUFFER_TEST_DEPTH          = 0x00044005,
    TAG_FRAMEBUFFER_TEST_PIXEL_ORDER    = 0x00044006,
    TAG_FRAMEBUFFER_TEST_ALPHA_MODE     = 0x00044007,
    TAG_FRAMEBUFFER_TEST_VIRT_OFFSET    = 0x00044009,
    TAG_FRAMEBUFFER_TEST_OVERSCAN       = 0x0004400A,
    TAG_FRAMEBUFFER_TEST_PALETTE        = 0x0004400B,

    TAG_FRAMEBUFFER_SET_PHYS_SIZE       = 0x00048003,
    TAG_FRAMEBUFFER_SET_VIRT_SIZE       = 0x00048004,
    TAG_FRAMEBUFFER_SET_DEPTH           = 0x00048005,
    TAG_FRAMEBUFFER_SET_PIXEL_ORDER     = 0x00048006,
    TAG_FRAMEBUFFER_SET_ALPHA_MODE      = 0x00048007,
    TAG_FRAMEBUFFER_SET_VIRT_OFFSET     = 0x00048009,
    TAG_FRAMEBUFFER_SET_OVERSCAN        = 0x0004800A,
    TAG_FRAMEBUFFER_SET_PALETTE         = 0x0004800B,

    TAG_FRAMEBUFFER_SET_CURSOR_INFO     = 0x00008010,
    TAG_FRAMEBUFFER_SET_CURSOR_STATE    = 0x00008011,

    // Configuration
    TAG_CONFIG_GET_CMD_LINE             = 0x00050001,

    // Direct Memory Channels
    TAG_DMA_GET_CHANNELS                = 0x00060001,
} mailbox_if_tag_t;

// States for tags
typedef enum
{
    TAG_STATE_REQUEST   = 0x0,
    TAG_STATE_RESPONSE  = 0x1,
} mailbox_if_tag_state_t;

// Clock IDs
typedef enum 
{
    TAG_CLOCK_RESERVED  = 0x0,
    TAG_CLOCK_EMMC      = 0x1,
    TAG_CLOCK_UART      = 0x2,
    TAG_CLOCK_ARM       = 0x3,
    TAG_CLOCK_CORE      = 0x4,
    TAG_CLOCK_V3D       = 0x5,
    TAG_CLOCK_H264      = 0x6,
    TAG_CLOCK_ISP       = 0x7,
    TAG_CLOCK_SDRAM     = 0x8,
    TAG_CLOCK_PIXEL     = 0x9,
    TAG_CLOCK_PWM       = 0xA, 
} mailbox_if_clock_id_t;

// Buffer offsets
typedef enum
{
    PT_SIZE             = 0x0,
    PT_REQ_OR_RESP      = 0x1,
} mailbox_if_tag_pt_t;

// Offsets
typedef enum
{
    O_IDENT         = 0x0,
    O_VALUE_SIZE    = 0x1,
    O_RESPONSE      = 0x2,
    O_VALUE         = 0x3,
} mailbox_if_tag_offset_t;

extern void mailbox_if_property_init(void);
extern int mailbox_if_property_process(void);
extern mailbox_if_property_t *mailbox_if_property_get(mailbox_if_tag_t tag);
extern int mailbox_if_add_tag(mailbox_if_tag_t tag, ...);

#endif