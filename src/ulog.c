/**
 * \author      Mateusz Jemielity matthew.jemielity@gmail.com
 * \brief       Definitions of logging framework methods.
 * \date        2015/04/24 20:56:01 PM
 * \file        ulog.c
 * \version     1.0
 *
 *
 **/

#define _POSIX_C_SOURCE 199309L /* for clock_gettime */

#include <assert.h>
#include <errno.h> /* EALREADY, EINVAL, etc. */
#include <stdbool.h> /* bool */
#include <stddef.h> /* NULL */
#include <stdint.h> /* uint64_t */
#include <stdlib.h> /* malloc, free */
#include <string.h> /* memcpy */
#include <time.h> /* clock_gettime, struct timespec */
#include <ulog/ulog.h>
#include <ulog/mutex.h> /* ulog_mutex */
#include <ulog/pointer_list.h> /* ulog_pointer_list */
#include <ulog/status.h> /* ulog_status */
#include <ulog/universal.h> /* THREADLOCAL, THREADUNSAFE, UNUSED */

#define NANOSECONDS_IN_MICROSECOND 1000U
#define MICROSECONDS_IN_MILLISECOND 1000U
#define MILLISECONDS_IN_SECOND 1000U

struct ulog_obj_state_struct
{
    bool initialized;
    ulog_pointer_list handlers;
    ulog_mutex_ctrl guard;
};

static uint64_t
from_timespec( struct timespec const value )
{
    return ( ( uint64_t ) value.tv_sec )
        * NANOSECONDS_IN_MICROSECOND
        * MICROSECONDS_IN_MILLISECOND
        * MILLISECONDS_IN_SECOND
        + ( ( uint64_t ) value.tv_nsec );
}

INDIRECT uint64_t
ulog_current_time( void )
{
    struct timespec result;

    if( 0 == clock_gettime( CLOCK_REALTIME, &result ))
    { return from_timespec( result ); }

    return 0;
}

typedef struct
{
    ulog_log_handler handler;
}
pointer_list_element;

static ulog_status
adding_callback( void * const pointer, void * const userdata )
{
    pointer_list_element const * const element = pointer;
    pointer_list_element const * const data = userdata;

    return ulog_status_from_int(
            ( element->handler == data->handler ) ? EEXIST : 0
        );
}

static ulog_status
add( ulog_obj const self, ulog_log_handler const handler )
{
    if( NULL == self.state )
    {
        return ulog_status_from_int( EINVAL );
    }

    pointer_list_element * const element =
        malloc( sizeof( pointer_list_element ));
    if( NULL == element )
    {
        return ulog_status_from_int( ENOMEM );
    }

    element->handler = handler;

    ulog_status result =
        self.state->guard.mutex.lock( self.state->guard.mutex );
    if( 0 != ulog_status_to_int( result ))
    {
        free( element );
        return result;
    }
    result = self.state->handlers.foreach(
            &( self.state->handlers ),
            adding_callback,
            element
        );
    /* if handler already exists result will be EEXIST */
    if(
        ( 0 == ulog_status_to_int( result ))
        || ( ENODATA == ulog_status_to_int( result ))
    )
    {
        result =
            self.state->handlers.add( &( self.state->handlers ), element );
    }
    UNUSED( self.state->guard.mutex.unlock( self.state->guard.mutex ));
    if( 0 != ulog_status_to_int( result ))
    {
        free( element );
    }
    return result;
}

/*
 * handler is what we search for
 * pointer will be list element containing handler, returned from foreach
 */
typedef struct
{
    ulog_log_handler handler;
    void * pointer;
}
removal_userdata;

static ulog_status
removal_callback( void * const pointer, void * const userdata )
{
    pointer_list_element const * const element = pointer;
    removal_userdata * const data = userdata;

    /* exactly one element will be equal */
    if( element->handler == data->handler )
    {
        data->pointer = pointer;
    }
    return ulog_status_from_int( 0 );
}

static ulog_status
remove( ulog_obj const self, ulog_log_handler const handler )
{
    if( NULL == self.state )
    {
        return ulog_status_from_int( EINVAL );
    }

    removal_userdata data =
    {
        .handler = handler,
        .pointer = NULL
    };

    ulog_status result =
        self.state->guard.mutex.lock( self.state->guard.mutex );
    if( 0 != ulog_status_to_int( result ))
    {
        return result;
    }
    result = self.state->handlers.foreach(
        &( self.state->handlers ),
        removal_callback,
        &data
    );
    if( 0 == ulog_status_to_int( result ))
    {
        /* data.pointer now contains pointer to remove from list */
        result = self.state->handlers.remove(
            &( self.state->handlers ),
            data.pointer
        );
        if( 0 == ulog_status_to_int( result ))
        {
            free( data.pointer );
        }
    }
    UNUSED( self.state->guard.mutex.unlock( self.state->guard.mutex ));
    return result;
}

