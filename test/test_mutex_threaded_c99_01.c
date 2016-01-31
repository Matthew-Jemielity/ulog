/**
 * \author      Mateusz Jemielity matthew.jemielity@gmail.com
 * \brief       Test mutex threaded C99/POSIX #01
 * \date        08/30/2015 11:15:27 AM
 * \file        test_mutex_threaded_c99_01.c
 * \version     1.0
 *
 *
 **/

#include <ulog/mutex.h>
#include <ulog/status.h> /* ulog_status_to_int */

#include <assert.h> /* assert */
#include <pthread.h>
#include <stddef.h> /* NULL */
#include <stdint.h> /* intptr_t */

static unsigned resource;
static unsigned loops = 1000000U;

void * foo( void * arg )
{
  ulog_mutex * mutex = arg;
  for( unsigned i = 0; i < loops; ++i )
  {
    assert( ulog_status_success( mutex->op->lock( mutex )));
    ++resource;
    assert( ulog_status_success( mutex->op->unlock( mutex )));
  }
  return NULL;
}

int main(void)
{
    ulog_mutex mutex = ulog_mutex_get();
    assert( ulog_status_success( mutex.op->setup( &mutex )));

    pthread_t t1, t2, t3, t4, t5, t6, t7, t8, t9, t10;
    assert( 0 == pthread_create( &t1, NULL, foo, &mutex ));
    assert( 0 == pthread_create( &t2, NULL, foo, &mutex ));
    assert( 0 == pthread_create( &t3, NULL, foo, &mutex ));
    assert( 0 == pthread_create( &t4, NULL, foo, &mutex ));
    assert( 0 == pthread_create( &t5, NULL, foo, &mutex ));
    assert( 0 == pthread_create( &t6, NULL, foo, &mutex ));
    assert( 0 == pthread_create( &t7, NULL, foo, &mutex ));
    assert( 0 == pthread_create( &t8, NULL, foo, &mutex ));
    assert( 0 == pthread_create( &t9, NULL, foo, &mutex ));
    assert( 0 == pthread_create( &t10, NULL, foo, &mutex ));
    assert( 0 == pthread_join( t1, NULL ));
    assert( 0 == pthread_join( t2, NULL ));
    assert( 0 == pthread_join( t3, NULL ));
    assert( 0 == pthread_join( t4, NULL ));
    assert( 0 == pthread_join( t5, NULL ));
    assert( 0 == pthread_join( t6, NULL ));
    assert( 0 == pthread_join( t7, NULL ));
    assert( 0 == pthread_join( t8, NULL ));
    assert( 0 == pthread_join( t9, NULL ));
    assert( 0 == pthread_join( t10, NULL ));

    assert(( 10 * loops ) == resource );

    assert( ulog_status_success( mutex.op->cleanup( &mutex )));
    return 0;
}

