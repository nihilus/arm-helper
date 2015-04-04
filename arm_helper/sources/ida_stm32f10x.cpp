#include "ida_stm32f10x.h"
#include "ida_stm32f10x_struct.h"
#include "ida_stm32f10x_vector.h"
#include "ida_name_vector.h"
#include "ida_name_register.h"

static const char STM32F10X_TypeName[ STM32F10X_TYPE_COUNT ][ 16 ] =
{
  "STM32F10X_LD_VL",
  "STM32F10X_MD_VL",
  "STM32F10X_HD_VL",
  "STM32F10X_LD   ",
  "STM32F10X_MD   ",
  "STM32F10X_HD   ",
  "STM32F10X_XL   ",
  "STM32F10X_CL   "
};

static const char strSTM32F10X_TypeHelp[ ] = //
  "  STM32F10X      FLASH Size  SRAM Size  Device Type\n"
  "--------------------------------------------------------\n"
  "  STM32F100x4    16 KB       4 KB       STM32F10X_LD_VL\n"
  "  STM32F100x6    32 KB       4 KB       STM32F10X_LD_VL\n"
  "  STM32F100x8    64 KB       8 KB       STM32F10X_MD_VL\n"
  "  STM32F100xB    128 KB      8 KB       STM32F10X_MD_VL\n"
  "  STM32F100xC    256 KB      24 KB      STM32F10X_HD_VL\n"
  "  STM32F100xD    384 KB      32 KB      STM32F10X_HD_VL\n"
  "  STM32F100xE    512 KB      32 KB      STM32F10X_HD_VL\n"
  "--------------------------------------------------------\n"
  "  STM32F101x4    16 KB       4 KB       STM32F10X_LD\n"
  "  STM32F101x6    32 KB       6 KB       STM32F10X_LD\n"
  "  STM32F101x8    64 KB       10 KB      STM32F10X_MD\n"
  "  STM32F101xB    128 KB      16 KB      STM32F10X_MD\n"
  "  STM32F101xC    256 KB      32 KB      STM32F10X_HD\n"
  "  STM32F101xD    384 KB      48 KB      STM32F10X_HD\n"
  "  STM32F101xE    512 KB      48 KB      STM32F10X_HD\n"
  "  STM32F101xF    768 KB      80 KB      STM32F10X_XL\n"
  "  STM32F101xG    1024 KB     80 KB      STM32F10X_XL\n"
  "--------------------------------------------------------\n"
  "  STM32F102x4    16 KB       4 KB       STM32F10X_LD\n"
  "  STM32F102x6    32 KB       6 KB       STM32F10X_LD\n"
  "  STM32F102x8    64 KB       10 KB      STM32F10X_MD\n"
  "  STM32F102xB    128 KB      16 KB      STM32F10X_MD\n"
  "--------------------------------------------------------\n"
  "  STM32F103x4    16 KB       6 KB       STM32F10X_LD\n"
  "  STM32F103x6    32 KB       10 KB      STM32F10X_LD\n"
  "  STM32F103x8    64 KB       20 KB      STM32F10X_MD\n"
  "  STM32F103xB    128 KB      20 KB      STM32F10X_MD\n"
  "  STM32F103xC    256 KB      48 KB      STM32F10X_HD\n"
  "  STM32F103xD    384 KB      64 KB      STM32F10X_HD\n"
  "  STM32F103xE    512 KB      64 KB      STM32F10X_HD\n"
  "  STM32F103xF    768 KB      96 KB      STM32F10X_XL\n"
  "  STM32F103xG    1024 KB     96 KB      STM32F10X_XL\n"
  "--------------------------------------------------------\n"
  "  STM32F105x8    64 KB       64 KB      STM32F10X_CL\n"
  "  STM32F105xB    128 KB      64 KB      STM32F10X_CL\n"
  "  STM32F105xC    256 KB      64 KB      STM32F10X_CL\n"
  "--------------------------------------------------------\n"
  "  STM32F107xB    128 KB      64 KB      STM32F10X_CL\n"
  "  STM32F107xC    256 KB      64 KB      STM32F10X_CL\n";

