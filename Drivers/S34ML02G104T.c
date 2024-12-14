///////////////////////////////////////////////////////////////////////////////
//                               S34ML02G104T.c                              //
//                                                                           //
// Driver for a S34ML02G104T Flash Memory.                                   //
//                                                                           //
// The driver will work on the PCD compiler.  It requires an entire 16 bit   //
// port to transfer data, and that port set to use fast_io.  This driver     //
// also uses the Ready/Busy pin of the flash, which requires either an       //
// external pullup resistor or the internal weak-pullup resistor to be       //
// by default the driver will enable the internal weak-pullup of the pin.    //
// define USE_EXTERNAL_BUSY_PULLUP to use an external pullup.                //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
//                                    API                                    //
///////////////////////////////////////////////////////////////////////////////
// Functions:                                                                //
//                                                                           //
//    flash_Init(void)                                                       //
//       - Initializes the flash.                                            //
//                                                                           //
//    flash_ReadID(FLASH_ID *ptr)                                            //
//       - Reads the ID of the flash.                                        //
//                                                                           //
//    flash_ReadData(FLASH_ADDR Addr, uint16_t *Data, uint32_t count)        //
//       - Reads count words starting at specified address from flash.  Page //
//         data from spare area are not read by this function.  To read data //
//         from the spare area see flash_ReadSpareData() or                  //
//         flash_ReadAllData() functions.                                    //
//                                                                           //
//    flash_WriteData(FLASH_ADDR Addr, uint16_t *Data, uint32_t count)       //
//       - Writes count words starting at specified address to flash.  Page  //
//         addresses from spare area are not written by this function.  To   //
//         write data to the spare area see flash_WriteSpareData() function. //
//                                                                           //
//    flash_EraseBlock(uint16_t Block, int1 Plane=0)                         //
//       - Erase the specified block on the flash.                           //
//                                                                           //
//    FLASH_ERR flash_ReadSpareData(FLASH_ADDR Addr, uint16_t *Data,         //
//                                  uint32_t count)                          //
//       - Reads count words starting at specified address from flash's      //
//         spare area.  Specified address must be in flash's spare area.     //
//                                                                           //
//    FLASH_ERR flash_WriteSpareData(FLASH_ADDR Addr, uint16_t *Data,        //
//                                   uint32_t count)                         //
//       - Write count words starting at specified address to flash's        //
//         spare area.  Specified address must be in flash's spare area.     //
//                                                                           //
//    FLASH_ERR flash_ReadAllData(FLASH_ADDR Addr, uint16_t *Data,           //
//                                uint32_t count)                            //
//       - Reads count words starting at specified address, this function    //
//         can read normal page addresses, spare page address or both.       //
//                                                                           //
//    flash_BulkErase(void)                                                  //
//       - Erase all blocks on flash.                                        //
//                                                                           //
//    flash_LatchCommand(uint16_t command)                                   //
//       - Latches a command into flash.  Commands are defined in            //
//         S34ML02G104T.h, search for Flash Command Defines.                 //
//                                                                           //
//    flash_LatchAddress8(uint8_t address)                                   //
//       - Latches a one byte address into flash.                            //
//                                                                           //
//    flash_LatchAddress(uint8_t *address, uint8_t count=5)                  //
//       - Latches address into flash, address can be from 1 to 5 bytes.     //
//                                                                           //
//    uint16_t flash_ReadWord(void)                                          //
//       - Toggles FLASH_RE pin to read a word from the flash.               //
//                                                                           //
//    flash_WriteWord(uint16_t data)                                         //
//       - Puts data on PIC's port and toggles FLASH_WE pin to write a word  //
//         to the flash.                                                     //
//                                                                           //
//    flash_GetAddressArray(FLASH_ADDR &Addr, uint8_t *ptr)                  //
//       - Takes passed FLASH_ADDR and converts it to 5 byte array that can  //
//         used with flash_LatchAddress() function.                          //
//                                                                           //
//    FLASH_ERR flash_IncAddressPage(FLASH_ADDR *Addr, uint16_t value)       //
//       - Increments the FLASH_ADDR page by the specified value.            //
//                                                                           //
//    FLASH_ERR flash_IncAddressPage(FLASH_ADDR *Addr)                       //
//       - Increments the FLASH_ADDR page by one.                            //
//                                                                           //
//    FLASH_ERR flash_DecAddressPage(FLASH_ADDR *Addr)                       //
//       - Decrements the FLASH_ADDR page by one.                            //
//                                                                           //
//    FLASH_ERR flash_IncAddressColumn(FLASH_ADDR *Addr, uint16_t value)     //
//       - Increments the FLASH_ADDR column by specified value.              //
//                                                                           //
//    FLASH_ERR flash_IncAddressColumn(FLASH_ADDR *Addr)                     //
//       - Increments the FLASH_ADDR column by one.                          //
//                                                                           //
//    FLASH_ERR flash_IncAddressBlock(FLASH_ADDR *Addr, uint16_t value)      //
//       - Increments the FLASH_ADDR block by specified value.               //
//                                                                           //
//    FLASH_ERR flash_IncAddressBlock(FLASH_ADDR *Addr)                      //
//       - Increments the FLASH_ADDR block by one.                           //
//                                                                           //
//    int1 flash_AddressGreater(FLASH_ADDR New, FLASH_ADDR Previous)         //
//       - Determines if the New FLASH_ADDR is greater then the Previous     //
//         FLASH_ADDR.                                                       //
//                                                                           //
//    int1 flash_IncAddress(FLASH_ADDR *Addr, uint16_t value)                //
//       - Increments the FLASH_ADDR by the uint16_t value.                  //
//                                                                           //
//    int1 flash_IncAddress(FLASH_ADDR *Addr, uint32_t value)                //
//       - Increments the FLASH_ADDR by the uint32_t value.                  //
//                                                                           //
//    int1 flash_IncAddress(FLASH_ADDR *Addr)                                //
//       - Increments the FLASH_ADDR by one.                                 //
//                                                                           //
//    FLASH_ERR flash_SetAddress(uint32_t wAddress, FLASH_ADDR *Address)     //
//       - Takes an uint32_t address and determine the FLASH_ADDR address.   //
//                                                                           //
// Defines:                                                                  //
//                                                                           //
//    FLASH_PAGE_WORDS                                                       //
//       - Sets the number of words, columns, per page.  Does not include    //
//         spare area page words.  Default is 1024.                          //
//                                                                           //
//    FLASH_SPARE_PAGE_WORDS                                                 //
//       - Sets the number of words in the spare area per page, default is   //
//         32.                                                               //
//                                                                           //
//    FLASH_BLOCK_PAGES                                                      //
//       - Sets the number of pages per block, default is 64.                //
//                                                                           //
//    FLASH_PLANE_BLOCKS                                                     //
//       - Sets the number of block per plane, default is 1024.              //
//                                                                           //
//    FLASH_PLANES                                                           //
//       - Set the number of planes flash has, default is 2.                 //
//                                                                           //
//    FLASH_ERASE_SIZE                                                       //
//       - Specifies the erase size in words, not including the spare area.  //
//         Determined from FLASH_PAGE_WORDS and FLASH_BLOCK_PAGES defines,   //
//         not a settable define.                                            //
//                                                                           //
//    FLASH_BLOCKS                                                           //
//       - Specifies the total number of erase block flash has.  Determined  //
//         from FLASH_PLANE_BLOCKS and FLASH_PLANES defines, not a settable  //
//         define.                                                           //
//                                                                           //
//    FLASH_CE                                                               //
//       - Sets the PIC pin that the Chip Enable pin of the flash is         //
//         connected to, default is PIN_B9.                                  //
//                                                                           //
//    FLASH_WE                                                               //
//       - Sets the PIC pin that the Write Enable pin of the flash is        //
//         connected to, default is PIN_B13.                                 //
//                                                                           //
//    FLASH_RE                                                               //
//       - Sets the PIC pin that the Read Enable pin of the flash is         //
//         connected to, default is PIN_B12.                                 //
//                                                                           //
//    FLASH_ALE                                                              //
//       - Sets the PIC pin that the Address Latch Enable pin of the flash   //
//         is connected to, default is PIN_B11.                              //
//                                                                           //
//    FLASH_CLE                                                              //
//       - Sets the PIC pin that the Command Latch Enable pin of the flash   //
//         is connected to, default is PIN_B14.                              //
//                                                                           //
//    FLASH_WP                                                               //
//       - Sets the PIC pin that the Write Protect pin of the flash is       //
//         connected to, default is PIN_B15.                                 //
//                                                                           //
//    FLASH_BUSY                                                             //
//       - Sets the PIC pin that the Ready Busy pin of the flash is          //
//         connected to, default is PIN_B10.                                 //
//                                                                           //
//    FLASH_DATA_PORT                                                        //
//       - Sets the PIC port that is used to write and read data to and from //
//         the flash, default is port d.                                     //
//                                                                           //
// Macros:                                                                   //
//                                                                           //
//    FlashWritePort(x)                                                      //
//       - Writes data to the PIC data port.                                 //
//                                                                           //
//    data = FlashReadPort()                                                 //
//       - Inputs data from the PIC data port.                               //
//                                                                           //
//    FlashPortInput()                                                       //
//       - Makes the PIC port pins data inputs.                              //
//                                                                           //
//    FlashPortOutput()                                                      //
//       - Makes the PIC port pins data outputs.                             //
//                                                                           //
//    FlashInitCE()                                                          //
//       - Initializes the FLASH_CD pin, called by flash_Init().             //
//                                                                           //
//    FlashInitWE()                                                          //
//       - Initializes the FLASH_WE pin, called by flash_Init().             //
//                                                                           //
//    FlashInitRE()                                                          //
//       - Initializes the FLASH_RE pin, called by flash_Init().             //
//                                                                           //
//    FlashInitALE()                                                         //
//       - Initializes the FLASH_ALE pin, called by flash_Init().            //
//                                                                           //
//    FlashInitCLE()                                                         //
//       - Initializes the FLASH_CLE pin, called by flash_Init().            //
//                                                                           //
//    FlashInitWP()                                                          //
//       - Initializes the FLASH_WP pin, called by flash_Init().             //
//                                                                           //
//    FlashInitBusy()                                                        //
//       - Initializes the FLASH_BUSY pin, called by flash_Init().           //
//                                                                           //
// Types:                                                                    //
//                                                                           //
//    FLASH_ERR:                                                             //
//       FLASH_ERR_OK          - No Error Occurred                           //
//       FLASH_ERR_INV_ADDR    - Invalid Address Passed to function          //
//       FLASH_ERR_READ_COUNT  - Read Count Overflowed Flash                 //
//       FLASH_ERR_WRITE_COUNT - Write Count Overflowed Flash                //
//       FLASH_ERR_FULL        - Flash Overflow Occurred                     //
//       FLASH_ERR_EMPTY       - Flash Underflow Occurred                    //
//                                                                           //
//    FLASH_ID: - structure                                                  //
//       uint8_t MC;  - Manufacturer Code                                    //
//       uint8_t DI;  - Device Identifier                                    //
//       uint8_t ICN; - Internal chip number, cell type, etc.                //
//       uint8_t PS;  - Page Size, Block Size, Spare Size, Organization      //
//       uint8_t MI;  - Multiplane information                               //
//                                                                           //
//    FLASH_ADDR: - structure                                                //
//       uint16_t Column;                                                    //
//       uint8_t  Page;                                                      //
//       int1     Plane;                                                     //
//       uint16_t Block;                                                     //
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

