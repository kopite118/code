#define RTC_SDA  PIN_c4
#define RTC_SCL  PIN_c3 
#use i2c(master, sda=RTC_SDA, scl=RTC_SCL) 

BYTE bin2bcd(BYTE binary_value); 
BYTE bcd2bin(BYTE bcd_value);
void doc_byte_1307(byte diachi,byte &dulieu);
void ghi_byte_1307(byte diachi,byte dulieu);
void i2c_bat_dau(int8 dia_chi);
void i2c_st();
void i2c_sto();                                                              
void i2c_w();
//========================================
void ds1307_init(void) // khoi tao cho ds1307 
{
   ghi_byte_1307(7,0x90); // tao xung 1 hz tai chan sout DS1307
   byte a; // byte luu tam gia tri o nho 0 cua ds
   doc_byte_1307(0,a); // doc o nho 0
   byte b=a&0x80; // byte dung de so sanh
   if (b!=0)
   {
      a&=0x7f;
      ghi_byte_1307(0,a);
   } 
} 
//======================================== 
void ds1307_dat_tgbt(byte gio_bt,phut_bt)
{ 
   //i2c_start();  
   //i2c_write(0xD0); // I2C write address 
   //i2c_write(0x20); // Start at REG 0 - Seconds   
   //disable_interrupts(global);
   i2c_bat_dau(20);
   i2c_write(gio_bt); // REG 0 
   i2c_write(phut_bt); // REG 1 
   i2c_sto();  
} 
//======================================== 
void ds1307_doc_kieu_thoi_gian(BYTE &kieu)
{
   i2c_bat_dau(22);
   i2c_st();
   i2c_write(0xD1);
   kieu = i2c_read(0);
   i2c_sto();        
}
//======================================== 
void ds1307_dat_kieu_thoi_gian(byte kieu)
{ 
   i2c_bat_dau(22);
   i2c_write(kieu); // REG 0 
   i2c_sto();  
} 
//========================================
/*int8 doc_thu()
{
   int8 t;
   //i2c_start();
   //i2c_write(0xD0);
   //i2c_write(0x00); // Start at REG 0 - Seconds    
   //disable_interrupts(global);
   i2c_bat_dau(3);
   i2c_st();
   i2c_write(0xD1);
   t = i2c_read(0);
   i2c_sto(); 
   return(t);   
} */ 
//======================================== 
void ds1307_doc_tgbt(BYTE &gio_bt,BYTE &phut_bt)
{
   //i2c_start();
   //i2c_write(0xD0);
   //i2c_write(0x20); // Start at REG 0 - Seconds   
   //disable_interrupts(global);
   i2c_bat_dau(20);
   i2c_st();
   i2c_write(0xD1);
   gio_bt = i2c_read(1);
   phut_bt = i2c_read(0);
   i2c_sto();        
}
//========================================
void ds1307_dat_TNTN(ngay,thang,nam)
{ 
   //i2c_start();  
   //i2c_write(0xD0); // I2C write address 
   //i2c_write(0x03); // Start at REG 0 - Seconds 
   //disable_interrupts(global);   
   i2c_bat_dau(4);
   i2c_write(bin2bcd(ngay)); // REG 0    
   i2c_write(bin2bcd(thang)); // REG 1 
   i2c_write(bin2bcd(nam)); // REG 2 
   i2c_write(0x90);            // REG 7 - Enable squarewave output pin 
   i2c_sto(); 
} 
//========================================
void ds1307_dat_NTN(byte ngay,thang,nam)
{ 
   //i2c_start();  
   //i2c_write(0xD0); // I2C write address 
   //i2c_write(0x04); // Start at REG 0 - Seconds
   //disable_interrupts(global);
   i2c_bat_dau(4);
   i2c_write(bin2bcd(ngay)); // REG 0 
   i2c_write(bin2bcd(thang)); // REG 1 
   i2c_write(bin2bcd(nam)); // REG 2 
   i2c_write(0x90);            // REG 7 - Enable squarewave output pin 
   i2c_sto();    
} 
//========================================
void ds1307_dat_GPG(byte gio,phut,giay) 
{ 
  //giay &= 0x7F; 
  //gio &= 0x3F; 
   //disable_interrupts(global);                                       
   //i2c_start();  
   //i2c_write(0xD0); // I2C write address 
   //i2c_write(0x00); // Start at REG 0 - Seconds 
   i2c_bat_dau(0);
   i2c_write(bin2bcd(giay)); // REG 0 
   i2c_write(bin2bcd(phut)); // REG 1 
   i2c_write(bin2bcd(gio)); // REG 2 
   i2c_sto();   
} 
//========================================
/*void ds1307_set_date_time(BYTE gio, phut, giay, thu, ngay, thang, nam) 
{
   //disable_interrupts(global);
  //giay &= 0x7F; 
  //gio &= 0x3F; 
                             
  //i2c_start();  
  //i2c_write(0xD0);            // I2C write address 
  //i2c_write(0x00);            // Start at REG 0 - Seconds 
  i2c_bat_dau(0);
  i2c_write(bin2bcd(giay));      // REG 0 
  i2c_write(bin2bcd(phut));      // REG 1 
  i2c_write(bin2bcd(gio));      // REG 2 
  i2c_write(bin2bcd(thu));      // REG 3     
  i2c_write(bin2bcd(ngay));      // REG 4 
  i2c_write(bin2bcd(thang));      // REG 5 
  i2c_write(bin2bcd(nam));      // REG 6 
  i2c_write(0x90);            // REG 7 - Enable squarewave output pin 
  i2c_sto();    
} */
//========================================
void ds1307_doc_giay(BYTE &giay)
{
   //i2c_start();
   //i2c_write(0xD0);                            
   //i2c_write(0x00); // Start at REG 0 - Seconds 
   //disable_interrupts(global);
   i2c_bat_dau(0);
   i2c_st();
   i2c_write(0xD1);
   giay = bcd2bin(i2c_read(0));
   i2c_sto();    
}
//========================================
void ds1307_get_time(BYTE &gio,BYTE &phut,BYTE &giay)
{
   //i2c_start();
   //i2c_write(0xD0);
   //i2c_write(0x00); // Start at REG 0 - Seconds
   //disable_interrupts(global);
   i2c_bat_dau(0);
   i2c_st();
   i2c_write(0xD1);
   giay = bcd2bin(i2c_read(1));
   phut = bcd2bin(i2c_read(1));
   gio = bcd2bin(i2c_read(0));
   i2c_sto();    
}
//========================================
void ds1307_get_date(BYTE &ngay,BYTE &thang,BYTE &nam)
{
   //i2c_start();
   //i2c_write(0xD0);
   //i2c_write(3);            // Start at REG 3 - Day of week  
   //disable_interrupts(global);
   i2c_bat_dau(4);
   //i2c_sto();
   i2c_st();
   i2c_write(0xD1);
   ngay  = bcd2bin(i2c_read(1));   // REG 4
   thang = bcd2bin(i2c_read(1));   // REG 5
   nam = bcd2bin(i2c_read(0));            // REG 6
   i2c_sto();
}
//========================================

