/**
 * \author      Mateusz Jemielity matthew.jemielity@gmail.com
 * \brief       Test mutex unlock #01
 * \date        08/30/2015 12:55:19 AM
 * \file        test_mutex_unlock_01.c
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
    assert( EINVAL == ulog_status_to_int( mutex.op->unlock( NULL )));
    assert( EINVAL == ulog_status_to_int( mutex.op->unlock( &mutex )));
    assert( ulog_status_success( mutex.op->setup( &mutex )));

    assert( ulog_status_success( mutex.op->lock( &mutex )));
    assert( ulog_status_success( mutex.op->unlock( &mutex )));

    assert( ulog_status_success( mutex.op->cleanup( &mutex )));
    return 0;
}

