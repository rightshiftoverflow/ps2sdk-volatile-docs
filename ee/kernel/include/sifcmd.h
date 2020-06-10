/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# (C)2001, Gustavo Scotti (gustavo@scotti.com)
# (c) 2003 Marcus R. Brown (mrbrown@0xd6.org)
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
*/

/**
 *	@file
 *	EE SIF command handling. Use RPC for a higher-level implementation.
 *	@see {@link /ee/kernel/include/sifrpc.h sifrpc.h}
 */

#ifndef __SIFCMD_H__
#define __SIFCMD_H__

#include <tamtypes.h>

/**
 *	Header for a command packet
 */
typedef struct t_SifCmdHeader
{
   /** Packet size. Min: 1x16 (header only), max: 7*16 */
   u32				psize:8;
   /** Payload size */
   u32				dsize:24;
   /** Destination address for payload. Can be NULL if there is no payload. */
   void				*dest;
   /** Command ID. Function number of function to call. */
   int				cid;
   /** Options. Can be freely used. */
   u32				opt;
} SifCmdHeader_t;

/** 
  *	Address to the root of system command handlers
  */
#define	SIF_CMD_ID_SYSTEM	0x80000000

// System handlers
/** Handler to change the system address */
#define SIF_CMD_CHANGE_SADDR	(SIF_CMD_ID_SYSTEM | 0)
/** Handler to set a software register */
#define SIF_CMD_SET_SREG	(SIF_CMD_ID_SYSTEM | 1)
/** Hander to initialize command handlers */
#define SIF_CMD_INIT_CMD	(SIF_CMD_ID_SYSTEM | 2)
/** Handler to reset command handlers */
#define SIF_CMD_RESET_CMD	(SIF_CMD_ID_SYSTEM | 3)
/** Handler to end a Remote Procedure Call */
#define SIF_CMD_RPC_END		(SIF_CMD_ID_SYSTEM | 8)
/** Handler to bind a Remote Procedure Call */
#define SIF_CMD_RPC_BIND	(SIF_CMD_ID_SYSTEM | 9)
/** Handler to call a Remote Procedure */
#define SIF_CMD_RPC_CALL	(SIF_CMD_ID_SYSTEM | 10)
/** Handler to read Remote Procedure data */
#define SIF_CMD_RPC_RDATA	(SIF_CMD_ID_SYSTEM | 12)

/** System software register (SREG) */
#define SIF_SREG_RPCINIT	0

/** 
 *	Structure for remotely (over the SIF) changing the value of a software register 
 *	(SREG).There are 32 software registers (0 - 31). Registers 0-7 are used by the 
 *	system.
 */
typedef struct t_SifCmdSRegData {
	/** header data */
	SifCmdHeader_t	header;
	/** index of the software register */
	int		index;
	/** value to set the software register to */
	unsigned int	value;
} SifCmdSRegData_t;

#ifdef __cplusplus
extern "C"
#endif

/**
 *	Command handler callback function.
 *	@param 	data data the function should process
 *	@param 	harg handler arguments
 *	@see SifAddCmdHandler()
 */
typedef void (*SifCmdHandler_t)(void *data, void *harg);

/**
 *	Structure to organize current command handlers
 */
typedef struct t_SifCmdHandlerData
{
	/** handler funciton pointer */
	SifCmdHandler_t	handler;
	/** pointer to the handler's arguments */
	void	 	*harg;
} SifCmdHandlerData_t;

/**
 *	Apparently having 79 arguments "Triggers an IOP reboot".
 *	Don't know if thats true but maybe this is the maximum number of
 *	255 bit wide arguments the IOP can take for a reboot context... ?:|
 */
#define RESET_ARG_MAX	79

/**
 *	Stucture containing data about how the IOP should reset
 */
typedef struct _iop_reset_pkt {
	/** header data */
	struct t_SifCmdHeader header;
	/** length of the argument list */
	int		arglen;
	/** IOP reset mode bits */
	int		mode;
	/** arg list */
	char	arg[RESET_ARG_MAX + 1];
} SifCmdResetData_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 *	Send a raw packet over the SIF. (non-interrupt)
 *	@param 	cmd function number of the function to call
 *	@param 	packet pointer to the packet structure
 *	@param 	packet_size size of the packet structure
 *	@param 	src_extra source data?
 *	@param 	dest_extra destination data?
 *	@param 	size_extra if 0, only send a ping, any other size sent with data
 *	@return 0 if the packet_size > 255, otherwise the result of {@link SifSetDma(dmat, count)}
 *	@see iSifSendCmd()
 */
unsigned int SifSendCmd(int cmd, void *packet, int packet_size, void *src_extra, 
						void *dest_extra, int size_extra);

/**
 *	Send a raw packet over the SIF. (interrupt)
 *	@param 	cmd function number of the function to call
 *	@param 	packet pointer to the packet structure
 *	@param 	packet_size size of the packet structure
 *	@param 	src_extra source data?
 *	@param 	dest_extra destination data?
 *	@param 	size_extra if 0, only send a ping, any other size sent with data
 *	@return 0 if the packet_size > 255, otherwise the result of {@link iSifSetDma()}
 *	@see SifSendCmd()
 */
unsigned int iSifSendCmd(int cmd, void *packet, int packet_size, void *src_extra,
						 void *dest_extra, int size_extra);

/**
 *	Add a command handler for a unique function id (sid)
 *	@param 	pos sid of the funciton, which is the position of the function
 *			pointer in memory
 *	@param 	hander callback handler for the function
 *	@param 	harg arguments to send to the handler
 */
void SifAddCmdHandler(int pos, SifCmdHandler_t handler, void *harg);

/**
 *	Removes a command handler for a unique function id (sid)
 *	@param 	pos sid of the function, which is the poition of the function
 *			pointer in memory
 */
void SifRemoveCmdHandler(int pos);

/**
 *	Initializes the command handler organization scheme
 */
void SifInitCmd(void);

/**
 *	Shuts down the command handler
 */
void SifExitCmd(void);

/**
 *	Overwrite the current user command handlers for a pre-populated buffer
 *	of handlers
 *	@param 	db pre-populated buffer of user handlers
 *	@param 	size size of the buffer in bytes
 *	@return the old set of user command handlers
 */
SifCmdHandlerData_t *SifSetCmdBuffer(SifCmdHandlerData_t *db, int size);

/**
 *	Returns the value of the specified software register
 *	@param 	index register number
 *	@return contents of SREG[index]
 */
int SifGetSreg(int index);

/**
 *	Write the specified data to the data cache for Write Back, specific for
 *	SIF packet source data
 *	@param 	ptr pointer to data to be cached and later saved
 *	@param 	size size of the pointer
 */
void SifWriteBackDCache(void *ptr, int size);	// EE only

// Send mode bits
/** 
 *	Used with {@link SifSendCmd()} or {@link iSifSendCmd()}. "SIF Command Mode Interrupt":
 *	Mode for calling within an interrupt context.
 */
#define SIF_CMD_M_INTR	0x01

/** 
 *	Used with {@link SifSendCmd()} or {@link iSifSendCmd()}. 
 *	"SIF Command Mode Write Back Data Cache": Mode to write back D-cache for extended data
 */
#define SIF_CMD_M_WBDC	0x04

#ifdef __cplusplus
}
#endif

#endif /* __SIFCMD_H__ */
