#include "ida_cortex_m3_struct.h"

struc_t * cm3Struct_SysTick_Type ( char * name )
{
  enum_t enum_id;
  member_t * mptr;

  tid_t tid = get_struc_id ( name ) ; // get struct id by name
  struc_t * sptr = get_struc ( tid ); // get struct by id

  if ( sptr != NULL )
    return sptr;

  tid = add_struc ( BADNODE, name ) ; // add struct with name
  sptr = get_struc ( tid );           // get struct by id

  if ( sptr == NULL )                 // error ?
    return sptr;

  set_struc_cmt ( get_struc_id ( name ) ,
                  "Structure type to access the System Timer (SysTick)", false );

  //****************************************************************************
  // SysTick Control and Status Register
  //****************************************************************************
  char comment_SysTick_CTRL[] = // both enum and struct member commment
    "SysTick Control and Status Register\n"
    "\n"
    "bits[31:17] --------\n"
    "Reserved.\n"
    "\n"
    "bits[16:16] COUNTFLAG\n"
    "Returns 1 if timer counted to 0 since last time this was read.\n"
    "\n"
    "bits[15:03] --------\n"
    "Reserved.\n"
    "\n"
    "bits[02:02] CLKSOURCE\n"
    "Returns 1 if timer counted to 0 since last time this was read.\n"
    "\n"
    "bits[01:01] TICKINT\n"
    "Enables SysTick exception request:\n"
    "0 = counting down to zero does not assert the SysTick exception request\n"
    "1 = counting down to zero asserts the SysTick exception request.\n"
    "Software can use COUNTFLAG to determine if SysTick has ever counted to zero.\n"
    "\n"
    "bits[00:00] ENABLE\n"
    "Enables the counter\n"
    "0 = counter disabled\n"
    "1 = counter enabled.\n"
    ;

  //
  // SysTick Control and Status Register : enum
  //
  enum_id = get_enum ( "SysTick_CTRL" );

  if ( enum_id == BADNODE )
  {
    enum_id = add_enum ( BADADDR, "SysTick_CTRL", hexflag() );
    set_enum_cmt ( enum_id, comment_SysTick_CTRL, true );
    set_enum_bf ( enum_id, true );
    add_enum_member ( enum_id, "SysTick_CTRL_ENABLE", ( 1 << 0 ), ( 1 << 0 ) );
    add_enum_member ( enum_id, "SysTick_CTRL_TICKINT", ( 1 << 1 ), ( 1 << 1 ) );
    add_enum_member ( enum_id, "SysTick_CTRL_CLKSOURCE", ( 1 << 2 ), ( 1 << 2 ) );
    add_enum_member ( enum_id, "SysTick_CTRL_COUNTFLAG", ( 1 << 16 ), ( 1 << 16 ) );
  }

  //
  // SysTick Control and Status Register : struct member
  //
  if ( 0 == add_struc_member ( sptr, "CTRL", BADADDR, dwrdflag(), NULL, 4 ) )
  {
    mptr = get_member_by_name ( sptr, "CTRL" );

    if ( mptr != NULL )
      set_member_cmt ( mptr, comment_SysTick_CTRL, true );
  }

  //****************************************************************************
  // SysTick Reload Value Register
  //****************************************************************************
  char comment_SysTick_LOAD[] = //
    "SysTick Reload Value Register\n"
    "\n"
    "bits[31:24] --------\n"
    "Reserved.\n"
    "\n"
    "bits[23:00] RELOAD\n"
    "Value to load into the SYST_CVR register when the counter is enabled \n"
    "and when it reaches 0, see Calculating the RELOAD value.\n"
    ;

  if ( 0 == add_struc_member ( sptr, "LOAD", BADADDR, dwrdflag(), NULL, 4 ) )
  {
    mptr = get_member_by_name ( sptr, "LOAD" );

    if ( mptr != NULL )
      set_member_cmt ( mptr, comment_SysTick_LOAD, true );
  }


  //****************************************************************************
  // SysTick Current Value Register
  //****************************************************************************
  char comment_SysTick_VAL[] = //
    "SysTick Current Value Register\n"
    "\n"
    "bits[31:24] --------\n"
    "Reserved.\n"
    "\n"
    "bits[23:00] CURRENT\n"
    "Reads return the current value of the SysTick counter.\n"
    "A write of any value clears the field to 0, and also clears the SYST_CSR COUNTFLAG bit to 0.\n"
    ;

  if ( 0 == add_struc_member ( sptr, "VAL", BADADDR, dwrdflag(), NULL, 4 ) )
  {
    mptr = get_member_by_name ( sptr, "VAL" );

    if ( mptr != NULL )
      set_member_cmt ( mptr, comment_SysTick_VAL, true );
  }


  //****************************************************************************
  // SysTick Calibration Register
  //****************************************************************************
  char comment_SysTick_CALIB[] = //
    "SysTick Calibration Value Register\n"
    "\n"
    "bits[31:31] NOREF\n"
    "Indicates whether the device provides a reference clock to the processor:\n"
    "0 = reference clock provided\n"
    "1 = no reference clock provided\n"
    "If your device does not provide a reference clock,\n"
    "the SYST_CSR.CLKSOURCE bit reads-as-one and ignores writes.\n"
    "\n"
    "bits[30:30] SKEW\n"
    "Indicates whether the TENMS value is exact:\n"
    "0 = TENMS value is exact\n"
    "1 = TENMS value is inexact, or not given.\n"
    "An inexact TENMS value can affect the suitability of SysTick as a software real time clock.\n"
    "\n"
    "bits[29:24] --------\n"
    "Reserved.\n"
    "\n"
    "bits[23:00] TENMS\n"
    "Reload value for 10ms (100Hz) timing, subject to system clock skew errors.\n"
    "If the value reads as zero, the calibration value is not known.\n"
    ;

  enum_id = get_enum ( "SysTick_CALIB" );

  if ( enum_id == BADNODE )
  {
    enum_id = add_enum ( BADADDR, "SysTick_CALIB", hexflag() );
    set_enum_cmt ( enum_id, comment_SysTick_CALIB, true );
    set_enum_bf ( enum_id, true );
    add_enum_member ( enum_id, "SysTick_SKEW", ( 1 << 30 ), ( 1 << 30 ) );
    add_enum_member ( enum_id, "SysTick_NOREF", ( 1 << 31 ), ( 1 << 31 ) );
  }

  if ( 0 == add_struc_member ( sptr, "CALIB", BADADDR, dwrdflag(), NULL, 4 ) )
  {
    mptr = get_member_by_name ( sptr, "CALIB" );

    if ( mptr != NULL )
      set_member_cmt ( mptr, comment_SysTick_CALIB, true );
  }

  return sptr;
}