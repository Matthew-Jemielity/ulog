/**
 * \author      Mateusz Jemielity matthew.jemielity@gmail.com
 * \brief       Checks whether proper error levels are passed.
 * \date        04/25/2015 07:39:13 AM
 * \file        test_error_levels_01.c
 * \version     1.0
 *
 *
 **/

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <ulog/status.h>
#include <ulog/ulog.h>

void error_log(
    ulog_level const level,
    char const * const format,
    va_list args
)
{
    ( void ) format;
    ( void ) args;
    assert( ERROR == level );
}

void warning_log(
    ulog_level const level,
    char const * const format,
    va_list args
)
{
    ( void ) format;
    ( void ) args;
    assert( WARNING == level );
}

void info_log(
    ulog_level const level,
    char const * const format,
    va_list args
)
{
    ( void ) format;
    ( void ) args;
    assert( INFO == level );
}

void debug_log(
    ulog_level const level,
    char const * const format,
    va_list args
)
{
    ( void ) format;
    ( void ) args;
    assert( DEBUG == level );
}

int main( int argc, char * args[] )
{
    ( void ) argc;
    ( void ) args;

    {
        ulog_ctrl ctrl = ulog_setup();
        if( 0 != ulog_status_to_int( ctrl.status ))
        {
            return ulog_status_to_int( ctrl.status );
        }
        assert( 0 == ulog_status_to_int( ctrl.log.add( ctrl.log, error_log )));
        UERROR( "" );
        assert( 0 == ulog_status_to_int( ulog_cleanup( ctrl )));
    }

    {
        ulog_ctrl ctrl = ulog_setup();
        if( 0 != ulog_status_to_int( ctrl.status ))
        {
            return ulog_status_to_int( ctrl.status );
        }
        assert( 0 == ulog_status_to_int( ctrl.log.add( ctrl.log, warning_log )));
        UWARNING( "" );
        assert( 0 == ulog_status_to_int( ulog_cleanup( ctrl )));
    }

    {
        ulog_ctrl ctrl = ulog_setup();
        if( 0 != ulog_status_to_int( ctrl.status ))
        {
            return ulog_status_to_int( ctrl.status );
        }
        assert( 0 == ulog_status_to_int( ctrl.log.add( ctrl.log, info_log )));
        UINFO( "" );
        assert( 0 == ulog_status_to_int( ulog_cleanup( ctrl )));
    }

    {
        ulog_ctrl ctrl = ulog_setup();
        if( 0 != ulog_status_to_int( ctrl.status ))
        {
            return ulog_status_to_int( ctrl.status );
        }
        assert( 0 == ulog_status_to_int( ctrl.log.add( ctrl.log, debug_log )));
        UDEBUG( "" );
        assert( 0 == ulog_status_to_int( ulog_cleanup( ctrl )));
    }

    return 0;
}

