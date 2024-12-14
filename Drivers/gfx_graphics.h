///////////////////////////////////////////////////////////////////////////////
//                               gfx_graphics.h                              //
//                                                                           //
// Header file for a gfx_graphics.c library.  See gfx_graphics.c for API,    //
// documentation and function descriptions.                                  //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
//             (C) Copyright 1996, 2015 Custom Computer Services             //
//   This source code may only be used by licensed users of the CCS C        //
//   compiler.  This source code may only be distributed to other licensed   //
//   users of the CCS C compiler.  No other use, reproduction or             //
//   distribution is permitted without written permission.  Derivative       //
//   programs created using this software in object code form are not        //
//   restricted in any way.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef _GFX_GRAPHICS_H_
#define _GFX_GRAPHICS_H_

#include <stdint.h>

/////////////////////////////// External Memory ///////////////////////////////

#ifndef MAX_IMAGES
 #define MAX_IMAGES       512
#endif

#ifndef MAX_IMAGE_AREAS
 #define MAX_IMAGE_AREAS   32
#endif

#ifndef MAX_FONT_WIDTH
 #define MAX_FONT_WIDTH    128
#endif

#ifndef MAX_FONT_HEIGHT
 #define MAX_FONT_HEIGHT   128
#endif

#ifndef MAX_FONT_CHARACTERS
 #define MAX_FONT_CHARACTERS  95 //space to ~
#endif

typedef enum 
{
   AREA_TYPE_BUTTON,   //Area is a Button
   AREA_TYPE_TEXT,     //Area is a Text Box
   AREA_TYPE_OTHER
} AREA_TYPE;

typedef struct
{
   uint16_t x1;         //(x1,y1) - top left hand corner of area, referenced to
   uint16_t y1;         //top left hand corner of image
   uint16_t x2;         //(x2,y2) - bottom right hand corner of area,
   uint16_t y2;         //referenced to top left hand corner of image
   AREA_TYPE Type;      //Area type
} AREA_INFO;

typedef struct
{
   uint16_t Index;      //Image Index - used to determine if Image has been written to flash, -1 no image in location
   uint16_t Width;      //Image Width - shouldn't exceed max width of display
   uint16_t Height;     //Image Height - shouldn't exceed max height of display (LCD Lines)
   uint8_t AreaCount;   //Number of Areas within an Image
   AREA_INFO Area[MAX_IMAGE_AREAS]; //Area Info
} IMAGE_HEADER;

typedef struct
{
   IMAGE_HEADER Header;          //Image Header
   FLASH_ADDR BitmapAddress;    //Address in Flash the Image actually starts
} IMAGE;

typedef struct
{
   uint16_t Index;         //Font Index - used to determine if Font has been written to flash, -1 no font in location
   uint8_t Width;          //Character Width
   uint8_t Height;         //Character Height
   uint8_t Count;          //Number of Characters
   char CharacterStart;    //The first character of font
} FONT_HEADER;

typedef struct
{
   FONT_HEADER Header;           //Font Header
   FLASH_ADDR BitmapAddress;    //Address in Flash the Font actually starts
} FONT;

#define IMAGE_BLOCKS       ((((GLCD_PIXELS * GLCD_LINES) + (sizeof(IMAGE)/2)) / FLASH_ERASE_SIZE) + (((((GLCD_PIXELS * GLCD_LINES)  + (sizeof(IMAGE)/2)) % FLASH_ERASE_SIZE) > 0) ? 1 : 0))

#if (MAX_IMAGES * IMAGE_BLOCKS) > FLASH_BLOCKS
 #error Flash not large enough, decrease maximum number of images
#endif

#define FONT_START_BLOCK   (MAX_IMAGES * IMAGE_BLOCKS)

