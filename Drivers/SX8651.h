///////////////////////////////////////////////////////////////////////////////
//                                  SX8651.h                                 //
//                                                                           //
// Header file for SX8651 Resistive Touchscreen Controller driver.  See      //
// SX8651.c for API and function descriptions.                               //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
//             (C) Copyright 1996, 2015 Custom Computer Services             //
//   This source code may only be used by licensed users of the CCS C        //
//   compiler.  This source code may only be distributed to other licensed   //
//   users of the CCS C compiler.  No other use, reproduction or             //
//   distribution is permitted without written permission.  Derivative       //
//   programs created using this software in object code form are not        //
//   restricted in any way.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef _SX8651_H_
#define _SX8651_H_

#include <stdint.h>

#ifndef TOUCH_ADDRESS
 #define TOUCH_ADDRESS  0x90
#endif

#ifndef TOUCH_SCL
 #define TOUCH_SCL   PIN_A2
#endif

#ifndef TOUCH_SDA
 #define TOUCH_SDA   PIN_A3
#endif

#ifndef TOUCH_BAUD
 #define TOUCH_BAUD  400000
#endif

#ifdef TOUCH_USE_HW_I2C
 #use I2C(MASTER, SCL=TOUCH_SCL, SDA=TOUCH_SDA, FAST=TOUCH_BAUD, stream=TOUCH_STREAM, FORCE_HW)
#else
 #use I2C(MASTER, SCL=TOUCH_SCL, SDA=TOUCH_SDA, FAST=TOUCH_BAUD, stream=TOUCH_STREAM, FORCE_SW)
#endif

#ifdef TOUCH_INT
 #define touch_Ready()        (input(TOUCH_INT) == FALSE)
 #define TouchInitINT()       output_float(TOUCH_INT)
#endif

// Touch Register Addresses
#define TOUCH_ADDR_CTRL0         0x00
#define TOUCH_ADDR_CTRL1         0x01
#define TOUCH_ADDR_CTRL2         0x02
#define TOUCH_ADDR_CTRL3         0x03
#define TOUCH_ADDR_CHAN_MAK      0x04
#define TOUCH_ADDR_STAT          0x05
#define TOUCH_ADDR_RESET         0x1F

// Touch Commands
#define TOUCH_CMD_WRITE          0x00
#define TOUCH_CMD_READ           0x40

// Touch Mode Defines
#define TOUCH_MODE_SELECT_CHAN   0x80
#define TOUCH_MODE_CONV_CHAN     0x90
#define TOUCH_MODE_MANAUTO       0xB0
#define TOUCH_MODE_PENDET        0xC0
#define TOUCH_MODE_PENTRIG       0xE0

// Touch Rate Defines
#define TOUCH_RATE_DISABLED      0x00
#define TOUCH_RATE_10_CPS        0x10
#define TOUCH_RATE_20_CPS        0x20
#define TOUCH_RATE_40_CPS        0x30
#define TOUCH_RATE_60_CPS        0x40
#define TOUCH_RATE_80_CPS        0x50
#define TOUCH_RATE_100_CPS       0x60
#define TOUCH_RATE_200_CPS       0x70
#define TOUCH_RATE_300_CPS       0x80
#define TOUCH_RATE_400_CPS       0x90
#define TOUCH_RATE_500_CPS       0xA0
#define TOUCH_RATE_1K_CPS        0xB0
#define TOUCH_RATE_2K_CPS        0xC0
#define TOUCH_RATE_3K_CPS        0xD0
#define TOUCH_RATE_4K_CPS        0xE0
#define TOUCH_RATE_5K_CPS        0xF0

// Touch Conversion Time Defines
#define TOUCH_CONV_TIME_0        0x00  // Immediate (0.5 us)
#define TOUCH_CONV_TIME_1        0x01  // 1.1 us
#define TOUCH_CONV_TIME_2        0x02  // 2.2 us
#define TOUCH_CONV_TIME_3        0x03  // 4.4 us
#define TOUCH_CONV_TIME_4        0x04  // 8.9 us
#define TOUCH_CONV_TIME_5        0x05  // 17.8 us
#define TOUCH_CONV_TIME_6        0x06  // 35.5 us
#define TOUCH_CONV_TIME_7        0x07  // 71.0 us
#define TOUCH_CONV_TIME_8        0x08  // 0.14 ms
#define TOUCH_CONV_TIME_9        0x09  // 0.28 ms
#define TOUCH_CONV_TIME_10       0x0A  // 0.57 ms
#define TOUCH_CONV_TIME_11       0x0B  // 1.14 ms
#define TOUCH_CONV_TIME_12       0x0C  // 2.27 ms
#define TOUCH_CONV_TIME_13       0x0D  // 4.55 ms
#define TOUCH_CONV_TIME_14       0x0E  // 9.09 ms
#define TOUCH_CONV_TIME_15       0x0F  // 18.19 ms

// Touch Auxiliary Defines
#define TOUCH_AUX_ANALOG         0x00  // AUX is used as an analog input
#define TOUCH_AUX_RE_START       0x40  // On rising AUX edge, wait conversion time and start acquisition
#define TOUCH_AUX_FE_START       0x80  // On falling AUX edge, wait conversion time and start acquisition
#define TOUCH_AUX_BE_START       0xC0  // On rising and falling edges, wait conversion time and start acquisition

