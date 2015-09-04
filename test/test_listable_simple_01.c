/**
 * \author      Mateusz Jemielity matthew.jemielity@gmail.com
 * \brief       Test listable simple #01
 * \date        09/02/2015 10:32:38 PM
 * \file        test_listable_simple_01.c
 * \version     1.0
 *
 *
 **/

#include <ulog/listable.h>
#include <ulog/status.h> /* ulog_status* */
#include <ulog/universal.h> /* UNUSED */

#include <assert.h>

typedef struct
{
    int i;
    ulog_listable list;
}
test_struct;

static int const test_value = 23;

static ulog_status
test_callback( ulog_listable * const element, void * const userdata )
{
    UNUSED( userdata );
    test_struct * const t =
        ulog_listable_get_container( element, test_struct, list );
    assert( test_value == t->i );
    return ulog_status_from_int( 0 );
}

int
main(void)
{
    test_struct t1 = { .i = test_value, .list = ulog_listable_get() };
    test_struct t2 = { .i = test_value, .list = ulog_listable_get() };
    ulog_list_ctrl ctrl = ulog_list_ctrl_get();

    assert( 0 == ulog_status_to_int( ctrl.op->add( &ctrl, &( t1.list ))));
    assert( 0 == ulog_status_to_int( ctrl.op->add( &ctrl, &( t2.list ))));
    assert(
        0 == ulog_status_to_int(
            ctrl.op->foreach( &ctrl, test_callback, NULL )
        )
    );
    assert( 0 == ulog_status_to_int( ctrl.op->remove( &ctrl, &( t1.list ))));
    assert( 0 == ulog_status_to_int( ctrl.op->remove( &ctrl, &( t2.list ))));

    return 0;
}