#define FONT_SIZE          ((((MAX_FONT_HEIGHT / 8) + (((MAX_FONT_HEIGHT % 8) > 0) ? 1 : 0)) * MAX_FONT_WIDTH) + ((((((MAX_FONT_HEIGHT / 8) + (((MAX_FONT_HEIGHT % 8) > 0) ? 1 : 0)) * MAX_FONT_WIDTH) % 2) > 0) ? 1 : 0))

#define FONT_BLOCKS        ((((FONT_SIZE * MAX_FONT_CHARACTERS) + ((sizeof(FONT) + (((sizeof(FONT) % 2) > 0) ? 1 : 0)))) / (FLASH_ERASE_SIZE * 2)) + (((((FONT_SIZE * MAX_FONT_CHARACTERS) + ((sizeof(FONT) + (((sizeof(FONT) % 2) > 0) ? 1 : 0)))) % (FLASH_ERASE_SIZE * 2)) > 0) ? 1 : 0))

#ifndef MAX_FONTS
 #define MAX_FONTS         ((FLASH_BLOCKS - FONT_START_BLOCK) / FONT_BLOCKS)
 
 #if MAX_FONTS == 0
  #error Flash not large enough, decrease maximum number of images
 #endif
 
 #error/information Maximum Number of Fonts is MAX_FONTS 
#else
 #if (FONT_START_BLOCK + (MAX_FONTS * FONT_BLOCKS)) > FLASH_BLOCKS
  #error Flash not large enough, decrease maximum number of images or fonts
 #elif (FONT_START_BLOCK + (MAX_FONTS * FONT_BLOCKS)) < FLASH_BLOCKS
  #define FIRST_FREE_BLOCK    (FONT_START_BLOCK + (MAX_FONTS * FONT_BLOCKS))
 #endif
 
 #ifdef FIRST_FREE_BLOCK
  #error/information First Free Flash Block is FIRST_FREE_BLOCK
 #endif
#endif

typedef enum
{
   IMAGE_ERR_OK,
   IMAGE_ERR_INDEX = 6,
   IMAGE_ERR_MEMORY,
   IMAGE_ERR_SIZE,
   IMAGE_ERR_NOT_LOADED,
   IMAGE_ERR_INDEX_ADDRESS,
   IMAGE_ERRFLASH_ADDRESS,
   IMAGE_ERR_DONE,
} IMAGE_ERR;

typedef enum
{
   FONT_ERR_OK,
   FONT_ERR_INDEX = 6,
   FONT_ERR_MEMORY,
   FONT_ERR_SIZE,
   FONT_ERR_NOT_LOADED,
   FONT_ERR_INDEX_ADDRESS,
   FONT_ERRFLASH_ADDRESS,
   FONT_ERR_DONE,
} FONT_ERR;

//Image and Font Prototypes
void gfx_LoadImage(uint16_t Count, uint8_t *Data, int1 New);
void gfx_EraseImage(uint16_t Index);
void gfx_LoadFont(uint16_t Count, uint8_t *Data, int1 New);
void gfx_EraseFont(uint16_t Index);

IMAGE_ERR GetImageAddress(uint16_t ImageIndex, FLASH_ADDR *Address);
IMAGE_ERR GetImageHeaderAddress(uint16_t ImageIndex, FLASH_ADDR *Address);
IMAGE_ERR GetImageHeader(uint16_t ImageIndex, IMAGE_HEADER *Header);
FONT_ERR GetFontAddress(uint16_t FontIndex, FLASH_ADDR *Address);
FONT_ERR GetFontHeaderAddress(uint16_t FontIndex, FLASH_ADDR *Address);
FONT_ERR GetFontInfo(uint16_t FontIndex, FONT *FontData);

//////////////////////////////////// Touch ////////////////////////////////////

#ifndef TOUCH_UPDATE_RATE
 #define TOUCH_UPDATE_RATE    (GFX_TICKS_PER_SECOND / 10) //100ms
#endif

#define NOT_TOUCHED FALSE
#define TOUCHED TRUE

