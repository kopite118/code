///////////////////////////////////////////////////////////////////////////////
//                                 S1D13517.c                                //
//                                                                           //
// A drive for S1D13517 Display Controller.  The driver provides a functions //
// to initialize the controller, to write and read registers and to draw a   //
// pixel or block of pixels.                                                 //
//                                                                           //
// The driver will work on the PCD compiler.  It requires an entire 16 bit   //
// port to transfer data, and that port set to use fast_io.                  //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
//                                    API                                    //
///////////////////////////////////////////////////////////////////////////////
// Functions:                                                                //
//                                                                           //
//    glcd_Init(void)                                                        //
//       - Initializes the controller hardware and sets up up the controller //
//         to the specified setting.                                         //
//                                                                           //
//    glcd_InitHW(void)                                                      //
//       - Initializes the controller hardware, is called by the glcd_Init() //
//         function.                                                         //
//                                                                           //
//    glcd_InitClock(void)                                                   //
//       - Initializes the controller clock, is called by the glcd_Init()    //
//         function.                                                         //
//                                                                           //
//    uint8_t glcd_ReadRegister(uint8_t address)                             //
//       - Reads one of the controller's control registers specified by      //
//         address.                                                          //
//                                                                           //
//    glcd_WriteRegister(uint8_t address, uint8_t data, int1 doCS=TRUE)      //
//       - Write one of the controller's control register specified by       //
//         address, with data.  doCS specifies whether to toggle GLCD_CS     //
//         pin, useful when writing multiple registers back to back.         //
//                                                                           //
//    uint16_t glcd_ReadPort(void)                                           //
//       - Reads data from Port, doesn't make port an input or set GLCD_DC   //
//         line for data mode.  Must call GLCDDataInput() and GLCD_DATA()    //
//         functions before glcd_ReadPort().                                 //
//                                                                           //
//    glcd_WritePort(uint16_t data)                                          //
//       - Write data to Port, doesn't make port an output or set GLCD_DC    //
//         line for data mode.  Must call GLCDDataOutput() and GLCD_DATA()   //
//         functions before glcd_WritePort().                                //
//                                                                           //
//    glcd_WritePortRegister(uint16_t data)                                  //
//       - Writes register data to Port, doesn't make port an output or set  //
//         GLCD_DC line for data or command mode.  Must call                 //
//         GLCDDataOutput() and GLCD_COMMAND() or GLCD_DATA() functions      //
//         glcd_WritePortRegister().                                         //
//                                                                           //
//    glcd_TransferData(void)                                                //
//       - Write data already on Port to display controller, only toggles    //
//         GLCD_WE pin with the appropriate delays between and after.        //
//         Useful in situations like reading data directly from an external  //
//         flash to the display.                                             //
//                                                                           //
//    glcd_DrawPixel(uint16_t x, uint16_t y, uint16_t color)                 //
//       - Draws a pixel with the specified color to coordinate (x,y) on the //
//         display.  Coordinate (0,0) is always in the top left hand corner  //
//         of display.                                                       //
//                                                                           //
//    glcd_DrawPixels(uint16_t StartX, uint16_t StartY, uint16_t Width,      //
//                    uint16_t Height, uint16_t *Data)                       //
//       - Draw a block of pixels with specified Width and Height starting   //
//         at coordinates (StartX, StartY).  Data is a pointer to the pixels //
//         to be displayed.  Data must be organized from left to right, top  //
//         to bottom.                                                        //
//                                                                           //
// Defines:                                                                  //
//                                                                           //
//    GLCD_PIXELS                                                            //
//       - Specifies the number of pixels on the x axis of display, default  //
//         is 800.                                                           //
//                                                                           //
//    GLCD_LINES                                                             //
//       - Specifies the number of lines of the display, i.e. pixels on the  //
//         y axis, default is 480.                                           //
//                                                                           //
//    GLCD_H_PERIOD                                                          //
//       - Specifies the Horizontal Display Width of display, default is     //
//         1056.                                                             //
//                                                                           //
//    GLCD_H_SYNC_LEVEL                                                      //
//       - Specifies the polarity of the horizontal sync signal, default is  //
//         0, active low.                                                    //
//                                                                           //
//    GLCD_H_SYNC_WIDTH                                                      //
//       - Specifies the width of the horizontal sync signal, default is 40. //
//                                                                           //
//    GLCD_H_FRONT_PORCH                                                     //
//       - Specifies the start position of the horizontal sync signal,       //
//         default is 210.                                                   //
//                                                                           //
//    GLCD_V_PERIOD                                                          //
//       - Specifies the Vertical Display Width of display, default is 525.  //
//                                                                           //
//    GLCD_V_SYNC_LEVEL                                                      //
//       - Specifies the polarity of the vertical sync signal, default is 0, //
//         active low.                                                       //
//                                                                           //
//    GLCD_V_SYNC_WIDTH                                                      //
//       - Specifies the width of the vertical sync signal, default is 20.   //
//                                                                           //
//    GLCD_V_FRONT_PORCH                                                     //
//       - Specifies the start position of the vertical sync signal, default //
//         is 22.                                                            //
//                                                                           //
//    GLCD_CLK_POLARITY                                                      //
//       - Specifies the polarity of the PCLK, pixel clock, default is 0,    //
//         data is outputted on the rising edge of PCLK.                     //
//                                                                           //
//    GLCD_TE_INT_OUTPUT                                                     //
//       - Specifies what is outputted on the GLCD_INT pin, default is 1,    //
//         Vertical Non-Display Period.                                      //
//                                                                           //
//    GLCD_CLK_D_DIVIDER                                                     //
//       - Specifies the divide ratio between the input clock and the input  //
//         clock to the PLL, default is 4, a ratio of 5:1                    //
//                                                                           //
//    GLCD_CLK_PLL_VCO_RANGE                                                 //
//       - Specifies the output range of the VCO, default is 0, a range of   //
//         50 to 100 MHz.                                                    //
//                                                                           //
//    GLCD_CLK_PLL_DIV_BY_2                                                  //
//       - Specifies whether the PLL output is divided by 2, default is 0,   //
//         PLL is not divided.                                               //
//                                                                           //
//    GLCD_CLK_N_DIVIDER                                                     //
//       - Specifies the value PLL input clock to get the PLL output clock,  //
//         default is 45.                                                    //
//                                                                           //
//    GLCD_SYSCLK_DIVIDER                                                    //
//       - Specifies the divide ratio of the internal system clock (SYSCLK)  //
//         from the SDRAM clock (SDCLK), default is 0, a ratio of 1:3.       //
//                                                                           //
//    GLCD_SRAM_SIZE                                                         //
//       - Specifies the SDRAM memory size, default is 2, 64 MBit.           //
//                                                                           //
//    GLCD_SRAM_REFRESH_RATE                                                 //
//       - Specifies the value of the SDRAM refresh counter, default is 351. //
//                                                                           //
//    GLCD_SRAM_BUFFER_SIZE                                                  //
//       - Specifies the buffer memory size of the SDRAM, default is 0x50,   //
//         1.25 M byte.                                                      //
//                                                                           //
//    GLCD_SDCLK                                                             //
//       - Specifies the frequency of the SDCLK, default is 90000000.  This  //
//         used by the driver to determine delays to meet timing             //
//         specification of the display controller.                          //
//                                                                           //
//    GLCD_CS                                                                //
//       - Specifies the PIC pin connected to the display controller's CS    //
//         pin, default is PIN_F4.                                           //
//                                                                           //
//    GLCD_RE                                                                //
//       - Specifies the PIC pin connected to the display controller's RE    //
//         pin, default is PIN_F5.                                           //
//                                                                           //
//    GLCD_WE                                                                //
//       - Specifies the PIC pin connected to the display controller's WE    //
//         pin, default is PIN_F8.                                           //
//                                                                           //
//    GLCD_RESET                                                             //
//       - Specifies the PIC pin connected to the display controller's RESET //
//         pin, default is PIN_F13.                                          //
//                                                                           //
//    GLCD_DC                                                                //
//       - Specifies the PIC pin connected to the display controller's RESET //
//         pin, default is PIN_F0.                                           //
//                                                                           //
//    GLCD_INIT                                                              //
//       - Specifies the PIC pin connected to the display controller's INT   //
//         pin, no default setting is made.                                  //
//                                                                           //
//    GLCD_DATA_PORT                                                         //
//       - Specifies the PIC port that is used to write and read data with   //
//         to and from the display controller, default is d.                 //
//                                                                           //
//    MINIMUM_WRITE_WIDTH                                                    //
//       - Specifies the minimum write width of the S1D13517.  Is set to 8,  //
//         value should not be changed.                                      //
//                                                                           //
// Macros:                                                                   //
//                                                                           //
//    data = GLCDReadPort()                                                  //
//       - Inputs data from the PIC port.                                    //
//                                                                           //
//    GLCDWritePort(data)                                                    //
//       - Writes data to the PIC port.                                      //
//                                                                           //
//    GLCDDataInput()                                                        //
//       - Makes PIC port pins inputs.                                       //
//                                                                           //
//    GLCDDataOutput()                                                       //
//       - Makes PIC port pins outputs.                                      //
//                                                                           //
//    GLCDInitCS()                                                           //
//       - Initializes the GLCD_CS pin, called by glcd_InitHW() function.    //
//                                                                           //
//    GLCDInitRE()                                                           //
//       - Initializes the GLCD_RE pin, called by glcd_InitHW() function.    //
//                                                                           //
//    GLCDInitWE()                                                           //
//       - Initializes the GLCD_WE pin, called by glcd_InitHW() function.    //
//                                                                           //
//    GLCDInitINT()                                                          //
//       - Initializes the GLCD_INT pin, called by glcd_InitHW() function.   //
//         Only available if GLCD_INT is defined.                            //
//                                                                           //
//    GLCDInitRESET()                                                        //
//       - Initializes the GLCD_RESET pin, called by glcd_InitHW() function. //
//                                                                           //
//    GLCDInitDC()                                                           //
//       - Initializes the GLCD_DC pin, called by glcd_InitHW() function.    //
//                                                                           //
//    GLCD_COMMAND()                                                         //
//       - Sets the GLCD_DC pin to command level.                            //
//                                                                           //
//    GLCD_DATA()                                                            //
//       - Set the GLCD_DC pin to data level.                                //
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

