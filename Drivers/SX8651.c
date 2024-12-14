///////////////////////////////////////////////////////////////////////////////
//                                  SX8651.c                                 //
//                                                                           //
// Driver for a SX8651 Resistive Touchscreen Controller.                     //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
//                                    API                                    //
///////////////////////////////////////////////////////////////////////////////
// Functions:                                                                //
//                                                                           //
//    touch_Init(void)                                                       //
//       - Initializes the SX8651, must be called before all other touch     //
//         functions.                                                        //
//                                                                           //
//    touch_WriteRegister(uint8_t addr, uint8_t data)                        //
//       - Writes data to the specified SX8651 register address.             //
//                                                                           //
//    uint8_t touch_ReadRegister(uint8_t addr)                               //
//       - Reads data from the specified SX8651 register address.            //
//                                                                           //
//    touch_SetMode(uint8_t data)                                            //
//       - Sets the operation mode of the SX8651.                            //
//                                                                           //
//    uint16_t touch_ReadChannel(uint8_t channel)                            //
//       - Reads specified channel from SX8651.  The available channels are  //
//         defined in SX8651.h, search for Touch Channel Defines.            //
//                                                                           //
//    touch_ReadChannels(uint16_t *ptr, uint8_t count)                       //
//       - Read count channels from SX8651 and save to uint16_t array        //
//         pointed to by ptr.                                                //
//                                                                           //
// Defines:                                                                  //
//                                                                           //
//    TOUCH_ADDRESS                                                          //
//       - Sets the I2C address of the SX8651, default is 0x90.              //
//                                                                           //
//    TOUCH_SCL                                                              //
//       - Sets the PIC's I2C SCL pin, default is PIN_A2.                    //
//                                                                           //
//    TOUCH_SDA                                                              //
//       - Sets the PIC's I2C SDA pin, default is PIN_A3.                    //
//                                                                           //
//    TOUCH_INT                                                              //
//       - Seta the PIC pin that is connected to the SX8651 NIRQ pin.  When  //
//         defined touch_Ready() function uses the TOUCH_INT pin to          //
//         determine when a conversion is ready to be read.  Pin is          //
//         undefined by default.                                             //
//                                                                           //
//    TOUCH_BAUD                                                             //
//       - Sets the I2C Baud rat, default is 400000.                         //
//                                                                           //
//    TOUCH_USE_HW_I2C                                                       //
//       - If defined specifies to use the Hardware I2C peripheral of PIC,   //
//         the TOUCH_SCL and TOUCH_SDA pin defines must match the  HW I2C    //
//         pins of the PIC.  TOUCH_USE_HW_I2C is undefined by default.       //
//                                                                           //
//    TOUCH_MODE                                                             //
//       - Sets the operating mode of the SX8651.  The available modes are   //
//         defined in SX8651.h, search for Touch Mode Defines.  Default is   //
//         TOUCH_MODE_MANAUTO.                                               //
//                                                                           //
//    TOUCH_RATE                                                             //
//       - Sets the conversion rate of the SX8651.  The available rates are  //
//         defined in SX8651.h, search for Touch Rate Defines.  Default is   //
//         TOUCH_RATE_DISABLED, manual mode.                                 //
//                                                                           //
//    TOUCH_CONV_TIME                                                        //
//       - Sets the conversion time of the SX8651.  The available conversion //
//         times are defined in SX8651.h, search for Touch Conversion Time   //
//         Defines.  Default is TOUCH_CONV_TIME_0, Immediate (0.5 us)        //
//                                                                           //
//    TOUCH_AUX                                                              //
//       - Sets how the AUX input pin is used by the SX8651.  The available  //
//         options are defined in SX8651.h, search for Touch Auxiliary       //
//         Defines.  Default is TOUCH_AUX_ANALOG.                            //
//                                                                           //
//    TOUCH_IRQ                                                              //
//       - Sets when the SX8651 interrupts are generated.  The available     //
//         options are defined in SX8651.h, search for Touch IRQ Defines.    //
//         Default is TOUCH_IRQ_PEN_DETECT.                                  //
//                                                                           //
//    TOUCH_PEN_RESISTOR                                                     //
//       - Sets the resistance of the Pen Detect.  The available options are //
//         defined in SX8651.h, search for Touch Pen Resistor Refines.       //
//         Default is TOUCH_PEN_RESISTOR_100K.                               //
//                                                                           //
//    TOUCH_FILTER                                                           //
//       - Sets up the SX8651 filter.  The available options are defined in  //
//         SX8651.h, search for Touch Filter Defines.  Default is            //
//         TOUCH_FILTER_DISABLED.                                            //
//                                                                           //
//    TOUCH_SETTLING_TIME                                                    //
//       - Sets the settling time when filtering.  The available options are //
//         defined in SX8651.h, search for Touch Settling Time Defines.      //
//         Default is TOUCH_SETTLING_TIME_0, Immediate (0.5 us).             //
//                                                                           //
//    TOUCH_PANEL_RESISTANCE                                                 //
//       - Sets the panel resistance.  The available options are defined in  //
//         SX8651.h, search for Touch X Panel Resistance Defines and Touch Y //
//         Panel Resistance Defines.  One option from each should be or'ed   //
//         together.  Default is (TOUCH_YRES_100_187 | TOUCH_XRES_100_187).  //
//                                                                           //
//    TOUCH_CONV_CHANNELS                                                    //
//       - Sets which channels are converted.  The available options are     //
//         defined in SX8651.h, search for Touch Conversion Channel Defines. //
//         Multiple channels can be enabled by or'ing the defines together.  //
//         Default is (TOUCH_CONV_CHANNEL_X | TOUCH_CONV_CHANNEL_Y).         //
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

