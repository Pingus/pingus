#  $Id: configure.ac,v 1.14 2003/04/18 17:29:01 grumbel Exp $
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

dnl BUG: This file should be updated with the macros from 'configure.scan'

# The required ClanLib version
CLANLIB_MAJOR_VERSION=0
CLANLIB_MINOR_VERSION=6
CLANLIB_PATCH_VERSION=5

dnl Process this file with autoconf to produce a configure script.
AC_INIT(Pingus, 0.6.0)
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_SRCDIR(src/pingus_main.cxx)
AM_INIT_AUTOMAKE(dist-bzip2)


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

AC_ARG_WITH(gettext,
	[  --without-gettext       disable support for multiple languages])

AC_ARG_WITH(clanGL,
        [  --with-clanGL        enable OpenGL support])

if test "x$with_gettext" != "xno"; then
	dnl -- gettext macros --
	ALL_LINGUAS="cs de fr sr tr it" # nl

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

AC_CHECK_HEADER(ClanLib/core.h,, [echo "Please get ClanLib from http://www.clanlib.org"; exit])

dnl FIXME: we don't need pthread, but some system might need it to succeed at the final link
AC_CHECK_LIB(pthread,   main,,)

dnl ---------------------------------------------------
dnl Check the location of the XML library and -headers.
AC_ARG_WITH(libxml,[  --with-libxml=PREFIX    set the path to your libxml installation])

dnl first check for xml2-config, then xml-config
if test "x$with_libxml" != "x"
then
        AC_MSG_CHECKING([for xml(2)-config])
        XML_CONFIG="$with_libxml/bin/xml2-config"
        if test -x "$XML_CONFIG"
        then
                AC_MSG_RESULT([$XML_CONFIG])
        else
		XML_CONFIG="$with_libxml/bin/xml-config"
		if test -x "$XML_CONFIG"
	        then
			AC_MSG_RESULT([$XML_CONFIG])
		else
                AC_MSG_ERROR([
*** I was not able to find the script \"xml(2)-config\".
*** Did you specify the correct prefix for --with-libxml ?])
		fi
	fi
else
	dnl try xml2-config, then xml-config
	AC_CHECK_PROGS(XML_CONFIG,xml2-config xml-config,no)
        if test "x$XML_CONFIG" = "xno"
        then
		AC_MSG_ERROR([
*** I was not able to find the script \"xml(2)-config\".
*** Please check your installation of libxml.])
	fi
fi

dnl Check the version of libxml.
ac_xml_version=`$XML_CONFIG --version`
case "$ac_xml_version" in
        2.*)
 	AC_DEFINE_UNQUOTED(LIBXML_2,, [Libxml2 is present])
        ;;
        *)
        #AC_CHECK_LIB(xml, xmlIsBlankNode,, [AC_DEFINE_UNQUOTED(NO_XMLISBLANKNODE)])
 	;;
esac

dnl Add the apropriate xml-flags to CPPFLAGS and LIBS.
CPPFLAGS="$CPPFLAGS `$XML_CONFIG --cflags`"
LIBS="$LIBS `$XML_CONFIG --libs`"
dnl ---------------------------------------------------


AC_CHECK_LIB(Hermes,   main,,)

AC_CHECK_LIB( clanCore,   main,, [
	echo "ClanLib installed, but -lclanCore not found, run 'ldconfig' and try again,"
	echo "if that doesn't help have a look at the file config.log and look into"
	echo "the FAQ."
	exit
	])

dnl FIXME: This should fail, if the libs are not detected
AC_CHECK_LIB( clanApp,   main)
AC_CHECK_LIB( clanDisplay,   main)
AC_CHECK_LIB( clanSound,   main)

AC_CHECK_LIB( clanMikMod,   main)
AC_CHECK_LIB( clanVorbis,   main)

#echo "ClanGL: $with_clanGL"
if test "x$with_clanGL" = "xyes"; then
    AC_CHECK_LIB( clanGL,   main)
else
    echo "clanGL support disabled";
fi

dnl AC_CHECK_LIB( clanGUI,   main,, [
dnl 	echo "ClanLib is installed, but clanGUI was not found. You probably"
dnl 	echo "forgot to compile clanGUI, which is part of the ClanLib package"
dnl 	echo "or you need to run 'ldconfig'."
dnl 	echo "If you think you have everything correctly installed have a look at"
dnl 	echo "the file 'config.log' to see what went wrong"
dnl 	exit
dnl 	])


dnl AC_CHECK_PROG( HAVE_CLANLIB_CONFIG, clanlib-config, "yes")

