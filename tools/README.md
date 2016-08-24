pingus-cvs-build:
  Shell script that downloads and builds Pingus and all the dependencies
  automatically from CVS

clanlib/:
  A bunch small bugfix patches for ClanLib, which aren't in the
  official version.

extract-gettext.*:
  extracts english messages from xml files and puts them into a .po
  style file


Converting old .xml levels into .pingus format
----------------------------------------------

Converting is a multi stage process, first step is to update old v1 `.xml`
format to a newer v2 `.xml`:

    xsltproc tools/pingusv1tov2.xsl your_level_v1.xml > your_level_v2.xml

Next step is converting the `.xml` to the s-expression based `.pingus` format:

    ruby tools/xml2sexpr.rb your_level_v2.xml your_level_v2.pingus

Final step is updating old resource path names into new ones:

    sed -i -f tools/pingus-old-resource-convert.sed your_level_v2.pingus


# EOF #