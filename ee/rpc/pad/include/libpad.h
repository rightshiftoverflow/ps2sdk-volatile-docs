/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2004, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
*/

/**
 *	@file
 *	Interface with GamePad externals. See the pad.c sample for more
 *	information and usages.
 *	TODO: maybe change error returns to 0 on successful, and non-zero
 *	on unsuccessful. This makes much more sense as many other
 *	programs return 0 on success and an error code (non-zero) on
 *	unsuccessful.
 */

#ifndef __LIBPAD_H__
#define __LIBPAD_H__

/*
 * Button bits
 */
#ifndef __INPUT_H__
/** Controller Button D-Pad Left */
#define PAD_LEFT      0x0080
/** Controller Button D-Pad Down */
#define PAD_DOWN      0x0040
/** Controller Button D-Pad Right */
#define PAD_RIGHT     0x0020
/** Controller Button D-Pad Up */
#define PAD_UP        0x0010
/** Controller Button Start */
#define PAD_START     0x0008
/** Controller Button R3 (right stick pushed down) */
#define PAD_R3        0x0004
/** Controller Button L3 (left stick pushed down) */
#define PAD_L3        0x0002
/** Controller Button Select */
#define PAD_SELECT    0x0001
/** Controller Button Square */
#define PAD_SQUARE    0x8000
/** Controller Button Cross (I know everyone 
 *	calls it the X but we have to be professional here) */
#define PAD_CROSS     0x4000
/** Controller Button Circle */
#define PAD_CIRCLE    0x2000
/** Controller Button Triangle */
#define PAD_TRIANGLE  0x1000
/** Controller Button R1 (upper right bumper) */
#define PAD_R1        0x0800
/** Controller Button L1 (upper left bumper) */
#define PAD_L1        0x0400
/** Controller Button R2 (lower right bumper) */
#define PAD_R2        0x0200
/** Controller Button L2 (lower left bumper) */
#define PAD_L2        0x0100
#endif

/*
 * Pad states
 */
/** GamePad is disconnected */
#define PAD_STATE_DISCONN       0x00
/** Currently trying to connect to the pad? */
#define PAD_STATE_FINDPAD       0x01
/** ??? */
#define PAD_STATE_FINDCTP1      0x02
/** GamePad is busy executing a command */
#define PAD_STATE_EXECCMD       0x05
/** GamePad is connected and responding */
#define PAD_STATE_STABLE        0x06
/** An error has occured with the GamePad */
#define PAD_STATE_ERROR         0x07

/*
 * Pad request states
 */
/** Pad state indicating that the pad request completed successfully */
#define PAD_RSTAT_COMPLETE      0x00
/** Pad state indicating that the pad request failed to complete */
#define PAD_RSTAT_FAILED        0x01
/** Pad state indicating that the pad request is still processing */
#define PAD_RSTAT_BUSY          0x02

/*
 * Connected pad type
 */
/** Namco NeGcon Controller {@link https://en.wikipedia.org/wiki/NeGcon} */
#define PAD_TYPE_NEJICON    0x2
/** Konami Justifier Controller
 *	{@link https://en.wikipedia.org/wiki/Konami_Justifier} */
#define PAD_TYPE_KONAMIGUN  0x3
/** Generic Digital Controller */
#define PAD_TYPE_DIGITAL    0x4
/** Generic Analog Controller */
#define PAD_TYPE_ANALOG     0x5
/** Namco Guncon Controller {@link https://en.wikipedia.org/wiki/GunCon} */
#define PAD_TYPE_NAMCOGUN   0x6
/** SONY DUALSHOCK [1, 2, 3] Controller
 *	{@link https://en.wikipedia.org/wiki/DualShock} */
#define PAD_TYPE_DUALSHOCK  0x7
/**	Namco Jogcon Controller {@link https://en.wikipedia.org/wiki/Jogcon} */
#define PAD_TYPE_JOGCON     0xE
/** I can't find information on this! Maybe Tsuri Fishing 
 *	controller for N64? */
#define PAD_TYPE_EX_TSURICON 0x100
/** Maybe a newer version of the Jogcon? */
#define PAD_TYPE_EX_JOGCON  0x300

/*
 * padInfoMode values
 */
