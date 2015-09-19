/**
 * \author      Mateusz Jemielity matthew.jemielity@gmail.com
 * \brief       Test ulog_obj's setup() #01
 * \date        09/19/2015 11:38:41 PM
 * \file        test_ulog_obj_setup_01.c
 * \version     1.0
 *
 *
 **/

#include <ulog/status.h>
#include <ulog/ulog.h>

#include <assert.h> /* assert */
#include <errno.h> /* EINVAL, etc. */
#include <stddef.h> /* NULL */

static ulog_obj bad;

int main( void )
{
    ulog_obj const * const ulog = ulog_obj_get();

    assert( EINVAL == ulog_status_to_int( ulog->op->setup( NULL )));
    assert( EINVAL == ulog_status_to_int( ulog->op->setup( &bad )));

    assert( ulog_status_success( ulog->op->setup( ulog )));

    assert( EINVAL == ulog_status_to_int( ulog->op->setup( NULL )));
    assert( EINVAL == ulog_status_to_int( ulog->op->setup( &bad )));

    assert( EALREADY == ulog_status_to_int( ulog->op->setup( ulog )));

    assert( ulog_status_success( ulog->op->cleanup( ulog )));

    assert( EINVAL == ulog_status_to_int( ulog->op->setup( NULL )));
    assert( EINVAL == ulog_status_to_int( ulog->op->setup( &bad )));

    assert( ulog_status_success( ulog->op->setup( ulog )));

    assert( EINVAL == ulog_status_to_int( ulog->op->setup( NULL )));
    assert( EINVAL == ulog_status_to_int( ulog->op->setup( &bad )));

    assert( EALREADY == ulog_status_to_int( ulog->op->setup( ulog )));

    assert( ulog_status_success( ulog->op->cleanup( ulog )));

    return 0;
}