static ulog_obj const *
get_global_ulog_obj( void )
{
    static ulog_obj_state state =
    {
        .initialized = false
    };

    static ulog_obj ulog =
    {
        .state = &state,
        .add = add,
        .remove = remove
    };

    return &ulog;
}

typedef struct
{
    ulog_level level;
    char const * format;
    va_list args;
}
callback_userdata;

/*
 * since we control what gets added to the pointer list,
 * we can omit most of the error checks
 */
static ulog_status
handler_callback( void * const pointer, void * const userdata )
{
    pointer_list_element const * const element = pointer;
    callback_userdata * data = userdata;

    va_list args;
    va_copy( args, data->args );
    element->handler( data->level, data->format, args );
    va_end( args );
    return ulog_status_from_int( 0 );
}

/* uses static variable log, won't modify it, except for using mutex */
INDIRECT void
ulog( ulog_level const level, char const * const format, ... )
{
    if( false == get_global_ulog_obj()->state->initialized )
    {
        return;
    }

    callback_userdata data =
    {
        .level = level,
        .format = format,
    };
    va_start( data.args, format );

    ulog_status result =
        get_global_ulog_obj()->state->guard.mutex.lock(
            get_global_ulog_obj()->state->guard.mutex
        );
    if( 0 == ulog_status_to_int( result ))
    {
        result = get_global_ulog_obj()->state->handlers.foreach(
            &( get_global_ulog_obj()->state->handlers ),
            handler_callback,
            &data
        );
        assert(
            ( 0 == ulog_status_to_int( result ))
            || ( ENODATA == ulog_status_to_int( result ))
        );
        UNUSED( get_global_ulog_obj()->state->guard.mutex.unlock(
            get_global_ulog_obj()->state->guard.mutex
        ));
    }

    va_end( data.args );
}

THREADUNSAFE ulog_ctrl
ulog_setup( void )
{
    if( true == get_global_ulog_obj()->state->initialized )
    {
        return ( ulog_ctrl )
        {
            .status = ulog_status_from_int( EALREADY ),
            .log = *( get_global_ulog_obj() )
        };
    }

    ulog_mutex_ctrl const guard = ulog_mutex_setup();
    if( 0 != ulog_status_to_int( guard.status ))
    {
        return ( ulog_ctrl )
        {
            .status = guard.status,
            .log = ( ulog_obj ) { NULL, }
        };
    }
    ulog_pointer_list const handlers = ulog_pointer_list_setup();

    *( get_global_ulog_obj()->state ) = ( ulog_obj_state )
    {
        .initialized = true,
        .handlers = handlers,
        .guard = guard
    };
    return ( ulog_ctrl )
    {
        .status = ulog_status_from_int( 0 ),
        .log = *( get_global_ulog_obj() )
    };
}

static ulog_status
free_callback( void * const pointer, void * const userdata )
{
    UNUSED( userdata );
    free( pointer );
    return ulog_status_from_int( 0 );
}

THREADUNSAFE ulog_status
ulog_cleanup( ulog_ctrl const ctrl )
{
    if(
        ( 0 != ulog_status_to_int( ctrl.status ))
        && ( EALREADY != ulog_status_to_int( ctrl.status ))
    )
    {
        return ulog_status_from_int( EINVAL );
    }
    if( false == ctrl.log.state->initialized )
    {
        return ulog_status_from_int( EALREADY );
    }

    ulog_status result =
        ctrl.log.state->guard.mutex.lock( ctrl.log.state->guard.mutex );
    if( 0 != ulog_status_to_int( result ))
    {
        return result;
    }
    result = ctrl.log.state->handlers.foreach(
        &( ctrl.log.state->handlers ),
        free_callback,
        NULL
    );
    assert(
        ( 0 == ulog_status_to_int( result ))
        || ( ENODATA == ulog_status_to_int( result ))
    );
    result = ulog_pointer_list_cleanup( &( ctrl.log.state->handlers ));
    UNUSED( ctrl.log.state->guard.mutex.unlock( ctrl.log.state->guard.mutex ));
    if(
        ( 0 == ulog_status_to_int( result ))
        || ( ENODATA == ulog_status_to_int( result ))
    )
    {
        result = ulog_mutex_cleanup( ctrl.log.state->guard );
    }
    ctrl.log.state->initialized =
        ( 0 != ulog_status_to_int( result ));
    return result;
}

