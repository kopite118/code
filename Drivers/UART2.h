/*
   UART2.h
   
   Library for sending/receiving data on the PIC's UART 2 peripheral.
   
   This is mostly a wrapper to the #use rs232() serial library that is 
   built-in to the compiler.
   
   CONFIGURATION:
   -------------------------------------------------------------------
   UART2_RX_BUFFER_SIZE
   If you want to use an ISR receive buffer, define this value to the size
   (in bytes) of the buffer you want to use.  On 8-bit PICmicros (PIC16,
   PIC18, etc) the library will turn on the appropiate peripheral interrupt 
   enable bit, but the user is responsible for turning on the global interrupt 
   enable bit (enable_interrupts(GLOBAL) in CCS C).
*/
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2015 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
////                     http://www.ccsinfo.com                        ////
///////////////////////////////////////////////////////////////////////////

#ifndef __UART2_H__
#define __UART2_H__

#include <stdint.h>
#include <stdbool.h>

#if !defined(UART2_RX_BUFFER_SIZE)
   #define UART2_RX_BUFFER_SIZE 0
#endif

#if (UART2_RX_BUFFER_SIZE == 0)
   typedef bool uart2_rx_size_t;
#elif (UART2_RX_BUFFER_SIZE >= 0x100)
   typedef uint16_t uart2_rx_size_t;
#else
   typedef uint8_t uart2_rx_size_t;
#endif

// Initialze and turn on the UART.
// If using RX buffering, will turn on the RX interrupt, but it WILL NOT turn
// on the global interrupt enable bit.
// You are limited to these baud rates:
//    300,1200,2400,4800,9600,19200,38400,57600,115200
void UART2Init(uint32_t baud);

// Send a character to the UART.
void UART2Putc(char c);

// Get a character from the UART.  Will wait for data to be ready for RX,
// which can cause a lockup condition if no data is being received.
char UART2Getc(void);

// if receive buffering was enabled, this returns the number of characters 
// in the receive buffer than can be read with UART2Getc().
// if receive buffering was not enabled, returns TRUE if there is data 
// to be read from the UART peripheral.
uart2_rx_size_t UART2Kbhit(void);

// Put a string (a null terminated array of chars) to the UART, does not send
// the null terminator.
void UART2Puts(char *p);
void UART2PutsROM(rom char *p);

#endif   //ifndef __UART2_H__