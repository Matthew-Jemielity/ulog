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
        ulog_obj const * const ulog = ulog_obj_get();
        /* verbosity is DEBUG by default */
        assert( ulog_status_success( ulog->op->setup( ulog )));
        assert( ulog_status_success( ulog->op->add( ulog, debug_log )));
        UDEBUG();
        assert( ulog_status_success( ulog->op->cleanup( ulog )));
    }

    {
        ulog_obj const * const ulog = ulog_obj_get();
        /* verbosity is DEBUG by default */
        assert( ulog_status_success( ulog->op->setup( ulog )));
        assert( ulog_status_success(
                ulog->op->add( ulog, should_not_trigger )));
        assert( ulog_status_success(
                ulog->op->verbosity( ulog, INFO )));
        UDEBUG();
        assert( ulog_status_success(
                ulog->op->verbosity( ulog, WARNING )));
        UDEBUG();
        UINFO();
        assert( ulog_status_success(
                ulog->op->verbosity( ulog, ERROR )));
        UDEBUG();
        UINFO();
        UWARNING();
        assert( ulog_status_success( ulog->op->cleanup( ulog )));
    }

    return 0;
}

