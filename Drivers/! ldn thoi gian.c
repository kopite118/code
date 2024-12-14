int8 thu,ngay,thang,nam,gio,phut,giay;
//int8 gio_bin;
//#bit b_f_gio=gio_bin.6  
//#bit b_am_pm=gio_bin.5 
int8 nhuan,ngay_tt;
//========================================
const int8 d_thang[] = {0,0,3,3,6,1,4,6,2,5,0,3,5};
void tinh_thu() 
{
   int8 a;
   int8 b;
   b=nam+5;//31/12/1999: THU 6
   a=nam&0xFC;
   a=a>>2;
   b=a+b;
   if(nam%4==0) // nam nhuan
   {
      if(thang>=3)
      {
         b++;
      }
      b--;
   }
   b=d_thang[thang]+b;
   a=ngay+b;
   a=a%7;
   thu=a+2;
}
//========================================
const int8 tss[] = {31,28,31,30,31,30,31,31,30,31,30,31};
void tinh_ntt()
{
   nhuan=0;
   ngay_tt=tss[thang-1];
   if(nam%4==0)
   {
      if(thang==2)
      {
         ngay_tt=29;
         nhuan=1;
      }
   }
}
//========================================
int8 gio_12;
int8 am_pm;
void h24_h12_c(int8 gio_24)
{
	am_pm=0;
	if(gio_24>12||gio_24==0)
	{
		am_pm=1; //pm
		if(gio_24==0)
		{
			gio_12=12;
		}
		else
		{
			gio_12=gio_24-12;
		}
	}
	else
	{
		gio_12=gio_24;
	}
}