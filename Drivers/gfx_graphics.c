///////////////////////////////////////////////////////////////////////////////
//                               gfx_graphics.c                              //
//                                                                           //
// The following graphics library is a combination Graphics library and      //
// Touch library.  The premise of this library is to store 16-bit RGB        //
// bitmaps on an external memory device, flash, eeprom, etc., and then to    //
// display one of the bitmaps onto the display by reading it off of the      //
// external memory device and sending to the display.                        //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
//                                    API                                    //
///////////////////////////////////////////////////////////////////////////////
// Display Functions:                                                        //
//                                                                           //
//    void gfx_InitGraphics(TOUCH_CAL TouchCalibration)                      //
//       - Initializes the library and set the touch calibration value.      //
//         Should be called before any other library functions are used, and //
//         after any hardware specific init functions are called.            //
//                                                                           //
//    uint16_t gfx_DisplayImage(uint16_t Index, uint16_t x, uint16_t y)      //
//       - Displays specified image starting at coordinate (x, y) and        //
//         returns the Handle for the image from 0 to 65534, if 65535 was    //
//         returned an error occurred.                                       //
//                                                                           //
//    void gfx_RemoveImage(uint16_t HandleId, int1 RedrawScreen = FALSE)     //
//       - Remove image with specified HandleId from memory, RedrawScreen    //
//         specifies whether to redraw the screen.                           //
//                                                                           //
//    void gfx_RemoveAllImages(void)                                         //
//       - Removes all images from memory doesn't change what is currently   //
//         being displayed on screen.                                        //
//                                                                           //
//    void gfx_ClearScreen(uint16_t Color, int1 RemoveHandles = TRUE)        //
//       - Set the entire screen to the specified 16-bit RGB color,          //
//         RemoveHandles specifies whether to remove all image HandleIds     //
//         from memory.                                                      //
//                                                                           //
//    void gfx_RedrawScreen(void)                                            //
//       - Redraws all images with HandleIds, images will be redrawn in same //
//         same order they were originally drawn in.                         //
//                                                                           //
//    void gfx_FillArea(uint16_t ImageHandle, uint8_t AreaIndex,             //
//                      FILL_DIR Direction, uint32_t Pixels, uint16_t Color) //
//       - Fills specified area on specified image with specified color.     //
//         Direction and Pixels are used to specify how and how much of area //
//         is filled.                                                        //
//                                                                           //
//    void gfx_FillAreaAbsolute(uint16_t x1, uint16_t y1, uint16_t x2,       //
//                              uint16_t y2, uint16_t Color)                 //
//       - Fills the specified area with the specified color.  (x1,y1) marks //
//         the upper left hand corner of area to fill, (x2,y2) marks the     //
//         lower right hand corner of area to fill.                          //
//                                                                           //
//    void gfx_DrawLineAbsolute(uint16_t x1, uint16_t y1, uint16_t x2,       //
//                              uint16_t y2, uint16_t Color)                 //
//       - Draw a line with the specified color from coordinate (x1,y1) to   //
//         coordinate (x2,y2).                                               //
//                                                                           //
//    void gfx_DrawCircleAbsolute(uint16_t x, uint16_t y, uint16_t Radius,   //
//                                uint16_t Color, int1 Fill=FALSE);          //
//       - Draw a line with the specified color centered at coordinates      //
//         (x,y) with specified radius.  If Fill is TRUE the circle will be  //
//          filled in.                                                       //
//                                                                           //
//    void gfx_DisplayString(uint16_t ImageHandle, uint8_t AreaIndex,        //
//                           uint8_t FontIndex, uint16_t Fcolor,             //
//                           uint16_t Bcolor, char *Text)                    //
//       - Displays the Text with the specified font in the specified area   //
//         on the specified image with the specified text color, Fcolor, and //
//         background color, Bcolor.  Text will only be displayed if area is //
//         designated as AREA_TYPE_TEXT.                                     //
//                                                                           //
//    void gfx_DisplayStringAbsolute(uint16_t x, uint16_t y,                 //
//                                   uint8_t FontIndex, uint16_t Fcolor,     //
//                                   uint16_t Bcolor, char *Text)            //
//       - Display the Text with the specified font with the specified text  //
//         color, Fcolor, and background color, Bcolor, start at coordinates //
//         (x,y).                                                            //
//                                                                           //
// Defines:                                                                  //
//                                                                           //
//    TAB_CHARACTERS                                                         //
//       - Specifies the number spaces a tab character, '\t', is equivalent  //
//         to, default is 3.                                                 //
//                                                                           //
//    DISPLAY_HORIZONTAL                                                     //
//       - Used to set the display's orientation to 0 degrees, not           //
//         changeable.                                                       //
//                                                                           //
//    DISPLAY_VERTICAL                                                       //
//       - Used to set the display's orientation to 90 degrees, not          //
//         changeable.                                                       //
//                                                                           //
// Types:                                                                    //
//                                                                           //
//    AREA_STRUCT: - structure                                               //
//       uint8_t AreaIndex;  - Index of the area.                            //
//       AREA_INFO AreaInfo; - Information about the area.                   //
//                                                                           //
//    HANDLE_STRUCT: - structure                                             //
//       uint16_t HandleId;   - The HandleId assigned to the image.          //
//       uint16_t ImageIndex; - The Index of the image for this HandleId.    //
//       uint16_t x;          - (x, y) the upper left hand coordinate that   //
//       uint16_t y;            the image is being displayed at.             //
//       uint8_t AreaCount;   - The number of Areas the image has.           //
//       AREA_STRUCT **Area;  - Pointer to a pointer of AREA_STRUCT.         //
//                                                                           //
//    FILL_DIR:                                                              //
//       FILL_TOP_TO_BOTTOM  - Fill area from top to bottom.                 //
//       FILL_BOTTOM_TO_TOP  - Fill area from bottom to top.                 //
//       FILL_LEFT_TO_RIGHT  - Fill area from left to right.                 //
//       FILL_RIGHT_TO_LEFT  - Fill area from right to left.                 //
//       FILL_ALL            - Fill entire area.                             //
//                                                                           //
// Variables:                                                                //
//                                                                           //
//    int1 gfx_DisplayOrientation;                                           //
//       - Specifies the rotation of the display.  When set to               //
//         DISPLAY_HORIZONTAL display orientation is 0 degrees, and when set //
//         to DISPLAY_VERTICAL display orientation is 90 degrees.  Default   //
//         orientation is DISPLAY_HORIZONTAL.                                //
//                                                                           //
//    uint16_t gfx_HandleId;                                                 //
//       - The next handle id that will be assigned to an image when the     //
//         gfx_DisplayImage() function is called.                            //
//                                                                           //
//    uint16_t gfx_HandleCount;                                              //
//       - The number of images that have HandleId's assigned to them, i.e.  //
//         the number of images being displayed.  This value is              //
//         automatically incremented, decremented and reset when the         //
//         gfx_DisplayImage(), gfx_RemoveImage() and gfx_RemoveAllImages()   //
//         functions are called.                                             //
//                                                                           //
//    HANDLE_STRUCT *gfx_ImageHandle;                                        //
//       - Pointer to an array of HANDLE_STRUCTs that contain info about the //
//         images that have HandleId's assigned to them, i.e. the images     //
//         currently being display.  The memory assigned to the array is     //
//         dynamically assigned as images are added and removed.             //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
// External Memory Function:                                                 //
//                                                                           //
//    void gfx_LoadImage(uint16_t Count, uint8_t *Data, int1 New)            //
//       - Used to load an image onto an external memory device. Function    //
//         is designed to be called multiple times.  Count is the number of  //
//         bytes in current packet, Data is pointer to the data for current  //
//         packet and New specifies whether packet is start of new image or  //
//         continuation of previous image.  For each image the first time    //
//         gfx_LoadImage is called New should be TRUE, then it should be     //
//         FALSE until entire Image has been sent.                           //
//                                                                           //
//    void gfx_EraseImage(uint16_t Index)                                    //
//       - Erases an image with the specified Index from the external memory //
//         device.                                                           //
//                                                                           //
//    void gfx_LoadFont(uint16_t Count, uint8_t *Data, int1 New)             //
//       - Used to load a font onto an external memory device.  Function is  //
//         designed to be called multiple times.  Count is the number of     //
//         bytes in current packet, Data is pointer to the data for current  //
//         packet and New specifies whether packet is start of new font or   //
//         continuation of previous font.  For each font the first time      //
//         gfx_LoadFont is called New should be TRUE, then it should be      //
//         until entire Font has been sent.                                  //
//                                                                           //
//    void gfx_EraseFont(uint16_t Index)                                     //
//       - Erase a Font with the specified Index from the external memory    //
//         device.                                                           //
//                                                                           //
// Defines:                                                                  //
//                                                                           //
//    MAX_IMAGES                                                             //
//       - Sets the maximum number of Images that can be saved on the        //
//         external memory device, default is 512.                           //
//                                                                           //
//    MAX_IMAGE_AREAS                                                        //
//       - Sets the maximum number of areas that each image can have,        //
//         default is 32.                                                    //
//                                                                           //
//    MAX_FONTS                                                              //
//       - Sets the maximum number of Fonts that can be saved on the         //
//         external memory device, default is maximum possible.              //
//                                                                           //
//    MAX_FONT_WIDTH                                                         //
//       - Sets the maximum width of a font that can be saved to external    //
//         memory device, default is 128.                                    //
//                                                                           //
//    MAX_FONT_HEIGHT                                                        //
//       - Sets the maximum height of a font that can be saved to external   //
//         memory device, default is 128.                                    //
//                                                                           //
//    MAX_FONT_CHARACTERS                                                    //
//       - Sets the maximum number of character a font can have hat is saved //
//         to the external memory device, default is 95 (space to ~).        //
//                                                                           //
// Types:                                                                    //
//                                                                           //
//    AREA_TYPE:                                                             //
//       AREA_TYPE_BUTTON - Area is a Button.                                //
//       AREA_TYPE_TEXT   - Area is a TextBox.                               //
//       AREA_TYPE_OTHER  - Area is not a Button or a TextBox.               //
//                                                                           //
//    AREA_INFO: - structure                                                 //
//       uint16_t x1;    - (x1,y1) top left hand corner of area.             //
//       uint16_t y1;                                                        //
//       uint16_t x2;    - (x2,y2) bottom right hand corner of area.         //
//       uint16_t y2;                                                        //
//       AREA_TYPE Type; - Area Type.                                        //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
// Touch Functions:                                                          //
//                                                                           //
//    void gfx_TouchTask(void)                                               //
//       - Task function that processes touches and calls user touch         //
//         function that was set with gfx_SetTouchCallback() function.       //
//         Function should be called frequently, at least once per iteration //
//         of user's main loop.                                              //
//                                                                           //
//    void gfx_SetTouchCallback(_touchpfunc func, int1 OnlyValidAreas,       //
//                              int1 OnlyUpDown)                             //
//       - Used to set user's touch function that is called by               //
//         gfx_TouchTask() when screen is touched.  OnlyValidAreas specifies //
//         whether any touch on screen causes function to be called or only  //
//         areas that are designated as an AREA_TYPE_BUTTON causes function  //
//         to be called.  OnlyUpDown specifies whether only pressing and     //
//         releasing screen causes function to be called, or pressing,       //
//         holding and releasing causes function to be called.               //
//                                                                           //
// Defines:                                                                  //
//                                                                           //
//    TOUCH_UPDATE_RATE                                                      //
//       - Specifies the rate in milliseconds at which the user's touch      //
//         function is called by gfx_TouchTask() when OnlyUpDown parameter   //
//         is set to FALSE with gfx_SetTouchCallback() function.  Default is //
//         100 ms.                                                           //
//                                                                           //
// Types:                                                                    //
//                                                                           //
//    TOUCH_TYPE:                                                            //
//       TOUCH_TYPE_PRESSED  - Display was pressed.                          //
//       TOUCH_TYPE_RELEASED - Display was released.                         //
//       TOUCH_TYPE_PRESSING - Display is being pressed.                     //
//                                                                           //
//    TOUCH_CAL_DATA: - structure                                            //
//       int16_t Slope;                                                      //
//       int16_t Offset;                                                     //
//       int1 Reversed;                                                      //
//                                                                           //
//       The above structure is used for calibrating the touch screen.  The  //
//       Slope and Offset are used in an equation to calculate the Error for //
//       one of the coordinates, either x or y.  The equation is as follows: //
//          Error = ((Slope * Coordinate) / 1000) + Offset;                  //
//                                                                           //
//       The Slope is multiplied by 1000 for rounding purposes.  The Error   //
//       is then subtracted from the coordinate to get the calibrated        //
//       coordinate.  Reversed is used to specify if the axis is flipped.    //
//                                                                           //
//    TOUCH_CAL: - structure                                                 //
//       TOUCH_CAL_DATA X; - Calibration data for the X axis.                //
//       TOUCH_CAL_DATA Y; - Calibration data for the Y axis.                //
//       int1 Rotated;     - Specifies is the X and Y axes are switch.       //
//                                                                           //
// Variables:                                                                //
//                                                                           //
//    TOUCH_CAL gfx_TouchCalibration;                                        //
//       - Contains calibration data for touch screen.  This variable is     //
//         initialized by the gfx_InitGraphics() function, user may want to  //
//         modify it in a calibration function.                              //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
// Backlight Functions:                                                      //
//                                                                           //
//    void gfx_InitBacklight(void)                                           //
//       - If GFX_BACKLIGHT_PIN is defined, function is used to initialize   //
//         the backlight.  Should be called before any other backlight       //
//         functions.                                                        //
//                                                                           //
//    void gfx_SetBacklightLevel(uint8_t duty)                               //
//       - If GFX_BACKLIGHT_PIN is defined, sets the backlight to the        //
//         specified duty, 0 to 100 percent.                                 //
//                                                                           //
//    void gfx_BacklightTask(void)                                           //
//       - If GFX_BACKLIGHT_PIN is defined, task function that adjust        //
//         backlight to level specified with gfx_SetBackLightLevel.          //
//         Function should be called frequently, at least once per iteration //
//         of user's main loop.                                              //
//                                                                           //
// Backlight Macros:                                                         //
//                                                                           //
//    gfx_BacklightOn()                                                      //
//       - Turns the backlight on.                                           //
//                                                                           //
//    gfx_BacklightOff()                                                     //
//       - Turns the backlight off.                                          //
//                                                                           //
// Backlight Defines:                                                        //
//                                                                           //
//    GFX_BACKLIGHT_PIN                                                      //
//       - Specifies the pin to use for the backlight PWM, uses #use pwm()   //
//         to generate the PWM signal turn on, turn off and dim backlight.   //
//         Pin must be one of the hardware Output Compare/CCP pins.  The     //
//         above backlight functions and macros are only available if        //
//         GFX_BACKLIGHT_PIN is defined.                                     //
//                                                                           //
//    GFX_BACKLIGHT_TIMER                                                    //
//       - Sets the timer the backlight uses, default is Timer 1.            //
//                                                                           //
//    GFX_BACKLIGHT_FREQUENCY                                                //
//       - Sets the frequency of the PWM used for the backlight, default is  //
//         25kHz.                                                            //
//                                                                           //
//    GFX_BACKLIGHT_START_DUTY                                               //
//       - Sets the duty cycle of the PWM used for the backlight, default is //
//         100%.                                                             //
//                                                                           //
//    GFX_BACKLIGHT_START_ON                                                 //
//       - Sets whether the backlight starts as being on or off, TRUE is on  //
//         and FALSE is off, default is off.                                 //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
// Serial Functions:                                                         //
//                                                                           //
//    void gfx_SerialTask(void)                                              //
//       - If GFX_USE_SERIAL_INTERFACE is defined, task function for         //
//         processing serial commands.  Function should be called            //
//         frequently, at least once per iteration of user's main loop.      //
//                                                                           //
//    Define the following Macros to select the serial interface to use.     //
//    Library will default to the last #use rs232() seen before this file    //
//    not defined.                                                           //
//       - gfx_putc                                                          //
//       - gfx_getc                                                          //
//       - gfx_kbhit                                                         //
//                                                                           //
// Serial Defines:                                                           //
//                                                                           //
//    GFX_USE_SERIAL_INTERFACE                                               //
//       - If defined will include the library's serial code including the   //
//         gfx_SerialTask() function.                                        //
//                                                                           //
//    MAX_SERIAL_DATA_SIZE                                                   //
//       - Sets the maximum data packet size that can be received by         //
//         gfx_SerialTask() function.  Default is 1024 bytes, if using CCS's //
//         PC software it should be left at the default size.                //
//                                                                           //
//    SERIAL_BUFFERS                                                         //
//       - Sets the number of serial buffers code uses, default is 3.        //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
// The following is a list of external functions, defines and types that are //
// required for the library to build and function.                           //
///////////////////////////////////////////////////////////////////////////////
// Functions:                                                                //
//                                                                           //
//    glcd_DrawPixel(uint16_t x, uint16_t y, uint16_t color)                 //
//                                                                           //
//    glcd_DrawPixels(uint16_t StartX, uint16_t StartY, uint16_t Width,      //
//                    uint16_t Height, uint16_t *Data)                       //
//                                                                           //
//    int1 touch_Ready(void)                                                 //
//       Returns: TRUE - touch coordinates ready to be read.                 //
//                FALSE - touch coordinates not read to be read.             //
//                                                                           //
//    touch_ReadChannels(uint16_t *ptr, uint8_t count)                       //
//                                                                           //
//    flash_ReadData(FLASH_ADDR Addr, uint16_t *Data, uint32_t count)        //
//                                                                           //
//    flash_WriteData(FLASH_ADDR Addr, uint16_t *Data, uint32_t count)       //
//                                                                           //
//    int1 flash_IncAddress(FLASH_ADDR *Addr, uint32_t value)                //
//       Returns: TRUE - operation was successful.                           //
//                FALSE - operation was unsuccessful.                        //
//                                                                           //
//    int1 flash_IncAddress(FLASH_ADDR *Addr)                                //
//       Returns: TRUE - operation was successful.                           //
//                FALSE - operation was unsuccessful.                        //
//                                                                           //
//    uint8_t flash_SetAddress(uint32_t wAddress, FLASH_ADDR *Address)       //
//       Returns: 0x00 - operation was successful.                           //
//                0x01 to 0xFF - an error occurred.                          //
//                                                                           //
//    flash_EraseBlocks(FLASH_ADDR Address, uint16_t Count)                  //
//                                                                           //
//    flash_BulkErase(void)                                                  //
//                                                                           //
// Defines:                                                                  //
//                                                                           //
//    GLCD_PIXELS                                                            //
//       - The Number of Pixels the LCD has, i.e. width of display.          //
//                                                                           //
//    GLCD_LINES                                                             //
//       - The Number of lines the LCD has, i.e. height of display.          //
//                                                                           //
//    FLASH_ERASE_SIZE                                                       //
//       - The size of the erase block on the external memory device.        //
//                                                                           //
//    FLASH_BLOCKS                                                           //
//       - The number of erase blocks on the external memory device.         //
//                                                                           //
// The following is a list of required functions if the define USE_FAST_DRAW //
// is defined in your project:                                               //
//                                                                           //
//    DrawBlock(uint16_t StartX, uint16_t StartY, uint16_t Width,            //
//              uint16_t Height, uint16_t Color)                             //
//                                                                           //
//    void WriteFastImage(IMAGE *ImageData, uint16_t StartX,                 //
//                        uint16_t StartY)                                   //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
//                        Image and Font Data Formats                        //
///////////////////////////////////////////////////////////////////////////////
// Since the library stores images and fonts on an external memory device it //
// requires that the data be stored on the device in a specific way.  The    //
// library functions gfx_LoadImage() and gfx_LoadFont() takes care of saving //
// them to the memory.  The user only need to make sure that the data is     //
// is passed to the function correctly.  All 16 bit values are sent with     //
// little-endian order, LSB sent first followed by MSB.                      //
//                                                                           //
// For the images the data needs to be passed to the gfx_LoadImage()         //
// function as follows:                                                      //
//                                                                           //
//    uint16_t Index;                                                        //
//        - the index of the image.  Can be any value from 0 to MAX_IMAGES.  //
//                                                                           //
//    uint16_t Width;                                                        //
//        - the width of the image in pixels.                                //
//                                                                           //
//    uint16_t Height;                                                       //
//        - the height of the image in lines.                                //
//                                                                           //
//    uint8_t AreaCount;                                                     //
//        - the number of areas in the image.                                //
//                                                                           //
//    AREA_INFO Area[AreaCount];                                             //
//        - the area info for each area of the image.  If AreaCount is 0     //
//          this section is skipped.  The area info for each area is sent as //
//          follows:                                                         //
//                                                                           //
//             uint16_t x1;                                                  //
//                - the top left corner x coordinate of area.                //
//                                                                           //
//             uint16_t y1;                                                  //
//                - the top left corner y coordinate of area.                //
//                                                                           //
//             uint16_t x2;                                                  //
//                - the bottom right corner x coordinate of area.            //
//                                                                           //
//             uint16_t y2;                                                  //
//                - the bottom right corner y coordinate of area.            //
//                                                                           //
//             uint8_t Type                                                  //
//                - the type of area.                                        //
//                                                                           //
//    uint16_t PixelData[Width * Height];                                    //
//       - the pixel data is sent in sequence from left to right, top to     //
//         bottom.  The pixels need to be converted to their RGB 565 color.  //
//                                                                           //
// For the fonts the data needs to be passed to the gfx_LoadFont() function  //
// as follows:                                                               //
//                                                                           //
//    uint16_t Index;                                                        //
//       - the index of the font. Can be any value from 1 to MAX_FONTS, font //
//         0 is an 8x8 font that comes with the library.                     //
//                                                                           //
//    uint8_t Width;                                                         //
//       - the width of the font in pixels.                                  //
//                                                                           //
//    uint8_t Height;                                                        //
//       - the height of the font in pixels.                                 //
//                                                                           //
//    uint8_t Count;                                                         //
//       - the number of characters in the font.                             //
//                                                                           //
//    char CharacterStart;                                                   //
//       - the first character in the font.                                  //
//                                                                           //
//    uint8_t CharacterData[ByteCount];                                      //
//       - the character data is organized from top to bottom, left to       //
//         right.  Each byte contains 8 pixels of the character.  If the     //
//         height is not a multiple of 8 then the most significant bits of   //
//         the last byte for each column should be padded with 0.  Also the  //
//         number of bytes for each character must be a multiple of 2.  If   //
//         not each character should be padded with an extra 0x00 byte to    //
//         make it a multiple of 2.                                          //
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