#include "S1D13517.h"

///////////////////////////////////////////////////////////////////////////////
// glcd_Init()
//
// Initializes the S1D13517 Display Controller.  
//
///////////////////////////////////////////////////////////////////////////////
void glcd_Init(void)
{
   uint8_t Status;

   glcd_InitHW();
   
   output_low(GLCD_RESET);
   delay_ms(2);
   output_high(GLCD_RESET);
   output_low(GLCD_RESET);
   output_high(GLCD_RESET);   
   
   glcd_InitClock();
   
   glcd_WriteRegister(GLCD_PANEL_TYPE, 0x04);                  //16bpp (RGB 5:6:5)
   glcd_WriteRegister(GLCD_H_DISPLAY_WIDTH, ((GLCD_PIXELS / 8) - 1));
   glcd_WriteRegister(GLCD_H_NON_DISPLAY_PERIOD, (((GLCD_H_PERIOD - GLCD_PIXELS) / 2) - 1));
   glcd_WriteRegister(GLCD_V_DISPLAY_HEIGHT_0, (GLCD_LINES - 1));
   glcd_WriteRegister(GLCD_V_DISPLAY_HEIGHT_1, (((uint16_t)(GLCD_LINES - 1)) >> 8));
   glcd_WriteRegister(GLCD_V_NON_DISPLAY_PERIOD, (((GLCD_V_PERIOD - GLCD_LINES) / 2) - 1));
   glcd_WriteRegister(GLCD_PHS_PULSE_WIDTH, (((uint8_t)GLCD_H_SYNC_LEVEL << 7) | (GLCD_H_SYNC_WIDTH - 1)));
   glcd_WriteRegister(GLCD_PHS_PULSE_START_POS, GLCD_H_FRONT_PORCH);
   glcd_WriteRegister(GLCD_PVS_PULSE_WIDTH, (((uint8_t)GLCD_V_SYNC_LEVEL << 7) | (GLCD_V_SYNC_WIDTH - 1)));
   glcd_WriteRegister(GLCD_PVS_PULSE_START_POS, GLCD_V_FRONT_PORCH);
   glcd_WriteRegister(GLCD_PCLK_POLARITY, ((uint8_t)GLCD_CLK_POLARITY << 7));
   glcd_WriteRegister(GLCD_NON_DISPLAY_PERIOD_CONTROL, GLCD_TE_INT_OUTPUT);
   
   glcd_WriteRegister(GLCD_SDRAM_CONTROL, GLCD_SRAM_SIZE);
   glcd_WriteRegister(GLCD_SDRAM_REFRESH_COUNTER_0, GLCD_SRAM_REFRESH_RATE);
   glcd_WriteRegister(GLCD_SDRAM_REFRESH_COUNTER_1, ((uint16_t)GLCD_SRAM_REFRESH_RATE >> 8));
   glcd_WriteRegister(GLCD_SDRAM_WRITE_BUFFER_SIZE_0, GLCD_SRAM_BUFFER_SIZE);
   
   glcd_WriteRegister(GLCD_POWER_SAVE, 0xE8);                  //Software Reset
   glcd_WriteRegister(GLCD_POWER_SAVE, 0x00);
   
   glcd_WriteRegister(GLCD_POWER_SAVE, 0x01);                  //enable SDCLK
   
   glcd_WriteRegister(GLCD_SDRAM_STATUS_0, 0x82);              //Initialize SDRAM
   
   do
   {
      Status = glcd_ReadRegister(GLCD_SDRAM_STATUS_1);
   } while(bit_test(Status, 1) == 0);
   
   glcd_WriteRegister(GLCD_DISPLAY_MODE, 0x00);                //Single Buffer Display
   glcd_WriteRegister(GLCD_DISPLAY_CONTROL, 0x80);             //Save Change   
   glcd_WriteRegister(GLCD_DISPLAY_MODE, 0x01);                //Enable Display
   
   delay_ms(10);                                         //This is necessary so that it will display things
                                                         //correctly if you try to write to the display
                                                         //immediately after initializing, wasn't documented
                                                         //anywhere discovered by experimentation.
}

