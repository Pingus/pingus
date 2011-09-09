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
    <pingus-level>
      <version>2</version>
      <head>
        <xsl:apply-templates select="global/*|action-list"/>
        <xsl:if test="count(global/music) = 0">
          <music>none</music>
        </xsl:if>
        <levelsize>
          <width><xsl:value-of  select="/pingus-level/global/width" /></width>
          <height><xsl:value-of select="/pingus-level/global/height" /></height>
        </levelsize>
      </head>
      <objects>
        <xsl:apply-templates select="background|exit|entrance|hotspot|worldobj|liquid|group|groundpiece|trap"/>
      </objects>
    </pingus-level>
  </xsl:template>

  <xsl:template match="/pingus-level/global/time/text()">
    <xsl:choose>
      <xsl:when test=".='9000'">
        <xsl:text>-1</xsl:text>
      </xsl:when>
      <xsl:otherwise>
      <xsl:value-of select="." />
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="/pingus-level/global/levelname">
    <xsl:choose>
      <xsl:when test="@lang='en'">
        <levelname><xsl:apply-templates /></levelname>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="/pingus-level/global/description">
    <xsl:choose>
      <xsl:when test="@lang='en'">
        <description><xsl:apply-templates /></description>
      </xsl:when>
    </xsl:choose>
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
    <modifier>
      <xsl:choose>
        <xsl:when test="string(modifier) != ''">
          <xsl:value-of select="modifier" />
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>ROT0</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </modifier>
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
      <xsl:apply-templates select="*"/>
    </xsl:element>
  </xsl:template>

  <xsl:template match="liquid/width">
    <xsl:choose>
      <xsl:when test="../@use-old-width-handling = '0'">
        <repeat><xsl:apply-templates /></repeat>
      </xsl:when>
      
      <xsl:otherwise> <!-- fixme: These must depend on the resources -->
        <xsl:variable name="resourcename" 
                      select="concat(../surface/resource/resource-datafile/text(),':',../surface/resource/resource-ident/text())" />

        <xsl:choose>
          <xsl:when test="$resourcename = 'global:Liquid/water'">
            <repeat><xsl:value-of select="floor(number(text()) div 32)" /></repeat>
          </xsl:when>
          
          <xsl:when test="$resourcename = 'liquids:Liquid/water'">
            <repeat><xsl:value-of select="floor(number(text()) div 32)" /></repeat>
          </xsl:when>
          
          <xsl:otherwise>
            <repeat><xsl:value-of select="floor(number(text()) div 64)" /></repeat>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:otherwise>
    </xsl:choose>    
  </xsl:template>

  <xsl:template match="liquid">
    <liquid><xsl:apply-templates /></liquid>
  </xsl:template>

  <xsl:template match="action-list/*">
    <xsl:element name="{name()}">
        <xsl:choose>
          <xsl:when test="@count">
            <xsl:value-of select="@count"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="text()"/>
          </xsl:otherwise>
        </xsl:choose>
    </xsl:element>
  </xsl:template>

  <xsl:template match="background">
    <xsl:choose>
      <xsl:when test="string(@type) != ''">
        <xsl:element name="{concat(@type, '-background')}">
          <xsl:apply-templates select="*"/>
        </xsl:element>
      </xsl:when>
      <xsl:otherwise>
        <xsl:element name="surface-background">
          <xsl:apply-templates select="*"/>
        </xsl:element>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="action-list">
    <actions>
      <xsl:apply-templates select="*"/>
    </actions>
  </xsl:template>

  <xsl:template match="background[@type='surface']/color">
    <xsl:choose>
      <xsl:when test="red/text() = '0' and green/text() = '0' and blue/text() = '0' and alpha/text() = '1'">
        <color>
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
          <alpha>0</alpha>
        </color>
      </xsl:when>

      <xsl:otherwise>
        <color>
          <xsl:apply-templates />
        </color>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="worldobj[@type='teleporter']">
    <teleporter>
      <target-id><xsl:value-of select="generate-id(.)" /></target-id>
      <xsl:apply-templates select="position" />
    </teleporter>
    <teleporter-target>
      <id><xsl:value-of select="generate-id(.)" /></id>
      <position>
        <x><xsl:value-of select="target/position/x-pos"/></x>
        <y><xsl:value-of select="target/position/y-pos"/></y>
        <z><xsl:value-of select="position/z-pos"/></z>
      </position>      
    </teleporter-target>
  </xsl:template>

  <xsl:template match="group">
    <xsl:apply-templates select="*"/>
  </xsl:template>

  <xsl:template match="x-pos">
    <x><xsl:apply-templates/></x>
  </xsl:template>

  <xsl:template match="y-pos">
    <y><xsl:apply-templates/></y>
  </xsl:template>

  <xsl:template match="z-pos">
    <z><xsl:apply-templates/></z>
  </xsl:template>

  <xsl:template match="groundpiece">
    <groundpiece>
      <type><xsl:value-of select="@type"/></type>
      <xsl:apply-templates select="*"/>
    </groundpiece>
  </xsl:template>

  <xsl:template match="trap">
    <xsl:element name="{type}">
      <xsl:apply-templates select="*"/>
    </xsl:element>
  </xsl:template>
  <xsl:template match="trap/type"/>

  <xsl:template match="surface|position">
    <xsl:copy>
      <xsl:apply-templates select="*"/>
    </xsl:copy>
  </xsl:template>

  <xsl:template match="exit/position">
    <xsl:choose>
      <xsl:when test="../@use-old-pos-handling = '0'">
        <position>
          <xsl:apply-templates />
        </position>
      </xsl:when>
      
      <xsl:otherwise>
        <xsl:variable name="resourcename" 
                      select="concat(../surface/resource/resource-datafile/text(),':',../surface/resource/resource-ident/text())" />

        <position>       
          <xsl:choose>
            <xsl:when test="$resourcename = 'exits:Exits/desert'">
              <x><xsl:value-of select="floor(number(x-pos) + 150 div 2)" /></x>
              <y><xsl:value-of select="number(y-pos) + 100" /></y>
              <z><xsl:value-of select="number(z-pos)" /></z>
            </xsl:when>

            <xsl:when test="$resourcename = 'exits:Exits/stone'">
              <x><xsl:value-of select="floor(number(x-pos) + 75 div 2)" /></x>
              <y><xsl:value-of select="number(y-pos) + 60" /></y>
              <z><xsl:value-of select="number(z-pos)" /></z>
            </xsl:when>

            <xsl:when test="$resourcename = 'exits:Exits/ice'">
              <x><xsl:value-of select="floor(number(x-pos) + 64 div 2)" /></x>
              <y><xsl:value-of select="number(y-pos) + 64" /></y>
              <z><xsl:value-of select="number(z-pos)" /></z>
            </xsl:when>

            <xsl:when test="$resourcename = 'exits:Exits/sortie_anim'">
              <x><xsl:value-of select="floor(number(x-pos) + 78 div 2)" /></x>
              <y><xsl:value-of select="number(y-pos) + 90" /></y>
              <z><xsl:value-of select="number(z-pos)" /></z>
            </xsl:when>

            <xsl:when test="$resourcename = 'exits:Exits/space'">
              <x><xsl:value-of select="floor(number(x-pos) + 154 div 2)" /></x>
              <y><xsl:value-of select="number(y-pos) + 118" /></y>
              <z><xsl:value-of select="number(z-pos)" /></z>
            </xsl:when>

            <xsl:when test="$resourcename = 'global:Exits/stone'">
              <x><xsl:value-of select="floor(number(x-pos) +  75 div 2)" /></x>
              <y><xsl:value-of select="number(y-pos) + 60" /></y>
              <z><xsl:value-of select="number(z-pos)" /></z>
            </xsl:when>

            <xsl:when test="$resourcename = 'exits:Exits/sweetexit'">
              <x><xsl:value-of select="floor(number(x-pos) +  180 div 2)" /></x>
              <y><xsl:value-of select="number(y-pos) + 121" /></y>
              <z><xsl:value-of select="number(z-pos)" /></z>
            </xsl:when>          

            <xsl:when test="$resourcename = 'exits:Exits/crystal'">
              <x><xsl:value-of select="floor(number(x-pos) +  154 div 2)" /></x>
              <y><xsl:value-of select="number(y-pos) + 105" /></y>
              <z><xsl:value-of select="number(z-pos)" /></z>
            </xsl:when>          

            <xsl:otherwise>
              <xsl:message terminate="yes">
                Critical error: resourcename: ((<xsl:value-of select="$resourcename" />))
              </xsl:message>
            </xsl:otherwise>
          </xsl:choose>
        </position>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="exit">
    <xsl:element name="exit">
      <xsl:choose>
        <xsl:when test="string(@owner-id) != ''">
          <xsl:apply-templates />
        </xsl:when>
        <xsl:otherwise>
          <owner-id>0</owner-id>
          <xsl:apply-templates />
        </xsl:otherwise>
      </xsl:choose>
    </xsl:element>
  </xsl:template>
</xsl:stylesheet>
