#include "ida_sdk.h"

static const char strStartItem[] = "STARTITEM 0\n";
static const char strHelp[] = "HELP\n";
static const char strEndHelp[] = "ENDHELP\n";

static const char strArchitectureTitle[] = "Select ARM Architecture\n\n";
static const char strArchitectureHelp[] = "Select ARM Architecture\n";

enum
{
  ARM_TYPE_DEVICE = 0,
  ARM_TYPE_ARM7_ARM9,
  ARM_TYPE_CORTEX_M0_M1,
  ARM_TYPE_CORTEX_M3_M4,
  ARM_ARCH_COUNT
};

static const char architectures[ARM_ARCH_COUNT ][24] =
{
  "Architecture By Device ",  //
  "ARMv4T (ARM7/ARM9)     ",  // ARMv4 : ARM only, ARMv4T : ARM + Thumb
  "ARMv6-M (CORTEX-M0/M1) ",  // Thumb + Thumb-2 : BL, MRS, MSR, ISB, DSB, DMB
  "ARMv7-M (CORTEX-M3/M4) ",  // Thumb + Thumb-2, M4 : DSP, M4F : FPv4
};

static int idaapi selectArchitecture ( int * pArchitecture )
{
  char dialog_form[MAXSTR * 8];
  char * dest = dialog_form;

  // where number is a number of input field the cursor will stand on (0 based)
  dest = qstpncpy ( dest, strStartItem, sizeof ( strStartItem ) );

  // Help
  dest = qstpncpy ( dest, strHelp, sizeof ( strHelp ) );
  dest = qstpncpy ( dest, strArchitectureHelp,
                    sizeof ( strArchitectureHelp ) );
  dest = qstpncpy ( dest, strEndHelp, sizeof ( strEndHelp ) );

  // Title : Next there must be the title line and 2 empty lines
  dest = qstpncpy ( dest, strArchitectureTitle,
                    sizeof ( strArchitectureTitle ) );

  // All text in the dialog box text string is copied to the dialog
  // without any modifications
  // dest = qstpncpy ( dest, strSelectText, sizeof ( strSelectText ) );

  // All input field types (except B and K) are valid format specifiers
  // Input fields are represented by the following combination
  //
  // < # hint_message # label : type : width : swidth : @hlp[] >
  //
  for ( int i = 0; i < ARM_ARCH_COUNT; i++ )
  {
    dest = qstpncpy ( dest, "<", 2 );
    dest = qstpncpy ( dest, &architectures[i][0], sizeof ( architectures[i] ) );
    dest = qstpncpy ( dest, ":R", 3 );

    if ( i == ARM_ARCH_COUNT - 1 )
      dest = qstpncpy ( dest, ">", 2 );

    dest = qstpncpy ( dest, ">\n", 3 );
  }

  return AskUsingForm_c ( dialog_form, pArchitecture );
}

enum
{
  DEVICE_MANF_ATMEL = 0,
  DEVICE_MANF_NXP,
  DEVICE_MANF_ST,
  DEVICE_MANF_COUNT
};

static const char manufacturers[DEVICE_MANF_COUNT][6] =
{
  "Atmel",
  "NXP",
  "ST"
};

static const char strManufacturerTitle[] = "Select Device Manufacturer\n\n";
static const char strManufacturerHelp[] = "Select Device Manufacturer\n";

static int idaapi selectManufacturer ( int * pManufacturer )
{
  char dialog_form[MAXSTR * 8];
  char * dest = dialog_form;

  // where number is a number of input field the cursor will stand on (0 based)
  dest = qstpncpy ( dest, strStartItem, sizeof ( strStartItem ) );

  // Help
  dest = qstpncpy ( dest, strHelp, sizeof ( strHelp ) );
  dest = qstpncpy ( dest, strManufacturerHelp,
                    sizeof ( strManufacturerHelp ) );
  dest = qstpncpy ( dest, strEndHelp, sizeof ( strEndHelp ) );

  // Title : Next there must be the title line and 2 empty lines
  dest = qstpncpy ( dest, strManufacturerTitle,
                    sizeof ( strManufacturerTitle ) );

  // All text in the dialog box text string is copied to the dialog
  // without any modifications
  // dest = qstpncpy ( dest, strSelectText, sizeof ( strSelectText ) );

  // All input field types (except B and K) are valid format specifiers
  // Input fields are represented by the following combination
  //
  // < # hint_message # label : type : width : swidth : @hlp[] >
  //
  for ( int i = 0; i < DEVICE_MANF_COUNT; i++ )
  {
    dest = qstpncpy ( dest, "<", 2 );
    dest = qstpncpy ( dest, &manufacturers[i][0], sizeof ( manufacturers[i] ) );
    dest = qstpncpy ( dest, ":R", 3 );

    if ( i == DEVICE_MANF_COUNT - 1 )
      dest = qstpncpy ( dest, ">", 2 );

    dest = qstpncpy ( dest, ">\n", 3 );
  }

  return AskUsingForm_c ( dialog_form, pManufacturer );
}

static void idaapi showSelectDialog ( void )
{
  static int architecture = ARM_TYPE_DEVICE;
  static int manufacturer = DEVICE_MANF_ST;

  while ( 1 )
  {
    if ( 0 == selectArchitecture ( &architecture ) )
      return;

    if ( architecture == ARM_TYPE_DEVICE )
    {
      if ( 0 == selectManufacturer ( &manufacturer ) )
        continue;
    }

    switch ( architecture )
    {
      case ARM_TYPE_DEVICE:
        break;

      case ARM_TYPE_ARM7_ARM9:
        break;

      case ARM_TYPE_CORTEX_M0_M1:
        break;

      case ARM_TYPE_CORTEX_M3_M4:
        break;
    }
  }
}