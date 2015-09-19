/**
 * \author      Mateusz Jemielity matthew.jemielity@gmail.com
 * \brief       Logging facilities.
 * \date        2015/09/12 21:53:19 PM
 * \file        ulog.h
 * \version     1.1
 *
 *
 **/

#ifndef ULOG_H__
# define ULOG_H__

# define __STDC_FORMAT_MACROS /* needed for PRIu64 */

# include <inttypes.h> /* PRIu64 */
# include <stdarg.h> /* va_list */
# include <stdint.h> /* uint64_t */
# include <ulog/status.h> /* ulog_status */
# include <ulog/universal.h> /* INDIRECT, THREADUNSAFE */

/**
 * \brief Defines type of log level.
 */
typedef enum
{
    /** Unrecoverable errors */
    ERROR,
    /** Problems where recovery is possible */
    WARNING,
    /** Messages that should be logged during normal operation */
    INFO,
    /** Debug-only messages */
    DEBUG
}
ulog_level;
/**
 * \brief Returns single letter representation of log level.
 * \param level Log level.
 * \return Single character representing level.
 * \warning If invalid value is passed, '?' is returned.
 *
 * Level representation:
 * ERROR - 'E'
 * WARNING - 'W'
 * INFO - 'I'
 * DEBUG - 'D'
 */
INDIRECT char
ulog_level_to_char_( ulog_level const level );
/**
 * \brief Returns current time.
 * \return Time in nanoseconds, with at least millisecond precision.
 *
 * The time reported here is equal to the wall clock time, thus it's affected
 * by jumps resulting from changing the wall clock time.
 */
INDIRECT uint64_t
ulog_current_time_( void );
/**
 * \brief Directs output of a log message to registered handlers.
 * \param level Log level.
 * \param format Formatting string, as in printf.
 * \param ... Arguments to output, according to format, as in printf.
 * \see ulog_level
 *
 * This function operates on static values. It's thread-safe.
 */
INDIRECT void
ulog_( ulog_level const level, char const * const format, ... );
/**
 * \defgroup ULOGGERS Group of logging macros.
 * \see ULOG_WRAPPERS
 *
 * The macros in this group are responsible for extended logging. They include
 * current time, file, function name and line from which they are called. The
 * macros defined in this group shouldn't be used directly, only through the
 * wrapper macros defined in group ULOG_WRAPPERS.
 *
 * @{
 */
# define ULOG____( LEVEL, FORMAT, ... ) \
    ulog_( \
        LEVEL, \
        "[%c][%"PRIu64"][%s:%s:%u] " FORMAT "%c", \
        ulog_level_to_char_(LEVEL), \
        ulog_current_time_(), \
        __FILE__, \
        __func__, \
        __LINE__, \
        __VA_ARGS__ \
    )
# define ULOG__( LEVEL, ... ) ULOG____( LEVEL, __VA_ARGS__, '\n' )
/**@}*/
/**
 * \defgroup ULOG_WRAPPERS Convenient wrappers for logging.
 * \warning The first argument must be a string literal.
 * \see ULOGGERS
 *
 * The macros in this group wrap around logging macros. They provide convenient
 * and easy to use way of logging messages. They should be used in way similar
 * to printf, i.e. UDEBUG( "This is a debug message: %s" , "argument" );, with
 * an exception that string format must be a literal string.
 *
 * @{
 */
# define UERROR( ... ) ULOG__( ERROR, __VA_ARGS__ )
# define UWARNING( ... ) ULOG__( WARNING, __VA_ARGS__ )
# define UINFO( ... ) ULOG__( INFO, __VA_ARGS__ )
# define UDEBUG( ... ) ULOG__( DEBUG, __VA_ARGS__ )
/**@}*/
/**
 * \brief Forward declaration of ulog_obj's private data type.
 */
typedef struct ulog_obj_private_struct ulog_obj_private;
/**
 * \brief Forward declaration of ulog_obj's table of operation.
 */
typedef struct ulog_obj_op_table_struct ulog_obj_op_table;
/**
 * \brief Definition of ulog_obj type.
 *
 * Allows setup and control of ulog framework, adding handlers, etc.
 * Sample usage:
 * ulog_obj * const u = ulog_obj_get();
 * u->op->setup(u);
 * u->op->add(u, some_handler);
 * UERROR("test");
 * u->op->remove(u, some_handler);
 * u->op->cleanup(u);
 */
