/**
 * \author      Mateusz Jemielity matthew.jemielity@gmail.com
 * \brief       Implementation of logging framework operations.
 * \date        2015/09/12 21:56:01 PM
 * \file        ulog.c
 * \version     1.1
 *
 *
 **/

#define _POSIX_C_SOURCE 201509L /* for clock_gettime */

#include <ulog/ulog.h>
#include <ulog/listable.h> /* ulog_listable */
#include <ulog/mutex.h> /* ulog_mutex */
#include <ulog/status.h> /* ulog_status */
#include <ulog/universal.h> /* THREADUNSAFE, UNUSED */

#include <assert.h> /* assert */
#include <errno.h> /* EALREADY, EINVAL, etc. */
#include <stdbool.h> /* bool */
#include <stddef.h> /* NULL */
#include <stdint.h> /* uint64_t */
#include <stdlib.h> /* malloc, free */
#include <time.h> /* clock_gettime, struct timespec */

#define NANOSECONDS_IN_MICROSECOND 1000U
#define MICROSECONDS_IN_MILLISECOND 1000U
#define MILLISECONDS_IN_SECOND 1000U

struct ulog_obj_private_struct
{
    ulog_level verbosity;
    ulog_list_ctrl handlers;
    ulog_mutex guard;
    ulog_obj_op_table const * op;
};

INDIRECT char
ulog_level_to_char_( ulog_level const level )
{
    switch( level )
    {
        case ERROR: return 'E';
        case WARNING: return 'W';
        case INFO: return 'I';
        case DEBUG: return 'D';
        default: return '?';
    }
}

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
ulog_current_time_( void )
{
    struct timespec result;

    if( 0 == clock_gettime( CLOCK_REALTIME, &result ))
    {
        return from_timespec( result );
    }

    return 0U;
}

typedef struct
{
    ulog_handler_fn handler;
    ulog_listable list;
}
handler_list_element;

static handler_list_element *
get_handler_list_element( ulog_listable * const element )
{
    return ulog_listable_get_container( element, handler_list_element, list );
}

static inline bool
is_initialized( ulog_obj const * const self );

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
handler_callback( ulog_listable * const element, void * const userdata )
{
    handler_list_element const * const item =
        get_handler_list_element( element );
    callback_userdata * data = userdata;

    va_list args;
    va_copy( args, data->args );
    item->handler( data->level, data->format, args );
    va_end( args );
    return ulog_status_descriptive( 0, "handler executed successfully" );
}

/* uses static variable log, won't modify it, except for using mutex */
INDIRECT void
ulog_( ulog_level const level, char const * const format, ... )
{
    ulog_obj const * const ulog = ulog_obj_get();
    if( !is_initialized( ulog )) { return; }
    if( level > ulog->state->verbosity ) { return; }

    callback_userdata data = { .level = level, .format = format };
    va_start( data.args, format );

    ulog_status result = ulog->state->guard.op->lock( &( ulog->state->guard ));
    if( !ulog_status_success( result )) { goto guard_failure; }
    result =
        ulog->state->handlers.op->foreach(
            &( ulog->state->handlers ),
            handler_callback,
            &data
        );
    UNUSED( ulog->state->guard.op->unlock( &( ulog->state->guard )));
guard_failure:
    va_end( data.args );
}

static inline ulog_status
generic_invalid( ulog_obj const * const self )
{
    UNUSED( self );
    return ulog_status_descriptive( EINVAL, "invalid ulog_obj object" );
}

static inline ulog_status
generic_uninitialized( ulog_obj const * const self )
{
    UNUSED( self );
    return
        ulog_status_descriptive( ENOTCONN, "ulog framework not initialized" );
}

static inline ulog_status
generic_ulog_obj_op_uninitialized(
    ulog_obj const * const self,
    ulog_handler_fn const handler
)
{
    UNUSED( handler );
    return generic_uninitialized( self );
}

static inline ulog_status
verbosity_uninitialized(
    ulog_obj const * const self,
    ulog_level const verbosity
)
{
    UNUSED( verbosity );
    return generic_uninitialized( self );
}