#include <S34ML02G104T.h>

///////////////////////////////////////////////////////////////////////////////
// flash_Init()
//
// Initializes the S34ML02G104T flash.
//
///////////////////////////////////////////////////////////////////////////////
void flash_Init(void)
{
   FlashInitWP();  
   FlashInitCE();
   FlashInitWE();  
   FlashInitRE();  
   FlashInitALE(); 
   FlashInitCLE();
   FlashInitBusy();
   
  #ifndef USE_EXTERNAL_BUSY_PULLUP
   set_pullup(TRUE, FLASH_BUSY);
  #endif
}

///////////////////////////////////////////////////////////////////////////////
// flash_ReadID()
//
// Reads the Flash ID of the S34ML02G104T.
//
// Parameter:
//    ptr - pointer to FLASH_ID struct to read the ID to.
//
///////////////////////////////////////////////////////////////////////////////
void flash_ReadID(FLASH_ID *ptr)
{
   output_low(FLASH_CE);
   
   flash_LatchCommand(FLASH_CMD_READ_ID);
   
   flash_LatchAddress8(0);
   
   FlashPortInput();
   
   while(!input(FLASH_BUSY));
   
   ptr->MC = flash_ReadWord();
   ptr->DI = flash_ReadWord();
   ptr->ICN = flash_ReadWord();
   ptr->PS = flash_ReadWord();
   ptr->MI = flash_ReadWord();
   
   output_high(FLASH_CE);
}

