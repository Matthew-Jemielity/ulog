/**
 * \author      Mateusz Jemielity matthew.jemielity@gmail.com
 * \brief       Definitions of pointer list methods.
 * \date        2015/04/24 18:28:42 AM
 * \file        pointer_list.c
 * \version     1.0
 *
 *
 **/

#include <errno.h> /* EINVAL, EEXIST, ENOMEM, ENODATA, ENOEXEC */
#include <stddef.h> /* NULL */
#include <stdlib.h>
#include <ulog/pointer_list.h>
#include <ulog/status.h> /* ulog_status */
#include <ulog/universal.h> /* UNUSED */

static ulog_status
add( ulog_pointer_list * const self, void * const pointer )
{
    if( NULL == self )
    {
        return ulog_status_from_int( EINVAL );
    }

    ulog_pointer_list_element ** current = &( self->head );

    for( ; NULL != *current; current = &(( *current )->next ))
    {
        if( pointer == ( *current )->pointer )
        {
            return ulog_status_from_int( EEXIST );
        }
    }

    *current = malloc( sizeof( ulog_pointer_list_element ));
    if( NULL == *current )
    {
        return ulog_status_from_int( ENOMEM );
    }
    ( *current )->pointer = pointer;
    ( *current )->next = NULL;
    return ulog_status_from_int( 0 );
}

static ulog_status
remove( ulog_pointer_list * const self, void * const pointer )
{
    if( NULL == self )
    {
        return ulog_status_from_int( EINVAL );
    }

    for(
        ulog_pointer_list_element ** current = &( self->head );
        NULL != *current;
        current = &(( *current )->next )
    )
    {
        if( pointer != ( *current )->pointer )
        {
            continue;
        }

        ulog_pointer_list_element * const found = *current;
        *current = ( *current )->next;
        free( found );
        return ulog_status_from_int( 0 );
    }

    return ulog_status_from_int( ENODATA );
}

static ulog_status foreach(
    ulog_pointer_list const * const self,
    ulog_pointer_list_foreach_callback const callback,
    void * const userdata
)
{
    if( NULL == self )
    {
        return ulog_status_from_int( EINVAL );
    }
    if( NULL == callback )
    {
        return ulog_status_from_int( ENOEXEC );
    }

    ulog_status result = ulog_status_from_int( ENODATA );
    for(
        ulog_pointer_list_element * current = self->head;
        current != NULL;
        current = current->next
    )
    {
        result = callback( current->pointer, userdata );
        if( 0 != ulog_status_to_int( result ))
        {
            break;
        }
    }
    return result;
}

ulog_pointer_list ulog_pointer_list_setup( void )
{
    return ( ulog_pointer_list )
    {
        .head = NULL,
        .add = add,
        .remove = remove,
        .foreach = foreach
    };
}

/*
 * removal of element known to be on the list will not fail
 * therefore we'll simplify the implementation to use that info
 */
ulog_status ulog_pointer_list_cleanup( ulog_pointer_list * const list )
{
    if( NULL == list )
    {
        return ulog_status_from_int( EINVAL );
    }

    while( NULL != list->head )
    {
        UNUSED( list->remove( list, list->head->pointer ));
    }
    return ulog_status_from_int( 0 );
}

