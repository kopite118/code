///////////////////////////////////////////////////////////////////////////
///                          usb_desc_mouse.h                          ////
////                                                                   ////
//// An example set of device / configuration descriptors.             ////
//// This set works exclusively with ex_usb_mouse.c example.           ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////                                                                   ////
//// Version History:                                                  ////
////                                                                   ////
//// March 5th, 2009:                                                  ////
////   Cleanup for Wizard.                                             ////
////   PIC24 Initial release.                                          ////
////                                                                   ////
//// June 20th, 2005:                                                  ////
////   Initial 18Fxx5x release                                         ////
////                                                                   ////
//// March 21st, 2005:                                                 ////
////   EP 0x81 now uses USB_EP1_TX_SIZE to define max packet size, to  ////
////      make it easier for dynamically changed code.                 ////
////   EP 0x81 will now use 1ms polling interval if using a full speed ////
////      device.                                                      ////
////                                                                   ////
//// June 24th, 2002: Cleanup                                          ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2005 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS    ////
//// C compiler.  This source code may only be distributed to other    ////
//// licensed users of the CCS C compiler.  No other use,              ////
//// reproduction or distribution is permitted without written         ////
//// permission.  Derivative programs created using this software      ////
//// in object code form are not restricted in any way.                ////
///////////////////////////////////////////////////////////////////////////

#IFNDEF __USB_DESCRIPTORS__
#DEFINE __USB_DESCRIPTORS__

///////// config options, although it's best to leave alone for this demo /////
#define  USB_CONFIG_PID       0x0022
#define  USB_CONFIG_VID       0x0461
#define  USB_CONFIG_BUS_POWER 100   //100mA  (range is 0..500)
#define  USB_CONFIG_VERSION   0x0100      //01.00  //range is 00.00 to 99.99
//////// end config ///////////////////////////////////////////////////////////



#DEFINE USB_HID_DEVICE  TRUE  //Tells the CCS PIC USB firmware
                              //to include HID handling code.

//turn on EP1 for IN interrupt transfers.  (IN = PIC -> PC)
#define USB_EP1_TX_ENABLE  USB_ENABLE_INTERRUPT 

#ifndef USB_EP1_TX_SIZE
#define USB_EP1_TX_SIZE 8     //max packet size of this endpoint
#endif

#include <usb.h>

   //////////////////////////////////////////////////////////////////
   ///
   ///  HID Report.  Tells HID driver how to handle and deal with
   ///  received data.  HID Reports can be extremely complex,
   ///  see HID specifcation for help on writing your own.
   ///
   ///  This examples configures HID driver to take received data
   ///  as mouse x, y and button data.
   ///
   //////////////////////////////////////////////////////////////////

   const char USB_CLASS_SPECIFIC_DESC[] = 
   {
      0x05, 0x01, // usage page (generic desktop Choose the usage page "mouse" is on
      0x09, 0x02, // usage (mouse) Device is a mouse
      0xA1, 0x01, // collection (application) This collection encompasses the report format
      0x09, 0x01, // usage (pointer) Choose the key code usage page
      0xA1, 0x00, // collection (physical) Physical collection
      0x05, 0x09, // usage page (buttons) Choose the �button� usage page
      0x19, 0x01, // usage minimum (1) There are three buttons
      0x29, 0x03, // usage maximum (3)
      0x15, 0x00, // logical minimum (0) Each button is represented by one bit
      0x25, 0x01, // logical maximum (1)
      0x95, 0x03, // report count (3) Three reports, one bit each
      0x75, 0x01, // report size (1)
      0x81, 0x02, // input (data, variable, absolute) Defined bits above are data bits
      0x95, 0x01, // report count (1) One report, five bits in length
      0x75, 0x05, // report size (5)
      0x81, 0x01, // input (constant) Bit stuff to fill byte
      0x05, 0x01, // usage page (generic desktop) Choose the usage pare �X� and �Y� are on
      0x09, 0x30, // usage (X) X direction of pointer
      0x09, 0x31, // usage (Y) Y direction of pointer
      0x09, 0x38,  // usage (wheel)
      0x15, 0x81, // logical minimum (-127) Range of report data is -127 to 127
      0x25, 0x7F, // logical maximum (127)
      0x75, 0x08, // report size (8) Two reports, eight bits each
      0x95, 0x03, // report count (3)
      0x81, 0x06, // input (data, variable, absolute) Defined bits above are data bits
      0xC0,       // end collection End physical collection
      0xC0        // end collection End application collection
   };

   //if a class has an extra descriptor not part of the config descriptor,
   // this lookup table defines where to look for it in the const
   // USB_CLASS_SPECIFIC_DESC[] array.
   //first element is the config number (if your device has more than one config)
   //second element is which interface number
   //set element to 0xFFFF if this config/interface combo doesn't exist
   const int16 USB_CLASS_SPECIFIC_DESC_LOOKUP[USB_NUM_CONFIGURATIONS][1] =
   {
   //config 1
      //interface 0
         0
   };

   //if a class has an extra descriptor not part of the config descriptor,
   // this lookup table defines the size of that descriptor.
   //first element is the config number (if your device has more than one config)
   //second element is which interface number
   //set element to 0xFFFF if this config/interface combo doesn't exist
   const int16 USB_CLASS_SPECIFIC_DESC_LOOKUP_SIZE[USB_NUM_CONFIGURATIONS][1] =
   {
   //config 1
      //interface 0
      sizeof(USB_CLASS_SPECIFIC_DESC)
   };



