///////////////////////////////////////////////////////////////////////////
////   Library for a ST VL6180 Proximity sensor                        ////
////                                                                   ////
////   init_proximity_sensor()  // Call first                          ////
////                                                                   ////
////   range_in_mm = proximity_sensor_read()  // Reads sensor          ////
////                                                                   ////
////   The main program may define PS_SDA                              ////
////   and PS_SCL to override the defaults below.                      ////
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


#ifndef PS_SDA
#define PS_SDA  PIN_C2
#define PS_SCL  PIN_C1
#endif

#use i2c(master, sda=PS_SDA, scl=PS_SCL)

#define IDENTIFICATION__MODEL_ID 0x000
#define IDENTIFICATION__MODEL_REV_MAJOR 0x001
#define IDENTIFICATION__MODEL_REV_MINOR 0x002
#define IDENTIFICATION__MODULE_REV_MAJOR 0x003
#define IDENTIFICATION__MODULE_REV_MINOR 0x004
#define IDENTIFICATION__DATE_HI 0x006
#define IDENTIFICATION__DATE_LO 0x007
#define SYSTEM__MODE_GPIO0 0x010
#define SYSTEM__MODE_GPIO1 0x011
#define SYSTEM__HISTORY_CTRL 0x012
#define SYSTEM__INTERRUPT_CONFIG_GPIO 0x014
#define SYSTEM__INTERRUPT_CLEAR 0x015
#define SYSTEM__FRESH_OUT_OF_RESET 0x016
#define SYSTEM__GROUPED_PARAMETER_HOLD 0x017
#define SYSRANGE__START 0x018
#define SYSRANGE__THRESH_HIGH 0x019
#define SYSRANGE__THRESH_LOW 0x01A
#define SYSRANGE__INTERMEASUREMENT_PERIOD 0x01B
#define SYSRANGE__MAX_CONVERGENCE_TIME 0x01C
#define SYSRANGE__CROSSTALK_COMPENSATION_RATE 0x01E
#define SYSRANGE__CROSSTALK_VALID_HEIGHT 0x021
#define SYSRANGE__EARLY_CONVERGENCE_ESTIMATE 0x022
#define SYSRANGE__PART_TO_PART_RANGE_OFFSET 0x024
#define SYSRANGE__RANGE_IGNORE_VALID_HEIGHT 0x025
#define SYSRANGE__RANGE_IGNORE_THRESHOLD 0x026
#define SYSRANGE__MAX_AMBIENT_LEVEL_MULT 0x02C
#define SYSRANGE__RANGE_CHECK_ENABLES 0x02D
#define SYSRANGE__VHV_RECALIBRATE 0x02E
#define SYSRANGE__VHV_REPEAT_RATE 0x031
#define SYSALS__START 0x038
#define SYSALS__THRESH_HIGH 0x03A
#define SYSALS__THRESH_LOW 0x03C
#define SYSALS__INTERMEASUREMENT_PERIOD 0x03E
#define SYSALS__ANALOGUE_GAIN 0x03F
#define SYSALS__INTEGRATION_PERIOD 0x040
#define RESULT__RANGE_STATUS 0x04D
#define RESULT__ALS_STATUS 0x04E
#define RESULT__INTERRUPT_STATUS_GPIO 0x04F
#define RESULT__ALS_VAL 0x050
#define RESULT__HISTORY_BUFFER_x (0x2)
#define RESULT__RANGE_VAL 0x062
#define RESULT__RANGE_RAW 0x064
#define RESULT__RANGE_RETURN_RATE 0x066
#define RESULT__RANGE_REFERENCE_RATE 0x068
#define RESULT__RANGE_RETURN_SIGNAL_COUNT 0x06C
#define RESULT__RANGE_REFERENCE_SIGNAL_COUNT 0x070
#define RESULT__RANGE_RETURN_AMB_COUNT 0x074
#define RESULT__RANGE_REFERENCE_AMB_COUNT 0x078
#define RESULT__RANGE_RETURN_CONV_TIME 0x07C
#define RESULT__RANGE_REFERENCE_CONV_TIME 0x080
#define READOUT__AVERAGING_SAMPLE_PERIOD 0x10A
#define FIRMWARE__BOOTUP 0x119
#define FIRMWARE__RESULT_SCALER 0x120
#define I2C_SLAVE__DEVICE_ADDRESS 0x212
#define INTERLEAVED_MODE__ENABLE 0x2A3

