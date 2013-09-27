#include "ida_name_vector.h"
#include "ida_name_register.h"
#include "ida_cortex_m3.h"
#include "ida_cortex_m3_struct.h"


#define FUNCTION_COUNT 3

static const char functions[FUNCTION_COUNT][15] =
{
  "Name Vectors  ",
  "Name Registers",
  "Detect Code   "
};

static const char strStartItem[] = "STARTITEM 0\n";
static const char strSelectTitle[] =  "Choose action\n\n";
static const char strVectorTable[] =  "<Vector Table(0x):M:8:8::>\n";
static const char strSramBase[]    =  "<SRAM Base Addr(0x):M:8:8::>\n";
static const char strSramSize[]    =  "<SRAM Base Size(0x):M:8:8::>\n";

int ida_cortex_m3::chooseAction ( void )
{
  char dialog_form[MAXSTR * 8];
  char * dest = dialog_form;
  vector_table = inf.minEA;
  checkMask = FUNC_ALL;
  int ret;
  // where number is a number of input field the cursor will stand on (0 based)
  dest = qstpncpy ( dest, strStartItem, sizeof ( strStartItem ) );
  // Title : Next there must be the title line and 2 empty lines
  dest = qstpncpy ( dest, strSelectTitle, sizeof ( strSelectTitle ) );

  // All input field types (except B and K) are valid format specifiers
  // Input fields are represented by the following combination
  //
  // < # hint_message # label : type : width : swidth : @hlp[] >
  //
  for ( int i = 0; i < FUNCTION_COUNT; i++ )
  {
    dest = qstpncpy ( dest, "<", 2 );
    dest = qstpncpy ( dest, &functions[i][0], sizeof ( functions[i] ) );
    dest = qstpncpy ( dest, ":C", 3 );

    if ( i == FUNCTION_COUNT - 1 )
      dest = qstpncpy ( dest, ">", 2 );

    dest = qstpncpy ( dest, ">\n", 3 );
  }

  dest = qstpncpy ( dest, strVectorTable, sizeof ( strVectorTable ) );
  dest = qstpncpy ( dest, strSramBase, sizeof ( strSramBase ) );
  dest = qstpncpy ( dest, strSramSize, sizeof ( strSramSize ) );

  while ( 1 )
  {
    ret = AskUsingForm_c ( dialog_form, &checkMask, &vector_table, &sram_base,
                           &sram_size );

    if ( ret == 0 )
      return ret;

    if ( checkMask & FUNC_NAME_VECTORS )
    {
      if ( ( vector_table & 0xFF ) || ( isEnabled ( vector_table ) == false ) )
      {
        info ( "vector table address is wrong!\n" );
        continue;
      }
    }

    return ret;
  }
}

/* Vector table definition                                   */
/*  __cs3_stack                 Top of Stack                 */
/*  Reset_Handler               Reset Handler                */
/*  NMI_Handler                 NMI Handler                  */
/*  HardFault_Handler           Hard Fault Handler           */
/*  MemManage_Handler           MPU Fault Handler            */
/*  BusFault_Handler            Bus Fault Handler            */
/*  UsageFault_Handler          Usage Fault Handler          */
/*  0                           Reserved                     */
/*  0                           Reserved                     */
/*  0                           Reserved                     */
/*  0                           Reserved                     */
/*  SVC_Handler                 SVCall Handler               */
/*  DebugMon_Handler            Debug Monitor Handler        */
/*  0                           Reserved                     */
/*  PendSV_Handler              PendSV Handler               */
/*  SysTick_Handler             SysTick Handler              */
#define CM3_IRQ_COUNT           (16)
static vector_t vectors[CM3_IRQ_COUNT] =
{
  {"TopOfStack",  "ToS", "Top Of Stack" },
  {"Reset_vect",  "Reset_Handler", "Reset Handler" },
  {"NMI_vect",  "NMI_Handler", "NMI Handler" },
  {"HardFault_vect",  "HardFault_Handler", "HardFault Handler" },
  {"MemManage_vect",  "MemManage_Handler", "MemManage Handler" },
  {"BusFault_vect",  "BusFault_Handler", "BusFault Handler" },
  {"UsageFault_vect",  "UsageFault_Handler", "UsageFault Handler" },
  {"Reserved0",  NULL, "Reserved" },
  {"Reserved1",  NULL, "Reserved" },
  {"Reserved2",  NULL, "Reserved" },
  {"Reserved3",  NULL, "Reserved" },
  {"SVC_vect",  "SVC_Handler", "SVC Handler" },
  {"DebugMon_vect",  "DebugMon_Handler", "DebugMon Handler" },
  {"Reserved4",  NULL, "Reserved" },
  {"PendSV_vect",  "PendSV_Handler", "PendSV Handler" },
  {"SysTick_vect",  "SysTick_Handler", "SysTick Handler" }
};

