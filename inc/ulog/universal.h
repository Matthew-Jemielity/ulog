/**
 * \author      Mateusz Jemielity matthew.jemielity@gmail.com
 * \brief       Global macros, methods, etc.
 * \date        2015/04/24 18:49:59 AM
 * \file        universal.h
 * \version     1.0
 *
 *
 **/

#ifndef ULOG_UNIVERSAL_H__
# define ULOG_UNIVERSAL_H__

# ifdef __cplusplus
extern "C" {
# endif /* __cplusplus */

/**
 * \brief Specifies that a method shouldn't be used directly.
 *
 * This macro is currently a no-op. It signifies that a method wasn't designed
 * to be used directly and that there are legitimate wrappers that should be
 * used instead.
 */
# ifndef INDIRECT
#  define INDIRECT
# endif /* INDIRECT */
/**
 * \brief Specifies that a method is not thread-safe.
 *
 * Currently a no-op. Signifies that method wasn't designed to be thread-safe
 * (for example it uses static variables).
 */
# ifndef THREADUNSAFE
#  define THREADUNSAFE
# endif /* THREADUNSAFE */
/**
 * \brief Specifies that variable is not used in a compiler-friendly way.
 *
 * We want our code to compile cleanly even with gcc's -pedantic flag. This
 * macro allows to mark a variable that is not used at the moment. Provides
 * a single keyworkd to grep for and even allows for additional functionality,
 * like logging, if the macro is expanded.
 */
# ifndef UNUSED
#  define UNUSED( VAR ) (( void ) ( VAR ))
# endif /* UNUSED */

# ifdef __cplusplus
}
# endif /* __cplusplus */

#endif /* IONIZE_UNIVERSAL_H__ */

