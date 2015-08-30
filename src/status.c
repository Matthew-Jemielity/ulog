/**
 * \author      Mateusz Jemielity matthew.jemielity@gmail.com
 * \brief       Definition of ulog_status helper methods.
 * \date        2015/08/28 22:45:59 AM
 * \file        status.c
 * \version     1.1
 *
 *
 **/

#include <ulog/status.h>

#include <stddef.h> /* NULL */

static char const * const default_description = "no details available";

ulog_status
ulog_status_descriptive( int const code, char const * const description )
{
    return ( ulog_status ) {
        .code = code,
        .description =
          (( NULL == description ) ? default_description : description )
    };
}

ulog_status
ulog_status_from_int( int const code )
{
    return ( ulog_status ) {
        .code = code,
        .description = default_description
    };
}

int
ulog_status_to_int( ulog_status const status )
{
    return status.code;
}

