/**
 * \author      Mateusz Jemielity matthew.jemielity@gmail.com
 * \brief       Definition of ulog_status type.
 * \date        2015/04/25 14:00:11 AM
 * \file        status.h
 * \version     1.0
 *
 *
 **/

#ifndef ULOG_STATUS_H__
# define ULOG_STATUS_H__

/**
 * \brief Definition of ulog_status type.
 *
 * Main type for reporting all kind of errors in ulog library.
 * The code values are compatible with errno.
 */
typedef struct
{
    /** Status code compatible with errno. */
    int code;
}
ulog_status;

/**
 * \brief Translates int to ulog_status.
 * \param code Integer to translate.
 * \return Valid ulog_status representing the given code.
 *
 * Note that definition of ulog_status may change. It is better to always
 * use this method for translation instead of direct use of ulog_status
 * internal structure.
 */
ulog_status
ulog_status_from_int( int const code );

/**
 * \brief Translates ulog_status to int for easy manipulation and comparison.
 * \param status The ulog_status structure to translate.
 * \return Integer value representing the status, errno-compatible.
 *
 * Note that definition of ulog_status may change. It is better to always
 * use this method for translation instead of direct use of ulog_status
 * internal structure.
 */
int
ulog_status_to_int( ulog_status const status );

#endif /* ULOG_STATUS_H__ */