typedef struct
{
    /** Private data for ulog_obj. */
    ulog_obj_private * const state;
    /** Table of operations for ulog_obj. */
    ulog_obj_op_table const * const op;
}
ulog_obj;
/**
 * \brief Defines type of setup or cleanup operations.
 * \param self The ulog_obj object on which we operate.
 * \returns Status object.
 * \see ulog_status
 * \see ulog_obj
 * \see ulog_mutex_ctrl_op
 * \see ulog_mutex_op
 *
 * This type defines operations that should be used at beginning and end of
 * ulog framework's lifecycle. While ulog_obj_get() function returns pointer
 * to static ulog_obj object, it is not initialized. The setup() operation
 * must be called, after which ulog framework is ready to register handlers
 * with its add() operation. By default no handlers are registered. After
 * registration ulog wrapper macros can be used to log messages. At the end
 * of program lifecycle handler can be unregistered eeither by hand with a
 * remove() operation call, or automatically with cleanup() operation call.
 * Cleanup frees resources used by ulog framework. The operations using this
 * type allocate or free resources for a static object, therefore they are
 * not thread-safe.
 * Possible status codes:
 * 1. setup:
 *    a. EINVAL - invalid ulog_obj object given;
 *    b. EALREADY - ulog framework already set up;
 *    d. any status code returned by ulog_mutex's setup() operation.
 * 2. cleanup:
 *    a. EINVAL - invalid ulog_obj object given;
 *    b. EALREADY - ulog framework already cleaned up;
 *    c. any status code returned by ulog_mutex's lock(), unlock(), cleanup().
 */
typedef THREADUNSAFE ulog_status
( * ulog_obj_ctrl_op )( ulog_obj const * const self );
/**
 * \brief Definition of a log handler function.
 * \param level Log level.
 * \param format Formatting string, as in printf.
 * \param args Arguments for the format string, as in vprintf.
 * \warning Handler implementations must be thread-safe.
 * \see ulog_level
 *
 * User-defined function of this type can be registered in ulog framework.
 * Afterwards they are guaranteed to be called once if logging wrapper macro
 * is called and it is not ignored according to log level setting.
 */
typedef void
( * ulog_handler_fn )(
    ulog_level const level,
    char const * const format,
    va_list args
);
/**
 * \brief Defines type of operations for adding or removing a log handler.
 * \param self The ulog_obj object on which we'll operate.
 * \param handler Handler for log messages.
 * \return Status object.
 * \see ulog_status
 * \see ulog_obj
 * \see ulog_handler_fn
 * \see ulog_mutex_op
 * \see ulog_list_op
 *
 * By default the logging system doesn't have any handlers registered. This
 * means logging is a no-op. By adding a handler we add an output for log
 * messages. These operations may add a specific handler or remove one from
 * the list of all handlers set in self object. The operations of this type
 * are thread-safe.
 * Possible error codes:
 * 1. add:
 *    a. EINVAL - invalid ulog_obj given;
 *    b. ENOTCONN - ulog framework not initialized;
 *    c. ENOMEM - failed to allocate memory for handler holder structure;
 *    d. EEXIST - handler already is on the list of handlers;
 *    e. any status code returned by ulog_mutex's lock() and unlock();
 *    f. any status code returned by ulog_list_ctrl add();
 * 2. remove:
 *    a. EINVAL - invalid ulog_obj given;
 *    b. ENOTCONN - ulog framework not initialized;
 *    c. ENODATA - handler is not on the list;
 *    d. any status code returned by ulog_mutex's lock() and unlock();
 *    e. any status code returned by ulog_list_ctrl remove().
 */
typedef ulog_status
( * ulog_obj_op )(
    ulog_obj const * const self,
    ulog_handler_fn const handler
);
/**
 * \brief Sets minimum log level below which messages will be ignored.
 * \param self The ulog_obj object on which we'll operate.
 * \param verbosity Minimal log level.
 * \return Status object.
 * \see ulog_status
 * \see ulog_obj
 * \see ulog_level
 * \see ulog_mutex_op
 *
 * By default the minimum log level is set to DEBUG. This means all logs
 * will reach user-supplied handlers. By setting the minimum log level to
 * a higher value, all logs below it will be ignored and will never reach
 * user handlers. This allows easy setting of global log verbosity. This
 * operation is thread-safe.
 * Possible error codes:
 * 1. EINVAL - invalid ulog_obj given;
 * 2. ENOTCONN - ulog framework not initialized;
 * 3. ENODATA - invalid value of verbosity given;
 * 4. any status code returned by ulog_mutex's lock() and unlock().
 */
typedef ulog_status
( * ulog_obj_verbosity_op )(
    ulog_obj const * const self,
    ulog_level const verbosity
);
/**
 * \brief Table of operations for ulog_obj.
 * \see ulog_obj_op_table
 * \see ulog_obj_ctrl_op
 * \see ulog_obj_op
 * \see ulog_obj_verbosity_op
 */
struct ulog_obj_op_table_struct
{
    /** Sets up and initializes ulog_obj. */
    ulog_obj_ctrl_op const setup;
    /** Cleans up and frees ulog_obj's resources. */
    ulog_obj_ctrl_op const cleanup;
    /** Adds handler to log object. */
    ulog_obj_op const add;
    /** Removes handler from log object. */
    ulog_obj_op const remove;
    /** Sets minimum log verbosity. */
    ulog_obj_verbosity_op const verbosity;
};
/**
 * \brief Returns the ulog_obj controlling ulog framework.
 * \return Pointer to singular static instance of ulog_obj.
 */
ulog_obj const *
ulog_obj_get( void );

#endif /* ULOG_H__ */

