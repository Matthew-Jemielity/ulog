/**
 * \author      Mateusz Jemielity matthew.jemielity@gmail.com
 * \brief       C99/C11 implementation of simple mutex wrapper.
 * \date        2015/08/29 17:16:23 AM
 * \file        mutex.c
 * \version     1.2
 *
 *
 **/

#include <ulog/mutex.h>
#include <ulog/status.h> /* ulog_status, ulog_status_descriptive */
#include <ulog/universal.h> /* THREADUNSAFE, UNUSED */

#include <errno.h> /* EALREADY, EBUSY, EINVAL, EIO, ENOMEM */
#include <stdbool.h> /* bool */
#include <stddef.h> /* NULL */
#include <stdlib.h> /* free, malloc */
#if __STDC_NO_THREADS__
# include <pthread.h>
#else /* !__STDC_NO_THREADS__ */
# include <threads.h>
#endif /* __STDC_NO_THREADS__ */

#if __STDC_NO_THREADS__
# define ULOG_MUTEX_SUCCESS 0
#else /* !__STDC_NO_THREADS__ */
# define ULOG_MUTEX_SUCCESS thrd_success
#endif /* __STDC_NO_THREADS__ */

typedef
#if __STDC_NO_THREADS__
    pthread_mutex_t
#else /* !__STDC_NO_THREADS__ */
    mtx_t
#endif /* __STDC_NO_THREADS__ */
mutex_obj;

struct ulog_mutex_state_struct
{
    mutex_obj mutex;
    ulog_mutex_op_table const * op;
};

typedef
#if __STDC_NO_THREADS__
    pthread_mutexattr_t const *
#else /* !__STDC_NO_THREADS__ */
    int
#endif /* __STDC_NO_THREADS__ */
internal_mutex_ctrl_arg;

typedef int
( * internal_mutex_ctrl )(
    mutex_obj * mutex,
    internal_mutex_ctrl_arg arg
);

static internal_mutex_ctrl const ctrl_init_operation =
#if __STDC_NO_THREADS__
    pthread_mutex_init
#else /* !__STDC_NO_THREADS__ */
    mtx_init
#endif /* __STDC_NO_THREADS__ */
;
static internal_mutex_ctrl_arg const default_init_arg =
#if __STDC_NO_THREADS__
    NULL
#else /* !__STDC_NO_THREADS__ */
    mtx_plain
#endif /* __STDC_NO_THREADS__ */
;

typedef int
( * internal_mutex_op )( mutex_obj * mutex );

static inline int
ulog_mutex_init( mutex_obj * mutex )
{
    return ctrl_init_operation( mutex, default_init_arg );
}

#if !__STDC_NO_THREADS__
static inline int
ulog_mutex_destroy( mutex_obj * mutex )
{
    mtx_destroy( mutex );
    return ULOG_MUTEX_SUCCESS;
}
#endif /* !__STDC_NO_THREADS__ */

typedef struct
{
    internal_mutex_op const op;
    char const * const success;
    char const * const failure;
}
generic_operation_arg;

static generic_operation_arg const generic_arg[ 4U ] =
{
    {
        .op = ulog_mutex_init,
        .success = "mutex set up successfully",
        .failure = "failure setting up the underlying mutex mechanism"
    },
    {
        .op =
#if __STDC_NO_THREADS__
            pthread_mutex_destroy
#else /* !__STDC_NO_THREADS__ */
            ulog_mutex_destroy
#endif /* __STDC_NO_THREADS__ */
        ,
        .success = "mutex cleaned up successfully",
        .failure = "failure cleaning up the underlying mutex mechanism"
    },
    {
        .op =
#if __STDC_NO_THREADS__
          pthread_mutex_lock
#else /* !__STDC_NO_THREADS__ */
          mtx_lock
#endif /* __STDC_NO_THREADS__ */
        ,
        .success = "mutex locked successfully",
        .failure = "error locking the underlying mutex mechanism"
    },
    {
        .op =
#if __STDC_NO_THREADS__
          pthread_mutex_unlock
#else /* !__STDC_NO_THREADS__ */
          mtx_unlock
#endif /* __STDC_NO_THREADS__ */
        ,
        .success = "mutex unlocked successfully",
        .failure = "error unlocking the underlying mutex mechanism"
    }
};

typedef enum
{
  SETUP = 0U,
  CLEANUP = 1U,
  LOCK = 2U,
  UNLOCK = 3U
}
generic_arg_type;

static inline bool
valid( ulog_mutex const * const self );

static inline ulog_status
generic_invalid( ulog_mutex const * const self )
{
    UNUSED( self );
    return ulog_status_descriptive( EINVAL, "invalid mutex object" );
}

