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
	glib-compile-resources --target=$@ --sourcedir=$(top_srcdir)/theme --manual-register --generate-header --c-name sol_resource $<

theme-resources.c: $(top_srcdir)/theme/theme.gresource.xml $(theme_resources)
	glib-compile-resources --target=$@ --sourcedir=$(top_srcdir)/theme --manual-register --generate-source --c-name sol_resource $<

AM_CFLAGS = \
	-fstack-protector -Wall -pedantic \
	-Wstrict-prototypes -Wundef -fno-common \
	-Werror-implicit-function-declaration \
	-Wformat -Wformat-security -Werror=format-security \
	-Wconversion -Wunused-variable -Wunreachable-code \
	-Wall -W -D_FORTIFY_SOURCE=2 -std=c11


# Mainly to avoid compiler warnings.
noinst_LTLIBRARIES = libsolus-mate-theme-assets.la

libsolus_mate_theme_assets_la_SOURCES = \
	theme-resources.h		\
	theme-resources.c

libsolus_mate_theme_assets_la_CFLAGS = \
	$(AM_CFLAGS)	 \
	$(THEME_CFLAGS)	\
	-Wno-pedantic

libsolus_mate_theme_assets_la_LIBADD = \
	$(THEME_LIBS)

# Main engine
engine_LTLIBRARIES = libsolus-mate-theme.la
enginedir = $(libdir)/mate-notification-daemon/engines

libsolus_mate_theme_la_SOURCES =	\
	src/engine-api.h		\
	src/engine.c			\
	src/notification-window.h	\
	src/notification-window.c

libsolus_mate_theme_la_CFLAGS = \
	$(AM_CFLAGS)	 \
	$(THEME_CFLAGS)

libsolus_mate_theme_la_LIBADD = \
	libsolus-mate-theme-assets.la \
	$(THEME_LIBS)

libsolus_mate_theme_la_LDFLAGS = 	\
	-module 		 	\
	-avoid-version	 	\
	-fvisibility=hidden
