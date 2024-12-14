///////////////////////////////////////////////////////////////////////////////
//                                 S1D13517.h                                //
//                                                                           //
// Header file for S1D13517 Display Controller driver.  See S1D13517.c for   //
// API and function descriptions.                                            //
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

#ifndef _S1D13517_H_
#define _S1D13517_H_

#include <stdint.h>

#ifndef concat
 #define concat(x,y)  x####y
#endif

/////////////////////////////////// Settings //////////////////////////////////

#ifndef GLCD_PIXELS
 #define GLCD_PIXELS             800
#endif

#ifndef GLCD_LINES
 #define GLCD_LINES              480
#endif

#ifndef GLCD_H_PERIOD
 #define GLCD_H_PERIOD           1056
#endif

#ifndef GLCD_H_SYNC_LEVEL
 #define GLCD_H_SYNC_LEVEL       0
#endif

#ifndef GLCD_H_SYNC_WIDTH
 #define GLCD_H_SYNC_WIDTH       40
#endif

#ifndef GLCD_H_FRONT_PORCH
 #define GLCD_H_FRONT_PORCH      210
#endif

#ifndef GLCD_V_PERIOD
 #define GLCD_V_PERIOD           525
#endif

#ifndef GLCD_V_SYNC_LEVEL
 #define GLCD_V_SYNC_LEVEL       0
#endif

#ifndef GLCD_V_SYNC_WIDTH
 #define GLCD_V_SYNC_WIDTH       20
#endif

#ifndef GLCD_V_FRONT_PORCH
 #define GLCD_V_FRONT_PORCH      22
#endif

#ifndef GLCD_CLK_POLARITY
 #define GLCD_CLK_POLARITY       0  //0 - data is output on rising edge of PCLCK, 1 - data is output on falling edge of PCLK
#endif

#ifndef GLCD_TE_INT_OUTPUT
 #define GLCD_TE_INT_OUTPUT      1  //0 - HNDP, 1 - VNDP, 2 - HNDP and VNDP, 3 - INT
#endif

#ifndef GLCD_CLK_D_DIVIDER
 #define GLCD_CLK_D_DIVIDER      8  //Divisor (2-64) to get input clock between 1MHz and 2MHz
#endif

#ifndef GLCD_CLK_PLL_VCO_RANGE
 #define GLCD_CLK_PLL_VCO_RANGE  0  //Output range of the frequency of the VCO, 0 - 50-100MHz, 4 - 102-140MHz, 5 - 142-180MHz
#endif

#ifndef GLCD_CLK_PLL_DIV_BY_2
 #define GLCD_CLK_PLL_DIV_BY_2   FALSE
#endif

#ifndef GLCD_CLK_N_DIVIDER
 #define GLCD_CLK_N_DIVIDER      45
#endif

#ifndef GLCD_SYSCLK_DIVIDER
 #define GLCD_SYSCLK_DIVIDER     0  //SDCLK:SYSCLK ratio: 0 - 3:1, 1 - 2:1
#endif

#ifndef GLCD_SRAM_SIZE
 #define GLCD_SRAM_SIZE          2  //0 - 16 MBit, 2 - 64 MBit, 3 - 128 MBit
#endif

#ifndef GLCD_SRAM_REFRESH_RATE
 #define GLCD_SRAM_REFRESH_RATE  351
#endif

#ifndef GLCD_SRAM_BUFFER_SIZE
 #define GLCD_SRAM_BUFFER_SIZE   0x50  //0x20 - HVGA (512 KByte), 0x40 - VGA (1 MByte), 0x50 - WVGA (1.25 MByte), 0x60 - SVGA (1.5 MByte), 0x70 - QHD (1.75 MByte)
#endif

#ifndef GLCD_SDCLK
 #define GLCD_SDCLK              90000000
#endif

#ifndef GLCD_CS
 #define GLCD_CS                 PIN_F4
#endif

#ifndef GLCD_RE
 #define GLCD_RE                 PIN_F5
#endif

#ifndef GLCD_WE
 #define GLCD_WE                 PIN_F8
#endif

#ifndef GLCD_RESET
 #define GLCD_RESET              PIN_F13
#endif

#ifndef GLCD_DC
 #define GLCD_DC                 PIN_F0
#endif

#ifndef GLCD_DATA_PORT
 #define GLCD_DATA_PORT          d
#endif

#define MINIMUM_WRITE_WIDTH      8

//////////////////////////////////// Macros ///////////////////////////////////

#define GLCDReadPort()      concat(input_,GLCD_DATA_PORT())
#define GLCDWritePort(data) concat(output_,GLCD_DATA_PORT(data))
#define GLCDDataInput()     concat(set_tris_,GLCD_DATA_PORT)(0xFFFF)
#define GLCDDataOutput()    concat(set_tris_,GLCD_DATA_PORT)(0)

#define GLCDInitCS()        {output_high(GLCD_CS); output_drive(GLCD_CS);}
#define GLCDInitRE()        {output_high(GLCD_RE); output_drive(GLCD_RE);}
#define GLCDInitWE()        {output_high(GLCD_WE); output_drive(GLCD_WE);}
#ifdef GLCD_INIT
 #define GLCDInitINT()      output_float(GLCD_INT)
#endif
#define GLCDInitRESET()     {output_high(GLCD_RESET); output_drive(GLCD_RESET);}
#define GLCDInitDC()        {output_low(GLCD_DC); output_drive(GLCD_DC);}

#define GLCD_COMMAND()      output_low(GLCD_DC);
#define GLCD_DATA()         output_high(GLCD_DC);

/////////////////////////// Register Address Defines //////////////////////////

//System Configuration Registers
#define GLCD_SYS_CONFIG_PRODUCT_CODE             0x00
#define GLCD_SYS_CONFIG_READBACK                 0x02

//Clock Configuration Registers
#define GLCD_CLK_CONFIG_PLL_D_DIVIDER            0x04
#define GLCD_CLK_CONFIG_PLL_0                    0x06
#define GLCD_CLK_CONFIG_PLL_1                    0x08
#define GLCD_CLK_CONFIG_PLL_2                    0x0A
#define GLCD_CLK_CONFIG_PLL_N_DIVIDER            0x0C
#define GLCD_CLK_CONFIG_PLL_SS_CONTROL0          0x0E
#define GLCD_CLK_CONFIG_PLL_SS_CONTROL1          0x10
#define GLCD_CLK_CONFIG_PLL_CLK_SOURCE           0x12

//Panel Configuration Registers
#define GLCD_PANEL_TYPE                          0x14
#define GLCD_H_DISPLAY_WIDTH                     0x16
#define GLCD_H_NON_DISPLAY_PERIOD                0x18
#define GLCD_V_DISPLAY_HEIGHT_0                  0x1A
#define GLCD_V_DISPLAY_HEIGHT_1                  0x1C
#define GLCD_V_NON_DISPLAY_PERIOD                0x1E
#define GLCD_PHS_PULSE_WIDTH                     0x20
#define GLCD_PHS_PULSE_START_POS                 0x22
#define GLCD_PVS_PULSE_WIDTH                     0x24
#define GLCD_PVS_PULSE_START_POS                 0x26
#define GLCD_PCLK_POLARITY                       0x28

//Display Mode Registers
#define GLCD_DISPLAY_MODE                        0x2A
#define GLCD_PIP1_START_ADDR_0                   0x2C
#define GLCD_PIP1_START_ADDR_1                   0x2E
#define GLCD_PIP1_START_ADDR_2                   0x30
#define GLCD_PIP1_X_START                        0x32
#define GLCD_PIP1_Y_START_0                      0x34
#define GLCD_PIP1_Y_START_1                      0x36
#define GLCD_PIP1_X_END                          0x38
#define GLCD_PIP1_Y_END_0                        0x3A
#define GLCD_PIP1_Y_END_1                        0x3C
#define GLCD_PIP2_START_ADDR_0                   0x3E
#define GLCD_PIP2_START_ADDR_1                   0x40
#define GLCD_PIP2_START_ADDR_2                   0x42
#define GLCD_PIP2_X_START                        0x44
#define GLCD_PIP2_Y_START_0                      0x46
#define GLCD_PIP2_Y_START_1                      0x48
#define GLCD_PIP2_X_END                          0x4A
#define GLCD_PIP2_Y_END_0                        0x4C
#define GLCD_PIP2_Y_END_1                        0x4E
#define GLCD_DISPLAY_CONTROL                     0x50

//Input Mode Registers
#define GLCD_INPUT_MODE                          0x52
#define GLCD_TRANS_KEY_COLOR_RED                 0x54
#define GLCD_TRANS_KEY_COLOR_GREEN               0x56
#define GLCD_TRANS_KEY_COLOR_BLUE                0x58
#define GLCD_WRITE_X_START_POS                   0x5A
#define GLCD_WRITE_Y_START_POS_0                 0x5C
#define GLCD_WRITE_Y_START_POS_1                 0x5E
#define GLCD_WRITE_X_END_POS                     0x60
#define GLCD_WRITE_Y_END_POS_0                   0x62
#define GLCD_WRITE_Y_END_POS_1                   0x64

