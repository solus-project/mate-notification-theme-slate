ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = \
	${top_srcdir}/README.md \
	${top_srcdir}/LICENSE \
	${top_srcdir}/theme/theme.gresource.xml \
	${top_srcdir}/theme/notification.ui \
	${top_srcdir}/theme/3.18/theme.css \
	${top_srcdir}/theme/3.18/theme_hc.css \
	${top_srcdir}/theme/3.20/theme.css \
	${top_srcdir}/theme/3.20/theme_hc.css

NULL =

theme_resources = $(shell glib-compile-resources --sourcedir=$(top_srcdir)/theme --generate-dependencies $(top_srcdir)/theme/theme.gresource.xml)

BUILT_SOURCES = \
	theme-resources.h \
	theme-resources.c

CLEANFILES = \
	$(BUILT_SOURCES)

# resources
theme-resources.h: $(top_srcdir)/theme/theme.gresource.xml $(theme_resources)
	glib-compile-resources --target=$@ --sourcedir=$(top_srcdir)/theme --manual-register --generate-header --c-name slate_resource $<

theme-resources.c: $(top_srcdir)/theme/theme.gresource.xml $(theme_resources)
	glib-compile-resources --target=$@ --sourcedir=$(top_srcdir)/theme --manual-register --generate-source --c-name slate_resource $<

AM_CFLAGS = \
	-fstack-protector -Wall -pedantic \
	-Wstrict-prototypes -Wundef -fno-common \
	-Werror-implicit-function-declaration \
	-Wformat -Wformat-security -Werror=format-security \
	-Wconversion -Wunused-variable -Wunreachable-code \
	-Wall -W -D_FORTIFY_SOURCE=2 -std=c11


# Mainly to avoid compiler warnings.
noinst_LTLIBRARIES = libslate-assets.la

libslate_assets_la_SOURCES = \
	theme-resources.h		\
	theme-resources.c

libslate_assets_la_CFLAGS = \
	$(AM_CFLAGS)	 \
	$(THEME_CFLAGS)	\
	-Wno-pedantic

libslate_assets_la_LIBADD = \
	$(THEME_LIBS)

# Main engine
engine_LTLIBRARIES = libslate.la
enginedir = $(libdir)/mate-notification-daemon/engines

libslate_la_SOURCES =	\
	src/engine-api.h		\
	src/engine.c			\
	src/notification-window.h	\
	src/notification-window.c	\
	src/util.h

libslate_la_CFLAGS = \
	$(AM_CFLAGS)	 \
	$(THEME_CFLAGS)

libslate_la_LIBADD = \
	libslate-assets.la \
	$(THEME_LIBS)

libslate_la_LDFLAGS = 	\
	-module 		 	\
	-avoid-version	 	\
	-fvisibility=hidden
