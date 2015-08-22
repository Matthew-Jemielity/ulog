/**
 * \author      Mateusz Jemielity matthew.jemielity@gmail.com
 * \brief       Basic test of libulog functionality.
 * \date        04/25/2015 07:39:13 AM
 * \file        test_simple_02.c
 * \version     1.0
 *
 *
 **/

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <ulog/status.h>
#include <ulog/ulog.h>

void
log_to_stderr(
    ulog_level const level,
    char const * const format,
    va_list args
)
{
    ( void ) level;
    ( void ) vfprintf( stderr, format, args );
    ( void ) fflush( stderr );
}

void
log_to_stdout(
    ulog_level const level,
    char const * const format,
    va_list args
)
{
    ( void ) level;
    ( void ) vfprintf( stdout, format, args );
    ( void ) fflush( stdout );
}

int
main( void )
{
    ulog_ctrl ctrl = ulog_setup();
    assert( 0 == ulog_status_to_int( ctrl.status ));
    assert( 0 == ulog_status_to_int( ctrl.log.add( ctrl.log, log_to_stderr )));
    assert( 0 == ulog_status_to_int( ctrl.log.add( ctrl.log, log_to_stdout )));

    /* test empty call */
    UERROR();
    UWARNING();
    UINFO();
    UDEBUG();

    /* test call with empty string */
    UERROR("");
    UWARNING("");
    UINFO("");
    UDEBUG("");

    assert( 0 == ulog_status_to_int( ulog_cleanup( ctrl )));
    return 0;
}

