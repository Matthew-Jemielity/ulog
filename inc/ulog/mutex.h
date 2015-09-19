/**
 * \author      Mateusz Jemielity matthew.jemielity@gmail.com
 * \brief       Basic mutex API, wrapper for possible implementations.
 * \date        2015/04/24 17:05:56
 * \file        mutex.h
 * \version     1.0
 *
 * Mutex will be implemented using C11 mtx_t if available, or POSIX
 * threads' pthread_mutex_t. This is only ease-of-use wrapper.
 **/

#ifndef ULOG_MUTEX_H__
# define ULOG_MUTEX_H__

# include <ulog/status.h> /* ulog_status */
# include <ulog/universal.h> /* THREADUNSAFE */

/**
 * \brief Forward declaration of opaque ulog_mutex state.
 * \see struct ulog_mutex_state_struct
 */
typedef struct ulog_mutex_state_struct ulog_mutex_state;
/**
 * \brief Forward declaration of mutex operations table.
 * \see struct ulog_mutex_op_table_struct
 */
typedef struct ulog_mutex_op_table_struct ulog_mutex_op_table;
/**
 * \brief Definition of mutex object.
 * \see ulog_mutex_state
 * \see ulog_mutex_op_table
 *
 * The ulog_mutex object starts its life cycle in a default,
 * uninitialized state. To use it, setup() has to be called.
 * Note that setup changed the ulog_mutex object and is not
 * thread-safe. Afterwards lock() and unlock() are available.
 * Both lock() and unlock() are safe to use in threads. When
 * mutex object is no longer needed, its cleanup() operation
 * has to be called. Cleanup operation also changes ulog_mutex
 * object and is not thread-safe. Sample usage:
 * ulog_mutex m = ulog_mutex_get();
 * m.op->setup(&m);
 * m.op->lock(&m);
 * critical_section();
 * m.op->unlock(&m);
 * m.op->cleanup(&m);
 *
 * Copying the ulog_mutex object is shallow and all quirks of
 * underlying mechanisms apply. Be careful with it. Here are
 * some test cases run in a single thread, with resultant
 * behaviour:
 * ulog_mutex m1 = ulog_mutex_get();
 * +------------------+-----------------+-----------------+
 * | separate objects |    undefined!   |   undefined!    |
 * +------------------+-----------------+-----------------+
 * |ulog_mutex m2=m1; |m.op->setup(&m1);|m.op->setup(&m1);|
 * |m1.op->setup(&m1);|m1.op->lock(&m1);|ulog_mutex m2=m1;|
 * |m2.op->setup(&m2);|ulog_mutex m2=m1;|m1.op->lock(&m1);|
 * |m1.op->lock(&m1); |m2.op->lock(&m2);|m2.op->lock(&m2);|
 * |m2.op->lock(&m2); |                 |                 |
 * +------------------+-----------------+-----------------+
 *
 * Usage of ulog_mutex object in multiple threads, example
 * using POSIX threads model:
 * ulog_mutex m = ulog_mutex_get();
 * m.op->setup(&m);
 * pthread_t thread_1, thread_2;
 * pthread_create(&thread_1, NULL, func_1, &m);
 * pthread_create(&thread_2, NULL, func_2, &m);
 * +-------------------------+-------------------------+
 * |        thread_1         |        thread_2         |
 * +-------------------------+-------------------------+
 * |ulog_mutex * m = arg;    |ulog_mutex m = arg;      |
 * |m->op->lock(m)           |m->op->lock(m);          |
 * |...                      |...                      |
 * |m->op->unlock(m);        |m->op->unlock(m);        |
 * +-------------------------+-------------------------+
 * pthread_join(thread_1, NULL);
 * pthread_join(thread_2, NULL);
 * m.op->cleanup(&m);
 */
typedef struct
{
    /** Object's state */
    ulog_mutex_state * state;
    /** Table of operations. */
    ulog_mutex_op_table const * op;
} ulog_mutex;
/**
 * \brief Defines type of control operations on ulog_mutex object.
 * \param self The ulog_mutex object on which we'll operate.
 * \return Status object.
 * \see THREADUNSAFE
 * \see ulog_status
 * \see ulog_mutex
 *
 * This type of operation changes the state of ulog_mutex object.
 * It is not thread-safe. Note that calling cleanup() when mutex
 * is locked is undefined behaviour. Caller must ensure that the
 * thread calling cleanup() does not own the locked mutex and that
 * there are no additional threads waiting on the mutex (apart from
 * current thread calling cleanup() and possible other thread which
 * is holding the mutex). In case mutex is held by another thread,
 * the cleanup() operation will block until the mutex is free. No
 * other threads can be waiting on the mutex after the cleanup()
 * operation returns.
 * Possible status codes:
 * 1. setup:
 *    a. 0 (zero) - setup successful;
 *    b. EINVAL - invalid self given;
 *    c. EALREADY - self already initialized;
 *    d. ENOMEM - cannot allocate memory for mutex state;
 *    e. EIO - failure setting up the underlying mutex mechanism;
 * 2. cleanup:
 *    a. 0 (zero) - cleanup successful;
 *    b. EINVAL - invalid self given;
 *    c. EALREADY - self already uninitialized;
 *    d. EIO - failure cleaning up the underlying mutex mechanism;
 * In all cases status description may be checked for further
 * information.
 */
typedef THREADUNSAFE ulog_status
    ( * ulog_mutex_ctrl_op )( ulog_mutex * const self );
/**
 * \brief Defines type of common operations on ulog_mutex object.
 * \param self The ulog_mutex object on which we'll operate.
 * \return Status object.
 * \see ulog_status
 * \see ulog_mutex
 *
 * Recursively locking the mutex is undefined behaviour. Unlocking
 * mutex that is not locked or is held by another thread is also
 * undefined behaviour.
 * Possible status codes:
 * 1. lock:
 *    a. 0 (zero) - mutex locked successfully
 *    b. EINVAL - invalid or uninitialized self given;
 *    c. EIO - error locking the underlying mutex mechanism;
 * 2. unlock:
 *    a. 0 (zero) - mutex unlocked successfully
 *    b. EINVAL - invalid or uninitialized self given;
 *    c. EIO - error unlocking the underlying mutex mechanism.
 * In all cases status description may be checked for further
 * information.
 */
typedef ulog_status
    ( * ulog_mutex_op )( ulog_mutex const * const self );
/**
 * \brief Definition of mutex operations table.
 * \see ulog_mutex_ctrl_op
 * \see ulog_mutex_op
 */
struct ulog_mutex_op_table_struct
{
    /** Mutex setup operation. */
    ulog_mutex_ctrl_op setup;
    /** Mutex cleanup operation. */
    ulog_mutex_ctrl_op cleanup;
    /** Mutex locking operation. */
    ulog_mutex_op lock;
    /** Mutex unlocking operation. */
    ulog_mutex_op unlock;
};
/**
 * \brief Creates mutex object in default state.
 * \return Mutex object.
 * \see ulog_mutex
 */
ulog_mutex
ulog_mutex_get( void );

#endif /* ULOG_MUTEX_H__ */

