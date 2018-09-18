/*
** Copyright (c) 2018, Xin YUAN, courses of Zhejiang University
** All rights reserved.
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the 2-Clause BSD License.
**
** Author contact information:
**   yxxinyuan@zju.edu.cn
**
*/

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// g_cover_htm_body

BEGIN_STATIC_CONST_STRING(g_cover_htm_body)
	STATIC_CONST_STRING_ENTRY(
"<p style=\"text-align:center\">" "\r\n"
"<img src=\"$$COVER$$\" alt=\"cover\" />" "\r\n"
"</p>" "\r\n"
	)
END_STATIC_CONST_STRING(g_cover_htm_body)

// g_chm_content_body

BEGIN_STATIC_CONST_STRING(g_chm_content_body)
	STATIC_CONST_STRING_ENTRY(
"<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML//EN\">" "\r\n"
"<HTML>" "\r\n"
"<HEAD>" "\r\n"
"<meta name=\"GENERATOR\" content=\"$$GENERATOR$$\">" "\r\n"
"<!-- Sitemap 1.0 -->" "\r\n"
"</HEAD><BODY>" "\r\n"
"<OBJECT type=\"text/site properties\">" "\r\n"
" <param name=\"Window Styles\" value=\"0x800025\">" "\r\n"
" <param name=\"ImageType\" value=\"Folder\">" "\r\n"
"</OBJECT>" "\r\n"
"<UL>" "\r\n"
"$$TREE$$"
"</UL>" "\r\n"
"</BODY></HTML>" "\r\n"
	)
END_STATIC_CONST_STRING(g_chm_content_body)

// g_chm_content_folder

BEGIN_STATIC_CONST_STRING(g_chm_content_folder)
	STATIC_CONST_STRING_ENTRY(
"<LI> <OBJECT type=\"text/sitemap\">" "\r\n"
"<param name=\"Name\" value=\"$$NAME$$\">" "\r\n"
"<param name=\"Local\" value=\"$$FILE$$\">" "\r\n"
"<param name=\"ImageNumber\" value=\"1\">" "\r\n"
"</OBJECT>" "\r\n"
	)
END_STATIC_CONST_STRING(g_chm_content_folder)

// g_chm_content_node

BEGIN_STATIC_CONST_STRING(g_chm_content_node)
	STATIC_CONST_STRING_ENTRY(
"<LI> <OBJECT type=\"text/sitemap\">" "\r\n"
"<param name=\"Name\" value=\"$$NAME$$\">" "\r\n"
"<param name=\"Local\" value=\"$$FILE$$\">" "\r\n"
"</OBJECT>" "\r\n"
	)
END_STATIC_CONST_STRING(g_chm_content_node)

// g_chm_index_body

BEGIN_STATIC_CONST_STRING(g_chm_index_body)
	STATIC_CONST_STRING_ENTRY(
"<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML//EN\">" "\r\n"
"<HTML>" "\r\n"
"<HEAD>" "\r\n"
"<meta name=\"GENERATOR\" content=\"$$GENERATOR$$\">" "\r\n"
"<!-- Sitemap 1.0 -->" "\r\n"
"</HEAD><BODY>" "\r\n"
"<UL>" "\r\n"
"$$FILELIST$$"
"</UL>" "\r\n"
"</BODY></HTML>" "\r\n"
	)
END_STATIC_CONST_STRING(g_chm_index_body)

// g_epub_mimetype_body

BEGIN_STATIC_CONST_STRING(g_epub_mimetype_body)
	STATIC_CONST_STRING_ENTRY(
"application/epub+zip"
	)
END_STATIC_CONST_STRING(g_epub_mimetype_body)

// g_epub_container_body

BEGIN_STATIC_CONST_STRING(g_epub_container_body)
	STATIC_CONST_STRING_ENTRY(
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>" "\r\n"
"<container version=\"1.0\" xmlns=\"urn:oasis:names:tc:opendocument:xmlns:container\">" "\r\n"
"\t<rootfiles>" "\r\n"
"\t\t<rootfile full-path=\"OEBPS/content.opf\" media-type=\"application/oebps-package+xml\"/>" "\r\n"
"\t</rootfiles>" "\r\n"
"</container>" "\r\n"
	)
END_STATIC_CONST_STRING(g_epub_container_body)

// g_epub_css_body

