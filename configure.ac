#
# configure.ac for SFourmi
# by GiTs <dodeskaden@free.fr>
#
#

# We want Autoconf 2.52 at least.
AC_PREREQ(2.52)
AC_INIT(SFourmi, 0.42.4)
# Auxiliary files.
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_FILES(config/Makefile)

test "$prefix" = NONE && prefix=/usr/local

# Automake.
AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)

# We want no optimization (too slow), and a lot of debugging, for both
# C and C++.
#CFLAGS="-O3 -Wall -std=c++17"
CFLAGS="-ggdb -Wall -std=c++17 -fsanitize=address -fno-omit-frame-pointer"
CXXFLAGS=$CFLAGS

# Look for a C++ compiler.
AC_LANG_CPLUSPLUS
AC_PROG_CXX

# Look for a C compiler for lib/.
AC_LANG_C
AC_PROG_CC
dnl Check for sdl
SDL_VERSION=1.2.0
AM_PATH_SDL($SDL_VERSION, :,AC_MSG_ERROR([*** SDL $SDL_VERSION not found ***]))
AC_CHECK_LIB(SDL_image, IMG_Load, [AC_DEFINE(HAVE_LIBIMG, 1,
				 [Define if you have the SDL_image library or equivalent. ]) LIBADD_SDL_IMAGE="-lSDL_image"])
AC_CHECK_LIB(SDL_ttf, TTF_Init, [AC_DEFINE(HAVE_LIBTTF, 1,
				 [Define if you have the SDL_ttf library or equivalent. ]) LIBADD_SDL_TTF="-lSDL_ttf"])
CFLAGS="-I\${top_srcdir}/src/include -I${srcdir} $CFLAGS $SDL_CFLAGS"
CPPFLAGS="$CFLAGS"
LIBS="$LIBS $SDL_LIBS $LIBADD_SDL_TTF $LIBADD_SDL_IMAGE"
LDFLAGS="$LIBS"
# Look for auxiliary programs.
#AM_PROG_LEX
#AC_PROG_YACC
AC_PROG_RANLIB
# Ask for config.h.
AM_CONFIG_HEADER(config.h)
# Ask for the Makefile creations.
AC_DEFINE_UNQUOTED(SFOURMI_DATADIR, "$prefix/share/$PACKAGE/", [Directory of SF datas])
AC_CONFIG_FILES(docs/Makefile  src/include/Makefile
		Makefile lib/Makefile lib/Os/Makefile 
		images/Makefile 
		lib/Os/sdl/Makefile
		src/Makefile src/animaux/Makefile
		src/animaux/autres/Makefile src/animaux/fourmis/Makefile
		src/common/Makefile src/terrain/Makefile)

# Intantiate the output files.
AC_OUTPUT

cat config.h|sed 's/NONE/\/usr/' > config.h