/** Pad Info Mode used with {@link padInfoMode()}. Info Mode Current Mode 
 *	Identifier. The current mode being used by the pad. */
#define PAD_MODECURID   1
/** Pad Info Mode used with {@link padInfoMode()}. Info Mode Current Extended 
 *	ID? Gets extended attributes about the pad. */
#define PAD_MODECUREXID 2
/** Pad Info Mode used with {@link padInfoMode()}. Info Mode Current Offsets? */
#define PAD_MODECUROFFS 3
/** Pad Info Mode used with {@link padInfoMode()}. Info Mode Table. Use this
 *	info mode with the index argument. Index -1 returns the count of info
 *	available, any other positive integer returns the value at the specified
 *	index on the table. */
#define PAD_MODETABLE   4

/*
 * padSetMainMode
 */
/** Diable DUALSHOCK and Analog functionality */
#define PAD_MMODE_DIGITAL   0
/** Enable DUALSHOCK and Analog funcitonality */
#define PAD_MMODE_DUALSHOCK 1
/** Allow the user to enable/disable DUALSHOCK funcitonality through the Analog 
 *	button */
#define PAD_MMODE_UNLOCK    2
/** Don't allow the user to change the controler's main mode */
#define PAD_MMODE_LOCK      3

/*
 * padInfoAct cmds
 */
#define PAD_ACTFUNC     1
#define PAD_ACTSUB      2
#define PAD_ACTSIZE     3
#define PAD_ACTCURR     4

/** Button info */
struct padButtonStatus
{
    unsigned char ok;
    unsigned char mode;
    unsigned short btns;
    // joysticks
    unsigned char rjoy_h;
    unsigned char rjoy_v;
    unsigned char ljoy_h;
    unsigned char ljoy_v;
    // pressure mode
    unsigned char right_p;
    unsigned char left_p;
    unsigned char up_p;
    unsigned char down_p;
    unsigned char triangle_p;
    unsigned char circle_p;
    unsigned char cross_p;
    unsigned char square_p;
    unsigned char l1_p;
    unsigned char r1_p;
    unsigned char l2_p;
    unsigned char r2_p;
    unsigned char unkn16[12];
} __attribute__((packed));

