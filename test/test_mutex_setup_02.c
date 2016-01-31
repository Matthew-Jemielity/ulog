/**
 * \author      Mateusz Jemielity matthew.jemielity@gmail.com
 * \brief       Test mutex setup #02
 * \date        08/30/2015 12:20:14 AM
 * \file        test_mutex_setup_02.c
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

    for( int i = 0; i < 1000000; ++i )
    {
      assert( ulog_status_success( mutex.op->setup( &mutex )));
      assert( ulog_status_success( mutex.op->cleanup( &mutex )));
    }

    return 0;
}

