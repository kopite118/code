///////////////////////////////////////////////////////////////////////////
///                       usb_desc_multihid2.h                         ////
////                                                                   ////
//// An example set of device / configuration descriptors.             ////
//// This set works exclusively with ex_usb_multihid2.c example.       ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////                                                                   ////
//// Version History:                                                  ////
////                                                                   ////
//// Dec 18, 2014:                                                     ////
////  Created.                                                         ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////  NOTE: DUE TO A LIMITATION OF THE WAY THIS CODE HANDLES MULTIPLE  ////
////        INTERFACES, ALL INTERFACES MUST START AT 0 AND BE          ////
////        SEQUENTIAL.  THEREFORE IF CONFIG 1 HAS 3 INTERFACES, THEY  ////
////        MUST BE INTERFACE 0, INTERFACE 1 and INTERFACE 2           ////
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
#define  USB_CONFIG_PID       0x0057
#define  USB_CONFIG_VID       0x0461
#define  USB_CONFIG_BUS_POWER 100   //100mA  (range is 0..500)
#define  USB_CONFIG_VERSION   0x0100      //01.00  //range is 00.00 to 99.99

#ifndef USB_CONFIG_HID_TX_SIZE
   //valid range is 0-255
   #define USB_CONFIG_HID_TX_SIZE   2     //compatible with hiddemo.exe
#endif

#ifndef USB_CONFIG_HID_RX_SIZE
   //valid range is 0-255
   #define USB_CONFIG_HID_RX_SIZE   2     //compatible with hiddemo.exe
#endif
//////// end config ///////////////////////////////////////////////////////////

//Tells the CCS PIC USB firmware to include HID handling code.
#DEFINE USB_HID_DEVICE  TRUE

//the following defines needed for the CCS USB PIC driver to enable the TX endpoint 1
#define USB_EP1_TX_ENABLE  USB_ENABLE_INTERRUPT   //turn on EP1 for IN bulk/interrupt transfers
#define USB_EP1_TX_SIZE 8

#define USB_EP1_RX_ENABLE  USB_ENABLE_INTERRUPT   //turn on EP1 for IN bulk/interrupt transfers
#define USB_EP1_RX_SIZE 8

