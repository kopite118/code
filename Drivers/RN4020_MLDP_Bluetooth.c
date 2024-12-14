///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                    RN4020_MLDP_Bluetooth.c                        ////
////                                                                   ////
//// Library for configuring Bluetooth module and using it in MLDP     ////
//// mode.                                                             ////
////                                                                   ////
//// See the RN4020_MLDP_Bluetooth.h for public API prototypes and     ////
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

#include <RN4020_MLDP_Bluetooth.h>

#ifndef PIN_BT_MLDP_CMD_MODE
   #define PIN_BT_MLDP_CMD_MODE PIN_D0
#endif

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

#if !defined(__DO_DEBUG_MLDP)
   #define debug_mldp0(a)
   #define debug_mldp1(a,b)
#else
   #define debug_mldp0(a)                debug_printf0(a)
   #define debug_mldp1(a,b)              debug_printf1(a,b)
#endif

rom char _Rn4020CmdStr[] = "CMD\r\n";
rom char _Rn4020AokStr[] = "AOK\r\n";

// returns TRUE if found
static bool _Rn4020WaitForString(rom char * romExpectString, uint16_t timeoutMs)
{
   rom char * pCurr;
   char c;
   
   pCurr = romExpectString;
   
   while(timeoutMs--)
   {
      while (BtSppKbhit())
      {
         c = BtSppGetc();

         if (c != *pCurr)
         {
            pCurr = romExpectString;
         }
         else
         {
            pCurr++;
            if (*pCurr == 0)
            {
               debug_mldp0("RN4020 WAIT: OK\r\n");
               return(true);
            }
         }
      }

      delay_ms(1);
   }

   debug_mldp0("RN4020 WAIT: ERROR\r\n");
 
   return(false);
}

static void _BtSppSwitchDelay(void)
{
   delay_ms(500);
}

// see RN4020_MLDP_Bluetooth.h for documentation
void BtSppInit(void)
{
   bool ok;
   int tries=0;
   
   output_low(PIN_BT_MLDP_CMD_MODE);
   output_drive(PIN_BT_MLDP_CMD_MODE);

   output_low(PIN_BT_SWAKE);
   output_drive(PIN_BT_SWAKE);

   delay_ms(100);

   UARTBtInit(115200);
  
   output_high(PIN_BT_SWAKE);

   debug_mldp0("BtSppInit() wait for cmd...\r\n");
  
   ok = _Rn4020WaitForString(_Rn4020CmdStr, 2000);
   
   ///// enable mldp, make unit discoverable.
   ///// try it twice
   if (ok)
   {
      while (tries < 2)
      {
         debug_mldp0("BtSppInit() send SR\r\n");
         
         UARTBtPutsROM((rom char *)"SR,3a004c00\r\n");
         //UARTBtPutsROM((rom char *)"SR,32004c00\r\n");
         
         ok = _Rn4020WaitForString(_Rn4020AokStr, 500);
         
         if (ok)
            break;
      }
   }
   
   ///// iOS required timing
   if (ok)
   {
      debug_mldp0("BtSppInit() send ST\r\n");
      
      UARTBtPutsROM((rom char *)"ST,0010,0000,0064\r\n");
   
      ok = _Rn4020WaitForString(_Rn4020AokStr, 500);
   }
  
   ////// reset unit, so the SR command takes effect.
   if (ok)
   {
      debug_mldp0("BtSppInit() send R\r\n");
      
      UARTBtPutsROM((rom char *)"R,1\r\n");
   
      ok = _Rn4020WaitForString(_Rn4020CmdStr, 2000);
   }
   
   output_high(PIN_BT_MLDP_CMD_MODE);
}

// see RN4020_MLDP_Bluetooth.h for documentation
inline bool BtSppKbhit(void)
{
   return(UARTBtKbhit());
}

// see RN4020_MLDP_Bluetooth.h for documentation
inline char BtSppGetc(void)
{
   return(UARTBtGetc());
}

// see RN4020_MLDP_Bluetooth.h for documentation
inline void BtSppPutc(char c)
{
   UARTBtPutc(c);
}

// see RN4020_MLDP_Bluetooth.h for documentation
void BtSppCommandEnter(void)
{
   output_low(PIN_BT_MLDP_CMD_MODE);
   output_drive(PIN_BT_MLDP_CMD_MODE);
   _BtSppSwitchDelay();
}

// see RN4020_MLDP_Bluetooth.h for documentation
void BtSppCommandExit(void)
{
   output_high(PIN_BT_MLDP_CMD_MODE);
   output_drive(PIN_BT_MLDP_CMD_MODE);
   _BtSppSwitchDelay();
}

// see RN4020_MLDP_Bluetooth.h for documentation
void BtSppCommandSetName(char *str)
{
   UARTBtPutsROM((rom char *)"SN,");
   UARTBtPuts(str);
   UARTBtPutsROM((rom char *)"\r");
   
   _Rn4020WaitForString(_Rn4020AokStr, 500);
}

// see RN4020_MLDP_Bluetooth.h for documentation
void BtSppCommandSetNameROM(rom char *str)
{
   UARTBtPutsROM((rom char *)"SN,");
   UARTBtPutsROM(str);
   UARTBtPutsROM((rom char *)"\r");
   
   _Rn4020WaitForString(_Rn4020AokStr, 500);
}

// see RN4020_MLDP_Bluetooth.h for documentation
void BtSppCommandSetNameWithMAC(char *str)
{
   UARTBtPutsROM((rom char *)"S-,");
   UARTBtPuts(str);
   UARTBtPutsROM((rom char *)"\r");
   
   _Rn4020WaitForString(_Rn4020AokStr, 500);
}

// see RN4020_MLDP_Bluetooth.h for documentation
void BtSppCommandSetNameWithMACROM(rom char *str)
{
   UARTBtPutsROM((rom char *)"S-,");
   UARTBtPutsROM(str);
   UARTBtPutsROM((rom char *)"\r");
   
   _Rn4020WaitForString(_Rn4020AokStr, 500);
}

#if defined(PIN_BT_SPP_CONNECTED) && !defined(PIN_BT_MLDP_CONNECTED)
   // original driver and examples would use the pin label 
   // PIN_BT_SPP_CONNECTED, but we later switched it to PIN_BT_MLDP_CONNECTED.
   // this check and define will keep old code backwards compatible with
   // the updated library.
   #define PIN_BT_MLDP_CONNECTED PIN_BT_SPP_CONNECTED
#endif

// see RN4020_MLDP_Bluetooth.h for documentation
#if defined(PIN_BT_MLDP_CONNECTED)
inline bool BtSppIsConnected(void)
{
   return(input(PIN_BT_MLDP_CONNECTED));
}
#endif

#endif
