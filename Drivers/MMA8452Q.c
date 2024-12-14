
///////////////////////////////////////////////////////////////////////////
////   Library for a Freescale MMA8452Q Accelerometer                  ////
////                                                                   ////
////   init_Accelerometer(addr,scale,rate) // Call first               ////
////       addr is 0 or 1 for the SA0 state                            ////
////       scale is 2,4 or 8 g                                         ////
////       rate is one of the ACC_RATE_ constants below                ////
////                                                                   ////
////   acc_read(addr, x, y, z)                                         ////
////       Returns x,y and z values for the unit at addr               ////
////                                                                   ////
////   The main program may define ACC_SDA                             ////
////   and ACC_SCL to override the defaults below.                     ////
////                                                                   ////
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

#ifndef ACC_SDA
#define ACC_SDA  PIN_C5
#define ACC_SCL  PIN_C4
#endif


#use i2c(master, sda=ACC_SDA, scl=ACC_SCL)

#define MMA_STATUS           0x00
#define MMA_OUT_X_MSB        0x01
#define MMA_OUT_X_LSB        0x02
#define MMA_OUT_Y_MSB        0x03
#define MMA_OUT_Y_LSB        0x04
#define MMA_OUT_Z_MSB        0x05
#define MMA_OUT_Z_LSB        0x06
#define MMA_SYSMOD           0x0B
#define MMA_INT_SOURCE       0x0C
#define MMA_WHO_AM_I         0x0D
#define MMA_XYZ_DATA_CFG     0x0E
#define MMA_HP_FILTER_CUTOFF 0x0F
#define MMA_PL_STATUS        0x10
#define MMA_PL_CFG           0x11
#define MMA_PL_COUNT         0x12
#define MMA_PL_BP_ZCOMP      0x13
#define MMA_P_L_THS_REG      0x14
#define MMA_FF_MT_CFG        0x15
#define MMA_FF_MT_SRC        0x16
#define MMA_FF_MT_THS        0x17
#define MMA_FF_MT_COUNT      0x18
#define MMA_TRANSIENT_CFG    0x1D
#define MMA_TRANSIENT_SRC    0x1E
#define MMA_TRANSIENT_THS    0x1F
#define MMA_TRANSIENT_COUNT  0x20
#define MMA_PULSE_CFG        0x21
#define MMA_PULSE_SRC        0x22
#define MMA_PULSE_THSX       0x23
#define MMA_PULSE_THSY       0x24
#define MMA_PULSE_THSZ       0x25
#define MMA_PULSE_TMLT       0x26
#define MMA_PULSE_LTCY       0x27
#define MMA_PULSE_WIND       0x28
#define MMA_ASLP_COUNT       0x29
#define MMA_CTRL_REG1        0x2A
#define MMA_CTRL_REG2        0x2B
#define MMA_CTRL_REG3        0x2C
#define MMA_CTRL_REG4        0x2D
#define MMA_CTRL_REG5        0x2E
#define MMA_OFF_X            0x2F
#define MMA_OFF_Y            0x30
#define MMA_OFF_Z            0x31

#define ACC_RATE_800HZ       0
#define ACC_RATE_400HZ       1
#define ACC_RATE_200HZ       2
#define ACC_RATE_100HZ       3
#define ACC_RATE_50HZ        4
#define ACC_RATE_12_5HZ      5
#define ACC_RATE_6_25HZ      6
#define ACC_RATE_1_56HZ      7


int8 acc_address;

unsigned int8 acc_read_reg(int8 address) {
   unsigned int8 d;
   
   i2c_start();
   i2c_write(acc_address);
   i2c_write(address);  
   i2c_start();
   i2c_write(acc_address|1);
   d = i2c_read(FALSE);   
   i2c_stop();
   return d;   
}

void acc_write_reg(int8 address, int8 data) {
   i2c_start();
   i2c_write(acc_address);
   i2c_write(address);  
   i2c_write(data);
   i2c_stop();   
}

int16 acc_read_reg16(int8 address) {
   unsigned int8 h,l;
   
   i2c_start();
   i2c_write(acc_address);
   i2c_write(address);  
   i2c_start();
   i2c_write(acc_address|1);
   h = i2c_read();   
   l = i2c_read(FALSE);   
   i2c_stop();
   return make16(h,l);
}

void acc_read(int8 addr, int16 & x, int16 & y, int16 & z) {
   int8 s,h,l;

   acc_address=0x38+2*addr;
  
   i2c_start();
   i2c_write(acc_address);
   i2c_write(MMA_STATUS);  
   i2c_start();
   i2c_write(acc_address|1);
   s = i2c_read();   
   h = i2c_read();   
   l = i2c_read();   
   x=make16(h,l);
   h = i2c_read();   
   l = i2c_read();   
   y=make16(h,l);
   h = i2c_read();       // Z does not have as many sig bits as the others  
   l = i2c_read(FALSE);  // Don't know why 
   z=make16(h,l);
   i2c_stop();
   if ((s&0x7)==0) {
     x=0;
     y=0;
     z=0;
   }
}

void init_accelerometer(int8 addr, int8 scale,int8 rate) {
   unsigned int8 d;
  
   acc_address=0x38+2*addr;
   d=acc_read_reg(MMA_CTRL_REG1);
   bit_clear(d,0);
   acc_write_reg(MMA_CTRL_REG1,d);
  
   switch(scale) {
     case 2 : d=0; break;
     case 4 : d=1; break;
     case 8 : d=2; break;
   }
   acc_write_reg(MMA_XYZ_DATA_CFG,d);
   
   d=rate<<3;
   if(scale!=8)
     d=d|4;  //Low noise
   acc_write_reg(MMA_CTRL_REG1,d);
   acc_write_reg(MMA_CTRL_REG3,2);
   acc_write_reg(MMA_CTRL_REG4,1);
   acc_write_reg(MMA_CTRL_REG5,1);
     
   d=acc_read_reg(MMA_CTRL_REG1);
   bit_set(d,0);
   acc_write_reg(MMA_CTRL_REG1,d);

}
