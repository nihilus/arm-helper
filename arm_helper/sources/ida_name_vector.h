#ifndef __IDA_NAME_VECTOR_H__
#define __IDA_NAME_VECTOR_H__

#include "ida_sdk.h"

typedef struct
{
  char * name;
  char * handler;
  char * comment;
} vector_t;

void idaapi nameVector ( vector_t * vect, ea_t entry, char * suffix );

#endif /* __IDA_NAME_VECTOR_H__ */