///////////////////////////////////////////////////////////////////////////////
// glcd_InitHW()
//
// Initializes the S1D13517 Display Controller's hardware pins.
//
///////////////////////////////////////////////////////////////////////////////
void glcd_InitHW(void)
{
   GLCDInitRESET();

   GLCDInitCS();
   GLCDInitRE();  
   GLCDInitWE();
  #ifdef GLCD_INIT
   GLCDInitINT(); 
  #endif
   GLCDInitDC();
}

///////////////////////////////////////////////////////////////////////////////
// glcd_InitClock()
//
// Initializes the S1D13517 Display Controller's clock.
//
///////////////////////////////////////////////////////////////////////////////
void glcd_InitClock(void)
{
   glcd_WriteRegister(GLCD_CLK_CONFIG_PLL_D_DIVIDER, (GLCD_CLK_D_DIVIDER - 1));
   glcd_WriteRegister(GLCD_CLK_CONFIG_PLL_0, ((GLCD_CLK_PLL_VCO_RANGE << 4) | 0x01));
   glcd_WriteRegister(GLCD_CLK_CONFIG_PLL_1, GLCD_CLK_PLL_DIV_BY_2);
   glcd_WriteRegister(GLCD_CLK_CONFIG_PLL_N_DIVIDER, (GLCD_CLK_N_DIVIDER - 1));
   glcd_WriteRegister(GLCD_CLK_CONFIG_PLL_CLK_SOURCE, ((uint8_t)GLCD_SYSCLK_DIVIDER << 4));
   glcd_WriteRegister(GLCD_CLK_CONFIG_PLL_D_DIVIDER, (0x80 | (GLCD_CLK_D_DIVIDER - 1)));
   
   delay_ms(10);
   
   glcd_WriteRegister(GLCD_CLK_CONFIG_PLL_CLK_SOURCE, 0x80 | ((uint8_t)GLCD_SYSCLK_DIVIDER << 4));
}

