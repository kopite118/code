///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                  RN4020_MLDP_Bluetooth.h                          ////
////                                                                   ////
//// Library for configuring Bluetooth module and using it in MLDP     ////
//// mode.  This library was meant to be compatible with the SPP       ////
//// Bluetooth driver, which is why the function names are called      ////
//// SPP instead of MLDP.  MLDP is Microchip's custom use of           ////
//// BLE GATT that operates just like SPP.                             ////
////                                                                   ////
//// This file only contains the public prototypes/definitions.        ////
//// RN4020_MLDP_Bluetooth.c contains the code.                        ////
////                                                                   ////
//// See the comments above the prototypes for documentation about     ////
//// API usage.                                                        ////
////                                                                   ////
//// Some useful IO pins that control the way RN4020 operates:         ////
////  #7 - WAKE_SW - When pulled high will wake unit from deep sleep   ////
////     Has a weak pull-down.                                         ////
////  #8 - CMD/MLDP - When high unit is in MLDP mode, when low unit    ////
////     is in command mode.                                           ////
////  #15 - WAKE_HW - Hardware wake from dormant state.  Has           ////
////     internal pull-down.                                           ////
////                                                                   ////
////  #18 - RTS - UART flow control pin (to RN4020).                   ////
//// The following GPIO pins are output from the RN4020 and can be     ////
//// used to control LEDs automatically:                               ////
////  #10 - PIO1 - high when connected, low when not connected.        ////
////  #11 - PIO2 - Event status.                                       ////
////  #12 - PIO3 - high when unit is awake.                            ////
////  #14 - CTS - UART flow control pin (from RN4020).                 ////
////                                                                   ////
//// CONFIG                                                            ////
//// ----------------------------------------------------------------- ////
////                                                                   ////
//// The API has the following required and optional parameters that   ////
//// need to be defined before including the Bluetooth_RN4020.c file.  ////
////                                                                   ////
//// STREAM_BT_SPP_UART                                                ////
////  User must define this, it is passed to the #use rs232() when     ////
////  creating the serial stream.  An examples (UART1):                ////
////     #define STREAM_BT_SPP 1                                       ////
////                                                                   ////
//// STREAM_BT_SPP_BUFFER                                              ////
////  An optional parameter, if used defines the RX buffer size used   ////
////  by the serial stream's receive ISR.  If this is used, user is    ////
////  responsible for turning on the associated interrupt (and global  ////
////  enable).                                                         ////
////     example:  #define STREAM_BT_SPP_BUFFER 32                     ////
////                                                                   ////
//// PIN_BT_MLDP_CMD_MODE                                              ////
////  User must define this, it is the pin connected to the CMD/MLDP   ////
////  pin of the RN4020.                                               ////
////                                                                   ////
//// PIN_BT_MLDP_CONNECTED                                             ////
////  Optional, if defined specifies the PIC GPIO connected to the     ////
////  RN4020's connection sense output pin PIO3.  This pin is high     ////
////  when the RN4020 is paired and connected to another MLDP device.  ////
////                                                                   ////
//// PIN_BT_SWAKE                                                      ////
////  Optional, if defined the init function will set this high.       ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2015 Custom Computer Services           ////
////                                                                   ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler or to users who purchased the rights to use this code.   ////
//// This source code may only be distributed to other licensed users  ////
//// of the CCS C compiler or other users who purchased the rights to  ////
//// this code.  The rights to use this code can only be granted       ////
//// by CCS.  No other use, reproduction or distribution is permitted  ////
//// without written permission.  Derivative programs created using    ////
//// this software in object code form are not restricted in any way.  ////
////                                                                   ////
////                     http://www.ccsinfo.com                        ////
///////////////////////////////////////////////////////////////////////////

#ifndef __RN4020_MLDP_Bluetooth_H__
#define __RN4020_MLDP_Bluetooth_H__

#if defined(__XC__)
   //microchip XC Compiler (XC8, XC16 or XC32)

   #include <system.h>  //common .h file for all units
#endif

#include <stdbool.h>


#ifndef STREAM_BT_SPP_UART
	#define STREAM_BT_SPP_UART 1
#endif

#if defined(STREAM_BT_SPP_BUFFER)
   #if (STREAM_BT_SPP_UART==1)
      #define UART1_RX_BUFFER_SIZE STREAM_BT_SPP_BUFFER
      #include <UART1.h>
   #elif (STREAM_BT_SPP_UART==2)
      #define UART2_RX_BUFFER_SIZE STREAM_BT_SPP_BUFFER
      #include <UART2.h>
   #elif (STREAM_BT_SPP_UART==3)
      #define UART3_RX_BUFFER_SIZE STREAM_BT_SPP_BUFFER
      #include <UART3.h>
   #elif (STREAM_BT_SPP_UART==4)
      #define UART4_RX_BUFFER_SIZE STREAM_BT_SPP_BUFFER
      #include <UART4.h>
   #else
      #error User chose an invalid UART number
   #endif
#endif

// initialize the BT module to go into MLDP mode.
// this puts the unit into command mode first to intialize the unit into
// the correct mode, therefore this function could take over 1s.
void BtSppInit(void);

// returns TRUE if there is data to be received in the receive buffer
inline bool BtSppKbhit(void);

inline char BtSppGetc(void);

// send a character over MLDP
inline void BtSppPutc(char c);

// enter command mode (exit data mode).  command mode is needed for
// controlling/setting variables on the RN4020 module.
//
// Warning: due to gaurd times, this takes over 800ms.
void BtSppCommandEnter(void);

// set the name of the module, this is what will be displayed when you scan
// for devices.
// you must be in command mode (see BtSppCommandEnter())
void BtSppCommandSetName(char *str);
void BtSppCommandSetNameROM(rom char *str);

// set the name of the module, this is what will be displayed when you scan
// for devices.
// this version will append the last 4 bytes of the Bluetooth's MAC address
// to the end of the name, automatically.
// you must be in command mode (see BtSppCommandEnter())
void BtSppCommandSetNameWithMAC(char *str);
void BtSppCommandSetNameWithMACROM(rom char *str);

// exit the command mode entered by BtSppCommandEnter()
void BtSppCommandExit(void);

// returns TRUE if BT/MLDP is connected to another device.
// requires PIN_BT_MLDP_CONNECTED
inline bool BtSppIsConnected(void);

#endif   //__RN4020_MLDP_Bluetooth_H__
