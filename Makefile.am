VPATH = $(top_srcdir)

lib_LTLIBRARIES = libulog.la
libulog_la_SOURCES = \
    inc/ulog/listable.h \
    inc/ulog/mutex.h \
    inc/ulog/status.h \
    inc/ulog/ulog.h \
    inc/ulog/universal.h \
    src/listable.c \
    src/mutex.c \
    src/status.c \
    src/ulog.c
libulog_la_CFLAGS = -Wall -Wextra -pedantic
libulog_la_CPPFLAGS = -I$(top_srcdir)/inc
libulog_la_LDFLAGS = -version-info 2:1:0

ulog_install_dir = $(includedir)/ulog
ulog_install__HEADERS = \
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
    test/test_simple_03 \
    test/test_ulog_obj_cleanup_01 \
    test/test_ulog_obj_get_01 \
    test/test_ulog_obj_setup_01 \
    test/test_ulog_obj_verbosity_01
TESTS = $(ULOG_UNIT_TESTS)
check_PROGRAMS = $(ULOG_UNIT_TESTS)

TESTS_C_FLAGS = -Wall -Wextra -pedantic
TESTS_CPP_FLAGS = -I$(top_srcdir)/inc
TESTS_LD_ADD = libulog.la

test_test_call_01_SOURCES = test/test_call_01.c
test_test_call_01_CFLAGS = ${TESTS_C_FLAGS}
test_test_call_01_CPPFLAGS = ${TESTS_CPP_FLAGS}
test_test_call_01_LDADD = ${TESTS_LD_ADD}

test_test_duplicate_01_SOURCES = test/test_duplicate_01.c
test_test_duplicate_01_CFLAGS = ${TESTS_C_FLAGS}
test_test_duplicate_01_CPPFLAGS = ${TESTS_CPP_FLAGS}
test_test_duplicate_01_LDADD = ${TESTS_LD_ADD}

test_test_listable_add_01_SOURCES = test/test_listable_add_01.c
test_test_listable_add_01_CFLAGS = ${TESTS_C_FLAGS}
test_test_listable_add_01_CPPFLAGS = ${TESTS_CPP_FLAGS}
test_test_listable_add_01_LDADD = ${TESTS_LD_ADD}

test_test_listable_foreach_01_SOURCES = test/test_listable_foreach_01.c
test_test_listable_foreach_01_CFLAGS = ${TESTS_C_FLAGS}
test_test_listable_foreach_01_CPPFLAGS = ${TESTS_CPP_FLAGS}
test_test_listable_foreach_01_LDADD = ${TESTS_LD_ADD}

test_test_listable_remove_01_SOURCES = test/test_listable_remove_01.c
test_test_listable_remove_01_CFLAGS = ${TESTS_C_FLAGS}
test_test_listable_remove_01_CPPFLAGS = ${TESTS_CPP_FLAGS}
test_test_listable_remove_01_LDADD = ${TESTS_LD_ADD}

test_test_listable_simple_01_SOURCES = test/test_listable_simple_01.c
test_test_listable_simple_01_CFLAGS = ${TESTS_C_FLAGS}
test_test_listable_simple_01_CPPFLAGS = ${TESTS_CPP_FLAGS}
test_test_listable_simple_01_LDADD = ${TESTS_LD_ADD}

test_test_log_levels_01_SOURCES = test/test_log_levels_01.c
test_test_log_levels_01_CFLAGS = ${TESTS_C_FLAGS}
test_test_log_levels_01_CPPFLAGS = ${TESTS_CPP_FLAGS}
test_test_log_levels_01_LDADD = ${TESTS_LD_ADD}

test_test_log_levels_02_SOURCES = test/test_log_levels_02.c
test_test_log_levels_02_CFLAGS = ${TESTS_C_FLAGS}
test_test_log_levels_02_CPPFLAGS = ${TESTS_CPP_FLAGS}
test_test_log_levels_02_LDADD = ${TESTS_LD_ADD}

test_test_log_level_to_char_SOURCES = test/test_log_level_to_char.c
test_test_log_level_to_char_CFLAGS = ${TESTS_C_FLAGS}
test_test_log_level_to_char_CPPFLAGS = ${TESTS_CPP_FLAGS}
test_test_log_level_to_char_LDADD = ${TESTS_LD_ADD}

test_test_mutex_cleanup_01_SOURCES = test/test_mutex_cleanup_01.c
test_test_mutex_cleanup_01_CFLAGS = ${TESTS_C_FLAGS}
test_test_mutex_cleanup_01_CPPFLAGS = ${TESTS_CPP_FLAGS}
test_test_mutex_cleanup_01_LDADD = ${TESTS_LD_ADD}

test_test_mutex_lock_01_SOURCES = test/test_mutex_lock_01.c
test_test_mutex_lock_01_CFLAGS = ${TESTS_C_FLAGS}
test_test_mutex_lock_01_CPPFLAGS = ${TESTS_CPP_FLAGS}
test_test_mutex_lock_01_LDADD = ${TESTS_LD_ADD}

test_test_mutex_lock_02_SOURCES = test/test_mutex_lock_02.c
test_test_mutex_lock_02_CFLAGS = ${TESTS_C_FLAGS}
test_test_mutex_lock_02_CPPFLAGS = ${TESTS_CPP_FLAGS}
test_test_mutex_lock_02_LDADD = ${TESTS_LD_ADD}