char * ida_cortex_m3::getNameSuffix ( char * name )
{
  char * suffix = NULL;
  char fullName[MAXSTR];
  char * pSuffix = qstpncpy ( fullName, name, strlen ( name ) + 1 );
  ea_t ea;

  do
  {
    suffix = askstr ( 0, "_0", "Please input suffix (Reset_Handler_0)" );

    if ( suffix == NULL ) // cancel
      break;

    qstpncpy ( pSuffix, suffix, strlen ( suffix ) + 1 );
    ea = get_name_ea ( BADADDR, fullName );
  }
  while ( ea != BADADDR );

  return suffix;
}

void ida_cortex_m3::addSramSegment ( void )
{
  segment_t seg;

  if ( !isEnabled ( get_long ( vector_table ) ) )
  {
    //
    // Define its start and ending virtual address.
    //
    seg.startEA = sram_base;
    seg.endEA = sram_base + sram_size;
    //
    // Assign it a unique selector value.
    //
    seg.sel = allocate_selector ( 0 );
    //
    // Let the kernel know that it is a DATA segment.
    //
    // seg.type = SEG_DATA;
    seg.flags = 0;
    seg.bitness = 1; //32 bits
    seg.comb = scPub;
    seg.align = saRelDble;
    seg.perm = SEGPERM_EXEC | SEGPERM_WRITE | SEGPERM_READ;
    //
    // Create the segment in the address space.
    //
    add_segm_ex ( &seg, "SRAM", NULL, ADDSEG_NOSREG /* | ADDSEG_QUIET */ );
    set_segm_class ( &seg, "DATA" );
  }
  else
  {
    set_segm_start ( get_long ( vector_table ), sram_base,
                     SEGMOD_KEEP /* | SEGMOD_SILENT */);
    set_segm_end ( get_long ( vector_table ), sram_base + sram_size,
                   SEGMOD_KEEP /* | SEGMOD_SILENT */);
  }
}

void ida_cortex_m3::addPeriphSegment ( char * name, ea_t startEA, ea_t endEA )
{
  segment_t seg;

  if ( isEnabled ( startEA ) )
    return;

  //
  // Define its start and ending virtual address.
  //
  seg.startEA = startEA;
  seg.endEA = endEA;
  //
  // Assign it a unique selector value.
  //
  seg.sel = allocate_selector ( 0 ); // segbase
  //
  // Let the kernel know that it is a DATA segment.
  //
  // seg.type = SEG_DATA;
  seg.flags = 0;
  seg.bitness = 1; //32 bits
  seg.comb = scPub;
  seg.align = saRelDble;
  seg.perm = SEGPERM_WRITE | SEGPERM_READ;
  //
  // Create the segment in the address space.
  //
  add_segm_ex ( &seg, name, NULL, ADDSEG_NOSREG /* | ADDSEG_QUIET */ );
  set_segm_class ( &seg, "DATA" );
}

