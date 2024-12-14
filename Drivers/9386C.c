///////////////////////////////////////////////////////////////////////////
////   Library for a MicroChip 93AA86C configured for a x16 org        ////
////                                                                   ////
////   init_ext_eeprom();    Call before the other functions are used  ////
////                                                                   ////
////   write_ext_eeprom(a, d);  Write the byte d to the address a      ////
////                                                                   ////
////   d = read_ext_eeprom(a);   Read the byte d from the address a    ////
////                                                                   ////
////   The main program may define eeprom_select, eeprom_di, eeprom_do ////
////   and eeprom_clk to override the defaults below.                  ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2003 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
///////////////////////////////////////////////////////////////////////////
#ifndef EEPROM_SELECT

#define EEPROM_SELECT PIN_B4
#define EEPROM_CLK    PIN_B2
#define EEPROM_DI     PIN_B1
#define EEPROM_DO     PIN_B0

#endif

#define EEPROM_ADDRESS int16
#define EEPROM_SIZE    2048

void init_ext_eeprom() {
   int8 cmd[2];
   int8 i;

   output_low(EEPROM_DI);
   output_low(EEPROM_CLK);
   output_low(EEPROM_SELECT);

   cmd[0]=0x00;      
   cmd[1]=0x98;      // 1001 1000  SB=1, OP=00, A10=1, A9=1, other AX=0

   output_high(EEPROM_SELECT);      

   for(i=1;i<=13;++i) {             // SB 1bit, OP 2bit, ADDR 10bit
      output_bit(EEPROM_DI, shift_left(cmd,2,0));
      output_high(EEPROM_CLK);
      output_low(EEPROM_CLK);
   }
   output_low(EEPROM_DI);
   output_low(EEPROM_SELECT);
}

void write_ext_eeprom(EEPROM_ADDRESS address, int16 data) {
   int8 cmd[4];
   int8 i;

   cmd[0]=data;               // lo(data)
   cmd[1]=data>>8;            // hi(data)
   cmd[2]=address;
   cmd[3]=0x14|(address>>8);   // 0,0,0,SB=1,OP=01,A9,A8

   for(i=1;i<=3;++i)           // start with SB
      shift_left(cmd,4,0);
   output_high(EEPROM_SELECT);

   for(i=1;i<=29;++i) {         // #bits : SB=1,OP=2,ADDR=10,DATA=16
       output_bit(EEPROM_DI, shift_left(cmd,4,0));
       output_high(EEPROM_CLK);
       output_low(EEPROM_CLK);
   }
   output_low(EEPROM_DI);
   output_low(EEPROM_SELECT);
   delay_ms(6);   
}

int16 read_ext_eeprom(EEPROM_ADDRESS address) {
   int8 cmd[4];
   int8 i;
   int16 data;

   cmd[0]=0;
   cmd[1]=0;               
   cmd[2]=address;        
   cmd[3]=(0x18|(address>>8));  // 0,0,0,SB=1,OP=10,A9,A8

   for(i=1;i<=3;++i)           // start with SB
      shift_left(cmd,4,0);
   output_high(EEPROM_SELECT);
   
   for(i=1;i<=29;++i) {        // #bits : SB=1,OP=2,ADDR=10,DATA=16
       output_bit(EEPROM_DI, shift_left(cmd,4,0));
       output_high(EEPROM_CLK);
       output_low(EEPROM_CLK);
       if(i>13)                // put out data
         shift_left(&data,2,input(EEPROM_DO));
   }
   output_low(EEPROM_SELECT);
   return(data);
}
