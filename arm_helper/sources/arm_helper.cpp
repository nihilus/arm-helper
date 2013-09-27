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

typedef enum
{
  ARM_MANF_ATMEL,
  ARM_MANF_NXP,
  ARM_MANF_ST,
  ARM_MANF_COUNT
} arm_manf_t;

typedef enum
{
  ARM_ARCH_ARM7_ARM9,
  ARM_ARCH_CORTEX_M0_M1,
  ARM_ARCH_CORTEX_M3_M4,
  ARM_ARCH_COUNT
} arm_arch_t;

typedef struct
{
  uint32 arm_arch;
  uint32 arm_manf;
  uint32 arm_series;
  uint32 arm_device;
  uint32 vector;
  uint32 sram_base;
  uint32 sram_size;
  uint32 action_mask;
} arm_infomation_t;

//Define the function prototype
typedef int ( * armHelperType ) ( arm_infomation_t * arm_infomation );

int armHelper ( arm_infomation_t * arm_infomation )
{
  int ret = 0;
  armHelperType armHelperPtr;
  char * pArmHelperPathAndName;
  char armHelperPathAndName[MAXSTR];
  HMODULE idaHandle = GetModuleHandleA ( NULL );
  GetModuleFileNameA ( idaHandle, armHelperPathAndName, MAXSTR );
  pArmHelperPathAndName = armHelperPathAndName;

  while ( strchr ( pArmHelperPathAndName, '\\' ) )
  {
    pArmHelperPathAndName = strchr ( pArmHelperPathAndName, '\\' );
    pArmHelperPathAndName++;
  }

  qstpncpy ( pArmHelperPathAndName, "plugins\\arm_helper.dll", MAXSTR );
  msg ( "%s\n", armHelperPathAndName );

  // Load the dll and keep the handle to it
  HMODULE dllHandle = LoadLibraryA ( armHelperPathAndName );

  // If the handle is valid, try to get the function address.
  if ( dllHandle != NULL )
  {
    // Get pointer to our function using GetProcAddress:
    armHelperPtr = ( armHelperType ) GetProcAddress ( dllHandle,
                   "armHelperDialog" );

    // If the function address is valid, call the function late.
    if ( armHelperPtr != NULL )
      ret = armHelperPtr ( arm_infomation );

    // Free the library:
    FreeLibrary ( dllHandle );
  }

  return ret;
}

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

void idaapi IDAP_run ( int arg )
{
  static arm_infomation_t arm_infomation;

  if ( ASKBTN_YES != armHelper ( &arm_infomation ) )
    return;

}


