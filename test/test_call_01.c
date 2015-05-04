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

int check = 0;

void dummy_log(
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

int main( int argc, char * args[] )
{
    ( void ) argc;
    ( void ) args;

    ulog_ctrl ctrl = ulog_setup();
    assert( 0 == ulog_status_to_int( ctrl.status ));
    assert( 0 == ulog_status_to_int( ctrl.log.add( ctrl.log, dummy_log )));

    assert( 0 == check );
    UERROR( "" );
    assert( 1 == check );

    assert( 0 == ulog_status_to_int( ulog_cleanup( ctrl )));
    return 0;
}

