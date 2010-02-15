<?xml version="1.0" encoding="UTF-8"?>
<!--
 - Copyright: (C) 2009 - Nokia Corporation
 - Date:      2009/07/16
 - Author:    "Tobias Koch" <ext-tobias.koch@nokia.com>
 - Desc:      Generates a nice HTML report from a test results XML file
 -            produced by the Maemo Test Automation System.
-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

	<!-- path from which static data will be served -->
	<xsl:param name="static_data_dir" select="'/data'"/>

	<!-- output XHTML 1.0 transitional -->
	<xsl:output method="xml" indent="no" encoding="UTF-8"
	doctype-system="http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"
	doctype-public="-//W3C//DTD XHTML 1.0 Transitional//EN"
	omit-xml-declaration="yes"/>

	<xsl:template match="/testresults">
		<!-- page start -->
		<html>
			<head>
				<meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
				<title>Test Results</title>
				<style type="text/css">
body {
	font-family: sans-serif;
	color: black;
}

div.testset-pass {
	margin-top: 1ex;
	padding: 0.5ex 2px 0.5ex 2px;
	background-color: #009900;
	color: white;
	font-size: 120%;
}

div.testset-fail {
	margin-top: 1ex;
	padding: 0.5ex 2px 0.5ex 2px;
	background-color: #990000;
	color: white;
	font-size: 120%;
}

div.testcase-pass {
	margin-top: 1px;
	padding: 0.5ex 0.5em 0.5ex 0.5em;
	background-color: #00aa00;
	color: white;
}

div.testcase-fail {
	margin-top: 1px;
	padding: 0.5ex 0.5em 0.5ex 0.5em;
	background-color: #aa0000;
	color: white;
}

div.teststep-pass {
	margin-top: 1px;
	padding: 0.25ex 0.5em 0.25ex 0.5em;
	background-color: #33ee00;
}

div.teststep-fail {
	margin-top: 1px;
	padding: 0.25ex 0.5em 0.25ex 0.5em;
	background-color: #ff6c6c;
}

pre.stdio {
	margin: 4px 0em 4px 0em;
	padding: 0.25ex 0.75em 0.25ex 0.75em;
	border: 1px dashed black;
}

a.toggle_stdio {
	text-decoration: underline;
	color: black;
	cursor: pointer;
}

a.toggle_details {
	text-decoration: none;
	color: white;
	cursor: pointer;
}

div.spacer {
	height: 0.5em;
}

table.dataview {
	border-top: 1px solid black;
	border-bottom: 1px solid black;
	border-collapse: collapse;
}

td.heading {
	font-weight: bold;
	border-bottom: 1px solid black;
}

td.number {
	text-align: right;
	padding: 2px 2px 2px 2px;
}

h1, h2, h3 {
	margin-top: 2ex;
	margin-bottom: 0.5ex;
}

h1 {
	margin-top: 0ex;
}

p {
	margin-top: 1ex;
	margin-bottom: 1ex;
}
				</style>
				<script type="text/javascript">
function toggleVisibility(elementID) {
	element = document.getElementById(elementID);
	if(element.style.display == 'none') {
		element.style.display = 'block';
	}
	else {
		element.style.display = 'none';
	}
}

