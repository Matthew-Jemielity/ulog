/**
 * \author      Mateusz Jemielity matthew.jemielity@gmail.com
 * \brief       Definition of ulog_status helper methods.
 * \date        2015/04/25 14:08:59 AM
 * \file        status.c
 * \version     1.0
 *
 *
 **/

#include <ulog/status.h>

ulog_status
ulog_status_from_int( int const code )
{
    return ( ulog_status ) { code };
}

int
ulog_status_to_int( ulog_status const status )
{
    return status.code;
}