///////////////////////////////////////////////////////////////////////////////
// glcd_ReadRegister()
//
// Reads one of the S1D13517 registers.
//
// Parameters:
//    address - The register address to read.  The register addresses are 
//              defined in the S1D13517.h file, search for Register Address
//              Defines.
//
// Returns:
//    uint8_t value read from S1D13517 register.
//  
///////////////////////////////////////////////////////////////////////////////
uint8_t glcd_ReadRegister(uint8_t address)
{
   uint8_t result;
   
   output_low(GLCD_CS);
   
   GLCDDataOutput();
   
   GLCD_COMMAND();
   glcd_WritePortRegister(address);
   
   GLCDDataInput();
   
   GLCD_DATA();
   
   result = glcd_ReadPort();
   
   output_high(GLCD_CS);
   
   return(result);
}

///////////////////////////////////////////////////////////////////////////////
// glcd_WriteRegister()
//
// Writes data to one of the S1D13517 register.
//
// Parameters:
//    address - The register address to write to.  The register addresses are 
//              defined in the S1D13517.h file, search for Register Address
//              Defines.
//
//    data    - The data to write to the register.
//
//    doCS    - Specifies whether to do control the GLCD_CS pin, TRUE pin is 
//              controlled, FALSE pin is not controlled.
//
///////////////////////////////////////////////////////////////////////////////
void glcd_WriteRegister(uint8_t address, uint8_t data, int1 doCS=TRUE)
{
   if(doCS)
      output_low(GLCD_CS);
   
   GLCDDataOutput();
   
   GLCD_COMMAND();   
   glcd_WritePortRegister(address);
   
   GLCD_DATA();
   glcd_WritePortRegister(data);
   
   if(doCS)
      output_high(GLCD_CS);
}

