/*****************************************************************************
   Bluetooth_Serial.c
   
   Chooses the proper Bluetooth Serial/SPP/MLDP driver based upon which H file
   was included in the system.h.
*****************************************************************************/
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

#ifndef __BLUETOOTH_SERIAL_C__
#define __BLUETOOTH_SERIAL_C__

#if defined(__XC__)
   //microchip XC Compiler (XC8, XC16 or XC32)

   #include <system.h>  //common .h file for all units
#endif

#if defined(__RN4020_MLDP_Bluetooth_H__)
   #include <RN4020_MLDP_Bluetooth.c>
#elif defined(__RN42_Bluetooth_H__)
   #include <RN42_Bluetooth.c>
#else
   #error Library can not tell which driver to use
#endif

#endif //__BLUETOOTH_SERIAL_C__