///////////////////////////////////////////////////////////////////////////////
// flash_ReadData()
//
// Reads data from the flash.  This function does not read any data from the 
// spare area, use flash_ReadSpareData() or flash_ReadAllData() functions to 
// read data from spare area.
//
// Parameters:
//    Addr - FLASH_ADDR address to start reading data from.
//
//    Data - pointer to uint16_t array to read data to.
//
//    count - the number of words to read from flash.
//
///////////////////////////////////////////////////////////////////////////////
void flash_ReadData(FLASH_ADDR Addr, uint16_t *Data, uint32_t count)
{
   uint32_t i;
   uint8_t address[5];
   
   flash_GetAddressArray(Addr, address);
   
   output_low(FLASH_CE);
   
   flash_LatchCommand(FLASH_CMD_READ);
   
   flash_LatchAddress(address);
   
   flash_LatchCommand(FLASH_CMD_READ_PAGE);

   while(!input(FLASH_BUSY));
  
   FlashPortInput();
   
   for(i=0;i<count;i++,Addr.Column++)
   {
      if(Addr.Column >= FLASH_PAGE_WORDS)
      {
         if(++Addr.Page >= FLASH_BLOCK_PAGES)
         {
            if(++Addr.Block >= FLASH_PLANE_BLOCKS)
            {
               Addr.Plane++;
               Addr.Block = 0;
            }
            
            Addr.Page = 0;
         }
         
         Addr.Column = 0;
         
         flash_GetAddressArray(Addr, address);
         
         flash_LatchCommand(FLASH_CMD_READ);
         
         flash_LatchAddress(address);
   
         flash_LatchCommand(FLASH_CMD_READ_PAGE);
         
         while(!input(FLASH_BUSY));
         
         FlashPortInput();
      }
      
      *Data++ = flash_ReadWord();
   }
   
   output_high(FLASH_CE);
}

///////////////////////////////////////////////////////////////////////////////
// flash_WriteData()
//
// Write data to the flash.  This function does not write any data to the spare 
// area, use flash_WriteSpareData() function to write to spare area.
//
// Parameter:
//    Addr - FLASH_ADDR address to start writing data to.
//
//    Data - pointer to array containing data to write to flash.
//
//    count - number of words to write to flash.
//
///////////////////////////////////////////////////////////////////////////////
void flash_WriteData(FLASH_ADDR Addr, uint16_t *Data, uint32_t count)
{
   uint32_t i;
   uint8_t address[5];
   
   flash_GetAddressArray(Addr, address);
   
   output_low(FLASH_CE);
   output_high(FLASH_WP);
   
   flash_LatchCommand(FLASH_CMD_PAGE_PROGRAM);
   
   flash_LatchAddress(address);
   
   FlashPortOutput();   
   
   for(i=0;i<count;i++,Addr.Column++)
   {
      if(Addr.Column >= FLASH_PAGE_WORDS)
      {
         flash_LatchCommand(FLASH_CMD_PAGE_PROGRAM_EXE);
         
         while(!input(FLASH_BUSY));
      
         if(++Addr.Page >= FLASH_BLOCK_PAGES)
         {
            if(++Addr.Block >= FLASH_PLANE_BLOCKS)
            {
               Addr.Plane++;
               Addr.Block = 0;
            }
            
            Addr.Page = 0;
         }
         
         Addr.Column = 0;
         
         flash_GetAddressArray(Addr, address);
         
         flash_LatchCommand(FLASH_CMD_PAGE_PROGRAM);
         
         flash_LatchAddress(address);
         
         FlashPortOutput();
      }
      
      flash_WriteWord(*Data++);      
   }
   
   flash_LatchCommand(FLASH_CMD_PAGE_PROGRAM_EXE);
         
   while(!input(FLASH_BUSY));
   
   output_low(FLASH_WP);
   output_high(FLASH_CE);
}