///////////////////////////////////////////////////////////////////////////////
// glcd_ReadPort()
//
// Toggles the GLCD_RE pin to read data from the S1D13517.
//
// Returns:
//    16 bit value read from S1D13517.
//
///////////////////////////////////////////////////////////////////////////////
#inline
uint16_t glcd_ReadPort(void)
{
   uint16_t result;

   output_low(GLCD_RE);
   result = GLCDReadPort();
   output_high(GLCD_RE);
   
   return(result);
}

///////////////////////////////////////////////////////////////////////////////
// glcd_WritePort
//
// Toggles the GLCD_WE pin to write data to the S1D13517.
//
// Parameters:
//    data - uint16_t value to write to the S1D13517.
//
///////////////////////////////////////////////////////////////////////////////
#inline
void glcd_WritePort(uint16_t data)
{
   GLCDWritePort(data);
   
   output_low(GLCD_WE);
  #if getenv("ENH24")
   #if ((((getenv("CLOCK") / 2) / 100000000) - 2) > 0)
    delay_cycles(((getenv("CLOCK") / 2) / 100000000));
   #else
    delay_cycles(1);
   #endif
  #else
   #if ((((getenv("CLOCK") / 2) / 100000000) - 1) > 0)
    delay_cycles(((getenv("CLOCK") / 2) / 100000000) + 1);
   #else
    delay_cycles(1);
   #endif
  #endif
   output_high(GLCD_WE);
   
  #if GLCD_SYSCLK_DIVIDER == 0
   #if getenv("ENH24")
    #if ((((7 * (getenv("CLOCK") / 2)) / GLCD_SDCLK) - ((getenv("CLOCK") / 2) / 100000000) - 2) > 0)
     delay_cycles(((7 * (getenv("CLOCK") / 2)) / GLCD_SDCLK) - ((getenv("CLOCK") / 2) / 100000000));
    #else
     delay_cycles(1);
    #endif
   #else
    #if ((((7 * (getenv("CLOCK") / 2)) / GLCD_SDCLK) - ((getenv("CLOCK") / 2) / 100000000) - 1) > 0)
     delay_cycles(((7 * (getenv("CLOCK") / 2)) / GLCD_SDCLK) - ((getenv("CLOCK") / 2) / 100000000) + 1);
    #else
     delay_cycles(1);
    #endif
   #endif
  #else
   #if getenv("ENH24")
    #if ((((14 * (getenv("CLOCK") / 2)) / GLCD_SDCLK) - ((getenv("CLOCK") / 2) / 100000000) - 2) > 0)
     delay_cycles(((7 * (getenv("CLOCK") / 2)) / GLCD_SDCLK) - ((getenv("CLOCK") / 2) / 100000000));
    #else
     delay_cycles(1);
    #endif
   #else
    #if ((((14 * (getenv("CLOCK") / 2)) / GLCD_SDCLK) - ((getenv("CLOCK") / 2) / 100000000) - 1) > 0)
     delay_cycles(((7 * (getenv("CLOCK") / 2)) / GLCD_SDCLK) - ((getenv("CLOCK") / 2) / 100000000) + 1);
    #else
     delay_cycles(1);
    #endif
   #endif
  #endif
}

