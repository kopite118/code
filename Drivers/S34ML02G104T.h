///////////////////////////////////////////////////////////////////////////////
//                               S34ML02G104T.h                              //
//                                                                           //
// Header file for a S34ML02G104T Flash Memory driver.  See S34ML02G104T.c   //
// for API and function descriptions.                                        //
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

#ifndef _S34ML02G104T_H_
#define _S34ML02G104T_H_

#include <stdint.h>

#ifndef concat
 #define concat(x,y)  x####y
#endif

#ifndef FLASH_PAGE_WORDS
 #define FLASH_PAGE_WORDS        1024
#endif

#ifndef FLASH_SPARE_PAGE_WORDS
 #define FLASH_SPARE_PAGE_WORDS  32
#endif

#ifndef FLASH_BLOCK_PAGES
 #define FLASH_BLOCK_PAGES       64
#endif

#ifndef FLASH_PLANE_BLOCKS
 #define FLASH_PLANE_BLOCKS      1024
#endif

#ifndef FLASH_PLANES
 #define FLASH_PLANES            2
#endif

#define FLASH_ERASE_SIZE         (FLASH_BLOCK_PAGES * FLASH_PAGE_WORDS)
#define FLASH_BLOCKS             (FLASH_PLANE_BLOCKS * FLASH_PLANES)

#ifndef FLASH_CE
 #define FLASH_CE         PIN_B9
#endif

#ifndef FLASH_WE
 #define FLASH_WE         PIN_B13
#endif

#ifndef FLASH_RE
 #define FLASH_RE         PIN_B12
#endif

#ifndef FLASH_ALE
 #define FLASH_ALE        PIN_B11
#endif

#ifndef FLASH_CLE
 #define FLASH_CLE        PIN_B14
#endif

#ifndef FLASH_WP
 #define FLASH_WP         PIN_B15
#endif

#ifndef FLASH_BUSY
 #define FLASH_BUSY       PIN_B10
#endif

#ifndef FLASH_DATA_PORT
 #define FLASH_DATA_PORT  d
#endif

// Macros
#define FlashWritePort(x)  concat(output_,FLASH_DATA_PORT(x))
#define FlashReadPort()    concat(input_,FLASH_DATA_PORT())
#define FlashPortInput()   concat(set_tris_,FLASH_DATA_PORT(0xFFFF))
#define FlashPortOutput()  concat(set_tris_,FLASH_DATA_PORT(0))

#define FlashInitCE()      {output_high(FLASH_CE); output_drive(FLASH_CE);}
#define FlashInitWE()      {output_high(FLASH_WE); output_drive(FLASH_WE);}
#define FlashInitRE()      {output_high(FLASH_RE); output_drive(FLASH_RE);}
#define FlashInitALE()     {output_low(FLASH_ALE); output_drive(FLASH_ALE);}
#define FlashInitCLE()     {output_low(FLASH_CLE); output_drive(FLASH_CLE);}
#define FlashInitWP()      {output_low(FLASH_WP); output_drive(FLASH_WP);}
#define FlashInitBusy()    output_float(FLASH_BUSY)

// Flash Command Defines
#define FLASH_CMD_READ              0x00
#define FLASH_CMD_READ_PAGE         0x30
#define FLASH_CMD_READ_COLUMN       0x05
#define FLASH_CMD_READ_COLUMN_SET   0xE0
#define FLASH_CMD_PAGE_PROGRAM      0x80
#define FLASH_CMD_PAGE_PROGRAM_EXE  0x10
#define FLASH_CMD_BLOCK_ERASE       0x60
#define FLASH_CMD_BLOCK_ERASE_EXE   0xD0 
#define FLASH_CMD_READ_ID           0x90
#define FLASH_CMD_READ_CACHE        0x31
#define FLASH_CMD_CATCH_END         0x3F

// Types
typedef enum {
   FLASH_ERR_OK=0,         //No Error Occurred
   FLASH_ERR_INV_ADDR,     //Invalid Address Passed to function
   FLASH_ERR_READ_COUNT,   //Read Count Overflowed Flash
   FLASH_ERR_WRITE_COUNT,  //Write Count Overflowed Flash
   FLASH_ERR_FULL,         //Flash Overflow Occurred
   FLASH_ERR_EMPTY         //Flash Underflow Occurred
} FLASH_ERR;

typedef struct
{
   uint8_t MC;    //Manufacturer Code
   uint8_t DI;    //Device Identifier
   uint8_t ICN;   //Internal chip number, cell type, etc.
   uint8_t PS;    //Page Size, Block Size, Spare Size, Organization
   uint8_t MI;    //Multiplane information
} FLASH_ID;

typedef struct
{
   uint16_t Column;
   uint8_t  Page;
   int1     Plane;
   uint16_t Block;      
} FLASH_ADDR;

// Prototypes
void flash_Init(void);
void flash_ReadID(FLASH_ID *ptr);
void flash_ReadData(FLASH_ADDR Addr, uint16_t *Data, uint32_t count);
void flash_WriteData(FLASH_ADDR Addr, uint16_t *Data, uint32_t count);
FLASH_ERR flash_ReadSpareData(FLASH_ADDR Addr, uint16_t *Data, uint32_t count);
FLASH_ERR flash_WriteSpareData(FLASH_ADDR Addr, uint16_t *Data, uint32_t count);
FLASH_ERR flash_ReadAllData(FLASH_ADDR Addr, uint16_t *Data, uint32_t count);
void flash_BulkErase(void);

void flash_LatchCommand(uint16_t command);
void flash_LatchAddress8(uint8_t address);
void flash_LatchAddress(uint8_t *address, uint8_t count=5);
void flash_EraseBlock(uint16_t Block, int1 Plane=0);

#inline 
uint16_t flash_ReadWord(void);
#inline
void flash_WriteWord(uint16_t data);
void flash_GetAddressArray(FLASH_ADDR &addr, uint8_t *ptr);
FLASH_ERR flash_IncPage(FLASH_ADDR *Addr, uint16_t value);
FLASH_ERR flash_IncPage(FLASH_ADDR *Addr);
FLASH_ERR flash_DecPage(FLASH_ADDR *Addr);
FLASH_ERR flash_IncAddressColumn(FLASH_ADDR *Addr, uint16_t value);
FLASH_ERR flash_IncAddressColumn(FLASH_ADDR *Addr);
FLASH_ERR flash_IncAddressBlock(FLASH_ADDR *Addr, uint16_t value);
FLASH_ERR flash_IncAddressBlock(FLASH_ADDR *Addr);
int1 flash_AddressGreater(FLASH_ADDR New, FLASH_ADDR Previous);

int1 flash_IncAddress(FLASH_ADDR *Addr, uint16_t value);
int1 flash_IncAddress(FLASH_ADDR *Addr, uint32_t value);
int1 flash_IncAddress(FLASH_ADDR *Addr);
void flash_EraseBlocks(FLASH_ADDR Address, uint16_t Count);
FLASH_ERR flash_SetAddress(uint32_t wAddress, FLASH_ADDR *Address);

#endif