///////////////////////////////////////////////////////////////////////////////
// flash_EraseBlock()
//
// Erase a block on the flash.
//
// Parameters:
//    Block - Block to erase.
//
//    Plane - Plane of Block to erase.
//
///////////////////////////////////////////////////////////////////////////////
void flash_EraseBlock(uint16_t Block, int1 Plane=0)
{
   uint8_t address[3];
   
   address[0] = ((uint8_t)Plane << 6) | ((Block & 0x01) << 7);
   address[1] = (Block >> 1) & 0xFF;
   address[2] = (Block >> 8) & 0x01;

   output_low(FLASH_CE);
   output_high(FLASH_WP);
   
   flash_LatchCommand(FLASH_CMD_BLOCK_ERASE);
   
   flash_LatchAddress(address, 3);
   
   flash_LatchCommand(FLASH_CMD_BLOCK_ERASE_EXE);
   
   while(!input(FLASH_BUSY));
   
   output_low(FLASH_WP);
   output_high(FLASH_CE);
}

///////////////////////////////////////////////////////////////////////////////
// flash_ReadSpareData()
//
// Read data from spare area of flash.
//
// Parameters:
//    Addr - FLASH_ADDR address to start reading from, address must be in spare
//           area.
//
//    Data - pointer to uint16_t array to read data to.
//
//    count - number of words to read from spare area.
//
// Returns:
//    FLASH_ERR_OK - if operation successful.
//    Other - if operation was unsuccessful.
//
///////////////////////////////////////////////////////////////////////////////
FLASH_ERR flash_ReadSpareData(FLASH_ADDR Addr, uint16_t *Data, uint32_t count)
{
   uint8_t address[5];
   uint32_t i;

   if((Addr.Column < FLASH_PAGE_WORDS) || (Addr.Column >= (FLASH_PAGE_WORDS + FLASH_SPARE_PAGE_WORDS)))
      return(FLASH_ERR_INV_ADDR);
  
   flash_GetAddressArray(Addr, address);
  
   output_low(FLASH_CE);
   
   flash_LatchCommand(FLASH_CMD_READ);
   
   flash_LatchAddress(address);
   
   flash_LatchCommand(FLASH_CMD_READ_PAGE);
   
   while(!input(FLASH_BUSY));
   
   FlashPortInput();
   
   for(i=0;i<count;i++,Addr.Column++)
   {
      if(Addr.Column >= (FLASH_PAGE_WORDS + FLASH_SPARE_PAGE_WORDS))
      {
         if(++Addr.Page >= FLASH_BLOCK_PAGES)
         {
            if(++Addr.Block >= FLASH_PLANE_BLOCKS)
            {
               if(++Addr.Plane == 0)
                  return(FLASH_ERR_READ_COUNT);
                  
               Addr.Block = 0;
            }
            
            Addr.Page = 0;
         }
         
         Addr.Column = FLASH_PAGE_WORDS;
         
         flash_GetAddressArray(Addr, address);
         
         flash_LatchCommand(FLASH_CMD_READ);
         
         flash_LatchAddress(address);
   
         flash_LatchCommand(FLASH_CMD_READ_PAGE);
         
         while(!input(FLASH_BUSY));
         
         FlashPortInput();
      }

      *Data++ = flash_ReadWord();
   }
   
   output_high(FLASH_CE);
   
   return(FLASH_ERR_OK);
}

///////////////////////////////////////////////////////////////////////////////
// flash_WriteSpareData()
//
// Writes data to the spare area of flash.
//
// Parameters:
//    Addr - FLASH_ADDR address to start writing from, address must be in spare
//           area.
//
//    Data - pointer to uint16_t Array containing data to write to flash.
//
//    count - number of words to write to flash.
//
// Returns:
//    FLASH_ERR_OK - if operation successful.
//    Other - if operation was unsuccessful.
//
///////////////////////////////////////////////////////////////////////////////
FLASH_ERR flash_WriteSpareData(FLASH_ADDR Addr, uint16_t *Data, uint32_t count)
{
   uint32_t i;
   uint8_t address[5];
   
   if((Addr.Column < FLASH_PAGE_WORDS) || (Addr.Column >= (FLASH_PAGE_WORDS + FLASH_SPARE_PAGE_WORDS)))
      return(FLASH_ERR_INV_ADDR);
   
   flash_GetAddressArray(Addr, address);
   
   output_low(FLASH_CE);
   output_high(FLASH_WP);
   
   flash_LatchCommand(FLASH_CMD_PAGE_PROGRAM);
   
   flash_LatchAddress(address);
   
   FlashPortOutput();   
   
   for(i=0;i<count;i++,Addr.Column++)
   {
      if(Addr.Column >= (FLASH_PAGE_WORDS + FLASH_SPARE_PAGE_WORDS))
      {
         flash_LatchCommand(FLASH_CMD_PAGE_PROGRAM_EXE);
         
         while(!input(FLASH_BUSY));
      
         if(++Addr.Page >= FLASH_BLOCK_PAGES)
         {
            if(++Addr.Block >= FLASH_PLANE_BLOCKS)
            {
               if(++Addr.Plane == 0)
               {
                  output_low(FLASH_WP);
                  output_high(FLASH_CE);
               
                  return(FLASH_ERR_WRITE_COUNT);
               }
                  
               Addr.Block = 0;
            }
            
            Addr.Page = 0;
         }
         
         Addr.Column = FLASH_PAGE_WORDS;
         
         flash_GetAddressArray(Addr, address);
         
         flash_LatchCommand(FLASH_CMD_PAGE_PROGRAM);
         
         flash_LatchAddress(address);
         
         FlashPortOutput();
      }
      
      flash_WriteWord(*Data++);      
   }
   
   flash_LatchCommand(FLASH_CMD_PAGE_PROGRAM_EXE);
         
   while(!input(FLASH_BUSY));
   
   output_low(FLASH_WP);
   output_high(FLASH_CE);
   
   return(FLASH_ERR_OK);
}