// Touch IRQ Defines
#define TOUCH_IRQ_ALWAYS         0x00
#define TOUCH_IRQ_PEN_DETECT     0x20

// Touch Pen Resistor Refines
#define TOUCH_PEN_RESISTOR_100K  0x00
#define TOUCH_PEN_RESISTOR_200K  0x04
#define TOUCH_PEN_RESISTOR_50K   0x08
#define TOUCH_PEN_RESISTOR_25K   0x0C

// Touch Filter Defines
#define TOUCH_FILTER_DISABLED    0x00
#define TOUCH_FILTER_3_SAMP_AVG  0x01
#define TOUCH_FILTER_5_SAMP_AVG  0x02
#define TOUCH_FILTER_7_SAMP_AQU  0x03  // 7 sample acquisition, sort, average 3 middle samples

// Touch Settling Time Defines
#define TOUCH_SETTLING_TIME_0    0x00  // Immediate (0.5 us)
#define TOUCH_SETTLING_TIME_1    0x01  // 1.1 us
#define TOUCH_SETTLING_TIME_2    0x02  // 2.2 us
#define TOUCH_SETTLING_TIME_3    0x03  // 4.4 us
#define TOUCH_SETTLING_TIME_4    0x04  // 8.9 us
#define TOUCH_SETTLING_TIME_5    0x05  // 17.8 us
#define TOUCH_SETTLING_TIME_6    0x06  // 35.5 us
#define TOUCH_SETTLING_TIME_7    0x07  // 71.0 us
#define TOUCH_SETTLING_TIME_8    0x08  // 0.14 ms
#define TOUCH_SETTLING_TIME_9    0x09  // 0.28 ms
#define TOUCH_SETTLING_TIME_10   0x0A  // 0.57 ms
#define TOUCH_SETTLING_TIME_11   0x0B  // 1.14 ms
#define TOUCH_SETTLING_TIME_12   0x0C  // 2.27 ms
#define TOUCH_SETTLING_TIME_13   0x0D  // 4.55 ms
#define TOUCH_SETTLING_TIME_14   0x0E  // 9.09 ms
#define TOUCH_SETTLING_TIME_15   0x0F  // 18.19 ms

// Touch X Panel Resistance Defines
#define TOUCH_XRES_100_187       0x00  //100 to 187 Ohms
#define TOUCH_XRES_188_312       0x01  //188 to 312 Ohms
#define TOUCH_XRES_313_938       0x02  //313 to 938 Ohms
#define TOUCH_XRES_939_1875      0x03  //939 to 1875 Ohms
#define TOUCH_XRES_1876_4375     0x04  //1876 to 4375 Ohms
#define TOUCH_XRES_4376_9375     0x05  //4376 to 9375 Ohms
#define TOUCH_XRES_9376_18780    0x06  //9376 to 18780 Ohms
#define TOUCH_XRES_GT_18780      0x07  //greater than 18780 Ohms

// Touch Y Panel Resistance Defines
#define TOUCH_YRES_100_187       0x00  //100 to 187 Ohms
#define TOUCH_YRES_188_312       0x08  //188 to 312 Ohms
#define TOUCH_YRES_313_938       0x10  //313 to 938 Ohms
#define TOUCH_YRES_939_1875      0x18  //939 to 1875 Ohms
#define TOUCH_YRES_1876_4375     0x20  //1876 to 4375 Ohms
#define TOUCH_YRES_4376_9375     0x28  //4376 to 9375 Ohms
#define TOUCH_YRES_9376_18780    0x30  //9376 to 18780 Ohms
#define TOUCH_YRES_GT_18780      0x38  //greater than 18780 Ohms

// Touch Conversion Channel Defines
#define TOUCH_CONV_CHANNEL_X     0x80
#define TOUCH_CONV_CHANNEL_Y     0x40
#define TOUCH_CONV_CHANNEL_Z1    0x20
#define TOUCH_CONV_CHANNEL_Z2    0x10
#define TOUCH_CONV_CHANNEL_AUX   0x08
#define TOUCH_CONV_CHANNEL_RX    0x04
#define TOUCH_CONV_CHANNEL_RY    0x02

// Touch Channel Defines
#define TOUCH_CHANNEL_X          0x00
#define TOUCH_CHANNEL_Y          0x01
#define TOUCH_CHANNEL_Z1         0x02
#define TOUCH_CHANNEL_Z2         0x03
#define TOUCH_CHANNEL_AUX        0x04
#define TOUCH_CHANNEL_RX         0x05
#define TOUCH_CHANNEL_RY         0x06
#define TOUCH_CHANNEL_SEQ        0x07

// Touch Prototypes
void touch_Init(void);
void touch_WriteRegister(uint8_t addr, uint8_t data);
uint8_t touch_ReadRegister(uint8_t addr);
void touch_SetMode(uint8_t data);
uint16_t touch_ReadChannel(uint8_t channel);
void touch_ReadChannels(uint16_t *ptr, uint8_t count);
#ifndef TOUCH_INT
int1 touch_Ready(void);
#endif

#endif
