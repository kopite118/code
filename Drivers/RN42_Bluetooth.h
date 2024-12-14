///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                       RN42_Bluetooth.h                            ////
////                                                                   ////
//// Library for configuring Bluetooth module and using it in SPP      ////
//// mode.                                                             ////
////                                                                   ////
//// This file only contains the public prototypes/definitions.        ////
//// Bluetooth_RN42.c contains the code.                               ////
////                                                                   ////
//// See the comments above the prototypes for documentation about     ////
//// API usage.                                                        ////
////                                                                   ////
//// Some useful GPIO pins that control the way RN42 operates:         ////
////  RESET = active-low reset. hold low for power-down.               ////
////     (default = pull-up).                                          ////
////  GPIO3 = auto discovery.  (high = auto discovery enabled.         //// 
////     default = pull-down).  If discovery isn't enabled, you can't  ////
////     initially pair with the device.                               ////
////  GPIO4 = set factory defaults (default = pull-down)               ////
////  GPIO6 = bt master mode.  (high = auto-master,                    ////
////     default = pull-down)                                          ////
////  GPIO7 = baud rate (high = 9600, low = 11500,                     ////
////     default = pull-down).                                         ////
////                                                                   ////
//// The following GPIO pins are output from the RN42 and can be       ////
//// used to control LEDs automatically:                               ////
////  GPIO2 = high when connected, low when not connected              ////
////  GPIO5 = led = toggles based on state, low on connect.            ////
////                                                                   ////
//// CONFIG                                                            ////
//// ----------------------------------------------------------------- ////
////                                                                   ////
//// The API has the following required and optional parameters that   ////
//// need to be defined before including the Bluetooth_RN42.c file.    ////
////                                                                   ////
//// STREAM_BT_SPP_UART                                                ////
////  User must define this, it is used to determine which UARTn.c     ////
////  library to use.  This configures the library for which hardware  ////
////  UART peripheral to use on the PIC.  Software UART cannot be      ////
////  used.                                                            ////
////     example:  #define STREAM_BT_SPP_UART 1                        ////
////                                                                   ////
//// STREAM_BT_SPP_BUFFER                                              ////
////  An optional parameter, if used defines the RX buffer size used   ////
////  by the serial stream's receive ISR.  If this is used, user is    ////
////  responsible for turning on the associated interrupt (and global  ////
////  enable).                                                         ////
////     example:  #define STREAM_BT_SPP_BUFFER 32                     ////
////                                                                   ////
//// PIN_BT_SPP_RESET                                                  ////
////  The PICmicro's GPIO pin tied to the RN42's reset pin.            ////
////                                                                   ////
//// PIN_BT_SPP_DISCOVERY                                              ////
////  An optional pin, if defined controlls the GPIO3 discovery        ////
////  pin on the RN42.  This pin has to be high to be disceverd by     ////
////  other devices for initial pairing.  The library will make        ////
////  it discoverable after power-up, use BTSPPDiscoverable() to       ////
////  change the setting.                                              ////
////                                                                   ////
//// PIN_BT_SPP_CONNECTED                                              ////
////  Optional, if defined specifies the PIC GPIO connected to the     ////
////  RN42's connection sense output pin GPIO2.  This pin is high      ////
////  when the RN42 is paired and connected to another SPP device.     ////
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

#ifndef __RN42_Bluetooth_H__
#define __RN42_Bluetooth_H__

#if defined(__XC8) || defined(__XC16) || defined(__XC32)
   #include <system.h>
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

// initialize the BT module to go into SPP mode.
// will also ping the RN42's RESET pin to cause RN42 to reset.
void BtSppInit(void);

// returns TRUE if there is data to be received in the receive buffer
inline bool BtSppKbhit(void);

inline char BtSppGetc(void);

// send a character over SPP
inline void BtSppPutc(char c);

// enter command mode (exit data mode).  command mode is needed for
// controlling/setting variables on the RN42 module.
//
// Warning: due to gaurd times, this takes over 800ms.
//
// Warning: the RN42 has a feature called 'fast data mode' that prevents
//    you from entering command mode 60 seconds after reset.
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

// returns TRUE if BT/SPP is connected to another device.
// requires PIN_BT_SPP_CONNECTED
inline bool BtSppIsConnected(void);

#endif