///////////////////////////////////////////////////////////////////////////////
// flash_ReadAllData()
//
// Reads data from flash.  Can read normal area, spare area, or both from 
// flash.
//
// Parameters:
//    Addr - FLASH_ADDR address to start reading from.
//
//    Data - pointer to uint16_t array to read data to.
//
//    count - number of words to read from flash.
//
// Returns:
//    FLASH_ERR_OK - if operation successful.
//    Other - if operation was unsuccessful.  
//
///////////////////////////////////////////////////////////////////////////////
FLASH_ERR flash_ReadAllData(FLASH_ADDR Addr, uint16_t *Data, uint32_t count)
{
   uint8_t address[5];
   uint32_t i;

   if(Addr.Column >= (FLASH_PAGE_WORDS + FLASH_SPARE_PAGE_WORDS))
      return(FLASH_ERR_INV_ADDR);
   
   flash_GetAddressArray(Addr, address);
   
   output_low(FLASH_CE);
   
   flash_LatchCommand(FLASH_CMD_READ);
   
   flash_LatchAddress(address);
   
   flash_LatchCommand(FLASH_CMD_READ_PAGE);
   
   while(!input(FLASH_BUSY));
   
   FlashPortInput();
   
   for(i=0;i<count;i++,Addr.Column++)
   {
      if(Addr.Column >= (FLASH_PAGE_WORDS + FLASH_SPARE_PAGE_WORDS))
      {
         if(++Addr.Page >= FLASH_BLOCK_PAGES)
         {
            if(++Addr.Block >= FLASH_PLANE_BLOCKS)
            {
               if(++Addr.Plane == 0)
                  return(FLASH_ERR_READ_COUNT);
                  
               Addr.Block = 0;
            }
            
            Addr.Page = 0;
         }
         
         Addr.Column = 0;
         
         flash_GetAddressArray(Addr, address);
         
         flash_LatchCommand(FLASH_CMD_READ);
         
         flash_LatchAddress(address);
   
         flash_LatchCommand(FLASH_CMD_READ_PAGE);
         
         while(!input(FLASH_BUSY));
         
         FlashPortInput();
      }
      
      *Data++ = flash_ReadWord();
   }
   
   output_high(FLASH_CE);
   
   return(FLASH_ERR_OK);
}

