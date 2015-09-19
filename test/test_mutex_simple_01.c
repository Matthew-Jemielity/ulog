/**
 * \author      Mateusz Jemielity matthew.jemielity@gmail.com
 * \brief       Test mutex #01
 * \date        08/29/2015 07:42:24 PM
 * \file        test_mutex_simple_01.c
 * \version     1.0
 *
 *
 **/

#include <ulog/mutex.h>
#include <ulog/status.h> /* ulog_status_to_int */

#include <assert.h> /* assert */
#include <stdint.h> /* intptr_t */

int main(void)
{
    ulog_mutex mutex = ulog_mutex_get();
    intptr_t const default_state = ( intptr_t ) mutex.op;

    assert( ulog_status_success( mutex.op->setup( &mutex )));
    assert( default_state != ( intptr_t ) mutex.op );
    intptr_t state = ( intptr_t )  mutex.op;

    assert( ulog_status_success( mutex.op->lock( &mutex )));
    assert( state == ( intptr_t ) mutex.op );

    assert( ulog_status_success( mutex.op->unlock( &mutex )));
    assert( state == ( intptr_t ) mutex.op );

    assert( ulog_status_success( mutex.op->cleanup( &mutex )));
    assert( state != ( intptr_t ) mutex.op );
    assert( default_state == ( intptr_t ) mutex.op );

    return 0;
}

