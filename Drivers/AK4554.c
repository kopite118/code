///////////////////////////////////////////////////////////////////////////
////   Library for a AK4554 CODEC                                      ////
////                                                                   ////
////   iInitCodec()  // Call first                                     ////
////                                                                   ////
////   Define functions for PROCESS_CODEC_LEFT_DATA and/or             ////
////   GET_CODEC_LEFT_DATA to process each CODEC word.  Only the       ////
////   left channel can be used with this CODEC and the PIC due to     ////
////   I2S vs TDM protocol differences.                                ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 2015  Custom Computer Services               ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
///////////////////////////////////////////////////////////////////////////


#ifndef CODEC_RESET1_PIN
 #define CODEC_RESET1_PIN   PIN_A1
 #define CODEC_RESET2_PIN   PIN_B1
#endif

#ifndef CODEC_FILTER_PIN
 #define CODEC_FILTER_PIN      PIN_B12  
#endif

#ifndef PROCESS_CODEC_LEFT_DATA
 #define PROCESS_CODEC_LEFT_DATA(x)  int16 dummy=x
#endif

#ifndef GET_CODEC_LEFT_DATA
 #define GET_CODEC_LEFT_DATA 0 
#endif


//#use spi(MASTER, SPI2, BAUD=1333333, MODE=3, XFER16, MSB_FIRST)
// Uses direct wires due tot eh frame mode

#word SPISTAT = getenv("SFR:SPI2STAT")
#word SPICON1 = getenv("SFR:SPI2CON1")
#word SPICON2 = getenv("SFR:SPI2CON2")
#word SPIBUF =  getenv("SFR:SPI2BUF")

void InitCodec(void) { 
   output_low(CODEC_RESET1_PIN);
   output_low(CODEC_RESET2_PIN);

   SPICON1 = 0x0450;  // ~15khz
   SPICON2 = 0x8000; 
   SPISTAT = 0x0010;   
   bit_set(SPICON1,5);
   bit_set(SPISTAT,15);
   
   
   delay_ms(1);
   output_high(CODEC_RESET2_PIN);
   delay_ms(1);
   output_low(CODEC_RESET2_PIN);
   delay_ms(1);
   output_high(CODEC_RESET1_PIN);
   delay_ms(1);
   output_low(CODEC_RESET1_PIN);
   delay_ms(1);
   output_high(CODEC_RESET1_PIN);
   output_high(CODEC_FILTER_PIN); 
   delay_ms(10);
   clear_interrupt(INT_SPI2);
   enable_interrupts(INT_SPI2);
   SPIBUF = 0;
   SPIBUF = 0;
}

#INT_SPI2
void codec_isr(void) {
   PROCESS_CODEC_LEFT_DATA(SPIBUF);
   SPIBUF=GET_CODEC_LEFT_DATA;
}


