#  $Id: configure.ac,v 1.22 2004/02/10 14:07:29 grumbel Exp $
#
#  Pingus - A free Lemmings clone
#  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
#
#  This program is free software; you can redistribute it and/or
#  modify it under the terms of the GNU General Public License
#  as published by the Free Software Foundation; either version 2
#  of the License, or (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

dnl Process this file with autoconf to produce a configure script.
AC_INIT(Pingus, 0.7.0)
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_SRCDIR(src/pingus_main.cxx)
AM_INIT_AUTOMAKE(dist-bzip2 dist-zip)

dnl Checks for programs.
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_LANG(C++)
AC_C_BIGENDIAN
PINGUS_EXTRA_PATHS

AC_MSG_CHECKING([whether debugging should be enabled])
AC_ARG_ENABLE(debug, AC_HELP_STRING([--enable-debug], [enable debug support]),,
              [enable_debug="no"])
AC_MSG_RESULT($enable_debug)
if test "x$enable_debug" != "xno"; then
   CXXFLAGS="-g -ggdb -O0 -Wall -Werror"
fi

AC_ARG_WITH(gettext,
	[  --without-gettext       disable support for multiple languages])

if test "x$with_gettext" != "xno"; then
	dnl -- gettext macros --
	ALL_LINGUAS="cs de da es fi fr it no pt sr sv tr nl"

	dnl AM_WITH_NLS
	dnl AM_LC_MESSAGES
        AM_ICONV
	AM_GNU_GETTEXT
else
	echo "Gnu gettext disabled"
	dnl We probally need a AC_SUBST(@INTLLIBS@) here
fi

dnl -- Static Libaries --
AC_PROG_RANLIB

REQUIRED_CLANLIB_VERSION="0.7.9"

PKG_CHECK_MODULES(PINGUS,
 [
  clanCore-0.7           >= $REQUIRED_CLANLIB_VERSION
  clanApp-0.7            >= $REQUIRED_CLANLIB_VERSION
  clanDisplay-0.7        >= $REQUIRED_CLANLIB_VERSION
  clanGL-0.7             >= $REQUIRED_CLANLIB_VERSION
  clanSDL-0.7            >= $REQUIRED_CLANLIB_VERSION
  clanGUI-0.7            >= $REQUIRED_CLANLIB_VERSION
  clanGUIStyleSilver-0.7 >= $REQUIRED_CLANLIB_VERSION
  clanSound-0.7          >= $REQUIRED_CLANLIB_VERSION
  clanVorbis-0.7         >= $REQUIRED_CLANLIB_VERSION
  clanMikMod-0.7         >= $REQUIRED_CLANLIB_VERSION
  ],
 [])

AC_SUBST(PINGUS_CFLAGS)
AC_SUBST(PINGUS_LIBS)

dnl Checks for header files.
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_CHECK_HEADERS(unistd.h)

AC_CHECK_HEADERS(sstream)
dnl => HAVE_SSTREAM
if test "$ac_cv_header_sstream" = "no"; 
then
	AC_CHECK_HEADER(strstream)
	if test "$ac_cv_header_strstream" = "no" -a "$ac_cv_header_strstream_h" = "no";
	then
		AC_MSG_ERROR(could not find either <sstream> or <strstream>.)
	fi
fi

dnl Checks for typedefs, structures, and compiler characteristics.
dnl AC_C_CONST

dnl Checks for library functions.
AC_FUNC_FNMATCH
AC_TYPE_SIGNAL
AC_CHECK_FUNCS(select strdup)

dnl Checking for the install prefix and then use it to define PINGUS_DATADIR
PINGUS_EXPAND_DIR(pingus_datadir, "$datadir/games/$PACKAGE")
AC_DEFINE_UNQUOTED(PINGUS_DATADIR, "$pingus_datadir", [Location of Pingus data files])

dnl -- Define the localedir, needed for gettext --
PINGUS_EXPAND_DIR(localedir, "$datadir/locale")
AC_DEFINE_UNQUOTED(LOCALEDIR, "$localedir", [Location of gettext files])

AC_SUBST(PINGUS_CFLAGS)
AC_SUBST(PINGUS_LIBS)

# po/Makefile.in intl/Makefile <- this must be in one line, if its not
# po/Makefile will not be generated.

AC_OUTPUT( Makefile  m4/Makefile  intl/Makefile 
           contrib/Makefile 
           src/Makefile 
           src/actions/Makefile
	   src/caimagemanipulation/Makefile
           src/colliders/Makefile
           src/gui/Makefile
           src/input/Makefile
	   src/input/axes/Makefile
	   src/input/buttons/Makefile
	   src/input/pointers/Makefile
	   src/input/scrollers/Makefile
           src/movers/Makefile
           src/particles/Makefile
           src/sound/Makefile
           src/worldobjs/Makefile
	   src/worldmap/Makefile
           data/Makefile 
           data/controller/Makefile 
           data/sounds/Makefile 
           data/music/Makefile 
           data/levels/Makefile
           data/prefabs/Makefile
           data/themes/Makefile
           data/worldmaps/Makefile
           data/demos/Makefile
           data/data/Makefile
           data/images/Makefile
           doc/Makefile
           po/Makefile.in
          )

# find . -name "Makefile.am" | grep "\./data/images" | grep -v old | grep -v unsorted | sed "s/^\.\//  /" | sed "s/.am\$//"


# ----------------------------------------

dnl Add here a list of the supported features

echo ""
echo "Pingus is now configured, now you can start 'make'."
echo ""

dnl Local Variables:
dnl compile-command: "./autogen.sh && ./configure"
dnl End:
