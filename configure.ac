AC_PREREQ([2.69])
AC_INIT([libsml], [0.0.1], [info@volkszaehler.org])
AC_CONFIG_SRCDIR([sml/src/sml_message.c])
AC_CONFIG_HEADERS([sml/src/config.h])
AC_PREFIX_DEFAULT([/usr/local])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
AC_CHECK_LIB([uuid], [uuid_generate])

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h limits.h stdint.h stdlib.h string.h sys/ioctl.h sys/time.h termios.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# --enable-debug/--disable-debug
AC_ARG_ENABLE(debug,
              [  --enable-debug          debugging mode],
              if test "$enable_debug" = "yes"; then
                  AC_DEFINE(SML_DEBUG, 1, [Define this if you want extra debugging.])
              fi)
AM_CONDITIONAL(SML_DEBUG, test "$enable_debug" = "yes")

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memset select strerror strstr])

AC_CONFIG_FILES([Makefile
                 examples/Makefile
                 sml/Makefile
                 test/Makefile])
AC_OUTPUT
