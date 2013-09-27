#ifndef __IDA_NAME_REGISTER_H__
#define __IDA_NAME_REGISTER_H__

#include "ida_sdk.h"

typedef struct
{
  ea_t offset;
  char * name;
  char * comment;
} register_t;

typedef struct
{
  ea_t offset;
  char * name;
  char * comment;
} reg_struct_t;

//----------------------------------------------------------------------
//
// name register at regAddr with regName and regComm
//
void idaapi nameReg8 ( register_t * reg );
void idaapi nameReg16 ( register_t * reg );
void idaapi nameReg32 ( register_t * reg );

void idaapi nameRegStruct ( //
  ea_t regAddr, char * regStructName,  char * regStructComm , char * structName,
  asize_t structLength );

#endif /* __IDA_NAME_REGISTER_H__ */