////////////////////////////////////////////////////////////////////////////
////                              MCP7940.C                             ////
////                     Driver for Real Time Clock                     ////
////                                                                    ////
////  rtc_init()                                   Call after power up  ////
////                                                                    ////
////  rtc_set_datetime(day,mth,year,dow,hour,min,leap)  Set date/time   ////
////                                                                    ////
////  rtc_get_date(day,mth,year,dow)               Get the date         ////
////                                                                    ////
////  rtc_get_time(hr,min,sec)                     Get the time         ////
////                                                                    ////
////  rtc_get_bcd(data)                          Convert data to BCD    ////
////                                                                    ////
////  rtc_rm_bcd(data)                           Convert data to int    ////
////                                                                    ////
////////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2013 Custom Computer Services            ////
//// This source code may only be used by licensed users of the CCS C   ////
//// compiler.  This source code may only be distributed to other       ////
//// licensed users of the CCS C compiler.  No other use, reproduction  ////
//// or distribution is permitted without written permission.           ////
//// Derivative programs created using this software in object code     ////
//// form are not restricted in any way.                                ////
////////////////////////////////////////////////////////////////////////////

#ifndef RTC_DATA

#define RTC_SCL  PIN_C3
#define RTC_SDA  PIN_C4
#define RTC_IO   PIN_B2

#endif

#use i2c(scl=RTC_SCL, sda=RTC_SDA, stream=MCP7940)

#define RTCSEC     0
#define RTCMIN     1
#define RTCHOUR    2
#define RTCWKDAY   3
#define RTCDATE    4
#define RTCMTH     5
#define RTCYEAR    6
#define RTCCONTROL 7
#define RTCOSC     8

#define RTCALM0SEC   10
#define RTCALM0MIN   11
#define RTCALM0HOUR  12
#define RTCALM0WKDAY 13
#define RTCALM0DATE  14
#define RTCALM0MTH   15

#define RTCALM1SEC   17
#define RTCALM1MIN   18
#define RTCALM1HOUR  19
#define RTCALM1WKDAY 20
#define RTCALM1DATE  21
#define RTCALM1MTH   22


void write_rtc_byte(unsigned int8 address,unsigned int8 data) {
   i2c_start();
   i2c_write(0xDE);
   i2c_write(address);
   i2c_write(data);
   i2c_stop();
}

unsigned int8 read_rtc_byte(unsigned int8 address) {
   unsigned int8 data;

   i2c_start();
   i2c_write(0xDE);
   i2c_write(address);
   i2c_start();
   i2c_write(0xDF);
   data=i2c_read(FALSE);
   i2c_stop();
   return(data);
}

void rtc_init(signed int8 trim) {
   write_rtc_byte(RTCCONTROL,0x40);
   write_rtc_byte(RTCOSC,trim);
}


unsigned int8 rtc_get_bcd(unsigned int8 data)
{
   int nibh;
   int nibl;

   nibh=data/10;
   nibl=data-(nibh*10);

   return((nibh<<4)|nibl);
}

unsigned int8 rtc_rm_bcd(unsigned int8 data)
{
   unsigned int8 i;

   i=data;
   data=(i>>4)*10;
   data=data+(i & 0x0F);

   return data;
}

void rtc_set_datetime(unsigned int8 day, unsigned int8 mth, unsigned int8 year, unsigned int8 dow,
                      unsigned int8 hour, unsigned int8 min, int1 leap){

   write_rtc_byte(RTCYEAR,rtc_get_bcd(year));
   write_rtc_byte(RTCMTH,rtc_get_bcd(mth)|(leap<<5));
   write_rtc_byte(RTCDATE,rtc_get_bcd(day));
   write_rtc_byte(RTCWKDAY,rtc_get_bcd(dow)|8);
   write_rtc_byte(RTCHOUR,rtc_get_bcd(hour));
   write_rtc_byte(RTCMIN,rtc_get_bcd(min));
   write_rtc_byte(RTCSEC,0X80);
}

void rtc_get_date(unsigned int8 &day, unsigned int8 &mth, unsigned int8 &year, unsigned int8 &dow) {
   year=rtc_rm_bcd(read_rtc_byte(RTCYEAR));
   mth=rtc_rm_bcd(read_rtc_byte(RTCMTH)&0X1f);
   day=rtc_rm_bcd(read_rtc_byte(RTCDATE));
   dow=rtc_rm_bcd(read_rtc_byte(RTCWKDAY)&7);
}

void rtc_get_time(unsigned int8 &hr, unsigned int8 &min, unsigned int8 &sec) {
   hr=rtc_rm_bcd(read_rtc_byte(RTCHOUR));
   min=rtc_rm_bcd(read_rtc_byte(RTCMIN));
   sec=rtc_rm_bcd(read_rtc_byte(RTCSEC)&0X7F);
}
