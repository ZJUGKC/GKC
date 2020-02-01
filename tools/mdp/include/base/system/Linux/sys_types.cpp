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

/*
This file contains global variables for header file.
*/

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

// g_chm_def_body

BEGIN_STATIC_CONST_ARRAY(g_chm_def_body)
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(char_a, "document: {"))
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(char_a, "  lcid: \"$$LCID$$\";"))
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(char_a, "  title: \"$$TOPIC$$\";"))
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(char_a, "  hhc: \"$$PROJECTNAME$$.hhc\";"))
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(char_a, "  hhk: \"$$PROJECTNAME$$.hhk\";"))
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(char_a, "  deftopic: \"$$PROJECTNAME$$-cover.htm\";"))
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(char_a, "}"))
	STATIC_CONST_ARRAY_ENTRY_LAST(IMPLEMENT_CONST_STRING_ENTRY(char_a, ""))
END_STATIC_CONST_ARRAY(g_chm_def_body)

// g_script_body

BEGIN_STATIC_CONST_STRING(g_script_body)
	STATIC_CONST_STRING_ENTRY(
"#!/bin/sh" "\n"
"\n"
"function fRunCmd() {" "\n"
"eval $1" "\n"
"if [ $? -ne 0 ]; then" "\n"
"echo \"Error: Command [$1] failed!\"" "\n"
"exit 1" "\n"
"fi" "\n"
"sleep 0.05" "\n"
"}" "\n"
"strTemp=\"\"" "\n"
"arrMd=($$FILES$$)" "\n"
"arrPrefix=($$CSSPREFIX$$)" "\n"
"echo \"Compile markdown files...\"" "\n"
"for s in ${arrMd[@]}; do" "\n"
"$$COMMANDS$$"
"done" "\n"
"$$ENCODING$$"
"echo \"Process html files...\"" "\n"
"function fHtmlProcessing() {" "\n"
"strTemp=\"txtf -html $$OPTIONS$$ \\\"$1.html\\\" \\\"$1$$HTMLEXT$$\\\"\"" "\n"
"fRunCmd \"$strTemp\"" "\n"
"strTemp=\"rm \\\"$1.html\\\"\"" "\n"
"fRunCmd \"$strTemp\"" "\n"
"strTemp=\"rm -f \\\"$1.md\\\"\"" "\n"
"fRunCmd \"$strTemp\"" "\n"
"}" "\n"
"for (( i=0; i<${#arrMd[@]}; i++ )); do" "\n"
"fHtmlProcessing \"${arrMd[$i]}\" \"${arrPrefix[$i]}\"" "\n"
"done" "\n"
"fHtmlProcessing \"$$DIRPREFIX$$/$$PROJECTNAME$$-cover\" \"\"" "\n"
"echo \"Generate eBook...\"" "\n"
"$$EBOOKCMD$$"
"echo \"Done.\"" "\n"
	)
END_STATIC_CONST_STRING(g_script_body)

// g_script_command_item

BEGIN_STATIC_CONST_STRING(g_script_command_item)
	STATIC_CONST_STRING_ENTRY(
"strTemp=\"$$CMDITEM$$\"" "\n"
"fRunCmd \"$strTemp\"" "\n"
	)
END_STATIC_CONST_STRING(g_script_command_item)

// g_script_chm_encoding

BEGIN_STATIC_CONST_STRING(g_script_chm_encoding)
	STATIC_CONST_STRING_ENTRY(
"echo \"Convert encoding...\"" "\n"
"function fChmEncoding() {" "\n"
"strTemp=\"txtf -encoding UTF-8 $$CHARSET$$ \\\"$1\\\" \\\"$1.tmp\\\"\"" "\n"
"fRunCmd \"$strTemp\"" "\n"
"strTemp=\"rm \\\"$1\\\"\"" "\n"
"fRunCmd \"$strTemp\"" "\n"
"strTemp=\"mv \\\"$1.tmp\\\" \\\"$1\\\"\"" "\n"
"fRunCmd \"$strTemp\"" "\n"
"}" "\n"
"for s in ${arrMd[@]}; do" "\n"
"fChmEncoding \"$s.html\"" "\n"
"done" "\n"
"fChmEncoding \"$$DIRPREFIX$$/$$PROJECTNAME$$-cover.html\"" "\n"
"fChmEncoding \"$$DIRPREFIX$$/$$PROJECTNAME$$$$DEF$$\"" "\n"
"fChmEncoding \"$$DIRPREFIX$$/$$PROJECTNAME$$.hhc\"" "\n"
"fChmEncoding \"$$DIRPREFIX$$/$$PROJECTNAME$$.hhk\"" "\n"
	)
END_STATIC_CONST_STRING(g_script_chm_encoding)

// g_script_chm_cmd

BEGIN_STATIC_CONST_STRING(g_script_chm_cmd)
	STATIC_CONST_STRING_ENTRY(
"cd doc" "\n"
"strTemp=\"chmcpl -c \\\"$$PROJECTNAME$$.conf\\\" -o \\\"$$PROJECTNAME$$.chm\\\" .\"" "\n"
"fRunCmd \"$strTemp\"" "\n"
"cd .." "\n"
"strTemp=\"mv \\\"doc/$$PROJECTNAME$$.chm\\\" .\"" "\n"
"fRunCmd \"$strTemp\"" "\n"
	)
END_STATIC_CONST_STRING(g_script_chm_cmd)

// g_script_epub_cmd

BEGIN_STATIC_CONST_STRING(g_script_epub_cmd)
	STATIC_CONST_STRING_ENTRY(
"cd doc" "\n"
"strTemp=\"zip -0Xq \\\"$$PROJECTNAME$$.epub\\\" mimetype\"" "\n"
"fRunCmd \"$strTemp\"" "\n"
"strTemp=\"zip -Xr9Dq \\\"$$PROJECTNAME$$.epub\\\" META-INF OEBPS\"" "\n"
"fRunCmd \"$strTemp\"" "\n"
"cd .." "\n"
"strTemp=\"mv \\\"doc/$$PROJECTNAME$$.epub\\\" .\"" "\n"
"fRunCmd \"$strTemp\"" "\n"
	)
END_STATIC_CONST_STRING(g_script_epub_cmd)

////////////////////////////////////////////////////////////////////////////////
