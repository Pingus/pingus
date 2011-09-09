<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  
  <xsl:output 
     method="text" 
     indent="yes" 
     encoding="UTF-8" /> <!-- ISO-8859-1 -->

  <xsl:template match="/pingus-demo/level">   
    <xsl:text>(level&#10;</xsl:text>
    <xsl:text>  (name "</xsl:text><xsl:value-of  select="text()" /><xsl:text>")</xsl:text>
    <xsl:text>)&#10;</xsl:text>
  </xsl:template>

  <xsl:template match="/pingus-demo/events/pingu-action">
    <xsl:text>(pingu-action </xsl:text>
    <xsl:text>(time </xsl:text><xsl:value-of  select="@time" /><xsl:text>) </xsl:text>
    <xsl:text>(id </xsl:text><xsl:value-of  select="@id" /><xsl:text>) </xsl:text>
    <xsl:text>(action "</xsl:text><xsl:value-of  select="@action" /><xsl:text>")</xsl:text>
    <xsl:text>)&#10;</xsl:text>
  </xsl:template>

  <xsl:template match="text()">
  </xsl:template>
</xsl:stylesheet>
