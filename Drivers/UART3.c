/*
   UART3.c
   
   Library for sending/receiving data on the PIC's UART 3 peripheral.
   
   See UART3.h for documentation
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

#ifndef __UART3_C__
#define __UART3_C__

#include <UART3.h>

#use rs232(UART3, stream=STREAM_UART_UART3, noinit, RECEIVE_BUFFER=UART3_RX_BUFFER_SIZE, errors)

#if (UART3_RX_BUFFER_SIZE > 0)
static void _UART3PhyRxIsrEnable(bool enable)
{
   if (enable)
   {
      enable_interrupts(INT_RDA3);
   }
   else
   {
      disable_interrupts(INT_RDA3);
   }
}
#endif

void UART3Init(uint32_t baud)
{
  #if (UART3_RX_BUFFER_SIZE > 0)
   _UART3PhyRxIsrEnable(false);
  #endif
  
   if (baud==300)  setup_uart(300, STREAM_UART_UART3);
   else if (baud==1200)  setup_uart(1200, STREAM_UART_UART3);
   else if (baud==2400)  setup_uart(2400, STREAM_UART_UART3);
   else if (baud==4800)  setup_uart(4800, STREAM_UART_UART3);
   else if (baud==9600)  setup_uart(9600, STREAM_UART_UART3);
   else if (baud==19200)  setup_uart(19200, STREAM_UART_UART3);
   else if (baud==38400)  setup_uart(38400, STREAM_UART_UART3);
   else if (baud==57600)  setup_uart(57600, STREAM_UART_UART3);
   else if (baud==115200)  setup_uart(115200, STREAM_UART_UART3);

  #if (UART3_RX_BUFFER_SIZE > 0)
   _UART3PhyRxIsrEnable(true);
  #endif
}

void UART3Putc(char c)
{
   fputc(c, STREAM_UART_UART3);
}

static char UART3Getc(void)
{
   return(fgetc(STREAM_UART_UART3));
}

uart3_rx_size_t UART3Kbhit(void)
{
  #if (UART3_RX_BUFFER_SIZE > 0)
   return(rcv_buffer_bytes(STREAM_UART_UART3));
  #else
   return(kbhit(STREAM_UART_UART3));
  #endif
}

void UART3Puts(char *p)
{
   char c;
   for(;;)
   {
      c = *p++;
      if (!c)
         break;
      UART3Putc(c);
   }
}

void UART3PutsROM(rom char *p)
{
   char c;
   for(;;)
   {
      c = *p++;
      if (!c)
         break;
      UART3Putc(c);
   }
}

#endif   //ifndef __UART3_C__
