/**
 * \author      Mateusz Jemielity matthew.jemielity@gmail.com
 * \brief       Test ulog_obj_get #01
 * \date        09/19/2015 10:57:42 PM
 * \file        test_ulog_obj_get_01.c
 * \version     1.0
 *
 *
 **/

#include <ulog/ulog.h>

#include <assert.h> /* assert */
#include <stddef.h> /* NULL */

int main( void )
{
    ulog_obj const * const ulog = ulog_obj_get();

    assert( NULL != ulog );
    assert( NULL != ulog->state );
    assert( NULL != ulog->op );

    return 0;
}