#ifdef __cplusplus
extern "C" {
#endif

/**
 *	Initialise libpad
 *	@param mode Unused, pass 0
 *	@return 0 if the pad has already been initialized, -1 if the pad 1 
 *	RPC binding failed, or -3 if the pad 2 RPC binding failed. 1 on successful.
 */
int padInit(int mode);

/**
 *	Initialise pad ports. Called by padInit(), there is no need for the
 *	user to call this function directly.
 *	@param mode Unused, pass 0
 *	@return 0 on error (only when compiling with xpad), 1 on successful.
 *
 *	Note: PADMAN from release 1.3.4 does not have this function implemented.
 * 	As a result, it is impossible to reinitialize libpad after calling padEnd().
 */
int padPortInit(int mode);

/** 
 *	Ends all pad communication. Note: PADMAN from release 1.3.4 does not have 
 *	padPortInit implemented. As a result, it is impossible to reinitialize
 *	libpad after calling padEnd(). This was known as padClose in the really 
 *	early official SDK releases.
 *	@return 1 on successful, anything else is an error
 */
int padEnd(void);

/**
 *	Open a port to a controller for the IOP to read.
 *	@param 	port port on which the slot that connects the controller is 
 *			on, usually 0
 *	@param 	slot slot of the controller, usually 0 or 1
 *	@param 	padArea the address of the buffer for storing the pad status 
 *			(user provided, library defined). Must be a 256-byte region 
 *			(2xsizeof(struct pad_data). Must be a 64-byte aligned address.
 *			For the old libpad, at least 16-bytes alignment.
 *	@return 0 on unsucessful, non-zero on successful
 */
int padPortOpen(int port, int slot, void *padArea);

/**
 *	Closes an opened controller port
 *	@param 	port port on which the slot that connects the controller is 
 *			on, usually 0
 *	@param 	slot slot of the controller, usually 0 or 1
 *	@return 0 on unsuccessful, non-zero on successful
 */
int padPortClose(int port, int slot);

/**
 *	Read pad data
 *	@param 	port port on which the slot that connects the controller is 
 *			on, usually 0
 *	@param 	slot slot of the controller, usually 0 or 1
 *	@param 	data pointer to a 32 byte array where the result is stored
 *	@return 0 on unsuccessful, non-zero on successful
 */
unsigned char padRead(int port, int slot, struct padButtonStatus *data);

/**
 *	Get current pad state. Wait until state == {@link PAD_STATE_STABLE}
 *	before trying to access the pad.
 *	@param 	port port on which the slot that connects the controller is 
 *			on, usually 0
 *	@param 	slot slot of the controller, usually 0 or 1
 *	@return state of the controller. Could be {@link PAD_STATE_STABLE},
 *			{@link PAD_STATE_DISCONN}, or {@link PAD_STATE_ERROR}
 */
int padGetState(int port, int slot);

/**
 *	Get the status of the pad request state
 *	@param 	port port on which the slot that connects the controller is 
 *			on, usually 0
 *	@param 	slot slot of the controller, usually 0 or 1
 *	@return the pad request state. Could be {@link PAD_RSTAT_COMPLETE} or
 *			{@link PAD_RSTAT_BUSY}
 */
unsigned char padGetReqState(int port, int slot);

/**
 *	Set pad request state (after a param setting)
 *	No need to export this one perhaps..
 *	@param 	port port on which the slot that connects the controller is 
 *			on, usually 0
 *	@param 	slot slot of the controller, usually 0 or 1
 *	@param 	state state to set the request as. Could be
 *			{@link PAD_RSTAT_COMPLETE}
 *	@return 1 every time
 */
int padSetReqState(int port, int slot, int state);

/*
 * Debug print functions
 */
/**
 *	Useful debug function to convert a pad state (not to be confused with
 *	pad request state) to string
 *	@param 	state the pad state, not pad request state. Could be
 *			{@link PAD_STATE_DISCONN} or {@link PAD_STATE_STABLE}
 *	@param 	buf the 16 wide string to put the string state in
 */
void padStateInt2String(int state, char buf[16]);

/**
 *	Useful debug function to convert a pad request state (not to be
 *	confused with pad state) to string
 *	@param 	state the pad request state, not pad state. Could be
 *			{@link PAD_RSTAT_COMPLETE} or {@link PAD_RSTAT_BUSY}
 *	@param 	buf the 16 wide string to put the string state in
 */
void padReqStateInt2String(int state, char buf[16]);


/**
 *	Returns the count of slots on the PS2. In the current version 
 *	of PADMAN (2017-10-14), always returns 2.
 */
int padGetPortMax(void);

/**
 *	Returns the count of slots the specified port has (usually 1)
 *	probably 4 if using a multi tap (not tested)
 *	@param 	port port on which the slots are located, usually 0
 *	@return count of the slots on the specified port
 */
int padGetSlotMax(int port);

/**
 *	Returns the PADMAN.IRX version. NOT SUPPORTED on module rom0:padman
 *	@return integer version of PADMAN.IRX
 */
int padGetModVersion();

/** 
 *	Get pad info/type. See the pad.c sample for more usage and information.
 *	@param 	port port on which the slot that connects the controller is 
 *			on, usually 0
 *	@param 	slot slot of the controller, usually 0 or 1
 *	@param 	infoMode what information to get. Valid modes are
 *			{@link PAD_MODECURID}, {@link PAD_MODECUREXID}, 
 *			{@link PAD_MODECUROFFS}, and {@link PAD_MODETABLE}.
 *	@param 	index only used when specifying the {@link PAD_MODETABLE} info
 *			mode. The index on the pad's info table of the desired information.
 *	@return the requested information about the controller. For controller types,
 *			possible values are {@link PAD_TYPE_DIGITAL} or
 *			{@link PAD_TYPE_DUALSHOCK}
 */
int padInfoMode(int port, int slot, int infoMode, int index);

/**
 *	Set the mode of a mode changeable controller, such as DUALSHOCK 1 or 2
 *	@param 	port port on which the slot that connects the controller is 
 *			on, usually 0
 *	@param 	slot slot of the controller, usually 0 or 1
 *	@param 	mode set to {@link PAD_MMODE_DUALSHOCK} to enable Analog and
 *			DUALSHOCK functionality, set set to {@link PAD_MMODE_DIGITAL}
 *			to change the controller into Digital mode.
 *	@param 	lock set to {@link PAD_MMODE_LOCK} to prevent the user from
 *			changing the mode through the Analog button on the controller.
 *			Set to {@link PAD_MMODE_UNLOCK} to reenable user preference.
 *	@return 0 on unsuccessful, non-zero on successful
 */
int padSetMainMode(int port, int slot, int mode, int lock);

/**
 *	Check if the pad has pressure sensitive buttons
 *	@param 	port port on which the slot that connects the controller is 
 *			on, usually 0
 *	@param 	slot slot of the controller, usually 0 or 1
 *	@return 0 if not supported, 1 otherwise
 */
int padInfoPressMode(int port, int slot);

/**
 *	Turn pressure sensitive mode ON
 *	@param 	port port on which the slot that connects the controller is 
 *			on, usually 0
 *	@param 	slot slot of the controller, usually 0 or 1
 *	@return 0 if unsuccessful, non-zero if successful (??)
 */
int padEnterPressMode(int port, int slot);

/** 
 *	Pressure sensitive mode OFF (Check for newer version)
 *	@param 	port port on which the slot that connects the controller is 
 *			on, usually 0
 *	@param 	slot slot of the controller, usually 0 or 1
 *	@return 0 if unsuccessful, non-zero if successful (??)
 */
int padExitPressMode(int port, int slot);

/*
 * Dunno if these need to be exported
 */
/**
 *	Get the button mask of a specified controller
 *	@param 	port port on which the slot that connects the controller is 
 *			on, usually 0
 *	@param 	slot slot of the controller, usually 0 or 1
 *	@return 0 for unsuccessful, non-zero for successful (??)
 */
int padGetButtonMask(int port, int slot);

/**
 *	Get the button mask of a specified controller
 *	@param 	port port on which the slot that connects the controller is 
 *			on, usually 0
 *	@param 	slot slot of the controller, usually 0 or 1
 *	@return 0 for unsuccessful, non-zero for successful (??)
 */
int padSetButtonInfo(int port, int slot, int buttonInfo);


/**
 *	Get actuator status for this controller. If
 *	padInfoAct(port, slot, -1, 0) != 0, the controller has actuators (i think ;) )
 *	@param 	port port on which the slot that connects the controller is 
 *			on, usually 0
 *	@param 	slot slot of the controller, usually 0 or 1
 *	@return info on the specified controller's rumble actuators
 */
unsigned char padInfoAct(int port, int slot, int word, int byte);

/**
 *	Initalize actuators. On dual shock controller:
 *	- set act_align[0] to 0 to enables 'small' engine
 *	- set act_align[1] to 1 to enables 'big' engine
 *	- set act_align[2-5] to 0xff to disable
 *
 *	@param 	port port on which the slot that connects the controller is 
 *			on, usually 0
 *	@param 	slot slot of the controller, usually 0 or 1
 *	@param 	act_align the "alignment" of the rumble motors. see above.
 *	@return 0 for unsuccessful, non-zero for successful
 */
int padSetActAlign(int port, int slot, char act_align[6]);

/**
 *	Set actuator status on dual shock controller:
 *	- act_align[0] = 0/1 turns off/on 'small' engine
 *	- act_align[1] = 0-255 sets 'big' engine speed
 *
 *	@param 	port port on which the slot that connects the controller is 
 *			on, usually 0
 *	@param 	slot slot of the controller, usually 0 or 1
 *	@param 	act_align the "direction" of the rumble motors. see above.
 *	@return 0 for unsuccessful, non-zero for successful
 */
int padSetActDirect(int port, int slot, char act_align[6]);

/** 
 *	Returns whether the device at port,slot is connected (1 = connected)
 *	Appears to have been removed very early during the PS2's lifetime.
 *	If possible, use the documented padGetState instead. NOT SUPPORTED
 *	with module rom0:padman
 *	@param 	slot slot of the controller, usually 0 or 1
 *	@param 	act_align the "direction" of the rumble motors. see above.
 *	@return 0 for unconnected, 1 for connected
 */
int padGetConnection(int port, int slot);

#ifdef __cplusplus
}
#endif

#endif /* __LIBPAD_H__ */
