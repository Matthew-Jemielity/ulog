/**
 * \author      Mateusz Jemielity matthew.jemielity@gmail.com
 * \brief       Tests output of ulog_level_to_char().
 * \date        08/22/2015 03:46:03 PM
 * \file        test_log_level_to_char.c
 * \version     1.0
 *
 *
 **/

#include <assert.h>
#include <ulog/ulog.h>

int
main( void )
{
    assert( 'E' == ulog_level_to_char( ERROR ));
    assert( 'W' == ulog_level_to_char( WARNING ));
    assert( 'I' == ulog_level_to_char( INFO ));
    assert( 'D' == ulog_level_to_char( DEBUG ));
    assert( '?' == ulog_level_to_char( 0xFFFFFFFF ));
}

