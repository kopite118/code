#include <16F877A.h>
#device ADC=16

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES HS

#include <dht.h>
#define LE_1   PIN_C0
#define LE_2   PIN_C1
#define LE_3   PIN_C2
#define LE_4   PIN_C3
