AC_DEFUN(AC_LIB_CLANLIB, [
	echo "checking for ClanLib... assuming yes, but haven't tested";
	LIBS="-lclan -lclanlayer2 $LIBS"]
	dnl This next check is dirty hack, it doesn't check for ClanLib, only
	dnl for its headers, and it also links some untested libs to it.
	dnl The following entry should enable static linkage
	dnl LIBS="-lz  /usr/local/lib/libclan.a /usr/local/lib/libclanlayer2.a /usr/lib/libz.a /usr/local/lib/libHermes.a $LIBS"
	)

dnl PINGUS_EXPAND_DIR(VARNAME, DIR)
dnl expands occurrences of ${prefix} and ${exec_prefix} in the given DIR,
dnl and assigns the resulting string to VARNAME
dnl example: PINGUS_EXPAND_DIR(LOCALEDIR, "$datadir/locale")
dnl eg, then: AC_DEFINE_UNQUOTED(LOCALEDIR, "$LOCALEDIR")
dnl by Alexandre Oliva 
dnl from http://www.cygnus.com/ml/automake/1998-Aug/0040.html
AC_DEFUN(PINGUS_EXPAND_DIR, [
        $1=$2
        $1=`(
            test "x$prefix" = xNONE && prefix="$ac_default_prefix"
            test "x$exec_prefix" = xNONE && exec_prefix="${prefix}"
            eval echo \""[$]$1"\"
        )`
])

AC_DEFUN(PINGUS_EXTRA_PATHS,
[

AC_ARG_WITH(extra-includes,
[  --with-extra-includes=DIR
                          add extra include paths],
  use_extra_includes="$withval",
  use_extra_includes=NO
)
if test -n "$use_extra_includes" && \
        test "$use_extra_includes" != "NO"; then
  ac_save_ifs=$IFS
  IFS=':'
  for dir in $use_extra_includes; do
    extra_includes="$extra_includes -I$dir"
  done
  IFS=$ac_save_ifs
  CPPFLAGS="$CPPFLAGS $extra_includes"
fi

AC_ARG_WITH(extra-libs,
[  --with-extra-libs=DIR   add extra library paths], 
  use_extra_libs=$withval,
  use_extra_libs=NO
)
if test -n "$use_extra_libs" && \
        test "$use_extra_libs" != "NO"; then
   ac_save_ifs=$IFS
   IFS=':'
   for dir in $use_extra_libs; do
     extra_libraries="$extra_libraries -L$dir"
   done
   IFS=$ac_save_ifs
fi

AC_SUBST(extra_includes)
AC_SUBST(extra_libraries)

])
