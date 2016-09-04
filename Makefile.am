ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = ${top_srcdir}/README.md \
			 ${top_srcdir}/LICENSE

NULL =
CLEANFILES =

AM_CFLAGS = \
	-fstack-protector -Wall -pedantic \
	-Wstrict-prototypes -Wundef -fno-common \
	-Werror-implicit-function-declaration \
	-Wformat -Wformat-security -Werror=format-security \
	-Wconversion -Wunused-variable -Wunreachable-code \
	-Wall -W -D_FORTIFY_SOURCE=2 -std=c11


engine_LTLIBRARIES = libsolus.la
enginedir = $(libdir)/mate-notification-daemon/engines

libsolus_la_SOURCES = \
	src/engine.c

libsolus_la_CFLAGS = \
	$(AM_CFLAGS)	 \
	$(THEME_CFLAGS)

libsolus_la_LIBADD = \
	$(THEME_LIBS)

libsolus_la_LDFLAGS = 	\
	-module 		 	\
	-avoid-version	 	\
	-fvisibility=hidden

# Just for our own testing right now, no lib
noinst_PROGRAMS = dummy-test

dummy_test_SOURCES = \
	src/util.h		 \
	src/main.c

dummy_test_CFLAGS = \
	$(AM_CFLAGS) \
	$(THEME_CFLAGS)

dummy_test_LDADD = \
	$(THEME_LIBS)  \
	libsolus.la

