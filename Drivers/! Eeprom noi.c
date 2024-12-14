
#include <internal_eeprom.c>
//======================================== 
int8 doc_rom(int8 dia_chi)
{
   return read_eeprom(dia_chi);
}
//======================================== 
void ghi_rom(int8 dia_chi, int8 du_lieu)
{
   write_eeprom(dia_chi, du_lieu);
}
//======================================== 
