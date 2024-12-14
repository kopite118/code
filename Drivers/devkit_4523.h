///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                          devkit_4523.h                            ////
////                                                                   ////
//// Header file containing relevant definitions for the CCS 184523    ////
//// development kit.  Will compile the program compatible             ////
//// with the UART bootloader shipped on the unit.  Bootloader         ////
//// will be disabled if you use an ICD for debugging.  Will #import   ////
//// the bootloader HEX so you can use an ICD to reprogram a unit to   ////
//// put the bootloader back on the device.                            ////
////                                                                   ////
//// Bootloader uses 19200 baud.  To use bootloader, connect USB       ////
//// cable to USB port and run the CCS SIOW software.  In CCS          ////
//// SIOW software, connect to the COM port connected to the           ////
//// hardware's YSB port and use 19200 baud.  Then power-cycle         ////
//// the hardware while holding down the B1 push-button.  In SIOW      ////
//// you should then see the following string:                         ////
////     Bootloader Ready                                              ////
//// Then in SIOW, goto 'Download Software' from the File menu         ////
//// and choose the HEX file you want to program onto the unit.        ////
////                                                                   ////
//// #defining DEVICE_USE_ICD before including this file will add the  ////
//// #device ICD=TRUE line needed to compile with ICD debugging        ////
//// support.                                                          ////
////                                                                   ////
//// #defining __NO_LOADER before including this file will prevent the ////
//// bootloader from being used in the built application.  Normally    ////
//// you will not want to define this.                                 ////
////                                                                   ////
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

#ifndef __DEVKIT_EZAPP_H__
#define __DEVKIT_EZAPP_H__

   #include <18F4523.h>
   
   #if defined(DEVICE_USE_ICD)
      #device ICD=TRUE
   #endif
   
   #device ADC=12

   #use delay(crystal=10Mhz, clock=40MHz)
   
   // NOTE: bootloader cannot change the fuses/config-bits!!
   #fuses NOWDT      //WDT is not permanently enabled.  setup_wdt() can be used to turn it on
   #fuses NOWRT      //program memory not write protected
   #fuses WRTB       //write protect boot-block
   #fuses WRTC       //write protect config bits
   #fuses NOWRTD     //data EEPROM not write protected
   #fuses WDT1024    //~4ms tick, therefore ~4s overvflow
   
   #define PROGROM_LOADER_SIZE  0x7FF //this maps to the write protected boot-block of this PIC
   
   #define PROGROM_LOADER_START  0
   #define PROGROM_LOADER_END    PROGROM_LOADER_SIZE
   #define PROGROM_APP_START     (PROGROM_LOADER_END+1)
   #define PROGROM_ISR_START     (PROGROM_LOADER_END+9)
   
   #if !defined(__NO_LOADER) && !getenv("ICD")
      #build(reset=PROGROM_APP_START, interrupt=PROGROM_ISR_START)
      
      #org 0, PROGROM_LOADER_END {}

      #import(HEX, file="bootloader_4523.hex", RANGE=PROGROM_LOADER_START:PROGROM_LOADER_END)
   #else
      #warning Bootloader disabled!  Programming this HEX onto the PIC using an ICD will erase the bootloader!
   #endif
   
   // UART1 (PIN_B7, PIN_B5) is connected to a USB<->Serial converter.
   // The USB<->Serial converter supports these baud rates:
   //    300, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200
   
   // Small LEDs.
   // set pin high to turn on LED, set pin low to turn off LED.
   #define PIN_LED_RED     PIN_B4
   #define PIN_LED_YELLOW  PIN_B5
   #define PIN_LED_GREEN   PIN_B3

   // Relays.
   #define PIN_OUTPUT_RELAY0  PIN_C5  
   
   // Speaker
   // this speaker requires oscillator, either pulse this pin over 1khz or use a PWM
   #define PIN_SPEAKER     PIN_C2
   
   // push-buttons.
   // the pin will be read low when button is held pressed.
   // the pin will be read high when button is idle (not pressed).
   #define PIN_INPUT_BUTTON0  PIN_B0
   #define PIN_INPUT_BUTTON1  PIN_B1
   
   // LCD connection
   // these definitions are compatible with CCS's lcd.c driver
   #define LCD_ENABLE_PIN  PIN_D0
   #define LCD_RS_PIN      PIN_D1
   #define LCD_RW_PIN      PIN_D2
   #define LCD_DATA4       PIN_D4
   #define LCD_DATA5       PIN_D5
   #define LCD_DATA6       PIN_D6
   #define LCD_DATA7       PIN_D7
   
   // Keypad
   // A keypad connection is at J6.  It is wired to Port D (PIN_D1, PIN_D2 .. PIN_D7)
   // and is compatible with CCS's kbd.c driver.
   
   // MCP7940 RTC Real Time Clock
   // MFP=PIN_B2, SCL=PIN_C3, SDA=PIN_C4
   
   // analog channels.
   // you can pass these to set_adc_channel().
   // don't forget setup_adc_ports(AN0) and setup_adc()
   #define AN_CHANNEL_POT0          0  //PIN_A0
   
unsigned int8 read_adc8(void)
{
   unsigned int16 ret;
   
   ret = read_adc();
  
  #if (getenv("ADC_RESOLUTION")==8)
  #elif (getenv("ADC_RESOLUTION")==10)
   ret /= 4;
  #elif (getenv("ADC_RESOLUTION")==12)
   ret /= 16;
  #else
   #error huh?
  #endif
  
   return(ret);
}

unsigned int16 read_adc10(void)
{
   unsigned int16 ret;
   
   ret = read_adc();
  
  #if (getenv("ADC_RESOLUTION")==8)
   ret *= 4;
  #elif (getenv("ADC_RESOLUTION")==10)
  #elif (getenv("ADC_RESOLUTION")==12)
   ret /= 4;
  #else
   #error huh?
  #endif
  
   return(ret);
}

unsigned int8 read_adc12(void)
{
   unsigned int16 ret;
   
   ret = read_adc();
  
  #if (getenv("ADC_RESOLUTION")==8)
   ret /= 16;
  #elif (getenv("ADC_RESOLUTION")==10)
   ret /= 4;
  #elif (getenv("ADC_RESOLUTION")==12)
  #else
   #error huh?
  #endif
  
   return(ret);
}

#endif
