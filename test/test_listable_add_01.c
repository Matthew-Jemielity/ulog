/**
 * \author      Mateusz Jemielity matthew.jemielity@gmail.com
 * \brief       Test listable add() #01
 * \date        09/03/2015 10:34:28 PM
 * \file        test_listable_add_01.c
 * \version     1.0
 *
 *
 **/

#include <ulog/listable.h>
#include <ulog/status.h> /* ulog_status* */

#include <assert.h> /* assert */
#include <errno.h> /* ENODATA, EINVAL */

typedef struct
{
    int i;
    ulog_listable list;
}
test_struct;

static int const test_value = 23;

int
main(void)
{
    test_struct t1 = { .i = test_value, .list = ulog_listable_get() };
    test_struct t2 = { .i = test_value, .list = ulog_listable_get() };
    test_struct t3 = { .i = test_value, .list = ulog_listable_get() };
    ulog_list_ctrl ctrl = ulog_list_ctrl_get();

    assert( EINVAL == ulog_status_to_int( ctrl.op->add( NULL, NULL )));
    assert( EINVAL == ulog_status_to_int( ctrl.op->add( NULL, &( t1.list ))));
    assert( ENODATA == ulog_status_to_int( ctrl.op->add( &ctrl, NULL )));

    assert( 0 == ulog_status_to_int( ctrl.op->add( &ctrl, &( t1.list ))));
    assert( ENODATA == ulog_status_to_int(
            ctrl.op->add( &ctrl, &( t1.list ))));

    ulog_listable invalid = { .next = NULL };
    assert( ENODATA == ulog_status_to_int( ctrl.op->add( &ctrl, &invalid )));

    assert( 0 == ulog_status_to_int( ctrl.op->add( &ctrl, &( t2.list ))));
    assert( 0 == ulog_status_to_int( ctrl.op->add( &ctrl, &( t3.list ))));
    assert( ENODATA == ulog_status_to_int(
            ctrl.op->add( &ctrl, &( t1.list ))));
    assert( ENODATA == ulog_status_to_int(
            ctrl.op->add( &ctrl, &( t2.list ))));
    assert( ENODATA == ulog_status_to_int(
            ctrl.op->add( &ctrl, &( t3.list ))));

    assert( 0 == ulog_status_to_int( ctrl.op->remove( &ctrl, &( t1.list ))));
    assert( 0 == ulog_status_to_int( ctrl.op->remove( &ctrl, &( t2.list ))));
    assert( ENODATA == ulog_status_to_int(
            ctrl.op->add( &ctrl, &( t3.list ))));
    assert( 0 == ulog_status_to_int( ctrl.op->add( &ctrl, &( t1.list ))));
    assert( ENODATA == ulog_status_to_int(
            ctrl.op->add( &ctrl, &( t1.list ))));

    assert( 0 == ulog_status_to_int( ctrl.op->remove( &ctrl, &( t1.list ))));
    assert( 0 == ulog_status_to_int( ctrl.op->remove( &ctrl, &( t3.list ))));
    assert( 0 == ulog_status_to_int( ctrl.op->add( &ctrl, &( t3.list ))));
    assert( 0 == ulog_status_to_int( ctrl.op->add( &ctrl, &( t1.list ))));
    assert( 0 == ulog_status_to_int( ctrl.op->add( &ctrl, &( t2.list ))));

    assert( 0 == ulog_status_to_int( ctrl.op->remove( &ctrl, &( t1.list ))));
    assert( 0 == ulog_status_to_int( ctrl.op->remove( &ctrl, &( t2.list ))));
    assert( 0 == ulog_status_to_int( ctrl.op->remove( &ctrl, &( t3.list ))));

    return 0;
}

