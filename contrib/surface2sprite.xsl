<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  
  <xsl:output 
    method="xml" 
    indent="yes" 
    encoding="ISO-8859-1" />

  <xsl:template match="node()|@*">
    <xsl:copy><xsl:apply-templates select="@* | node()" /></xsl:copy>
  </xsl:template>

  <xsl:template match="surface">
    <xsl:choose>
      <xsl:when test="@array">
        <sprite name="{@name}">
          <image file="{@file}">
            <grid
              pos="{@x},{@y}"
              size="{@width},{@height}"
              array="{@array}" />
          </image>
        </sprite>
      </xsl:when>
      <xsl:otherwise>
        <sprite name="{@name}">
          <image file="{@file}" />
        </sprite>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

</xsl:stylesheet>