static inline ulog_status
generic_uninitialized( ulog_mutex const * const self )
{
    UNUSED( self );
    return ulog_status_descriptive( EINVAL, "mutex object uninitialized" );
}

static inline ulog_status
generic_already( ulog_mutex const * const self, char const * const message )
{
    UNUSED( self );
    return ulog_status_descriptive( EALREADY, message );
}

static inline ulog_status
setup_already( ulog_mutex * const self )
{
    return generic_already( self, "mutex already set up" );
}

static inline ulog_status
cleanup_already( ulog_mutex * const self )
{
    return generic_already( self, "mutex already cleaned up" );
}

static inline ulog_status
generic_operation(
    ulog_mutex const * const self,
    generic_operation_arg const * const arg
)
{
    switch( arg->op( &( self->state->mutex )))
    {
        case ULOG_MUTEX_SUCCESS:
            return ulog_status_descriptive( 0, arg->success );
        default:
            return ulog_status_descriptive( EIO, arg->failure );
    }
}

static THREADUNSAFE ulog_status
setup_safe( ulog_mutex * const self );
static THREADUNSAFE ulog_status
cleanup_safe( ulog_mutex * const self );
static inline ulog_status
lock_safe( ulog_mutex const * const self );
static inline ulog_status
unlock_safe( ulog_mutex const * const self );

static ulog_mutex_op_table const default_op =
{
    .setup = setup_safe,
    .cleanup = cleanup_already,
    .lock = generic_uninitialized,
    .unlock = generic_uninitialized
};
static ulog_mutex_op_table const setup_op =
{
    .setup = setup_already,
    .cleanup = cleanup_safe,
    .lock = lock_safe,
    .unlock = unlock_safe
};
/*
 * it would be nice to have an additional "locked" state
 * however this would at least require atomic op changes
 * additionally trying to lock a locked mutex would fail
 * instead of blocking the calling thread
 */

static ulog_mutex_state guard = { .op = &default_op };

static THREADUNSAFE ulog_status
setup_safe( ulog_mutex * const self )
{
    ulog_mutex_state * const state = malloc( sizeof( ulog_mutex_state ));
    if( NULL == state )
    {
        return ulog_status_descriptive(
            ENOMEM,
            "cannot allocate memory for mutex state"
        );
    }
    self->state = state;
    self->state->op = &setup_op;

    ulog_status const result =
        generic_operation( self, &( generic_arg[ SETUP ] ));
    if( !ulog_status_success( result ))
    {
        free( self->state );
        self->state = &guard;
    }
    return result;
}

static THREADUNSAFE ulog_status
cleanup_safe( ulog_mutex * const self )
{
    /*
     * In case of pthreads (and possibly C11 threads, since they seem to use
     * pthreads) the implementation should allow safe destruction of mutex
     * right after it has been unlocked.
     */
    self->op->lock( self );
    self->op->unlock( self );
    ulog_status const result =
        generic_operation( self, &( generic_arg[ CLEANUP ] ));
    if( ulog_status_success( result ))
    {
        free( self->state );
        self->state = &guard;
    }
    return result;
}

static inline ulog_status
lock_safe( ulog_mutex const * const self )
{
    return generic_operation( self, &( generic_arg[ LOCK ] ));
}

static inline ulog_status
unlock_safe( ulog_mutex const * const self )
{
    return generic_operation( self, &( generic_arg[ UNLOCK ] ));
}

static inline THREADUNSAFE ulog_status
setup( ulog_mutex * const self )
{
    if( !valid( self )) { return generic_invalid( self ); }
    return self->state->op->setup( self );
}

static inline THREADUNSAFE ulog_status
cleanup( ulog_mutex * const self )
{
    if( !valid( self )) { return generic_invalid( self ); }
    return self->state->op->cleanup( self );
}

static inline ulog_status
lock( ulog_mutex const * const self )
{
    if( !valid( self )) { return generic_invalid( self ); }
    return self->state->op->lock( self );
}

static inline ulog_status
unlock( ulog_mutex const * const self )
{
    if( !valid( self )) { return generic_invalid( self ); }
    return self->state->op->unlock( self );
}

static ulog_mutex_op_table const op =
{
    .setup = setup,
    .cleanup = cleanup,
    .lock = lock,
    .unlock = unlock
};

static inline bool
valid( ulog_mutex const * const self )
{
    return
        (
            ( NULL != self )
            && (
                (
                    ( &guard == self->state )
                    && ( &default_op == self->state->op )
                )
                || (
                    ( NULL != self->state )
                    && ( &guard != self->state )
                    && ( &setup_op == self->state->op )
                )
            )
            && ( &op == self->op )
        );
}

ulog_mutex
ulog_mutex_get( void )
{
    return ( ulog_mutex ) { .state = &guard, .op = &op };
}

