////////////////////////////////////////////////////////////////////////////////
/////                                                                      /////
/////                  Wii_Nunchuck.c - Library Functions                  /////
/////                                                                      /////
////////////////////////////////////////////////////////////////////////////////
/////                                                                      /////
///// This is a basic driver file for a Wii Nunchuck that allows a user to /////
///// access the data from the analog stick, accelerometer, and buttons    /////
///// from a struct in which the decoded data has been stored.             /////
/////                                                                      /////
////////////////////////////////////////////////////////////////////////////////
/////                                                                      /////
///// Pinout for Wii Nunchuck:                                             /////
/////                                                                      /////
/////    _________       Purpose:                   Wire Color:            /////
/////   /         \   1. SDA                        Green                  /////
/////   |  1 2 3  |   2. N.C. / Connection Detect   Copper                 /////
/////   |  -----  |   3. 3.3V                       Red                    /////
/////   |  4 5 6  |   4. GND                        White                  /////
/////   |   ___   |   5. N.C.                                              /////
/////   |__|   |__|   6. SCL                        Yellow                 /////
/////                                                                      /////
/////   *Note*: The wire colors may be different on different versions of  /////
/////           the Wii Nunchuck.  If cutting the plug off to attach a     /////
/////           different header, make sure to check which wire colors     /////
/////           connect to which pins on the pinout above.                 /////
/////                                                                      /////
////////////////////////////////////////////////////////////////////////////////
/////                                                                      /////
///// The results are stored in the global struct 'wii_data'               /////
/////                                                                      /////
////////////////////////////////////////////////////////////////////////////////
/////                                                                      /////
/////                            User Functions                            /////
/////                                                                      /////
///// wii_nunchuck_init():                                                 /////
/////    This function initializes the Wii Nunchuck.                       /////
/////                                                                      /////
///// wii_nunchuck_data():                                                 /////
/////    This function reads the data buffer on the Wii Nunchuck to        /////
/////    retrieve the values from the analog stick, accelerometer, and     /////
/////    buttons.  The values are then decoded and stored in a struct.     /////
/////                                                                      /////
/////    *Note*: The wii_nunchuck_data() function needs to be called in    /////
/////            order to read new data from the Wii Nunchuck and update   /////
/////            the values stored in the struct.                          /////
/////                                                                      /////
///// wii_is_connected()                                                   /////
/////    Returns TRUE if a Wii controller is connected and communicating   /////
/////    successfully.                                                     /////
/////                                                                      /////
///// wii_nunchuck_address():                                              /////
/////    This function can be used to verify that the predefined addresses /////
/////    are correct for the version of the Wii Nunchuck being used if the /////
/////    device is unresponsive after being connected to the system.  This /////
/////    function runs through a range of valid addresses checking for a   /////
/////    set that returns an ACK, that address is returned from the        /////
/////    function.  The returned address is the write address, adding one  /////
/////    to this address is the read address.                              /////
/////                                                                      /////
////////////////////////////////////////////////////////////////////////////////
/////                                                                      /////
/////          (C) Copyright 1996, 2015 Custom Computer Services           /////
/////                                                                      /////
///// This source code may only be used by licensed users of the CCS C     /////
///// compiler.  This source code may only be distributed to other or      /////
///// licensed users of the CCS C compiler.  No other use, reproduction    /////
///// distribution is permitted without written permission.  Derivative    /////
///// programs created using this software in object code form are not     /////
///// restricted in any way.                                               /////
/////                                                                      /////
////////////////////////////////////////////////////////////////////////////////

// Define Wii Nunchuck Read and Write Addresses
#define WII_NUNCHUCK_WRITE  0xA4
#define WII_NUNCHUCK_READ   0xA5

// Define I2C Clock and Data Pins
#ifndef WII_SCL
   #if defined(__PCD__)
      #define WII_SCL  PIN_G2
      #define WII_SDA  PIN_G3
   #else
      #define WII_SCL  PIN_C3
      #define WII_SDA  PIN_C4
   #endif
#endif

// Function Prototypes
int1 wii_is_connected(void);
void wii_nunchuck_init(void);
void wii_nunchuck_data(void);
unsigned int8 wii_nunchuck_address(void);