BEGIN_STATIC_CONST_STRING(g_epub_css_body)
	STATIC_CONST_STRING_ENTRY(
"body {" "\r\n"
"  margin: 10px;" "\r\n"
"  font-size: 1.0em;" "\r\n"
"  word-wrap: break-word;" "\r\n"
"  text-align: justify;" "\r\n"
"}" "\r\n"
"h1 {" "\r\n"
"  font-size: 1.6em;" "\r\n"
"  font-weight: bold;" "\r\n"
"}" "\r\n"
"h2 {" "\r\n"
"  display: block;" "\r\n"
"  font-size: 1.2em;" "\r\n"
"  font-weight: bold;" "\r\n"
"  margin-bottom: 0.83em;" "\r\n"
"  margin-left: 0;" "\r\n"
"  margin-right: 0;" "\r\n"
"  margin-top: 1em;" "\r\n"
"}" "\r\n"
"h3 {" "\r\n"
"  font-size: large;" "\r\n"
"  font-weight: bold;" "\r\n"
"}" "\r\n"
"h4 {" "\r\n"
"  font-size: medium;" "\r\n"
"  font-weight: bold;" "\r\n"
"}" "\r\n"
"h5 {" "\r\n"
"  font-size: small;" "\r\n"
"  font-weight: bold;" "\r\n"
"}" "\r\n"
"h6 {" "\r\n"
"  font-size: x-small;" "\r\n"
"  font-weight: bold;" "\r\n"
"}" "\r\n"
".mbppagebreak {" "\r\n"
"  display: block;" "\r\n"
"  margin-bottom: 0;" "\r\n"
"  margin-left: 0;" "\r\n"
"  margin-right: 0;" "\r\n"
"  margin-top: 0;" "\r\n"
"}" "\r\n"
"p {" "\r\n"
"  text-indent: 2em;" "\r\n"
"  line-height: 1.5em;" "\r\n"
"  margin-top: 0;" "\r\n"
"  margin-bottom: 1.5em;" "\r\n"
"}" "\r\n"
"a {" "\r\n"
"  color: inherit;" "\r\n"
"  text-decoration: none;" "\r\n"
"  cursor: default;" "\r\n"
"}" "\r\n"
"a[href] {" "\r\n"
"  color: blue;" "\r\n"
"  text-decoration: none;" "\r\n"
"  cursor: pointer;" "\r\n"
"}" "\r\n"
"em {" "\r\n"
"  font-style: italic;" "\r\n"
"}" "\r\n"
"table {" "\r\n"
"  border-collapse: collapse;" "\r\n"
"  border-spacing: 0px;" "\r\n"
"  width: 100%;" "\r\n"
"  overflow: auto;" "\r\n"
"  break-inside: auto;" "\r\n"
"  text-align: left;" "\r\n"
"  padding: 0px;" "\r\n"
"  word-break: initial;" "\r\n"
"}" "\r\n"
"thead {" "\r\n"
"  display: table-header-group;" "\r\n"
"}" "\r\n"
"tr {" "\r\n"
"  break-inside: avoid;" "\r\n"
"  break-after: auto;" "\r\n"
"  border-top-width: 1px;" "\r\n"
"  border-top-style: solid;" "\r\n"
"  border-top-color: rgb(204, 204, 204);" "\r\n"
"  margin: 0px;" "\r\n"
"  padding: 0px;" "\r\n"
"  background-color: white;" "\r\n"
"}" "\r\n"
"table tr:nth-child(2n) {" "\r\n"
"  background-color: rgb(248, 248, 248);" "\r\n"
"}" "\r\n"
"table tr th {" "\r\n"
"  font-weight: bold;" "\r\n"
"  border: 1px solid rgb(204, 204, 204);" "\r\n"
"  text-align: left;" "\r\n"
"  margin: 0px;" "\r\n"
"  padding: 6px 13px;" "\r\n"
"}" "\r\n"
"table tr td {" "\r\n"
"  border: 1px solid rgb(204, 204, 204);" "\r\n"
"  text-align: left;" "\r\n"
"  margin: 0px;" "\r\n"
"  padding: 6px 13px;" "\r\n"
"}" "\r\n"
"table tr th:first-child, table tr td:first-child {" "\r\n"
"  margin-top: 0px;" "\r\n"
"}" "\r\n"
"table tr th:last-child, table tr td:last-child {" "\r\n"
"  margin-bottom: 0px;" "\r\n"
"}" "\r\n"
"ul, ol {" "\r\n"
"  padding-left: 30px;" "\r\n"
"}" "\r\n"
"ul li {" "\r\n"
"  list-style-type: disc;" "\r\n"
"}" "\r\n"
"ol li {" "\r\n"
"  list-style-type: decimal;" "\r\n"
"}" "\r\n"
"li {" "\r\n"
"  border-bottom: 1px solid #D5D5D5;" "\r\n"
"  margin: 0;" "\r\n"
"  padding: 0;" "\r\n"
"  position: relative;" "\r\n"
"}" "\r\n"
"li div {" "\r\n"
"  padding-top: 0px;" "\r\n"
"}" "\r\n"
"ul:first-child, ol:first-child {"  "\r\n"
"  margin-top: 0px;" "\r\n"
"}" "\r\n"
"ul:last-child, ol:last-child {" "\r\n"
"  margin-bottom: 0px;" "\r\n"
"}" "\r\n"
"blockquote {" "\r\n"
"  border-left-width: 4px;" "\r\n"
"  border-left-style: solid;" "\r\n"
"  border-left-color: rgb(221, 221, 221);" "\r\n"
"  padding: 0px 15px;" "\r\n"
"  color: rgb(119, 119, 119);" "\r\n"
"  margin: 1rem 0px;" "\r\n"
"}" "\r\n"
"blockquote blockquote {" "\r\n"
"  padding-right: 0px;" "\r\n"
"}" "\r\n"
"blockquote > :last-child {" "\r\n"
"  margin-bottom: 0px;" "\r\n"
"}" "\r\n"
"blockquote > :first-child {" "\r\n"
"  margin-top: 0px;" "\r\n"
"}" "\r\n"
"code, tt {" "\r\n"
"  border: 1px solid rgb(221, 221, 221);" "\r\n"
"  border-radius: 3px;" "\r\n"
"  font-family: Consolas, \"Liberation Mono\", Courier, monospace;" "\r\n"
"  padding: 2px 4px 0px;" "\r\n"
"  font-size: 0.9em;" "\r\n"
"  background-color: rgb(248, 248, 248);" "\r\n"
"}" "\r\n"
	)