#include <SX8651.h>

#ifndef TOUCH_MODE
 #define TOUCH_MODE              TOUCH_MODE_MANAUTO
#endif

#ifndef TOUCH_RATE
 #define TOUCH_RATE              TOUCH_RATE_DISABLED
#endif

#ifndef TOUCH_CONV_TIME
 #define TOUCH_CONV_TIME         TOUCH_CONV_TIME_0
#endif

#ifndef TOUCH_AUX
 #define TOUCH_AUX               TOUCH_AUX_ANALOG
#endif

#ifndef TOUCH_IRQ
 #define TOUCH_IRQ               TOUCH_IRQ_PEN_DETECT
#endif

#ifndef TOUCH_PEN_RESISTOR
 #define TOUCH_PEN_RESISTOR      TOUCH_PEN_RESISTOR_100K
#endif

#ifndef TOUCH_FILTER
 #define TOUCH_FILTER            TOUCH_FILTER_DISABLED
#endif

#ifndef TOUCH_SETTLING_TIME
 #define TOUCH_SETTLING_TIME     TOUCH_SETTLING_TIME_0
#endif

#ifndef TOUCH_PANEL_RESISTANCE
 #define TOUCH_PANEL_RESISTANCE  (TOUCH_YRES_100_187 | TOUCH_XRES_100_187)
#endif

#ifndef TOUCH_CONV_CHANNELS
 #define TOUCH_CONV_CHANNELS     (TOUCH_CONV_CHANNEL_X | TOUCH_CONV_CHANNEL_Y)
#endif

///////////////////////////////////////////////////////////////////////////////
// touch_Init()
//
// Initializes the SX8651 to the specified settings.
//
///////////////////////////////////////////////////////////////////////////////
void touch_Init(void)
{
  #ifdef TOUCH_INT
   TouchInitINT();
  #endif
   
   touch_WriteRegister(TOUCH_ADDR_RESET, 0xDE);
   
  #ifdef TOUCH_INT
   while(touch_Ready());
  #else
   delay_ms(1);
  #endif
   
   touch_WriteRegister(TOUCH_ADDR_CTRL0, TOUCH_RATE | TOUCH_CONV_TIME);
   touch_WriteRegister(TOUCH_ADDR_CTRL1, TOUCH_AUX | TOUCH_IRQ | TOUCH_PEN_RESISTOR | TOUCH_FILTER);
   touch_WriteRegister(TOUCH_ADDR_CTRL2, TOUCH_SETTLING_TIME);
   touch_WriteRegister(TOUCH_ADDR_CTRL3, TOUCH_PANEL_RESISTANCE);
   
   touch_WriteRegister(TOUCH_ADDR_CHAN_MAK, TOUCH_CONV_CHANNELS);
   
   touch_SetMode(TOUCH_MODE);
}

///////////////////////////////////////////////////////////////////////////////
// touch_WriteRegister()
//
// Writes a data byte to one of the SX8651 registers.
//
// Parameters:
//    addr - address of register to write.  Addresses are defined in SX8651.h
//           search for Touch Register Addresses.
//
//    data - data byte to write to register.
//
///////////////////////////////////////////////////////////////////////////////
void touch_WriteRegister(uint8_t addr, uint8_t data)
{
   i2c_start(TOUCH_STREAM);
   i2c_write(TOUCH_STREAM, TOUCH_ADDRESS);
   i2c_write(TOUCH_STREAM, (TOUCH_CMD_WRITE | addr));
   i2c_write(TOUCH_STREAM, data);
   i2c_stop(TOUCH_STREAM);
}

