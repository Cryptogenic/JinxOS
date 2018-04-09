#ifndef _SYS_MAILBOX_H
#define _SYS_MAILBOX_H

#ifdef RPI2
    #define MAILBOX_BASE   0x3F00B880UL
#else
    #define MAILBOX_BASE   0x2000B880UL
#endif

typedef enum
{
    MB_POWER_MANAGEMENT = 0,
    MB_FRAMEBUFFER,
    MB_VIRTUAL_UART,
    MB_VCHIQ,
    MB_LEDS,
    MB_BUTTONS,
    MB_TOUCHSCREEN,
    MB_RESERVED,
    MB_TAGS_ARM_TO_VC,
    MB_TAGS_VC_TO_ARM,
} mailbox_channel_t;

enum mailbox_status_reg_bits
{
    ARM_MS_FULL  = 0x80000000,
    ARM_MS_EMPTY = 0x40000000,
    ARM_MS_LEVEL = 0x400000FF,
};

typedef struct
{
    volatile unsigned int Read;
    volatile unsigned int reserved[3];
    volatile unsigned int Poll;
    volatile unsigned int Sender;
    volatile unsigned int Status;
    volatile unsigned int Configuration;
    volatile unsigned int Write;
} mailbox_t;

/*
 * Provides a wrapper to write values to / read values from
 * mailbox0 in the specified channel.
 */
extern int mailbox_write(mailbox_channel_t channel, unsigned int value);
extern int mailbox_read(mailbox_channel_t channel);

/*
 * Mutex for mailbox0:
 * Mailbox has a "status" member which indicates if it's full/empty,
 * depending on the request, we'll busy wait until it's been made
 * available again.
 */
void _mtx_mailbox_waitw();
void _mtx_mailbox_waitr();

#endif