///////////////////////////////////////////////////////////////////////////////
// flash_BulkErase()
//
// Erases entire flash.
//
///////////////////////////////////////////////////////////////////////////////
void flash_BulkErase(void)
{
   uint32_t i;
   uint32_t Count;
   int1 Plane = 0;
   uint16_t Block = 0;
   
   Count = FLASH_PLANE_BLOCKS * FLASH_PLANES;

   for(i=0;i<Count;i++)
   {
      flash_EraseBlock(Block, Plane);
      if(++Block >= FLASH_PLANE_BLOCKS)
      {
         Plane++;
         Block = 0;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
// flash_LatchCommand()
//
// Latches a command into flash.
//
// Parameters:
//    command - command to latch into flash.  Commands are defined in 
//              S34ML02G104T.h, search for Flash Command Defines.
//
///////////////////////////////////////////////////////////////////////////////
void flash_LatchCommand(uint16_t command)
{
   output_high(FLASH_CLE);
   
   FlashWritePort(command);
   FlashPortOutput();
   
   output_low(FLASH_WE);
   output_high(FLASH_WE);
   
   output_low(FLASH_CLE);
   
  #if getenv("ENH24")
   #if ((((getenv("CLOCK") / 2) / 10000000) - 2) > 1)
    delay_cycles(((getenv("CLOCK") / 2) / 10000000) - 2);
   #endif
  #else
   #if ((((getenv("CLOCK") / 2) / 10000000) - 1) > 1)
    delay_cycles(((getenv("CLOCK") / 2) / 10000000) - 1);
   #endif
  #endif
}

///////////////////////////////////////////////////////////////////////////////
// flash_LatchAddress8()
//
// Latches one byte address into flash.
//
// Parameters:
//    address - address to latch into flash.
//
///////////////////////////////////////////////////////////////////////////////
void flash_LatchAddress8(uint8_t address)
{
   output_high(FLASH_ALE);
   
   FlashWritePort(address);
   FlashPortOutput();
   
   output_low(FLASH_WE);
   output_high(FLASH_WE);
   
   output_low(FLASH_ALE);
}

///////////////////////////////////////////////////////////////////////////////
// flash_LatchAddress()
//
// Latch 1 to 5 byte address into flash.
//
// Parameters:
//    address - pointer to uint8_t array containing address to latch into 
//              flash.
//
//    count - number of bytes to latch into flash.  Flash uses one, three and 
//            five byte addresses.  Five is most common, recommend using 
//            flash_LatchAddress8() function for 1 byte addresses.
//
///////////////////////////////////////////////////////////////////////////////
void flash_LatchAddress(uint8_t *address, uint8_t count=5)
{
   uint8_t i;
   
   FlashPortOutput();
   
   for(i=0;i<count;i++)
   {
      output_high(FLASH_ALE);
      output_low(FLASH_WE);
      
      FlashWritePort(address[i]);
      
      output_high(FLASH_WE);      
      output_low(FLASH_ALE);
   }
}

///////////////////////////////////////////////////////////////////////////////
// flash_ReadWord()
// 
// Toggles FLASH_RE() pin to read data from flash.
//
// Returns:
//    uint16_t value read from flash.
//
///////////////////////////////////////////////////////////////////////////////
#inline 
uint16_t flash_ReadWord(void)
{
   uint16_t result; 

   output_low(FLASH_RE);
   output_high(FLASH_RE);   
   
   result = FlashReadPort();

   return(result);
}

///////////////////////////////////////////////////////////////////////////////
// flash_WriteWord()
//
// Puts data on PIC data port and toggles FLASH_WE pin to write data to flash.
//
// Parameters:
//    data - data to write to flash.
//
///////////////////////////////////////////////////////////////////////////////
#inline
void flash_WriteWord(uint16_t data)
{
   FlashWritePort(data);
   output_low(FLASH_WE);
   output_high(FLASH_WE);
}

///////////////////////////////////////////////////////////////////////////////
// flash_GetAddressArray()
//
// Takes FLASH_ADDR address and turns it into byte array that the 
// flash_LatchAddress() can use to latch address.
//
// Parameters:
//    Addr - FLASH_ADDR address to convert.
//
//    ptr - pointer to uint8_t array to copy converted address to.
//
///////////////////////////////////////////////////////////////////////////////
void flash_GetAddressArray(FLASH_ADDR &Addr, uint8_t *ptr)
{
   *ptr++ = make8(Addr.Column,0);
   *ptr++ = make8(Addr.Column,1);
   *ptr++ = (Addr.Page & 0x3F) | ((uint8_t)Addr.Plane << 6) | ((Addr.Block & 1) << 7);
   *ptr++ = (Addr.Block >> 1) & 0xFF;
   *ptr = (Addr.Block >> 9) & 1;
}

///////////////////////////////////////////////////////////////////////////////
// flash_IncAddressPage()
//
// Increments the FLASH_ADDR address by specified number of pages.  Function 
// expects and returns an address within the normal area of the flash.  
// Returned address will not be in the spare area of flash.
//
// Parameter:
//    Addr - pointer to FLASH_ADDR address to increment.
//
//    value - number of pages to increment address by.
//
// Returns:
//    FLASH_ERR_OK - if operation successful.
//    Other - if operation was unsuccessful. 
//
///////////////////////////////////////////////////////////////////////////////
FLASH_ERR flash_IncAddressPage(FLASH_ADDR *Addr, uint16_t value)
{
   Addr->Page += value;
   
   while(Addr->Page >= FLASH_BLOCK_PAGES)
   {      
      if(++Addr->Block >= FLASH_PLANE_BLOCKS)
      {
         if(++Addr->Plane == 0)
            return(FLASH_ERR_FULL);
         
         Addr->Block = 0;
      }
      
      Addr->Page -= FLASH_BLOCK_PAGES;
   }
   
   return(FLASH_ERR_OK);
}

///////////////////////////////////////////////////////////////////////////////
// flash_IncAddressPage()
//
// Increments the FLASH_ADDR address by one page.  Function expects and 
// returns an address within the normal area of the flash.  Returned address 
// will not be in the spare area of flash.
//
// Parameters:
//    Addr - pointer to FLASH_ADDR address to increment.
//
// Returns:
//    FLASH_ERR_OK - if operation successful.
//    Other - if operation was unsuccessful.
//
///////////////////////////////////////////////////////////////////////////////
FLASH_ERR flash_IncAddressPage(FLASH_ADDR *Addr)
{
   return(flash_IncAddressPage(Addr, 1));
}

///////////////////////////////////////////////////////////////////////////////
// flash_DecAddressPage()
//
// Decrements the FLASH_ADDR address by one page.  Function expects and returns
// an address within the normal area of the flash.  Returned address will not
// be in the spare area of flash.
//
// Parameters:
//    Addr - pointer to FLASH_ADDR address to decrement.
//
// Returns:
//    FLASH_ERR_OK - if operation successful.
//    Other - if operation was unsuccessful.
//
///////////////////////////////////////////////////////////////////////////////
FLASH_ERR flash_DecAddressPage(FLASH_ADDR *Addr)
{
   if(--Addr->Page >= FLASH_BLOCK_PAGES)
   {
      if(--Addr->Block >= FLASH_PLANE_BLOCKS)
      {
         if(--Addr->Plane == 1)
            return(FLASH_ERR_EMPTY);
         
         Addr->Block = FLASH_PLANE_BLOCKS - 1;
      }
      
      Addr->Page = FLASH_BLOCK_PAGES - 1;
   }
   
   return(FLASH_ERR_OK);
}

///////////////////////////////////////////////////////////////////////////////
// flash_IncAddressColumn()
//
// Increments the FLASH_ADDR address by specified number of columns.  Function 
// expects and returns an address within the normal area of the flash.  
// Returned address will not be in the spare area of flash.
//
// Parameters:
//    Addr - pointer to FLASH_ADDR address to increment.
//
//    value - number of columns to increment address by.
//
// Returns:
//    FLASH_ERR_OK - if operation successful.
//    Other - if operation was unsuccessful.
//
///////////////////////////////////////////////////////////////////////////////
FLASH_ERR flash_IncAddressColumn(FLASH_ADDR *Addr, uint16_t value)
{
   Addr->Column += value;
   
   while(Addr->Column >= FLASH_PAGE_WORDS)
   {
      if(++Addr->Page >= FLASH_BLOCK_PAGES)
      {
         if(++Addr->Block >= FLASH_PLANE_BLOCKS)
         {
            if(++Addr->Plane == 0)
               return(FLASH_ERR_FULL);
            
            Addr->Block = 0;
         }
         
         Addr->Page = 0;
      }
      
      Addr->Column -= FLASH_PAGE_WORDS;
   }
   
   return(FLASH_ERR_OK);
}

///////////////////////////////////////////////////////////////////////////////
// flash_IncAddressColumn()
//
// Increments the FLASH_ADDR address by one column.  Function expects and
// returns an address within the normal area of the flash.  Returned address
// will not be in the spare area of flash.
//
// Parameters:
//    Addr - pointer to FLASH_ADDR address to increment.
//
// Returns:
//    FLASH_ERR_OK - if operation successful.
//    Other - if operation was unsuccessful.
//
///////////////////////////////////////////////////////////////////////////////
FLASH_ERR flash_IncAddressColumn(FLASH_ADDR *Addr)
{
   return(flash_IncAddressColumn(Addr, 1));
}

///////////////////////////////////////////////////////////////////////////////
// flash_IncAddressBlock()
//
// Increments the FLASH_ADDR address by specified number of blocks.  Function 
// expects and returns an address within the normal area of the flash.  
// Returned address will not be in the spare area of flash.
//
// Parameters:
//    Addr - pointer to FLASH_ADDR address to increment.
//
//    value - number of blocks to increment address by.
//
// Returns:
//    FLASH_ERR_OK - if operation successful.
//    Other - if operation was unsuccessful.
//
///////////////////////////////////////////////////////////////////////////////
FLASH_ERR flash_IncAddressBlock(FLASH_ADDR *Addr, uint16_t value)
{
   Addr->Block += value;
   
   while(Addr->Block >= FLASH_PLANE_BLOCKS)
   {
      if(++Addr->Plane == 0)
         return(FLASH_ERR_FULL);
      
      Addr->Block -= FLASH_PLANE_BLOCKS;
   }
   
   return(FLASH_ERR_OK);
}

///////////////////////////////////////////////////////////////////////////////
// flash_IncAddressBlock()
//
// Increments the FLASH_ADDR address by one block.  Function expects and 
// returns an address within the normal area of the flash.  Returned address 
// will not be in the spare area of flash.
//
// Parameters:
//    Addr - pointer of FLASH_ADDR address to increment.
//
// Returns:
//    FLASH_ERR_OK - if operation successful.
//    Other - if operation was unsuccessful.
//
///////////////////////////////////////////////////////////////////////////////
FLASH_ERR flash_IncAddressBlock(FLASH_ADDR *Addr)
{
   return(flash_IncAddressBlock(Addr, 1));
}

///////////////////////////////////////////////////////////////////////////////
// flash_AddressGreater()
//
// Determines if New FLASH_ADDR address is greater then Previous FLASH_ADDR 
// address.  Function expects addresses to be within the normal area of the
// flash.
//
// Parameters:
//    New - FLASH_ADDR address to see if it is greater.
//
//    Previous - FLASH_ADDR address to compare other address to.
//
// Returns:
//    TRUE if New is greater then Previous.
//    FALSE if New is less then Previous.
//
///////////////////////////////////////////////////////////////////////////////
int1 flash_AddressGreater(FLASH_ADDR New, FLASH_ADDR Previous)
{
   uint32_t NewAddress, PreviousAddress;
   
   NewAddress = ((uint32_t)New.Plane * FLASH_PLANE_BLOCKS * FLASH_BLOCK_PAGES * FLASH_PAGE_WORDS) + 
                ((uint32_t)New.Block * FLASH_BLOCK_PAGES * FLASH_PAGE_WORDS) + 
                ((uint32_t)New.Page * FLASH_PAGE_WORDS) + New.Column;
   
   PreviousAddress = ((uint32_t)Previous.Plane * FLASH_PLANE_BLOCKS * FLASH_BLOCK_PAGES * FLASH_PAGE_WORDS) + 
                     ((uint32_t)Previous.Block * FLASH_BLOCK_PAGES * FLASH_PAGE_WORDS) + 
                     ((uint32_t)Previous.Page * FLASH_PAGE_WORDS) + Previous.Column;
   
   if(NewAddress >= PreviousAddress)
      return(TRUE);
   else
      return(FALSE);
}

///////////////////////////////////////////////////////////////////////////////
// flash_IncAddress()
//
// Increments the FLASH_ADDR address by specified number of words.  Functions 
// is basically the same as flash_IncAddressColumn() with a different return 
// value.  Function expects and returns an address within the normal area of
// the flash.  Returned address will not be in the spare area of flash.
//
// Parameters:
//    Addr - pointer to the FLASH_ADDR address to increment.
//
//    value - the number of words to increase the address by.
//
// Returns:
//    TRUE - if operation was successful.
//    FALSE - if operation was unsuccessful.
//
///////////////////////////////////////////////////////////////////////////////
int1 flash_IncAddress(FLASH_ADDR *Addr, uint16_t value)
{
   Addr->Column += value;
   
   while(Addr->Column >= FLASH_PAGE_WORDS)
   {
      if(++Addr->Page >= FLASH_BLOCK_PAGES)
      {
         if(++Addr->Block >= FLASH_PLANE_BLOCKS)
         {
            if(++Addr->Plane == 0)
               return(FALSE);
            
            Addr->Block = 0;
         }
         
         Addr->Page = 0;
      }
      
      Addr->Column -= FLASH_PAGE_WORDS;
   }
   
   return(TRUE);
}

///////////////////////////////////////////////////////////////////////////////
// flash_IncAddress()
//
// Increments the FLASH_ADDR address by specified number of words.  Functions 
// is basically the same as flash_IncAddressColumn() with a different return 
// value.  Function expects and returns an address within the normal area of
// the flash.  Returned address will not be in the spare area of flash.
//
// Parameters:
//    Addr - pointer to the FLASH_ADDR address to increment.
//
//    value - the number of words to increase the address by.
//
// Returns:
//    TRUE - if operation was successful.
//    FALSE - if operation was unsuccessful.
//
///////////////////////////////////////////////////////////////////////////////
int1 flash_IncAddress(FLASH_ADDR *Addr, uint32_t value)
{
   if(value >= FLASH_PAGE_WORDS)
   {
      if(flash_IncAddressPage(Addr, value / FLASH_PAGE_WORDS) != FLASH_ERR_OK)
         return(FALSE);
      
      while(value >= FLASH_PAGE_WORDS)
         value -= FLASH_PAGE_WORDS;
   }
   
   if(value != 0)
   {
      if(flash_IncAddressColumn(Addr, value) != FLASH_ERR_OK)
         return(FALSE);
   }
   
   return(TRUE);
}

///////////////////////////////////////////////////////////////////////////////
// flash_IncAddress()
//
// Increments the FLASH_ADDR address by one words.  Functions is basically the
// same as flash_IncAddressColumn() with a different return value.  Function 
// expects and returns an address within the normal area of the flash.
// Returned address will not be in the spare area of flash.
//
// Parameters:
//    Addr - pointer to the FLASH_ADDR address to increment.
//
//    value - the number of words to increase the address by.
//
// Returns:
//    TRUE - if operation was successful.
//    FALSE - if operation was unsuccessful.
//
///////////////////////////////////////////////////////////////////////////////
int1 flash_IncAddress(FLASH_ADDR *Addr)
{
   return(flash_IncAddress(Addr, 1));
}

///////////////////////////////////////////////////////////////////////////////
// flash_EraseBlocks()
//
// Erase specified number of blocks starting with the block of FLASH_ADDR 
// address.
//
// Parameters:
//    Address - the block of FLASH_ADDR to start erasing.
//
//    Count - number of blocks to erase.
//
///////////////////////////////////////////////////////////////////////////////
void flash_EraseBlocks(FLASH_ADDR Address, uint16_t Count)
{
   uint16_t i;
   
   for(i=0;i<Count;i++)
   {
      flash_EraseBlock(Address.Block, Address.Plane);
      flash_IncAddressBlock(&Address);
   }
}

///////////////////////////////////////////////////////////////////////////////
// flash_SetAddress()
//
// Takes the uint32_t address and sets it to an equivalent FLASH_ADDR address.
//
// Parameters:
//    wAddress - uint32_t address to convert to the FLASH_ADDR address.
//
//    address - pointer to FLASH_ADDR address to return the converted address
//              to.
//
// Returns:
//    FLASH_ERR_OK - if operation successful.
//    Other - if operation was unsuccessful.
//
///////////////////////////////////////////////////////////////////////////////
FLASH_ERR flash_SetAddress(uint32_t wAddress, FLASH_ADDR *Address)
{
   int1 Plane;
   
   Address->Column = 0;
   Address->Page = 0;
   Address->Block = 0;
   Address->Plane = 0;
   Plane = 0;
   
   while(wAddress >= ((uint32_t)FLASH_PLANE_BLOCKS * (uint32_t)FLASH_BLOCK_PAGES * (uint32_t)FLASH_PAGE_WORDS))
   {
      if(++Plane == 0)
         return(FLASH_ERR_INV_ADDR);

      Address->Plane = Plane;
      
      wAddress -= ((uint32_t)FLASH_PLANE_BLOCKS * (uint32_t)FLASH_BLOCK_PAGES * (uint32_t)FLASH_PAGE_WORDS);
   }
   
   if(wAddress >= ((uint32_t)FLASH_BLOCK_PAGES * (uint32_t)FLASH_PAGE_WORDS))
   {
      if(flash_IncAddressBlock(Address, wAddress / ((uint32_t)FLASH_BLOCK_PAGES * (uint32_t)FLASH_PAGE_WORDS)) != FLASH_ERR_OK)
         return(FLASH_ERR_INV_ADDR);
      
      while(wAddress >= ((uint32_t)FLASH_BLOCK_PAGES * (uint32_t)FLASH_PAGE_WORDS))
         wAddress -= ((uint32_t)FLASH_BLOCK_PAGES * (uint32_t)FLASH_PAGE_WORDS);
   }
   
   if(wAddress >= (FLASH_PAGE_WORDS))
   {
      if(flash_IncAddressPage(Address, wAddress / FLASH_PAGE_WORDS) != FLASH_ERR_OK)
         return(FLASH_ERR_INV_ADDR);
      
      while(wAddress >= FLASH_PAGE_WORDS)
         wAddress -= FLASH_PAGE_WORDS;
         
   }
   
   Address->Column = wAddress;
    
   return(FLASH_ERR_OK); 
}