END_STATIC_CONST_STRING(g_epub_css_body)

// g_epub_opf_body

BEGIN_STATIC_CONST_STRING(g_epub_opf_body)
	STATIC_CONST_STRING_ENTRY(
"<?xml version=\'1.0\' encoding=\'utf-8\'?>" "\r\n"
"\r\n"
"<package xmlns=\"http://www.idpf.org/2007/opf\" xmlns:dc=\"http://purl.org/dc/elements/1.1/\" unique-identifier=\"bookid\" version=\"2.0\">" "\r\n"
"\r\n"
"<metadata xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:opf=\"http://www.idpf.org/2007/opf\">" "\r\n"
"<dc:title>$$TOPIC$$</dc:title>" "\r\n"
"<dc:creator>$$AUTHOR$$</dc:creator>" "\r\n"
"<dc:description>$$DESCRIPTION$$</dc:description>" "\r\n"
"<dc:language>$$LCSS$$</dc:language>" "\r\n"
"<dc:date>$$DATE$$</dc:date>" "\r\n"
"<dc:contributor>$$CONTRIBUTOR$$</dc:contributor>" "\r\n"
"<dc:publisher>$$PUBLISHER$$</dc:publisher>" "\r\n"
"<dc:subject>$$SUBJECT$$</dc:subject>" "\r\n"
"<dc:rights>$$RIGHTS$$</dc:rights>" "\r\n"
"<dc:identifier id=\"bookid\">$$IDENTIFIER$$</dc:identifier>" "\r\n"
"<meta name=\"cover\" content=\"$$COVER$$\" />" "\r\n"
"</metadata>" "\r\n"
"\r\n"
"<manifest>" "\r\n"
"<item href=\"toc.ncx\" media-type=\"application/x-dtbncx+xml\" id=\"ncx\"/>" "\r\n"
"<item href=\"stylesheet.css\" id=\"css\" media-type=\"text/css\"/>" "\r\n"
"<item href=\"$$PROJECTNAME$$-cover.xhtml\" id=\"$$PROJECTNAME$$-cover\" media-type=\"application/xhtml+xml\"/>" "\r\n"
"$$ITEMLIST$$"
"</manifest>" "\r\n"
"\r\n"
"<spine toc=\"ncx\">" "\r\n"
"<itemref idref=\"$$PROJECTNAME$$-cover\" linear=\"no\"/>" "\r\n"
"$$ITEMREFLIST$$"
"</spine>" "\r\n"
"\r\n"
"<guide>" "\r\n"
"<reference href=\"$$PROJECTNAME$$-cover.xhtml\" type=\"cover\" title=\"$$COVERNAME$$\"/>" "\r\n"
"$$GUIDELIST$$"
"</guide>" "\r\n"
"\r\n"
"</package>" "\r\n"
	)