//Memory Access Registers
#define GLCD_MEMORY_DATA_PORT_0                  0x66
#define GLCD_MEMORY_DATA_PORT_1                  0x67

//Miscellaneous Registers
#define GLCD_POWER_SAVE                          0x68
#define GLCD_NON_DISPLAY_PERIOD_CONTROL          0x6A

//General Purpose Output Pins
#define GLCD_GENERAL_PURPOSE_OUTPUT_0            0x6C
#define GLCD_GENERAL_PURPOSE_OUTPUT_1            0x6E

//PWM Registers
#define GLCD_PWM_CONTROL                         0x70
#define GLCD_PWM_HIGH_DUTY_CYCLE_0               0x72
#define GLCD_PWM_HIGH_DUTY_CYCLE_1               0x74
#define GLCD_PWM_HIGH_DUTY_CYCLE_2               0x76
#define GLCD_PWM_HIGH_DUTY_CYCLE_3               0x78
#define GLCD_PWM_LOW_DUTY_CYCLE_0                0x7A
#define GLCD_PWM_LOW_DUTY_CYCLE_1                0x7C
#define GLCD_PWM_LOW_DUTY_CYCLE_2                0x7E
#define GLCD_PWM_LOW_DUTY_CYCLE_3                0x80

//SDRAM Control Register
#define GLCD_SDRAM_CONTROL                       0x82
#define GLCD_SDRAM_STATUS_0                      0x84
#define GLCD_SDRAM_STATUS_1                      0x86
#define GLCD_SDRAM_MRS_VALUE_0                   0x88
#define GLCD_SDRAM_MRS_VALUE_1                   0x8A
#define GLCD_SDRAM_REFRESH_COUNTER_0             0x8C
#define GLCD_SDRAM_REFRESH_COUNTER_1             0x8E
#define GLCD_SDRAM_WRITE_BUFFER_SIZE_0           0x90
#define GLCD_SDRAM_WRITE_BUFFER_SIZE_1           0x92

//Alpha-Blend Registers
#define GLCD_ALPHA_BLEND_CONTROL                 0x94
#define GLCD_ALPHA_BLEND_H_SIZE                  0x98
#define GLCD_ALPHA_BLEND_V_SIZE_0                0x9A
#define GLCD_ALPHA_BLEND_V_SIZE_1                0x9C
#define GLCD_ALPHA_BLEND_VALUE                   0x9E
#define GLCD_ALPHA_BLEND_INPUT_IMAGE1_START_0    0xA0
#define GLCD_ALPHA_BLEND_INPUT_IMAGE1_START_1    0xA2
#define GLCD_ALPHA_BLEND_INPUT_IMAGE1_StART_2    0xA4
#define GLCD_ALPHA_BLEND_INPUT_IMAGE2_START_0    0xA6
#define GLCD_ALPHA_BLEND_INPUT_IMAGE2_START_1    0xA8
#define GLCD_ALPHA_BLEND_INPUT_IMAGE2_START_2    0xAA
#define GLCD_ALPHA_BLEND_OUTPUT_IMAGE_START_0    0xAC
#define GLCD_ALPHA_BLEND_OUTPUT_IMAGE_START_1    0xAE
#define GLCD_ALPHA_BLEND_OUTPUT_IMAGE_START_2    0xB0

//Interrupt Registers
#define GLCD_INT_CONTROL                         0xB2
#define GLCD_INT_STATUS                          0xB4
#define GLCD_INT_CLEAR                           0xB6

///////////////////////////// Function Prototypes /////////////////////////////

void glcd_Init(void);
void glcd_InitHW(void);
void glcd_InitClock(void);

uint8_t glcd_ReadRegister(uint8_t address);
void glcd_WriteRegister(uint8_t address, uint8_t data, int1 doCS=TRUE);

#inline
uint16_t glcd_ReadPort(void);
#inline
void glcd_WritePort(uint16_t data);
#inline
void glcd_WritePortRegister(uint16_t data);
#inline
void glcd_TransferData(void);

void glcd_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
void glcd_DrawPixels(uint16_t StartX, uint16_t StartY, uint16_t Width, uint16_t Height, uint16_t *Data);

#endif
