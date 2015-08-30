/**
 * \author      Mateusz Jemielity matthew.jemielity@gmail.com
 * \brief       C99/C11 implementation of simple mutex wrapper.
 * \date        2015/08/29 17:16:23 AM
 * \file        mutex.c
 * \version     1.1
 *
 *
 **/

#include <errno.h> /* EALREADY, EBUSY, EINVAL, EIO, ENOMEM */
#include <stddef.h> /* NULL */
#include <stdlib.h> /* free, malloc */
#if __STDC_NO_THREADS__
# include <pthread.h>
#else /* !__STDC_NO_THREADS__ */
# include <threads.h>
#endif /* __STDC_NO_THREADS__ */
#include <ulog/mutex.h>
#include <ulog/status.h> /* ulog_status, ulog_status_descriptive */
#include <ulog/universal.h> /* THREADUNSAFE, UNUSED */

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
};

typedef int
( * internal_mutex_op )( mutex_obj * mutex );

static internal_mutex_op const lock_operation =
#if __STDC_NO_THREADS__
    pthread_mutex_lock
#else /* !__STDC_NO_THREADS__ */
    mtx_lock
#endif /* __STDC_NO_THREADS__ */
;
static internal_mutex_op const unlock_operation =
#if __STDC_NO_THREADS__
    pthread_mutex_unlock
#else /* !__STDC_NO_THREADS__ */
    mtx_unlock
#endif /* __STDC_NO_THREADS__ */
;
/*
 * The state machine could be made safer with additional locked state.
 * However this would require that ulog_mutex object was shallow - copied
 * into every thread that wanted to use it. This meant it couldn't be held
 * in opaque pointers and would change how object containing it was used.
 * After long deliberation ease of use was chosen over safety.
 */

static inline ulog_status
generic_invalid( ulog_mutex const * const self )
{
    UNUSED( self );
    return ulog_status_descriptive( EINVAL, "invalid mutex object" );
}

static inline ulog_status
generic_uninitialized( ulog_mutex const * const self )
{
    if( NULL == self ) { return generic_invalid( self ); }
    return ulog_status_descriptive( EINVAL, "mutex object uninitialized" );
}

static inline ulog_status
setup_already( ulog_mutex * const self )
{
    if( NULL == self ) { return generic_invalid( self ); }
    return ulog_status_descriptive( EALREADY, "mutex already initialized" );
}

static inline ulog_status
cleanup_already( ulog_mutex * const self )
{
    if( NULL == self ) { return generic_invalid( self ); }
    return ulog_status_descriptive( EALREADY, "mutex already cleaned up" );
}

static THREADUNSAFE ulog_status
setup( ulog_mutex * const self );
static THREADUNSAFE ulog_status
cleanup( ulog_mutex * const self );
static ulog_status
lock( ulog_mutex const * const self );
static ulog_status
unlock( ulog_mutex const * const self );

static ulog_mutex_op_table const default_state = {
    .setup = setup,
    .cleanup = cleanup_already,
    .lock = generic_uninitialized,
    .unlock = generic_uninitialized
};
static ulog_mutex_op_table const setup_state = {
    .setup = setup_already,
    .cleanup = cleanup,
    .lock = lock,
    .unlock = unlock
};

THREADUNSAFE ulog_status
setup( ulog_mutex * const self )
{
    if( NULL == self ) { return generic_invalid( self ); }
    ulog_mutex_state * const state = malloc( sizeof( ulog_mutex_state ));
    if( NULL == state )
    {
        return ulog_status_descriptive(
            ENOMEM,
            "cannot allocate memory for mutex state"
        );
    }

    int const result =
#if __STDC_NO_THREADS__
        pthread_mutex_init( &( state->mutex ), NULL )
#else /* !__STDC_NO_THREADS__ */
        mtx_init( &( state->mutex ), mtx_plain )
#endif /* __STDC_NO_THREADS__ */
    ;

    switch( result )
    {
#if __STDC_NO_THREADS__
        case 0:
#else /* !__STDC_NO_THREADS__ */
        case thrd_success:
#endif /* __STDC_NO_THREADS__ */
            self->state = state;
            self->op = &setup_state;
            return ulog_status_descriptive( 0, "mutex set up successfully" );
        default:
            free( state );
            return ulog_status_descriptive(
                EIO,
                "failure setting up the underlying mutex mechanism"
            );
    }
}

THREADUNSAFE ulog_status
cleanup( ulog_mutex * const self )
{
    if( NULL == self ) { return generic_invalid( self ); }
    /*
     * in case of pthreads (and possibly C11 threads, since they seem to use
     * pthreads) the implementation should allow safe destruction of mutex
     * right after it has been unlocked
     */
    self->op->lock( self );
    self->op->unlock( self );
#if __STDC_NO_THREADS__
    if( 0 != pthread_mutex_destroy( &( self->state->mutex )))
    {
        return ulog_status_descriptive(
            EIO,
            "failure cleaning up the underlying mutex mechanism"
        );
    }
#else /* !__STDC_NO_THREADS__ */
    mtx_destroy( &( self->state->mutex ));
#endif /* __STDC_NO_THREADS__ */
    free( self->state );
    self->state = NULL;
    self->op = &default_state;
    return ulog_status_descriptive( 0, "mutex cleaned up successfully" );
}

static ulog_status
lock( ulog_mutex const * const self )
{
    if( NULL == self ) { return generic_invalid( self ); }
    switch( lock_operation( &( self->state->mutex )))
    {
#if __STDC_NO_THREADS__
        case 0:
#else /* !__STDC_NO_THREADS__ */
        case thrd_success:
#endif /* __STDC_NO_THREADS__ */
            return ulog_status_descriptive( 0, "mutex locked successfully" );
        default:
            return ulog_status_descriptive(
                EIO,
                "error locking the underlying mutex mechanism"
            );
    }
}

static ulog_status
unlock( ulog_mutex const * const self )
{
    if( NULL == self ) { return generic_invalid( self ); }
    switch( unlock_operation( &( self->state->mutex )))
    {
#if __STDC_NO_THREADS__
        case 0:
#else /* !__STDC_NO_THREADS__ */
        case thrd_success:
#endif /* __STDC_NO_THREADS__ */
            return ulog_status_descriptive( 0, "mutex unlocked successfully" );
        default:
            return ulog_status_descriptive(
                EIO,
                "error unlocking the underlying mutex mechanism"
            );
    }
}

ulog_mutex
ulog_mutex_get( void )
{
  return ( ulog_mutex ) { .state = NULL, .op = &default_state };
}

