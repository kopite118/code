/*-------Dinh nghia one-wire-------*/ 
#ifndef ONE_WIRE_PIN
   #define ONE_WIRE_PIN PIN_A0 
#endif

#define OW_MATCH_ROM   0x55
#define OW_SKIP_ROM       0xCC
#define   OW_SEARCH_ROM   0xF0

#define   OW_SEARCH_FIRST   0xFF      // start new search
#define   OW_PRESENCE_ERR   0xFF
#define   OW_DATA_ERR       0xFE
#define OW_LAST_DEVICE   0x00      // last device found
#define OW_CONF_DELAYOFFSET 0
int8 onewire_reset_()  // reset one wire
{
   int8 err;
   output_low(ONE_WIRE_PIN);
   delay_us(480);

   //sreg=SREG;

   // set Pin as input - wait for clients to pull low
   //OW_DIR_IN(); // input
   output_float(ONE_WIRE_PIN);

   delay_us(66);
   err = input(ONE_WIRE_PIN);      // no presence detect
   // nobody pulled to low, still high

   //SREG=sreg; // sei()

   // after a delay the clients should release the line
   // and input-pin gets back to high due to pull-up-resistor
   delay_us(480-66);
   if(input(ONE_WIRE_PIN) == 0 )      // short circuit
   err = 1;

   return err;
}
//======================================== 
int8 ow_bit_io( int8 b )
{
   output_low(ONE_WIRE_PIN);
   delay_us(1); // Recovery-Time wuffwuff was 1
   if ( b ) output_float(ONE_WIRE_PIN);//OW_DIR_IN(); // if bit is 1 set bus high (by ext. pull-up)
   // wuffwuff delay was 15uS-1 see comment above
   delay_us(15-1-OW_CONF_DELAYOFFSET);
   if( input(ONE_WIRE_PIN) == 0 ) b = 0;  // sample at end of read-timeslot
   delay_us(60-15);
   //OW_DIR_IN();
   output_float(ONE_WIRE_PIN);
   return b;
}
//======================================== 
//Ghi vao 1 wire
int8 ghi_1wire_(int8 data) 
{ 
   int8 i = 8, j;
   do
   { j = ow_bit_io( data & 1 );
      data >>= 1;
      if( j ) data |= 0x80;
   }
   while( --i );
   return data;
} 
//====================
//Doc 1 byte tu 1 wire
int8 doc_1wire_() 
{ 
   return ghi_1wire_( 0xFF ); 
} 
//======================================== 
void onewire_reset()  // reset one wire 
{ 
 output_low(ONE_WIRE_PIN); 
 delay_us( 500 ); // keo 1-wire xuong thap de reset
 output_float(ONE_WIRE_PIN); // tha 1-wire len muc cao
 delay_us( 250 ); // cho 1-wire san sang
} 
//====================
//Ghi vao 1 wire
void ghi_1wire(int data) 
{ 
 int dem; // khai bao bien dem 
 for (dem=0; dem<8; ++dem) // Lap lai 8 lan
 { 
  output_low(ONE_WIRE_PIN); // dua chan 1 wire xuong thap 
  output_bit(ONE_WIRE_PIN, shift_right(&data,1,0)); // xuat tung bit du lieu tu 0 - 8 
  delay_us(60); // tao tre 60us
  output_float(ONE_WIRE_PIN); // dua 1-wire len muc cao, 
 } 
} 
//====================
//Doc 1 byte tu 1 wire
int doc_1wire() 
{ 
 int dem, dulieu; // khai bao bien dem va bien du lieu 
 for (dem=0; dem<8; ++dem) // Lap lai 8 lan 
 { 
  output_low(ONE_WIRE_PIN); // dua chan 1 wire xuong thap 
  output_float(ONE_WIRE_PIN); // tha chan 1 wire len cao
  shift_right(&dulieu,1,input(ONE_WIRE_PIN)); // nhan du lieu tu 1 wire. 
  delay_us( 60 ); // cho 1 wire san sang
 } 
 return( dulieu ); //tra ve ket qua cho chuong trinh chinh
} 

/*-------1-wire definitions-------*/ 
//#define ONE_WIRE_PIN PIN_A0

/*******************1-wire communication functions********************/ 

/************onewire_reset*************************************************/ 
/*This function initiates the 1wire bus */ 
/* */ 
/*PARAMETERS: */ 
/*RETURNS: */ 
/*********************************************************************/ 

void onewire_reset_2()  // OK if just using a single permanently connected device 
{ 
 output_low(ONE_WIRE_PIN); 
 delay_us( 500 ); // pull 1-wire low for reset pulse 
 output_float(ONE_WIRE_PIN); // float 1-wire high 
 delay_us( 500 ); // wait-out remaining initialisation window. 
 output_float(ONE_WIRE_PIN); 
} 

/*********************** onewire_write() ********************************/ 
/*This function writes a byte to the sensor.*/ 
/* */ 
/*Parameters: byte - the byte to be written to the 1-wire */ 
/*Returns: */ 
/*********************************************************************/ 

void onewire_write(int data) 
{ 
 int count; 

 for (count=0; count<8; ++count) 
 { 
  output_low(ONE_WIRE_PIN); 
  delay_us( 2 ); // pull 1-wire low to initiate write time-slot. 
  output_bit(ONE_WIRE_PIN, shift_right(&data,1,0)); // set output bit on 1-wire 
  delay_us( 60 ); // wait until end of write slot. 
  output_float(ONE_WIRE_PIN); // set 1-wire high again, 
  delay_us( 2 ); // for more than 1us minimum. 
 } 
} 

/*********************** read1wire() *********************************/ 
/*This function reads the 8 -bit data via the 1-wire sensor. */ 
/* */ 
/*Parameters: */ 
/*Returns: 8-bit (1-byte) data from sensor */ 
/*********************************************************************/ 

int onewire_read() 
{ 
 int count, data; 

 for (count=0; count<8; ++count) 
 { 
  output_low(ONE_WIRE_PIN); 
  delay_us( 2 ); // pull 1-wire low to initiate read time-slot. 
  output_float(ONE_WIRE_PIN); // now let 1-wire float high, 
  delay_us( 8 ); // let device state stabilise, 
  shift_right(&data,1,input(ONE_WIRE_PIN)); // and load result. 
  delay_us( 120 ); // wait until end of read slot. 
 } 

 return( data ); 
} 

