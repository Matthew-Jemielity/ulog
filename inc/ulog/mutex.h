/**
 * \author      Mateusz Jemielity matthew.jemielity@gmail.com
 * \brief       Basic mutex API, wrapper for possible implementations.
 * \date        2015/04/24 17:05:56
 * \file        mutex.h
 * \version     1.0
 *
 * Mutex will be implemented using C11 mtx_t if available, or POSIX
 * threads' pthread_mutex_t.
 **/

#ifndef ULOG_MUTEX_H__
# define ULOG_MUTEX_H__

# include <ulog/status.h> /* ulog_status */

/**
 * \brief Forward declaration of ulog_mutex object.
 */
typedef struct ulog_mutex_struct ulog_mutex;

/**
 * \brief Common operations on ulog_mutex object.
 * \param self The ulog_mutex object on which we'll operate
 * \return Zero on success, else error code.
 *
 * Possible error codes:
 * 1. EINVAL - invalid value of self given;
 * 2. EIO - operation failed.
 */
typedef ulog_status ( * ulog_mutex_func )( ulog_mutex const self );

/**
 * \brief Forward declaration of ulog_mutex state object.
 */
typedef struct ulog_mutex_state_struct ulog_mutex_state;

/**
 * \brief Declaration of mutex object.
 */
struct ulog_mutex_struct
{
    ulog_mutex_state * state; /** Object's state */
    ulog_mutex_func lock; /** Mutex locking method. */
    ulog_mutex_func unlock; /** Mutex unlocking method. */
};

/**
 * \brief Declaration of type returned by ulog_mutex_setup.
 *
 * If status is nonzero then state of mutex is undefined.
 */
typedef struct
{
    ulog_status status; /** Zero on success, else error code. */
    ulog_mutex mutex; /** Mutex object. */
}
ulog_mutex_ctrl;

/**
 * \brief Creates simple mutex.
 * \return Structure containing error code and mutex object.
 * \see ulog_mutex_setup_result
 *
 * Possible error codes:
 * 1. ENOMEM - couldn't allocate memory for mutex state;
 * 2. EIO - mutex initialization failed.
 * Mutex inside the control object is usable only if returned status is zero.
 */
ulog_mutex_ctrl ulog_mutex_setup( void );

/**
 * \brief Safely destroys a ulog_mutex object.
 * \param ctrl Control object returned by ulog_mutex_setup.
 * \return Zero on success, else error code.
 * \see ulog_mutex_setup
 *
 * Possible error codes:
 * 1. EINVAL - invalid mutex control object given;
 * 2. EIO - failure destroying the mutex.
 * After cleanup finishes successfully, using the mutex in control object
 * is undefined.
 */
ulog_status ulog_mutex_cleanup( ulog_mutex_ctrl const ctrl );

#endif /* ULOG_MUTEX_H__ */