function toggleBullet(spanID) {
	span = document.getElementById(spanID);
	text = span.innerHTML;

	if(text == "(+)") {
		span.innerHTML = "(-)";
	}
	else {
		span.innerHTML = "(+)";
	}
}
				</script>
			</head>
			<body>

				<!-- print document content -->
				<xsl:call-template name="testresults" select="."/>

			</body>
		</html>
	</xsl:template>

	<xsl:template name="testresults">
		<h1>Automated Testing Results</h1>
		<xsl:choose>
			<xsl:when test="count(suite/descendant::case) != 0">
				<table border="0" cellspacing="0" cellpadding="0" class="dataview" style="width: 600px">
					<tr class="heading">
						<td class="heading">Test suites </td>
						<td class="heading">Passing     </td>
						<td class="heading">No. tests</td>
						<td class="heading">Passing     </td>
					</tr>
					<tr>
						<td width="25%"><xsl:value-of select="count(suite)"/></td>
						<td width="25%"><xsl:value-of select="count(suite[count(descendant::case[@result != 'PASS']) = 0])"/></td>
						<td width="25%"><xsl:value-of select="count(suite//case)"/></td>
						<td width="25%">
							<xsl:value-of select="count(suite//case[@result = 'PASS'])"/>
							<xsl:text> (</xsl:text>
							<xsl:value-of select="round(count(suite//case[@result = 'PASS']) * 100 div count(suite//case) * 100) div 100"/>
							<xsl:text>%)</xsl:text>
						</td>
					</tr>
				</table>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>No test results found for this request.</xsl:text>
			</xsl:otherwise>
		</xsl:choose>
		<xsl:apply-templates/>
	</xsl:template>

	<xsl:template match="suite">
		<h2>Test Suite <xsl:value-of select="@name"/></h2>
		<table border="0" cellspacing="0" cellpadding="0" class="dataview" style="width: 600px">
			<tr class="heading">
				<td class="heading">Test sets </td>
				<td class="heading">Passing     </td>
				<td class="heading">No. tests</td>
				<td class="heading">Passing     </td>
			</tr>
			<tr>
				<td width="25%"><xsl:value-of select="count(set)"/></td>
				<td width="25%"><xsl:value-of select="count(set[count(descendant::case[@result != 'PASS']) = 0])"/></td>
				<td width="25%"><xsl:value-of select="count(set//case)"/></td>
				<td width="25%">
					<xsl:value-of select="count(set//case[@result = 'PASS'])"/>
					<xsl:text> (</xsl:text>
					<xsl:value-of select="round(count(set//case[@result = 'PASS']) * 100 div count(set//case) * 100) div 100"/>
					<xsl:text>%)</xsl:text>
				</td>
			</tr>
		</table>
		
		<h3>Test Sets</h3>
		<xsl:apply-templates/>
	</xsl:template>

	<xsl:template match="set">
		<xsl:variable name="cssclass">
			<xsl:choose>
				<xsl:when test="case[@result = 'FAIL']">
					<xsl:text>testset-fail</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>testset-pass</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<div class="{$cssclass}">
			<b><a class="toggle_details" onclick="toggleVisibility('details:{generate-id(.)}'); toggleBullet('bullet:{generate-id(.)}')">
				<table border="0" cellspacing="0" cellpadding="0" style="width: 100%">
					<tr>
						<td style="padding-left: 2px; padding-right: 4px;">
							<span id="bullet:{generate-id(.)}"><xsl:text>(+)</xsl:text></span>
						</td>
						<td style="width: 100%">
							<xsl:value-of select="@name"/>
						</td>
					</tr>
				</table>
			</a></b>
		</div>
		<div id="details:{generate-id(.)}" style="display: none">
			<xsl:apply-templates/>
		</div>
	</xsl:template>

	<xsl:template match="case">
		<xsl:variable name="cssclass">
			<xsl:choose>
				<xsl:when test="@result = 'FAIL'">
					<xsl:text>testcase-fail</xsl:text>
				</xsl:when>
				<xsl:when test="@result = 'PASS'">
					<xsl:text>testcase-pass</xsl:text>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>
		<div class="{$cssclass}">
			<a class="toggle_details" onclick="toggleVisibility('details:{generate-id(.)}')">
				<table border="0" cellspacing="0" cellpadding="0" style="width: 100%">
					<tr>
						<td>
							<b><xsl:text></xsl:text><xsl:value-of select="@name"/></b>
							<xsl:if test="@name != @description">
								<xsl:text> (</xsl:text><xsl:value-of select="@description"/><xsl:text>)</xsl:text>
							</xsl:if>
						</td>
					</tr>
				</table>
			</a>
		</div>
		<div id="details:{generate-id(.)}" style="display: none">
			<xsl:apply-templates/>
		</div>
	</xsl:template>

	<xsl:template match="step">
		<xsl:variable name="cssclass">
			<xsl:choose>
				<xsl:when test="@result = 'FAIL'">
					<xsl:text>teststep-fail</xsl:text>
				</xsl:when>
				<xsl:when test="@result = 'PASS'">
					<xsl:text>teststep-pass</xsl:text>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>
		<div class="{$cssclass}">
			<div>
				<xsl:text>[Step </xsl:text><xsl:value-of select="count(preceding-sibling::step) + 1"/><xsl:text>]</xsl:text>
					<xsl:text disable-output-escaping="yes">&amp;nbsp;&amp;nbsp;&amp;nbsp;</xsl:text>
				<xsl:value-of select="@command"/>
			</div>
			<div>
				<xsl:text>Start time: </xsl:text><xsl:value-of select="start"/>
					<xsl:text disable-output-escaping="yes">&amp;nbsp;&amp;nbsp;&amp;nbsp;</xsl:text>
				<xsl:text>End time:   </xsl:text><xsl:value-of select="end"/>
					<xsl:text disable-output-escaping="yes">&amp;nbsp;&amp;nbsp;&amp;nbsp;</xsl:text>
				<xsl:text>Exit code:  </xsl:text><xsl:value-of select="return_code"/>
					<xsl:text disable-output-escaping="yes">&amp;nbsp;&amp;nbsp;&amp;nbsp;</xsl:text>

				<xsl:if test="stdout != '' or stderr != ''">
					<xsl:text>Show: </xsl:text>
				</xsl:if>
				<!-- if there was output on stdout, enable toggle button -->
				<xsl:if test="stdout != ''">
					<a class="toggle_stdio" onclick="toggleVisibility('{generate-id(stdout)}');"><xsl:text>stdout</xsl:text></a>
				</xsl:if>

				<!-- if there was output on stderr, enable toggle button -->
				<xsl:if test="stderr != ''">
					<xsl:if test="stdout != ''">
						<xsl:text>, </xsl:text>
					</xsl:if>
					<a class="toggle_stdio" onclick="toggleVisibility('{generate-id(stderr)}');"><xsl:text>stderr</xsl:text></a>
				</xsl:if>
			</div>
		</div>

		<!-- show stdout -->
		<xsl:if test="stdout != ''">
			<div id="{generate-id(stdout)}" style="display: none">
				<pre class="stdio">
					<xsl:value-of select="stdout"/>
				</pre>
			</div>
		</xsl:if>

		<!-- show stderr -->
		<xsl:if test="stderr != ''">
			<div id="{generate-id(stderr)}" style="display: none">
				<pre class="stdio">
					<xsl:value-of select="stderr"/>
				</pre>
			</div>
		</xsl:if>

		<!-- vertical space -->
		<div class="spacer"></div>
	</xsl:template>

	<!-- filter out all unmatched nodes -->
	<xsl:template match="comment()|*"/>

</xsl:stylesheet>

