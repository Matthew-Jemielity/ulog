/**
 * \author      Mateusz Jemielity matthew.jemielity@gmail.com
 * \brief       Test listable remove() #01
 * \date        09/03/2015 10:53:08 PM
 * \file        test_listable_remove_01.c
 * \version     1.0
 *
 *
 **/

#include <ulog/listable.h>
#include <ulog/status.h> /* ulog_status* */

#include <assert.h> /* assert */
#include <errno.h> /* ENOENT, EEXIST, ENODATA, EINVAL */

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

    assert( EINVAL == ulog_status_to_int( ctrl.op->remove( NULL, NULL )));
    assert( EINVAL == ulog_status_to_int(
            ctrl.op->remove( NULL, &( t1.list ))));
    assert( ENODATA == ulog_status_to_int( ctrl.op->remove( &ctrl, NULL )));
    assert( ENOENT == ulog_status_to_int(
            ctrl.op->remove( &ctrl, &( t1.list ))));

    assert( ulog_status_success( ctrl.op->add( &ctrl, &( t1.list ))));
    assert( ulog_status_success( ctrl.op->remove( &ctrl, &( t1.list ))));
    assert( ENOENT == ulog_status_to_int(
            ctrl.op->remove( &ctrl, &( t1.list ))));

    assert( ulog_status_success( ctrl.op->add( &ctrl, &( t1.list ))));
    ulog_listable invalid = { .next = NULL };
    assert( EEXIST == ulog_status_to_int( ctrl.op->remove( &ctrl, &invalid )));
    assert( ulog_status_success( ctrl.op->add( &ctrl, &( t2.list ))));
    assert( ulog_status_success( ctrl.op->add( &ctrl, &( t3.list ))));

    assert( ulog_status_success( ctrl.op->remove( &ctrl, &( t1.list ))));
    assert( ulog_status_success( ctrl.op->remove( &ctrl, &( t3.list ))));
    assert( EEXIST == ulog_status_to_int(
            ctrl.op->remove( &ctrl, &( t1.list ))));
    assert( EEXIST == ulog_status_to_int(
            ctrl.op->remove( &ctrl, &( t3.list ))));
    assert( ulog_status_success( ctrl.op->remove( &ctrl, &( t2.list ))));

    assert( ulog_status_success( ctrl.op->add( &ctrl, &( t3.list ))));
    assert( ulog_status_success( ctrl.op->add( &ctrl, &( t1.list ))));
    assert( ulog_status_success( ctrl.op->add( &ctrl, &( t2.list ))));
    while( ulog_status_success( ctrl.op->remove( &ctrl, ctrl.head ))) {}
    assert( ENOENT == ulog_status_to_int(
            ctrl.op->remove( &ctrl, &( t1.list ))));
    assert( ENOENT == ulog_status_to_int(
            ctrl.op->remove( &ctrl, &( t2.list ))));
    assert( ENOENT == ulog_status_to_int(
            ctrl.op->remove( &ctrl, &( t3.list ))));

    return 0;
}

