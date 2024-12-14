void truyen_3_dl_595();                                                
void ghi_lenh_lcd(int8 lenh); 
void ghi_kt_lcd(int8 kt);
void khoi_tao_lcd();
void bit_ma_h(int8 ma);
void bit_ma_l(int8 ma);
void lcd_gotoxy(int8 x,int8 y); 
void ghi_byte_lcd(int8 dc,int8 dl) ;
#define tre_lcd_1 10
                                                                                 
int8 b5951,b5952,b5953;                 
#bit rs=b5951.0
#bit en=b5951.1      
#bit d4=b5951.2
#bit d5=b5951.3                                       
#bit d6=b5951.4                     
#bit d7=b5951.5
#bit l10=b5951.1
#bit ll0=b5951.6    
                   
#bit c8=b5952.5 
#bit c9=b5952.4
#bit c10=b5952.3
#bit c11=b5952.2
#bit c12=b5952.1

#bit c1=b5953.7
#bit c2=b5953.6                                                                                                                         
#bit c3=b5953.5                     
#bit c4=b5953.4  
#bit c5=b5953.3
#bit c6=b5953.2
#bit c7=b5953.1 
//========================================
void truyen_595(int8 DuLieu); //Dich 1 byte du lieu toi thanh ghi dich ic 74595               
#define da pin_b7                                                                                                                      
#define sh pin_b6                 
#define st pin_b5                                  
void truyen_595(int8 DuLieu) //Dich 1 byte du lieu
{                              
   int8 i;
   for (i=0; i<8; i++) // Lap lai 8 lan
   {                                         
      output_low (sh);                 
      {
         output_bit(da, shift_left(&DuLieu,1,0)); // xuat tung bit du lieu tu 0 - 8
         delay_us(1); 
         output_high (sh); // dua chan len muc cao 
         delay_us(1); 
         output_low (sh); // dua chan xuong muc thap 
         delay_us(1); 
      }
   }
} 
//========================================
void truyen_3_dl_595()           
{                      
   truyen_595(b5953);      
   truyen_595(b5952);                               
   truyen_595(b5951); 
   output_high(st);    
   output_low(st);
   output_high(st);
}  
//======================================== 
void ghi_4_bit_lcd(unsigned int8 n)
{
  bit_ma_l(n);  
  truyen_3_dl_595(); 
  delay_cycles(1); 
  en=1;
  truyen_3_dl_595(); 
  delay_us(2);
  en=0;                                           
  truyen_3_dl_595();
}
//======================================== 
void ghi_kt_lcd(int8 kt) 
{                         
   ghi_byte_lcd(1,kt);                                                                
} 
//======================================== 
void ghi_lenh_lcd(int8 lenh) 
{                         
   ghi_byte_lcd(0,lenh);
}  
//======================================== 
void ghi_byte_lcd(int8 dc,int8 dl)  
{                                  
   rs = 0; 
   truyen_3_dl_595();
   delay_ms(tre_lcd_1);
   rs = dc; 
   truyen_3_dl_595(); 
   delay_cycles(1);                                                        
   en=0;                                                   
   truyen_3_dl_595();
   ghi_4_bit_lcd(dl >> 4);
   ghi_4_bit_lcd(dl & 0xf);
} 

//======================================== 
void khoi_tao_lcd()            
{       
   #define LCD_TYPE 2                                         
   int8 i;  
   int8 LCD_INIT_STRING[4] = {0x20 | (LCD_TYPE << 2), 0xc, 1, 6}; 
   rs=0;en=0; 
   truyen_3_dl_595();   
   delay_ms(15); 
   for(i=1;i<=3;++i)
   {
       ghi_4_bit_lcd(3);
       delay_ms(5);
   } 
   ghi_4_bit_lcd(2);
   delay_ms(5);      
    
   for(i=0;i<=3;++i)
      ghi_byte_lcd(0,LCD_INIT_STRING[i]); 

}                                           
//======================================== 
void bit_ma_l(int8 ma)
{
   d4=(ma&0x1);
   ma>>=1;                   
   d5=(ma&0x1);
   ma>>=1;
   d6=(ma&0x1);
   ma>>=1;
   d7=(ma&0x1);          
}
//======================================== 
void bit_ma_h(int8 ma)  
{
   ma>>=4;
   d4=(ma&0x1); 
   ma>>=1;
   d5=(ma&0x1);
   ma>>=1;                                                                                        
   d6=(ma&0x1);                                                             
   ma>>=1;
   d7=(ma&0x1);          
}                                      
//========================================  
void lcd_gotoxy(int8 x,int8 y)
{
   int8 address=0;                                      
   if(y!=1)
      address=0x40;       
   address+=x-1;
   ghi_byte_lcd(0,0x80|address);    
}     
