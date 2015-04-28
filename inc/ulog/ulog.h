/**
 * \author      Mateusz Jemielity matthew.jemielity@gmail.com
 * \brief       Logging facilities.
 * \date        2015/04/24 18:53:19 PM
 * \file        ulog.h
 * \version     1.0
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
# include <ulog/universal.h> /* THREADUNSAFE */

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
 * \brief Returns current time.
 * \return Time in nanoseconds, with at least millisecond precision.
 *
 * The time reported here is equal to the wall clock time, thus it's affected
 * by jumps resulting from changing the wall clock time.
 */
INDIRECT uint64_t ulog_current_time( void );

/**
 * \brief Directs output of a log message to registered handlers.
 * \param level Log level.
 * \param format Formatting string, as in printf.
 * \param ... Arguments to output, according to format, as in printf.
 * \see ulog_level
 *
 * This method operates on static values. It's thread-safe.
 */
INDIRECT void
ulog( ulog_level const level, char const * const format, ... );

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
    ulog( \
        LEVEL, \
        "[%"PRIu64"][%s:%s:%u] " FORMAT "%c", \
        ulog_current_time(), \
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
 * to printf, i.e. UDEBUG( "This is a debug message: %s" , "argument" );
 *
 * @{
 */
# define UERROR( ... ) ULOG__( ERROR, __VA_ARGS__ )
# define UWARNING( ... ) ULOG__( WARNING, __VA_ARGS__ )
# define UINFO( ... ) ULOG__( INFO, __VA_ARGS__ )
# define UDEBUG( ... ) ULOG__( DEBUG, __VA_ARGS__ )
/**@}*/

/**
 * \brief Forward declaration of ulog_obj.
 */
typedef struct ulog_obj_struct ulog_obj;

/**
 * \brief Definition of a log handler function.
 * \param level Log level.
 * \param format Formatting string, as in printf.
 * \param args Arguments for the format string, as in vprintf.
 * \warning Handler implementations must be thread-safe.
 */
typedef void ( * ulog_log_handler )(
    ulog_level const level,
    char const * const format,
    va_list args
);

/**
 * \brief Operates on logging framework with a handler for log messages.
 * \param self Log object on which we'll operate.
 * \param handler Handler for log messages.
 * \return Zero on success, else error code.
 *
 * By default the logging system doesn't have any handlers registered. This
 * means logging is a no-op. By adding a handler we add an output for log
 * messages. This method may add a specific handler or remove one from the
 * list of all handlers set in self object.
 * Possible error codes:
 * 1. common:
 * a) EINVAL - invalid ulog_obj given;
 * 2. adding handler:
 * a) ENOMEM - failed to allocate memory for handler holder structure;
 * b) EEXIST - handler already is on the list of handlers;
 * c) codes returned by ulog_mutex locking and unlocking methods;
 * d) codes returned by ulog_pointer_list add method;
 * 3. removing handler:
 * a) codes returned by ulog_mutex  locking and unlocking methods;
 * b) codes returned by ulog_pointer_list remove method.
 * The methods implementing this typedef are thread-safe.
 */
typedef ulog_status ( * ulog_control_func )(
    ulog_obj const self,
    ulog_log_handler const handler
);

typedef struct ulog_obj_state_struct ulog_obj_state;

/**
 * \brief Declaration of log object.
 */
struct ulog_obj_struct
{
    /** Log object state. */
    ulog_obj_state * state;
    /** Adds handler to log object. */
    ulog_control_func add;
    /** Removes handler from log object. */
    ulog_control_func remove;
};

/**
 * \brief Defines return type of ulog_setup.
 */
typedef struct
{
    /** Zero on sucess, else error code. */
    ulog_status status;
    /** Log object. */
    ulog_obj log;
}
ulog_ctrl;

/**
 * \brief Initializes the logging framework.
 * \returns Control structure with error code and log object.
 * \see ulog_ctrl
 * \see ulog_mutex_setup
 *
 * This method must be called first before any logging macros or methods. It
 * initializes the internal structures used when logging. This method works
 * on static objects, which allows for easy, hassle free usage of logging
 * macros. By default, no handlers are registered (therefore logging would
 * do nothing). This method is NOT thread-safe as it has to use static
 * variables. If EALREADY is returned as status, the returned log pointer will
 * point to initialized structure that can be used normally.
 * Possible status codes:
 * 1. EALREADY - logging framework already initialized;
 * 2. any error code returned by ulog_mutex_setup.
 */
THREADUNSAFE ulog_ctrl ulog_setup( void );

/**
 * \brief Deinitializes and frees resources used by the logging framework.
 * \param ctrl Control object to deinitialize and free.
 * \return Zero on success, else error code.
 * \see ulog_mutex_cleanup
 * \see ulog_mutex_func
 * \see ulog_pointer_list_cleanup
 *
 * This method frees internal structures used by logging framework. It operates
 * on static values. It's NOT thread-safe as it has to use static variables.
 * Possible status codes:
 * 1. EALREADY - logging framework already uninitialized;
 * 2. any error codes returned by ulog_mutex lock and unlock methods;
 * 3. any error codee returned by ulog_pointer_list_cleanup;
 * 4. any error code returned by ulog_mutex_cleanup.
 * After the cleanup method returns successfuly, using the log object is
 * undefined.
 */
THREADUNSAFE ulog_status ulog_cleanup( ulog_ctrl const ctrl );

#endif /* ULOG_H__ */