///////////////////////////////////////////////////////////////////////////////
// glcd_WritePortRegister()
//
// Toggles the GLCD_WE pin to write data to the S1D13517.  The only difference
// between the glcd_WritePort() and glcd_WritePortRegister() functions is the
// delay time between the toggling the GLCD_WE pin and before exiting function.
// This function is setup to use the timing specifications for writing to one   
// of the registers and glcd_WritePort() is setup to use the timing 
// specifications for writing pixel data to the S1D13517.
//
// Parameters:
//    data - uint16_t value to write to the S1D13517.
//
///////////////////////////////////////////////////////////////////////////////
#inline
void glcd_WritePortRegister(uint16_t data)
{
   GLCDWritePort(data);
   
   output_low(GLCD_WE);
  #if getenv("ENH24")
   #if ((((getenv("CLOCK") / 2) / 100000000) - 2) > 0)
    delay_cycles(((getenv("CLOCK") / 2) / 100000000));
   #else
    delay_cycles(1);
   #endif
  #else
   #if ((((getenv("CLOCK") / 2) / 100000000) - 1) > 0)
    delay_cycles(((getenv("CLOCK") / 2) / 100000000) + 1);
   #else
    delay_cycles(1);
   #endif
  #endif
   output_high(GLCD_WE);
   
  #if getenv("ENH24")
   #if (((((getenv("CLOCK") / 2) / 1000000000) * 30) - 2) > 0)
    delay_cycles((((getenv("CLOCK") / 2) / 1000000000) * 30));
   #else
    delay_cycles(1);
   #endif
  #else
   #if (((((getenv("CLOCK") / 2) / 1000000000) * 30) - 1) > 0)
    delay_cycles((((getenv("CLOCK") / 2) / 1000000000) * 30) + 1);
   #else
    delay_cycles(1);
   #endif
  #endif
}

