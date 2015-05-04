/**
 * \author      Mateusz Jemielity matthew.jemielity@gmail.com
 * \brief       C version-specific implementation of simple mutex wrapper.
 * \date        2015/04/24 17:16:23 AM
 * \file        mutex.c
 * \version     1.0
 *
 *
 **/

#include <errno.h> /* EINVAL, EIO, ENOMEM */
#include <stddef.h> /* NULL */
#include <stdlib.h>
#if __STDC_NO_THREADS__
# include <pthread.h>
#else /* !__STDC_NO_THREADS__ */
# include <threads.h>
#endif /* __STDC_NO_THREADS__ */
#include <ulog/mutex.h>
#include <ulog/status.h>

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

static ulog_status
operation( ulog_mutex const self, internal_mutex_op const internal_operation )
{
    if( NULL == self.state )
    {
        return ulog_status_from_int( EINVAL );
    }

    switch( internal_operation( &( self.state->mutex )))
    {
#if __STDC_NO_THREADS__
        case 0:
#else /* !__STDC_NO_THREADS__ */
        case thrd_success:
#endif /* __STDC_NO_THREADS__ */
            return ulog_status_from_int( 0 );
        default:
            return ulog_status_from_int( EIO );
    }
}

static ulog_status
lock( ulog_mutex const self )
{
    return operation( self, lock_operation );
}

static ulog_status
unlock( ulog_mutex const self )
{
    return operation( self, unlock_operation );
}

ulog_mutex_ctrl
ulog_mutex_setup( void )
{
    ulog_mutex_state * const state = malloc( sizeof( ulog_mutex_state ));
    ulog_mutex self =
    {
        .state = state,
        .lock = lock,
        .unlock = unlock
    };

    if( NULL == self.state )
    {
        return ( ulog_mutex_ctrl )
        {
            .status = ulog_status_from_int( ENOMEM ),
            .mutex = ( ulog_mutex ) { NULL, }
        };
    }

#if __STDC_NO_THREADS__
    int const result = pthread_mutex_init( &( self.state->mutex ), NULL );
#else /* !__STDC_NO_THREADS__ */
    int const result = mtx_init( &( self.state->mutex ), mtx_plain );
#endif /* __STDC_NO_THREADS__ */
    switch( result )
    {
#if __STDC_NO_THREADS__
        case 0:
#else /* !__STDC_NO_THREADS__ */
        case thrd_success:
#endif /* __STDC_NO_THREADS__ */
            return ( ulog_mutex_ctrl )
            {
                .status = ulog_status_from_int( 0 ),
                .mutex = self
            };
        default:
            free( self.state );
            return ( ulog_mutex_ctrl )
            {
                .status = ulog_status_from_int( EIO ),
                .mutex = ( ulog_mutex ) { NULL, }
            };
    }
}

ulog_status
ulog_mutex_cleanup( ulog_mutex_ctrl const ctrl )
{
    if( 0 != ulog_status_to_int( ctrl.status ))
    {
        return ulog_status_from_int( EINVAL );
    }

    /*
     * in case of pthreads (and possibly C11 threads, since they seem to use
     * pthreads) the implementation should allow safe destruction of mutex
     * right after it has been unlocked
     */
    ctrl.mutex.lock( ctrl.mutex );
    ctrl.mutex.unlock( ctrl.mutex );
#if __STDC_NO_THREADS__
    if( 0 != pthread_mutex_destroy( &( ctrl.mutex.state->mutex )))
    {
        return ulog_status_from_int( EIO );
    }
#else /* !__STDC_NO_THREADS__ */
    mtx_destroy( &( ctrl.mutex.state->mutex ));
#endif /* __STDC_NO_THREADS__ */
    free( ctrl.mutex.state );
    return ulog_status_from_int( 0 );
}

