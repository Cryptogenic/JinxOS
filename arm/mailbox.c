#include <sys/mailbox.h>
#include <sys/errno.h>

#include <stdint.h>

static mailbox_t* rpiMailbox = (mailbox_t*)MAILBOX_BASE;

int mailbox_write(mailbox_channel_t channel, unsigned int value)
{
	if(channel > 0xF)
	{
		/*
		 * Only 15 possible channels, if user
		 * has specified more, return ERR_INVALARG.
		 */
		error(ERR_INVALARG);
		return -1;
	}

	// Lower 4-bits of the value should contain the channel number.
	value &= ~(0xF);
	value |= channel;

	// Wait on a status update
	_mtx_mailbox_waitw();

	rpiMailbox->Write = value;

	// On success, return the written value
	return value;
}

int mailbox_read(mailbox_channel_t channel)
{
	int value = 0;

	// Only read values that are specific to our channel
	while((value & 0xF) != channel)
	{
		// Wait on a status update
		_mtx_mailbox_waitr();

		value = rpiMailbox->Read;
	}

	// Lower 4-bits of the value is the channel, ommit
	return value >> 4;
}

void _mtx_mailbox_waitw()
{
	while((rpiMailbox->Status & ARM_MS_FULL) != 0)
	{
		// Busy Wait
	}
}

void _mtx_mailbox_waitr()
{
	while((rpiMailbox->Status & ARM_MS_EMPTY))
	{
		// Busy Wait
	}
}