static const char strStartItem[ ] = "STARTITEM 0\n";
static const char strHelp[ ] = "HELP\n";
static const char strEndHelp[ ] = "ENDHELP\n";

static const char strSelectTitle[ ] = "Select STM32F10X Type\n\n";
static const char strSelectText[ ] = "See Help for more information\n\n";

static const char strConfirmButton[ ] =
  "BUTTON YES Yes\nBUTTON NO* No\nBUTTON CANCEL NONE\n";
static const char strConfirmTitle[ ] = "Confirm\n\n";
static const char strConfirmText[ ] =
  "You selected type is\n\n%A\n\nCountine?\n\n";

int ida_stm32f10x::showConfirmDialog ( void )
{
  int ret;
  char dialog_form[ MAXSTR ];
  char * dest = dialog_form;
  dest = qstpncpy ( dest, strConfirmButton, sizeof ( strConfirmButton ) );
  dest = qstpncpy ( dest, strConfirmTitle, sizeof ( strConfirmTitle ) );
  dest = qstpncpy ( dest, strConfirmText, sizeof ( strConfirmText ) );
  ret = AskUsingForm_c ( dialog_form, STM32F10X_TypeName[ type ] );
  return ret;
}

int ida_stm32f10x::showSelectDialog ( void )
{
  int ret;
  uval_t i;
  ushort btnIndex;
  char dialog_form[ MAXSTR * 8 ];
  char * dest = dialog_form;
  // where number is a number of input field the cursor will stand on (0 based)
  dest = qstpncpy ( dest, strStartItem, sizeof ( strStartItem ) );
  // Help
  dest = qstpncpy ( dest, strHelp, sizeof ( strHelp ) );
  dest = qstpncpy ( dest, strSTM32F10X_TypeHelp,
                    sizeof ( strSTM32F10X_TypeHelp ) );
  dest = qstpncpy ( dest, strEndHelp, sizeof ( strEndHelp ) );
  // Title : Next there must be the title line and 2 empty lines
  dest = qstpncpy ( dest, strSelectTitle, sizeof ( strSelectTitle ) );
  // All text in the dialog box text string is copied to the dialog
  // without any modifications
  dest = qstpncpy ( dest, strSelectText, sizeof ( strSelectText ) );

  // All input field types (except B and K) are valid format specifiers
  // Input fields are represented by the following combination
  //
  // < # hint_message # label : type : width : swidth : @hlp[] >
  //
  for ( i = 0; i < STM32F10X_TYPE_COUNT; i++ )
  {
    dest = qstpncpy ( dest, "<", 2 );
    dest = qstpncpy ( dest, &STM32F10X_TypeName[ i ][ 0 ],
                      sizeof ( STM32F10X_TypeName[ i ] ) );
    dest = qstpncpy ( dest, ":R", 3 );

    if ( i == STM32F10X_TYPE_COUNT - 1 )
      dest = qstpncpy ( dest, ">", 2 );

    dest = qstpncpy ( dest, ">\n", 3 );
  }

  btnIndex = type;
  ret = AskUsingForm_c ( dialog_form, &btnIndex );
  type = ( eSTM32F10X_TYPE ) btnIndex;
  return ret;
}

int ida_stm32f10x::selectType ( void )
{
  int ret;

  while ( 1 )
  {
    ret = showSelectDialog( );

    if ( ret == 0 )
      return ret;

    ret = showConfirmDialog( );

    if ( ret == ASKBTN_YES )
      return 1;
  }
}

ida_stm32f10x::ida_stm32f10x ( void )
{
}

ida_stm32f10x::~ida_stm32f10x ( void )
{
}

void ida_stm32f10x::nameRegisters ( void )
{
  msg ( "ida_stm32f10x::nameRegisters\n" );
}