///////////////////////////////////////////////////////////////////////////////
// glcd_TransferData()
//
// Toggles the GLCD_WE pin to write the data that is currently on the port to
// the S1D13517.  Use the timing specifications for writing pixel data to the
// S1D13517.
//
///////////////////////////////////////////////////////////////////////////////
#inline
void glcd_TransferData(void)
{
   output_low(GLCD_WE);
  #if getenv("ENH24")
   #if ((((getenv("CLOCK") / 2) / 100000000) - 2) > 0)
    delay_cycles(((getenv("CLOCK") / 2) / 100000000));
   #else
    delay_cycles(1);
   #endif
  #else
   #if ((((getenv("CLOCK") / 2) / 100000000) - 1) > 0)
    delay_cycles(((getenv("CLOCK") / 2) / 100000000) + 1);
   #else
    delay_cycles(1);
   #endif
  #endif
   output_high(GLCD_WE);
   
  #if GLCD_SYSCLK_DIVIDER == 0
   #if getenv("ENH24")
    #if ((((7 * (getenv("CLOCK") / 2)) / GLCD_SDCLK) - ((getenv("CLOCK") / 2) / 100000000) - 2) > 0)
     delay_cycles(((7 * (getenv("CLOCK") / 2)) / GLCD_SDCLK) - ((getenv("CLOCK") / 2) / 100000000));
    #else
     delay_cycles(1);
    #endif
   #else
    #if ((((7 * (getenv("CLOCK") / 2)) / GLCD_SDCLK) - ((getenv("CLOCK") / 2) / 100000000) - 1) > 0)
     delay_cycles(((7 * (getenv("CLOCK") / 2)) / GLCD_SDCLK) - ((getenv("CLOCK") / 2) / 100000000) + 1);
    #else
     delay_cycles(1);
    #endif
   #endif
  #else
   #if getenv("ENH24")
    #if ((((14 * (getenv("CLOCK") / 2)) / GLCD_SDCLK) - ((getenv("CLOCK") / 2) / 100000000) - 2) > 0)
     delay_cycles(((7 * (getenv("CLOCK") / 2)) / GLCD_SDCLK) - ((getenv("CLOCK") / 2) / 100000000));
    #else
     delay_cycles(1);
    #endif
   #else
    #if ((((14 * (getenv("CLOCK") / 2)) / GLCD_SDCLK) - ((getenv("CLOCK") / 2) / 100000000) - 1) > 0)
     delay_cycles(((7 * (getenv("CLOCK") / 2)) / GLCD_SDCLK) - ((getenv("CLOCK") / 2) / 100000000) + 1);
    #else
     delay_cycles(1);
    #endif
   #endif
  #endif
}

///////////////////////////////////////////////////////////////////////////////
// glcd_DrawPixel()
//
// Draws a pixel on the display with the specified color at the specified 
// coordinate.  Coordinate (0,0) is in the top left hand corner of the display.
// The minimum write width of the S1D13517 is 8 pixels.  To write a single 
// pixel what the function does is set the desired pixel to the color and the 
// other 7 pixels to iColor (color xor'd with 0xFFFF) and then sets iColor as
// the transparent color of the display and writes all eight pixels.
//
// Parameters:
//    x - the x coordinate to write the pixel.
//
//    y - the y coordinate to write the pixel.
//
//    color - the RBG 565 color to set the pixel to.
//
///////////////////////////////////////////////////////////////////////////////
void glcd_DrawPixel(uint16_t x, uint16_t y, uint16_t color)
{
   uint16_t iColor;
   uint16_t StartX, StopX;
   uint16_t i;
   
   iColor = color ^ 0xFFFF;
   
   if((x % 8) != 0)
      StartX = x - (x % 8);
   else
      StartX = x;
      
   StopX = StartX + 8;
   
   output_low(GLCD_CS);
   
   glcd_WriteRegister(GLCD_WRITE_X_START_POS, StartX / 4, FALSE);
   glcd_WriteRegister(GLCD_WRITE_Y_START_POS_0, (y >> 2), FALSE);
   glcd_WriteRegister(GLCD_WRITE_Y_START_POS_1, (y & 0x03), FALSE);
   
   glcd_WriteRegister(GLCD_WRITE_X_END_POS, StartX / 4, FALSE);
   glcd_WriteRegister(GLCD_WRITE_Y_END_POS_0, (y >> 2), FALSE);
   glcd_WriteRegister(GLCD_WRITE_Y_END_POS_1, (y & 0x03), FALSE);
   
   glcd_WriteRegister(GLCD_INPUT_MODE, 0x08, FALSE);
               
   glcd_WriteRegister(GLCD_TRANS_KEY_COLOR_RED, make8(iColor, 1) & 0xF8, FALSE);
   glcd_WriteRegister(GLCD_TRANS_KEY_COLOR_GREEN, (iColor & 0x07E0) >> 3, FALSE);
   glcd_WriteRegister(GLCD_TRANS_KEY_COLOR_BLUE, (make8(iColor, 0) & 0x1F) << 3, FALSE);
   
   GLCDDataOutput();

   GLCD_COMMAND();   
   glcd_WritePortRegister(GLCD_MEMORY_DATA_PORT_0);

   GLCD_DATA();
   
   for(i=StartX;i<StopX;i++)
   {
      if(i == x)
         glcd_WritePort(color);
      else
         glcd_WritePort(iColor);
   }      
   
   glcd_WriteRegister(GLCD_INPUT_MODE, 0x00, FALSE);
   
   output_high(GLCD_CS);
}

