<?xml version="1.0"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method="text"/>

  <xsl:param name="filename"/>

  <xsl:template match="/">

    <xsl:text># -- Begin Level: </xsl:text>
    <xsl:value-of select="$filename"/>
    <xsl:text>&#x0a;</xsl:text>
    <xsl:apply-templates/>
    <xsl:text># -- End Level: </xsl:text>
    <xsl:value-of select="$filename"/>
    <xsl:text>&#x0a;</xsl:text>
    <xsl:text>&#x0a;</xsl:text>
    <xsl:text>&#x0a;</xsl:text>
  </xsl:template>

  <xsl:template match="/pingus-level/global/levelname">
    <xsl:text># Levelname: &#x0a;</xsl:text>
    <xsl:text>msgid  "</xsl:text>
    <xsl:value-of select="text()"/>
    <xsl:text>"&#x0a;</xsl:text>
    <xsl:text>msgstr ""&#x0a;&#x0a;</xsl:text>
  </xsl:template>

  <xsl:template match="/pingus-level/global/description">
    <xsl:text># Description: &#x0a;</xsl:text>
    <xsl:text>msgid "</xsl:text>
    <xsl:value-of select="text()"/>
    <xsl:text>"&#x0a;</xsl:text>
    <xsl:text>msgstr ""&#x0a;</xsl:text>
  </xsl:template>

  <xsl:template match="text()|@*"/>

</xsl:stylesheet>
