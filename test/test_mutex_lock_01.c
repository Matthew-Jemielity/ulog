/**
 * \author      Mateusz Jemielity matthew.jemielity@gmail.com
 * \brief       Test mutex lock #01
 * \date        08/30/2015 12:38:41 AM
 * \file        test_mutex_lock_01.c
 * \version     1.0
 *
 *
 **/

#include <ulog/mutex.h>
#include <ulog/status.h> /* ulog_status_to_int */

#include <assert.h> /* assert */
#include <errno.h> /* EALREADY, EINVAL */
#include <stddef.h> /* NULL */
#include <stdint.h> /* intptr_t */

int main(void)
{
    ulog_mutex mutex = ulog_mutex_get();
    assert( EINVAL == ulog_status_to_int( mutex.op->lock( NULL )));
    assert( EINVAL == ulog_status_to_int( mutex.op->lock( &mutex )));
    assert( 0 == ulog_status_to_int( mutex.op->setup( &mutex )));
    intptr_t const setup_state = ( intptr_t ) mutex.op;

    assert( 0 == ulog_status_to_int( mutex.op->lock( &mutex )));
    assert( setup_state == ( intptr_t ) mutex.op );

    assert( 0 == ulog_status_to_int( mutex.op->unlock( &mutex )));
    assert( setup_state == ( intptr_t ) mutex.op );
    assert( 0 == ulog_status_to_int( mutex.op->lock( &mutex )));
    assert( 0 == ulog_status_to_int( mutex.op->unlock( &mutex )));

    assert( 0 == ulog_status_to_int( mutex.op->cleanup( &mutex )));
    return 0;
}

