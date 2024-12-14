///////////////////////////////////////////////////////////////////////////
////                                                                   ////
//// RAPIDUSB.H                                                        ////
//// *************************************************************     ////
////                                                                   ////
//// Header file for developing applications compatible with the       ////
//// CCS Rapid USB development kit and it's bootloader.                ////
////                                                                   ////
//// Include this file instead of the traditional CCS device header    ////
//// file.  See the E3mini exercise book for more information and      ////
//// examples.  This file should not be modified.                      ////
////                                                                   ////
////                                                                   ////
//// DEVKIT I/O CONFIGURATION                                          ////
//// *************************************************************     ////
////                                                                   ////
//// PIN_A5 = Green LED (high is on)                                   ////
//// PIN_C6 = Red LED (high is on)                                     ////
//// PIN_C1 = Button (low is pressed)                                  ////
////                                                                   ////
//// Green LED and Red LED cannot both be on at the same time.         ////
////                                                                   ////
////                                                                   ////
//// USB API                                                           ////
//// *************************************************************     ////
////                                                                   ////
//// API for adding a virtual COM port on your PC over USB using       ////
//// the standard Communication Device Class (CDC) specification.      ////
//// On Windows, a CDC device will look like a COM port and can        ////
//// be opened like a COM serial port.                                 ////
////                                                                   ////
//// This device creates a virtual RS232 link between the PC and       ////
//// the PIC, therefore the API provided will be familiar to           ////
//// anyone with standard UART stream I/O:                             ////
////                                                                   ////
//// usb_cdc_kbhit() - Returns TRUE if there is one or more            ////
////      character received and waiting in the receive buffer.        ////
////                                                                   ////
//// usb_cdc_getc() - Gets a character from the receive buffer.  If    ////
////      there is no data in the receive buffer it will wait until    ////
////      there is data in the receive buffer.  If you do not want     ////
////      to wait in an infinit loop, use usb_cdc_kbhit() first to     ////
////      check if there is data before calling usb_cdc_getc().        ////
////                                                                   ////
//// usb_cdc_putc(char c) - Puts a character into the transmit         ////
////      buffer.  If the transmit buffer is full it will wait until   ////
////      the transmit buffer is not full before putting the char      ////
////      into the transmit buffer.  The transmit buffer is read by    ////
////      the PC very quickly, and therefore the buffer should only    ////
////      be full for a few milli-seconds.  If you are concerned       ////
////      and don't want to be stuck in a long or infinite loop,       ////
////      use usb_cdc_putready() to see if there is space in the       ////
////      transmit buffer before putting data into the transmit        ////
////      buffer.                                                      ////
////                                                                   ////
//// usb_cdc_putc_safe(char c) - Similar to usb_cdc_putc(), but will   ////
////     abort TX if transmit buffer is full.                          ////
////                                                                   ////
//// usb_cdc_putready() - Returns TRUE if there is room left in the    ////
////      transmit buffer for another character.                       ////
////                                                                   ////
//// usb_enumerated() - Returns TRUE if the device was detected by     ////
////     the operating system over USB.                                ////
////                                                                   ////
//// usb_cdc_opened() - Returns TRUE if we received a                  ////
////      Set_Line_Coding.  On most serial terminal programs (such     ////
////      as Hyperterminal), they will send a Set_Line_Coding          ////
////      message when the program starts and it opens the virtual     ////
////      COM port.  This is a simple way to determine if the PC       ////
////      is ready to display data on a serial terminal program,       ////
////      but is not garaunteed to work all the time or on other       ////
////      terminal programs.                                           ////
////                                                                   ////
//// usb_cdc_serial_state(state) - Transmit to the host our            ////
////       SERIAL_STATE.  This holds information such as DCD, DSR,     ////
////       ring, break and more.  See the documentation above          ////
////       usb_cdc_serial_state() for more information, including      ////
////       format of state structure.                                  ////
////                                                                   ////
//// usb_cdc_info(*usb_cdc_info_t) - Returns information about the     ////
////     virtual serial port.  See usb_cdc_info_t for more             ////
////     information.                                                  ////
////                                                                   ////
//// The following functions are also provided, and are ports of the   ////
//// I/O functions in input.c.  See input.c and the CCS manual for     ////
//// documentation:                                                    ////
////   get_float_usb() - Read a float number from the user             ////
////   get_long_usb() - Read a long number from the user               ////
////   get_int_usb() - Read an integer number from the user            ////
////   get_string_usb(char *s, int max) - Read a string from the user. ////
////   gethex_usb() - Read a byte, in HEX, from the user               ////
////   gethex1_usb() - Read a HEX character                            ////
////                                                                   ////
//// An .INF file is provided (cdc_NTXP.inf) that will load the        ////
//// standard CDC drivers for a virtual COM port in Windows            ////
//// NT/2000/XP and above.                                             ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2011 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
///////////////////////////////////////////////////////////////////////////

