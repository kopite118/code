/*
   Tick.c
   
   Uses a PIC's timer peripheral to create a tick system that increments at
   a rate of TICKS_PER_SECOND.
   
   See Tick.h for documentation.
*/
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2015 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
////                     http://www.ccsinfo.com                        ////
///////////////////////////////////////////////////////////////////////////

#ifndef __TICK_C__
#define __TICK_C__

#include <Tick.h>

void TickInit(void)
{
   // nothing to do, this is done by the #use timer
}

TICK TickGet(void)
{
   return(get_ticks(STREAM_TIMER_TICK));
}

void TickDelay(TICK duration)
{
	TICK t;
	
	t = TickGet();
	
	while(!TickIsExpired(t, duration)) { }
}

// Delay (deadlock) for 'duration' ticks, but restart WDT during delay.
void TickDelayWDT(TICK duration)
{
	TICK t;
	
	t = TickGet();
	
	while(!TickIsExpired(t, duration))	
	{
		restart_wdt();
	}
}

#endif   //ifndef __TICK_H__
