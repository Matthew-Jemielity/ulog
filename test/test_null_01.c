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
main( int argc, char * args[] )
{
    ( void ) argc;
    ( void ) args;

    ulog_ctrl ctrl = ulog_setup();
    assert( 0 == ulog_status_to_int( ctrl.status ));

    UERROR( "this shouldn't be printed" );

    assert( 0 == ulog_status_to_int( ulog_cleanup( ctrl )));
    return 0;
}

