# ulog
Micro logging library written in C.

Usage:
void log_to_stderr(
    ulog_level const level,
    char const * const format,
    va_list args
)
{
    vprintf( stderr, format, args );
    fflush( stderr );
}

ulog_ctrl ctrl = ulog_setup();
ctrl.log.add( ctrl.log, log_to_stderr );
...
UERROR( "my log here" );
...
ulog_cleanup( ctrl );

