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

    assert( EINVAL == ulog_status_to_int( mutex.op->setup( NULL )));

    assert( ulog_status_success( mutex.op->setup( &mutex )));

    assert( EALREADY == ulog_status_to_int( mutex.op->setup( &mutex )));

    assert( ulog_status_success( mutex.op->lock( &mutex )));
    assert( EALREADY == ulog_status_to_int( mutex.op->setup( &mutex )));

    assert( ulog_status_success( mutex.op->unlock( &mutex )));
    assert( EALREADY == ulog_status_to_int( mutex.op->setup( &mutex )));

    assert( ulog_status_success( mutex.op->cleanup( &mutex )));

    return 0;
}

