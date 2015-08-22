/**
 * \author      Mateusz Jemielity matthew.jemielity@gmail.com
 * \brief       Defines API for linked list of pointers.
 * \date        2015/04/24 18:16:54 AM
 * \file        pointer_list.h
 * \version     1.0
 *
 *
 **/

#ifndef ULOG_POINTER_LIST_H__
# define ULOG_POINTER_LIST_H__

# include <ulog/status.h> /* ulog_status */

/**
 * \brief Forward declaration of ulog_pointer_list element.
 */
typedef struct ulog_pointer_list_element_struct ulog_pointer_list_element;

/**
 * \brief Definition of ulog_pointer_list element.
 */
struct ulog_pointer_list_element_struct
{
    /** Pointer held in the list. */
    void * pointer;
    /** Pointer to next list element. */
    ulog_pointer_list_element * next;
};

/*
 * \brief Forward declaration of ulog_pointer_list.
 */
typedef struct ulog_pointer_list_struct ulog_pointer_list;

/**
 * \brief Common list operation function type.
 * \param self ulog_pointer_list on which we'll operate.
 * \param pointer Pointer with which the operation will be done.
 *
 * Error codes possible:
 * 1. Common:
 * a) EINVAL - if invalid self pointer is given.
 * 2. When adding a pointer:
 * a) ENOMEM - memory allocation for list element failed;
 * b) EEXIST - given pointer already exists in the list.
 * 3. When removing a pointer:
 * a) ENODATA - given pointer doesn't exist in the list.
 */
typedef ulog_status
( * ulog_pointer_list_func )(
    ulog_pointer_list * const self,
    void * const pointer
);

/**
 * \brief Definition of callback type ued in foreach loop.
 * \param pointer List element.
 * \param userdata Pointer to user-supplied data.
 * \see ulog_pointer_list_foreach_func
 *
 * The ulog_pointer_list's foreach method takes this callback type as an
 * argument. For every item in the list this callback will be executed. The
 * execution will continue as long as callbacks are successful, that is they
 * return 0. First returned error will stop the foreach loop and return the
 * received error.
 */
typedef ulog_status
( * ulog_pointer_list_foreach_callback )(
    void * const pointer,
    void * const userdata
);

/**
 * \brief Defines type of foreach function.
 * \param self ulog_pointer_list on which we'll operate.
 * \param callback Callback executed for each list item.
 * \param userdata Pointer to user-supplied data.
 * \see ulog_pointer_list_foreach_callback
 *
 * This method will iterate through te list. In each iteration it will execute
 * the given callback, passing the list item (void pointer) and user data.
 * Iteration will continue as long as callbacks return successful status
 * (zero).
 * Possible error codes:
 * 1. EINVAL - invalid list object passed;
 * 2. ENOEXEC - passed callback is NULL;
 * 3. ENODATA - the list is empty;
 * 4. any error code returned by the callback.
 */
typedef ulog_status
( * ulog_pointer_list_foreach_func )(
    ulog_pointer_list const * const self,
    ulog_pointer_list_foreach_callback const callback,
    void * const userdata
);

/**
 * \brief Definition of ulog_pointer_list.
 * \see ulog_pointer_list_element
 * \see ulog_pointer_list_func
 * \see ulog_pointer_list_foreach_func
 *
 * The list holds unique pointers to arbitrary data. Adding a pointer will
 * first check whether such pointer exists.
 */
struct ulog_pointer_list_struct
{
    /** Pointer to first list element */
    ulog_pointer_list_element * head;
    /** Adds pointer to the list. */
    ulog_pointer_list_func add;
    /** Removes existing pointer. */
    ulog_pointer_list_func remove;
    /** Iterator. */
    ulog_pointer_list_foreach_func foreach;
};

/**
 * \brief Sets up a ulog_pointer_list.
 * \return A ulog_pointer_list object.
 *
 * Head is initialized to NULL. This method will not fail.
 */
ulog_pointer_list
ulog_pointer_list_setup( void );

/**
 * \brief Deinitializes and frees ulog_pointer_list.
 * \param list Pointer to ulog_pointer_list to cleanup.
 * \return Zero on success, else error code.
 * \warning User is responsible to safely free (if needed) list pointers.
 *
 * The error codes returned can be:
 * 1. EINVAL if invalid argument is given;
 * 2. the same as returned by list element removal method.
 * After the method successfully returns, the list head will be NULL.
 */
ulog_status
ulog_pointer_list_cleanup( ulog_pointer_list * const list );

#endif /* ULOG_POINTER_LIST_H__ */