///////////////////////////////////////////////////////////////////////////////
// glcd_DrawPixels()
//
// Draws a block of pixels on the display with the specified width and height
// starting at the specified coordinate.  The pixels are drawn from left to
// right and top to bottom until then entire width and height are drawn.
//
// Parameters:
//    StartX - the x coordinate to start drawing the pixels.
//
//    StartY - the y coordinate to start drawing the pixels.
//
//    Width - the number of 
//
///////////////////////////////////////////////////////////////////////////////
void glcd_DrawPixels(uint16_t StartX, uint16_t StartY, uint16_t Width, uint16_t Height, uint16_t *Data)
{
   uint32_t i;
   uint32_t Pixels;
   uint16_t LineSwitch;
   
   Pixels = (uint32_t)Width * (uint32_t)Height;
   
   if(((StartX % 8) == 0) && ((Width % 8) == 0))
   {
      output_low(GLCD_CS);
      
      glcd_WriteRegister(GLCD_WRITE_X_START_POS, StartX / 4, FALSE);
      glcd_WriteRegister(GLCD_WRITE_Y_START_POS_0, (StartY >> 2), FALSE);
      glcd_WriteRegister(GLCD_WRITE_Y_START_POS_1, (StartY & 0x03), FALSE);
      
      glcd_WriteRegister(GLCD_WRITE_X_END_POS, ((StartX / 4) + (Width / 4) - 2), FALSE);
      glcd_WriteRegister(GLCD_WRITE_Y_END_POS_0, (((StartY + Height) - 1) >> 2), FALSE);
      glcd_WriteRegister(GLCD_WRITE_Y_END_POS_1, (((StartY + Height) - 1) & 0x03), FALSE);
      
      GLCDDataOutput();
   
      GLCD_COMMAND();   
      glcd_WritePortRegister(GLCD_MEMORY_DATA_PORT_0);
   
      GLCD_DATA();
      
      if((Pixels % 64) == 0)
         LineSwitch = 64;
      else if ((Pixels % 32) == 0)
         LineSwitch = 32;
      else if((Pixels % 16) == 0)
         LineSwitch = 16;
      else if((Pixels % 8) == 0)
         LineSwitch = 8;
      else if((Pixels % 4) == 0)
         LineSwitch = 4;
      else if((Pixels % 2) == 0)
         LineSwitch = 2;
      else
         LineSwitch = 1;
      
      i=0;
      
      do
      {
         switch(LineSwitch)
         {
            case 64:
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               i+=32;
            case 32:
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               i+=16;
            case 16:
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               i+=8;
            case 8:
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               i+=4;
            case 4:
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               i+=2;
            case 2:
               glcd_WritePort(*Data++);
               glcd_WritePort(*Data++);
               i+=2;
               break;
            default:
               glcd_WritePort(*Data++);
               i++;
               break;
         }
      } while(i<Pixels);
      
      output_high(GLCD_CS);        //Chip Select GLCD Driver
   }
   else
   {
      uint16_t CurrentX, CurrentY, EndX;
      
      CurrentX = StartX;
      CurrentY = StartY;
      EndX = StartX + Width;
      
      i = 0;
      
      while(i < Pixels)
      {
         glcd_DrawPixel(CurrentX, CurrentY, *Data++);
         
         if(++CurrentX >= EndX)
         {
            CurrentY++;
            CurrentX = StartX;
         }
         
         i++;
      }
   }
}
