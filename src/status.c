/**
 * \author      Mateusz Jemielity matthew.jemielity@gmail.com
 * \brief       Implements ulog_status helper methods.
 * \date        2015/08/28 22:45:59 AM
 * \file        status.c
 * \version     1.1
 *
 *
 **/

#include <ulog/status.h>

#include <stdbool.h> /* bool */
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

int
ulog_status_to_int( ulog_status const status )
{
    return status.code;
}

bool
ulog_status_success( ulog_status const status )
{
    return (0 == status.code);
}