typedef enum {TOUCH_TYPE_PRESSED, TOUCH_TYPE_RELEASED, TOUCH_TYPE_PRESSING} TOUCH_TYPE;

typedef void(*_touchpfunc)(uint16_t HandleId, uint8_t AreaIndex, TOUCH_TYPE Kind, uint16_t x, uint16_t y);

_touchpfunc g_TouchFunctionPtr = NULL;
int1 g_TouchOnlyValidAreas;
int1 g_TouchOnlyUpDown;

typedef struct {
   int16_t Slope;
   int16_t Offset;
   int1 Reversed;
} TOUCH_CAL_DATA;

typedef struct{
   TOUCH_CAL_DATA X;
   TOUCH_CAL_DATA Y;
   int1 Rotated;
} TOUCH_CAL;

TOUCH_CAL gfx_TouchCalibration = {{0, 0, 0}, {0, 0, 0}, 0};

//Touch Prototypes
void gfx_TouchTask(void);
void gfx_SetTouchCallback(_touchpfunc func, int1 OnlyValidAreas, int1 OnlyUpDown);
void TouchCallback(TOUCH_TYPE Kind, uint16_t RealX, uint16_t RealY);
void TouchFunction(uint16_t HandleId, uint8_t AreaIndex, TOUCH_TYPE Kind, uint16_t x, uint16_t y);

/////////////////////////////////// Display ///////////////////////////////////

#ifndef TAB_CHARACTERS
 #define TAB_CHARACTERS    3 
#endif

#define DISPLAY_HORIZONTAL 0
#define DISPLAY_VERTICAL   1

int1 gfx_DisplayOrientation = DISPLAY_HORIZONTAL;   //0-0 degrees, 1-90 degrees

uint16_t gfx_HandleId = 0;
uint16_t gfx_HandleCount = 0;

typedef struct {
   uint8_t AreaIndex;
   AREA_INFO AreaInfo;
} AREA_STRUCT;

typedef struct {
   uint16_t HandleId;
   uint16_t ImageIndex;
   uint16_t x;
   uint16_t y;
   uint8_t AreaCount;
   AREA_STRUCT **Area;
} HANDLE_STRUCT;

HANDLE_STRUCT *gfx_ImageHandle = NULL;

typedef enum{FILL_TOP_TO_BOTTOM, FILL_BOTTOM_TO_TOP, FILL_LEFT_TO_RIGHT, FILL_RIGHT_TO_LEFT, FILL_ALL} FILL_DIR;

//Display Prototypes
void gfx_InitGraphics(TOUCH_CAL TouchCalibration);
uint16_t gfx_DisplayImage(uint16_t Index, uint16_t x, uint16_t y);
void gfx_RemoveImage(uint16_t HandleId, int1 RedrawScreen = FALSE);
void gfx_RemoveAllImages(void);
void gfx_ClearScreen(uint16_t Color, int1 RemoveHandles = TRUE);
void gfx_RedrawScreen(void);
void gfx_FillArea(uint16_t ImageHandle, uint8_t AreaIndex, FILL_DIR Direction, uint32_t Pixels, uint16_t Color);
void gfx_FillAreaAbsolute(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t Color);
void gfx_DrawLineAbsolute(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t Color);
void gfx_DrawCircleAbsolute(uint16_t x, uint16_t y, uint16_t Radius, uint16_t Color, int1 Fill=FALSE);
void gfx_DisplayString(uint16_t ImageHandle, uint8_t AreaIndex, uint8_t FontIndex, uint16_t Fcolor, uint16_t Bcolor, char *Text);
void gfx_DisplayStringAbsolute(uint16_t x, uint16_t y, uint8_t FontIndex, uint16_t Fcolor, uint16_t Bcolor, char *Text);

void DrawBitmap(uint16_t StartX, uint16_t StartY, uint16_t Width, uint16_t Height, uint16_t *Data);
void DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void DrawRectangle(uint16_t StartX, uint16_t StartY, uint16_t StopX, uint16_t StopY, uint16_t Color, int1 Fill = FALSE);
int1 WriteImage(uint16_t ImageIndex, uint16_t StartX, uint16_t StartY);

