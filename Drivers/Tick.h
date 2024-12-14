/******************************************************************************

   Tick.h
   
   Uses a PIC's timer peripheral to create a tick system that increments at
   a rate of TICKS_PER_SECOND.  On PIC12-PIC16 Timer 1 is used. On PIC18 
   Timer 0 is used.  On PIC24, dsPIC30, dsPIC33 and PIC32 Timer 1 is used.
   
   Interrupt usage is optional.  If you want to use interrupts, define
   TICK_USES_ISR to configure library to use interrupts.  The library
   will enable the proper peripheral/timer interrupt, if using an 
   8-bit PICmicro (PIC16, PIC18, etc) then the user will need to enable
   the global interrupt enable (enable_interrupts(GLOBAL)).
 
   If interrupts are not used then the timer overflow is polled in TickGet().
   If you are not polling TickGet()/TickTask() often, then add a
   TickGet() in your main loop to check for overflows.  On PIC12-PIC16, at
   maximum oscillator speed the overflow will happen over 9 times a second;
   that means TickGet()/TickTask() must be called more than 9 times a second
   on these PICs or there will be some timing errors.  On a PIC24/dsPIC33 at
   140MHz (70MIPS) the overflow will happen over 4 times a second.
   
   A #use delay() must have already been used before including this file so the
   library knows what clock speed is being used.
   
   This library largely acts as a wrapper to the built-in library #use timer().

*****************************************************************************/
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

#ifndef __TICK_H__
#define __TICK_H__

#include <stdint.h>

// Data type returned by TickGet()
#if defined(TICK_IS_16BITS)
typedef uint16_t TICK;
#define __TICK_RESOLUTION_BITS 16
#else
typedef uint32_t TICK;
#define __TICK_RESOLUTION_BITS 32
#endif

// Initialize the Tick system, including turning on and configuring the proper
// timer peripheral on the PICmicro.
void TickInit(void);

// Get current TICK time.  TICK increments at a rate of TICKS_PER_SECOND.
TICK TickGet(void);

#define TickTask() TickGet()

// Delay (deadlock) for 'duration' ticks.
void TickDelay(TICK duration);

// Delay (deadlock) for 'duration' ticks, but restart WDT during delay.
void TickDelayWDT(TICK duration);

#define TickIsExpired(start, duration) ((TickGet() - (TICK)start) > (TICK)(duration))

#if defined(__PCD__) || defined(__PCM__)
   //PIC12-PIC16, PIC24, dsPIC30, dsPIC33
   #define __CCS_USE_TIMER_PERIPH 1
#else
   //PIC18
   #define __CCS_USE_TIMER_PERIPH 0
#endif

#if defined(TICK_USES_ISR)
   #use timer(timer=__CCS_USE_TIMER_PERIPH, tick=1ms, stream=STREAM_TIMER_TICK, bits=__TICK_RESOLUTION_BITS, ISR, define=TICKS_PER_SECOND)
#else
   #use timer(timer=__CCS_USE_TIMER_PERIPH, tick=1ms, stream=STREAM_TIMER_TICK, bits=__TICK_RESOLUTION_BITS, NOISR, define=TICKS_PER_SECOND)
#endif
   
#endif   //ifndef __TICK_H__
