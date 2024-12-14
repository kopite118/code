//typedef unsigned char u8;
//#include <math.h>
typedef unsigned int u16;
typedef unsigned long u32;
typedef unsigned char uchar;
//typedef unsigned char byte;


//!#define ohm 0xf4                    
//!#define B_coefficient 3950.00      /* B coefficient of NTC Thermistor*/
//!#define Room_temperature 25.00
//!#define Series_Resistance 10000.00
//!

//!int16 Get_Temperature(int16 analog_value)
//!{
//!    int16 Thermistor_temperature;
//!    int16 NTC_Resistance;
//!    //analog_value = read_adc_lap();		/* store adc value on val register */
//!
//!    /* calculate the NTC resistance */
//!	NTC_Resistance = ((10230000/analog_value) - 10000);		  	
//!	Thermistor_temperature = log(NTC_Resistance);	/* calculate natural log of resistance */
//!
//!    /* Calculate Temperature using B parameter Equation */
//!    /* 1/T = 1/T0 + ((1/B_coefficient)*log(NTC_Resistance/Series_Resistance)) */
//!//    Thermistor_temperature = (1.0/(Room_temperature + 273.15))+
//! //                            (1.0/B_coefficient)*log(NTC_Resistance/Series_Resistance);
//!  //  Thermistor_temperature = (1/Thermistor_temperature) - 273.15;	/* convert kelvin to °C */
//!	return Thermistor_temperature;
//!}

//!float Get_Temperature1(int analog_value)
//!{
//!    float Thermistor_temperature;
//!    long NTC_Resistance;
//!    //analog_value = read_adc_lap();		/* store adc value on val register */
//!
//!    /* calculate the NTC resistance */
//!	NTC_Resistance = ((1023*Series_Resistance/analog_value) - Series_Resistance);		  	
//!	Thermistor_temperature = log(NTC_Resistance);	/* calculate natural log of resistance */
//!
//!    /* Calculate Temperature using B parameter Equation */
//!    /* 1/T = 1/T0 + ((1/B_coefficient)*log(NTC_Resistance/Series_Resistance)) */
//!    Thermistor_temperature = (1.0/(Room_temperature + 273.15))+
//!                             (1.0/B_coefficient)*log(NTC_Resistance/Series_Resistance);
//!    Thermistor_temperature = (1/Thermistor_temperature) - 273.15;	/* convert kelvin to °C */
//!  	
//!	return Thermistor_temperature;
//!}
//!
long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

long map_(long x, long in_min, long in_max, long out_min, long out_max)
{
  return out_max-(x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