unsigned int8 ps_read_reg(int16 address) {
   unsigned int8 d;
   
   i2c_start();
   d=i2c_write(0x52);
   i2c_write(address>>8);  
   i2c_write(address);  
   i2c_start();
   i2c_write(0x53);
   d = i2c_read();   
   i2c_stop();
   return d;   
}

void ps_write_reg(int16 address, int8 data) {
   i2c_start();
   i2c_write(0x52);
   i2c_write(address>>8);  
   i2c_write(address);  
   i2c_write(data);
   i2c_stop();   
}

int8 proximity_sensor_read(void) {
   int8 range,error;

   ps_write_reg(SYSRANGE__START,0x01);
   while((ps_read_reg(RESULT__INTERRUPT_STATUS_GPIO)&0x7)!=4) ;
   error=ps_read_reg(RESULT__ALS_STATUS);
   range=ps_read_reg(RESULT__RANGE_VAL);
   ps_write_reg(SYSTEM__INTERRUPT_CLEAR,0x07);
   return range;
}

void init_proximity_sensor(void) {
  ps_write_reg(0x0207, 0x01);
  ps_write_reg(0x0208, 0x01);
  ps_write_reg(0x0096, 0x00);
  ps_write_reg(0x0097, 0xfd);
  ps_write_reg(0x00e3, 0x00);
  ps_write_reg(0x00e4, 0x04);
  ps_write_reg(0x00e5, 0x02);
  ps_write_reg(0x00e6, 0x01);
  ps_write_reg(0x00e7, 0x03);
  ps_write_reg(0x00f5, 0x02);
  ps_write_reg(0x00d9, 0x05);
  ps_write_reg(0x00db, 0xce);
  ps_write_reg(0x00dc, 0x03);
  ps_write_reg(0x00dd, 0xf8);
  ps_write_reg(0x009f, 0x00);
  ps_write_reg(0x00a3, 0x3c);
  ps_write_reg(0x00b7, 0x00);
  ps_write_reg(0x00bb, 0x3c);
  ps_write_reg(0x00b2, 0x09);
  ps_write_reg(0x00ca, 0x09);
  ps_write_reg(0x0198, 0x01);
  ps_write_reg(0x01b0, 0x17);
  ps_write_reg(0x01ad, 0x00);
  ps_write_reg(0x00ff, 0x05);
  ps_write_reg(0x0100, 0x05);
  ps_write_reg(0x0199, 0x05);
  ps_write_reg(0x01a6, 0x1b);
  ps_write_reg(0x01ac, 0x3e);
  ps_write_reg(0x01a7, 0x1f);
  ps_write_reg(0x0030, 0x00);
  ps_write_reg(SYSTEM__MODE_GPIO1, 0x10); // Enables polling for ‘New Sample ready’ when measurement completes
  ps_write_reg(READOUT__AVERAGING_SAMPLE_PERIOD, 0x30); // Set the averaging sample period (compromise between lower noise and increased execution time)
  ps_write_reg(SYSALS__ANALOGUE_GAIN, 0x46); // Sets the light and dark gain (upper nibble). Dark gain should not be changed.
  ps_write_reg(SYSRANGE__VHV_REPEAT_RATE, 0xFF); // sets the # of range measurements after which auto calibration of system is performed
  ps_write_reg(SYSALS__INTEGRATION_PERIOD, 0x63); // Set ALS integration time to 100ms
  ps_write_reg(SYSRANGE__VHV_RECALIBRATE, 0x01); // perform a single temperature calibration of the ranging sensor
  ps_write_reg(SYSRANGE__INTERMEASUREMENT_PERIOD, 0x09); // Set default ranging inter-measurement period to 100ms
  ps_write_reg(SYSALS__INTERMEASUREMENT_PERIOD, 0x31); // Set default ALS inter-measurement period to 500ms
  ps_write_reg(SYSTEM__INTERRUPT_CONFIG_GPIO, 0x24); // Configures interrupt on
  ps_write_reg(SYSTEM__FRESH_OUT_OF_RESET, 0x00); //change fresh out of set status to 0
}

