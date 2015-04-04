#pragma once

#include "ida_sdk.h"
#include "ida_name_vector.h"

#define FUNC_NAME_VECTORS       (1<<0)
#define FUNC_NAME_REGISTERS     (1<<1)
#define FUNC_DETECT_CODE        (1<<2)

#define FUNC_ALL                \
  ( FUNC_NAME_VECTORS           \
    | FUNC_NAME_REGISTERS       \
    | FUNC_DETECT_CODE )


class ida_cortex_m3
{
  public:
    ushort checkMask;
    ea_t vector_table;
    ea_t sram_base;
    asize_t sram_size;

    ida_cortex_m3 ( void );
    ~ida_cortex_m3 ( void );

    int chooseAction ( void );
    char * getNameSuffix ( char * name );

    void addSegments ( void );
    void addSramSegment ( void );
    void addPeriphSegment ( char * name, ea_t startEA, ea_t endEA );

    void detectCode ( void );
    void nameRegisters ( void );
    void nameVectors ( vector_t * vectors, ea_t vector_base, asize_t size );

    virtual void run ( void );
    virtual int init ( void );
};

