/**
 * \author      Mateusz Jemielity matthew.jemielity@gmail.com
 * \brief       Defines API for singly linked list.
 * \date        08/30/2015 09:22:52 PM
 * \file        listable.h
 * \version     1.0
 *
 *
 **/

#ifndef ULOG_LISTABLE_H__
# define ULOG_LISTABLE_H__

# include <ulog/status.h> /* ulog_status */

# include <stddef.h> /* offsetof */
# include <stdint.h> /* uint8_t */

/**
 * \brief Gets container of listable element.
 * \param LISTABLE Pointer to listable element.
 * \param TYPE Type of container, must contain listable element.
 * \param MEMBER Name of the listable element in TYPE container.
 * \return Pointer to the container of passed listable element.
 * \see ulog_listable_compile_time_check
 * \see ulog_listable
 *
 * Caller must ensure that passed pointer to listable element is
 * actually part of memory holding structure with given TYPE.
 */
# define ulog_listable_get_container( LISTABLE, TYPE, MEMBER ) \
    ( \
      ulog_listable_compile_time_check( *( LISTABLE )), \
      ( TYPE * )((( uint8_t * )( LISTABLE )) - offsetof( TYPE, MEMBER )) \
    )
/**
 * \brief Forward declaration of list operations table.
 * \see struct ulog_list_op_table_struct
 */
typedef struct ulog_list_op_table_struct ulog_list_op_table;
/**
 * \brief Forward declaration of list element.
 * \see struct ulog_listable_struct
 */
typedef struct ulog_listable_struct ulog_listable;
/**
 * \brief Definition of list element.
 * \see ulog_listable
 */
struct ulog_listable_struct
{
    /** Pointer to next list element. */
    ulog_listable * next;
};
/**
 * \brief Definition of list control structure.
 * \see ulog_listable
 * \see ulog_list_op_table
 *
 * The list control structure manages a singly linked list. The list
 * does not allow duplicate items - only unique entries will be held.
 * After ulog_list_ctrl_get(), the list control is ready for adding
 * or removing items. To add an item of a certain type to the list,
 * the type must include ulog_listable element.
 * Sample code:
 * typedef struct
 * {
 *    int i;
 *    ulog_listable list;
 * }
 * foo;
 *
 * ulog_status bar(ulog_listable * const element, void * const userdata)
 * {
 *    UNUSED(userdata);
 *    foo * const f = ulog_listable_get_container(element, foo, list);
 *    printf("%d\n", f->i);
 *    return ulog_status_descriptive(0, "success");
 * }
 *
 * foo f = { 23, ulog_listable_get() };
 * ulog_list_ctrl ctrl = ulog_list_ctrl_get();
 * ctrl.op->add(&ctrl, &(f.list));
 * ctrl.op->foreach(&ctrl, bar, NULL);
 * ctrl.op->remove(&ctrl, &(f.list));
 */
typedef struct
{
    /** Head of the list. */
    ulog_listable * head;
    /** Table of operations. */
    ulog_list_op_table const * op;
}
ulog_list_ctrl;
/**
 * \brief Defines type of common operations on list control structure.
 * \param self List control on which we operate.
 * \param element Valid pointer to listable member of some data type.
 * \return Status object.
 * \see ulog_status
 * \see ulog_list_ctrl
 * \see ulog_listable
 *
 * Caller is responsible for memory management of passed element. If
 * a listable element is member of one list then it cannot be added
 * to any other list.
 * Possible status codes:
 * 1. add:
 *    a. 0 (zero) - element was successfully added to the list;
 *    b. EINVAL - invalid self given;
 *    c. EALREADY - given element already exists in the list;
 *    d. ENODATA - given element is invalid, e.g. it already is on some list;
 * 2. remove:
 *    a. 0 (zero) - element was successfully removed from the list;
 *    b. EINVAL - invalid self given;
 *    c. ENODATA - given element is invalid;
 *    d. ENOENT - list is empty;
 *    e. EEXIST - given element wasn't found in the list.
 */
typedef ulog_status
    ( * ulog_list_op )(
        ulog_list_ctrl * const self,
        ulog_listable * const element
    );
/**
 * \brief Declares type of callback used in foreach loop.
 * \param element Pointer to element; can be used to get it's container.
 * \param userdata Pointer to user-supplied data given in foreach() call.
 * \return Status object.
 * \see ulog_status
 * \see ulog_listable
 *
 * The list control object's foreach() operation takes this callback type as
 * an argument. This callback will be executed for each element in the list.
 * Macro ulog_listable_get_container() can be used to get container of the
 * listable element given. Callback execution will continue as long as the
 * status objects returned describe success (i.e. 0 == ulog_status_to_int()).
 * First returned error will stop the foreach loop and return the received
 * error status object.
 */
typedef ulog_status
    ( * ulog_list_foreach_op_callback_fn )(
        ulog_listable * const element,
        void * const userdata
    );
/**
 * \brief Defines type of foreach() operation of list control object.
 * \param self List control object on which we operate.
 * \param callback Callback function to execute for each list element.
 * \param userdata Pointer to user-supplied data passed to callback.
 * \return Status object.
 * \see ulog_status
 * \see ulog_list_ctrl
 * \see ulog_list_foreach_op_callback_fn
 *
 * The foreach() operation will iterate through the list managed by the
 * list control object. In each iteration given callback will be executed,
 * taking the listable element and given user data. Iteration will continue
 * as long as callback return successful status objects.
 * Possible status codes:
 * 1. 0 (zero) - all iterations completed successfully;
 * 2. EINVAL - invalid self given;
 * 3. ENOEXEC - given callback is NULL;
 * 4. ENOENT - list is empty;
 * 5. any code returned by callback.
 */
typedef ulog_status
    ( * ulog_list_foreach_op )(
        ulog_list_ctrl const * const self,
        ulog_list_foreach_op_callback_fn const callback,
        void * const userdata
    );
/**
 * \brief Definition of list control object's operation table.
 * \see ulog_list_op
 * \see ulog_list_foreach_op
 */
struct ulog_list_op_table_struct
{
    /** Adds element to the list. */
    ulog_list_op add;
    /** Removes element from the list. */
    ulog_list_op remove;
    /** Calls user function for each element in the list. */
    ulog_list_foreach_op foreach;
};
/**
 * \brief Returns valid, initialized list element.
 * \return List element.
 * \see ulog_listable
 */
ulog_listable
ulog_listable_get( void );
/**
 * \brief Returns valid, initialized list control object.
 * \return List control object.
 * \see ulog_list_ctrl
 */
ulog_list_ctrl
ulog_list_ctrl_get( void );
/**
 * \brief Ensures compile-time type safety for list elements.
 * \param element List element.
 * \see ulog_listable_get_container
 * \see ulog_listable
 */
void
ulog_listable_compile_time_check( ulog_listable const element );

#endif /* ULOG_LISTABLE_H__ */

