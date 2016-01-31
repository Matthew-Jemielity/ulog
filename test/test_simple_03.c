/**
 * \author      Mateusz Jemielity matthew.jemielity@gmail.com
 * \brief       Basic test of libulog functionality.
 * \date        01/31/2016 01:02:13 AM
 * \file        test_simple_03.c
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

void
foo( void )
{
    UERROR("from foo");
    UWARNING("from foo");
    UINFO("from foo");
    UDEBUG("from foo");
}

void
bar( void )
{
    UERROR("from bar");
    UWARNING("from bar");
    UINFO("from bar");
    UDEBUG("from bar");
}

int
main( void )
{
    ulog_obj const * const ulog = ulog_obj_get();
    assert( ulog_status_success( ulog->op->setup( ulog )));
    assert( ulog_status_success( ulog->op->add( ulog, log_to_stderr )));
    assert( ulog_status_success( ulog->op->add( ulog, log_to_stdout )));

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

    foo();
    bar();

    assert( ulog_status_success( ulog->op->cleanup( ulog )));
    return 0;
}

