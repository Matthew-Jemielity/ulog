/**
 * \author      Mateusz Jemielity matthew.jemielity@gmail.com
 * \brief       Test mutex cleanup #01
 * \date        08/30/2015 12:24:07 AM
 * \file        test_mutex_cleanup_01.c
 * \version     1.0
 *
 *
 **/

#include <ulog/mutex.h>
#include <ulog/status.h> /* ulog_status_to_int */

#include <assert.h> /* assert */
#include <errno.h> /* EALREADY, EBUSY, EINVAL */
#include <stddef.h> /* NULL */
#include <stdint.h> /* intptr_t */

int main(void)
{
    ulog_mutex mutex = ulog_mutex_get();

    assert( EINVAL == ulog_status_to_int( mutex.op->cleanup( NULL )));
    assert( EALREADY == ulog_status_to_int( mutex.op->cleanup( &mutex )));

    assert( ulog_status_success( mutex.op->setup( &mutex )));

    assert( ulog_status_success( mutex.op->cleanup( &mutex )));
    assert( EALREADY == ulog_status_to_int( mutex.op->cleanup( &mutex )));

    return 0;
}