#ifndef __E3MINI_H__
#define __E3MINI_H__

#include <16f1459.h>
#if !defined(__e3loader)
   #device NORETFIE
#endif

#device ADC=8
#device NO_RESERVE_ERRORS
#device PASS_STRINGS=IN_RAM

#if !defined(__e3loader)
#use delay(clock=48MHz)
#endif

#if defined(__e3loader)
#use delay(int=8MHz, clock=48MHz, USB_FULL, act=USB)
#endif

#fuses MCLR
#fuses NOLVP
#fuses NOSTVREN
#fuses NOPROTECT
#fuses NOWDT
#fuses NOIESO
#fuses NOFCMEN
#fuses BORV19
#fuses NOBROWNOUT
#fuses WRT_1000

#fuses INTRC_IO
#fuses PUT
#fuses NOCLKOUT
#fuses NOCPUDIV
#fuses LS48MHZ
#fuses PLL3X
#fuses PLLEN
#fuses NOLPBOR
#fuses NODEBUG


#define HW_INIT() setup_comparator(NC_NC_NC_NC)

//how big is the bootloader?
//the bootloader will reside from address 0x0000 to this location.  the
//application will then sit at this location+1 to the end of program memory.
#if !defined(__DONT_USE_BOOTLOADER__)
   #define LOADER_SIZE        (0xFFF)
   #define LOADER_START       (0)
   #define LOADER_END         (LOADER_SIZE)
   #define APPLICATION_START  (LOADER_SIZE+1)
   #define APPLICATION_END    (getenv("PROGRAM_MEMORY")-1)
   #define APPLICATION_ISR    (APPLICATION_START+8)
   #define LOADER_RAM_END   0x1eF //this and everything before it is USB and loader
   #define APPLICATION_RAM_START 0x220
   #define APPLICATION_RAM_END   0x64F

   /*
      0-8: compiler scratch
      9-a: USB_RAM_API
   */
   //was SCRATCH_RAM_END
   #define SCRATCH_RAM_SIZE    0xa    //this and everything before this is common scratch shared between loader and application
   
   #byte SFR_PCLATH=getenv("SFR:PCLATH")
#endif

#define LOADER_PROXY_SIZE     0x18
#define USB_IS_ENUMRATED_PROXY   0x590
#define USB_CDC_INFO_PROXY       0x5a8
#define USB_CDC_KBHIT_PROXY      0x5c0
#define USB_CDC_GETC_PROXY       0x5d8
#define USB_CDC_TBE_PROXY        0x5f0
#define USB_CDC_PUTC_PROXY       0x608
#define USB_CDC_STATE_PROXY      0x620

#define CALL_INSTRUCTION(x)   \
   #asm  \
      MOVLP x>>8  \
      call x   \
    __CALL_INSTRUCTION_RETURN:   \
      MOVPHW __CALL_INSTRUCTION_RETURN  \
      MOVWF SFR_PCLATH  \
   #endasm

#define GOTO_INSTRUCTION(x)   \
   #asm  \
      MOVLP x>>8  \
      goto x   \
   #endasm

