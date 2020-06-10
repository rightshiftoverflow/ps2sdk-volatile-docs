/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# (C)2001, Gustavo Scotti (gustavo@scotti.com)
# (c) Marcus R. Brown (mrbrown@0xd6.org)
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
*/

/**
 * @file
 * EE SIF RPC commands prototypes
 */

#ifndef __SIFRPC_H__
#define __SIFRPC_H__

#include <sifcmd.h>

/* Modes for bind() and call() */
/** 
 *	Mode for {@link SifBindRpc()}, {@link SifCallRpc()}, and 
 *	{@link SifRpcGetOtherData()}. Don't wait for the end 
 *	function to complete.
 */
#define SIF_RPC_M_NOWAIT	0x01

/** 
 *	Mode for {@link SifBindRpc()}, {@link SifCallRpc()}, and 
 *	{@link SifRpcGetOtherData()}. Don't write back to the 
 *	data cache.
 */
#define SIF_RPC_M_NOWBDC	0x02

#ifdef __cplusplus
extern "C" {
#endif

typedef void * (*SifRpcFunc_t)(int fno, void *buffer, int length);
/**
 *	Used with {@link SifCallRpc()} as a completion handler
 */
typedef void (*SifRpcEndFunc_t)(void *end_param);

#ifdef __cplusplus
}
#endif

/**
 *	Generic packet header
 */
typedef struct t_SifRpcPktHeader {
	struct t_SifCmdHeader	sifcmd;
	int			rec_id;
	void		*pkt_addr;
	int			rpc_id;
} SifRpcPktHeader_t;

/**
 *	Render Packet?
 */
typedef struct t_SifRpcRendPkt
{
   struct t_SifCmdHeader	sifcmd;
   int				rec_id;		/* 04 */
   void				*pkt_addr;	/* 05 */
   int				rpc_id;		/* 06 */

   struct t_SifRpcClientData	*client;	/* 7 */
   u32                          cid;		/* 8 */
   struct t_SifRpcServerData	*server;	/* 9 */
   void				*buff,		/* 10 */
      				*cbuff;		/* 11 */
} SifRpcRendPkt_t;

/**
 *	Miscellaneous data packet for general purpose
 */
typedef struct t_SifRpcOtherDataPkt
{
   struct t_SifCmdHeader	sifcmd;
   int				rec_id;		/* 04 */
   void				*pkt_addr;	/* 05 */
   int				rpc_id;		/* 06 */

   struct t_SifRpcReceiveData	*receive;	/* 07 */
   void				*src;		/* 08 */
   void				*dest;		/* 09 */
   int				size;		/* 10 */
} SifRpcOtherDataPkt_t;

/**
 *	Bind packet.
 *	Created when calling SifBindRpc(). This is a 
 *	local struct and does not to to be created or
 *	accessed by the user.
 */
typedef struct t_SifRpcBindPkt
{
   struct t_SifCmdHeader	sifcmd;
   int				rec_id;		/* 04 */
   void				*pkt_addr;	/* 05 */
   int				rpc_id;		/* 06 */
   struct t_SifRpcClientData	*client;	/* 07 */
   int				sid;		/* 08 */
} SifRpcBindPkt_t;

/**
 *	Call packet.
 *	Created when calling SifCallRpc(). This is a 
 *	local struct and does not to to be created or
 *	accessed by the user.
 */
typedef struct t_SifRpcCallPkt
{
   struct t_SifCmdHeader	sifcmd;
   int				rec_id;		/* 04 */
   void				*pkt_addr;	/* 05 */
   int				rpc_id;		/* 06 */
   struct t_SifRpcClientData	*client;	/* 07 */
   int				rpc_number;	/* 08 */
   int				send_size;	/* 09 */
   void				*receive;	/* 10 */
   int				recv_size;	/* 11 */
   int				rmode;		/* 12 */
   struct t_SifRpcServerData	*server;	/* 13 */
} SifRpcCallPkt_t;