int ida_stm32f10x::init ( void )
{
  int ret;
  // msg("ida_stm32f10x::run\n");
  type = STM32F10X_HD;

  ret = selectType( );

  if ( ret == 0 )
    return ret;

  sram_base = 0x20000000;

  switch ( type )
  {
    case STM32F10X_LD_VL:
      sram_size = 4 * 1024;
      break;

    case STM32F10X_MD_VL:
      sram_size = 8 * 1024;
      break;

    case STM32F10X_HD_VL:
      sram_size = 32 * 1024;
      break;

    case STM32F10X_LD:
      sram_size = 10 * 1024;
      break;

    case STM32F10X_MD:
      sram_size = 20 * 1024;
      break;

    case STM32F10X_HD:
      sram_size = 64 * 1024;
      break;

    case STM32F10X_XL:
      sram_size = 96 * 1024;
      break;

    case STM32F10X_CL:
      sram_size = 64 * 1024;
      break;

    default:
      sram_size = 0x00000000;
      break;
  }

  ret = chooseAction ();

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

#define CM3_IRQ_COUNT           (16)

void ida_stm32f10x::nameAllVectors ( void )
{
  nameVectors ( stm32f10x_common_vectors, vector_table + CM3_IRQ_COUNT * 4,
                STM32F10X_COMMON_IRQ_COUNT );

  switch ( type )
  {
    case STM32F10X_LD_VL:
      nameVectors ( stm32f10x_ld_vl_vectors,
                    vector_table + CM3_IRQ_COUNT * 4 + STM32F10X_COMMON_IRQ_COUNT * 4,
                    STM32F10X_LD_VL_IRQ_COUNT );
      break;

    case STM32F10X_MD_VL:
      nameVectors ( stm32f10x_md_vl_vectors,
                    vector_table + CM3_IRQ_COUNT * 4 + STM32F10X_COMMON_IRQ_COUNT * 4,
                    STM32F10X_MD_VL_IRQ_COUNT );
      break;

    case STM32F10X_HD_VL:
      nameVectors ( stm32f10x_hd_vl_vectors,
                    vector_table + CM3_IRQ_COUNT * 4 + STM32F10X_COMMON_IRQ_COUNT * 4,
                    STM32F10X_HD_VL_IRQ_COUNT );
      break;

    case STM32F10X_LD:
      nameVectors ( stm32f10x_ld_vectors,
                    vector_table + CM3_IRQ_COUNT * 4 + STM32F10X_COMMON_IRQ_COUNT * 4,
                    STM32F10X_LD_IRQ_COUNT );
      break;

    case STM32F10X_MD:
      nameVectors ( stm32f10x_md_vectors,
                    vector_table + CM3_IRQ_COUNT * 4 + STM32F10X_COMMON_IRQ_COUNT * 4,
                    STM32F10X_MD_IRQ_COUNT );
      break;

    case STM32F10X_HD:
      nameVectors ( stm32f10x_hd_vectors,
                    vector_table + CM3_IRQ_COUNT * 4 + STM32F10X_COMMON_IRQ_COUNT * 4,
                    STM32F10X_HD_IRQ_COUNT );
      break;

    case STM32F10X_XL:
      nameVectors ( stm32f10x_xl_vectors,
                    vector_table + CM3_IRQ_COUNT * 4 + STM32F10X_COMMON_IRQ_COUNT * 4,
                    STM32F10X_XL_IRQ_COUNT );
      break;

    case STM32F10X_CL:
      nameVectors ( stm32f10x_cl_vectors,
                    vector_table + CM3_IRQ_COUNT * 4 + STM32F10X_COMMON_IRQ_COUNT * 4,
                    STM32F10X_CL_IRQ_COUNT );
      break;

    default:
      break;
  }
}

void ida_stm32f10x::run ( void )
{
  msg("ida_cortex_m3::run()\n");
  ida_cortex_m3::run();

  msg("ida_stm32f10x::run()\n");

  if ( checkMask & FUNC_NAME_VECTORS )
    nameAllVectors ( );

  if ( checkMask & FUNC_NAME_REGISTERS )
    nameRegisters ();
}