uint16_t AdHandle(uint16_t ImageIndex, uint16_t x, uint16_t y);
int1 HasHandle(uint16_t HandleId);
int1 GetHandle(uint16_t HandleId, HANDLE_STRUCT *Handle);
uint16_t GetHandleId(void);
int1 GetArea(uint8_t AreaIndex, HANDLE_STRUCT *Handle, AREA_INFO *Area);
void GetCharacter(FONT *Font, char Character, uint8_t *Ptr);

////////////////////////////////// Backlight //////////////////////////////////

#ifdef GFX_BACKLIGHT_PIN
 #ifndef GFX_BACKLIGHT_TIMER
  #define GFX_BACKLIGHT_TIMER       1
 #endif

 #ifndef GFX_BACKLIGHT_FREQUENCY
  #define GFX_BACKLIGHT_FREQUENCY   25000
 #endif

 #ifndef GFX_BACKLIGHT_START_DUTY
  #define GFX_BACKLIGHT_START_DUTY  100
 #endif

 #ifndef GFX_BACKLIGHT_START_ON
  #define GFX_BACKLIGHT_START_ON    FALSE
 #endif

 #if GFX_BACKLIGHT_START_ON == TRUE
  #use pwm(output=GFX_BACKLIGHT_PIN, timer=GFX_BACKLIGHT_TIMER, frequency=GFX_BACKLIGHT_FREQUENCY, duty=GFX_BACKLIGHT_START_DUTY, PWM_ON, stream=GFX_BACKLIGHT_STREAM)
 #else
  #use pwm(output=GFX_BACKLIGHT_PIN, timer=GFX_BACKLIGHT_TIMER, frequency=GFX_BACKLIGHT_FREQUENCY, duty=GFX_BACKLIGHT_START_DUTY, PWM_OFF, stream=GFX_BACKLIGHT_STREAM)
 #endif

 #define gfx_BacklightOn()    pwm_on(GFX_BACKLIGHT_STREAM)
 #define gfx_BacklightOff()   pwm_off(GFX_BACKLIGHT_STREAM)
 
 #define BACKLIGHT_MAX_DUTY     1000
 
 uint16_t g_BacklightLevel, g_CurrentBacklightLevel;
 
 //Backlight Prototypes
 void gfx_InitBacklight(void);
 void gfx_SetBacklightLevel(uint8_t pLevel);
 void gfx_BacklightTask(void);
#endif

//////////////////////////////////// Serial ///////////////////////////////////

#ifdef GFX_USE_SERIAL_INTERFACE

#ifndef gfx_kbhit
 #define gfx_kbhit   kbhit
#endif

#ifndef gfx_getc
 #define gfx_getc    getc
#endif

#ifndef gfx_putc
 #define gfx_putc    putc
#endif

#ifndef MAX_SERIAL_DATA_SIZE
 #define MAX_SERIAL_DATA_SIZE          1024
#endif

#ifndef SERIAL_BUFFERS
 #define SERIAL_BUFFERS                3
#endif

#define SERIAL_START_BYTE              0xAA

#define SERIAL_CMD_IMAGE_START         0x01
#define SERIAL_CMD_IMAGE_CONT          0x02
#define SERIAL_CMD_FONT_START          0x03
#define SERIAL_CMD_FONT_CONT           0x04

#define SERIAL_CMD_ACK                 0x06
#define SERIAL_CMD_NACK                0x15

#define SERIAL_CMD_DISPLAY_IMAGE       0x05
#define SERIAL_CMD_REMOVE_IMAGE        0x07
#define SERIAL_CMD_CLEAR_SCREEN        0x08
#define SERIAL_CMD_DISPLY_STRING       0x09
#define SERIAL_CMD_FILL_AREA           0x0A
#define SERIAL_CMD_GET_LOCATION        0x0B

