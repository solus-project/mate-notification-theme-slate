AC_INIT([mate-notification-theme-slate], 3, [ikey@solus-project.com], [mate-notification-theme-slate], [https://github.com/solus-project/mate-notification-theme-slate/issues])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wno-portability no-dist-gzip dist-xz foreign subdir-objects])
AC_PROG_CC
AC_PROG_CC_STDC
LT_PREREQ(2.2)
AC_CONFIG_HEADERS([config.h])
AC_PREFIX_DEFAULT(/usr/local)
AM_SILENT_RULES([yes])
LT_INIT([disable-static])
AC_CONFIG_MACRO_DIR([m4])

AC_CONFIG_FILES([Makefile])

m4_define([gtk_required_version], [3.18.0])

PKG_CHECK_MODULES(THEME,
        [
         gtk+-3.0 >= gtk_required_version
        ]
)

AC_PATH_PROG(GLIB_COMPILE_RESOURCES, glib-compile-resources)

AC_OUTPUT

AC_MSG_RESULT([
        mate-notification-theme-slate $VERSION
        ========

        prefix:                 ${prefix}
        libdir:                 ${libdir}
        sysconfdir:             ${sysconfdir}
        exec_prefix:            ${exec_prefix}
        bindir:                 ${bindir}
        datarootdir:            ${datarootdir}

        compiler:               ${CC}
        cflags:                 ${CFLAGS}
        ldflags:                ${LDFLAGS}
])
