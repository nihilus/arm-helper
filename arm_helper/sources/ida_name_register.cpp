#include "ida_name_register.h"

#define DUMMY_REG_ADDR      0x00000000
#define DUMMY_REG_NAME      "DUMMY_REG8"
#define DUMMY_REG_COMM      \
  "DUMMY_REG\n\n"           \
  "  B7 : Bit 7\n"          \
  "  B0 : Bit 0"

void idaapi nameReg8 ( ea_t regAddr, char * regName, char * regComm )
{
  do_unknown ( regAddr, DOUNK_EXPAND );
  do_data_ex ( regAddr, byteflag (), 1, BADNODE );
  set_name ( regAddr, regName );
  set_cmt ( regAddr, regComm, true );
}

void idaapi nameReg16 ( ea_t regAddr, char * regName, char * regComm )
{
  do_unknown ( regAddr, DOUNK_EXPAND );
  do_data_ex ( regAddr, wordflag (), 2, BADNODE );
  set_name ( regAddr, regName );
  set_cmt ( regAddr, regComm, true );
}

void idaapi nameReg32 ( ea_t regAddr, char * regName, char * regComm )
{
  do_unknown ( regAddr, DOUNK_EXPAND );
  do_data_ex ( regAddr, dwrdflag (), 4, BADNODE );
  set_name ( regAddr, regName );
  set_cmt ( regAddr, regComm, true );
}

void idaapi nameRegStruct ( //
  ea_t regAddr, char * regStructName,  char * regStructComm , char * structName,
  asize_t structLength )
{
  tid_t tid = get_struc_id ( structName );

  if ( tid == BADADDR )
    return;

  do_unknown ( regAddr, DOUNK_EXPAND );
  do_data_ex ( regAddr, struflag(), structLength, tid );

  if ( regStructName )
    set_name ( regAddr, regStructName );

  if ( regStructComm )
    set_cmt ( regAddr, regStructComm, true );
}
