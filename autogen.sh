rm -f config.cache
aclocal
autoheader
automake --add-missing
autoconf
