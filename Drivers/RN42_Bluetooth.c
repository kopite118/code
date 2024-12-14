///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                       RN42_Bluetooth.c                            ////
////                                                                   ////
//// Library for configuring Bluetooth module and using it in SPP      ////
//// mode.                                                             ////
////                                                                   ////
//// See the Bluetooth_RN42.h for public API prototypes and            ////
//// documentation.                                                    ////
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

#ifndef __BTSPP_C__
#define __BTSPP_C__

#include <RN42_Bluetooth.h>

#if (STREAM_BT_SPP_UART==1)
	#define UARTBtInit    UART1Init
	#define UARTBtPutc    UART1Putc
	#define UARTBtGetc    UART1Getc
	#define UARTBtKbhit   UART1Kbhit
	#define UARTBtPuts    UART1Puts
	#define UARTBtPutsROM UART1PutsROM
	#include <UART1.c>
#elif (STREAM_BT_SPP_UART==2)
	#define UARTBtInit    UART2Init
	#define UARTBtPutc    UART2Putc
	#define UARTBtGetc    UART2Getc
	#define UARTBtKbhit   UART2Kbhit
	#define UARTBtPuts    UART2Puts
	#define UARTBtPutsROM UART2PutsROM
	#include <UART2.c>
#elif (STREAM_BT_SPP_UART==3)
	#define UARTBtInit    UART3Init
	#define UARTBtPutc    UART3Putc
	#define UARTBtGetc    UART3Getc
	#define UARTBtKbhit   UART3Kbhit
	#define UARTBtPuts    UART3Puts
	#define UARTBtPutsROM UART3PutsROM
	#include <UART3.c>
#elif (STREAM_BT_SPP_UART==4)
	#define UARTBtInit    UART4Init
	#define UARTBtPutc    UART4Putc
	#define UARTBtGetc    UART4Getc
	#define UARTBtKbhit   UART4Kbhit
	#define UARTBtPuts    UART4Puts
	#define UARTBtPutsROM UART4PutsROM
	#include <UART4.c>
#endif

#ifndef PIN_BT_SPP_RESET
   #define PIN_BT_SPP_RESET PIN_D0
#endif

// see Bluetooth_RN42.h for documentation
void BtSppInit(void)
{
	UARTBtInit(115200);
	
   output_low(PIN_BT_SPP_RESET);
   output_drive(PIN_BT_SPP_RESET);
   
  #if defined(PIN_BT_SPP_DISCOVERY)
   output_high(PIN_BT_SPP_DISCOVERY);
   output_drive(PIN_BT_SPP_DISCOVERY);
  #endif
  
   delay_ms(10);
   
   output_high(PIN_BT_SPP_RESET);
}

// see Bluetooth_RN42.h for documentation
inline bool BtSppKbhit(void)
{
   return(UARTBtKbhit());
}

// see Bluetooth_RN42.h for documentation
inline char BtSppGetc(void)
{
   return(UARTBtGetc());
}

// see Bluetooth_RN42.h for documentation
inline void BtSppPutc(char c)
{
	UARTBtPutc(c);
}

static void _BtSppCommandDelay(void)
{
   delay_ms(100);
}

// see Bluetooth_RN42.h for documentation
void BtSppCommandEnter(void)
{
   delay_ms(750);
	UARTBtPutsROM((rom char *)"$$$");
   _BtSppCommandDelay();
}

// see Bluetooth_RN42.h for documentation
void BtSppCommandSetName(char *str)
{
	UARTBtPutsROM((rom char *)"SN,");
	UARTBtPuts(str);
	UARTBtPutsROM((rom char *)"\r");
	
   _BtSppCommandDelay();
}

// see Bluetooth_RN42.h for documentation
void BtSppCommandSetNameROM(rom char *str)
{
	UARTBtPutsROM((rom char *)"SN,");
	UARTBtPutsROM(str);
	UARTBtPutsROM((rom char *)"\r");

   _BtSppCommandDelay();
}

// see Bluetooth_RN42.h for documentation
void BtSppCommandSetNameWithMAC(char *str)
{
   UARTBtPutsROM((rom char *)"S-,");
	UARTBtPuts(str);
	UARTBtPutsROM((rom char *)"\r");

	_BtSppCommandDelay();
}

// see Bluetooth_RN42.h for documentation
void BtSppCommandSetNameWithMACROM(rom char *str)
{
   UARTBtPutsROM((rom char *)"S-,");
	UARTBtPutsROM(str);
	UARTBtPutsROM((rom char *)"\r");

   _BtSppCommandDelay();
}


// see Bluetooth_RN42.h for documentation
void BtSppCommandExit(void)
{
   UARTBtPutsROM((rom char *)"---\r");
	
   _BtSppCommandDelay();
}

// see Bluetooth_RN42.h for documentation
#if defined(PIN_BT_SPP_CONNECTED)
inline bool BtSppIsConnected(void)
{
   return(input(PIN_BT_SPP_CONNECTED));
}
#endif

#endif