BYTE bin2bcd(BYTE binary_value)//19 
{
   BYTE temp; 
   BYTE retval; 
   temp = binary_value; 
   retval = 0; 
   while(true) 
   { // Get the tens digit by doing multiple subtraction 
      if(temp >= 10) // of 10 from the binary value.
      { 
         temp -= 10; 
         retval += 0x10; 
      } 
      else // Get the ones digit by adding the remainder. 
      { 
         retval += temp; 
         break; 
      } 
   }               
   return(retval); 
} 
//========================================

// Input range - 00 to 99. 
BYTE bcd2bin(BYTE bcd_value) //19
{
  byte tam; 
  tam = bcd_value; 
  tam >>= 1; // Shifting upper digit right by 1 is same as multiplying by 8.
  tam &= 0x78; // Isolate the bits for the upper digit. 
  return(tam + (tam >> 2) + (bcd_value & 0x0f)); // Now return: (Tens * 8) + (Tens * 2) + Ones 
  //return ( (bcd_val/16*10) + (bcd_val%16) );
}
//======================================== 
void doc_byte_1307(byte diachi,byte &dulieu)
{
   //i2c_start();
   //i2c_write(0xD0); // ghi vao ds1307 chieu ghi
   //i2c_write(diachi); // ghi dia chi can doc   
   //disable_interrupts(global);
   i2c_bat_dau(diachi);
   i2c_st(); // start bit i2c 
   i2c_write(0xD1); // ghi vao ds1307 chieu doc
   dulieu = i2c_read(0);
   i2c_sto();  
}
//======================================== 
void ghi_byte_1307(byte diachi,byte dulieu)
{
   //i2c_start();
   //i2c_write(0xD0); // ghi vao ds1307 chieu ghi
   //i2c_write(diachi); // ghi dia chi can doc
   //disable_interrupts(global);
   i2c_bat_dau(diachi);
   i2c_write(dulieu); // 
   i2c_sto();   
}
//======================================== 
void i2c_bat_dau(int8 dia_chi)
{    
   i2c_st();
   i2c_write(0xD0); // ghi vao ds1307 chieu ghi
   i2c_write(dia_chi); // ghi dia chi can doc
}
//======================================== 
void i2c_st()
{              
   disable_interrupts(global); 
   i2c_start();   
}
//======================================== 
void i2c_sto()
{
   i2c_stop(); 
   enable_interrupts(global);
}