/**
 *	SIF server data, used to identify the server
 */
typedef struct t_SifRpcServerData
{
   int				sid;		/* 04	00 */

   SifRpcFunc_t			func;		/* 05	01 */
   void				*buff;		/* 06	02 */
   int				size;		/* 07	03 */

   SifRpcFunc_t			cfunc;		/* 08	04 */
   void				*cbuff;		/* 09	05 */
   int				size2;		/* 10	06 */

   struct t_SifRpcClientData	*client;	/* 11	07 */
   void				*pkt_addr;	/* 12	08 */
   int				rpc_number;	/* 13	09 */

   void				*receive;	/* 14	10 */
   int				rsize;		/* 15	11 */
   int				rmode;		/* 16	12 */
   int				rid;		/* 17	13 */

   struct t_SifRpcServerData	*link;		/* 18	14 */
   struct t_SifRpcServerData	*next;		/* 19	15 */
   struct t_SifRpcDataQueue	*base;		/* 20	16 */
} SifRpcServerData_t;


typedef struct t_SifRpcHeader
{
   void				*pkt_addr;	/* 04	00 */
   u32				rpc_id;		/* 05	01 */
   int				sema_id;	/* 06	02 */
   u32			 	mode;		/* 07	03 */
} SifRpcHeader_t;


typedef struct t_SifRpcClientData
{
   struct t_SifRpcHeader	hdr;
   u32				command;	/* 04 	08 */
   void				*buff, 		/* 05 	09 */
      				*cbuff;		/* 06 	10 */
   SifRpcEndFunc_t 		end_function;	/* 07 	11 */
   void				*end_param;	/* 08 	12*/
   struct t_SifRpcServerData	*server;	/* 09 	13 */
} SifRpcClientData_t;

typedef struct t_SifRpcReceiveData
{
   struct t_SifRpcHeader	hdr;
   void				*src,		/* 04 */
      				*dest;		/* 05 */
   int	                        size;		/* 06 */
} SifRpcReceiveData_t;

typedef struct t_SifRpcDataQueue
{
   int				thread_id,	/* 00 */
      				active;		/* 01 */
   struct t_SifRpcServerData	*link,		/* 02 */
      				*start,		/* 03 */
                                *end;		/* 04 */
   struct t_SifRpcDataQueue	*next;  	/* 05 */
} SifRpcDataQueue_t;

