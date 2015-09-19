/**
 * \author      Mateusz Jemielity matthew.jemielity@gmail.com
 * \brief       Test ulog_obj's verbosity() #01
 * \date        09/19/2015 11:03:04 PM
 * \file        test_ulog_obj_verbosity_01.c
 * \version     1.0
 *
 *
 **/

#include <ulog/status.h>
#include <ulog/ulog.h>

#include <assert.h> /* assert */
#include <errno.h> /* EINVAL, etc. */
#include <stddef.h> /* NULL */

static ulog_obj bad;

int main( void )
{
    ulog_obj const * const ulog = ulog_obj_get();

    assert( EINVAL == ulog_status_to_int( ulog->op->verbosity( NULL, 0xFFFF )));
    assert( EINVAL == ulog_status_to_int( ulog->op->verbosity( NULL, ERROR )));
    assert( EINVAL == ulog_status_to_int(
            ulog->op->verbosity( NULL, WARNING )));
    assert( EINVAL == ulog_status_to_int( ulog->op->verbosity( NULL, INFO )));
    assert( EINVAL == ulog_status_to_int( ulog->op->verbosity( NULL, DEBUG )));

    assert( EINVAL == ulog_status_to_int( ulog->op->verbosity( &bad, 0xFFFF )));
    assert( EINVAL == ulog_status_to_int( ulog->op->verbosity( &bad, ERROR )));
    assert( EINVAL == ulog_status_to_int(
            ulog->op->verbosity( &bad, WARNING )));
    assert( EINVAL == ulog_status_to_int( ulog->op->verbosity( &bad, INFO )));
    assert( EINVAL == ulog_status_to_int( ulog->op->verbosity( &bad, DEBUG )));

    assert( ENOTCONN == ulog_status_to_int(
            ulog->op->verbosity( ulog, 0xFFFF )));
    assert( ENOTCONN == ulog_status_to_int(
            ulog->op->verbosity( ulog, ERROR )));
    assert( ENOTCONN == ulog_status_to_int(
            ulog->op->verbosity( ulog, WARNING )));
    assert( ENOTCONN == ulog_status_to_int(
            ulog->op->verbosity( ulog, INFO )));
    assert( ENOTCONN == ulog_status_to_int(
            ulog->op->verbosity( ulog, DEBUG )));

    assert( ulog_status_success( ulog->op->setup( ulog )));

    assert( ENODATA == ulog_status_to_int(
            ulog->op->verbosity( ulog, 0xFFFF )));

    assert( ulog_status_success( ulog->op->verbosity( ulog, DEBUG )));
    assert( ulog_status_success( ulog->op->verbosity( ulog, INFO )));
    assert( ulog_status_success( ulog->op->verbosity( ulog, WARNING )));
    assert( ulog_status_success( ulog->op->verbosity( ulog, ERROR )));

    assert( ulog_status_success( ulog->op->cleanup( ulog )));
}