///////////////////////////////////////////////////////////////////////////////
// touch_ReadRegister()
//
// Read a byte from one of the SX8651 registers.
//
// Parameters:
//    addr - address of register to read.  Addresses are defined in SX8651.h
//           search for Touch Register Addresses.
//
// Returns:
//    uint8_t value read from register.
//
///////////////////////////////////////////////////////////////////////////////
uint8_t touch_ReadRegister(uint8_t addr)
{
   uint8_t result;
   
   i2c_start(TOUCH_STREAM);
   i2c_write(TOUCH_STREAM, TOUCH_ADDRESS);
   i2c_write(TOUCH_STREAM, (TOUCH_CMD_READ | addr));
   i2c_start(TOUCH_STREAM);
   i2c_write(TOUCH_STREAM, (TOUCH_ADDRESS | 1));
   result = i2c_read(TOUCH_STREAM, 0);
   i2c_stop(TOUCH_STREAM);
   
   return(result);
}

///////////////////////////////////////////////////////////////////////////////
// touch_SetMode()
//
// Sets the operating mode of the SX8651.  Can also be used select the read
// channel, and start the conversion of a channel.
//
// Parameters:
//    data - mode to set the SX8651 to.  Modes defined in SX8651.h, search for 
//           Touch Mode Defines.
//
///////////////////////////////////////////////////////////////////////////////
void touch_SetMode(uint8_t data)
{
   i2c_start(TOUCH_STREAM);
   i2c_write(TOUCH_STREAM, TOUCH_ADDRESS);
   i2c_write(TOUCH_STREAM, data);
   i2c_stop(TOUCH_STREAM);
   
}

///////////////////////////////////////////////////////////////////////////////
// touch_ReadChannel()
//
// Read the specified channel from the SX8651.
//
// Parameters:
//    channel - the channel to read.  Channels are defined in SX8651.h, search 
//              for Touch Channel Defines.
//
// Returns:
//    uint16_t value that was read from the channel.
//
///////////////////////////////////////////////////////////////////////////////
uint16_t touch_ReadChannel(uint8_t channel)
{
   uint16_t result;
   uint8_t Data[2];
   
   touch_SetMode(TOUCH_MODE_SELECT_CHAN | channel);
   
   i2c_start(TOUCH_STREAM);
   i2c_write(TOUCH_STREAM, (TOUCH_ADDRESS | 1));
   Data[0] = i2c_read(TOUCH_STREAM, 1);
   Data[1] = i2c_read(TOUCH_STREAM, 0);
   i2c_stop(TOUCH_STREAM);
   
   result = make16(Data[0] & 0x0F, Data[1]);
   
   return(result);
}

///////////////////////////////////////////////////////////////////////////////
// touch_ReadChannels()
//
// Reads multiple channels from the SX8651.
//
// Parameters:
//    ptr - pointer to an uint16_t array to read channel readings to.
//
//    count - number of channels to read, max is 5 channels.
//
///////////////////////////////////////////////////////////////////////////////
void touch_ReadChannels(uint16_t *ptr, uint8_t count)
{
   uint8_t i;
   uint8_t Data[10];
   uint8_t rCount;
   
   rCount = count * 2;
   
   if(count > 5)
   {
      rCount = 10;
      count = 5;
   }
   
   i2c_start(TOUCH_STREAM);
   i2c_write(TOUCH_STREAM, (TOUCH_ADDRESS | 1));
   
   for(i=0;i<(rCount - 1);i++)
      Data[i] = i2c_read(TOUCH_STREAM, 1);
   
   Data[i] = i2c_read(TOUCH_STREAM, 0);
   i2c_stop(TOUCH_STREAM);
   
   for(i=0;i<count;i++)
      ptr[i] = make16(Data[i*2] & 0x0F, Data[(i*2) + 1]);
}

#ifndef TOUCH_INT
///////////////////////////////////////////////////////////////////////////////
// touch_Ready()
//
// Checks if a conversion is ready to be read.
//
// Returns:
//    TRUE  - if conversion is done a ready to be read.
//    FALSE - if conversion is not done.
//
///////////////////////////////////////////////////////////////////////////////
int1 touch_Ready(void)
{
   uint8_t result;
   
   i2c_start(TOUCH_STREAM);
   i2c_write(TOUCH_STREAM, TOUCH_ADDRESS);
   i2c_write(TOUCH_STREAM, (TOUCH_CMD_READ | TOUCH_ADDR_STAT));
   i2c_start(TOUCH_STREAM);
   i2c_write(TOUCH_STREAM, (TOUCH_ADDRESS | 1));
   result = i2c_read(TOUCH_STREAM, 0);
   i2c_stop(TOUCH_STREAM);
   
   return(bit_test(result, 7));
}
#endif