test_test_mutex_setup_01_SOURCES = test/test_mutex_setup_01.c
test_test_mutex_setup_01_CFLAGS = ${TESTS_C_FLAGS}
test_test_mutex_setup_01_CPPFLAGS = ${TESTS_CPP_FLAGS}
test_test_mutex_setup_01_LDADD = ${TESTS_LD_ADD}

test_test_mutex_setup_02_SOURCES = test/test_mutex_setup_02.c
test_test_mutex_setup_02_CFLAGS = ${TESTS_C_FLAGS}
test_test_mutex_setup_02_CPPFLAGS = ${TESTS_CPP_FLAGS}
test_test_mutex_setup_02_LDADD = ${TESTS_LD_ADD}

test_test_mutex_simple_01_SOURCES = test/test_mutex_simple_01.c
test_test_mutex_simple_01_CFLAGS = ${TESTS_C_FLAGS}
test_test_mutex_simple_01_CPPFLAGS = ${TESTS_CPP_FLAGS}
test_test_mutex_simple_01_LDADD = ${TESTS_LD_ADD}

test_test_mutex_simple_02_SOURCES = test/test_mutex_simple_02.c
test_test_mutex_simple_02_CFLAGS = ${TESTS_C_FLAGS}
test_test_mutex_simple_02_CPPFLAGS = ${TESTS_CPP_FLAGS}
test_test_mutex_simple_02_LDADD = ${TESTS_LD_ADD}

test_test_mutex_threaded_c99_01_SOURCES = test/test_mutex_threaded_c99_01.c
test_test_mutex_threaded_c99_01_CFLAGS = ${TESTS_C_FLAGS}
test_test_mutex_threaded_c99_01_CPPFLAGS = ${TESTS_CPP_FLAGS}
test_test_mutex_threaded_c99_01_LDADD = ${TESTS_LD_ADD}

test_test_mutex_unlock_01_SOURCES = test/test_mutex_unlock_01.c
test_test_mutex_unlock_01_CFLAGS = ${TESTS_C_FLAGS}
test_test_mutex_unlock_01_CPPFLAGS = ${TESTS_CPP_FLAGS}
test_test_mutex_unlock_01_LDADD = ${TESTS_LD_ADD}

test_test_null_01_SOURCES = test/test_null_01.c
test_test_null_01_CFLAGS = ${TESTS_C_FLAGS}
test_test_null_01_CPPFLAGS = ${TESTS_CPP_FLAGS}
test_test_null_01_LDADD = ${TESTS_LD_ADD}

test_test_simple_01_SOURCES = test/test_simple_01.c
test_test_simple_01_CFLAGS = ${TESTS_C_FLAGS}
test_test_simple_01_CPPFLAGS = ${TESTS_CPP_FLAGS}
test_test_simple_01_LDADD = ${TESTS_LD_ADD}

test_test_simple_02_SOURCES = test/test_simple_02.c
test_test_simple_02_CFLAGS = ${TESTS_C_FLAGS}
test_test_simple_02_CPPFLAGS = ${TESTS_CPP_FLAGS}
test_test_simple_02_LDADD = ${TESTS_LD_ADD}

test_test_simple_03_SOURCES = test/test_simple_03.c
test_test_simple_03_CFLAGS = ${TESTS_C_FLAGS}
test_test_simple_03_CPPFLAGS = ${TESTS_CPP_FLAGS}
test_test_simple_03_LDADD = ${TESTS_LD_ADD}

test_test_ulog_obj_cleanup_01_SOURCES = test/test_ulog_obj_cleanup_01.c
test_test_ulog_obj_cleanup_01_CFLAGS = ${TESTS_C_FLAGS}
test_test_ulog_obj_cleanup_01_CPPFLAGS = ${TESTS_CPP_FLAGS}
test_test_ulog_obj_cleanup_01_LDADD = ${TESTS_LD_ADD}

test_test_ulog_obj_get_01_SOURCES = test/test_ulog_obj_get_01.c
test_test_ulog_obj_get_01_CFLAGS = ${TESTS_C_FLAGS}
test_test_ulog_obj_get_01_CPPFLAGS = ${TESTS_CPP_FLAGS}
test_test_ulog_obj_get_01_LDADD = ${TESTS_LD_ADD}

test_test_ulog_obj_setup_01_SOURCES = test/test_ulog_obj_setup_01.c
test_test_ulog_obj_setup_01_CFLAGS = ${TESTS_C_FLAGS}
test_test_ulog_obj_setup_01_CPPFLAGS = ${TESTS_CPP_FLAGS}
test_test_ulog_obj_setup_01_LDADD = ${TESTS_LD_ADD}

test_test_ulog_obj_verbosity_01_SOURCES = test/test_ulog_obj_verbosity_01.c
test_test_ulog_obj_verbosity_01_CFLAGS = ${TESTS_C_FLAGS}
test_test_ulog_obj_verbosity_01_CPPFLAGS = ${TESTS_CPP_FLAGS}
test_test_ulog_obj_verbosity_01_LDADD = ${TESTS_LD_ADD}

