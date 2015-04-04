#include "ida_name_vector.h"

void idaapi nameVector ( vector_t * vect, ea_t entry, char * suffix )
{
  char fullName[ MAXSTR];
  char fullHandler[ MAXSTR];
  char * pFullName;
  char * pFullHandler;
  refinfo_t ri;

  pFullName = vect->name;
  pFullHandler = vect->handler;

  if ( suffix )
  {
    if ( vect->name )
    {
      pFullName = qstpncpy ( fullName, vect->name, strlen ( vect->name ) + 1 );
      qstpncpy ( pFullName, suffix, strlen ( suffix ) + 1 );
      pFullName = fullName;
    }

    if ( vect->handler )
    {
      pFullHandler = qstpncpy ( fullHandler, vect->handler,
                                strlen ( vect->handler ) + 1 );
      qstpncpy ( pFullHandler, suffix, strlen ( suffix ) + 1 );
      pFullHandler = fullHandler;
    }
  }

  do_unknown ( entry, DOUNK_EXPAND );
  do_data_ex ( entry, dwrdflag (), 4, BADNODE );

  if ( pFullName )
    set_name ( entry, pFullName );

  if ( vect->comment )
    set_cmt ( entry, vect->comment, false );

  if ( pFullHandler && isLoaded ( entry ) )
  {
    ea_t handler = get_long ( entry );
    adiff_t diff = handler & 1;
    if ( isEnabled ( handler ) == false )
      return;

    handler &= ~1;
    //add_dref ( base + vect->offset, handler,  ( dref_t ) ( dr_O | XREF_USER ) );

    if ( get_name ( BADADDR, handler, fullName, MAXSTR ) == NULL )
    {
      do_unknown ( handler, DOUNK_EXPAND );
      if ( diff == 0 ) // Top of Stack
        do_data_ex ( handler, dwrdflag (), 4, BADNODE );
      else if ( isLoaded ( handler ) ) // Handler
        add_func ( handler, BADADDR );

      set_name ( handler, pFullHandler );

      if ( vect->comment )
        set_cmt ( handler, vect->comment, false );
    }

    ri.init ( REF_OFF32, 0, handler, diff );
    op_offset_ex ( entry, 0, &ri );
    return;
  }
}