static inline ulog_status
generic_already( ulog_obj const * const self, char const * const message )
{
    UNUSED( self );
    return ulog_status_descriptive( EALREADY, message );
}

static inline ulog_status
setup_already( ulog_obj const * const self )
{
    return generic_already( self, "ulog framework already set up" );
}

static inline ulog_status
cleanup_already( ulog_obj const * const self )
{
    return generic_already( self, "ulog framework already cleaned up" );
}

static ulog_status
adding_callback( ulog_listable * const element, void * const userdata )
{
    handler_list_element const * const item =
        get_handler_list_element( element );
    handler_list_element const * const addition = userdata;

    return
        ( item->handler == addition->handler ) ?
            ulog_status_descriptive( EEXIST, "handler already on list")
            : ulog_status_descriptive( 0, "handler can be added to list" );
}

static ulog_status
add_internal( ulog_obj const * const self, ulog_handler_fn const handler )
{
    handler_list_element * const element =
        malloc( sizeof( handler_list_element ));
    if( NULL == element )
    {
        return
          ulog_status_descriptive(
              ENOMEM,
              "cannot allocate handler list element"
          );
    }

    element->handler = handler;
    element->list = ulog_listable_get();

    ulog_status result =
        self->state->guard.op->lock( &( self->state->guard ));
    if( !ulog_status_success( result ))
    {
        free( element );
        return result;
    }
    result = self->state->handlers.op->foreach(
            &( self->state->handlers ),
            adding_callback,
            element
        );
    /* if handler already exists result will be EEXIST */
    if(
        ( ulog_status_success( result ))
        || ( ENOENT == ulog_status_to_int( result ))
    )
    {
        result =
            self->state->handlers.op->add(
                &( self->state->handlers ),
                &( element->list )
            );
    }
    UNUSED( self->state->guard.op->unlock( &( self->state->guard )));
    if( !ulog_status_success( result )) {
      free( element );
      return result;
    }
    return ulog_status_descriptive( 0, "handler added successfully" );
}

/*
 * handler is what we search for
 * element will be list element containing handler, returned from foreach
 */
typedef struct
{
    ulog_handler_fn handler;
    ulog_listable * element;
}
removal_userdata;

static ulog_status
removal_callback( ulog_listable * const element, void * const userdata )
{
    handler_list_element const * const item =
        get_handler_list_element( element );
    removal_userdata * const data = userdata;

    /* exactly one element will be equal */
    if( item->handler == data->handler ) { data->element = element; }
    return ulog_status_descriptive( 0, "handled list element" );
}

static ulog_status
remove_internal( ulog_obj const * const self, ulog_handler_fn const handler )
{
    removal_userdata data =
    {
        .handler = handler,
        .element = NULL
    };

    ulog_status result = self->state->guard.op->lock( &( self->state->guard ));
    if( !ulog_status_success( result )) { return result; }
    result = self->state->handlers.op->foreach(
        &( self->state->handlers ),
        removal_callback,
        &data
    );
    if( ulog_status_success( result ))
    {
        /* data.pointer now contains pointer to remove from list */
        result = self->state->handlers.op->remove(
            &( self->state->handlers ),
            data.element
        );
        if( ulog_status_success( result ))
        {
            free( get_handler_list_element( data.element ));
        }
    }
    UNUSED( self->state->guard.op->unlock( &( self->state->guard )));
    if( !ulog_status_success( result )) { return result; }
    return ulog_status_descriptive( 0, "handler removed successfully" );
}

static ulog_status
verbosity_internal( ulog_obj const * const self, ulog_level const verbosity )
{
    switch( verbosity )
    {
        case ERROR: break;
        case WARNING: break;
        case INFO: break;
        case DEBUG: break;
        default:
            return ulog_status_descriptive( ENODATA, "invalid verbosity" );
    }

    ulog_status result =
        self->state->guard.op->lock( &( self->state->guard ));
    if( !ulog_status_success( result )) { return result; }
    self->state->verbosity = verbosity;
    result = self->state->guard.op->unlock( &( self->state->guard ));
    if( !ulog_status_success( result )) { return result; }
    return ulog_status_descriptive( 0, "verbosity level set up successfully" );
}

