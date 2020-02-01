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
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(char_a, "[OPTIONS]"))
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(char_a, "Compatibility=1.1 or later"))
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(char_a, "Compiled file=$$PROJECTNAME$$.chm"))
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(char_a, "Contents file=$$PROJECTNAME$$.hhc"))
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(char_a, "Default topic=$$PROJECTNAME$$-cover.htm"))
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(char_a, "Display compile progress=No"))
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(char_a, "Full-text search=Yes"))
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(char_a, "Index file=$$PROJECTNAME$$.hhk"))
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(char_a, "Language=$$LCID$$"))
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(char_a, "Title=$$TOPIC$$"))
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(char_a, ""))
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(char_a, ""))
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(char_a, "[FILES]"))
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(char_a, "$$FILES$$"))
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(char_a, ""))
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(char_a, "[INFOTYPES]"))
	STATIC_CONST_ARRAY_ENTRY_LAST(IMPLEMENT_CONST_STRING_ENTRY(char_a, ""))
END_STATIC_CONST_ARRAY(g_chm_def_body)

// g_script_body

BEGIN_STATIC_CONST_STRING(g_script_body)
	STATIC_CONST_STRING_ENTRY(
"Sub fRunCmd(objWsh, strCmd)" "\r\n"
"Dim iRet" "\r\n"
"iRet = objWsh.Run(strCmd, 0, true)" "\r\n"
"If iRet <> 0 Then" "\r\n"
"WScript.Echo \"Error: Command [\" & strCmd & \"] failed!\"" "\r\n"
"WScript.Quit(1)" "\r\n"
"End If" "\r\n"
"WScript.Sleep 50" "\r\n"
"End Sub" "\r\n"
"Set objWsh = WScript.CreateObject(\"WScript.Shell\")" "\r\n"
"Dim strTemp" "\r\n"
"Dim arrMd" "\r\n"
"Dim arrPrefix" "\r\n"
"arrMd = Array($$FILES$$)" "\r\n"
"arrPrefix = Array($$CSSPREFIX$$)" "\r\n"
"WScript.Echo \"Compile markdown files...\"" "\r\n"
"For i = 0 To UBound(arrMd)" "\r\n"
"$$COMMANDS$$"
"Next" "\r\n"
"$$ENCODING$$"
"WScript.Echo \"Process html files...\"" "\r\n"
"Sub fHtmlProcessing(strTitle, strPrefix)" "\r\n"
"strTemp = \"txtf -html $$OPTIONS$$ \" & Chr(34) & strTitle & \".html\" & Chr(34) & \" \" & Chr(34) & strTitle & \"$$HTMLEXT$$\" & Chr(34)" "\r\n"
"fRunCmd objWsh, strTemp" "\r\n"
"strTemp = \"%comspec% /C del \" & Chr(34) & strTitle & \".html\" & Chr(34)" "\r\n"
"fRunCmd objWsh, strTemp" "\r\n"
"strTemp = \"%comspec% /C del /F /Q \" & Chr(34) & strTitle & \".md\" & Chr(34)" "\r\n"
"fRunCmd objWsh, strTemp" "\r\n"
"End Sub" "\r\n"
"For i = 0 To UBound(arrMd)" "\r\n"
"fHtmlProcessing arrMd(i), arrPrefix(i)" "\r\n"
"Next" "\r\n"
"fHtmlProcessing \"$$DIRPREFIX$$\\$$PROJECTNAME$$-cover\", \"\"" "\r\n"
"WScript.Echo \"Generate eBook...\"" "\r\n"
"$$EBOOKCMD$$"
"Set objWsh = Nothing" "\r\n"
"WScript.Echo \"Done.\"" "\r\n"
	)
END_STATIC_CONST_STRING(g_script_body)

// g_script_command_item

BEGIN_STATIC_CONST_STRING(g_script_command_item)
	STATIC_CONST_STRING_ENTRY(
"strTemp = \"$$CMDITEM$$\"" "\r\n"
"fRunCmd objWsh, strTemp" "\r\n"
	)