#if !defined(__DONT_USE_BOOTLOADER__)
   #define USB_RAM_API_START        LOADER_RAM_END-2  //ideally this should be LOADER_RAM_END-1 but this causes a bug in the compiler
   unsigned int16 USB_RAM_API;
   #locate USB_RAM_API = USB_RAM_API_START

   typedef struct {
           unsigned int32   dwDTERrate;   //data terminal rate, in bits per second
           unsigned int8    bCharFormat;  //num of stop bits (0=1, 1=1.5, 2=2)
           unsigned int8    bParityType;  //parity (0=none, 1=odd, 2=even, 3=mark, 4=space)
           unsigned int8    bDataBits;    //data bits (5,6,7,8 or 16)
           struct
           {
               int1 dte_present; //(aka DTR) 1=DTE present, 0=DTE not present
               int1 active;      //1=activate carrier, 0=deactivate carrier
           } carrier;
   } usb_cdc_info_t;

   #if !defined(__e3loader)
   #build(reset=APPLICATION_START, interrupt=APPLICATION_ISR, stack=6, NOSLEEP_LOCK)   //loader uses 10 stack locations in isr
   #org LOADER_START, LOADER_END {}
   #reserve 0:LOADER_RAM_END
   
   int1 usb_cdc_kbhit(void)
   {
      CALL_INSTRUCTION(USB_CDC_KBHIT_PROXY);
      return(USB_RAM_API);
   }

   char usb_cdc_getc(void)
   {
      CALL_INSTRUCTION(USB_CDC_GETC_PROXY);
      return(USB_RAM_API);
   }

   int1 usb_cdc_putready(void)
   {
      CALL_INSTRUCTION(USB_CDC_TBE_PROXY);
      return(USB_RAM_API);
   }

   int1 usb_enumerated(void)
   {
      CALL_INSTRUCTION(USB_IS_ENUMRATED_PROXY);
      return(USB_RAM_API);
   }
   
   void usb_cdc_putc(char c)
   {
      if (usb_enumerated())
      {
         USB_RAM_API = c;
         CALL_INSTRUCTION(USB_CDC_PUTC_PROXY);
      }
   }

   void usb_cdc_putc_safe(char c)
   {
      if (usb_enumerated() && usb_cdc_putready())
      {
         usb_cdc_putc(c);
      }
   }

   void usb_cdc_info(usb_cdc_info_t *pInfo)
   {
      USB_RAM_API = pInfo;
      CALL_INSTRUCTION(USB_CDC_INFO_PROXY);
   }

   typedef struct
   {
      unsigned int bRxCarrier:1; //State of receiver carrier detection mechanism of device. This signal corresponds to V.24 signal 109 and RS-232 signal DCD.
      unsigned int bTxCarrier:1; //State of transmission carrier. This signal corresponds to V.24 signal 106 and RS-232 signal DSR
      unsigned int bBreak:1;  //State of break detection mechanism of the device.
      unsigned int bRingSignal:1;   //State of ring signal detection of the device.
      unsigned int bFraming:1;   //A framing error has occurred.
      unsigned int bParity:1;    //A parity error has occurred.
      unsigned int bOverRun:1;   //Received data has been discarded due to overrun in the device.
      unsigned int reserved0:1;   //future use
      unsigned int reserved1:8;   //future use
   } cdc_serial_state_t;

   /*
      Send SERIAL_STATE notification to the host.  This contains DSR, DCD, Ring,
      break signal and more (see cdc_serial_state_t).
      Some of these values are held by the host (meaning it's value only needs
      to be sent on change), but some values are one shot (meaning you continously
      need to send value to host while being held).

      Returns TRUE if succesfully sent, FALSE if not sent (probably because
      endpoint is full)
   */
   int1 usb_cdc_serial_state(cdc_serial_state_t state)
   {
      USB_RAM_API = (unsigned int16)state;

      CALL_INSTRUCTION(USB_CDC_STATE_PROXY);
      
      return(USB_RAM_API);
   }

   int1 usb_cdc_opened(void)
   {
      usb_cdc_info_t info;
      usb_cdc_info(&info);
      return(info.carrier.active && usb_enumerated());
   }

   void usb_init(void)
   {
      while (!usb_cdc_opened());
      delay_ms(500);
      while (usb_cdc_kbhit())
      {
         usb_cdc_getc();
      }
   }

   void usb_init_cs(void)
   {
   }

   void usb_task(void)
   {
   }
   
   #use rs232(call_putc=usb_cdc_putc, call_getc=usb_cdc_getc, call_kbhit=usb_cdc_kbhit)

   #define __USB_CDC_HELPERS_ONLY__
   #include <stdlib.h>
   #include <usb_cdc.h>

   #endif   //#if !defined(__e3loader)
   #else
   #include <stdlib.h>
   #include <usb_cdc.h>
   #endif   //!defined(__DONT_USE_BOOTLOADER__)

#ifndef HW_INIT
   #define HW_INIT()
#endif

#endif   //__E3MINI_H__