//////////////////////////////////////////////////////////////////
///
///   start config descriptor
///   right now we only support one configuration descriptor.
///   the config, interface, class, and endpoint goes into this array.
///
//////////////////////////////////////////////////////////////////

   #DEFINE USB_TOTAL_CONFIG_LEN      34 //config+interface+class+endpoint

   const char USB_CONFIG_DESC[] = {
   //IN ORDER TO COMPLY WITH WINDOWS HOSTS, THE ORDER OF THIS ARRAY MUST BE:
      //    config(s)
      //    interface(s)
      //    class(es)
      //    endpoint(s)

   //config_descriptor for config index 1
         USB_DESC_CONFIG_LEN, //length of descriptor size          ==1
         USB_DESC_CONFIG_TYPE, //constant CONFIGURATION (CONFIGURATION 0x02)     ==2
         USB_TOTAL_CONFIG_LEN,0, //size of all data returned for this config      ==3,4
         1, //number of interfaces this device supports       ==5
         0x01, //identifier for this configuration.  (IF we had more than one configurations)      ==6
         0x00, //index of string descriptor for this configuration      ==7
        #if USB_CONFIG_BUS_POWER
         0x80, //bit 6=1 if self powered, bit 5=1 if supports remote wakeup (we don't), bits 0-4 unused and bit7=1         ==8
        #else
         0xC0, //bit 6=1 if self powered, bit 5=1 if supports remote wakeup (we don't), bits 0-4 unused and bit7=1         ==8
        #endif
         USB_CONFIG_BUS_POWER/2, //maximum bus power required (maximum milliamperes/2)  (0x32 = 100mA)   ==9

   //interface descriptor 1
         USB_DESC_INTERFACE_LEN, //length of descriptor      =10
         USB_DESC_INTERFACE_TYPE, //constant INTERFACE (INTERFACE 0x04)       =11
         0x00, //number defining this interface (IF we had more than one interface)    ==12
         0x00, //alternate setting     ==13
         1, //number of endpoints, except 0     ==14
         0x03, //class code, 03 = HID     ==15
         0x01, //subclass code //boot     ==16
         0x02, //protocol code      ==17
         0x00, //index of string descriptor for interface      ==18

   //class descriptor 1  (HID)
         USB_DESC_CLASS_LEN, //length of descriptor    ==19
         USB_DESC_CLASS_TYPE, //dscriptor type (0x21 == HID)      ==20
         0x00,0x01, //hid class release number (1.0) (try 1.10)      ==21,22
         0x00, //localized country code (0 = none)       ==23
         0x01, //number of hid class descrptors that follow (1)      ==24
         0x22, //report descriptor type (0x22 == HID)                ==25
         USB_CLASS_SPECIFIC_DESC_LOOKUP_SIZE[0][0], 0x00, //length of report descriptor            ==26,27

   //endpoint descriptor
         USB_DESC_ENDPOINT_LEN, //length of descriptor                   ==28
         USB_DESC_ENDPOINT_TYPE, //constant ENDPOINT (ENDPOINT 0x05)          ==29
         0x81, //endpoint number and direction (0x81 = EP1 IN)       ==30
         USB_ENDPOINT_TYPE_INTERRUPT, //transfer type supported (0x03 is interrupt)         ==31
         USB_EP1_TX_SIZE,0x00, //maximum packet size supported                  ==32,33
         10  //polling interval, in ms.  (cant be smaller than 10 for slow speed devices)     ==34
   };


   //****** BEGIN CONFIG DESCRIPTOR LOOKUP TABLES ********
   //since we can't make pointers to constants in certain pic16s, this is an offset table to find
   //  a specific descriptor in the above table.

   //NOTE: DO TO A LIMITATION OF THE CCS CODE, ALL HID INTERFACES MUST START AT 0 AND BE SEQUENTIAL
   //      FOR EXAMPLE, IF YOU HAVE 2 HID INTERFACES THEY MUST BE INTERFACE 0 AND INTERFACE 1
   #define USB_NUM_HID_INTERFACES   1

   //the maximum number of interfaces seen on any config
   //for example, if config 1 has 1 interface and config 2 has 2 interfaces you must define this as 2
   #define USB_MAX_NUM_INTERFACES   1

   //define how many interfaces there are per config.  [0] is the first config, etc.
   const char USB_NUM_INTERFACES[USB_NUM_CONFIGURATIONS]={1};

   //define where to find class descriptors
   //first dimension is the config number
   //second dimension specifies which interface
   //last dimension specifies which class in this interface to get, but most will only have 1 class per interface
   //if a class descriptor is not valid, set the value to 0xFFFF
   const int16 USB_CLASS_DESCRIPTORS[USB_NUM_CONFIGURATIONS][USB_NUM_HID_INTERFACES][1]=
   {
   //config 1
      //interface 0
         //class 1
         18
   };


   #if (sizeof(USB_CONFIG_DESC) != USB_TOTAL_CONFIG_LEN)
      #error USB_TOTAL_CONFIG_LEN not defined correctly
   #endif


