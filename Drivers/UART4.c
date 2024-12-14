/*
   UART4.c
   
   Library for sending/receiving data on the PIC's UART 4 peripheral.
   
   See UART4.h for documentation
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

#ifndef __UART4_C__
#define __UART4_C__

#include <UART4.h>

#use rs232(UART4, stream=STREAM_UART_UART4, noinit, RECEIVE_BUFFER=UART4_RX_BUFFER_SIZE, errors)

#if (UART4_RX_BUFFER_SIZE > 0)
static void _UART4PhyRxIsrEnable(bool enable)
{
   if (enable)
   {
      enable_interrupts(INT_RDA4);
   }
   else
   {
      disable_interrupts(INT_RDA4);
   }
}
#endif

void UART4Init(uint32_t baud)
{
  #if (UART4_RX_BUFFER_SIZE > 0)
   _UART4PhyRxIsrEnable(false);
  #endif
  
   if (baud==300)  setup_uart(300, STREAM_UART_UART4);
   else if (baud==1200)  setup_uart(1200, STREAM_UART_UART4);
   else if (baud==2400)  setup_uart(2400, STREAM_UART_UART4);
   else if (baud==4800)  setup_uart(4800, STREAM_UART_UART4);
   else if (baud==9600)  setup_uart(9600, STREAM_UART_UART4);
   else if (baud==19200)  setup_uart(19200, STREAM_UART_UART4);
   else if (baud==38400)  setup_uart(38400, STREAM_UART_UART4);
   else if (baud==57600)  setup_uart(57600, STREAM_UART_UART4);
   else if (baud==115200)  setup_uart(115200, STREAM_UART_UART4);

  #if (UART4_RX_BUFFER_SIZE > 0)
   _UART4PhyRxIsrEnable(true);
  #endif
}

void UART4Putc(char c)
{
   fputc(c, STREAM_UART_UART4);
}

static char UART4Getc(void)
{
   return(fgetc(STREAM_UART_UART4));
}

uart4_rx_size_t UART4Kbhit(void)
{
  #if (UART4_RX_BUFFER_SIZE > 0)
   return(rcv_buffer_bytes(STREAM_UART_UART4));
  #else
   return(kbhit(STREAM_UART_UART4));
  #endif
}

void UART4Puts(char *p)
{
   char c;
   for(;;)
   {
      c = *p++;
      if (!c)
         break;
      UART4Putc(c);
   }
}

void UART4PutsROM(rom char *p)
{
   char c;
   for(;;)
   {
      c = *p++;
      if (!c)
         break;
      UART4Putc(c);
   }
}

#endif   //ifndef __UART4_C__
