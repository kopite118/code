///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                          devkit_ezapp.h                           ////
////                                                                   ////
//// Header file containing relevant definitions for the CCS EZ App    ////
//// Lynx development kit.  Will compile the program compatible        ////
//// with the UART2 bootloader shipped on the unit.  Bootloader        ////
//// will be disabled if you use an ICD for debugging.  Will #import   ////
//// the bootloader HEX so you can use an ICD to reprogram a unit to   ////
//// put the bootloader back on the device.                            ////
////                                                                   ////
//// Bootloader uses 19200 baud.  To use bootloader, connect CPC       ////
//// cable to D6/D7 port and run the CCS SIOW software.  In CCS        ////
//// SIOW software, connect to the COM port connected to the           ////
//// hardware's D6/D7 port and use 19200 baud.  Then power-cycle       ////
//// the hardware while holding down the B4 push-button.  In SIOW      ////
//// you should then see the following string:                         ////
////     Bootloader Version 1.0                                        ////
////     Waiting for download...                                       ////
//// Then in SIOW, goto 'Download Software' from the File menu         ////
//// and choose the HEX file you want to program onto the unit.        ////
////                                                                   ////
//// #defining DEVICE_USE_ICD before including this file will add the  ////
//// #device ICD=TRUE line needed to compile with ICD debugging        ////
//// support.                                                          ////
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

   #include <18F45K22.h>
   
   #if defined(DEVICE_USE_ICD)
      #device ICD=TRUE
   #endif
   
   #device ADC=10

   // Datasheet shows 2% error at 16mhz, any other values aren't documented.
   #use delay(int=8MHz, clock=16MHz)
   
   // NOTE: bootloader cannot change the fuses/config-bits!!
   #fuses NOWDT      //WDT is not permanently enabled.  setup_wdt() can be used to turn it on
   #fuses WDT1024    //WDT perdiod (no prescalar) ~4.1ms.  1024*4.1ms = ~4s period
   #fuses NOWRT      //program memory not write protected
   #fuses WRTB       //write protect boot-block
   #fuses WRTC       //write protect config bits
   #fuses NOWRTD     //data EEPROM not write protected
   
   #define PROGROM_LOADER_SIZE  0x7FF //this maps to the write protected boot-block of this PIC
   
   #define PROGROM_LOADER_START  0
   #define PROGROM_LOADER_END    PROGROM_LOADER_SIZE
   #define PROGROM_APP_START     (PROGROM_LOADER_END+1)
   #define PROGROM_ISR_START     (PROGROM_LOADER_END+9)
   
   #if !defined(__NO_LOADER) && !getenv("ICD")
      #build(reset=PROGROM_APP_START, interrupt=PROGROM_ISR_START)
      
      #org 0, PROGROM_LOADER_END {}

      #import(HEX, file="bootloader_ezapp.hex", RANGE=PROGROM_LOADER_START:PROGROM_LOADER_END)
   #else
      #warning Bootloader disabled!  Programming this HEX onto the PIC using an ICD will erase the bootloader!
   #endif
   
   // RN4020 daughterboard connected to PDIP socket.
   // these definitions used by RN4020_MLDP_Bluetooth.c driver.
   #define PIN_BT_MLDP_CONNECTED PIN_C3   //pio1, pin#10
   #define PIN_BT_MLDP_CMD_MODE  PIN_C5   //pio3, pin#8
   #define PIN_BT_SWAKE          PIN_D0   //wake_sw, pin#7
   #define PIN_BT_CTS            PIN_C4   //cts, pin#14
   #define PIN_BT_RTS            PIN_D1   //rts, pin#18
   
   // RN42 daughterboard connected to PDIP socket.
   // these definitions used by RN42_Bluetooth.c driver.
   // gpio6 = pin_a1 = bt master mode.  (high = auto-master, default = pull-down)
   // gpio7 = floating = baud rate (high = 9600, low = 11500, default = pull-down)
   // reset = pin_a0 = active-low reset. hold low for power-down. (default = pull-up)
   // gpio3 = pin_a2 = audo discovery.  (high = auto discovery enabled.  default = pull-down)
   // gpio4 = pin_a3 = set factory defaults (default = pull-down)
   // gpio5 = led = toggles based on state, low on connect.
   // gpio2 = led = high when connected, low when not connected
   #define PIN_BT_SPP_RESET      PIN_C4
   #define PIN_BT_SPP_DISCOVERY  PIN_D1
   #define PIN_BT_SPP_CONNECTED  PIN_D0

   // UART1 connected to PDIP daughterboard socket.
   // this definition used by both RN4020_MLDP_Bluetooth.c and 
   // RN42_Bluetooth.c drivers.
   #define STREAM_BT_SPP_UART 1
   
   // UART2 is connected to an RS232 level converter.
   
   // Small LEDs.
   // set pin high to turn on LED, set pin low to turn off LED.
   #define PIN_LED_RED     PIN_E0
   #define PIN_LED_YELLOW  PIN_E1
   #define PIN_LED_GREEN   PIN_E2
   
   // BIG LED.
   #define PIN_LED_BIG_RED    PIN_C1
   
   // Relays.
   #define PIN_OUTPUT_RELAY0  PIN_D2
   #define PIN_OUTPUT_RELAY1  PIN_D5
   
   // push-buttons.
   // the pin will be read low when button is held pressed.
   // the pin will be read high when button is idle (not pressed).
   #define PIN_INPUT_BUTTON0  PIN_D3
   #define PIN_INPUT_BUTTON1  PIN_B4

   // analog channels.
   // you can pass these to read_adc.
   // don't forget setup_adc_ports(sAN0 | sAN1 | sAN2 | sAN3) and setup_adc()
   #define AN_CHANNEL_POT0          0  //PIN_A0
   #define AN_CHANNEL_POT1          1  //PIN_A1
   #define AN_CHANNEL_PHOTO         2  //PIN_A2 - photo diode
   #define AN_CHANNEL_THERMISTOR    3  //PIN_A3
#endif
