<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  
  <xsl:output 
    method="xml" 
    indent="yes" 
    encoding="UTF-8" /> <!-- ISO-8859-1 -->

  <xsl:template match="node()|@*">
    <xsl:copy><xsl:apply-templates select="@* | node()" /></xsl:copy>
  </xsl:template>
  
  <xsl:template match="/pingus-level/global/width" />
  <xsl:template match="/pingus-level/global/height" />

  <xsl:template match="pingus-level">
    <pingus-level version="1">
      <head>
        <xsl:apply-templates select="global/*|action-list"/>
        <levelsize>
          <width><xsl:value-of  select="/pingus-level/global/width" /></width>
          <height><xsl:value-of select="/pingus-level/global/height" /></height>
        </levelsize>
      </head>
      <objects>
        <xsl:apply-templates select="background|exit|entrance|hotspot|worldobj|liquid|group"/>
      </objects>
    </pingus-level>
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

  <xsl:template match="worldobj">
    <xsl:element name="{@type}">
      <xsl:apply-templates />
    </xsl:element>
  </xsl:template>

  <xsl:template match="liquid">
    <liquid><xsl:apply-templates /></liquid>
  </xsl:template>

  <xsl:template match="exit">
    <xsl:if test="@use-old-pos-handling != '0'">
      <xsl:message terminate="yes">
        old-pos-handling is not supported
      </xsl:message>
    </xsl:if>
    <exit><xsl:apply-templates /></exit>
  </xsl:template>

  <xsl:template match="action-list/*">
    <xsl:element name="{name()}">
      <xsl:value-of select="@count"/>
    </xsl:element>
  </xsl:template>

  <xsl:template match="background">
    <xsl:element name="{concat(@type, '-background')}">
      <xsl:apply-templates />
    </xsl:element>
  </xsl:template>

  <xsl:template match="action-list">
    <actions>
      <xsl:apply-templates />
    </actions>
  </xsl:template>

  <xsl:template match="group">
    <xsl:apply-templates />
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