/* Memory mapping of Cortex-M3 Hardware */
#define ITM_BASE            (0xE0000000UL)            /* ITM Base Address                   */
#define DWT_BASE            (0xE0001000UL)            /* DWT Base Address                   */
#define SCS_BASE            (0xE000E000UL)            /* System Control Space Base Address  */
#define SysTick_BASE        (0xE000E010UL)            /* SysTick Base Address               */
#define NVIC_BASE           (0xE000E100UL)            /* NVIC Base Address                  */
#define SCB_BASE            (0xE000ED00UL)            /* System Control Block Base Address  */
#define MPU_BASE            (0xE000ED90UL)            /* Memory Protection Unit             */
#define CoreDebug_BASE      (0xE000EDF0UL)            /* Core Debug Base Address            */
#define TPIU_BASE           (0xE0040000UL)            /* TPI Base Address                   */

void ida_cortex_m3::addSegments ( void )
{
  addSramSegment ();
  //Private Peripheral Bus : Internal
  addPeriphSegment ( "ITM", 0xE0000000, 0xE0001000 );
  addPeriphSegment ( "DWT", 0xE0001000, 0xE0002000 );
  addPeriphSegment ( "FPB", 0xE0002000, 0xE0003000 );
  addPeriphSegment ( "SCS", 0xE000E000, 0xE000F000 );
  //Private Peripheral Bus : Debug/External
  addPeriphSegment ( "TPIU", 0xE0040000, 0xE0041000 );
  addPeriphSegment ( "ETM", 0xE0041000, 0xE0042000 );
  //addPeriphSegment (  "EXT_PERIPH", 0xE0042000, 0xE00FE000 );
  //addPeriphSegment (  "ROM_TABLE", 0xE00FE000, 0xE00FF000 );
  //addPeriphSegment (  "VENDOR", 0xE0100000, 0xFFFFFFFC );
}

void ida_cortex_m3::nameVectors ( vector_t * vectors, ea_t vector_base,
                                  asize_t size )
{
  msg ( "ida_cortex_m3::nameVectors\n" );

  char * suffix = NULL;

  if ( get_name_ea ( BADADDR, vectors[0].name ) != BADADDR )
  {
    suffix = getNameSuffix ( vectors[0].name );

    if ( suffix == NULL ) // cancel
      return;
  }

  for ( uint32 i = 0; i < size; i++ )
    nameVector ( &vectors[i], vector_base + i * 4, suffix );
}


void ida_cortex_m3::nameRegisters ( void )
{
  msg ( "ida_cortex_m3::nameRegisters\n" );
  struc_t * sptr;

  if ( sptr = cm3Struct_SysTick_Type ( "SysTick_Type" ) )
  {
    nameRegStruct ( 0xE000E010UL, "SysTick", "System Tick Timer (SysTick)",
                    "SysTick_Type", get_struc_size ( sptr ) );
  }
}

void ida_cortex_m3::detectCode ( void )
{
  msg ( "ida_cortex_m3::detectCode\n" );

}

int ida_cortex_m3::init ( void )
{
  sram_base = 0x20000000;
  sram_size = 0;
  int ret = chooseAction ();

  if ( ret )
  {
    if ( sram_size == 0 )
    {
      ea_t tos = get_long ( vector_table );
      sram_base = ( ( tos >> 16 ) << 16 );  // 64 KB
      sram_size = ( ( ( tos & 0xFFF ) + 0x1000 ) >> 12 ) << 12; // 4KB * n
    }
  }

  return ret;
}

void ida_cortex_m3::run ( void )
{
  SetDefaultRegisterValue( getseg(inf.beginEA), str2reg("T"), 1);

  addSegments();

  if ( checkMask & FUNC_NAME_VECTORS )
    nameVectors ( vectors, vector_table, CM3_IRQ_COUNT );

  if ( checkMask & FUNC_NAME_REGISTERS )
    nameRegisters ();

  if ( checkMask & FUNC_DETECT_CODE )
    detectCode ();
}

ida_cortex_m3::ida_cortex_m3 ( void )
{
}


ida_cortex_m3::~ida_cortex_m3 ( void )
{
}
