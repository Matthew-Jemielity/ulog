/**
 * \author      Mateusz Jemielity matthew.jemielity@gmail.com
 * \brief       Checks whether proper log levels are passed.
 * \date        04/25/2015 07:39:13 AM
 * \file        test_log_levels_01.c
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
error_log(
    ulog_level const level,
    char const * const format,
    va_list args
)
{
    ( void ) format;
    ( void ) args;
    assert( ERROR == level );
}

void
warning_log(
    ulog_level const level,
    char const * const format,
    va_list args
)
{
    ( void ) format;
    ( void ) args;
    assert( WARNING == level );
}

void
info_log(
    ulog_level const level,
    char const * const format,
    va_list args
)
{
    ( void ) format;
    ( void ) args;
    assert( INFO == level );
}

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

int
main( void )
{
    {
        ulog_obj const * const ulog = ulog_obj_get();
        assert( ulog_status_success( ulog->op->setup( ulog )));
        assert( ulog_status_success( ulog->op->add( ulog, error_log )));
        UERROR( "" );
        assert( ulog_status_success( ulog->op->cleanup( ulog )));
    }

    {
        ulog_obj const * const ulog = ulog_obj_get();
        assert( ulog_status_success( ulog->op->setup( ulog )));
        assert( ulog_status_success( ulog->op->add( ulog, warning_log )));
        UWARNING( "" );
        assert( ulog_status_success( ulog->op->cleanup( ulog )));
    }

    {
        ulog_obj const * const ulog = ulog_obj_get();
        assert( ulog_status_success( ulog->op->setup( ulog )));
        assert( ulog_status_success( ulog->op->add( ulog, info_log )));
        UINFO( "" );
        assert( ulog_status_success( ulog->op->cleanup( ulog )));
    }

    {
        ulog_obj const * const ulog = ulog_obj_get();
        assert( ulog_status_success( ulog->op->setup( ulog )));
        assert( ulog_status_success( ulog->op->add( ulog, debug_log )));
        UDEBUG( "" );
        assert( ulog_status_success( ulog->op->cleanup( ulog )));
    }

    return 0;
}

