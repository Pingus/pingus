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

  <xsl:template match="edge">
    <edge>
      <name><xsl:value-of select="@name" /></name>
      <source><xsl:value-of select="@source" /></source>
      <destination><xsl:value-of select="@destination" /></destination>
      <positions>
        <xsl:apply-templates />
      </positions>
    </edge>
  </xsl:template> 

  <xsl:template match="resource">
    <xsl:variable name="datafile">
      <xsl:value-of select="resource-datafile" />
    </xsl:variable>
    <xsl:variable name="ident">
      <xsl:value-of select="resource-ident" />
    </xsl:variable>
    <image>
      <xsl:call-template name="replace-alias">
        <xsl:with-param name="text" select="concat(translate($datafile, '-', '/'), '/', $ident)" />
      </xsl:call-template>
    </image>
    <modifer><xsl:value-of select="modifier" /></modifer>
  </xsl:template>

  <xsl:template name="replace-alias">
    <xsl:param name="text" />
    <xsl:choose>
      <xsl:when test="document('../data/data/alias.xml')/resources/alias[@name = $text]/@link">
        <xsl:value-of select="document('../data/data/alias.xml')/resources/alias[@name = $text]/@link" />
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$text" />
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="objects/surface">
    <surface>
      <name><xsl:value-of select="@name"/></name>
      <xsl:apply-templates />
    </surface>
  </xsl:template>

  <xsl:template match="x-pos">
    <x><xsl:apply-templates /></x>
  </xsl:template>

  <xsl:template match="y-pos">
    <y><xsl:apply-templates /></y>
  </xsl:template>

  <xsl:template match="z-pos">
    <z><xsl:apply-templates /></z>
  </xsl:template>


</xsl:stylesheet>