/**
 * \author      Mateusz Jemielity matthew.jemielity@gmail.com
 * \brief       Definition of ulog_status type.
 * \date        2015/04/25 14:00:11 AM
 * \file        status.h
 * \version     1.1
 *
 *
 **/

#ifndef ULOG_STATUS_H__
# define ULOG_STATUS_H__

# include <stdbool.h> /* bool */

# ifdef __cplusplus
extern "C" {
# endif /* __cplusplus */

/**
 * \brief Definition of ulog_status type.
 *
 * Main type for reporting all kinds of errors in ulog library.
 * The code values are compatible with errno (i.e. ENOMEM when
 * out of memory, etc.). The description string is optional and
 * by default equal to "no details available".
 */
typedef struct
{
    /** Status code compatible with errno. */
    int code;
    /** String literal with detailed status description. */
    char const * description;
}
ulog_status;
/**
 * \brief Creates valid status from given code and description.
 * \param code Integer to translate.
 * \param description Description to include in status.
 * \warning Description used as-is, caller responsible for memory management.
 * \see ulog_status
 *
 * If NULL is passed as description value, default description will be used.
 * The following is always true:
 * code == ulog_status_to_int(ulog_status_descriptive(code, "string"))
 */
ulog_status
ulog_status_descriptive( int const code, char const * const description );
/**
 * \brief Translates ulog_status to int for easy manipulation and comparison.
 * \param status The ulog_status structure to translate.
 * \return Integer value representing the status, errno-compatible.
 * \see ulog_status
 *
 * Note that definition of ulog_status may change. It is better to always
 * use this method for translation instead of direct use of ulog_status
 * internal structure.
 * The following is always true:
 * code == ulog_status_to_int(ulog_status_descriptive(code, "string"))
 */
int
ulog_status_to_int( ulog_status const status );
/**
 * \brief Checks whether status code represents success.
 * \param status The ulog_status structure to evaluate.
 * \return True if status code is successful, false otherwise.
 * \see ulog_status
 *
 * Note that definition of ulog_status may change. Therefore it is better to
 * always use this function for evaluation instead of checking ulog_status
 * directly. It's also possible that the value of successful code will change,
 * so checking it with ulog_status_to_int may become invalid.
 */
bool
ulog_status_success( ulog_status const status );

# ifdef __cplusplus
}
# endif /* __cplusplus */

#endif /* ULOG_STATUS_H__ */