// Structure that Contains Decoded Wii Nunchuck Data
struct wii_struct{
   unsigned int8 x_analog;
   unsigned int8 y_analog;
   unsigned int16 x_axis;
   unsigned int16 y_axis;
   unsigned int16 z_axis;
   unsigned int1 c_button;
   unsigned int1 z_button;
} wii_data;

////////////////// library code follows, don't modify or use this ///////////

// Setup I2C
#use I2C(MASTER, SCL = WII_SCL, SDA = WII_SDA)

int1 _g_wii_is_init = 0;

// a simple look at the SCL and SDA lines to see if a controller is connected
int1 wii_is_connected(void)
{
   return(_g_wii_is_init);
}

static void _wii_isnt_connected(void)
{
   _g_wii_is_init = 0;
   
   // not connected.
   // return center position values.
   wii_data.x_analog = 0x80;
   wii_data.y_analog = 0x80;
   wii_data.x_axis = 0x200;
   wii_data.y_axis = 0x200;
   wii_data.z_axis = 0x200;
   wii_data.c_button = 0;
   wii_data.z_button = 0;
}

void wii_nunchuck_init(void) 
{
   int1 ack;
   
   _wii_isnt_connected();
      
   i2c_start();
   ack = !i2c_write(WII_NUNCHUCK_WRITE);
   if (ack)
      ack = !i2c_write(0x40);
   if (ack)
      ack = !i2c_write(0x00);
   i2c_stop();
   
   _g_wii_is_init = ack;
}

void wii_nunchuck_data(void) 
{
   int1 ack;
   unsigned int8 data_buffer[6];
   
   if (!_g_wii_is_init)
   {
      wii_nunchuck_init();
      delay_ms(5);
   }
   
   if (!_g_wii_is_init)
   {
      _wii_isnt_connected();

      return;
   }
   
   i2c_start();
   ack = !i2c_write(WII_NUNCHUCK_WRITE);
   if (ack)
      ack = !i2c_write(0x00);
   i2c_stop();
   
   if (!ack)
   {
      _wii_isnt_connected();
      return;
   }
   
   delay_ms(1);
   
   i2c_start();
   ack = !i2c_write(WII_NUNCHUCK_READ);
   if (ack)
   {
      data_buffer[0] = i2c_read();
      data_buffer[1] = i2c_read();
      data_buffer[2] = i2c_read();
      data_buffer[3] = i2c_read();
      data_buffer[4] = i2c_read();
      data_buffer[5] = i2c_read(0x00);
   }
   i2c_stop();
   
   delay_ms(1);
   
   if (!ack)
   {
      _wii_isnt_connected();
      return;
   }
   
   data_buffer[0] = (data_buffer[0] ^ 0x17) + 0x17;
   data_buffer[1] = (data_buffer[1] ^ 0x17) + 0x17;
   data_buffer[2] = (data_buffer[2] ^ 0x17) + 0x17;
   data_buffer[3] = (data_buffer[3] ^ 0x17) + 0x17;
   data_buffer[4] = (data_buffer[4] ^ 0x17) + 0x17;
   data_buffer[5] = (data_buffer[5] ^ 0x17) + 0x17;


   wii_data.x_analog = data_buffer[0];
   wii_data.y_analog = data_buffer[1];
   wii_data.x_axis = ((unsigned int16)data_buffer[2] << 2)
                        | ((data_buffer[5] & 0x0C) >> 2);
   wii_data.y_axis = ((unsigned int16)data_buffer[3] << 2)
                        | ((data_buffer[5] & 0x30) >> 4);
   wii_data.z_axis = ((unsigned int16)data_buffer[4] << 2)
                        | ((data_buffer[5] & 0xC0) >> 6);
   wii_data.c_button = ~((data_buffer[5] & 0x02) >> 1);
   wii_data.z_button = ~(data_buffer[5] & 0x01);
}

unsigned int8 wii_nunchuck_address(void) {
   unsigned int8 address, result;
   
   if (!wii_is_connected())
   {
      return(0);
   }
   
   for(address = 0x10; address < 0xF0; address += 2) {
      i2c_start();
      result = i2c_write(address);
      i2c_stop();
      
      if(!result) {
         return address;
      }
   }
}

