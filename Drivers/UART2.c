/*
   UART2.c
   
   Library for sending/receiving data on the PIC's UART 2 peripheral.
   
   See UART2.h for documentation
*/
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

#ifndef __UART2_C__
#define __UART2_C__

#include <UART2.h>

#use rs232(UART2, stream=STREAM_UART_UART2, noinit, RECEIVE_BUFFER=UART2_RX_BUFFER_SIZE, errors)

#if (UART2_RX_BUFFER_SIZE > 0)
static void _UART2PhyRxIsrEnable(bool enable)
{
   if (enable)
   {
      enable_interrupts(INT_RDA2);
   }
   else
   {
      disable_interrupts(INT_RDA2);
   }
}
#endif

void UART2Init(uint32_t baud)
{
  #if (UART2_RX_BUFFER_SIZE > 0)
   _UART2PhyRxIsrEnable(false);
  #endif
  
   if (baud==300)  setup_uart(300, STREAM_UART_UART2);
   else if (baud==1200)  setup_uart(1200, STREAM_UART_UART2);
   else if (baud==2400)  setup_uart(2400, STREAM_UART_UART2);
   else if (baud==4800)  setup_uart(4800, STREAM_UART_UART2);
   else if (baud==9600)  setup_uart(9600, STREAM_UART_UART2);
   else if (baud==19200)  setup_uart(19200, STREAM_UART_UART2);
   else if (baud==38400)  setup_uart(38400, STREAM_UART_UART2);
   else if (baud==57600)  setup_uart(57600, STREAM_UART_UART2);
   else if (baud==115200)  setup_uart(115200, STREAM_UART_UART2);

  #if (UART2_RX_BUFFER_SIZE > 0)
   _UART2PhyRxIsrEnable(true);
  #endif
}

void UART2Putc(char c)
{
   fputc(c, STREAM_UART_UART2);
}

static char UART2Getc(void)
{
   return(fgetc(STREAM_UART_UART2));
}

uart2_rx_size_t UART2Kbhit(void)
{
  #if (UART2_RX_BUFFER_SIZE > 0)
   return(rcv_buffer_bytes(STREAM_UART_UART2));
  #else
   return(kbhit(STREAM_UART_UART2));
  #endif
}

void UART2Puts(char *p)
{
   char c;
   for(;;)
   {
      c = *p++;
      if (!c)
         break;
      UART2Putc(c);
   }
}

void UART2PutsROM(rom char *p)
{
   char c;
   for(;;)
   {
      c = *p++;
      if (!c)
         break;
      UART2Putc(c);
   }
}

#endif   //ifndef __UART2_C__