#define SERIAL_CMD_SET_TOUCH_CALLBACK  0x0C

#define SERIAL_CMD_DISPLAY_STRING_AB   0x0E
#define SERIAL_CMD_FILL_AREA_AB        0x0F
#define SERIAL_CMD_DRAW_LINE_AB        0x10

#define SERIAL_CMD_ERASE_FLASH         0x11
#define SERIAL_CMD_ERASE_IMAGE         0x12
#define SERIAL_CMD_ERASE_FONT          0x13

typedef enum{SERIAL_START, SERIAL_CMD, SERIAL_SIZE, SERIAL_DATA, SERIAL_CS} SERIAL_RX_MODE;

typedef struct
{
   uint8_t Cmd;
   uint16_t Count;
   uint8_t Data[MAX_SERIAL_DATA_SIZE];
} SERIAL_STRUCT;

SERIAL_STRUCT g_SerialBuffer[SERIAL_BUFFERS];

#if SERIAL_BUFFERS > 256
 uint16_t g_SerialNextIn = 0;
 uint16_t g_SerialNextOut = 0;
#else
 uint8_t g_SerialNextIn = 0;
 uint8_t g_SerialNextOut = 0;
#endif

//Serial Prototypes
void gfx_SerialTask(void);
void SerialRxTask(void);
int1 SerialCommandKbhit(void);
void SendAckNack(uint8_t Command);
uint8_t SerialSendByte(uint8_t Data, uint8_t cs);
void SerialSendData(uint8_t SerialCmd, uint8_t *Data, uint16_t Count);
#endif

/////////////////////////////////// 8x8 Font //////////////////////////////////