END_STATIC_CONST_STRING(g_epub_opf_body)

// g_epub_opf_item

BEGIN_STATIC_CONST_STRING(g_epub_opf_item)
	STATIC_CONST_STRING_ENTRY(
"<item href=\"$$FILE$$\" id=\"$$FILEID$$\" media-type=\"$$TYPE$$\"/>" "\r\n"
	)
END_STATIC_CONST_STRING(g_epub_opf_item)

// g_epub_opf_itemref

BEGIN_STATIC_CONST_STRING(g_epub_opf_itemref)
	STATIC_CONST_STRING_ENTRY(
"<itemref idref=\"$$FILEID$$\"/>" "\r\n"
	)
END_STATIC_CONST_STRING(g_epub_opf_itemref)

// g_epub_opf_guide

BEGIN_STATIC_CONST_STRING(g_epub_opf_guide)
	STATIC_CONST_STRING_ENTRY(
"<reference href=\"$$FILE$$\" type=\"$$TYPE$$\" title=\"$$NAME$$\"/>" "\r\n"
	)
END_STATIC_CONST_STRING(g_epub_opf_guide)

// g_epub_ncx_body

BEGIN_STATIC_CONST_STRING(g_epub_ncx_body)
	STATIC_CONST_STRING_ENTRY(
"<?xml version=\'1.0\' encoding=\'utf-8\'?>" "\r\n"
"<!DOCTYPE ncx PUBLIC \"-//NISO//DTD ncx 2005-1//EN\" " "\r\n"
" \"http://www.daisy.org/z3986/2005/ncx-2005-1.dtd\">" "\r\n"
"\r\n"
"<ncx xmlns=\"http://www.daisy.org/z3986/2005/ncx/\" version=\"2005-1\" xml:lang=\"$$LCSS$$\">" "\r\n"
"\r\n"
"<head>" "\r\n"
"<meta content=\"$$IDENTIFIER$$\" name=\"dtb:uid\"/>" "\r\n"
"<meta content=\"$$DEPTH$$\" name=\"dtb:depth\"/>" "\r\n"
"<meta content=\"mdp\" name=\"dtb:generator\"/>" "\r\n"
"<meta content=\"0\" name=\"dtb:totalPageCount\"/>" "\r\n"
"<meta content=\"0\" name=\"dtb:maxPageNumber\"/>" "\r\n"
"</head>" "\r\n"
"\r\n"
"<docTitle>" "\r\n"
"<text>$$TOPIC$$</text>" "\r\n"
"</docTitle>" "\r\n"
"\r\n"
"<docAuthor>" "\r\n"
"<text>$$AUTHOR$$</text>" "\r\n"
"</docAuthor>" "\r\n"
"\r\n"
"<navMap>" "\r\n"
"<navPoint id=\"$$PROJECTNAME$$-cover\" playOrder=\"1\">" "\r\n"
" <navLabel>" "\r\n"
"  <text>$$COVERNAME$$</text>" "\r\n"
" </navLabel>" "\r\n"
" <content src=\"$$PROJECTNAME$$-cover.xhtml\"/>" "\r\n"
"</navPoint>" "\r\n"
"$$TREE$$"
"</navMap>" "\r\n"
"\r\n"
"</ncx>" "\r\n"
	)
END_STATIC_CONST_STRING(g_epub_ncx_body)

// g_epub_ncx_node

BEGIN_STATIC_CONST_STRING(g_epub_ncx_node)
	STATIC_CONST_STRING_ENTRY(
"<navPoint id=\"$$FILEID$$\" playOrder=\"$$ORDER$$\">" "\r\n"
" <navLabel>" "\r\n"
"  <text>$$NAME$$</text>" "\r\n"
" </navLabel>" "\r\n"
" <content src=\"$$FILE$$\"/>" "\r\n"
	)
END_STATIC_CONST_STRING(g_epub_ncx_node)

// g_epub_ncx_node_tail
BEGIN_STATIC_CONST_STRING(g_epub_ncx_node_tail)
	STATIC_CONST_STRING_ENTRY(
"</navPoint>" "\r\n"
	)
END_STATIC_CONST_STRING(g_epub_ncx_node_tail)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
