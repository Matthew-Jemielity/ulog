/**
 * \author      Mateusz Jemielity matthew.jemielity@gmail.com
 * \brief       Implements singly linked list operations.
 * \date        08/31/2015 10:47:21 PM
 * \file        listable.c
 * \version     1.0
 *
 *
 **/

#include <ulog/listable.h>
#include <ulog/universal.h> /* UNUSED */
#include <ulog/status.h> /* ulog_status, ulog_status_* */

#include <errno.h> /*ENOENT,EAGAIN,EALREADY,EEXIST,EINVAL,ENODATA,ENOEXEC*/
#include <stdbool.h> /* bool */
#include <stddef.h> /* NULL */

static ulog_listable guard;
static ulog_listable clean;

static inline bool
valid( ulog_list_ctrl const * const self );

static inline bool
valid_element( ulog_listable const * const element)
{
    return
      (
          ( NULL != element )
          && ( &guard != element )
          && ( &clean != element )
      );
}

static inline bool
valid_new_element( ulog_listable const * const element )
{
    return (valid_element( element ) && ( &clean == element->next ));
}

static inline ulog_status
generic_invalid( ulog_list_ctrl const * const self )
{
    UNUSED( self );
    return ulog_status_descriptive( EINVAL, "invalid list control object" );
}

static inline ulog_status
generic_invalid_element( ulog_list_ctrl const * const self )
{
    UNUSED( self );
    return ulog_status_descriptive( ENODATA, "invalid listable object" );
}

static ulog_status
foreach_safe(
    ulog_list_ctrl const * const self,
    ulog_list_foreach_op_callback_fn const callback,
    void * const userdata,
    int const expected
);

static inline ulog_status
add_callback( ulog_listable * const element, void * const userdata )
{
    ulog_listable * const item = userdata;
    if( element == item )
    {
        return
          ulog_status_descriptive(
              EALREADY,
              "listable object already in list"
          );
    }
    if ( &guard == element->next ) {
      element->next = item;
      item->next = &guard;
      /*
       * We added userdata as last element, so next foreach iteration
       * would trigger EALREADY. To counteract that treat zero as
       * unexpected error value and EAGAIN as normal output. The
       * foreach will return zero as we want eventually, unless
       * someone manually messes with the list, but in that case
       * all bets are off anyway.
       */
      return ulog_status_descriptive( 0, "listable object added to list" );
    }
    return ulog_status_descriptive(EAGAIN, "check next element");
}

static inline ulog_status
add( ulog_list_ctrl * const self, ulog_listable * const element )
{
    if( !valid( self )) { return generic_invalid( self ); }
    if( !valid_new_element( element ))
    {
        return generic_invalid_element( self );
    }
    if( &guard == self->head )
    {
        self->head = element;
        element->next = &guard;
        return ulog_status_descriptive( 0, "listable object added as head" );
    }
    return foreach_safe( self, add_callback, element, EAGAIN );
}

static inline ulog_status
remove_callback( ulog_listable * const element, void * const userdata )
{
    ulog_listable * const item = userdata;
    if( item == element->next )
    {
        element->next = element->next->next;
        item->next = &clean;
        return
          ulog_status_descriptive(
              0,
              "listable object removed from list"
          );
    }
    return
      ulog_status_descriptive(
          EEXIST,
          "listable object not found in list"
      );
}

static inline ulog_status
remove( ulog_list_ctrl * const self, ulog_listable * const element )
{
    if( !valid( self )) { return generic_invalid( self ); }
    if( !valid_element( element )) { return generic_invalid_element( self ); }
    if( &guard == self->head )
    {
        return ulog_status_descriptive( ENOENT, "list empty" );
    }
    if( element == self->head )
    {
        self->head = self->head->next;
        element->next = &clean;
        return
          ulog_status_descriptive( 0, "listable object removed from head" );
    }
    return foreach_safe( self, remove_callback, element, EEXIST );
}

static ulog_status
foreach_safe(
    ulog_list_ctrl const * const self,
    ulog_list_foreach_op_callback_fn const callback,
    void * const userdata,
    int const expected
)
{
    ulog_listable * iterator;
    ulog_status result;
    for( iterator = self->head; iterator != &guard; iterator = iterator->next )
    {
        result = callback( iterator, userdata );
        if( expected != ulog_status_to_int( result )) { break; }
    }
    return result;
}

static inline ulog_status
foreach(
    ulog_list_ctrl const * const self,
    ulog_list_foreach_op_callback_fn const callback,
    void * const userdata
)
{
    if( !valid( self )) { return generic_invalid( self ); }
    if( NULL == callback )
    {
        return ulog_status_descriptive( ENOEXEC, "foreach callback invalid" );
    }
    if( &guard == self->head )
    {
        return ulog_status_descriptive( ENOENT, "list empty" );
    }
    return foreach_safe( self, callback, userdata, 0 );
}

static ulog_list_op_table const default_state =
{
    .add = add,
    .remove = remove,
    .foreach = foreach
};

static inline bool
valid( ulog_list_ctrl const * const self )
{
    return
      (
          ( NULL != self )
          && ( NULL != self->head )
          && ( &default_state == self->op )
      );
}

ulog_listable
ulog_listable_get( void )
{
    return ( ulog_listable ) { .next = &clean };
}

ulog_list_ctrl
ulog_list_ctrl_get( void )
{
    return ( ulog_list_ctrl ) { .head = &guard, .op = &default_state };
}

void
ulog_listable_compile_time_check( ulog_listable const element )
{
    UNUSED( element );
    /* Exists only to validate macro argument at compile time. */
}