#include <gfx_graphics.h>

#include <stdlibm.h>

////////////////////////////// Display Functions //////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// gfx_InitGraphics()
//
// Initializes the graphics library and set the touch calibration value.
//
// Parameter:
//    TouchCalibration - a TOUCH_CAL structure containing the touch 
//                       calibration data.
//
///////////////////////////////////////////////////////////////////////////////
void gfx_InitGraphics(TOUCH_CAL TouchCalibration)
{
  #ifdef GFX_ON_OFF_PIN
   output_high(GFX_ON_OFF_PIN);
   output_drive(GFX_ON_OFF_PIN);
  #endif
  
  #ifdef GFX_BACKLIGHT_PIN
   gfx_InitBacklight();
  #endif
  
  memcpy(&gfx_TouchCalibration, &TouchCalibration, sizeof(TOUCH_CAL));
  
  gfx_SetTouchCallback(TouchFunction, TRUE, TRUE);
}

///////////////////////////////////////////////////////////////////////////////
// gfx_DisplayImage()
//
// Displays one of the images saved on the external memory starting at the 
// specified coordinate.
//
// Parameters:
//    Index - the index of the image to display.
//
//    x - the x coordinate to start displaying the image at.
//
//    y - the y coordinate to start displaying the image at.
//
// Returns:
//    uint16_t value representing the Handle Id assigned to the image.  If it 
//    returns 0xFFFF and error occurred and image wasn't displayed.
//
///////////////////////////////////////////////////////////////////////////////
uint16_t gfx_DisplayImage(uint16_t Index, uint16_t x, uint16_t y)
{
   if(WriteImage(Index, x, y))
      return(AdHandle(Index, x, y));
   else
      return(-1);
}

