/*
                               mcp23017.c

   Driver for Microchip MCP23017 16bit I/O expander using I2C.
   
   This device also has weak internal pull-ups and interrupt on change.
   
   MCP23017_I2C_ADDRESS - user can overwrite this #define before including
      this library.  It user doesn't, the driver will use the default value
      of 0x40 (the value if A1, A2 and A3 are ground)

   MCP23017_I2C_STREAM - if this is defined, the library will use this I2C
      stream identifier and won't create a new one with #use i2c().
   
   MCP23017_IC2_PINS - if this is defined and MCP23017_I2C_STREAM isn't
      defined, then this will be used when creating #use i2c().
      Some examples:
         #define MCP23017_IC2_PINS SCL=PIN_C3 SDA=PIN_C4
         #define MCP23017_IC2_PINS I2C1, FORCE_HW
   
   MCP23017_I2C_BAUD - user can overwrite this #define before including
      this library.  Max speed at 1.7M at 5V, 400K at 2.7V, 100K for all
      other voltages.  If user doesn't specify, will use 100K speed.
   
   MCP23017_PIN_RESET - user must define this to the ~RESET pin of the chip.
   
   mcp23017_init() - initialize chip and library.
   
   mcp23017_set_tris(int16 tris) - set tris of port A and port B.  port A
      is the LSB, port B is the MSB.
   
   int16 mcp23017_input() - read both port A and port B at the same 
      time.  A will be LSB, B will be MSB.
      
   int8 mcp23017_input_a() - read port A
   
   int8 mcp23017_input_b() - read port B
   
   mcp23017_output(int16 val) - set output bits for port A and port B.  Port A
      is the LSB and port B is the MSB.
      
   int16 mcp23017_get_latch() - get the output bits for port A and port B.
      Port A is the LSB and port B is the MSB.
   
   mcp23017_write_register(int8 register, int8 val) - write to one of the
      21 internal registers of the chip
   
   int8 mcp23017_read_register(int8 register) - read from one of the 21
      internal registers of the chip
*/
///////////////////////////////////////////////////////////////////////////
////       (C) Copyright 1996,2014 Custom Computer Services            ////
////                  http://www.ccsinfo.com                           ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
///////////////////////////////////////////////////////////////////////////

#ifndef __MCP23017_C__
#define __MCP23017_C__

// these register defines are only valid if you leave the BANK bit in IOCON 
// clear.  this library will always leave this bit clear.
#define MCP23017_REG_IODIRA 0x00
#define MCP23017_REG_IODIRB 0x01
#define MCP23017_REG_IPOLA 0x02
#define MCP23017_REG_IPOLB 0x03
#define MCP23017_REG_GPINTENA 0x04
#define MCP23017_REG_GPINTENB 0x05
#define MCP23017_REG_DEFVALA 0x06
#define MCP23017_REG_DEFVALB 0x07
#define MCP23017_REG_INTCONA 0x08
#define MCP23017_REG_INTCONB 0x09
#define MCP23017_REG_IOCON 0x0a
//#define MCP23017_REG_IOCON 0x0b
#define MCP23017_REG_GPPUA 0x0c
#define MCP23017_REG_GPPUB 0x0d
#define MCP23017_REG_INTFA 0x0e
#define MCP23017_REG_INTFB 0x0f
#define MCP23017_REG_INTCAPA 0x010
#define MCP23017_REG_INTCAPB 0x11
#define MCP23017_REG_GPIOA 0x12
#define MCP23017_REG_GPIOB 0x13
#define MCP23017_REG_OLATA 0x14
#define MCP23017_REG_OLATB 0x15

#ifndef MCP23017_I2C_ADDRESS
#define MCP23017_I2C_ADDRESS  0x40
#endif

#ifndef MCP23017_I2C_STREAM
   #ifndef MCP23017_IC2_PINS
      #define MCP23017_IC2_PINS  I2C1, FORCE_HW
   #endif
   
   #ifndef MCP23017_I2C_BAUD
      #define MCP23017_I2C_BAUD 100000
   #endif
   
   #define MCP23017_LIBRARY_CREATED_STREAM
   
   #use i2c(MASTER, stream=MCP23017_I2C_STREAM, MCP23017_IC2_PINS, fast=MCP23017_I2C_BAUD, NOINIT)
#endif

void mcp23017_init(void)
{
  #if defined(MCP23017_LIBRARY_CREATED_STREAM)
   i2c_init(MCP23017_I2C_STREAM);
  #endif
   
   output_low(MCP23017_PIN_RESET);
   delay_us(2);
   output_high(MCP23017_PIN_RESET);
   delay_us(2);
}

static void mcp23017_write_bytes(unsigned int8 reg, unsigned int8 *p, unsigned int8 n)
{
   i2c_start();
   i2c_write(MCP23017_I2C_ADDRESS);
   i2c_write(reg);
   while(n--)
   {
      i2c_write(*p++);
   }
   i2c_stop();
}

static void mcp23017_read_bytes(unsigned int8 *p, unsigned int8 reg, unsigned int8 n)
{
   i2c_start();
   i2c_write(MCP23017_I2C_ADDRESS);
   i2c_write(reg);
   i2c_start();
   i2c_write(MCP23017_I2C_ADDRESS | 1);
   while(n--)
   {
      *p++ = i2c_read(n!=0);
   }
   i2c_stop();
}

void mcp23017_set_tris(unsigned int16 tris)
{
   mcp23017_write_bytes(MCP23017_REG_IODIRA, &tris, sizeof(tris));
}  

unsigned int16 mcp23017_input(void)
{
   unsigned int16 ret;
   
   mcp23017_read_bytes(&ret, MCP23017_REG_GPIOA, sizeof(ret));
   
   return(ret);
}
      
unsigned int8 mcp23017_input_a(void)
{
   unsigned int8 ret;
   
   mcp23017_read_bytes(&ret, MCP23017_REG_GPIOA, sizeof(ret));
   
   return(ret);
}

unsigned int8 mcp23017_input_b(void)
{
   unsigned int8 ret;
   
   mcp23017_read_bytes(&ret, MCP23017_REG_GPIOB, sizeof(ret));
   
   return(ret);
}
   

void mcp23017_output(unsigned int16 val)
{
   mcp23017_write_bytes(MCP23017_REG_OLATA, &val, sizeof(val));
}

unsigned int16 mcp23017_get_latch(void)
{
   unsigned int16 ret;
   
   mcp23017_read_bytes(&ret, MCP23017_REG_OLATA, sizeof(ret));
   
   return(ret);
}
   
void mcp23017_write_register(unsigned int8 reg, unsigned int8 val)
{
   mcp23017_write_bytes(reg, &val, sizeof(val));
}
   
unsigned int8 mcp23017_read_register(unsigned int8 reg)
{
   unsigned int8 val;
   
   mcp23017_read_bytes(&val, reg, sizeof(val));
   
   return(val);
}

#endif
