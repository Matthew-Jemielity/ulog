/**
 * \author      Mateusz Jemielity matthew.jemielity@gmail.com
 * \brief       Checks whether callback is executed.
 * \date        04/25/2015 07:39:13 AM
 * \file        test_call_01.c
 * \version     1.0
 *
 *
 **/

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <ulog/status.h>
#include <ulog/ulog.h>

static int check = 0;

void
dummy_log(
    ulog_level const level,
    char const * const format,
    va_list args
)
{
    ( void ) level;
    ( void ) format;
    ( void ) args;
    check = 1;
}

int
main( void )
{
    ulog_obj const * const ulog = ulog_obj_get();
    assert( ulog_status_success( ulog->op->setup( ulog )));
    assert( ulog_status_success( ulog->op->add( ulog, dummy_log )));

    assert( 0 == check );
    UERROR( "" );
    assert( 1 == check );

    assert( ulog_status_success( ulog->op->cleanup( ulog )));
    return 0;
}