#ifdef __cplusplus
extern "C" {
#endif

/**	
 *	Begins interface between EE and IOP
 * 	@param mode operation mode for System Interface (currently unused, pass 0)
 */
void SifInitRpc(int mode);

/**
 *	Ends interface between EE and IOP
 */
void SifExitRpc(void);

/* SIF RPC client API */
/**
 *	<sub>SIF RPC client API</sub><br/>
 *	Bind/connect to a Remote Procedure Call. Used to create a relationship
 *	with the IOP for a specified purpose to use with {@link SifCallRpc()}.
 *	@param 	client a pointer the "client context" to create and return to
 *			the user.
 *	@param 	rpc_number the unique id (sid) to the Call to bind to. (TODO: parameter 
 *			should be renamed to "sid" to avoid confusion)
 *	@param 	mode mode of operation the packet should recieve. Current supported
 *			values are 0x0, {@link SIF_RPC_M_NOWAIT}, and {@link SIF_RPC_M_NOWBDC}.
 *	@return 0 if successful; -{@link E_SIF_PKT_ALLOC}, -{@link -E_SIF_PKT_SEND}, or 
 *			-{@link E_LIB_SEMA_CREATE} on error.
 */
int SifBindRpc(SifRpcClientData_t *client, int rpc_number, int mode);

/**
 *	<sub>SIF RPC client API</sub><br/>
 *	Call on a Remote Rocedure Call. Get information from the IOP.
 *	@param 	client pointer to "client context" created by {@link SifBindRpc()}.
 *	@param 	rpc_number special call number for the call(??).
 *	@param 	mode mode of operation the packet should recieve. Current supported
 *			values are 0x0, {@link SIF_RPC_M_NOWAIT}, and {@link SIF_RPC_M_NOWBDC}.
 *	@param 	send pointer to the data to send with the call
 *	@param 	ssize send size, or the size of the data that will be sent
 *	@param 	receive the pointer of the data to set as a result of the call 
 *			(TODO: spelling error! i before e people!)
 *	@param 	rsize recieve size, or the size of the data that will be mutated
 *	@param 	end_function {@link SifRpcEndFunc_t} called when Call is completed
 *	@param 	end_param parameter sent to the completion function
 *	@return 0 if successful; -{@link E_SIF_PKT_ALLOC}, -{@link E_SIF_PKT_SEND}, or
 *			-{@link E_LIB_SEMA_CREATE} on error.
 */
int SifCallRpc(SifRpcClientData_t *client, int rpc_number, int mode, void *send, 
			   int ssize, void *receive, int rsize, SifRpcEndFunc_t end_function, 
			   void *end_param);

/**
 *	<sub>SIF RPC client API</sub><br/>
 *	Generic call to the IOP for data.
 *	@param 	rd pointer to a {@link SifRpcReceiveData} that contains
 *			the information for the call
 *	@param 	src unused. Use rd->src instead. (TODO: remove unused argument)
 *	@param 	dest unused. Use rd->dest instead. (TODO: remove unused argument)
 *	@param 	size unused. Use rd->size instead. (TODO: remove unused argument)
 *	@param 	mode mode of operation the packet should recieve. Current supported
 *			values are 0x0, {@link SIF_RPC_M_NOWAIT}, and {@link SIF_RPC_M_NOWBDC}.
 *	@return 0 if successful; -{@link E_SIF_PKT_ALLOC}, -{@link E_SIF_PKT_SEND}, or 
 *			-{@link E_LIB_SEMA_CREATE} on error.
 */
int SifRpcGetOtherData(SifRpcReceiveData_t *rd, void *src, void *dest, int size, 
					   int mode);

/**
 *	<sub>SIF RPC client API</sub><br/>
 *	Return header status of a {@link SifRpcClientData_t}
 *	@param 	cd pointer to a "client context"
 *	@return 1 if it checks out, 0 if the header is null, the header's rpc_id is not 
 *	identical to the header packets's rpc_id, or if the header isn't allocated.
 */
int SifCheckStatRpc(SifRpcClientData_t *cd);

/* SIF RPC server API */
/**
 *	<sub>SIF RPC server API</sub><br/>
 *	Add an action to the Remote Procedure Call Queue.
 *	@param 	q pointer to data that specifies the queue information
 *	@param 	thread_id id to set to the data's thread_id. q->thread_id = thread_id.
 *			(TODO: kinda useless argument... let the user do this manually)
 *	@return 
 */
SifRpcDataQueue_t *SifSetRpcQueue(SifRpcDataQueue_t *q, int thread_id);

SifRpcDataQueue_t *SifRemoveRpcQueue(SifRpcDataQueue_t *qd);

SifRpcServerData_t *SifRegisterRpc(SifRpcServerData_t *srv, int sid, SifRpcFunc_t func, 
								   void *buff, SifRpcFunc_t cfunc, void *cbuff, 
								   SifRpcDataQueue_t *qd);

SifRpcServerData_t *SifRemoveRpc(SifRpcServerData_t *sd, SifRpcDataQueue_t *queue);

SifRpcServerData_t *SifGetNextRequest(SifRpcDataQueue_t *qd);

void SifExecRequest(SifRpcServerData_t *srv);
void SifRpcLoop(SifRpcDataQueue_t *q);

#ifdef __cplusplus
}
#endif

#endif /* __SIFRPC_H__ */