static THREADUNSAFE ulog_status
setup_internal( ulog_obj const * const self );
static THREADUNSAFE ulog_status
cleanup_internal( ulog_obj const * const self );

static ulog_obj_op_table const default_state =
{
    .setup = setup_internal,
    .cleanup = cleanup_already,
    .add = generic_ulog_obj_op_uninitialized,
    .remove = generic_ulog_obj_op_uninitialized,
    .verbosity = verbosity_uninitialized
};
static ulog_obj_op_table const setup_state =
{
    .setup = setup_already,
    .cleanup = cleanup_internal,
    .add = add_internal,
    .remove = remove_internal,
    .verbosity = verbosity_internal
};

static inline bool
is_initialized( ulog_obj const * const self )
{
    return ( &setup_state == self->state->op );
}

static THREADUNSAFE ulog_status
setup_internal( ulog_obj const * const self )
{
    self->state->guard = ulog_mutex_get();
    ulog_status const guard_status =
        self->state->guard.op->setup( &( self->state->guard ));
    if( !ulog_status_success( guard_status )) { return guard_status; }

    self->state->handlers = ulog_list_ctrl_get();
    self->state->verbosity = DEBUG;
    self->state->op = &setup_state;

    return ulog_status_descriptive( 0, "ulog framework set up successfully" );
}

static THREADUNSAFE ulog_status
cleanup_internal( ulog_obj const * const self )
{
    ulog_status result =
        self->state->guard.op->lock( &( self->state->guard ));
    if( !ulog_status_success( result )) { return result; }
    ulog_list_ctrl * const ctrl = &( self->state->handlers );
    ulog_listable * element = ctrl->head;
    while( ulog_status_success( ctrl->op->remove( ctrl, ctrl->head )))
    {
        free( get_handler_list_element( element ));
        element = ctrl->head;
    }
    UNUSED( self->state->guard.op->unlock( &( self->state->guard )));
    result = self->state->guard.op->cleanup( &( self->state->guard ));
    if( !ulog_status_success( result )) { return result; }
    self->state->op = &default_state;
    return
        ulog_status_descriptive( 0, "ulog framework cleaned up successfully" );
}

static inline bool
valid( ulog_obj const * const self );

static inline THREADUNSAFE ulog_status
setup( ulog_obj const * const self )
{
    if( !valid( self )) { return generic_invalid( self ); }
    return self->state->op->setup( self );
}

static inline THREADUNSAFE ulog_status
cleanup( ulog_obj const * const self )
{
    if( !valid( self )) { return generic_invalid( self ); }
    return self->state->op->cleanup( self );
}

static inline ulog_status
add( ulog_obj const * const self, ulog_handler_fn const handler )
{
    if( !valid( self )) { return generic_invalid( self ); }
    return self->state->op->add( self, handler );
}

static inline ulog_status
remove( ulog_obj const * const self, ulog_handler_fn const handler )
{
    if( !valid( self )) { return generic_invalid( self ); }
    return self->state->op->remove( self, handler );
}

static inline ulog_status
verbosity_( ulog_obj const * const self, ulog_level const verbosity )
{
    if( !valid( self )) { return generic_invalid( self ); }
    return self->state->op->verbosity( self, verbosity );
}

static ulog_obj_op_table const op_table =
{
    .setup = setup,
    .cleanup = cleanup,
    .add = add,
    .remove = remove,
    .verbosity = verbosity_
};

static ulog_obj_private state = { .op = &default_state };

static inline bool
valid( ulog_obj const * const self )
{
    return (
        ( NULL != self )
        && ( &state == self->state )
        && ( &op_table == self->op )
    );
}

ulog_obj const *
ulog_obj_get( void )
{
    assert( ERROR < WARNING );
    assert( WARNING < INFO );
    assert( INFO < DEBUG );

    static ulog_obj const ulog =
    {
        .state = &state,
        .op = &op_table
    };

    return &ulog;
}

