#include <vvvvvvvvvvv.h>



// Connection pin between PIC18F4550 and DHT22 sensor
#BIT Data_Pin = 0x06.0                     // Pin mapped to PORTB.0
#BIT Data_Pin_Direction = 0x86.0             // Pin direction mapped to TRISB.0
void hienthi8led7doan();
char message1[] = "Temp = 00.0 C";
char message2[] = "RH   = 00.0 %";
short Time_out;
unsigned int8 T_byte1, T_byte2, RH_byte1, RH_byte2, CheckSum ,nd;
unsigned int8 const Maled[10] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};
void check(unsigned int8 x){
   if(x > 40)
     {
         output_high(pin_C5);
         output_low(pin_C6);
         output_low(pin_C7);
     }
     else if(x < 30)
     {
         output_high(pin_C6);
         output_low(pin_C5);
         output_low(pin_C7);
     }
     else
     {
         
         output_high(pin_C7);
         output_low(pin_C6);
         output_low(pin_C5);
     }
}
void start_signal(){
  Data_Pin_Direction = 0;              // Configure connection pin as output
  Data_Pin = 0;                        // Connection pin output low
  delay_ms(18);
  Data_Pin = 1;                        // Connection pin output high
  delay_us(30);
  Data_Pin_Direction = 1;              // Configure connection pin as input
}
short check_response(){
  delay_us(40);
  if(!Data_Pin){                     // Read and test if connection pin is low
    delay_us(80);
    if(Data_Pin){                    // Read and test if connection pin is high
      delay_us(50);
      return 1;}
    }
}
unsigned int8 Read_Data(){
  unsigned int8 i, k, _data = 0;     // k is used to count 1 bit reading duration
  if(Time_out)
    break;
  for(i = 0; i < 8; i++){
    k = 0;
    while(!Data_Pin){                          // Wait until pin goes high
      k++;
      if (k > 100) {Time_out = 1; break;}
      delay_us(1);}
    delay_us(30);
    if(!Data_Pin)
      bit_clear(_data, (7 - i));               // Clear bit (7 - i)
    else{
      bit_set(_data, (7 - i));                 // Set bit (7 - i)
      while(Data_Pin){                         // Wait until pin goes low
      k++;
      if (k > 100) {Time_out = 1; break;}
      delay_us(1);}
    }
  }
  return _data;
}

void main(){
  SET_TRIS_D(0x00);
   SET_TRIS_C(0x00);   
 
   // LCD clear
  while(TRUE){
 
    Time_out = 0;
    Start_signal();

    if(check_response()){                    // If there is response from sensor
      RH_byte1 = Read_Data();                 // read RH byte1
      RH_byte2 = Read_Data();                 // read RH byte2
      T_byte1 = Read_Data();                  // read T byte1
      check(T_byte1);
}
  hienthi8led7doan();

  }
}
void hienthi8led7doan(){
  nd=T_byte1;
  output_high(PIN_C0);
   output_d(MaLed[nd/10]);
   delay_us(1000);
   output_low(PIN_C0);
  
   
  
   
   output_d(MaLed[nd%10]);
   output_high(pin_C1);
   delay_us(1000);
   output_low(pin_C1);
   
  
   
   output_d(0x9c);
   output_high(pin_C2);
  delay_us(1000);
   output_low(pin_C2);
   
   
   
   output_d(0xC6);
   output_high(pin_C3);
 delay_us(1000);
   output_low(pin_C3);

}
