/////////////////////////////////////////////////////////////////////////////
///                                                                       ///
///                               time.h                                  ///
///                                                                       ///
/// Time algorithms.  This follows the standard C API, with the following ///
/// exceptions:                                                           ///
///   * ctime() and asctime() have another optional input parameter which ///
///     is the pointer to where to save the result string.                ///
///     Normally ctime() and asctime() write the output to a globally     ///
///     allocated string and return a pointer to this string.  By using   ///
///     this optional extra function parameter, you can store the         ///
///     result string into a specified location.                          ///
///   * The output string of ctime() and asctime() are not statically     ///
///     allocated.  That means any other code that runs after ctime()     ///
///     or asctime() can corrupt the result value.  Either perform        ///
///     a string copy of the result into a new location, or use the       ///
///     optional parameter of these functions to store the result         ///

///   * strftime() is not supported.                                      ///
///   * SetTime() is added that initializes/set the current time.         ///
///   * GetTime() is added so you can read directly into a struct_tm,     ///
///      instead of having to use localtime(time()) which is usually      ///
///       innefecient.                                                    ///
///   * TimeInit() added for any real time clock devices that may need    ///
///      to be initialized first.                                         ///
///   * Not all timebases have a tick system, so it's not recommended     ///
///      to use clock().  If you need clock(), look at CCS's              ///
///      #use timer() library.                                            ///
///   * Added timeasc(), which converts a string to a time_t.  It is      ///
///      basically the reverse of asctime().  Returns TRUE if successful  ///
///      conversion, FALSE if not because format is wrong.                ///
///   * Added TIME_T_USES_2010.  Normally time_t is number of seconds     ///
///      since January 1 1970.  However, if time_t is a 32bit number this ///
///      value will rollover sometime in 2032.  If TIME_T_USES_2010 is    ///
///      defined then time_t is number of seconds since January 1 2010,   ///
///      and a 32bit time_t won't rollover until 2072.   Most modern      ///
///      operating systems (like Windows and Linux) uses a 64bit time_t   ///
///      so they aren't effected.  If you are interfacing with another    ///
///      system that uses a normal time_t, then that system will need to  ///
///      add 1262304000 to the value coming from this library.            ///
///      To enable this feature, then #define TIME_T_USES_2010            ///
///                                                                       ///
/// This file only provides the prototypes and definitions needed to      ///
/// proved a time alogrithm that follows the C standard library.  You     ///
/// also need to include/link the actual library that performs the time   ///
/// base.  As of this writing CCS provides the following compatible       ///
/// timebase libraries:                                                   ///
///      rtcperipheral.c - for PICs with internal real time clock.        ///
///      ds1305.c - external DS1305 real time clock.                      ///
///      rtcticks.c - Use a PIC's timer with CCS #use timer() library.    ///
///                                                                       ///
/// API:                                                                  ///
///                                                                       ///
/// Variable definitions:                                                 ///
///   c - clock timer (clock_t), number of ticks since powerup.  See      ///
///       CLOCKS_PER_SECOND to determine clock rate.                      ///
///                                                                       ///
///   t - second timer (time_t), number of seconds since Jan 1st, 1970.   ///
///                                                                       ///
///   ts - time struct (struct_tm), a structure that holds time in        ///
///        descriptive format (seconds, minutes, hours, day, month, etc). ///
///                                                                       ///
/// CLOCKS_PER_SECOND - This is a constant which needs to be defined that ///
///   configures the timebase used by the clock timer and clock().        ///
///   If you are not using clock() then you don't need to define this.    ///
///   If you are using a method such a PIC's timer for the timebase then  ///
///   you will need to set this.                                          ///
///                                                                       ///
/// c = clock() - Return current clock timer.                             ///
///                                                                       ///
/// t = time(*t) - Return current second timer.  Returns twice (as a      ///
///         a return, and saves to input pointer).                        ///
///                                                                       ///
/// SetTime(*tm) - Initializes the current time with a struct_tm          ///
///                                                                       ///
/// SetTimeSec(t) - Initializes the current time with a seconds time      ///
///                                                                       ///
/// t = mktime(*tm) - Converts a time struct to a second timer.           ///
///                                                                       ///
/// t = difftime(t,t) - Returns difference between two second timers.     ///
///                                                                       ///
/// *char = ctime(*t, *char) - Converts second timer to a readable string ///
///                            Www Mmm dd hh:mm:ss yyyy                   ///
///                                                                       ///
/// *char = asctime(*ts, *char) - Converts time struct to a readable      ///
///                            string.                                    ///
///                            Www Mmm dd hh:mm:ss yyyy                   ///
///                                                                       ///
/// *ts = localtime(*t) - Converts second timer to a time struct.         ///
///                  *ts points to a global time struct and will be       ///
///                  corrupted in future calls to localtime().            ///
///                                                                       ///
/// GetTime(*tm) - Returns the current time as a time struct              ///
///                                                                       ///
/// TimeInit() - Initializes the timing device                            ///
///                                                                       ///
///////////////////////////////////////////////////////////////////////////
///                                                                     ///
/// VERSION HISTORY                                                     ///
///                                                                     ///
/// June 9 2015:                                                        ///
///   Fixed the struct_tm typedef so 'struct tm' would be legal         ///
///      syntax.                                                        ///
///   The parameters that CCS added to asctime() and ctime() are now    ///
///      optional.                                                      ///
///   Fixed a bug where tm_mday was sometimes off by 1 day.             ///
///                                                                     ///
/// September 25 2014:                                                  ///
///   Cleaned up documentation.                                         ///
///   Added timeasc().                                                  ///
///   Added TIME_T_USES_2010 compile time option.                       ///
///                                                                     ///
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2011 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS    ////
//// C compiler.  This source code may only be distributed to other    ////
//// licensed users of the CCS C compiler.  No other use,              ////
//// reproduction or distribution is permitted without written         ////
//// permission.  Derivative programs created using this software      ////
//// in object code form are not restricted in any way.                ////
///////////////////////////////////////////////////////////////////////////

