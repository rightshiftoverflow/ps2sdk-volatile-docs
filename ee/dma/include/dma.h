/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# (c) 2005 Naomi Peori <naomi@peori.ca>
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
*/

/**
 * @file
 * DMA channel utilities
 */

#ifndef __DMA_H__
#define __DMA_H__

#include <tamtypes.h>

#define DMA_CHANNEL_VIF0     0x00
#define DMA_CHANNEL_VIF1     0x01
#define DMA_CHANNEL_GIF      0x02
#define DMA_CHANNEL_fromIPU  0x03
#define DMA_CHANNEL_toIPU    0x04
#define DMA_CHANNEL_fromSIF0 0x05
#define DMA_CHANNEL_toSIF1   0x06
#define DMA_CHANNEL_SIF2     0x07
#define DMA_CHANNEL_fromSPR  0x08
#define DMA_CHANNEL_toSPR    0x09

#define DMA_FLAG_TRANSFERTAG   0x01
#define DMA_FLAG_INTERRUPTSAFE 0x02

#ifdef __cplusplus
extern "C" {
#endif

/**
 *	Shuts down all dma channels
 */
int dma_reset(void);

/**
 *	Initializes the specified dma channel.
 *	@param channel 	Channel to initialize. Could be {@link DMA_CHANNEL_VIF0}
 *	@param handler 	A "controller" handler that is triggered when certain
 *					actions happen on the DMA
 *	@param flags 	A bitlist of flags. Supported values are
 *					{@link DMA_FLAG_TRANSFERTAG} and {@link DMA_FLAG_INTERRUPTSAFE}
 *	@return -1 on fail, 0 on success
 */
int dma_channel_initialize(int channel, void *handler, int flags);

/**
 *	Enables dma fast waits for a channel
 *	@param channel 	Channel that allows fast waits (don't know what this means yet...)
 */
void dma_channel_fast_waits(int channel);

/**
 *	Waits until channel is usable based on coprocessor status
 */
void dma_wait_fast(void);

/**
 *	Wait until the specified dma channel is ready
 *	@param channel 	Channel to wait for
 *	@param timeout 	An integer specifying how long until the the function will 
 *					give up. You can sort of time this 
 *	@return 0 on success, -1 if the function reached timeout
 */
int dma_channel_wait(int channel, int timeout);

/**
 *	Send a dmachain to the specified dma channel.
 *	@param channel 	Channel to send the chain to
 *	@param data 	Data sent over the dma
 *	@param qwc 		Length of the data
 *	@param flags 	A bitlist of flags. Supported values are
 *					{@link DMA_FLAG_TRANSFERTAG} and {@link DMA_FLAG_INTERRUPTSAFE}
 *	@param spr 		0 or 1 indicating if the tag is located in Scratchpad RAM
 *	@return always 0
 */
int dma_channel_send_chain(int channel, void *data, int qwc, int flags, int spr);

/**
 *	Send a Uncached Accelerated Memory (ucab) dmachain to the specified dma channel
 *	@param channel 	Channel to send the chain to
 *	@param data 	Data sent over the dma
 *	@param qwc 		Length of the data
 *	@param flags 	A bitlist of flags. Supported values are
 *					{@link DMA_FLAG_TRANSFERTAG} and {@link DMA_FLAG_INTERRUPTSAFE}
 *	@return always 0
 */
int dma_channel_send_chain_ucab(int channel, void *data, int qwc, int flags);

/** Send data to the specified dma channel. */
int dma_channel_send_normal(int channel, void *data, int qwc, int flags, int spr);

/** Send ucab data to the specified dma channel. */
int dma_channel_send_normal_ucab(int channel, void *data, int qwc, int flags);

/** Receive data from the specified dma channel. */
int dma_channel_receive_normal(int channel, void *data, int data_size, int flags, int spr);

/** Receive data from the specified dma channel. */
int dma_channel_receive_chain(int channel, void *data, int data_size, int flags, int spr);

/** 
 *	Shut down the specified dma channel.
 *	@param channel 	Channel that will be killed
 *	@param flags 	A bitlist of flags. Supported values are
 *					{@link DMA_FLAG_TRANSFERTAG} and {@link DMA_FLAG_INTERRUPTSAFE}
 *	@return always 0	
 */
int dma_channel_shutdown(int channel, int flags);

#ifdef __cplusplus
}
#endif

#endif /* __DMA_H__ */
