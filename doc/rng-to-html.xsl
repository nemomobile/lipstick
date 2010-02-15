<?xml version="1.0" encoding="utf-8"?>

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:a="http://relaxng.org/ns/compatibility/annotations/1.0"
                xmlns:rng="http://relaxng.org/ns/structure/1.0"
                version="1.0"
                extension-element-prefixes="rng a">

  <xsl:param name="title">Declarative Settings Language Documentation</xsl:param>
  <xsl:param name="default.documentation.string" select="'No documentation available.'"/>

  <xsl:output indent="yes"/>
  
  <xsl:template match="/">
    <html>
      <head>
        <title><xsl:value-of select="$title"/></title>
        <link href="dui.css" rel="stylesheet" type="text/css"/>
      </head>
      <body>
        <table>
          <tr>
            <td>
              <img border="0" align="left" src="dui-logo.png"/>
            </td>
            <td>
              <a href="index.html">Home</a>
              ·
              <a href="namespaces.html">All Namespaces</a>
              ·
              <a href="classes.html">All Classes</a>
              ·
              <a href="mainclasses.html">Main Classes</a>
            </td>
          </tr>
        </table>
        
        <div class="contents">
          <h1><xsl:value-of select="$title"/></h1>
          <xsl:apply-templates/>
        </div>

        <hr/>
        <table width="100%" cellspacing="0" border="0">
          <tr class="address">
            <td width="30%" align="left">Copyright <xsl:text disable-output-escaping="yes">&amp;copy;</xsl:text> 2009 Nokia Corporation</td>
            <td width="40%" align="center"><!-- TODO: this was: "Generated on Thu Jun 18 11:09:38 2009". Could the date/time be generated with XSL? --></td>
            <td width="30%" align="right"><div align="right">DirectUI</div></td>
          </tr>
        </table>
      </body>
    </html>
  </xsl:template>

  <xsl:template match="rng:element">
    <xsl:variable name="name" select="@name|rng:name"/>
    <xsl:variable name="nsuri">
      <xsl:choose>
        <xsl:when test="ancestor::rng:div[@ns]">
          <xsl:value-of select="ancestor::rng:div[@ns][1]/@ns"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="ancestor::rng:grammar[@ns][1]/@ns"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="nsprefix">
      <xsl:if test="$nsuri">
        <xsl:value-of select="name(namespace::*[.=$nsuri])"/>
      </xsl:if>
    </xsl:variable>
    <xsl:variable name="qname">
      <xsl:choose>
        <xsl:when test="not($nsprefix='')">
          <xsl:value-of select="$nsprefix"/>:<xsl:value-of select="$name"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$name"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <div class="declElement">
      <xsl:attribute name="id">
        <xsl:call-template name="makeid">
          <xsl:with-param name="node" select="."/>
        </xsl:call-template>
      </xsl:attribute>
      <xsl:choose>
        <!-- This tries to test if the current node is the root node.
             Not sure if this is a good way to do it but seemed to work somehow. -->
        <xsl:when test="count(ancestor::node()) = 1">
          <h2>Root element: <xsl:value-of select="$qname"/></h2>
        </xsl:when>
        <xsl:otherwise>
          <h2>Element: <xsl:value-of select="$qname"/></h2>
        </xsl:otherwise>
      </xsl:choose>
      

      <xsl:if test="not($nsuri='')">
        <h3>Namespace</h3>
        <p><xsl:value-of select="$nsuri"/></p>
      </xsl:if>
      
      <xsl:if test="a:documentation or $default.documentation.string">
        <h3>Documentation</h3>
        <xsl:choose>
          <xsl:when test="a:documentation">
            <p><xsl:value-of select="a:documentation"/></p>
          </xsl:when>
          <xsl:otherwise>
            <p><xsl:value-of select="$default.documentation.string"/></p>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:if>

      <h3>Content Model</h3>
      <p><xsl:apply-templates mode="content-model"/></p>

      <xsl:variable name="hasatts">
        <xsl:apply-templates select="." mode="has-attributes"/>
      </xsl:variable>
      
      <xsl:if test="starts-with($hasatts, 'true')">
        <h3>Attributes</h3>
        <table class="declAttributes">
          <tr>
            <th>Attribute</th>
            <th>Type</th>
            <th>Use</th>
            <th>Documentation</th>
          </tr>
          <xsl:variable name="nesting" select="count(ancestor::rng:element)"/>
          <xsl:apply-templates select=".//rng:attribute[count(ancestor::rng:element)=$nesting+1] | .//rng:ref[count(ancestor::rng:element)=$nesting+1]" mode="attributes">
            <xsl:with-param name="matched" select="."/>
            <xsl:with-param name="optional" select="false()"/>
          </xsl:apply-templates>
        </table>
      </xsl:if>
    </div>

    <xsl:apply-templates select="rng:*"/>
  </xsl:template>
  
  <xsl:template match="rng:attribute">
  </xsl:template>

  <xsl:template match="a:documentation">
    <!--<xsl:value-of select="."/>-->
  </xsl:template>

  <xsl:template match="rng:interleave">
    <xsl:apply-templates/>
  </xsl:template>
  
  <!-- ================================================= -->
  <!-- ATTRIBUTES TEMPLATES                              -->
  <!-- ================================================= -->
  <xsl:template match="rng:attribute" mode="attributes">
    <xsl:param name="matched"/>
    <xsl:param name="optional"/>
    <tr>
      <td><xsl:value-of select="@name"/></td>
      <td>
        <xsl:choose>
          <xsl:when test="rng:data">
            <xsl:choose>
              <xsl:when test="rng:data/a:documentation">
                <xsl:value-of select="rng:data/a:documentation"/>
              </xsl:when>
              <xsl:otherwise>
                xsd:<xsl:value-of select="rng:data/@type"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:when>
          <xsl:when test="rng:text">
            TEXT
          </xsl:when>
          <xsl:when test="rng:choice">
            One of:<xsl:text> </xsl:text>
            <xsl:for-each select="rng:choice/rng:value">
              "<xsl:value-of select="."/>"<xsl:if test="following-sibling::*">, </xsl:if>
            </xsl:for-each>
          </xsl:when>
          <xsl:otherwise>
            TEXT
          </xsl:otherwise>
        </xsl:choose>
      </td>
      <td>
        <xsl:choose>
          <xsl:when test="ancestor::rng:optional">Optional</xsl:when>
          <xsl:when test="boolean($optional)">Optional</xsl:when>
          <xsl:otherwise>Required</xsl:otherwise>
        </xsl:choose>
      </td>
      <td>
        <xsl:value-of select="a:documentation[1]"/>
      </td>
    </tr>
  </xsl:template>
  
  <xsl:template match="rng:ref" mode="attributes">
    <xsl:param name="matched"/>
    <xsl:param name="optional"/>
    <xsl:variable name="name" select="@name"/>
    <xsl:variable name="opt" select="count(ancestor::rng:optional) > 0"/>
    <xsl:apply-templates select="//rng:define[@name=$name]" mode="attributes">
      <xsl:with-param name="matched" select="$matched"/>
      <xsl:with-param name="optional" select="boolean($optional) or boolean($opt)"/>
    </xsl:apply-templates>
  </xsl:template>
  
  <xsl:template match="rng:define" mode="attributes">
    <xsl:param name="matched"/>
    <xsl:param name="optional"/>
    <xsl:if test="not(count(matched)=count(matched|.))">
      <xsl:apply-templates select=".//rng:attribute[not(ancestor::rng:element)] | .//rng:ref[not(ancestor::rng:element)]" mode="attributes">
        <xsl:with-param name="matched" select="$matched|."/>
        <xsl:with-param name="optional" select="$optional or ancestor::rng:optional"/>
      </xsl:apply-templates>
    </xsl:if>
  </xsl:template>

  
  <!-- ================================================= -->
  <!-- HAS-ATTRIBUTES TEMPLATES                          -->
  <!-- ================================================= -->
  <xsl:template match="rng:element" mode="has-attributes">
    <xsl:choose>
      <xsl:when test=".//rng:attribute[count(ancestor::rng:element)=count(current()/ancestor::rng:element) + 1]">true</xsl:when>
      <xsl:otherwise>
        <xsl:apply-templates select=".//rng:ref[count(ancestor::rng:element)=count(current()/ancestor::rng:element) + 1]" mode="has-attributes"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  
  <xsl:template match="rng:ref" mode="has-attributes">
    <xsl:variable name="name" select="@name"/>
    <xsl:apply-templates select="//rng:define[@name=$name]" mode="has-attributes"/>
  </xsl:template>
  
  <xsl:template match="rng:define" mode="has-attributes">
    <xsl:choose>
      <xsl:when test=".//rng:attribute[not(ancestor::rng:element)]">true</xsl:when>
      <xsl:otherwise>
        <xsl:apply-templates select=".//rng:ref[not(ancestor::rng:element)]" mode="has-attributes"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  
  <xsl:template match="rng:*" mode="has-attributes">
    <xsl:apply-templates mode="has-attributes"/>
  </xsl:template>
  
  <xsl:template match="*|node()" mode="has-attributes">
    <!-- suppress -->
  </xsl:template>


  <!-- ================================================= -->
  <!-- RNG:DEFINE TEMPLATES                              -->
  <!-- ================================================= -->
  <xsl:template match="rng:define">
    <xsl:variable name="name" select="@name"/>
    <xsl:choose>
      <xsl:when test="following::rng:define[@name=$name and not(@combine)]">
        <xsl:apply-templates select="//rng:define[@name=$name and not(@combine)]" mode="define-base"/>
      </xsl:when>
      <xsl:when test="not(preceding::rng:define[@name=$name])">
        <xsl:apply-templates select="." mode="define-base"/>
      </xsl:when>
    </xsl:choose>
  </xsl:template>
  
  <xsl:template match="rng:define" mode="define-base">
    <xsl:variable name="name" select="@name"/>
    <xsl:variable name="haselements">
      <xsl:apply-templates select="." mode="find-element">
        <xsl:with-param name="matched" select=".."/>
      </xsl:apply-templates>
    </xsl:variable>
    <xsl:variable name="combined">
      <xsl:if test="@combine">
        <xsl:value-of select="following::rng:define[@name=$name]"/>
      </xsl:if>
      <xsl:if test="not(@combine)">
        <xsl:value-of select="//rng:define[@name=$name and @combine]"/>
      </xsl:if>
    </xsl:variable>
    <xsl:variable name="nsuri">
      <xsl:choose>
        <xsl:when test="ancestor::rng:div[@ns][1]">
          <xsl:value-of select="ancestor::rng:div[@ns][1]/@ns"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="ancestor::rng:grammar[@ns][1]/@ns"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    
    <div>
      <xsl:attribute name="id">
        <xsl:value-of select="@name"/>
      </xsl:attribute>
      <h2>Pattern: <xsl:value-of select="@name"/></h2>
      <table>
        <tr>
          <th>Namespace</th>
          <td><xsl:value-of select="$nsuri"/></td>
        </tr>
        <xsl:if test="a:documentation or not($default.documentation.string='')">
          <tr>
            <th>Documentation</th>
            <td>
              <xsl:choose>
                <xsl:when test="a:documentation">
                  <xsl:apply-templates select="a:documentation"/>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="$default.documentation.string"/>
                </xsl:otherwise>
              </xsl:choose>
            </td>
          </tr>
        </xsl:if>
        <xsl:if test="starts-with($haselements, 'true')">
          <tr>
            <th>Content Model</th>
            <td>
              <xsl:apply-templates select="*" mode="content-model"/>
              <xsl:if test="@combine">
                <xsl:apply-templates select="following::rng:define[@name=$name]" mode="define-combine"/>
              </xsl:if>
              <xsl:if test="not(@combine)">
                <xsl:apply-templates select="//rng:define[@name=$name and @combine]" mode="define-combine"/>
              </xsl:if>
            </td>
          </tr>
        </xsl:if>
        
        <xsl:variable name="hasatts">
          <xsl:apply-templates select="." mode="has-attributes"/>
        </xsl:variable>
        
        <xsl:if test="starts-with($hasatts, 'true')">
          <tr>
            <th>Attributes</th>
            <td>
              <table>
                <tr>
                  <th>Attribute</th>
                  <th>Type</th>
                  <th>Use</th>
                  <th>Documentation</th>
                </tr>
                <xsl:variable name="nesting" select="count(ancestor::rng:element)"/>
                <xsl:apply-templates select=".//rng:attribute[count(ancestor::rng:element)=$nesting] | .//rng:ref[count(ancestor::rng:element)=$nesting]" mode="attributes">
                  <xsl:with-param name="matched" select="."/>
                </xsl:apply-templates>
              </table>
            </td>
          </tr>
        </xsl:if>
      </table>
    </div>
  </xsl:template>
  
  <xsl:template match="rng:define" mode="define-combine">
    <xsl:choose>
      <xsl:when test="@combine='choice'">
        | (<xsl:apply-templates mode="content-model"/>)
      </xsl:when>
      <xsl:when test="@combine='interleave'">
        &amp; (<xsl:apply-templates mode="content-model"/>)
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <!-- ================================================= -->
  <!-- MAKEID TEMPLATES                                  -->
  <!-- ================================================= -->
  <xsl:template match="rng:element" mode="makeid">
    <xsl:apply-templates select="ancestor::rng:element[1]" mode="makeid"/>.<xsl:value-of select="@name"/>
  </xsl:template>
  
  <xsl:template match="rng:define" mode="makeid">
    <xsl:value-of select="@name"/>
  </xsl:template>

  <xsl:template match="*" mode="makeid">
    <xsl:apply-templates select="ancestor::rng:element[1] | ancestor::rng:define[1]"/>
  </xsl:template>
  
  <xsl:template name="makeid">
    <xsl:param name="node"/>
    <xsl:variable name="id">
      <xsl:apply-templates select="$node" mode="makeid"/>
    </xsl:variable>
    <xsl:choose>
      <xsl:when test="ancestor-or-self::rng:define">
        <xsl:value-of select="ancestor-or-self::rng:define[1]/@name"/>
        <xsl:value-of select="$id"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="substring-after($id, '.')"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- ================================================= -->
  <!-- CONTENT MODEL PATTERNS                            -->
  <!-- The following patterns construct a text           -->
  <!-- description of an element content model           -->
  <!-- ================================================= -->
  <xsl:template match="rng:element" mode="content-model">
    <a>
      <xsl:attribute name="href">
        #<xsl:call-template name="makeid">
          <xsl:with-param name="node" select="."/>
        </xsl:call-template>
      </xsl:attribute>
      <xsl:value-of select="@name"/>
    </a>
    <xsl:if test="not(parent::rng:choice) and (following-sibling::rng:element | following-sibling::rng:optional | following-sibling::rng:oneOrMore | following-sibling::rng:zeroOrMore)">,</xsl:if>
  </xsl:template>

  <xsl:template match="rng:interleave" mode="content-model">
    Any combination of:
    <ul>
      <xsl:apply-templates mode="content-model"/>
    </ul>
  </xsl:template>
  
  <xsl:template match="rng:group" mode="content-model">
    These groupped:
    <ul>
      <xsl:apply-templates mode="content-model"/>
    </ul>
  </xsl:template>

  <xsl:template match="rng:optional" mode="content-model">
    <xsl:if test=".//rng:element | .//rng:ref[not(ancestor::rng:attribute)]">
      <li><xsl:apply-templates mode="content-model"/> (optional)</li>
    </xsl:if>
  </xsl:template>

  <xsl:template match="rng:oneOrMore" mode="content-model">
    <li><xsl:apply-templates mode="content-model"/> (at least one)</li>
  </xsl:template>

  <xsl:template match="rng:zeroOrMore" mode="content-model">
    <li><xsl:apply-templates mode="content-model"/> (any number)</li>
  </xsl:template>

  <xsl:template match="rng:choice" mode="content-model">
    One of these:
    <ul>
      <xsl:for-each select="*">
        <xsl:apply-templates select="." mode="content-model"/>
      </xsl:for-each>
    </ul>
  </xsl:template>

  <xsl:template match="rng:value" mode="content-model">
    "<xsl:value-of select="."/>"
  </xsl:template>

  <xsl:template match="rng:empty" mode="content-model">
    EMPTY
  </xsl:template>

  <xsl:template match="rng:ref" mode="content-model">
    <xsl:variable name="haselement"><xsl:apply-templates select="." mode="find-element"/></xsl:variable>
    <xsl:if test="starts-with($haselement, 'true')">
      <link linkend="{@name}">%<xsl:value-of select="@name"/>;</link>
      <xsl:if test="not(parent::rng:choice) and (following-sibling::rng:element | following-sibling::rng:optional | following-sibling::rng:oneOrMore | following-sibling::rng:zeroOrMore)">, </xsl:if>
    </xsl:if>
  </xsl:template>

  <xsl:template match="rng:text" mode="content-model">
    TEXT
    <xsl:if test="not(parent::rng:choice) and (following-sibling::rng:element | following-sibling::rng:optional | following-sibling::rng:oneOrMore | following-sibling::rng:zeroOrMore)">, </xsl:if>
  </xsl:template>

  <xsl:template match="rng:data" mode="content-model">
    <xsl:choose>
      <xsl:when test="a:documentation">
        <xsl:value-of select="a:documentation"/>
      </xsl:when>
      <xsl:otherwise>
        xsd:<xsl:value-of select="@type"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="*" mode="content-model">
    <!-- suppress -->
  </xsl:template>

  <!-- ================================================= -->
  <!-- FIND-ELEMENT TEMPLATES                            -->
  <!-- ================================================= -->
  <xsl:template match="rng:define" mode="find-element">
    <xsl:param name="matched"/>
    <xsl:if test="not(count($matched | .)=count($matched))">
      <xsl:choose>
        <xsl:when test=".//rng:element|.//rng:text">
          <xsl:value-of select="true()"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:apply-templates select=".//rng:ref[not(ancestor::rng:attribute)]" mode="find-element">
            <xsl:with-param name="matched" select="$matched | ."/>
          </xsl:apply-templates>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:if>
  </xsl:template>
  
  <xsl:template match="rng:ref" mode="find-element">
    <xsl:param name="matched" select="."/>
    <xsl:variable name="ref" select="@name"/>
    <xsl:apply-templates select="//rng:define[@name=$ref]" mode="find-element">
      <xsl:with-param name="matched" select="$matched"/>
    </xsl:apply-templates>
  </xsl:template>

</xsl:stylesheet>