#include <usb.h>

   //////////////////////////////////////////////////////////////////
   ///
   ///  HID Report.  Tells HID driver how to handle and deal with
   ///  received data.  HID Reports can be extremely complex,
   ///  see HID specifcation for help on writing your own.
   ///
   ///  First it defines a block for the first report ID for a mouse
   ///  protocol.  Second it defines a block for the second report ID
   ///  for a keyboard protocol.
   ///
   //////////////////////////////////////////////////////////////////

   const char USB_CLASS_SPECIFIC_DESC[] = 
   {
     // begin report id==1 (mouse)
      0x05, 0x01, // usage page (generic desktop Choose the usage page "mouse" is on   //0,1
      0x09, 0x02, // usage (mouse) Device is a mouse  //2,3
      0xA1, 0x01, // collection (application) This collection encompasses the report format  //4,5
      0x09, 0x01, // usage (pointer) Choose the key code usage page  //6,7
      0xA1, 0x00, // collection (physical) Physical collection //8,9
      0x05, 0x09, // usage page (buttons) Choose the �button� usage page   //10,11
      0x85, 0x01, //report id (1)   //12, 13
      0x19, 0x01, // usage minimum (1) There are three buttons //14,15
      0x29, 0x03, // usage maximum (3) //16,17
      0x15, 0x00, // logical minimum (0) Each button is represented by one bit   //18,19
      0x25, 0x01, // logical maximum (1)  //20,21
      0x95, 0x03, // report count (3) Three reports, one bit each //22,23
      0x75, 0x01, // report size (1)   //24, 25
      0x81, 0x02, // input (data, variable, absolute) Defined bits above are data bits //26,27
      0x95, 0x01, // report count (1) One report, five bits in length   //28, 29
      0x75, 0x05, // report size (5)   //30, 31
      0x81, 0x01, // input (constant) Bit stuff to fill byte   //32, 33
      0x05, 0x01, // usage page (generic desktop) Choose the usage pare �X� and �Y� are on   //34, 35
      0x09, 0x30, // usage (X) X direction of pointer //36, 37
      0x09, 0x31, // usage (Y) Y direction of pointer //38, 39
      0x09, 0x38,  // usage (wheel)  //40, 41
      0x15, 0x81, // logical minimum (-127) Range of report data is -127 to 127  //42,43
      0x25, 0x7F, // logical maximum (127)   //44, 45
      0x75, 0x08, // report size (8) Two reports, eight bits each //46, 47
      0x95, 0x03, // report count (3)  //48, 49
      0x81, 0x06, // input (data, variable, absolute) Defined bits above are data bits //50, 51
      0xC0,       // end collection End physical collection //52
      0xC0,        // end collection End application collection //53
     // begin report id==2 (keyboard)
      0x05, 0x01, //usage page (generic desktop)   //54, 55
      0x09, 0x06, //usage (keyboard)   //56, 57
      0xA1, 0x01, //collection (application) //58, 59
      0x85, 0x02, //report id (2)   //60, 61
      0x05, 0x07, //usage page (key codes)   //62, 63
      0x19, 0xE0, //usage min (224) //64, 65
      0x29, 0xE7, //usage max (231) //66, 67
      0x15, 0x00, //logical min (0) //68, 69
      0x25, 0x01, //logical max (1) //70, 71
      0x75, 0x01, //report size (1) //72, 73
      0x95, 0x08, //report count (8)   //74, 75
      0x81, 0x02, //input (data, variable, absolute) [modifier byte] //76, 77
      0x95, 0x01, //report count (1)   //78, 79
      0x75, 0x08, //report size (8)    //80, 81
      0x81, 0x01, //input (constant) [reserved byte]  //82, 83
      0x95, 0x05, //report count (5)   //84, 85
      0x75, 0x01, //report size (1)    //86, 87
      0x05, 0x08, //usage page (page# for leds) //88, 89
      0x19, 0x01, //usage min (1)   //90, 91
      0x29, 0x05, //usage max (5)   //92, 93
      0x91, 0x02, //output (data, var, abs) [led report] //94, 95
      0x95, 0x01, //report count (1)   //96, 97
      0x75, 0x03, //report size (3) //98, 99
      0x91, 0x01, //output (constant) [led report padding]  //100, 101
      0x95, 0x05, //report count (5)   //102, 103
      0x75, 0x08, //report size (8) //104, 105
      0x15, 0x00, //logical min (0) //106, 107
      0x25, 0x65, //logical max (101)  //108, 109
      0x05, 0x07, //usage page (key codes)   //110, 111
      0x19, 0x00, //usage min (0)   //112, 113
      0x29, 0x65, //usage max (101) //114, 115
      0x81, 0x00, //input (data, array)   //116, 117
      0xC0,        //end collection  //118
     // begin report id==3 (hiddemo.exe vendor specific)
      6, 0, 255,        // Usage Page = Vendor Defined   //119,120,121
      9, 1,             // Usage = IO device             //122,123
      0xa1, 1,          // Collection = Application      //124,125
      0x85, 0x03, //report id (3)                        //126,127
      0x19, 1,          // Usage minimum                 //128,129
      0x29, 8,          // Usage maximum                 //130,131

      0x15, 0x80,       // Logical minimum (-128)        //132,133
      0x25, 0x7F,       // Logical maximum (127)         //134,135

      0x75, 8,          // Report size = 8 (bits)        //136,137
      0x95, USB_CONFIG_HID_TX_SIZE,        // Report count = 16 bits (2 bytes)   //138,139
      0x81, 2,          // Input (Data, Var, Abs)        //140,141
      0x19, 1,          // Usage minimum                 //142,143
      0x29, 8,          // Usage maximum                 //144,145
      0x75, 8,          // Report size = 8 (bits)        //146,147
      0x95, USB_CONFIG_HID_RX_SIZE,        // Report count = 16 bits (2 bytes)   //148,149
      0x91, 2,          // Output (Data, Var, Abs)       //150,151
      0xc0              // End Collection      //152
      
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
         153
   };

