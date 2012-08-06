AC_PREREQ([2.69])
AC_INIT([piimg], [0.1], [alex@alexchamberlain.co.uk])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/piimg.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_CC_C_O

# Checks for libraries.
AC_CHECK_LIB([parted], [ped_device_get])

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stdint.h stdlib.h string.h sys/mount.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_TYPE_UINT64_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([ftruncate memset strerror])

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
