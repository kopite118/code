////////////////////////////////////////////////////////////////////////////
////                              BQ32000.C                             ////
////                     Driver for Real Time Clock                     ////
////                                                                    ////
////  rtc_init(trickle_charge)                     Call after power up  ////
////       set trickle_charge to TRUE when                              ////
////       using a super cap                                            ////
////                                                                    ////
////  rtc_set_datetime(day,mth,year,dow,hour,min)  Set the date/time    ////
////                                                                    ////
////  rtc_get_date(day,mth,year,dow)               Get the date         ////
////                                                                    ////
////  rtc_get_time(hr,min,sec)                     Get the time         ////
////                                                                    ////
////  rtc_tune(test, adjust)                       Calibrate            ////
////       adjust may be -127 to +127                                   ////
////       test  set to TRUE will output cal signal                     ////
////             on IRQ                                                 ////
////                                                                    ////
///   for dow Sunday is 1                                               ////
////////////////////////////////////////////////////////////////////////////
////                                                                    ////
//// VERSION HISTORY                                                    ////
////                                                                    ////
//// October 2nd, 2014                                                  ////
////  Added time.h compatible functions.                                ////
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

#ifndef __BQ32000_C__
#define __BQ32000_C__

#ifndef RTC_SCL
#define RTC_SCL  PIN_B0
#define RTC_SDA  PIN_B1
#endif

#define RTC_READ_ADDR 0xD1
#define RTC_WRITE_ADDR 0xD0

#use i2c(master, scl=RTC_SCL, sda=RTC_SDA)

void write_rtc_byte(unsigned int8 reg, unsigned int8 data_byte) {

   i2c_start();
   i2c_write(RTC_WRITE_ADDR);
   i2c_write(reg);
   i2c_write(data_byte);
   i2c_stop();
}

unsigned int8 read_rtc_byte(unsigned int8 reg) {
   unsigned int8 data;

   i2c_start();
   i2c_write(RTC_WRITE_ADDR);
   i2c_write(reg);
   i2c_start();
   i2c_write(RTC_READ_ADDR);
   data=i2c_read(0);
   i2c_stop();
   return(data);
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
                      unsigned int8 hour, unsigned int8 min, unsigned int8 sec){
   write_rtc_byte(0,rtc_get_bcd(sec));
   write_rtc_byte(1,rtc_get_bcd(min));
   write_rtc_byte(2,rtc_get_bcd(hour));
   write_rtc_byte(3,rtc_get_bcd(dow));
   write_rtc_byte(4,rtc_get_bcd(day));
   write_rtc_byte(5,rtc_get_bcd(mth));
   write_rtc_byte(6,rtc_get_bcd(year));
}

void rtc_get_date(unsigned int8 &day, unsigned int8 &mth, unsigned int8 &year, unsigned int8 &dow) {
   dow=rtc_rm_bcd(read_rtc_byte(3));
   day=rtc_rm_bcd(read_rtc_byte(4));
   mth=rtc_rm_bcd(read_rtc_byte(5));
   year=rtc_rm_bcd(read_rtc_byte(6));
}

void rtc_get_time(unsigned int8 &hr, unsigned int8 &min, unsigned int8 &sec) {
   i2c_start();
   i2c_write(RTC_WRITE_ADDR);
   i2c_write(0);
   i2c_start();
   i2c_write(RTC_READ_ADDR);
   sec=i2c_read();
   sec=rtc_rm_bcd(sec);
   min=i2c_read();
   min=rtc_rm_bcd(min&0x7f);
   hr=i2c_read(0);
   hr=rtc_rm_bcd(hr);
   i2c_stop();
}

void rtc_init(int1 trickle_charge = 0) {
   if(trickle_charge) {
      write_rtc_byte(8,0x20);
      write_rtc_byte(9,0x45);
   } else {
      write_rtc_byte(9,0);
   }
}

void rtc_tune(int1 test, signed int8 adjust) {
   unsigned int8 data;
   if(test)
      data=0x40;
   else
      data=0;
   adjust=adjust/2;
   if(adjust<0) {
      adjust=adjust/2;
      data|=0x20;
      adjust=-adjust;
   }
   data|=(adjust&0x1F);
   write_rtc_byte(7,data);
}

int1 rtc_get_tune(signed int8 * adjust) {
   unsigned int8 data;
   data=read_rtc_byte(7);
   adjust=(data&0x1f)*2;
   if(data&0x20)
      adjust=-(adjust*2);
   if(data&0x40)
     return(1);
   else
     return(0);
}

// add functions to be time.h compatible
#if defined(__TIME_H__)
#include <time.c>

/* Globals & Resources */
static time_t calendar_time; //seconds since Jan 1, 1970 00:00:00

/* Returns processor time used by program (clock ticks) 
 * The Ds1305 does not use ticks, so return 0
 */
clock_t clock()
{
   return 0;
}

/* Returns the current calendar time.
 * First gets the time from the Ds1305, converts it to seconds,
 * and sets this as the calendar time.
 * If a timer is specified, it will be set to the current calendar time.
 * It is assumed that SetTime has already been called and the Ds1305
 * is initialized.
 */
time_t time(time_t * timer)
{
   struct_tm nTime;
   
   GetTime(&nTime);

   calendar_time = mktime(&nTime);
   
   if(timer != NULL)
   {
      *timer = calendar_time;
   }
   
   return calendar_time;
}

/* Sets the current calendar time (in seconds)from the given time, 
 * and initializes and sets the time/date on the Ds1305.
 */
void SetTime(struct_tm * nTime)
{   
   // date must be adjusted to be Ds1305 compatible
   
   struct_tm tm;
   
   memcpy(&tm, nTime, sizeof(struct_tm));
   
   tm.tm_mday += 1;
   tm.tm_mon += 1;
   tm.tm_year -= 100;
   tm.tm_wday += 1;
   
   /*
   void rtc_set_datetime(unsigned int8 day, unsigned int8 mth, unsigned int8 year, unsigned int8 dow,
                      unsigned int8 hour, unsigned int8 min, unsigned int8 sec){
   */
   rtc_set_datetime(tm.tm_mday, tm.tm_mon, tm.tm_year, tm.tm_wday, tm.tm_hour, tm.tm_min, 0);
   
   calendar_time = mktime(nTime);
}

/* Sets the current time on the DS1305 given a time in seconds and
 * sets the calendar time to this time.
 */
void SetTimeSec(time_t sTime)
{
   struct_tm * nTime;
   nTime = localtime(&sTime);
   // date must be adjusted to be Ds1305 compatible
   rtc_set_datetime(nTime->tm_mday+1, nTime->tm_mon+1, nTime->tm_year-100, nTime->tm_wday+1, nTime->tm_hour, nTime->tm_min, 0);
   calendar_time = sTime;
}

/* Returns the current time from the DS1305 as a time struct
 */
void GetTime(struct_tm *pRetTm)
{
   struct_tm cTime;
   unsigned int8 hour,min,sec,mday,mon,year,wday;
   
   rtc_get_time(hour, min, sec);
   rtc_get_date(mday, mon, year, wday);

   cTime.tm_hour = hour;
   cTime.tm_min = min;
   cTime.tm_sec = sec;
   // adjust values to be time.h compatible
   cTime.tm_mday = mday - 1;   // 0-30
   cTime.tm_mon = mon - 1;    // 0-11
   cTime.tm_year = year + 100; // starting at 1900
   cTime.tm_wday = wday - 1; // 0-6
   
   memcpy(pRetTm, &cTime, sizeof(struct_tm));
}


/* Initializes The DS1305
 */
void TimeInit(void)
{
   rtc_init(0);
}
#endif   //__TIME_H__

#endif
