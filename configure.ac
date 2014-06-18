#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([jwmtools],[0.1.78],[http://github.com/kostelnik/jwmtools/issues])
AC_CONFIG_SRCDIR([appmenugen/appmenugen.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([scripts])
AC_USE_SYSTEM_EXTENSIONS

DATE='2014-06-17'
AC_SUBST([DATE])

AM_INIT_AUTOMAKE([gnits 1.14 dist-xz no-dist-gzip std-options -Wall color-tests silent-rules tar-pax])
AM_MAINTAINER_MODE

# Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AM_PROG_AR
AC_PROG_MKDIR_P

PKG_PROG_PKG_CONFIG

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lmetar':
AC_CHECK_LIB([metar], [main])

# Checks for header files.
AC_PATH_X
AC_CHECK_HEADERS([shadow.h stdlib.h string.h unistd.h values.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([endpwent getspnam strerror strrchr strstr])

PKG_CHECK_MODULES([JWMT],[cairo pango pangocairo gtk+-2.0 gmodule-2.0 gmodule-export-2.0 libglade-2.0 glib-2.0 xscrnsaver xext x11])
AC_SUBST([JWMT_CFLAGS])
AC_SUBST([JWMT_LIBS])

AC_DEFINE([VERY_OLD_GLIB],[0],[Define to 1 for glib<2.26])
AC_DEFINE_UNQUOTED([SYS_BATTERY_PATH],["/sys/bus/acpi/drivers/battery/PNP0C0A:00/power_supply/C16D/"],
		[Path to acpi battery info])

AC_CONFIG_FILES([Makefile
		 appmenugen/Makefile
		 libonce/Makefile
		 movemousecursor/Makefile
		 scandirstr/Makefile
		 slock/Makefile
		 s/Makefile
		 sysmonitor/Makefile
		 traybattery/Makefile
		 traykeymap/Makefile
		 traymonitor/Makefile
		 trayscript/Makefile
		 trayusermount/Makefile
		 trayvolume/Makefile
		 trayweather/Makefile
                 xidletools/Makefile])
dnl AC_CONFIG_SUBDIRS([jwm])
AC_OUTPUT
