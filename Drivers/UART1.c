/*
   UART1.c
   
   Library for sending/receiving data on the PIC's UART 1 peripheral.
   
   See UART1.h for documentation
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

#ifndef __UART1_C__
#define __UART1_C__

#include <UART1.h>

#use rs232(UART1, stream=STREAM_UART_UART1, noinit, RECEIVE_BUFFER=UART1_RX_BUFFER_SIZE, errors)

#if (UART1_RX_BUFFER_SIZE > 0)
static void _UART1PhyRxIsrEnable(bool enable)
{
   if (enable)
   {
      enable_interrupts(INT_RDA);
   }
   else
   {
      disable_interrupts(INT_RDA);
   }
}
#endif

void UART1Init(uint32_t baud)
{
  #if (UART1_RX_BUFFER_SIZE > 0)
   _UART1PhyRxIsrEnable(false);
  #endif
  
   if (baud==300)  setup_uart(300, STREAM_UART_UART1);
   else if (baud==1200)  setup_uart(1200, STREAM_UART_UART1);
   else if (baud==2400)  setup_uart(2400, STREAM_UART_UART1);
   else if (baud==4800)  setup_uart(4800, STREAM_UART_UART1);
   else if (baud==9600)  setup_uart(9600, STREAM_UART_UART1);
   else if (baud==19200)  setup_uart(19200, STREAM_UART_UART1);
   else if (baud==38400)  setup_uart(38400, STREAM_UART_UART1);
   else if (baud==57600)  setup_uart(57600, STREAM_UART_UART1);
   else if (baud==115200)  setup_uart(115200, STREAM_UART_UART1);

  #if (UART1_RX_BUFFER_SIZE > 0)
   _UART1PhyRxIsrEnable(true);
  #endif
}

void UART1Putc(char c)
{
   fputc(c, STREAM_UART_UART1);
}

static char UART1Getc(void)
{
   return(fgetc(STREAM_UART_UART1));
}

uart1_rx_size_t UART1Kbhit(void)
{
  #if (UART1_RX_BUFFER_SIZE > 0)
   return(rcv_buffer_bytes(STREAM_UART_UART1));
  #else
   return(kbhit(STREAM_UART_UART1));
  #endif
}

void UART1Puts(char *p)
{
   char c;
   for(;;)
   {
      c = *p++;
      if (!c)
         break;
      UART1Putc(c);
   }
}

void UART1PutsROM(rom char *p)
{
   char c;
   for(;;)
   {
      c = *p++;
      if (!c)
         break;
      UART1Putc(c);
   }
}

#endif   //ifndef __UART1_C__