//////////////////////////////////////////////////////////////////
///
///   start device descriptors
///
//////////////////////////////////////////////////////////////////

   const char USB_DEVICE_DESC[] = {
      //starts of with device configuration. only one possible
         USB_DESC_DEVICE_LEN, //the length of this report   ==1
         0x01, //the constant DEVICE (DEVICE 0x01)  ==2
         0x10,0x01, //usb version in bcd (pic167xx is 1.1) ==3,4
         0x00, //class code ==5
         0x00, //subclass code ==6
         0x00, //protocol code ==7
         USB_MAX_EP0_PACKET_LENGTH, //max packet size for endpoint 0. (SLOW SPEED SPECIFIES 8) ==8
         USB_CONFIG_VID & 0xFF, ((USB_CONFIG_VID >> 8) & 0xFF), //vendor id       ==9, 10
         USB_CONFIG_PID & 0xFF, ((USB_CONFIG_PID >> 8) & 0xFF), //product id, don't use 0xffff       ==11, 12
         USB_CONFIG_VERSION & 0xFF, ((USB_CONFIG_VERSION >> 8) & 0xFF), //device release number  ==13,14
         0x01, //index of string description of manufacturer. therefore we point to string_1 array (see below)  ==15
         0x02, //index of string descriptor of the product  ==16
         0x00, //index of string descriptor of serial number  ==17
         USB_NUM_CONFIGURATIONS  //number of possible configurations  ==18
   };

   #if (sizeof(USB_DEVICE_DESC) != USB_DESC_DEVICE_LEN)
      #error USB_DESC_DEVICE_LEN not defined correctly
   #endif


//////////////////////////////////////////////////////////////////
///
///   start string descriptors
///   String 0 is a special language string, and must be defined.  People in U.S.A. can leave this alone.
///
//////////////////////////////////////////////////////////////////

//the offset of the starting location of each string.  offset[0] is the start of string 0, offset[1] is the start of string 1, etc.
const char USB_STRING_DESC_OFFSET[]={0,4,12};

//number of strings you have, including string 0.
#define USB_STRING_DESC_COUNT sizeof(USB_STRING_DESC_OFFSET)

// Here is where the "CCS" Manufacturer string and "CCS USB Mouse" are stored.
// Strings are saved as unicode.
char const USB_STRING_DESC[]={
   //string 0
         4, //length of string index
         USB_DESC_STRING_TYPE, //descriptor type 0x03 (STRING)
         0x09,0x04,   //Microsoft Defined for US-English
   //string 1
         8, //length of string index
         USB_DESC_STRING_TYPE, //descriptor type 0x03 (STRING)
         'C',0,
         'C',0,
         'S',0,
   //string 2
         28, //length of string index
         USB_DESC_STRING_TYPE, //descriptor type 0x03 (STRING)
         'C',0,
         'C',0,
         'S',0,
         ' ',0,
         'U',0,
         'S',0,
         'B',0,
         ' ',0,
         'M',0,
         'o',0,
         'u',0,
         's',0,
         'e',0
};

#endif