///////////////////////////////////////////////////////////////////////////////
// gfx_RemoveImage()
//
// Removes the image with the specified Handle Id gfx_ImageHandle array.  
// Removing it from the gfx_ImageHandle array will keep the gfx_FillArea() and 
// gfx_DisplayString() from drawing on the screen when that Handle Id is used
// with them, and if a touch is detected from reporting that it happened in 
// one of the Handle Id's areas.  Doesn't physically remove the image from the
// unless the RedrawScreen parameter is set to TRUE.
//
// Parameters:
//    HandleId - the Handle Id of the image to remove.
//
//    RedrawScreen - if TRUE the display will be redrawn, if FALSE screen will 
//                   not be redrawn.
//
///////////////////////////////////////////////////////////////////////////////
void gfx_RemoveImage(uint16_t HandleId, int1 RedrawScreen = FALSE)
{
   uint16_t i,j;
   uint8_t k;
   HANDLE_STRUCT *Handles;
   
   if(HasHandle(HandleId))
   {
      if(--gfx_HandleCount == 0)
      {
         if(gfx_ImageHandle[0].AreaCount != 0)
         {
            for(i=0;i<gfx_ImageHandle[0].AreaCount;i++)
               free(gfx_ImageHandle[0].Area[i]);
            
            free(gfx_ImageHandle[0].Area);
         }
         
         free(gfx_ImageHandle);
         gfx_ImageHandle = NULL;
      }
      else
      {
         Handles = malloc(sizeof(HANDLE_STRUCT) * gfx_HandleCount);
         
         if(Handles != NULL)
         {
            for(i=0,j=0;i<=gfx_HandleCount;i++)
            {
               if(gfx_ImageHandle[i].HandleId == HandleId)
               {
                  if(gfx_ImageHandle[i].AreaCount != 0)
                  {
                     for(k=0;k<gfx_ImageHandle[i].AreaCount;k++)
                        free(gfx_ImageHandle[i].Area[k]);
                     
                     free(gfx_ImageHandle[i].Area);
                  }
               }
               else
               {
                  memcpy(&Handles[j], &gfx_ImageHandle[i], sizeof(HANDLE_STRUCT));
                  j++;
               }
            }
            
            free(gfx_ImageHandle);
            gfx_ImageHandle = Handles;
            
            if(RedrawScreen)
               gfx_RedrawScreen();
         }
         else
            gfx_HandleCount++;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
// gfx_RemoveAllImages()
//
// Removes all Handle Ids from the gfx_ImageHandle array, the screen is not
// redrawn.
//
///////////////////////////////////////////////////////////////////////////////
void gfx_RemoveAllImages(void)
{
   uint16_t i,j;
   
   for(i=0;i<gfx_HandleCount;i++)
   {
      if(gfx_ImageHandle[i].AreaCount != 0)
      {
         for(j=0;j<gfx_ImageHandle[i].AreaCount;j++)
            free(gfx_ImageHandle[i].Area[j]);
         
         free(gfx_ImageHandle[i].Area);
      }
   }
   
   free(gfx_ImageHandle);
   gfx_ImageHandle = NULL;
   gfx_HandleCount = 0;
}

///////////////////////////////////////////////////////////////////////////////
// gfx_ClearScreen()
//
// Sets the entire display to the specified color, if RemoveHandles is set to 
// TRUE is will also remove all the Handle Ids from the gfx_ImageHandle array.
//
// Parameters:
//    Color - the RGB 565 color to set the display to.
//
//    RemoveHandles - if TRUE will remove all the Handle Ids from the 
//                    gfx_ImageHandle array, if FALSE the Handle Ids are not 
//                    removed.
//
///////////////////////////////////////////////////////////////////////////////
void gfx_ClearScreen(uint16_t Color, int1 RemoveHandles = TRUE)
{
   if(RemoveHandles)
      gfx_RemoveAllImages();
   
   if(gfx_DisplayOrientation == DISPLAY_HORIZONTAL)
      DrawRectangle(0, 0, (GLCD_PIXELS - 1), (GLCD_LINES - 1), Color, TRUE);
   else
      DrawRectangle(0, 0, (GLCD_LINES - 1), (GLCD_PIXELS - 1), Color, TRUE);
}

///////////////////////////////////////////////////////////////////////////////
// gfx_RedrawScreen()
//
// Redraws the display with the images gfx_ImageHandle array.  If not images
// are in the gfx_ImageHandle array, i.e. gfx_HandleCount = 0, then nothing is 
// drawn.
//
///////////////////////////////////////////////////////////////////////////////
void gfx_RedrawScreen(void)
{
   uint16_t i;
   
   for(i=0;i<gfx_HandleCount;i++)
      WriteImage(gfx_ImageHandle[i].ImageIndex, gfx_ImageHandle[i].x, gfx_ImageHandle[i].y);
}

///////////////////////////////////////////////////////////////////////////////
// gfx_FillArea()
//
// Fills the specified area on the image with the specified Handle Id with the 
// the specified color.
//
// Parameters:
//    ImageHandle - the Handle Id of the image with the area to fill.
//
//    AreaIndex - the index of the area to fill.
//
//    Direction - how to fill the area, can be one of the following:
//                   * FILL_TOP_TO_BOTTOM - fill area from top to bottom.
//                   * FILL_BOTTOM_TO_TOP - fill area from bottom to top.
//                   * FILL_LEFT_TO_RIGHT - fill area from left to right.
//                   * FILL_RIGHT_TO_LEFT - fill area from right to left.
//                   * FILL_ALL - fill entire area.
//
//    Pixels - the number of pixels to fill in, used for all directions except
//             FILL_ALL.
//
//    Color - the RGB 565 color to fill the area with.
//
///////////////////////////////////////////////////////////////////////////////
void gfx_FillArea(uint16_t ImageHandle, uint8_t AreaIndex, FILL_DIR Direction, uint32_t Pixels, uint16_t Color)
{
   HANDLE_STRUCT Handle;
   AREA_INFO Area;
   uint16_t Width;
   uint16_t Height;
   uint16_t StartX, StartY;
   uint16_t x, y;
   
   uint32_t i;
   
   if(GetHandle(ImageHandle, &Handle))
   {
      if(GetArea(AreaIndex, &Handle, &Area))
      {
         if(Direction == FILL_ALL)
            DrawRectangle(Handle.x + Area.x1, Handle.y + Area.y1, Handle.x + Area.x2, Handle.y + Area.y2, Color, TRUE);
         else
         {
            Width = Area.x2 - Area.x1 + 1;
            Height = Area.y2 - Area.y1 + 1;
            
            if(Pixels >= ((uint32_t)Width * (uint32_t)Height))
               DrawRectangle(Handle.x + Area.x1, Handle.y + Area.y1, Handle.x + Area.x2, Handle.y + Area.y2, Color, TRUE);
            else
            {
               StartX = Handle.x + Area.x1;
               StartY = Handle.y + Area.y1;
               
               switch(Direction)
               {
                  case FILL_TOP_TO_BOTTOM:
                     if((Pixels % Width) == 0)
                        DrawRectangle(StartX, StartY, StartX + Width - 1, StartY + (Pixels / Width) - 1, Color, TRUE);
                     else
                     {
                        x = StartX;
                        y = StartY;
                        
                        for(i=0;i<Pixels;i++)
                        {
                           glcd_DrawPixel(x, y, Color);
                           
                           if(++x >= (StartX + Width))
                           {
                              x = 0;
                              y++;
                           }
                        }
                     }
                     break;
                  case FILL_BOTTOM_TO_TOP:
                     if((Pixels % Width) == 0)
                        DrawRectangle(StartX, StartY + (Pixels / Width) - 1, StartX + Width - 1, StartY + Height - 1, Color, TRUE);
                     else
                     {
                        x = StartX + Width - 1;
                        y = StartY + Height - 1;
                        
                        for(i=0;i<Pixels;i++)
                        {
                           glcd_DrawPixel(x, y, Color);
                           
                           if(--x == StartX)
                           {
                              x = StartX + Width - 1;
                              y--;
                           }
                        }
                     }
                     break;
                  case FILL_LEFT_TO_RIGHT:
                     if((Pixels % Height) ==0)
                        DrawRectangle(StartX, StartY, StartX + (Pixels / Height) - 1, StartY + Height - 1, Color, TRUE);
                     else
                     {
                        x = StartX;
                        y = StartY;
                        
                        for(i=0;i<Pixels;i++)
                        {
                           glcd_DrawPixel(x, y, Color);
                           
                           if(++y >= (StartY + Height))
                           {
                              x++;
                              y = StartY;
                           }
                        }
                     }
                     break;
                  case FILL_RIGHT_TO_LEFT:
                     if((Pixels % Height) == 0)
                        DrawRectangle(StartX + (Pixels / Height), StartY, StartX + Width - 1, StartY + Height - 1, Color, TRUE);
                     else
                     {
                        x = StartX + Width - 1;
                        y = StartY;
                        
                        for(i=0;i<Pixels;i++)
                        {
                           glcd_DrawPixel(x, y, Color);
                           
                           if(++y >= (StartY + Height))
                           {
                              x--;
                              y = StartY;
                           }
                        }
                     }
                     break;
               }
            }
         }
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
// gfx_FillAreaAbsolute()
//
// Fill an area from (x1,y1) to (x2,y2) with the specified color.
//
// Parameters:
//    x1 - the top left hand x coordinate of the area to fill.
//
//    y1 - the top left hand y coordinate of the area to fill.
//
//    x2 - the bottom right hand x coordinate of the area to fill.
//
//    y2 - the bottom right hand y coordinate of the area to fill.
//
//    Color - the RGB 565 color to fill the area with.
//
///////////////////////////////////////////////////////////////////////////////
void gfx_FillAreaAbsolute(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t Color)
{
   DrawRectangle(x1, y1, x2, y2, Color, TRUE);
}

///////////////////////////////////////////////////////////////////////////////
// gfx_DrawLineAbsolute()
//
// Draws a line from coordinate (x1,y1) to coordinate (x2,y2) with the 
// specified color.
//
// Parameter:
//    x1 - the x coordinate to start drawing the line from.
//
//    y1 - the y coordinate to start drawing the line from.
//
//    x2 - the x coordinate to draw the line to.
//
//    y2 - the y coordinate to draw the line to.
//
//    Color - the RGB 565 color to draw the line with.
//
///////////////////////////////////////////////////////////////////////////////
void gfx_DrawLineAbsolute(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t Color)
{
   DrawLine(x1, y1, x2, y2, Color);
}

///////////////////////////////////////////////////////////////////////////////
// gfx_DrawCircleAbsolute()
//
// Draws a circle centered on coordinate (x,y) with the specified radius and 
// color.
//
// Parameter:
//    x - the x coordinate to center the circle on.
//
//    y - the y coordinate to center the circle on.
//
//    Radius - the radius of the circle.
//
//    Color - the RGB 565 color to draw the circle with.
//
//    Fill - if TRUE it will fill in the circle, if FALSE it will not fill in
//           the circle.
//
///////////////////////////////////////////////////////////////////////////////
void gfx_DrawCircleAbsolute(uint16_t x, uint16_t y, uint16_t Radius, uint16_t Color, int1 Fill=FALSE)
{
   int16_t a, b, P;
   
   a = 0;
   b = Radius;
   P = 1 - Radius;
   
   if((Fill == FALSE) && (gfx_DisplayOrientation == DISPLAY_VERTICAL))
   {
      uint16_t temp;
      
      temp = x;
      
      x = y;
      y = GLCD_LINES - temp;
   }
   
   do
   {
      if(Fill)
      {
         DrawLine(x-a, y+b, x+a, y+b, Color);
         DrawLine(x-a, y-b, x+a, y-b, Color);
         DrawLine(x-b, y+a, x+b, y+a, Color);
         DrawLine(x-b, y-a, x+b, y-a, Color);
      }
      else
      {
         glcd_DrawPixel(a+x, b+y, Color);
         glcd_DrawPixel(b+x, a+y, Color);
         glcd_DrawPixel(x-a, b+y, Color);
         glcd_DrawPixel(x-b, a+y, Color);
         glcd_DrawPixel(b+x, y-a, Color);
         glcd_DrawPixel(a+x, y-b, Color);
         glcd_DrawPixel(x-a, y-b, Color);
         glcd_DrawPixel(x-b, y-a, Color);
      }
      
      if(P < 0)
         P += 3 + 2*a++;
      else
         P += 5 + 2*(a++ - b--);
   } while(a <= b);
}

///////////////////////////////////////////////////////////////////////////////
// gfx_DisplayString()
//
// Displays a text string in the specified area on the image with the specified
// Handle Id.  Will only display the text is the area is of type
// AREA_TYPE_TEXT.
//
// Parameter:
//    ImageHandle - the Handle Id of the image with the area to display the
//                  text.
//
//    AreaIndex - the index of the area to display the text on.
//
//    FontIndex - the index of the font to display the text with.
//
//    Fcolor - the RGB 565 color to display the text with.
//
//    Bcolor - the RGB 565 color of the text background.
//
//    Text - pointer to null terminated string to display.
//
///////////////////////////////////////////////////////////////////////////////
void gfx_DisplayString(uint16_t ImageHandle, uint8_t AreaIndex, uint8_t FontIndex, uint16_t Fcolor, uint16_t Bcolor, char *Text)
{
   HANDLE_STRUCT Handle;
   AREA_INFO Area;
   uint16_t StartX, StartY;
   uint16_t EndX, EndY;
   uint8_t CharWidth, CharHeight;   
   uint16_t CurrentX, CurrentY;
   uint8_t *CharacterData;
   uint16_t *Pixels;
   int16_t i;
   uint16_t j; 
   uint8_t hBytes;
   FONT FontData;
   uint16_t PixelIndex;
   uint8_t *Ptr;
   uint8_t bit;
   FONT_ERR Error;
   
   if(GetHandle(ImageHandle, &Handle))
   {
      if(GetArea(AreaIndex, &Handle, &Area))
      {
         if(Area.Type == AREA_TYPE_TEXT) //Make sure area is a Text Box
         {
            StartX = Handle.x + Area.x1;
            StartY = Handle.y + Area.y1;
            EndX = Handle.x + Area.x2 + 1;
            EndY = Handle.y + Area.y2 + 1;
            
            CurrentX = StartX;
            CurrentY = StartY;
            
            if(gfx_DisplayOrientation == DISPLAY_HORIZONTAL)
            {
               if(EndX >= GLCD_PIXELS)
                  EndX = GLCD_PIXELS;
               
               if(EndY >= GLCD_LINES)
                  EndY = GLCD_LINES;
            }
            else
            {
               if(EndX >= GLCD_LINES)
                  EndX = GLCD_LINES;
               
               if(EndY >= GLCD_PIXELS)
                  EndY = GLCD_PIXELS;
            }
            
            if(FontIndex == 0)
            {            
               CharWidth = 8;
               CharHeight = 8;
               hBytes = 1;
               
               FontData.Header.Index = 0;
            }
            else
            {
               Error = GetFontInfo(FontIndex, &FontData);
               
               if(Error != FONT_ERR_OK)
                  return;
               
               CharWidth = FontData.Header.Width;
               CharHeight = FontData.Header.Height;
               
               hBytes = CharHeight / 8;
               
               if((CharHeight % 8) > 0)
                  hBytes++;
            }
            
            Pixels = malloc(((uint16_t)CharWidth * (uint16_t)CharHeight) * sizeof(uint16_t));
            CharacterData = malloc((uint16_t)hBytes * (uint16_t)CharWidth);
            
            if((Pixels == NULL) || (CharacterData == NULL))
            {
               free(Pixels);
               free(CharacterData);
               
               return;
            }
            
            if((CurrentX + CharWidth) <= EndX)   //Make sure TextBox is wide enough to display at least one character
            {
               while((*Text != '\0') && ((CurrentY + CharHeight) <= EndY))
               {
                  if(*Text == '\r')
                     CurrentX = StartX;
                  else if(*Text == '\n')
                     CurrentY += CharHeight;
                  else if(*Text == '\t')
                     CurrentX += (CharWidth * TAB_CHARACTERS);
                  else
                  {  
                     GetCharacter(&FontData, *Text, CharacterData);
                     
                     if(gfx_DisplayOrientation == DISPLAY_HORIZONTAL)
                     {
                        PixelIndex = 0;
                        
                        for(i=0;i<CharHeight;i++)
                        {
                           Ptr = &CharacterData[i / 8];
                           bit = i % 8;
                           
                           for(j=0;j<CharWidth;j++)
                           {
                              if(bit_test(*Ptr, bit))
                                 Pixels[PixelIndex] = Fcolor;
                              else
                                 Pixels[PixelIndex] = Bcolor;
                              
                              PixelIndex++;
                              Ptr += hBytes;
                           }
                        }
                     }
                     else
                     {
                        PixelIndex = 0;
                        
                        for(i=(CharWidth-1);i>=0;i--)
                        {
                           Ptr = &CharacterData[i*hBytes];
                           
                           for(j=0;j<CharHeight;j++)
                           {
                              if(bit_test(*Ptr,j % 8))
                                 Pixels[PixelIndex] = Fcolor;
                              else
                                 Pixels[PixelIndex] = Bcolor;
                              
                              PixelIndex++;
                              
                              if(((j + 1) % 8) == 0)
                                 Ptr++;
                           }
                        }     
                     }
                     
                     DrawBitmap(CurrentX, CurrentY, CharWidth, CharHeight, Pixels);
                     
                     CurrentX += CharWidth;
                  }
                  
                  Text++;
                  
                  if((CurrentX + CharWidth) > EndX)
                  {
                     CurrentY += CharHeight;
                     CurrentX = StartX;
                  }
               }
            }
            
            free(Pixels);
            free(CharacterData);
         }
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
// gfx_DisplayStringAbsolute()
//
// Display a text string starting at the coordinates (x,y).
//
// Parameters:
//    x - the upper left hand corner x coordinate to start displaying the text
//        at.
//
//    y - the upper left hand corner y coordinate to start displaying the text
//        at.
//
//    FontIndex - the index of the font to display the text with.
//
//    Fcolor - the RGB 565 color to display the text with.
//
//    Bcolor - the RGB 565 color of the text background.
//
//    Text - pointer to null terminated string to display. 
//
///////////////////////////////////////////////////////////////////////////////
void gfx_DisplayStringAbsolute(uint16_t x, uint16_t y, uint8_t FontIndex, uint16_t Fcolor, uint16_t Bcolor, char *Text)
{
   uint8_t *CharacterData;
   uint16_t *Pixels;
   int16_t i;
   uint16_t j;   
   uint16_t CurrentX, CurrentY;
   uint16_t CharWidth, CharHeight;
   uint16_t EndX, EndY;
   FONT FontData;
   uint8_t hBytes;
   uint16_t PixelIndex;
   uint8_t *Ptr;
   uint8_t bit;
   FONT_ERR Error;
   
   CurrentX = x;
   CurrentY = y;
   
   if(gfx_DisplayOrientation == DISPLAY_HORIZONTAL)
   {
      EndX = GLCD_PIXELS;
      EndY = GLCD_LINES;
   }
   else
   {
      EndX = GLCD_LINES;
      EndY = GLCD_PIXELS;
   }
   
   if(FontIndex == 0)
   {            
      CharWidth = 8;
      CharHeight = 8;
      hBytes = 1;
      
      FontData.Header.Index = 0;
   }
   else
   {
      Error = GetFontInfo(FontIndex, &FontData);
      
      if(Error != FONT_ERR_OK)
         return;
      
      CharWidth = FontData.Header.Width;
      CharHeight = FontData.Header.Height;
      
      hBytes = CharHeight / 8;
      
      if((CharHeight % 8) > 0)
         hBytes++;
   }
            
   Pixels = malloc(((uint16_t)CharWidth * (uint16_t)CharHeight) * sizeof(uint16_t));
   CharacterData = malloc((uint16_t)hBytes * (uint16_t)CharWidth);
   
   if((Pixels == NULL) || (CharacterData == NULL))
   {
      free(Pixels);
      free(CharacterData);
      
      return;
   }
   
   if((CurrentX + CharWidth) > EndX)
   {
      CurrentY += CharHeight;
      CurrentX = 0;
   }
   
   if((CurrentY + CharHeight) > EndY)
   {
      CurrentX = 0;
      CurrentY = 0;
   }
   
   while((*Text != '\0'))
   {
      if(*Text == '\r')
         CurrentX = 0;
      else if(*Text == '\n')
         CurrentY += CharHeight;
      else if(*Text == '\t')
         CurrentX += (CharWidth * TAB_CHARACTERS);
      else
      {  
         GetCharacter(&FontData, *Text, CharacterData);
                     
         if(gfx_DisplayOrientation == DISPLAY_HORIZONTAL)
         {
            PixelIndex = 0;
            
            for(i=0;i<CharHeight;i++)
            {
               Ptr = &CharacterData[i / 8];
               bit = i % 8;
               
               for(j=0;j<CharWidth;j++)
               {
                  if(bit_test(*Ptr, bit))
                     Pixels[PixelIndex] = Fcolor;
                  else
                     Pixels[PixelIndex] = Bcolor;
                  
                  PixelIndex++;
                  Ptr += hBytes;
               }
            }
         }
         else
         {
            PixelIndex = 0;
            
            for(i=(CharWidth-1);i>=0;i--)
            {
               Ptr = &CharacterData[i*hBytes];
               
               for(j=0;j<CharHeight;j++)
               {
                  if(bit_test(*Ptr,j % 8))
                     Pixels[PixelIndex] = Fcolor;
                  else
                     Pixels[PixelIndex] = Bcolor;
                  
                  PixelIndex++;
                  
                  if(((j + 1) % 8) == 0)
                     Ptr++;
               }
            }     
         }
         
         DrawBitmap(CurrentX, CurrentY, CharWidth, CharHeight, Pixels);
         
         CurrentX += CharWidth;
      }
      
      Text++;
      
      if((CurrentX + CharWidth) > EndX)
      {
         CurrentY += CharHeight;
         CurrentX = 0;
         
         if((CurrentY + CharHeight) > EndY)
            CurrentY = 0;
      }
   }
   
   free(Pixels);
   free(CharacterData);
}

///////////////////////////////////////////////////////////////////////////////
// DrawBitmap()
//
// Draws the block of pixel data to the display.
//
// Parameter:
//    StartX - the upper left hand corner x coordinate to start drawing the
//             pixels.
//
//    StartY - the upper left hand corner y coordinate to start drawing the 
//             pixels.
//
//    Width - the width of pixel data to draw.
//
//    Height - the height of pixel data to draw.
//
//    Data - pointer to the RGB 565 pixel data to draw.  The memory being
//           pointed to should be Width * Height words in size.
//
///////////////////////////////////////////////////////////////////////////////
void DrawBitmap(uint16_t StartX, uint16_t StartY, uint16_t Width, uint16_t Height, uint16_t *Data)
{
   if(gfx_DisplayOrientation ==  DISPLAY_VERTICAL)
   {
      uint16_t temp;

      temp = StartX;

      StartX = StartY;
      StartY = GLCD_LINES - temp - Width;
      
      temp = Width;
      
      Width = Height;
      Height = temp;
   }
   
   glcd_DrawPixels(StartX, StartY, Width, Height, Data);
}

///////////////////////////////////////////////////////////////////////////////
// DrawLine()
//
// Draws a line from coordinate (x1,y1) to coordinate (x2,y2) with the 
// specified color.
//
// Parameter:
//    x1 - the x coordinate to start drawing the line from.
//
//    y1 - the y coordinate to start drawing the line from.
//
//    x2 - the x coordinate to draw the line to.
//
//    y2 - the y coordinate to draw the line to.
//
//    Color - the RGB 565 color to draw the line with.
//
///////////////////////////////////////////////////////////////////////////////
void DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
   int16_t x, y, addx, addy, dx, dy;
   int16_t P;
   uint16_t i;
   
  #ifdef USE_FAST_DRAW
   if((gfx_DisplayOrientation == DISPLAY_HORIZONTAL) && (y1 == y2))
   {
      if(x1 < x2)
         DrawBlock(x1, y1, (x2 - x1) + 1, 1, color);
      else
         DrawBlock(x2, y1, (x1 - x2) + 1, 1, color);
   }
   else if((gfx_DisplayOrientation == DISPLAY_VERTICAL) && (x1 == x2))
   {
      //check to make sure line being requested is on screen
      if(x1 <= GLCD_LINES)
      {  
         if(y1 < y2)
            DrawBlock(y1, GLCD_LINES - x1, (y2 - y1) + 1, 1, color);
         else
            DrawBlock(y2, GLCD_LINES - x1, (y1 - y2) + 1, 1, color);
      }
   }
   else
  #endif
   {
      dx = abs((int16_t)(x2 - x1));
      dy = abs((int16_t)(y2 - y1));
      x = x1;
      y = y1;
      
      if(x1 > x2)
         addx = -1;
      else
         addx = 1;
      if(y1 > y2)
         addy = -1;
      else
         addy = 1;
      
      if(dx >= dy)
      {
         P = 2*dy - dx;
   
         for(i=0; i<=dx; ++i)
         {
            if(gfx_DisplayOrientation == DISPLAY_HORIZONTAL)
               glcd_DrawPixel(x, y, color);
            else
            {
               //check to make sure pixel is on display
               if(x <= GLCD_LINES)
                  glcd_DrawPixel(y, GLCD_LINES - x, color);
            }
   
            if(P < 0)
            {
               P += 2*dy;
               x += addx;
            }
            else
            {
               P += 2*dy - 2*dx;
               x += addx;
               y += addy;
            }
         }
      }
      else
      {
         P = 2*dx - dy;
   
         for(i=0; i<=dy; ++i)
         {
            if(gfx_DisplayOrientation == DISPLAY_HORIZONTAL)
               glcd_DrawPixel(x, y, color);
            else
            {
               //check to make sure pixel is on display
               if(x <= GLCD_LINES)
                  glcd_DrawPixel(y, GLCD_LINES - x, color);
            }
   
            if(P < 0)
            {
               P += 2*dx;
               y += addy;
            }
            else
            {
               P += 2*dx - 2*dy;
               x += addx;
               y += addy;
            }
         }
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
// DrawRectangle()
//
// Draws a rectangle between specified coordinates with specified color.
//
// Parameters:
//    StartX - the first x coordinate to draw the rectangle between.
//
//    StartY - the first y coordinate to draw the rectangle between.
//
//    StopX - the second x coordinate to draw the rectangle between.
//
//    StopY - the second y coordinate to draw the rectangle between.
//
//    Color - the RGB 565 color to draw the rectangle with.
//
//    Fill - if TRUE it will fill the rectangle, if FALSE it will not fill the
//           rectangle.
//
///////////////////////////////////////////////////////////////////////////////
void DrawRectangle(uint16_t StartX, uint16_t StartY, uint16_t StopX, uint16_t StopY, uint16_t Color, int1 Fill = FALSE)
{
   uint16_t Width, Height;
   uint16_t ActualStartX, ActualStartY;
   uint16_t i;
   
   if(StartX < StopX)
   {
      ActualStartX = StartX;
      Width = ((StopX - StartX) + 1);
   }
   else
   {
      ActualStartX = StopX;
      Width = ((StartX - StopX) + 1);
   }
   
   if(StartY < StopY)
   {
      ActualStartY = StartY;
      Height = (StopY - StartY) + 1;
   }
   else
   {
      ActualStartY = StopY;
      Height = (StartY - StopY) + 1;
   }
   
   if(Fill)
   {
     #ifdef USE_FAST_DRAW
      if(gfx_DisplayOrientation == DISPLAY_HORIZONTAL)
         DrawBlock(ActualStartX, ActualStartY, Width, Height, Color);
      else
      {
     #endif
         for(i=0;i<=Width;i++,ActualStartX++)
            DrawLine(ActualStartX, ActualStartY, ActualStartX, (ActualStartY + Height) - 1, Color);
     #ifdef USE_FAST_DRAW
      }
     #endif
   }
   else
   {
      DrawLine(ActualStartX, ActualStartY, (ActualStartX + Width) - 1, ActualStartY, Color);
      DrawLine(ActualStartX, (ActualStartY + Height) - 1, (ActualStartX + Width) - 1, (ActualStartY + Height) - 1, Color);
      DrawLine(ActualStartX, ActualStartY + 1, ActualStartX, (ActualStartY + Height) - 2, Color);
      DrawLine(ActualStartX + Width - 1, ActualStartY + 1, ActualStartX + Width - 1, ActualStartY + Height - 2, Color);
   }
}

///////////////////////////////////////////////////////////////////////////////
// WriteImage()
//
// Displays the specified Image onto the Display starting at the specified 
// coordinates.
//
// Parameters:
//    ImageIndex - the index of the image to display.
//
//    StartX - the upper left hand corner x position to start displaying image
//             at.
//
//    StartY - the upper left hand corner y position to start displaying image 
//             at.
//
// Returns:
//    TRUE is successful, FALSE if an error occurred.
//
///////////////////////////////////////////////////////////////////////////////
int1 WriteImage(uint16_t ImageIndex, uint16_t StartX, uint16_t StartY)
{
   IMAGE ImageData;
   IMAGE_ERR Error;
   FLASH_ADDR Address;
   uint16_t aWidth, aHeight;
   uint16_t i,j;
   uint32_t Pixels;
   uint32_t Count;
   uint16_t *PixelData;
   uint16_t CurrentX, CurrentY, EndX, EndY;
   uint16_t Pixel;
   
   Error = GetImageHeader(ImageIndex, &ImageData.Header);
   Error |= GetImageAddress(ImageIndex, &ImageData.BitmapAddress);
   
   if(Error != IMAGE_ERR_OK)
      return(FALSE);
   
  #ifdef USE_FAST_DRAW
   if(gfx_DisplayOrientation == DISPLAY_HORIZONTAL)
      WriteFastImage(&ImageData, StartX, StartY);
   else
   {
  #endif
      if(gfx_DisplayOrientation == DISPLAY_HORIZONTAL)
      {
         if((StartX + ImageData.Header.Width) > GLCD_PIXELS)
            aWidth = GLCD_PIXELS - StartX;
         else
            aWidth = ImageData.Header.Width;
            
         if((StartY + ImageData.Header.Height) > GLCD_LINES)
            aHeight = GLCD_LINES - StartY;
         else
            aHeight = ImageData.Header.Height;
      }
      else
      {
         if((StartX + ImageData.Header.Width) > GLCD_LINES)
            aWidth = GLCD_LINES - StartX;
         else
            aWidth = ImageData.Header.Width;
         
         if((StartY + ImageData.Header.Height) > GLCD_PIXELS)
            aHeight = GLCD_PIXELS - StartY;
         else
            aHeight = ImageData.Header.Height;
      }
         
      memcpy(&Address, &ImageData.BitmapAddress, sizeof(FLASH_ADDR));
     
      if(gfx_DisplayOrientation == DISPLAY_HORIZONTAL)
         PixelData = malloc(aWidth * 2);
      else
         PixelData = malloc(aHeight * 2);
         
      if(PixelData != NULL)
      {
         if(gfx_DisplayOrientation == DISPLAY_HORIZONTAL)
         {
            for(i=0,CurrentY=StartY;i<aHeight;i++,CurrentY++)
            {
               flash_ReadData(Address, PixelData, aWidth);
               glcd_DrawPixels(StartX, CurrentY, aWidth, 1, PixelData);
               flash_IncAddress(&Address, ImageData.Header.Width);
            }
         }
         else
         {
            CurrentX = StartY;
            CurrentY = GLCD_LINES - StartX - aWidth;
         
            for(i=0;i<aWidth;i++)
            {
               memcpy(&Address, &ImageData.BitmapAddress, sizeof(FLASH_ADDR));
               flash_IncAddress(&Address, aWidth - i);
               
               for(j=0;j<aHeight;j++)
               {
                  flash_ReadData(Address, &PixelData[j], 1);
                  flash_IncAddress(&Address, ImageData.Header.Width);
               }
               
               glcd_DrawPixels(CurrentX,CurrentY, aHeight, 1, PixelData);
               CurrentY++;
            }     
         }
         
         free(PixelData);
      }
      else
      {
         Pixels = (uint32_t)aWidth * (uint32_t)aHeight;
         
         if(gfx_DisplayOrientation == DISPLAY_HORIZONTAL)
         {
            CurrentX = StartX;
            CurrentY = StartY;
            EndX = StartX + aWidth;
         }
         else
         {
            CurrentX = StartY;
            CurrentY = GLCD_LINES - StartX;
            EndY = CurrentY - aWidth;
         }
         
         Count=0;
         while(Count < Pixels)
         {
            flash_ReadData(Address, &Pixel, 1);
            glcd_DrawPixel(CurrentX, CurrentY, Pixel);
            
            flash_IncAddress(&Address);
            Count++;
            
            if(gfx_DisplayOrientation == DISPLAY_HORIZONTAL)
            {
               if(++CurrentX >= EndX)
               {
                  CurrentY++;
                  CurrentX = StartX;
                  
                  if(aWidth != ImageData.Header.Width)
                     flash_IncAddress(&Address, (ImageData.Header.Width - aWidth));
               }
            }
            else
            {
               if(--CurrentY <= EndY)
               {
                  CurrentX++;
                  CurrentY = GLCD_LINES - StartX;
                  
                  if(aWidth != ImageData.Header.Width)
                     flash_IncAddress(&Address, (ImageData.Header.Width - aWidth));
               }
            }
         }
      }
  #ifdef USE_FAST_DRAW
   }
  #endif
   
   return(TRUE);
}

///////////////////////////////////////////////////////////////////////////////
// AdHandle()
//
// Adds and specified image to gfx_ImageHandle array and returns the Handle Id
// assigned to it.
//
// Parameters:
//    ImageIndex - the index of the image to add to the array.
//
//    x - the x position on the display that the image was displayed at.
//
//    y - the y position of the display that the image was displayed at.
//
// Returns:
//    uint16_t Handle Id assigned to the image.
//
///////////////////////////////////////////////////////////////////////////////
uint16_t AdHandle(uint16_t ImageIndex, uint16_t x, uint16_t y)
{
   uint16_t HandleId;
   uint16_t HandleIndex;
   HANDLE_STRUCT *Ptr = NULL;
   IMAGE_HEADER Header;
   uint8_t i;
   
   if(GetImageHeader(ImageIndex, &Header) != IMAGE_ERR_OK)
      return(-1);
   
   HandleId = GetHandleId();
   
   HandleIndex = gfx_HandleCount;
   gfx_HandleCount++;
   
   Ptr = realloc(gfx_ImageHandle, (sizeof(HANDLE_STRUCT) * gfx_HandleCount));
   
   if(Ptr != NULL)
   {
      gfx_ImageHandle = Ptr;
      
      gfx_ImageHandle[HandleIndex].HandleId = HandleId;
      gfx_ImageHandle[HandleIndex].ImageIndex = ImageIndex;
      gfx_ImageHandle[HandleIndex].x = x;
      gfx_ImageHandle[HandleIndex].y = y;
      gfx_ImageHandle[HandleIndex].AreaCount = Header.AreaCount;
      
      if(Header.AreaCount > 0)
      {
         gfx_ImageHandle[HandleIndex].Area = malloc(sizeof(AREA_STRUCT*) * Header.AreaCount);
         
         if(gfx_ImageHandle[HandleIndex].Area != NULL)
         {
            for(i=0;i<Header.AreaCount;i++)
            {
               gfx_ImageHandle[HandleIndex].Area[i] = malloc(sizeof(AREA_STRUCT));
               
               if(gfx_ImageHandle[HandleIndex].Area[i] != NULL)
               {
                  gfx_ImageHandle[HandleIndex].Area[i]->AreaIndex = i;
                  gfx_ImageHandle[HandleIndex].Area[i]->AreaInfo.x1 = Header.Area[i].x1;
                  gfx_ImageHandle[HandleIndex].Area[i]->AreaInfo.y1 = Header.Area[i].y1;
                  gfx_ImageHandle[HandleIndex].Area[i]->AreaInfo.x2 = Header.Area[i].x2;
                  gfx_ImageHandle[HandleIndex].Area[i]->AreaInfo.y2 = Header.Area[i].y2;
                  gfx_ImageHandle[HandleIndex].Area[i]->AreaInfo.Type = Header.Area[i].Type;
               }
               else
                  break;
            }
         }
      }
      else
         gfx_ImageHandle[HandleIndex].Area = NULL;
   }
   else
   {
      gfx_HandleCount--;
      HandleIndex = -1;
   }
   
   return(HandleId);
}

///////////////////////////////////////////////////////////////////////////////
// HasHandle()
//
// Determines if specified Handle Id is in gfx_ImageHandle array.
//
// Parameters:
//    HandleId - Handle Id to check.
//
// Return:
//    TRUE if Handle Id is assigned to an image and in array, FALSE if not in 
//    array.
//
///////////////////////////////////////////////////////////////////////////////
int1 HasHandle(uint16_t HandleId)
{
   uint16_t i;
   
   for(i=0;i<gfx_HandleCount;i++)
   {
      if(gfx_ImageHandle[i].HandleId == HandleId)
         return(TRUE);
   }
   
   return(FALSE);
}

///////////////////////////////////////////////////////////////////////////////
// GetHandle()
//
// Gets the Handle info for the specified Handle Id from gfx_ImageHandle array.
//
// Parameters:
//    HandleId - the Handle Id of to get Handle info for.
//
//    Handle - pointer to a HANDLE_STRUCT structure to return info to.
//
// Returns:
//    TRUE is Handle Id was in structure and info was retrieved, FALSE if
//    Handle Id is not in array.
//
///////////////////////////////////////////////////////////////////////////////
int1 GetHandle(uint16_t HandleId, HANDLE_STRUCT *Handle)
{
   uint16_t i;
   
   for(i=0;i<gfx_HandleCount;i++)
   {
      if(gfx_ImageHandle[i].HandleId == HandleId)
      {
         memcpy(Handle, &gfx_ImageHandle[i], sizeof(HANDLE_STRUCT));
         return(TRUE);
      }
   }
   
   return(FALSE);
}

///////////////////////////////////////////////////////////////////////////////
// GetHandleId()
//
// Returns the next free Handle Id that can be assigned to an image.  Also
// increments the gfx_HandleId to next value, does not check if next value is 
// available.  This function should be called before assigning a Handle Id to
// and image.
//
// Returns:
//    uint16_t value.
//
///////////////////////////////////////////////////////////////////////////////
uint16_t GetHandleId(void)
{
   uint16_t i;
   uint16_t result;
   
   if(gfx_HandleCount > 0)
   {
      int1 Done = FALSE;
      
      while(!Done)
      {
         for(i=0;i<gfx_HandleCount;i++)
         {
            if(gfx_ImageHandle[i].HandleId == gfx_HandleId)
               break;
         }
         
         if(i >= gfx_HandleCount)
         {
            result = gfx_HandleId;
            Done = TRUE;
         }
         else
         {
            if(++gfx_HandleId == -1)
               gfx_HandleId = 0;
         }
      }
   }
   else
      result = gfx_HandleId;
   
   if(++gfx_HandleId == -1)
      gfx_HandleId = 0;
   
   return(result);
}       

///////////////////////////////////////////////////////////////////////////////
// GetArea()
//
// Returns Area info for specified Area.
//
// Parameters:
//    AreaIndex - index of Area to return info about.
//
//    Handle - pointer to HANDLE_STRUCT to get Area info from.
//
//    Area - pointer to AREA_INFO structure to return info to.
//
// Returns:
//    TRUE is successful, FALSE is Area Index is not in HANDLE_STRUCT
//
///////////////////////////////////////////////////////////////////////////////
int1 GetArea(uint8_t AreaIndex, HANDLE_STRUCT *Handle, AREA_INFO *Area)
{
   uint8_t i;
   
   for(i=0;i<Handle->AreaCount;i++)
   {
      if(Handle->Area[i]->AreaIndex == AreaIndex)
      {
         memcpy(Area, &Handle->Area[i]->AreaInfo, sizeof(AREA_INFO));
         return(TRUE);
      }
   }
   
   return(FALSE);
}

///////////////////////////////////////////////////////////////////////////////
// GetCharacter()
//
// Gets specified character from external memory device or FONT8x8 array.
//
// Parameters:
//    Font - pointer to FONT structure to lookup character from.
//
//    Character - the character to get.
//
//    Ptr - pointer to uint8_t array to return character pixel data to.
//
///////////////////////////////////////////////////////////////////////////////
void GetCharacter(FONT *Font, char Character, uint8_t *Ptr)
{
   uint8_t i;
   uint8_t cIndex;
   FLASH_ADDR Address;
   uint16_t FontSize;
   
   if(Font->Header.Index == 0)
   {
      cIndex = Character - ' ';
      
      for(i=0;i<8;i++)
         Ptr[i] = FONT8x8[cIndex][i];
   }
   else
   {
      cIndex = Character - Font->Header.CharacterStart;
      
      memcpy(&Address, &Font->BitmapAddress, sizeof(FLASH_ADDR));
      
      FontSize = Font->Header.Height / 8;
      if((Font->Header.Height % 8) > 0)
         FontSize++;
      
      FontSize *= Font->Header.Width;
      
      if(bit_test(FontSize, 0))
         FontSize++;
         
      FontSize /= 2;
      
      flash_IncAddress(&Address, (uint32_t)FontSize * cIndex);
      
      flash_ReadData(Address, Ptr, FontSize);
   }
}

/////////////////////////// External Memory Function //////////////////////////

#ifndef MAX_DATA_SIZE
 #error MAX_DATA_SIZE not defined, should be defined to max size of array that is passed to gfx_LoadImage() and gfx_Load_Font() functions
#endif

///////////////////////////////////////////////////////////////////////////////
// gfx_LoadImage()
//
// Loads Images into the external memory.  Function is designed to be called 
// multiple times.  For each new image to load the New parameter should be set
// to TRUE on first call and to FALSE for all other calls until entire image
// is loaded.  The define MAX_DATA_SIZE determines the maximum data size that
// can be passed to function for each call.
//
// Parameters:
//    Count - the size of the Data packet being passed to function, should not 
//            exceed MAX_DATA_SIZE.
//
//    Data - pointer to array containing data packet.
//
//    New - specifies if a new Image is being loaded onto external memory.
//
///////////////////////////////////////////////////////////////////////////////
void gfx_LoadImage(uint16_t Count, uint8_t *Data, int1 New)
{
   static IMAGE *BitmapData;
   static uint8_t *DataBuffer = NULL;
   static uint16_t DataIndex;
   static uint16_t *PageBuffer = NULL;
   static uint16_t PageIndex;
   static uint32_t Pixels;
   static uint32_t PixelCount;
   static FLASH_ADDR WriteAddress;
   static FLASH_ADDR HeaderAddress;
   static IMAGE_ERR  Error;
   uint16_t i,j;
   uint16_t ImageIndex;
   uint8_t *Ptr;
   
   if(New)
   {
      ImageIndex = make16(Data[1], Data[0]);
      
      if(ImageIndex >= MAX_IMAGES)
      {
         Error = IMAGE_ERR_INDEX;
         return;
      }
      
      BitmapData = malloc(sizeof(IMAGE));
      
      if(BitmapData == NULL)
      {
         Error = IMAGE_ERR_MEMORY;
         return;
      }
      
      BitmapData->Header.Index = ImageIndex;
      BitmapData->Header.Width = make16(Data[3], Data[2]);
      BitmapData->Header.Height = make16(Data[5], Data[4]);
      BitmapData->Header.AreaCount = Data[6];
      
      Ptr = &Data[7];
      
      i = 0;
      while((i < BitmapData->Header.AreaCount) && (i < MAX_IMAGE_AREAS))
      {
         BitmapData->Header.Area[i].x1 = make16(*(Ptr+1), *Ptr);
         Ptr += 2;
         BitmapData->Header.Area[i].y1 = make16(*(Ptr+1), *Ptr);
         Ptr += 2;
         BitmapData->Header.Area[i].x2 = make16(*(Ptr+1), *Ptr);
         Ptr += 2;
         BitmapData->Header.Area[i].y2 = make16(*(Ptr+1), *Ptr);
         Ptr += 2;
         BitmapData->Header.Area[i].Type = *Ptr;
         Ptr++;
         
         i++;
      }
      
      if(((uint32_t)BitmapData->Header.Width * (uint32_t)BitmapData->Header.Height) > ((uint32_t)GLCD_PIXELS * (uint32_t)GLCD_LINES))
      {
         free(BitmapData);
         BitmapData = NULL;
         
         Error = IMAGE_ERR_SIZE;
         return;
      }
      
      Error = GetImageAddress(ImageIndex, &WriteAddress);
      
      if(Error != IMAGE_ERR_OK)
      {
         free(BitmapData);
         BitmapData = NULL;
         
         return;
      }
      
      memcpy(&BitmapData->BitmapAddress, &WriteAddress, sizeof(FLASH_ADDR));
      
      Error = GetImageHeaderAddress(ImageIndex, &HeaderAddress);
      
      if(Error != IMAGE_ERR_OK)
      {
         free(BitmapData);
         BitmapData = NULL;
         
         return;
      }
      
      DataBuffer = realloc(DataBuffer, MAX_DATA_SIZE + 2);
      PageBuffer = realloc(PageBuffer, MAX_WRITE_SIZE * 2);
      
      if((DataBuffer == NULL) || (PageBuffer == NULL))
      {
         free(DataBuffer);
         free(PageBuffer);
         free(BitmapData);
         
         DataBuffer = NULL;
         PageBuffer = NULL;
         BitmapData = NULL;
         
         Error = IMAGE_ERR_MEMORY;
         return;
      }
      
      flash_ReadData(HeaderAddress, &ImageIndex, 1);
      
      if(ImageIndex != -1)
         flash_EraseBlocks(BitmapData->BitmapAddress, IMAGE_BLOCKS);
     
      DataIndex = 0;
      PixelCount = 0;
      Error = IMAGE_ERR_OK;
      Pixels = (uint32_t)BitmapData->Header.Width * (uint32_t)BitmapData->Header.Height;
      
      i = 7 + (BitmapData->Header.AreaCount * 9);
      
      if(BitmapData->Header.AreaCount > MAX_IMAGE_AREAS)
         BitmapData->Header.AreaCount = MAX_IMAGE_AREAS;
      
      PageIndex = 0;
   }
   
   if(Error == IMAGE_ERR_OK)
   {
      if(New)
      {
         for(j=0;i<Count;i++,j++)
            DataBuffer[j] = Data[i];
         
         DataIndex = j;
      }
      else
      {
         for(j=DataIndex,i=0;i<Count;i++,j++)
            DataBuffer[j] = Data[i];
         
         DataIndex += Count;
      }
      
      i=0;
      
      while((i < (DataIndex - 1)) && (Error == IMAGE_ERR_OK))
      {
         PageBuffer[PageIndex] = make16(DataBuffer[i+1], DataBuffer[i]);
         
         if(++PageIndex >= MAX_WRITE_SIZE)
         {
            flash_WriteData(WriteAddress, PageBuffer, MAX_WRITE_SIZE);
            if(flash_IncAddress(&WriteAddress, MAX_WRITE_SIZE) == FALSE)
               Error = IMAGE_ERRFLASH_ADDRESS;
            PageIndex = 0;
         }
         
         PixelCount++;
         i += 2;
      }
      
      if(i != DataIndex)
      {
         for(j=i;j<DataIndex;j++)
            DataBuffer[j-i] = DataBuffer[j];
         
         DataIndex = DataIndex - i;
      }
      else
         DataIndex = 0;
   }
   
   if((Error != IMAGE_ERR_OK) || (PixelCount >= Pixels))
   {
      if((Error == IMAGE_ERR_OK) && (PageIndex != 0))
         flash_WriteData(WriteAddress, PageBuffer, PageIndex);
         
      if(Error == IMAGE_ERR_OK)
         flash_WriteData(HeaderAddress, BitmapData, (sizeof(IMAGE) / 2));
      
      free(PageBuffer);
      free(DataBuffer);
      free(BitmapData);
      
      PageBuffer = NULL;
      DataBuffer = NULL;
      BitmapData = NULL;
      
      if(Error == IMAGE_ERR_OK)
         Error = IMAGE_ERR_DONE;
   }
}

///////////////////////////////////////////////////////////////////////////////
// gfx_EraseImage()
//
// Erases specified Image from external memory.
//
// Parameters:
//    Index - index of Image to erase from external memory.
//
///////////////////////////////////////////////////////////////////////////////
void gfx_EraseImage(uint16_t Index)
{
   FLASH_ADDR Address;
   IMAGE_ERR Error;
   
   Error = GetImageAddress(Index, &Address);
   
   if(Error == IMAGE_ERR_OK)
   {
      flash_EraseBlocks(Address, IMAGE_BLOCKS);
   }
}

///////////////////////////////////////////////////////////////////////////////
// gfx_LoadFont()
//
// Loads Fonts into the external memory.  Function is designed to be called
// multiple times.  For each new Font to load the New parameter should be set
// to TRUE on first call and to FALSE for all other called until entire Font
// is loaded.  The define MAX_DATA_SIZE determines the maximum data size that
// can be passed to function for each call.
//
// Parameters:
//    Count - the size of the Data packet being passed to function, should not 
//            exceed MAX_DATA_SIZE.
//
//    Data - pointer to array containing data packet.
//
//    New - specifies if a new Font is being loaded onto external memory.
//
///////////////////////////////////////////////////////////////////////////////
void gfx_LoadFont(uint16_t Count, uint8_t *Data, int1 New)
{
   static FONT *FontData;
   static uint8_t *DataBuffer = NULL;
   static uint16_t DataIndex;
   static uint16_t *PageBuffer = NULL;
   static uint16_t PageIndex;
   static uint32_t Bytes;
   static uint32_t ByteCount;
   static FLASH_ADDR WriteAddress;
   static FLASH_ADDR HeaderAddress;
   static FONT_ERR  Error;
   uint16_t i,j;
   uint16_t FontIndex;
      
   if(New)
   {
      FontIndex = make16(Data[1], Data[0]);
      
      if(FontIndex >= MAX_FONTS)
      {
         Error = FONT_ERR_INDEX;
         
         return;
      }
      
      FontData = malloc(sizeof(FONT));
      
      if(FontData == NULL)
      {
         Error = FONT_ERR_MEMORY;
         
         return;
      }
      
      FontData->Header.Index = FontIndex;
      FontData->Header.Width = Data[2];
      FontData->Header.Height = Data[3];
      FontData->Header.Count = Data[4];
      FontData->Header.CharacterStart = Data[5];
      
      if((FontData->Header.Width > MAX_FONT_WIDTH) || (FontData->Header.Height > MAX_FONT_HEIGHT))
      {
         free(FontData);
         FontData = NULL;
         
         Error = FONT_ERR_SIZE;
         return;
      }
      
      Error = GetFontAddress(FontIndex, &WriteAddress);
      
      if(Error != FONT_ERR_OK)
      {
         free(FontData);
         FontData = NULL;
         
         return;
      }
      
      memcpy(&FontData->BitmapAddress, &WriteAddress, sizeof(FLASH_ADDR));
      
      Error = GetFontHeaderAddress(FontIndex, &HeaderAddress);
      
      if(Error != FONT_ERR_OK)
      {
         free(FontData);
         FontData = NULL;
         
         return;
      }
      
      DataBuffer = realloc(DataBuffer, MAX_DATA_SIZE + 2);
      PageBuffer = realloc(PageBuffer, MAX_WRITE_SIZE * 2);
      
      if((DataBuffer == NULL) || (PageBuffer == NULL))
      {
         free(DataBuffer);
         free(PageBuffer);
         free(FontData);
         
         DataBuffer = NULL;
         PageBuffer = NULL;
         FontData = NULL;
         
         Error = FONT_ERR_MEMORY;
         
         return;
      }
      
      flash_ReadData(HeaderAddress, &FontIndex, 1);
      
      if(FontIndex != -1)
         flash_EraseBlocks(FontData->BitmapAddress, FONT_BLOCKS);
      
      DataIndex = 0;
      ByteCount = 0;
      Error = IMAGE_ERR_OK;
      Bytes = ((FontData->Header.Height / 8) + (((FontData->Header.Height % 8) > 0) ? 1 : 0)) * FontData->Header.Width;
      Bytes += (((Bytes % 2) > 0) ? 1 : 0);
      Bytes *= FontData->Header.Count;
      
      i = 6;
      PageIndex = 0;
   }
   
   if(Error == FONT_ERR_OK)
   {
      if(New)
      {
         for(j=0;i<Count;i++,j++)
            DataBuffer[j] = Data[i];
         
         DataIndex = j;
      }
      else
      {
         for(j=DataIndex,i=0;i<Count;i++,j++)
            DataBuffer[j] = Data[i];
         
         DataIndex += Count;
      }
      
      i=0;
      
      while((i < (DataIndex - 1)) && (Error == FONT_ERR_OK))
      {
         PageBuffer[PageIndex] = make16(DataBuffer[i+1], DataBuffer[i]);
         
         if(++PageIndex >= MAX_WRITE_SIZE)
         {
            flash_WriteData(WriteAddress, PageBuffer, MAX_WRITE_SIZE);
            if(flash_IncAddress(&WriteAddress, MAX_WRITE_SIZE) == FALSE)
               Error = FONT_ERRFLASH_ADDRESS;
            PageIndex = 0;
         }
         
         ByteCount += 2;
         i += 2;
      }
      
      if(i != DataIndex)
      {
         for(j=i;j<DataIndex;j++)
            DataBuffer[j-i] = DataBuffer[j];
         
         DataIndex = DataIndex - i;
      }
      else
         DataIndex = 0;
   }
   
   if((Error != FONT_ERR_OK) || (ByteCount >= Bytes))
   {
      if((Error == FONT_ERR_OK) && (PageIndex != 0))
         flash_WriteData(WriteAddress, PageBuffer, PageIndex);
         
      if(Error == FONT_ERR_OK)
         flash_WriteData(HeaderAddress, FontData, (sizeof(FONT) / 2));
      
      free(PageBuffer);
      free(DataBuffer);
      free(FontData);
      
      PageBuffer = NULL;
      DataBuffer = NULL;
      FontData = NULL;
      
      if(Error == FONT_ERR_OK)
         Error = FONT_ERR_DONE;
   }
}

///////////////////////////////////////////////////////////////////////////////
// gfx_EraseFont()
//
// Erases specified Font from external memory.
//
// Parameters:
//    Index - index of Font to erase from external memory.
//
///////////////////////////////////////////////////////////////////////////////
void gfx_EraseFont(uint16_t Index)
{
   FLASH_ADDR Address;
   FONT_ERR Error;
   
   Error = GetFontAddress(Index, &Address);
   
   if(Error == FONT_ERR_OK)
   {
      flash_EraseBlocks(Address, FONT_BLOCKS);
   }
}

///////////////////////////////////////////////////////////////////////////////
// GetImageAddress()
//
// Returns the address in the external memory that the specified Image is
// located.
//
// Parameters:
//    ImageIndex - the index of the Image address to retrieve.
//
//    Address - pointer to a FLASH_ADDR Address to return the retrieved address
//              to.
//
// Returns:
//    IMAGE_ERR value.  IMAGE_ERR_OK if address was retrieved successfully, 
//    other value if an error occurred.
//
///////////////////////////////////////////////////////////////////////////////
IMAGE_ERR GetImageAddress(uint16_t ImageIndex, FLASH_ADDR *Address)
{
   if(ImageIndex >= MAX_IMAGES)
      return(IMAGE_ERR_INDEX);
      
   return(flash_SetAddress(ImageIndex * (IMAGE_BLOCKS * FLASH_ERASE_SIZE), Address));
}

///////////////////////////////////////////////////////////////////////////////
// GetImageHeaderAddress()
//
// Returns the address in the external memory that the specified Image Header 
// is located.
//
// Parameters:
//    ImageIndex - the index of the Image Header address to retrieve.
//
//    Address - pointer to a FLASH_ADDR Address to return the retrieved address
//              to.
//
// Returns:
//    IMAGE_ERR value.  IMAGE_ERR_OK if address was retrieved successfully,
//    other value if an error occurred.
//
///////////////////////////////////////////////////////////////////////////////
IMAGE_ERR GetImageHeaderAddress(uint16_t ImageIndex, FLASH_ADDR *Address)
{
   IMAGE_ERR Error = IMAGE_ERR_OK;
   
   if(ImageIndex >= MAX_IMAGES)
      return(IMAGE_ERR_INDEX);
      
   Error = flash_SetAddress(ImageIndex * (IMAGE_BLOCKS * FLASH_ERASE_SIZE), Address);
   
   if(Error != IMAGE_ERR_OK)
      return(Error);

   if(flash_IncAddress(Address, (GLCD_PIXELS * GLCD_LINES)) == FALSE)
      Error = IMAGE_ERR_INDEX_ADDRESS;
   
   return(Error);
}

///////////////////////////////////////////////////////////////////////////////
// GetImageHeader()
//
// Gets the IMAGE_HEADER for the specified Image.
//
// Parameters:
//    ImageIndex - the index of the Image Header the retrieve.
//
//    Header - pointer to the IMAGE_HEADER structure to return the retrieved 
//             header to.
//
// Returns:
//    IMAGE_ERR value.  IMAGE_ERR_OK if header was retrieved successfully,
//    other value if an error occurred
//
///////////////////////////////////////////////////////////////////////////////
IMAGE_ERR GetImageHeader(uint16_t ImageIndex, IMAGE_HEADER *Header)
{
   IMAGE_ERR Error;
   FLASH_ADDR Address;
   
   Error = GetImageHeaderAddress(ImageIndex, &Address);
   
   if(Error == IMAGE_ERR_OK)
   {
      flash_ReadData(Address, Header, (sizeof(IMAGE_HEADER) / 2));
      
      if(Header->Index == -1)
         Error = IMAGE_ERR_NOT_LOADED;
      else if(Header->Index != ImageIndex)
         Error = IMAGE_ERR_INDEX;
   }
   
   return(Error);
}

///////////////////////////////////////////////////////////////////////////////
// GetFontAddress()
//
// Returns the address in the external memory that the specified Font is
// located.
//
// Parameters:
//    FontIndex - the index of the Font address to retrieve.
//
//    Address - pointer to the FLASH_ADDR address to return the retrieved 
//              address to.
//
// Returns:
//    FONT_ERR value.  FONT_ERR_OK if address was retrieved successfully, other 
//    value if an error occurred.
//
///////////////////////////////////////////////////////////////////////////////
FONT_ERR GetFontAddress(uint16_t FontIndex, FLASH_ADDR *Address)
{
   if((FontIndex == 0) || (FontIndex > MAX_FONTS))
      return(FONT_ERR_INDEX);
   
   return(flash_SetAddress((FONT_START_BLOCK * FLASH_ERASE_SIZE) + ((FontIndex - 1) * (FONT_BLOCKS * FLASH_ERASE_SIZE)), Address));
}

///////////////////////////////////////////////////////////////////////////////
// GetFontHeaderAddress()
//
// Returns the address in the external memory that the specified Font Header is
// located.
//
// Parameters:
//    FontIndex - the index of the Font Header address to retrieve
//
//    Address - pointer to the FLASH_ADDR address to return the retrieved
//              address to.
//
// Returns:
//    FONT_ERR value.  FONT_ERR_OK if address was retrieved successfully, other 
//    value if an error occurred.
//
///////////////////////////////////////////////////////////////////////////////
FONT_ERR GetFontHeaderAddress(uint16_t FontIndex, FLASH_ADDR *Address)
{
   FONT_ERR Error = FONT_ERR_OK;
   
   if((FontIndex == 0) || (FontIndex > MAX_FONTS))
      return(FONT_ERR_INDEX);
   
   Error = flash_SetAddress((FONT_START_BLOCK * FLASH_ERASE_SIZE) + ((FontIndex - 1) * (FONT_BLOCKS * FLASH_ERASE_SIZE)), Address);
   
   if(Error == FONT_ERR_OK)
   {  
      if(flash_IncAddress(Address, ((FONT_SIZE * MAX_FONT_CHARACTERS) / 2)) == FALSE)
         Error = FONT_ERR_INDEX_ADDRESS;
   }
   
   return(Error);
}

///////////////////////////////////////////////////////////////////////////////
// GetFontInfo()
//
// Gets the FONT data for the specified Font.
//
// Parameters:
//    FontIndex - the index of the FONT data to retrieve.
//
//    FontData - pointer to a FONT structure to return the retrieved data to.
//
// Returns:
//    FONT_ERR value.  FONT_ERR_OK if data was retrieved successfully, other 
//    value if an error occurred.
//
///////////////////////////////////////////////////////////////////////////////
FONT_ERR GetFontInfo(uint16_t FontIndex, FONT *FontData)
{
   FONT_ERR Error;
   FLASH_ADDR Address;
   
   Error = GetFontHeaderAddress(FontIndex, &Address);
   
   if(Error == FONT_ERR_OK)
   {
      flash_ReadData(Address, FontData, (sizeof(FONT) / 2));
      
      if(FontData->Header.Index == -1)
         Error = FONT_ERR_NOT_LOADED;
      else if(FontData->Header.Index != FontIndex)
         Error = FONT_ERR_INDEX;
   }
   
   return(Error);
}

/////////////////////////////// Touch Functions ///////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// gfx_TouchTask()
//
// Task function that processes touches and calls user touch function that was 
// set with gfx_SetTouchCallback() function.  Function should be called 
// frequently, at least once per iteration of user's main loop.
//
///////////////////////////////////////////////////////////////////////////////
void gfx_TouchTask(void)
{
   uint16_t Coordinates[2];
   static int1 newTouch = FALSE;
   static uint16_t XPosition, YPosition;
   static int1 PreviousState = NOT_TOUCHED;
   GFX_TICK CurrentTick;
   static GFX_TICK PreviousSentTick, PreviousTouchTick;
   uint16_t Temp;
   
   CurrentTick = gfx_get_tick();
   
   if(touch_Ready())
   {
      touch_ReadChannels(Coordinates, 2);
      
      if(gfx_TouchCalibration.Rotated)
      {
         XPosition = ((uint32_t)(Coordinates[0] & 0x0FFF) * (GLCD_LINES - 1)) / 4095;
         YPosition = ((uint32_t)(Coordinates[1] & 0x0FFF) * (GLCD_PIXELS - 1)) / 4095;
         
         if(gfx_TouchCalibration.X.Reversed == FALSE)
            XPosition -= ((((int32_t)gfx_TouchCalibration.X.Slope * XPosition) / 1000) + gfx_TouchCalibration.X.Offset);
         else
            XPosition = GLCD_LINES - (XPosition - ((((int32_t)gfx_TouchCalibration.X.Slope * XPosition) / 1000) + gfx_TouchCalibration.X.Offset));
         
         if(gfx_TouchCalibration.Y.Reversed == FALSE)
            YPosition -= ((((int32_t)gfx_TouchCalibration.Y.Slope * YPosition) / 1000) + gfx_TouchCalibration.Y.Offset);
         else
            YPosition = GLCD_PIXELS - (YPosition - ((((int32_t)gfx_TouchCalibration.Y.Slope * YPosition) / 1000) + gfx_TouchCalibration.Y.Offset));
      }
      else
      {
         XPosition = ((uint32_t)(Coordinates[0] & 0x0FFF) * (GLCD_PIXELS - 1)) / 4095;
         YPosition = ((uint32_t)(Coordinates[1] & 0x0FFF) * (GLCD_LINES - 1)) / 4095;
         
         if(gfx_TouchCalibration.X.Reversed == FALSE)
            XPosition -= ((((int32_t)gfx_TouchCalibration.X.Slope * XPosition) / 1000) + gfx_TouchCalibration.X.Offset);
         else
            XPosition = GLCD_PIXELS - (XPosition - ((((int32_t)gfx_TouchCalibration.X.Slope * XPosition) / 1000) + gfx_TouchCalibration.X.Offset));
         
         if(gfx_TouchCalibration.Y.Reversed == FALSE)
            YPosition -= ((((int32_t)gfx_TouchCalibration.Y.Slope * YPosition) / 1000) + gfx_TouchCalibration.Y.Offset);
         else
            YPosition = GLCD_LINES - (YPosition - ((((int32_t)gfx_TouchCalibration.Y.Slope * YPosition) / 1000) + gfx_TouchCalibration.Y.Offset));
      }
      
      if((gfx_DisplayOrientation == DISPLAY_VERTICAL) && (gfx_TouchCalibration.Rotated == FALSE))
      {
         Temp = XPosition;
         XPosition = GLCD_LINES - YPosition;
         YPosition = Temp;
      }
      else if(gfx_DisplayOrientation == DISPLAY_VERTICAL)
      {
         XPosition = GLCD_LINES - XPosition;
      }
      else if(gfx_TouchCalibration.Rotated == TRUE)
      {
         Temp = XPosition;
         XPosition = YPosition;
         YPosition = Temp;
      }
     
      newTouch = TRUE;
   }
   
   if(PreviousState == TOUCHED)
   {
      if(newTouch)
      {
         newTouch = FALSE;
         
         PreviousTouchTick = CurrentTick;
      }
      else if(gfx_tick_difference(CurrentTick, PreviousTouchTick) >= (GFX_TICK)TOUCH_UPDATE_RATE)
      {
         PreviousState = NOT_TOUCHED;
         
         if(g_TouchFunctionPtr != NULL)
            TouchCallback(TOUCH_TYPE_RELEASED, XPosition, YPosition);
      }
      
      if((PreviousState == TOUCHED) && (g_TouchOnlyUpDown == FALSE) && (gfx_tick_difference(CurrentTick, PreviousSentTick) >= (TICK_TYPE)TOUCH_UPDATE_RATE))
      {
         if(g_TouchFunctionPtr != NULL)
            TouchCallback(TOUCH_TYPE_PRESSING, XPosition, YPosition);
         
         PreviousSentTick = CurrentTick;
      }
   }
   else
   {
      if(newTouch)
      {
         if(g_TouchFunctionPtr != NULL)
            TouchCallback(TOUCH_TYPE_PRESSED, XPosition, YPosition);
         
         newTouch = FALSE;
         PreviousState = TOUCHED;
         PreviousTouchTick = PreviousSentTick = CurrentTick;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
// gfx_SetTouchCallback()
//
// Used to set user's touch function that is called by gfx_TouchTask() when
// screen is touched.
//
// Parameters:
//    func - function that is to be called when a touch is detected.
//
//    OnlyValidAreas - specifies what areas of screen causes gfx_TouchTask()
//                     to call user function when screen is touched.  If TRUE
//                     will only call function when an area of type
//                     AREA_TYPE_BUTTON is touched.  If FALSE will call
//                     function when any position on screen is pressed.
//
//    OnlyUpDown - specifies when gfx_TouchTask() calls user function.  If TRUE
//                 it will call the function when screen is touched and when 
//                 screen is released.  If FALSE it will call the function when
//                 screen is touched, when screen is release and at a rate of
//                 TOUCH_UPDATE_RATE while screen is being pressed.
//
///////////////////////////////////////////////////////////////////////////////
void gfx_SetTouchCallback(_touchpfunc func, int1 OnlyValidAreas, int1 OnlyUpDown)
{
   g_TouchFunctionPtr = func;
   g_TouchOnlyValidAreas = OnlyValidAreas;
   g_TouchOnlyUpDown = OnlyUpDown;
}

///////////////////////////////////////////////////////////////////////////////
// TouchCallback()
//
// Called by gfx_TouchTask() when screen is touched, screen is released and at
// TOUCH_UPDATE_RATE while screen is being pressed, if g_TouchOnlyUpDown is 
// FALSE.  It determines what area was touched and calls the users's touch
// function that was set with gfx_SetTouchCallback().
//
// Parameters:
//    Kind - specifies the type of touch.  Can be TOUCH_TYPE_PRESSED, 
//           TOUCH_TYPE_RELEASED, TOUCH_TYPE_PRESSING.
//
//    RealX - the actual x position that was touched.
//
//    RealY - the actual y position that was touched.
//       
//
///////////////////////////////////////////////////////////////////////////////
void TouchCallback(TOUCH_TYPE Kind, uint16_t RealX, uint16_t RealY)
{
   HANDLE_STRUCT *Handle;
   AREA_STRUCT *Area;
   int16_t i;
   uint8_t j;
   uint16_t ImageHandle = -1;
   uint8_t AreaIndex = -1;
   
   if(gfx_HandleCount > 0)
   {
      i = gfx_HandleCount - 1;
      
      while((i >= 0) && (ImageHandle == -1))
      {
         Handle = &gfx_ImageHandle[i];
         
         if(Handle->AreaCount > 0)
         {
            j = 0;
            
            while((j<Handle->AreaCount) && (AreaIndex == -1))
            {
               Area = Handle->Area[j];
               
               if((RealX >= (Handle->x + Area->AreaInfo.x1)) && (RealX <= (Handle->x + Area->AreaInfo.x2)) &&
                  (RealY >= (Handle->y + Area->AreaInfo.y1)) && (RealY <= (Handle->y + Area->AreaInfo.y2)))
               {
                  if(((g_TouchOnlyValidAreas == TRUE) && (Area->AreaInfo.Type == AREA_TYPE_BUTTON)) || (g_TouchOnlyValidAreas == FALSE))
                  {
                     AreaIndex = Area->AreaIndex;
                     
                     RealX -= (Handle->x + Area->AreaInfo.x1);
                     RealY -= (Handle->y + Area->AreaInfo.y1);
                  }
               }
               
               j++;
            }
            
            if(AreaIndex != -1)
               ImageHandle = Handle->HandleId;
         }
         
         i--;
      }
   }
   
   if((ImageHandle != -1) || (g_TouchOnlyValidAreas == FALSE))
      (*g_TouchFunctionPtr)(ImageHandle, AreaIndex, Kind, RealX, RealY);
}

///////////////////////////////////////////////////////////////////////////////
// TouchFunction()
//
// Dummy user touch function that gets assigned with the gfx_InitGraphics(),
// required so library will compile correctly.
//
///////////////////////////////////////////////////////////////////////////////
void TouchFunction(uint16_t HandleId, uint8_t AreaIndex, TOUCH_TYPE Kind, uint16_t x, uint16_t y)
{

}

///////////////////////////// Backlight Functions /////////////////////////////

#ifdef GFX_BACKLIGHT_PIN
///////////////////////////////////////////////////////////////////////////////
// gfx_InitBacklight()
//
// Initializes the Backlight.  Only available if GFX_BACKLIGHT_PIN if defined.
//
///////////////////////////////////////////////////////////////////////////////
void gfx_InitBacklight(void)
{
   g_CurrentBacklightLevel = g_CurrentBacklightLevel = (GFX_BACKLIGHT_START_DUTY * 10);
}

///////////////////////////////////////////////////////////////////////////////
// gfx_SetBacklightLevel()
//
// Sets the backlight pwm to the specified pwm duty percent.  Only available if 
// GFX_BACKLIGHT_PIN is defined.
//
// Parameters:
//    duty - the percentage to set the pwm duty to, can be 0 - 100.
//
///////////////////////////////////////////////////////////////////////////////
void gfx_SetBacklightLevel(uint8_t duty)
{
   if(duty > 100)
      g_BacklightLevel = 1000;
   else
      g_BacklightLevel = (uint16_t)duty * 10;
}

///////////////////////////////////////////////////////////////////////////////
// gfx_BacklightTask()
//
// Task function that actual adjust the pwm duty.  Only available if 
// GFX_BACKLIGHT_PIN defined.
//
///////////////////////////////////////////////////////////////////////////////
void gfx_BacklightTask(void)
{
   static GFX_TICK CurrentTick, PreviousTick;
   
   CurrentTick = gfx_get_tick();
   
   if(gfx_tick_difference(CurrentTick, PreviousTick) >= ((GFX_TICK)GFX_TICKS_PER_SECOND / 1000))
   {
      if(g_CurrentBacklightLevel != g_BacklightLevel)
      {
         if(g_CurrentBacklightLevel < g_BacklightLevel)
         {
            g_CurrentBacklightLevel += 100;
            
            if(g_CurrentBacklightLevel > g_BacklightLevel)
               g_CurrentBacklightLevel = g_BacklightLevel;
         }
         else
         {
            g_CurrentBacklightLevel -= 100;
            
            if((g_CurrentBacklightLevel < g_BacklightLevel) || (g_CurrentBacklightLevel >= BACKLIGHT_MAX_DUTY))
               g_CurrentBacklightLevel = g_BacklightLevel;
         }
            
         pwm_set_duty(GFX_BACKLIGHT_STREAM, g_CurrentBacklightLevel);
      }
      
      PreviousTick = CurrentTick;
   }
}
#endif

/////////////////////////////// Serial Functions //////////////////////////////

#ifdef GFX_USE_SERIAL_INTERFACE
///////////////////////////////////////////////////////////////////////////////
// gfx_SerialTask()
//
// Optional Serial Task function that can be added to communicate with CCS's
// PC Application.
//
///////////////////////////////////////////////////////////////////////////////
void gfx_SerialTask(void)
{
   SERIAL_STRUCT *Ptr;
   uint16_t Index;
   uint16_t HandleId;
   uint16_t cPacketCount;
   static uint16_t pPacketCount;
   
   SerialRxTask();
   
   if(SerialCommandKbhit())
   {
      Ptr = &g_SerialBuffer[g_SerialNextOut];
      
      switch(Ptr->Cmd)
      {
         case SERIAL_CMD_IMAGE_START:
         case SERIAL_CMD_IMAGE_CONT:
            cPacketCount = make16(Ptr->Data[1], Ptr->Data[0]);
            
            if(Ptr->Cmd == SERIAL_CMD_IMAGE_START)
            {
               if(cPacketCount != 0)
               {
                  SendAckNack(SERIAL_CMD_NACK);
                  break;
               }
               
               pPacketCount = -1;
            }
            
            if(pPacketCount != cPacketCount)
            {
               gfx_LoadImage(Ptr->Count - 2, &Ptr->Data[2], Ptr->Cmd == SERIAL_CMD_IMAGE_START);
               
               pPacketCount = cPacketCount;
            }
            
            SendAckNack(SERIAL_CMD_ACK);
            break;
         case SERIAL_CMD_FONT_START:
         case SERIAL_CMD_FONT_CONT:
            cPacketCount = make16(Ptr->Data[1], Ptr->Data[0]);
            
            if(Ptr->Cmd == SERIAL_CMD_FONT_START)
            {
               if(cPacketCount != 0)
               {
                  SendAckNack(SERIAL_CMD_NACK);
                  break;
               }
               
               pPacketCount = -1;
            }
            
            if(pPacketCount != cPacketCount)
            {
               gfx_LoadFont(Ptr->Count - 2, &Ptr->Data[2], Ptr->Cmd == SERIAL_CMD_FONT_START);
               
               pPacketCount = cPacketCount;
            }
            
            SendAckNack(SERIAL_CMD_ACK);
            break;
         case SERIAL_CMD_DISPLAY_IMAGE:
            if(Ptr->Count == 6)
            {
               HandleId = gfx_DisplayImage(make16(Ptr->Data[1], Ptr->Data[0]), make16(Ptr->Data[3], Ptr->Data[2]), make16(Ptr->Data[5], Ptr->Data[4]));
               
               if(HandleId != -1)
                  SerialSendData(SERIAL_CMD_DISPLAY_IMAGE, &HandleId, 2);
            }
            break;
         case SERIAL_CMD_REMOVE_IMAGE:
            if(Ptr->Count >= 2)
            {
               if(Ptr->Count == 2)
                  Ptr->Data[2] = FALSE;
               
               gfx_RemoveImage(make16(Ptr->Data[1], Ptr->Data[0]), Ptr->Data[2]);
            }
            break;
         case SERIAL_CMD_ERASE_FLASH:
            flash_BulkErase();
            break;
         case SERIAL_CMD_ERASE_IMAGE:
            if(Ptr->Count > 0)
            {
               if(Ptr->Count > 1)
                  Index = make16(Ptr->Data[1], Ptr->Data[0]);
               else
                  Index = make16(0, Ptr->Data[0]);
               
               gfx_EraseImage(Index);
            }
            break;
         case SERIAL_CMD_CLEAR_SCREEN:
            if(Ptr->Count >= 2)
            {
               if(Ptr->Count == 2)
                  Ptr->Data[2] = TRUE;
               
               gfx_ClearScreen(make16(Ptr->Data[1], Ptr->Data[0]), Ptr->Data[2]);
            }
            break;
         case SERIAL_CMD_DISPLY_STRING:
            if(Ptr->Count > 8)
               gfx_DisplayString(make16(Ptr->Data[1], Ptr->Data[0]), Ptr->Data[2], Ptr->Data[3], make16(Ptr->Data[5], Ptr->Data[4]), make16(Ptr->Data[7], Ptr->Data[6]), &Ptr->Data[8]);
            break;
         case SERIAL_CMD_FILL_AREA:
            if(Ptr->Count == 10)
               gfx_FillArea(make16(Ptr->Data[1], Ptr->Data[0]), Ptr->Data[2], Ptr->Data[3], make32(Ptr->Data[7], Ptr->Data[6], Ptr->Data[5], Ptr->Data[4]), make16(Ptr->Data[9], Ptr->Data[8]));
            break;
         case SERIAL_CMD_DISPLAY_STRING_AB:
            if(Ptr->Count > 9)
               gfx_DisplayStringAbsolute(make16(Ptr->Data[1], Ptr->Data[0]), make16(Ptr->Data[3], Ptr->Data[2]), Ptr->Data[4], make16(Ptr->Data[6], Ptr->Data[5]), make16(Ptr->Data[8], Ptr->Data[7]), &Ptr->Data[9]);
            break;
         case SERIAL_CMD_FILL_AREA_AB:
            if(Ptr->Count == 10)
               gfx_FillAreaAbsolute(make16(Ptr->Data[1], Ptr->Data[0]), make16(Ptr->Data[3], Ptr->Data[2]), make16(Ptr->Data[5], Ptr->Data[4]), make16(Ptr->Data[7], Ptr->Data[6]), make16(Ptr->Data[9], Ptr->Data[8]));
            break;
         case SERIAL_CMD_DRAW_LINE_AB:
            if(Ptr->Count == 10)
               gfx_DrawLineAbsolute(make16(Ptr->Data[1], Ptr->Data[0]), make16(Ptr->Data[3], Ptr->Data[2]), make16(Ptr->Data[5], Ptr->Data[4]), make16(Ptr->Data[7], Ptr->Data[6]), make16(Ptr->Data[9], Ptr->Data[8]));
            break;
      }
      
      if(++g_SerialNextOut >= SERIAL_BUFFERS)
         g_SerialNextOut = 0;
   }
}

///////////////////////////////////////////////////////////////////////////////
// SerialRxTask()
//
// Optional Task function for receiving data packets from CCS's PC application.
// Function is automatically called by gfx_SerialTask() function.
//
///////////////////////////////////////////////////////////////////////////////
void SerialRxTask(void)
{
   static SERIAL_RX_MODE RxMode = SERIAL_START;
   static uint16_t Index = 0;
   static uint8_t LsByte;
   static uint8_t cs;
   static uint16_t Count;
   uint8_t c;
   
   if(gfx_kbhit())
   {
      c = gfx_getc();
      
      switch(RxMode)
      {
         case SERIAL_START:
            if(c == SERIAL_START_BYTE)
            {
               RxMode++;
               Index = 0;
               cs = 0;
            }
            break;
         case SERIAL_CMD:
            cs += c;
            
            g_SerialBuffer[g_SerialNextIn].Cmd = c;
            RxMode++;
            break;
         case SERIAL_SIZE:
            cs += c;
            
            if(Index == 0)
            {
               LsByte = c;
               Index++;
            }
            else
            {
               Count = make16(c, LsByte);
               
               if(Count <= MAX_SERIAL_DATA_SIZE)
               {
                  if(Count > 0)
                  {
                     g_SerialBuffer[g_SerialNextIn].Count = Count;
                     Index = 0;
                     RxMode++;
                  }
                  else
                     RxMode = SERIAL_CS;
               }
               else
                  RxMode = SERIAL_START;
            }
            break;
         case SERIAL_DATA:
            cs += c;
            
            g_SerialBuffer[g_SerialNextIn].Data[Index] = c;
            
            if(++Index >= Count)
               RxMode++;
            break;
         case SERIAL_CS:
            if(c != cs)
               SendAckNack(SERIAL_CMD_NACK);
            else
            {
               if(++g_SerialNextIn >= SERIAL_BUFFERS)
                  g_SerialNextIn = 0;
            }
            RxMode = SERIAL_START;
            break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
// SerialCommandKbhit()
//
// Optional function for determining if there is a data packet that needs to be
// pulled from serial buffer and processed.  Function is automatically called
// by gfx_SerialTask() function.
//
// Returns:
//    TRUE if there is a data packet in serial buffer.  FALSE if there is no
//    packet.
//
///////////////////////////////////////////////////////////////////////////////
int1 SerialCommandKbhit(void)
{
   if(g_SerialNextOut != g_SerialNextIn)
      return(TRUE);
   else
      return(FALSE);
}

///////////////////////////////////////////////////////////////////////////////
// SendAckNack()
//
// Optional function for sending a Serial ACK or NAK.  Function is 
// automatically called by gfx_SerialTask() and SerialRxTask() functions.
//
// Parameters:
//    Command - specifies whether to send an ACK or NACK.
//
///////////////////////////////////////////////////////////////////////////////
void SendAckNack(uint8_t Command)
{
   gfx_putc(SERIAL_START_BYTE);
   gfx_putc(Command);
   gfx_putc(0x00);
   gfx_putc(0x00);
   gfx_putc(Command);
}

///////////////////////////////////////////////////////////////////////////////
// SerialSendByte()
//
// Optional function used to send a byte and add value to checksum.  Function 
// is automatically called by SerialSendData() function.
//
// Parameters:
//    Data - data byte to send.
//
//    cs - initial checksum value to add data byte to.
//
// Returns:
//    uint8_t checksum value.
//
///////////////////////////////////////////////////////////////////////////////
uint8_t SerialSendByte(uint8_t Data, uint8_t cs)
{
   gfx_putc(Data);
   
   return(cs + Data);
}

///////////////////////////////////////////////////////////////////////////////
// SerialSendData()
//
// Optional function for sending a data packet.  Automatically called by 
// gfx_SerialTask() function.
//
// Parameters:
//    SerialCmd - command to send packet with.  Commands are defined in 
//                gfx_graphics.h.
//
//    Data - pointer to uint8_t data array containing packet data to send.
//
//    Count - number of byte is data portion of packet.  Should not exceed
//            MAX_SERIAL_DATA_SIZE.
//
///////////////////////////////////////////////////////////////////////////////
void SerialSendData(uint8_t SerialCmd, uint8_t *Data, uint16_t Count)
{
   uint8_t cs = 0;
   uint16_t i;
   
   gfx_putc(SERIAL_START_BYTE);
   
   cs = SerialSendByte(SerialCmd, cs);
   cs = SerialSendByte(make8(Count, 0), cs);
   cs = SerialSendByte(make8(Count, 1), cs);
   
   for(i=0;i<Count;i++)
      cs = SerialSendByte(Data[i], cs);
   
   gfx_putc(cs);
}
#endif
