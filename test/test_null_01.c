/**
 * \author      Mateusz Jemielity matthew.jemielity@gmail.com
 * \brief       Basic test of libulog functionality.
 * \date        04/25/2015 07:39:13 AM
 * \file        test_simple_01.c
 * \version     1.0
 *
 *
 **/

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <ulog/status.h>
#include <ulog/ulog.h>

int
main( void )
{
    ulog_obj const * const ulog = ulog_obj_get();

    UERROR( "this shouldn't be printed" );

    assert( ulog_status_success( ulog->op->setup( ulog )));

    UERROR( "this shouldn't be printed" );

    assert( ulog_status_success( ulog->op->cleanup( ulog )));
    return 0;
}

