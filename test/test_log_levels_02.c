/**
 * \author      Mateusz Jemielity matthew.jemielity@gmail.com
 * \brief       Checks whether log levels are ignored properly.
 * \date        04/25/2015 07:39:13 AM
 * \file        test_log_levels_02.c
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
debug_log(
    ulog_level const level,
    char const * const format,
    va_list args
)
{
    ( void ) format;
    ( void ) args;
    assert( DEBUG == level );
}

void
should_not_trigger(
    ulog_level const level,
    char const * const format,
    va_list args
)
{
    ( void ) level;
    ( void ) format;
    ( void ) args;
    assert( 0 );
}

int
main( void )
{
    {
        ulog_ctrl ctrl = ulog_setup(); /* verbosity is DEBUG by default */
        assert( 0 == ulog_status_to_int( ctrl.status ));
        assert( 0 == ulog_status_to_int( ctrl.log.add( ctrl.log, debug_log )));
        UDEBUG();
        assert( 0 == ulog_status_to_int( ulog_cleanup( ctrl )));
    }

    {
        ulog_ctrl ctrl = ulog_setup(); /* verbosity is DEBUG by default */
        assert( 0 == ulog_status_to_int( ctrl.status ));
        assert( 0 == ulog_status_to_int(
                ctrl.log.add( ctrl.log, should_not_trigger )));
        assert( 0 == ulog_status_to_int(
                ctrl.log.verbosity( ctrl.log, INFO )));
        UDEBUG();
        assert( 0 == ulog_status_to_int(
                ctrl.log.verbosity( ctrl.log, WARNING )));
        UDEBUG();
        UINFO();
        assert( 0 == ulog_status_to_int(
                ctrl.log.verbosity( ctrl.log, ERROR )));
        UDEBUG();
        UINFO();
        UWARNING();
        assert( 0 == ulog_status_to_int( ulog_cleanup( ctrl )));
    }

    return 0;
}

