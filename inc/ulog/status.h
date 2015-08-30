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
 * \brief Translates int to ulog_status.
 * \param code Integer to translate.
 * \return Valid ulog_status representing the given code.
 * \see ulog_status
 *
 * Note that definition of ulog_status may change. It is better to always
 * use this method for translation instead of direct use of ulog_status
 * internal structure. Status description will be set to default.
 * The following is always true:
 * code == ulog_status_to_int(ulog_status_from_int(code))
 */
ulog_status
ulog_status_from_int( int const code );

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
 * code == ulog_status_to_int(ulog_status_from_int(code))
 */
int
ulog_status_to_int( ulog_status const status );

#endif /* ULOG_STATUS_H__ */

