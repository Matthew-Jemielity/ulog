AM_CFLAGS = -Wall -Wextra -pedantic -I$(top_srcdir)/inc

lib_LTLIBRARIES = libulog.la
libulog_la_SOURCES = \
    inc/ulog/listable.h \
    inc/ulog/mutex.h \
    src/listable.c \
    src/mutex.c \
    src/status.c \
    src/ulog.c
libulog_la_LDFLAGS = -version-info 1:0:0
include_HEADERS = \
    inc/ulog/status.h \
    inc/ulog/ulog.h \
    inc/ulog/universal.h

ULOG_UNIT_TESTS = \
    test/test_call_01 \
    test/test_duplicate_01 \
    test/test_listable_add_01 \
    test/test_listable_foreach_01 \
    test/test_listable_remove_01 \
    test/test_listable_simple_01 \
    test/test_log_levels_01 \
    test/test_log_levels_02 \
    test/test_log_level_to_char \
    test/test_mutex_cleanup_01 \
    test/test_mutex_lock_01 \
    test/test_mutex_lock_02 \
    test/test_mutex_setup_01 \
    test/test_mutex_setup_02 \
    test/test_mutex_simple_01 \
    test/test_mutex_simple_02 \
    test/test_mutex_threaded_c99_01 \
    test/test_mutex_unlock_01 \
    test/test_null_01 \
    test/test_simple_01 \
    test/test_simple_02 \
    test/test_ulog_obj_cleanup_01 \
    test/test_ulog_obj_get_01 \
    test/test_ulog_obj_setup_01 \
    test/test_ulog_obj_verbosity_01
TESTS = $(ULOG_UNIT_TESTS)
check_PROGRAMS = $(ULOG_UNIT_TESTS)

test_test_call_01_SOURCES = test/test_call_01.c
test_test_call_01_LDADD = libulog.la

test_test_duplicate_01_SOURCES = test/test_duplicate_01.c
test_test_duplicate_01_LDADD = libulog.la

test_test_listable_add_01_SOURCES = test/test_listable_add_01.c
test_test_listable_add_01_LDADD = libulog.la

test_test_listable_foreach_01_SOURCES = test/test_listable_foreach_01.c
test_test_listable_foreach_01_LDADD = libulog.la

test_test_listable_remove_01_SOURCES = test/test_listable_remove_01.c
test_test_listable_remove_01_LDADD = libulog.la

test_test_listable_simple_01_SOURCES = test/test_listable_simple_01.c
test_test_listable_simple_01_LDADD = libulog.la

test_test_log_levels_01_SOURCES = test/test_log_levels_01.c
test_test_log_levels_01_LDADD = libulog.la

test_test_log_levels_02_SOURCES = test/test_log_levels_02.c
test_test_log_levels_02_LDADD = libulog.la

test_test_log_level_to_char_SOURCES = test/test_log_level_to_char.c
test_test_log_level_to_char_LDADD = libulog.la

test_test_mutex_cleanup_01_SOURCES = test/test_mutex_cleanup_01.c
test_test_mutex_cleanup_01_LDADD = libulog.la

test_test_mutex_lock_01_SOURCES = test/test_mutex_lock_01.c
test_test_mutex_lock_01_LDADD = libulog.la

test_test_mutex_lock_02_SOURCES = test/test_mutex_lock_02.c
test_test_mutex_lock_02_LDADD = libulog.la

test_test_mutex_setup_01_SOURCES = test/test_mutex_setup_01.c
test_test_mutex_setup_01_LDADD = libulog.la

test_test_mutex_setup_02_SOURCES = test/test_mutex_setup_02.c
test_test_mutex_setup_02_LDADD = libulog.la

test_test_mutex_simple_01_SOURCES = test/test_mutex_simple_01.c
test_test_mutex_simple_01_LDADD = libulog.la

test_test_mutex_simple_02_SOURCES = test/test_mutex_simple_02.c
test_test_mutex_simple_02_LDADD = libulog.la

test_test_mutex_threaded_c99_01_SOURCES = test/test_mutex_threaded_c99_01.c
test_test_mutex_threaded_c99_01_LDADD = libulog.la

test_test_mutex_unlock_01_SOURCES = test/test_mutex_unlock_01.c
test_test_mutex_unlock_01_LDADD = libulog.la

test_test_null_01_SOURCES = test/test_null_01.c
test_test_null_01_LDADD = libulog.la

test_test_simple_01_SOURCES = test/test_simple_01.c
test_test_simple_01_LDADD = libulog.la

test_test_simple_02_SOURCES = test/test_simple_02.c
test_test_simple_02_LDADD = libulog.la

test_test_ulog_obj_cleanup_01_SOURCES = test/test_ulog_obj_cleanup_01.c
test_test_ulog_obj_cleanup_01_LDADD = libulog.la

test_test_ulog_obj_get_01_SOURCES = test/test_ulog_obj_get_01.c
test_test_ulog_obj_get_01_LDADD = libulog.la

test_test_ulog_obj_setup_01_SOURCES = test/test_ulog_obj_setup_01.c
test_test_ulog_obj_setup_01_LDADD = libulog.la

test_test_ulog_obj_verbosity_01_SOURCES = test/test_ulog_obj_verbosity_01.c
test_test_ulog_obj_verbosity_01_LDADD = libulog.la

