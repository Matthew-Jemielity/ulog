/**
 * \author      Mateusz Jemielity matthew.jemielity@gmail.com
 * \brief       Test listable foreach() #01
 * \date        09/04/2015 09:07:23 PM
 * \file        test_listable_foreach_01.c
 * \version     1.0
 *
 *
 **/

#include <ulog/listable.h>
#include <ulog/status.h> /* ulog_status* */
#include <ulog/universal.h> /* UNUSED */

#include <assert.h>
#include <errno.h> /* ENOEXEC, ENOENT, EINVAL */

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
    return ulog_status_descriptive( 0, "test callback successful" );
}

static int counter;

static ulog_status
counter_callback( ulog_listable * const element, void * const userdata )
{
    UNUSED( element );
    UNUSED( userdata );
    ++counter;
    return ulog_status_descriptive( 0, "counter callback successful" );
}

static ulog_status
assert_callback( ulog_listable * const element, void * const userdata )
{
    UNUSED( element );
    UNUSED( userdata );
    assert( 0 );
    return ulog_status_descriptive( 0, "assert callback successful" );
}

static ulog_status
failure_callback( ulog_listable * const element, void * const userdata )
{
    UNUSED( element );
    UNUSED( userdata );
    return ulog_status_descriptive( test_value, "forced failure in callback" );
}

int
main(void)
{
    test_struct t1 = { .i = test_value, .list = ulog_listable_get() };
    test_struct t2 = { .i = test_value, .list = ulog_listable_get() };
    ulog_list_ctrl ctrl = ulog_list_ctrl_get();

    assert(
        EINVAL == ulog_status_to_int( ctrl.op->foreach( NULL, NULL, NULL ))
    );
    assert(
        EINVAL == ulog_status_to_int(
            ctrl.op->foreach( NULL, test_callback, NULL )
        )
    );
    assert(
        ENOEXEC == ulog_status_to_int( ctrl.op->foreach( &ctrl, NULL, NULL ))
    );

    assert(
        ENOENT == ulog_status_to_int(
            ctrl.op->foreach( &ctrl, assert_callback, NULL )
        )
    );

    assert( ulog_status_success( ctrl.op->add( &ctrl, &( t1.list ))));
    assert( ulog_status_success( ctrl.op->add( &ctrl, &( t2.list ))));
    assert(
        ulog_status_success(
            ctrl.op->foreach( &ctrl, test_callback, NULL )
        )
    );

    counter = 0;
    assert(
        ulog_status_success(
            ctrl.op->foreach( &ctrl, counter_callback, NULL )
        )
    );
    assert( 2 == counter );

    assert(
        test_value == ulog_status_to_int(
            ctrl.op->foreach( &ctrl, failure_callback, NULL )
        )
    );

    assert( ulog_status_success( ctrl.op->remove( &ctrl, &( t1.list ))));
    assert( ulog_status_success( ctrl.op->remove( &ctrl, &( t2.list ))));

    return 0;
}