const uint8_t FONT8x8[95][8] =      {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // SPACE
                                     0x00, 0x00, 0x00, 0x5F, 0x00, 0x00, 0x00, 0x00, // !
                                     0x00, 0x00, 0x03, 0x00, 0x03, 0x00, 0x00, 0x00, // "
                                     0x00, 0x14, 0x3E, 0x14, 0x3E, 0x14, 0x00, 0x00, // #
                                     0x00, 0x24, 0x2A, 0x7F, 0x2A, 0x12, 0x00, 0x00, // $
                                     0x00, 0x43, 0x33, 0x08, 0x66, 0x61, 0x00, 0x00, // %
                                     0x00, 0x36, 0x49, 0x55, 0x22, 0x50, 0x00, 0x00, // &
                                     0x00, 0x00, 0x05, 0x03, 0x00, 0x00, 0x00, 0x00, // '
                                     0x00, 0x00, 0x1C, 0x22, 0x41, 0x00, 0x00, 0x00, // (
                                     0x00, 0x00, 0x41, 0x22, 0x1C, 0x00, 0x00, 0x00, // )
                                     0x00, 0x14, 0x08, 0x3E, 0x08, 0x14, 0x00, 0x00, // *
                                     0x00, 0x08, 0x08, 0x3E, 0x08, 0x08, 0x00, 0x00, // +
                                     0x00, 0x00, 0x50, 0x30, 0x00, 0x00, 0x00, 0x00, // ,
                                     0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, // -
                                     0x00, 0x00, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, // .
                                     0x00, 0x20, 0x10, 0x08, 0x04, 0x02, 0x00, 0x00, // /
                                     0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E, 0x00, 0x00, // 0
                                     0x00, 0x00, 0x04, 0x02, 0x7F, 0x00, 0x00, 0x00, // 1
                                     0x00, 0x42, 0x61, 0x51, 0x49, 0x46, 0x00, 0x00, // 2
                                     0x00, 0x22, 0x41, 0x49, 0x49, 0x36, 0x00, 0x00, // 3
                                     0x00, 0x18, 0x14, 0x12, 0x7F, 0x10, 0x00, 0x00, // 4
                                     0x00, 0x27, 0x45, 0x45, 0x45, 0x39, 0x00, 0x00, // 5
                                     0x00, 0x3E, 0x49, 0x49, 0x49, 0x32, 0x00, 0x00, // 6
                                     0x00, 0x01, 0x01, 0x71, 0x09, 0x07, 0x00, 0x00, // 7
                                     0x00, 0x36, 0x49, 0x49, 0x49, 0x36, 0x00, 0x00, // 8
                                     0x00, 0x26, 0x49, 0x49, 0x49, 0x3E, 0x00, 0x00, // 9
                                     0x00, 0x00, 0x36, 0x36, 0x00, 0x00, 0x00, 0x00, // :
                                     0x00, 0x00, 0x56, 0x36, 0x00, 0x00, 0x00, 0x00, // ;
                                     0x00, 0x08, 0x14, 0x22, 0x41, 0x00, 0x00, 0x00, // <
                                     0x00, 0x14, 0x14, 0x14, 0x14, 0x14, 0x00, 0x00, // =
                                     0x00, 0x00, 0x41, 0x22, 0x14, 0x08, 0x00, 0x00, // >
                                     0x00, 0x02, 0x01, 0x51, 0x09, 0x06, 0x00, 0x00, // ?
                                     0x00, 0x3E, 0x41, 0x59, 0x55, 0x5E, 0x00, 0x00, // @
                                     0x00, 0x7E, 0x09, 0x09, 0x09, 0x7E, 0x00, 0x00, // A
                                     0x00, 0x7F, 0x49, 0x49, 0x49, 0x36, 0x00, 0x00, // B
                                     0x00, 0x3E, 0x41, 0x41, 0x41, 0x22, 0x00, 0x00, // C
                                     0x00, 0x7F, 0x41, 0x41, 0x41, 0x3E, 0x00, 0x00, // D
                                     0x00, 0x7F, 0x49, 0x49, 0x49, 0x41, 0x00, 0x00, // E
                                     0x00, 0x7F, 0x09, 0x09, 0x09, 0x01, 0x00, 0x00, // F
                                     0x00, 0x3E, 0x41, 0x41, 0x49, 0x3A, 0x00, 0x00, // G
                                     0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F, 0x00, 0x00, // H
                                     0x00, 0x00, 0x41, 0x7F, 0x41, 0x00, 0x00, 0x00, // I
                                     0x00, 0x30, 0x40, 0x40, 0x40, 0x3F, 0x00, 0x00, // J
                                     0x00, 0x7F, 0x08, 0x14, 0x22, 0x41, 0x00, 0x00, // K
                                     0x00, 0x7F, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00, // L
                                     0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F, 0x00, 0x00, // M
                                     0x00, 0x7F, 0x02, 0x04, 0x08, 0x7F, 0x00, 0x00, // N
                                     0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E, 0x00, 0x00, // O
                                     0x00, 0x7F, 0x09, 0x09, 0x09, 0x06, 0x00, 0x00, // P
                                     0x00, 0x1E, 0x21, 0x21, 0x21, 0x5E, 0x00, 0x00, // Q
                                     0x00, 0x7F, 0x09, 0x09, 0x09, 0x76, 0x00, 0x00, // R
                                     0x00, 0x26, 0x49, 0x49, 0x49, 0x32, 0x00, 0x00, // S
                                     0x00, 0x01, 0x01, 0x7F, 0x01, 0x01, 0x00, 0x00, // T
                                     0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F, 0x00, 0x00, // U
                                     0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F, 0x00, 0x00, // V
                                     0x00, 0x7F, 0x20, 0x10, 0x20, 0x7F, 0x00, 0x00, // W
                                     0x00, 0x41, 0x22, 0x1C, 0x22, 0x41, 0x00, 0x00, // X
                                     0x00, 0x07, 0x08, 0x70, 0x08, 0x07, 0x00, 0x00, // Y
                                     0x00, 0x61, 0x51, 0x49, 0x45, 0x43, 0x00, 0x00, // Z
                                     0x00, 0x00, 0x7F, 0x41, 0x00, 0x00, 0x00, 0x00, // [
                                     0x00, 0x02, 0x04, 0x08, 0x10, 0x20, 0x00, 0x00, // \
                                     0x00, 0x00, 0x00, 0x41, 0x7F, 0x00, 0x00, 0x00, // ]
                                     0x00, 0x04, 0x02, 0x01, 0x02, 0x04, 0x00, 0x00, // ^
                                     0x00, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00, // _
                                     0x00, 0x00, 0x01, 0x02, 0x04, 0x00, 0x00, 0x00, // `
                                     0x00, 0x20, 0x54, 0x54, 0x54, 0x78, 0x00, 0x00, // a
                                     0x00, 0x7F, 0x44, 0x44, 0x44, 0x38, 0x00, 0x00, // b
                                     0x00, 0x38, 0x44, 0x44, 0x44, 0x44, 0x00, 0x00, // c
                                     0x00, 0x38, 0x44, 0x44, 0x44, 0x7F, 0x00, 0x00, // d
                                     0x00, 0x38, 0x54, 0x54, 0x54, 0x18, 0x00, 0x00, // e
                                     0x00, 0x04, 0x04, 0x7E, 0x05, 0x05, 0x00, 0x00, // f
                                     0x00, 0x08, 0x54, 0x54, 0x54, 0x3C, 0x00, 0x00, // g
                                     0x00, 0x7F, 0x08, 0x04, 0x04, 0x78, 0x00, 0x00, // h
                                     0x00, 0x00, 0x44, 0x7D, 0x40, 0x00, 0x00, 0x00, // i
                                     0x00, 0x20, 0x40, 0x44, 0x3D, 0x00, 0x00, 0x00, // j
                                     0x00, 0x7F, 0x10, 0x28, 0x44, 0x00, 0x00, 0x00, // k
                                     0x00, 0x00, 0x41, 0x7F, 0x40, 0x00, 0x00, 0x00, // l
                                     0x00, 0x7C, 0x04, 0x78, 0x04, 0x78, 0x00, 0x00, // m
                                     0x00, 0x7C, 0x08, 0x04, 0x04, 0x78, 0x00, 0x00, // n
                                     0x00, 0x38, 0x44, 0x44, 0x44, 0x38, 0x00, 0x00, // o
                                     0x00, 0x7C, 0x14, 0x14, 0x14, 0x08, 0x00, 0x00, // p
                                     0x00, 0x08, 0x14, 0x14, 0x14, 0x7C, 0x00, 0x00, // q
                                     0x00, 0x00, 0x7C, 0x08, 0x04, 0x04, 0x00, 0x00, // r
                                     0x00, 0x48, 0x54, 0x54, 0x54, 0x20, 0x00, 0x00, // s
                                     0x00, 0x04, 0x04, 0x3F, 0x44, 0x44, 0x00, 0x00, // t
                                     0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C, 0x00, 0x00, // u
                                     0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C, 0x00, 0x00, // v
                                     0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C, 0x00, 0x00, // w
                                     0x00, 0x44, 0x28, 0x10, 0x28, 0x44, 0x00, 0x00, // x
                                     0x00, 0x0C, 0x50, 0x50, 0x50, 0x3C, 0x00, 0x00, // y
                                     0x00, 0x44, 0x64, 0x54, 0x4C, 0x44, 0x00, 0x00, // z
                                     0x00, 0x00, 0x08, 0x36, 0x41, 0x41, 0x00, 0x00, // {
                                     0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, // |
                                     0x00, 0x41, 0x41, 0x36, 0x08, 0x00, 0x00, 0x00, // }
                                     0x00, 0x02, 0x01, 0x02, 0x04, 0x02, 0x00, 0x00};// ~

#endif
