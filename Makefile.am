AM_CFLAGS = -I$(top_srcdir)/inc

lib_LTLIBRARIES = libulog.la
libulog_la_SOURCES = \
    inc/ulog/mutex.h \
    inc/ulog/pointer_list.h \
    src/mutex.c \
    src/pointer_list.c \
    src/status.c \
    src/ulog.c
libulog_la_LDFLAGS = -version-info 1:0:0
include_HEADERS = \
    inc/ulog/status.h \
    inc/ulog/ulog.h \
    inc/ulog/universal.h


