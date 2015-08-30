/**
 * \author      Mateusz Jemielity matthew.jemielity@gmail.com
 * \brief       Test mutex setup #01
 * \date        08/29/2015 07:57:30 PM
 * \file        test_mutex_setup_01.c
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
    assert( NULL == mutex.state );
    intptr_t const default_state = ( intptr_t ) mutex.op;
    intptr_t state = default_state;

    assert( EINVAL == ulog_status_to_int( mutex.op->setup( NULL )));

    assert( 0 == ulog_status_to_int( mutex.op->setup( &mutex )));
    assert( NULL != mutex.state );
    assert( default_state != ( intptr_t ) mutex.op );
    state = ( intptr_t ) mutex.op;

    assert( EALREADY == ulog_status_to_int( mutex.op->setup( &mutex )));
    assert( NULL != mutex.state );
    assert( default_state != ( intptr_t ) mutex.op );
    assert( state == ( intptr_t ) mutex.op );

    assert( 0 == ulog_status_to_int( mutex.op->lock( &mutex )));
    assert( EALREADY == ulog_status_to_int( mutex.op->setup( &mutex )));
    assert( NULL != mutex.state );
    assert( default_state != ( intptr_t ) mutex.op );
    assert( state == ( intptr_t ) mutex.op );

    assert( 0 == ulog_status_to_int( mutex.op->unlock( &mutex )));
    assert( EALREADY == ulog_status_to_int( mutex.op->setup( &mutex )));
    assert( NULL != mutex.state );
    assert( default_state != ( intptr_t ) mutex.op );
    assert( state == ( intptr_t ) mutex.op );

    assert( 0 == ulog_status_to_int( mutex.op->cleanup( &mutex )));

    return 0;
}