//////////////////////////////////////////////////////////////////
///
///   start config descriptor
///   right now we only support one configuration descriptor.
///   the config, interface, class, and endpoint goes into this array.
///
//////////////////////////////////////////////////////////////////

   #DEFINE USB_TOTAL_CONFIG_LEN      41 //config+interface+class+endpoint

   const char USB_CONFIG_DESC[] = {
   //IN ORDER TO COMPLY WITH WINDOWS HOSTS, THE ORDER OF THIS ARRAY MUST BE:
      //    config(s)
      //    interface(s)
      //    class(es)
      //    endpoint(s)

   //config_descriptor for config index 1
         USB_DESC_CONFIG_LEN, //length of descriptor size          ==0
         USB_DESC_CONFIG_TYPE, //constant CONFIGURATION (CONFIGURATION 0x02)     ==1
         USB_TOTAL_CONFIG_LEN,0, //size of all data returned for this config      ==2,3
         1, //number of interfaces this device supports       ==4
         0x01, //identifier for this configuration.  (IF we had more than one configurations)      ==5
         0x00, //index of string descriptor for this configuration      ==6
        #if USB_CONFIG_BUS_POWER
         0x80, //bit 6=1 if self powered, bit 5=1 if supports remote wakeup (we don't), bits 0-4 unused and bit7=1         ==7
        #else
         0xC0, //bit 6=1 if self powered, bit 5=1 if supports remote wakeup (we don't), bits 0-4 unused and bit7=1         ==7
        #endif
         USB_CONFIG_BUS_POWER/2, //maximum bus power required (maximum milliamperes/2)  (0x32 = 100mA)   ==8

   //interface descriptor 1 (MOUSE)
         USB_DESC_INTERFACE_LEN, //length of descriptor      =9
         USB_DESC_INTERFACE_TYPE, //constant INTERFACE (INTERFACE 0x04)       =10
         0x00, //number defining this interface (IF we had more than one interface)    ==11
         0x00, //alternate setting     ==12
         2, //number of endpoints for this interface  ==13
         0x03, //class code, 03 = HID     ==14
         0x00, //subclass code //boot     ==15
         0x00, //protocol code       ==16
         0x00, //index of string descriptor for interface      ==17

   //class descriptor 1  (HID)
         USB_DESC_CLASS_LEN, //length of descriptor    ==18
         USB_DESC_CLASS_TYPE, //dscriptor type (0x21 == HID)      ==19
         0x00,0x01, //hid class release number (1.0) (try 1.10)      ==20,21
         0x00, //localized country code (0 = none)       ==22
         0x01, //number of hid class descrptors that follow (1)      ==23
         0x22, //report descriptor type (0x22 == HID)                ==24
         USB_CLASS_SPECIFIC_DESC_LOOKUP_SIZE[0][0], 0x00, //length of report descriptor            ==25,26

   //endpoint descriptor 1 IN
         USB_DESC_ENDPOINT_LEN, //length of descriptor                   ==27
         USB_DESC_ENDPOINT_TYPE, //constant ENDPOINT (ENDPOINT 0x05)          ==28
         0x81, //endpoint number and direction (0x81 = EP1 IN)       ==29
         USB_ENDPOINT_TYPE_INTERRUPT, //transfer type supported (0x03 is interrupt)         ==30
         USB_EP1_TX_SIZE,0x00, //maximum packet size supported                  ==31,32
         10,  //polling interval, in ms.  (cant be smaller than 10 for slow speed devices)     ==33

   //endpoint descriptor 1 OUT
         USB_DESC_ENDPOINT_LEN, //length of descriptor                   ==34
         USB_DESC_ENDPOINT_TYPE, //constant ENDPOINT (ENDPOINT 0x05)          ==35
         0x01, //endpoint number and direction (0x01 = EP1 OUT)       ==36
         USB_ENDPOINT_TYPE_INTERRUPT, //transfer type supported (0x03 is interrupt)         ==37
         USB_EP1_RX_SIZE,0x00, //maximum packet size supported                  ==38,39
         10  //polling interval, in ms.  (cant be smaller than 10 for slow speed devices)     ==40
   };


   // this defined used by demos.
   // this is the EP used for all HID reports in this demo.
   #define USB_HID_ENDPOINT   1

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
   const int16 USB_CLASS_DESCRIPTORS[USB_NUM_CONFIGURATIONS][1][1]=
   {
   //config 1
      //interface 0
         //class 1
         18
   };

   //****** END CONFIG DESCRIPTOR LOOKUP TABLES ********

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

// Here is where the "CCS" Manufacturer string and "CCS USB Mouse/Keyboard" are stored.
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
         38, //length of string index
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
         'u',0,
         'l',0,
         't',0,
         'i',0,
         ' ',0,
         'H',0,
         'I',0,
         'D',0,
         '2',0
};
