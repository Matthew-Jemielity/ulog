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
    assert( 'E' == ulog_level_to_char_( ERROR ));
    assert( 'W' == ulog_level_to_char_( WARNING ));
    assert( 'I' == ulog_level_to_char_( INFO ));
    assert( 'D' == ulog_level_to_char_( DEBUG ));
    assert( '?' == ulog_level_to_char_( 0xFFFFFFFF ));
}

