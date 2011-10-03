all: pingus.6

%.6: %.xml Makefile
	echo "<!ENTITY pingus_version \"$$(cat ../../VERSION)\">" > pingus.ent
	db2x_xsltproc --stylesheet man "$<" | db2x_manxml
	xmlto xhtml -o pingus.html -m config.xsl pingus.xml
	mv -v pingus.html/index.html pingus.html/index.orig.html
	tidy -q -utf8 -indent -wrap 1000  -o pingus.html/index.html pingus.html/index.orig.html
#	docbook2x-texi --output-dir "../build/doc/" "$<"
#        MANWIDTH=80 man -l pingus.1

# EOF #