# if test "x$HAVE_CLANLIB_CONFIG" != "xyes" ; then
# 	echo
# 	echo "Couldn't find 'clanlib-config. Make sure that you"
# 	echo "have installed ClanLib and that clanlib-config"
# 	echo "is installed in you PATH.";
# 	echo "Since Pingus does not use clanlib-config you can ignore"
#         echo "this... %-)" 
# dnl 	echo 
# dnl fi

dnl clanlib_config_major_version=`clanlib-config --version | \
dnl            sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
dnl clanlib_config_minor_version=`clanlib-config --version | \
dnl            sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
dnl clanlib_config_micro_version=`clanlib-config --version | \
dnl            sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`

dnl if test $clanlib_config_major_version -eq $CLANLIB_MAJOR_VERSION;
dnl  dnl if test $clanlib_config_minor_version -eq
dnl $CLANLIB_MINOR_VERSION; then dnl if test $clanlib_config_patch_version
dnl -eq $CLANLIB_PATCH_VERSION; then dnl fi dnl fi dnl fi

#  dnl Checking if ClanLib is new enough
#  echo -n "checking for working ClanLib >= $CLANLIB_MAJOR_VERSION.$CLANLIB_MINOR_VERSION.$CLANLIB_PATCH_VERSION... "
#  AC_TRY_RUN([
#  #include <ClanLib/core.h>
#   int main(){
#  	printf ("CL_VERSION: %d\n", CL_CURRENT_VERSION);
#  	printf ("wanted CL_VERSION: %d\n", CL_VERSION($CLANLIB_MAJOR_VERSION,$CLANLIB_MINOR_VERSION,$CLANLIB_PATCH_VERSION));
#   if (CL_CURRENT_VERSION >= CL_VERSION($CLANLIB_MAJOR_VERSION,$CLANLIB_MINOR_VERSION,$CLANLIB_PATCH_VERSION))
#   {
#   	return 0;
#   }
#   else
#   {
#   	return 1;
#   }
#   }
#   ],
#   	[echo "yes"],
#   	[echo ; echo;
#   	 echo "Your ClanLib is to old or some other thing went wrong while compiling"
#   	 echo "the test application, you need at least version $CLANLIB_MAJOR_VERSION.$CLANLIB_MINOR_VERSION.$CLANLIB_PATCH_VERSION"
#   	 echo "or greater. If you have that version installed have a look at the file"
#  	 echo "config.log to see what went wrong."
#   	 echo "The ClanLib webpage is at:"; echo;
#   	 echo "                    http://www.clanlib.org";
#            echo
#   	 echo "The ClanLib source tarball can be found at:"
#   	 echo
#   	 echo "      http://clanlib.org/download/index.html"
#   	 echo
#   	 exit],
#   	[echo "cross compiling detected, assuming that this is correct and continue"]
#   )

AC_CHECK_LIB( clanJPEG,   main,, [
	echo "ClanLib is installed, but clanJPEG was not found. You probably"
	echo "forgot to compile clanJPEG, which is part of the ClanLib package"
	echo "or you need to run 'ldconfig'."
	echo "If you think you have everything correctly installed have a look at"
	echo "the file 'config.log' to see what went wrong"
	exit
	])

AC_CHECK_LIB( clanPNG,   main,, [
	echo "ClanLib is installed, but clanPNG was not found. You probably"
	echo "forgot to compile clanPNG, which is part of the ClanLib package"
	echo "or you need to run 'ldconfig'."
	echo "If you think you have everything correctly installed have a look at"
	echo "the file 'config.log' to see what went wrong"
	exit
	])

AC_CHECK_LIB( clanGUI,   main,, [
	echo "ClanLib is installed, but clanGUI was not found. You probably"
	echo "forgot to compile clanPNG, which is part of the ClanLib package"
	echo "or you need to run 'ldconfig'."
	echo "If you think you have everything correctly installed have a look at"
	echo "the file 'config.log' to see what went wrong"
	exit
	])

# AC_CHECK_PROG(HAVE_DATAFILE_COMPILER, datafile_compiler, "yes")
# if test "x$HAVE_DATAFILE_COMPILER" != "xyes" ; then
# 	echo
# 	echo "Couldn't find the ClanLib datafile_compiler. Make sure"
# 	echo "it is correctly installed and the path were it is"
# 	echo "installed is in you PATH.";
# 	echo
# 	exit;
# fi

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

# po/Makefile.in intl/Makefile <- this must be in one line, if its not
# po/Makefile will not be generated.

AC_OUTPUT( Makefile  m4/Makefile  intl/Makefile 
           contrib/Makefile 
           src/Makefile 
           src/actions/Makefile
	   src/caimagemanipulation/Makefile
           src/colliders/Makefile
           src/editor/Makefile
	   src/editorobjs/Makefile
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
           src/worldobjs/entrances/Makefile
	   src/worldobjsdata/Makefile
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