#ifndef __TIME_H__
#define __TIME_H__

#include <stdlibm.h>

/* API Types*/
typedef signed int32 time_t;
typedef unsigned int32 clock_t;

typedef enum
{
   SUNDAY = 0,
   MONDAY,
   TUESDAY,
   WEDNESDAY,
   THURSDAY,
   FRIDAY,
   SATURDAY   
}  Weekday;

typedef enum
{
   JANUARY = 0,
   FEBRUARY,
   MARCH,
   APRIL,
   MAY,
   JUNE,
   JULY,
   AUGUST,
   SEPTEMBER,
   OCTOBER,
   NOVEMBER,
   DECEMBER
}  Month;

typedef struct tm
{
   unsigned int8 tm_sec;   // seconds after the minute (0-59)
   unsigned int8 tm_min;   // minutes after the hour (0-59)
   unsigned int8 tm_hour;  // hours since midnight (0-23)
   unsigned int8 tm_mday;  // day of the month (1-31)
   Month tm_mon;           // month of the year (0-11)
   unsigned int16 tm_year; // years since 1900
   Weekday tm_wday;        // day of the week (0-6) (Sunday=0)
   unsigned int16 tm_yday; // day of the year (0-365)
} struct_tm;


/* returns number of ticks incremented at CLOCKS_PER_SECOND rate.  This
   function must be provided by the library of your timebase */
clock_t clock(void);

/* returns time in seconds since January 1 1970 (or January 1 2010 if 
   TIME_T_USES_2010 is being used).  This function must be provided by the
   library of your timebase.  For example, see CCS's ds1305.c driver. */
time_t time(time_t * timer);

/* Returns the difference in seconds between two times
 * Times later and earlier are expressed in seconds 
 */
signed int32 difftime(time_t later, time_t earlier);

/* Calculates the Unix Time from a standard time format
 * Returns the Unix Time (time in seconds since Jan 1, 1970 00:00:00)
 */
time_t mktime(struct_tm * timeT);

/* Returns the given time as a string of the form: 
 *  Day Mon X HH:MM:SS YYYY\0 
 */
char * asctime(struct_tm * timeptr, char *szTime = 0);

/* the inverse of asctime().  you can skip the wday portion of the string, and
   then this function will not populate the tm_wday in pTm.  If you do skip
   the wday portion of the string, then you can use localtime(mktime()) to
   populate tm_wday with the correct value based on the date.  if pTm is
   NULL it won't save result, but function will still return TRUE or FALSE.
   Function returns TRUE if the format of the input string is valid. */
int1 timeasc(struct_tm *pTm, char *szTime);

/* Converts the given calendar time (in seconds) to local time
 * and returns the equivalent string. 
 */
char * ctime(time_t * timer, char *szTime = 0);

/* Converts the given calendar time (in seconds) to local time
 * and sets this time in the global g_lTime
 * Returns a pointer to g_lTime
 */
struct_tm * localtime ( time_t * timer );
#define gmtime(t) localtime(t)

/* Set the time of your system.  This function must pe provided by the library
   of your timebase (see CCS's ds1305.c driver) */
void SetTime(struct_tm * nTime);

/* Set the time of your system.  This function must pe provided by the library
   of your timebase (see CCS's ds1305.c driver) */
void SetTimeSec(time_t sTime);

/* Get the time of your system.  This is similar to the standard time()
   function, but this returns directly to a struct_tm.  Since most RTC's work
   with a structure similar to struct_tm, this may be more effecient to use
   than time().   This function must pe provided by the library of your 
   timebase (see CCS's ds1305.c driver) */
void GetTime(struct_tm *pRetTm);

/* Initialize your timebase. This function must pe provided by the library of your 
   timebase (see CCS's ds1305.c driver) */
void TimeInit(void);

#endif
