/*
** Copyright (c) 2016, Xin YUAN, courses of Zhejiang University
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

// g_charset_cp_map

BEGIN_STATIC_CONST_ARRAY(g_charset_cp_map)
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("US-ASCII")),
		IMPLEMENT_CONST_STRING_ENTRY(CharS, CODEPAGE_1252)
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("Arabic")),
		IMPLEMENT_CONST_STRING_ENTRY(CharS, CODEPAGE_1256)
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("GB2312")),
		IMPLEMENT_CONST_STRING_ENTRY(CharS, CODEPAGE_936)
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("Big5")),
		IMPLEMENT_CONST_STRING_ENTRY(CharS, CODEPAGE_950)
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("ISO-8859-2")),
		IMPLEMENT_CONST_STRING_ENTRY(CharS, CODEPAGE_1250)
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("Greek")),
		IMPLEMENT_CONST_STRING_ENTRY(CharS, CODEPAGE_1253)
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("Hebrew")),
		IMPLEMENT_CONST_STRING_ENTRY(CharS, CODEPAGE_1255)
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("Shift-JIS")),
		IMPLEMENT_CONST_STRING_ENTRY(CharS, CODEPAGE_932)
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("ISO-2022-KR")),
		IMPLEMENT_CONST_STRING_ENTRY(CharS, CODEPAGE_949)
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("ISO-8859-5")),
		IMPLEMENT_CONST_STRING_ENTRY(CharS, CODEPAGE_1251)
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("ISO-8859-9")),
		IMPLEMENT_CONST_STRING_ENTRY(CharS, CODEPAGE_1254)
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("")),
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S(""))
	END_STATIC_CONST_ARRAY_GROUP_LAST()
END_STATIC_CONST_ARRAY(g_charset_cp_map)

// _Find_CodePage_From_Charset

ConstStringS _Find_CodePage_From_Charset(const ConstStringS& strCharset) throw()
{
	ConstArray<_Charset_CP> c_arr(g_charset_cp_map::GetAddress(), g_charset_cp_map::GetCount());
	auto iter(c_arr.GetBegin());
	for( ; iter != c_arr.GetEnd(); iter.MoveNext() ) {
		const _Charset_CP& cc = iter.get_Value();
		if( ConstStringCompareTrait<ConstStringS>::IsEQ(strCharset, ConstStringS(cc.m_charset.m_first, cc.m_charset.m_size)) ) {
			return ConstStringS(cc.m_codepage.m_first, cc.m_codepage.m_size);
		}
	}
	return ConstStringS();
}

// g_lcid_short_string

BEGIN_STATIC_CONST_ARRAY(g_lcid_short_string)
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		1033,
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("en-US")),
		0
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		1025,
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("ar-SA")),
		1
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		2052,
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("zh-CN")),
		2
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		1028,
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("zh-TW")),
		3
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		1029,
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("cs-CZ")),
		4
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		1032,
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("el-GR")),
		5
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		1037,
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("he-IL")),
		6
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		1038,
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("hu-HU")),
		4
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		1041,
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("ja-JP")),
		7
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		1042,
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("ko-KR")),
		8
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		1045,
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("pl-PL")),
		4
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		1049,
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("ru-RU")),
		9
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		1051,
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("sk-SK")),
		4
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		1060,
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("sl-SI")),
		4
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		1055,
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("tr-TR")),
		10
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		1026,
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("bg-BG")),
		9
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		0,
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("")),
		(uint)-1
	END_STATIC_CONST_ARRAY_GROUP_LAST()
END_STATIC_CONST_ARRAY(g_lcid_short_string)

// _Find_LCID_From_ShortString

uint _Find_LCID_From_ShortString(const ConstStringS& strShortString, uint& uIndex) throw()
{
	ConstArray<_LCID_ShortString> c_arr(g_lcid_short_string::GetAddress(), g_lcid_short_string::GetCount());
	auto iter(c_arr.GetBegin());
	for( ; iter != c_arr.GetEnd(); iter.MoveNext() ) {
		const _LCID_ShortString& ls = iter.get_Value();
		if( ConstStringCompareTrait<ConstStringS>::IsEQ(strShortString, ConstStringS(ls.m_short_string.m_first, ls.m_short_string.m_size)) ) {
			uIndex = ls.m_uIndex;
			return ls.m_lcid;
		}
	}
	return 0;
}

// _Find_ShortString_From_LCID

ConstStringS _Find_ShortString_From_LCID(uint uLCID, uint& uIndex) throw()
{
	ConstArray<_LCID_ShortString> c_arr(g_lcid_short_string::GetAddress(), g_lcid_short_string::GetCount());
	auto iter(c_arr.GetBegin());
	for( ; iter != c_arr.GetEnd(); iter.MoveNext() ) {
		const _LCID_ShortString& ls = iter.get_Value();
		if( ls.m_lcid == uLCID ) {
			uIndex = ls.m_uIndex;
			return ConstStringS(ls.m_short_string.m_first, ls.m_short_string.m_size);
		}
	}
	return ConstStringS();
}

// g_html_github_header

//  GitHub-Style
BEGIN_STATIC_CONST_STRING(g_html_github_header)
	STATIC_CONST_STRING_ENTRY(
"<!DOCTYPE html>" "\r\n"
"<html>" "\r\n"
"<head>" "\r\n"
"<meta charset=\'$$CHARSET$$\'>" "\r\n"
"<meta name=\'viewport\' content=\'width=device-width initial-scale=1\'>" "\r\n"
"<title>untitled</title>" "\r\n"
"<link href=\'http://fonts.googleapis.com/css?family=Open+Sans:400italic,700italic,700,400&subset=latin,latin-ext\' rel=\'stylesheet\' type=\'text/css\'>" "\r\n"
"<style type=\'text/css\'>" "\r\n"
"html, body {overflow-x: initial !important;}" "\r\n"
"html { font-size: 14px; color: rgb(51, 51, 51); background-color: rgb(255, 255, 255); }" "\r\n"
"body { margin: 0px; padding: 0px; height: auto; bottom: 0px; top: 0px; left: 0px; right: 0px; font-family: \"Helvetica Neue\", Helvetica, Arial, sans-serif; font-size: 1rem; line-height: 1.42857; overflow-x: hidden; background: inherit; }" "\r\n"
"a:active, a:hover { outline: 0px; }" "\r\n"
".in-text-selection, ::selection { text-shadow: none; background: rgb(181, 214, 252); }" "\r\n"
"#write { margin: 0px auto; height: auto; width: inherit; word-break: normal; word-wrap: break-word; position: relative; padding-bottom: 70px; white-space: pre-wrap; overflow-x: auto; }" "\r\n"
"@media screen and (max-width: 500px) {" "\r\n"
"  .CodeMirror-sizer { margin-left: 0px !important; }" "\r\n"
"  .CodeMirror-gutters { display: none !important; }" "\r\n"
"}" "\r\n"
"#write > p:first-child, #write > ul:first-child, #write > ol:first-child, #write > pre:first-child, #write > blockquote:first-child, #write > div:first-child, #write > table:first-child { margin-top: 30px; }" "\r\n"
"#write li > table:first-child { margin-top: -20px; }" "\r\n"
"img { max-width: 100%; }" "\r\n"
"input, button, select, textarea { color: inherit; font-style: inherit; font-variant: inherit; font-weight: inherit; font-stretch: inherit; font-size: inherit; line-height: inherit; font-family: inherit; }" "\r\n"
"input[type=\"checkbox\"], input[type=\"radio\"] { line-height: normal; padding: 0px; }" "\r\n"
"::before, ::after, * { box-sizing: border-box; }" "\r\n"
"#write p, #write h1, #write h2, #write h3, #write h4, #write h5, #write h6, #write div, #write pre { width: inherit; }" "\r\n"
"#write p, #write h1, #write h2, #write h3, #write h4, #write h5, #write h6 { position: relative; }" "\r\n"
"h1 { font-size: 2rem; }" "\r\n"
"h2 { font-size: 1.8rem; }" "\r\n"
"h3 { font-size: 1.6rem; }" "\r\n"
"h4 { font-size: 1.4rem; }" "\r\n"
"h5 { font-size: 1.2rem; }" "\r\n"
"h6 { font-size: 1rem; }" "\r\n"
"p { -webkit-margin-before: 1rem; -webkit-margin-after: 1rem; -webkit-margin-start: 0px; -webkit-margin-end: 0px; }" "\r\n"
".mathjax-block { margin-top: 0px; margin-bottom: 0px; -webkit-margin-before: 0rem; -webkit-margin-after: 0rem; }" "\r\n"
".hidden { display: none; }" "\r\n"
".md-blockmeta { color: rgb(204, 204, 204); font-weight: bold; font-style: italic; }" "\r\n"
"a { cursor: pointer; }" "\r\n"
"#write input[type=\"checkbox\"] { cursor: pointer; width: inherit; height: inherit; margin: 4px 0px 0px; }" "\r\n"
"tr { break-inside: avoid; break-after: auto; }" "\r\n"
"thead { display: table-header-group; }" "\r\n"
"table { border-collapse: collapse; border-spacing: 0px; width: 100%; overflow: auto; break-inside: auto; text-align: left; }" "\r\n"
"table.md-table td { min-width: 80px; }" "\r\n"
".CodeMirror-gutters { border-right-width: 0px; background-color: inherit; }" "\r\n"
".CodeMirror { text-align: left; }" "\r\n"
".CodeMirror-placeholder { opacity: 0.3; }" "\r\n"
".CodeMirror pre { padding: 0px 4px; }" "\r\n"
".CodeMirror-lines { padding: 0px; }" "\r\n"
"div.hr:focus { cursor: none; }" "\r\n"
"pre { white-space: pre-wrap; }" "\r\n"
".CodeMirror-gutters { margin-right: 4px; }" "\r\n"
".md-fences { font-size: 0.9rem; display: block; break-inside: avoid; text-align: left; overflow: visible; white-space: pre; position: relative !important; background: inherit; }" "\r\n"
".md-diagram-panel { width: 100%; margin-top: 10px; text-align: center; padding-top: 0px; padding-bottom: 8px; overflow-x: auto; }" "\r\n"
".md-fences .CodeMirror.CodeMirror-wrap { top: -1.6em; margin-bottom: -1.6em; }" "\r\n"
".md-fences.mock-cm { white-space: pre-wrap; }" "\r\n"
".show-fences-line-number .md-fences { padding-left: 0px; }" "\r\n"
".show-fences-line-number .md-fences.mock-cm { padding-left: 40px; }" "\r\n"
".footnotes { opacity: 0.8; font-size: 0.9rem; padding-top: 1em; padding-bottom: 1em; }" "\r\n"
".footnotes + .footnotes { margin-top: -1em; }" "\r\n"
".md-reset { margin: 0px; padding: 0px; border: 0px; outline: 0px; vertical-align: top; text-decoration: none; text-shadow: none; float: none; position: static; width: auto; height: auto; white-space: nowrap; cursor: inherit; -webkit-tap-highlight-color: transparent; line-height: normal; font-weight: normal; text-align: left; box-sizing: content-box; direction: ltr; background: transparent; }" "\r\n"
"li div { padding-top: 0px; }" "\r\n"
"blockquote { margin: 1rem 0px; }" "\r\n"
"li p, li .mathjax-block { margin: 0.5rem 0px; }" "\r\n"
"li { margin: 0px; position: relative; }" "\r\n"
"blockquote > :last-child { margin-bottom: 0px; }" "\r\n"
"blockquote > :first-child { margin-top: 0px; }" "\r\n"
".footnotes-area { color: rgb(136, 136, 136); margin-top: 0.714rem; padding-bottom: 0.143rem; }" "\r\n"
"@media print {" "\r\n"
"  html, body { height: 100%; }" "\r\n"
"  h1, h2, h3, h4, h5, h6 { break-after: avoid-page; orphans: 2; }" "\r\n"
"  p { orphans: 4; }" "\r\n"
"  html.blink-to-pdf { font-size: 13px; }" "\r\n"
"  @page { margin: 20mm 0mm; }" "\r\n"
"}" "\r\n"
".footnote-line { margin-top: 0.714em; font-size: 0.7em; }" "\r\n"
"a img, img a { cursor: pointer; }" "\r\n"
"pre.md-meta-block { font-size: 0.8rem; min-height: 2.86rem; white-space: pre-wrap; display: block; background: rgb(204, 204, 204); }" "\r\n"
"p > .md-image:only-child { display: inline-block; width: 100%; text-align: center; }" "\r\n"
"#write .MathJax_Display { margin: 0.8em 0px 0px; }" "\r\n"
".mathjax-block { white-space: pre; overflow: hidden; width: 100%; }" "\r\n"
"p + .mathjax-block { margin-top: -1.143rem; }" "\r\n"
".mathjax-block:not(:empty)::after { display: none; }" "\r\n"
"[contenteditable=\"true\"]:active, [contenteditable=\"true\"]:focus { outline: none; box-shadow: none; }" "\r\n"
".task-list { list-style-type: none; }" "\r\n"
".task-list-item { position: relative; padding-left: 1em; }" "\r\n"
".task-list-item input { position: absolute; top: 0px; left: 0px; }" "\r\n"
".math { font-size: 1rem; }" "\r\n"
".md-toc { min-height: 3.58rem; position: relative; font-size: 0.9rem; border-radius: 10px; }" "\r\n"
".md-toc-content { position: relative; margin-left: 0px; }" "\r\n"
".md-toc::after, .md-toc-content::after { display: none; }" "\r\n"
".md-toc-item { display: block; color: rgb(65, 131, 196); text-decoration: none; }" "\r\n"
".md-toc-inner:hover { }" "\r\n"
".md-toc-inner { display: inline-block; cursor: pointer; }" "\r\n"
".md-toc-h1 .md-toc-inner { margin-left: 0px; font-weight: bold; }" "\r\n"
".md-toc-h2 .md-toc-inner { margin-left: 2em; }" "\r\n"
".md-toc-h3 .md-toc-inner { margin-left: 4em; }" "\r\n"
".md-toc-h4 .md-toc-inner { margin-left: 6em; }" "\r\n"
".md-toc-h5 .md-toc-inner { margin-left: 8em; }" "\r\n"
".md-toc-h6 .md-toc-inner { margin-left: 10em; }" "\r\n"
"@media screen and (max-width: 48em) {" "\r\n"
"  .md-toc-h3 .md-toc-inner { margin-left: 3.5em; }" "\r\n"
"  .md-toc-h4 .md-toc-inner { margin-left: 5em; }" "\r\n"
"  .md-toc-h5 .md-toc-inner { margin-left: 6.5em; }" "\r\n"
"  .md-toc-h6 .md-toc-inner { margin-left: 8em; }" "\r\n"
"}" "\r\n"
"a.md-toc-inner { font-size: inherit; font-style: inherit; font-weight: inherit; line-height: inherit; }" "\r\n"
".footnote-line a:not(.reversefootnote) { color: inherit; }" "\r\n"
".md-attr { display: none; }" "\r\n"
".md-fn-count::after { content: \".\"; }" "\r\n"
".md-tag { opacity: 0.5; }" "\r\n"
".md-comment { color: rgb(162, 127, 3); opacity: 0.8; font-family: monospace; }" "\r\n"
"code { text-align: left; }" "\r\n"
"h1 .md-tag, h2 .md-tag, h3 .md-tag, h4 .md-tag, h5 .md-tag, h6 .md-tag { font-weight: initial; opacity: 0.35; }" "\r\n"
"a.md-header-anchor.md-print-anchor { border: none !important; display: inline-block !important; position: absolute !important; width: 1px !important; right: 0px !important; outline: none !important; text-decoration: initial !important; text-shadow: initial !important; background: transparent !important; }" "\r\n"
".md-inline-math .MathJax_SVG .noError { display: none !important; }" "\r\n"
".mathjax-block .MathJax_SVG_Display { text-align: center; margin: 1em 0em; position: relative; text-indent: 0px; max-width: none; max-height: none; min-width: 0px; min-height: 0px; width: 100%; display: block !important; }" "\r\n"
".MathJax_SVG_Display, .md-inline-math .MathJax_SVG_Display { width: auto; margin: inherit; display: inline-block !important; }" "\r\n"
".MathJax_SVG .MJX-monospace { font-family: monospace; }" "\r\n"
".MathJax_SVG .MJX-sans-serif { font-family: sans-serif; }" "\r\n"
".MathJax_SVG { display: inline; font-style: normal; font-weight: normal; line-height: normal; zoom: 90%; text-indent: 0px; text-align: left; text-transform: none; letter-spacing: normal; word-spacing: normal; word-wrap: normal; white-space: nowrap; float: none; direction: ltr; max-width: none; max-height: none; min-width: 0px; min-height: 0px; border: 0px; padding: 0px; margin: 0px; }" "\r\n"
".MathJax_SVG * { transition: none; }" "\r\n"
"\r\n"
"@font-face { font-family: \"Open Sans\"; font-style: normal; font-weight: normal; src: local(\"Open Sans Regular\"), url(\"./github/400.woff\") format(\"woff\"); }" "\r\n"
"@font-face { font-family: \"Open Sans\"; font-style: italic; font-weight: normal; src: local(\"Open Sans Italic\"), url(\"./github/400i.woff\") format(\"woff\"); }" "\r\n"
"@font-face { font-family: \"Open Sans\"; font-style: normal; font-weight: bold; src: local(\"Open Sans Bold\"), url(\"./github/700.woff\") format(\"woff\"); }" "\r\n"
"@font-face { font-family: \"Open Sans\"; font-style: italic; font-weight: bold; src: local(\"Open Sans Bold Italic\"), url(\"./github/700i.woff\") format(\"woff\"); }" "\r\n"
"html { font-size: 16px; }" "\r\n"
"body { font-family: \"Open Sans\", \"Clear Sans\", \"Helvetica Neue\", Helvetica, Arial, sans-serif; color: rgb(51, 51, 51); line-height: 1.6; }" "\r\n"
"#write { max-width: 860px; margin: 0px auto; padding: 20px 30px 100px; }" "\r\n"
"#write > ul:first-child, #write > ol:first-child { margin-top: 30px; }" "\r\n"
"body > :first-child { margin-top: 0px !important; }" "\r\n"
"body > :last-child { margin-bottom: 0px !important; }" "\r\n"
"a { color: rgb(65, 131, 196); }" "\r\n"
"h1, h2, h3, h4, h5, h6 { position: relative; margin-top: 1rem; margin-bottom: 1rem; font-weight: bold; line-height: 1.4; cursor: text; }" "\r\n"
"h1:hover a.anchor, h2:hover a.anchor, h3:hover a.anchor, h4:hover a.anchor, h5:hover a.anchor, h6:hover a.anchor { text-decoration: none; }" "\r\n"
"h1 tt, h1 code { font-size: inherit; }" "\r\n"
"h2 tt, h2 code { font-size: inherit; }" "\r\n"
"h3 tt, h3 code { font-size: inherit; }" "\r\n"
"h4 tt, h4 code { font-size: inherit; }" "\r\n"
"h5 tt, h5 code { font-size: inherit; }" "\r\n"
"h6 tt, h6 code { font-size: inherit; }" "\r\n"
"h1 { padding-bottom: 0.3em; font-size: 2.25em; line-height: 1.2; border-bottom-width: 1px; border-bottom-style: solid; border-bottom-color: rgb(238, 238, 238); }" "\r\n"
"h2 { padding-bottom: 0.3em; font-size: 1.75em; line-height: 1.225; border-bottom-width: 1px; border-bottom-style: solid; border-bottom-color: rgb(238, 238, 238); }" "\r\n"
"h3 { font-size: 1.5em; line-height: 1.43; }" "\r\n"
"h4 { font-size: 1.25em; }" "\r\n"
"h5 { font-size: 1em; }" "\r\n"
"h6 { font-size: 1em; color: rgb(119, 119, 119); }" "\r\n"
"p, blockquote, ul, ol, dl, table { margin: 0.8em 0px; }" "\r\n"
"li > ol, li > ul { margin: 0px; }" "\r\n"
"hr { height: 4px; padding: 0px; margin: 16px 0px; border-width: 0px 0px 1px; border-style: none none solid; overflow: hidden; box-sizing: content-box; border-bottom-color: rgb(221, 221, 221); background-color: rgb(231, 231, 231); }" "\r\n"
"body > h2:first-child { margin-top: 0px; padding-top: 0px; }" "\r\n"
"body > h1:first-child { margin-top: 0px; padding-top: 0px; }" "\r\n"
"body > h1:first-child + h2 { margin-top: 0px; padding-top: 0px; }" "\r\n"
"body > h3:first-child, body > h4:first-child, body > h5:first-child, body > h6:first-child { margin-top: 0px; padding-top: 0px; }" "\r\n"
"a:first-child h1, a:first-child h2, a:first-child h3, a:first-child h4, a:first-child h5, a:first-child h6 { margin-top: 0px; padding-top: 0px; }" "\r\n"
"h1 p, h2 p, h3 p, h4 p, h5 p, h6 p { margin-top: 0px; }" "\r\n"
"li p.first { display: inline-block; }" "\r\n"
"ul, ol { padding-left: 30px; }" "\r\n"
"ul:first-child, ol:first-child { margin-top: 0px; }" "\r\n"
"ul:last-child, ol:last-child { margin-bottom: 0px; }" "\r\n"
"blockquote { border-left-width: 4px; border-left-style: solid; border-left-color: rgb(221, 221, 221); padding: 0px 15px; color: rgb(119, 119, 119); }" "\r\n"
"blockquote blockquote { padding-right: 0px; }" "\r\n"
"table { padding: 0px; word-break: initial; }" "\r\n"
"#write { overflow-x: auto; }" "\r\n"
"table tr { border-top-width: 1px; border-top-style: solid; border-top-color: rgb(204, 204, 204); margin: 0px; padding: 0px; background-color: white; }" "\r\n"
"table tr:nth-child(2n) { background-color: rgb(248, 248, 248); }" "\r\n"
"table tr th { font-weight: bold; border: 1px solid rgb(204, 204, 204); text-align: left; margin: 0px; padding: 6px 13px; }" "\r\n"
"table tr td { border: 1px solid rgb(204, 204, 204); text-align: left; margin: 0px; padding: 6px 13px; }" "\r\n"
"table tr th:first-child, table tr td:first-child { margin-top: 0px; }" "\r\n"
"table tr th:last-child, table tr td:last-child { margin-bottom: 0px; }" "\r\n"
".CodeMirror-gutters { border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); }" "\r\n"
".md-fences, code, tt { border: 1px solid rgb(221, 221, 221); border-radius: 3px; font-family: Consolas, \"Liberation Mono\", Courier, monospace; padding: 2px 4px 0px; font-size: 0.9em; background-color: rgb(248, 248, 248); }" "\r\n"
".md-fences { margin-bottom: 15px; margin-top: 15px; padding: 8px 1em 6px; }" "\r\n"
".task-list { padding-left: 0px; }" "\r\n"
".task-list-item { padding-left: 32px; }" "\r\n"
".task-list-item input { top: 3px; left: 8px; }" "\r\n"
"@media screen and (min-width: 914px) {" "\r\n"
"}" "\r\n"
"@media print {" "\r\n"
"  html { font-size: 13px; }" "\r\n"
"  table, pre { break-inside: avoid; }" "\r\n"
"  pre { word-wrap: break-word; }" "\r\n"
"}" "\r\n"
".md-fences { background-color: rgb(248, 248, 248); }" "\r\n"
"#write pre.md-meta-block { padding: 1rem; font-size: 85%; line-height: 1.45; border: 0px; border-radius: 3px; color: rgb(119, 119, 119); margin-top: 0px !important; background-color: rgb(247, 247, 247); }" "\r\n"
".mathjax-block > .code-tooltip { bottom: 0.375rem; }" "\r\n"
"#write > h3.md-focus::before { left: -1.5625rem; top: 0.375rem; }" "\r\n"
"#write > h4.md-focus::before { left: -1.5625rem; top: 0.285714rem; }" "\r\n"
"#write > h5.md-focus::before { left: -1.5625rem; top: 0.285714rem; }" "\r\n"
"#write > h6.md-focus::before { left: -1.5625rem; top: 0.285714rem; }" "\r\n"
".md-image > .md-meta { border: 1px solid rgb(221, 221, 221); border-radius: 3px; font-family: Consolas, \"Liberation Mono\", Courier, monospace; padding: 2px 4px 0px; font-size: 0.9em; color: inherit; background-color: rgb(248, 248, 248); }" "\r\n"
".md-tag { color: inherit; }" "\r\n"
".md-toc { margin-top: 20px; padding-bottom: 20px; }" "\r\n"
".on-focus-mode blockquote { border-left-color: rgba(85, 85, 85, 0.117647); }" "\r\n"
"header, .context-menu, .megamenu-content, footer { font-family: \"Segoe UI\", Arial, sans-serif; }" "\r\n"
"</style>" "\r\n"
"</head>" "\r\n"
"\r\n"
"<body>" "\r\n"
	)
END_STATIC_CONST_STRING(g_html_github_header)

// g_html_tail

BEGIN_STATIC_CONST_STRING(g_html_tail)
	STATIC_CONST_STRING_ENTRY(
		"</body>" "\r\n"
		"</html>" "\r\n"
	)
END_STATIC_CONST_STRING(g_html_tail)

// g_html_xhtml_header

BEGIN_STATIC_CONST_STRING(g_html_xhtml_header)
	STATIC_CONST_STRING_ENTRY(
"<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"no\"?>" "\r\n"
"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\" \"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">" "\r\n"
"<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"$$LANG$$\">" "\r\n"
"<head>" "\r\n"
"<title>untitled</title>" "\r\n"
"<link href=\"$$CSSFILE$$\" type=\"text/css\" rel=\"stylesheet\"/>" "\r\n"
"<style type=\"text/css\">" "\r\n"
"@page { margin-bottom: 5.000000pt; margin-top: 5.000000pt; }" "\r\n"
"</style>" "\r\n"
"</head>" "\r\n"
"\r\n"
"<body>" "\r\n"
	)
END_STATIC_CONST_STRING(g_html_xhtml_header)

// g_html_xhtml_tail

BEGIN_STATIC_CONST_STRING(g_html_xhtml_tail)
	STATIC_CONST_STRING_ENTRY(
"<div class=\"mbppagebreak\"></div>" "\r\n"
"</body>" "\r\n"
"\r\n"
"</html>" "\r\n"
	)
END_STATIC_CONST_STRING(g_html_xhtml_tail)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