END_STATIC_CONST_STRING(g_script_command_item)

// g_script_chm_encoding

BEGIN_STATIC_CONST_STRING(g_script_chm_encoding)
	STATIC_CONST_STRING_ENTRY(
"WScript.Echo \"Convert encoding...\"" "\r\n"
"Sub fChmEncoding(strFile)" "\r\n"
"strTemp = \"txtf -encoding UTF-8 $$CHARSET$$ \" & Chr(34) & strFile & Chr(34) & \" \" & Chr(34) & strFile & \".tmp\" & Chr(34)" "\r\n"
"fRunCmd objWsh, strTemp" "\r\n"
"strTemp = \"%comspec% /C del \" & Chr(34) & strFile & Chr(34)" "\r\n"
"fRunCmd objWsh, strTemp" "\r\n"
"strTemp = \"%comspec% /C move \" & Chr(34) & strFile & \".tmp\" & Chr(34) & \" \" & Chr(34) & strFile & Chr(34)" "\r\n"
"fRunCmd objWsh, strTemp" "\r\n"
"End Sub" "\r\n"
"For i = 0 To UBound(arrMd)" "\r\n"
"fChmEncoding arrMd(i) & \".html\"" "\r\n"
"Next" "\r\n"
"fChmEncoding \"$$DIRPREFIX$$\\$$PROJECTNAME$$-cover.html\"" "\r\n"
"fChmEncoding \"$$DIRPREFIX$$\\$$PROJECTNAME$$$$DEF$$\"" "\r\n"
"fChmEncoding \"$$DIRPREFIX$$\\$$PROJECTNAME$$.hhc\"" "\r\n"
"fChmEncoding \"$$DIRPREFIX$$\\$$PROJECTNAME$$.hhk\"" "\r\n"
	)
END_STATIC_CONST_STRING(g_script_chm_encoding)

// g_script_chm_cmd

BEGIN_STATIC_CONST_STRING(g_script_chm_cmd)
	STATIC_CONST_STRING_ENTRY(
"strTemp = \"%comspec% /C cd doc & \"" "\r\n"
"strTemp = strTemp & \"hhc \" & Chr(34) & \"$$PROJECTNAME$$.hhp\" & Chr(34)" "\r\n"
"strTemp = strTemp & \" & cd ..\"" "\r\n"
"fRunCmd objWsh, strTemp" "\r\n"
"strTemp = \"%comspec% /C move \" & Chr(34) & \"doc\\$$PROJECTNAME$$.chm\" & Chr(34) & \" .\"" "\r\n"
"fRunCmd objWsh, strTemp" "\r\n"
	)
END_STATIC_CONST_STRING(g_script_chm_cmd)

// g_script_epub_cmd

BEGIN_STATIC_CONST_STRING(g_script_epub_cmd)
	STATIC_CONST_STRING_ENTRY(
"strTemp = \"%comspec% /C cd doc & \"" "\r\n"
"strTemp = strTemp & \"7z a -mx0 -tzip \" & Chr(34) & \"$$PROJECTNAME$$.epub\" & Chr(34) & \" mimetype\"" "\r\n"
"strTemp = strTemp & \" & \"" "\r\n"
"strTemp = strTemp & \"7z a -mx9 -r -tzip \" & Chr(34) & \"$$PROJECTNAME$$.epub\" & Chr(34) & \" META-INF OEBPS\"" "\r\n"
"strTemp = strTemp & \" & cd ..\"" "\r\n"
"fRunCmd objWsh, strTemp" "\r\n"
"strTemp = \"%comspec% /C move \" & Chr(34) & \"doc\\$$PROJECTNAME$$.epub\" & Chr(34) & \" .\"" "\r\n"
"fRunCmd objWsh, strTemp" "\r\n"
	)
END_STATIC_CONST_STRING(g_script_epub_cmd)

////////////////////////////////////////////////////////////////////////////////
