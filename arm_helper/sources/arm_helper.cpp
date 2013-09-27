#include "ida_sdk.h"
#include "ida_stm32f10x.h"
#include "ida_cortex_m3.h"

//------------------------------------------------------------------------------
// This is the preferred name of the plugin module in the menu system
// The preferred name may be overriden in plugins.cfg file
// 在Edit->Plugins 菜单中，插件的现实名称，它能被用户的plugins.cfg文件改写
//
char IDAP_name[ ] = "ARM Utility";

char IDAP_comment[ ] = "ARM Utility";

char IDAP_help[ ] = "ARM Utility\n";

//------------------------------------------------------------------------------
// This is the preferred hotkey for the plugin module
// The preferred hotkey may be overriden in plugins.cfg file
// Note: IDA won't tell you if the hotkey is not correct
//       It will just disable the hotkey.
//
char IDAP_hotkey[ ] = "Ctrl-Alt-A"; // 启动插件的热键

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//
// Initialize.
//
// IDA will call this function only once.
// If this function returns PLGUIN_SKIP, IDA will never load it again.
// If this function returns PLUGIN_OK, IDA will unload the plugin but
// remember that the plugin agreed to work with the database.
// The plugin will be loaded again if the user invokes it by
// pressing the hotkey or selecting it from the menu.
// After the second load the plugin will stay on memory.
// If this function returns PLUGIN_KEEP, IDA will keep the plugin
// in the memory. In this case the initialization function can hook
// into the processor module and user interface notification points.
// See the hook_to_notification_point() function.
//
// In this example we check the input file format and make the decision.
// You may or may not check any other conditions to decide what you do:
// whether you agree to work with the database or not.
//
// 在这里做一些校验，以确保您的插件是被用在合适的环境里。
//
int idaapi IDAP_init ( void )
{
  // msg("IDAP_init\n");

  // Only support ARM architecture
  if ( strncmp ( inf.procName, "ARM", 3 ) != 0 )
  {
    warning ( "Only support ARM architecture, sorry." );
    return PLUGIN_SKIP;
  }

  return ( PLUGIN.flags & PLUGIN_UNL ) ? PLUGIN_OK : PLUGIN_KEEP;
}

//------------------------------------------------------------------------------
//
// Terminate.
//
// Usually this callback is empty.
// The plugin should unhook from the notification lists if
// hook_to_notification_point() was used.
//
// IDA will call this function when the user asks to exit.
// This function won't be called in the case of emergency exits.
//
// 当结束插件时，一般您可以在此添加一点任务清理的代码。
//
void idaapi IDAP_term ( void )
{
  // msg("IDAP_term\n");
}

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

//------------------------------------------------------------------------------
//
// PLUGIN DESCRIPTION BLOCK
// 所有PLUGIN对象导出的重要属性。
//
void idaapi IDAP_run ( int arg );

plugin_t PLUGIN =
{
  IDP_INTERFACE_VERSION, // IDA version plug-in is written for
  PLUGIN_UNL, // Flags (see below)
  IDAP_init, // Initialisation function
  IDAP_term, // Clean-up function
  IDAP_run, // Main plug-in body
  IDAP_comment, // Comment – unused
  IDAP_help, // As above – unused
  IDAP_name, // Plug-in name shown in Edit->Plugins menu
  IDAP_hotkey // Hot key to run the plug-in
};

//------------------------------------------------------------------------------
//
// The plugin method
//
// This is the main function of plugin.
//
// It will be called when the user activates the plugin.
//
// arg - the input argument, it can be specified in plugins.cfg file.
// The default is zero.
//
// 插件的实体
// 插件可以从plugins.cfg文件中，被传进一个整型参数。
// 当按下不同的热键或者菜单时，您需要一个插件做不同的事情时，这非常有用。
//
//Define the function prototype
typedef int ( CALLBACK * ShowDialogType ) ( void );

void idaapi IDAP_run ( int arg )
{
  // Load the dll and keep the handle to it
  HMODULE dllHandle = LoadLibraryA ( "D:\\IDA64\\arm_helpher.dll" );

  if ( dllHandle == NULL )
  {
    msg("load failed %08X\n", GetLastError() );
    return;
  }

  // If the handle is valid, try to get the function address.
  // Get pointer to our function using GetProcAddress:
  ShowDialogType showDialogPtr = ( ShowDialogType ) GetProcAddress ( dllHandle,
                                 "showDialog" );

  // If the function address is valid, call the function.
  if ( showDialogPtr != NULL )
    showDialogPtr();

  // Free the library:
  FreeLibrary(dllHandle); 

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


