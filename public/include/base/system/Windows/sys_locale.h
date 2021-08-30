/*
** Copyright (c) 2015, Xin YUAN, courses of Zhejiang University
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
// internal header
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

//Windows

//------------------------------------------------------------------------------
// tools

// get_current_locale
inline void get_current_locale(OUT locale_info& info, OUT code_page& cp) throw()
{
	info.szLanguageName[0] = 0;
	info.uLanguageLength = 0;
	info.szRegionName[0] = 0;
	info.uRegionLength = 0;
	cp.szName[0] = 0;
	cp.uLength = 0;
	//locale
	//  language[-script][-region][-x-custom][_xxxx]
	WCHAR sz[LOCALE_NAME_MAX_LENGTH + 1];
	int ret = ::GetSystemDefaultLocaleName(sz, LOCALE_NAME_MAX_LENGTH + 1);
	assert( ret > 0 );
	WCHAR* szT;
	WCHAR* szE = sz + ret - 1;  // ret including the terminating null character
	//find '_'
	szT = find_string_char(sz, L'_');
	if( szT != NULL )
		szE = szT;
	//find "-x-"
	szT = find_string_string(sz, L"-x-");
	if( szT != NULL )
		szE = szT;
	//find '-'
	szT = find_string_char(sz, L'-');
	if( szT != NULL && szT < szE ) {
		WCHAR* szR = szT;
		//find second '-'
		WCHAR* sz1 = find_string_char(szT + 1, L'-');
		if( sz1 != NULL && sz1 < szE )
			szR = sz1;
		//region
		info.uRegionLength = (uint)(uintptr)(szE - (szR + 1));
		assert( info.uRegionLength < locale_info::REGION_LEN );
		mem_copy(szR + 1, info.uRegionLength * sizeof(WCHAR), info.szRegionName);
		info.szRegionName[info.uRegionLength] = 0;
		szE = szT;
	}
	//language
	info.uLanguageLength = (uint)(uintptr)(szE - sz);
	assert( info.uLanguageLength < locale_info::LANGUAGE_LEN );
	mem_copy(sz, info.uLanguageLength * sizeof(WCHAR), info.szLanguageName);
	info.szLanguageName[info.uLanguageLength] = 0;
	//code page
	UINT uCP = ::GetACP();
	ret = value_to_string(cp.szName, code_page::NAME_LEN, L"%u", uCP);
	if( ret >= 0 ) {
		cp.szName[ret] = 0;
		cp.uLength = ret;
	}
}

//------------------------------------------------------------------------------
// constant strings for code page

#define \
   CODEPAGE_037       _S("037")     // IBM037                     [IBM EBCDIC US-Canada]
// CODEPAGE_708       _S("708")     // ASMO-708                   [Arabic (ASMO 708)]
// CODEPAGE_709       _S("709")     //                            [Arabic (ASMO-449+, BCON V4)]
// CODEPAGE_710       _S("710")     //                            [Arabic - Transparent Arabic]
// CODEPAGE_720       _S("720")     // DOS-720                    [Arabic (Transparent ASMO); Arabic (DOS)]
#define \
   CODEPAGE_737       _S("737")     // ibm737                     [OEM Greek (formerly 437G); Greek (DOS)]
#define \
   CODEPAGE_775       _S("775")     // ibm775                     [OEM Baltic; Baltic (DOS)]
// CODEPAGE_858       _S("858")     // IBM00858                   [OEM Multilingual Latin 1 + Euro symbol]
#define \
   CODEPAGE_870       _S("870")     // IBM870                     [IBM EBCDIC Multilingual/ROECE (Latin 2); IBM EBCDIC Multilingual Latin 2]
#define \
   CODEPAGE_875       _S("875")     // cp875                      [IBM EBCDIC Greek Modern]
#define \
   CODEPAGE_932       _S("932")     // shift_jis                  [ANSI/OEM Japanese; Japanese (Shift-JIS)]
#define \
   CODEPAGE_936       _S("936")     // gb2312                     [ANSI/OEM Simplified Chinese (PRC, Singapore); Chinese Simplified (GB2312)]
#define \
   CODEPAGE_949       _S("949")     // ks_c_5601-1987             [ANSI/OEM Korean (Unified Hangul Code)]
#define \
   CODEPAGE_950       _S("950")     // big5                       [ANSI/OEM Traditional Chinese (Taiwan; Hong Kong SAR, PRC); Chinese Traditional (Big5)]
#define \
   CODEPAGE_1140      _S("1140")    // IBM01140                   [IBM EBCDIC US-Canada (037 + Euro symbol); IBM EBCDIC (US-Canada-Euro)]
#define \
   CODEPAGE_1141      _S("1141")    // IBM01141                   [IBM EBCDIC Germany (20273 + Euro symbol); IBM EBCDIC (Germany-Euro)]
#define \
   CODEPAGE_1142      _S("1142")    // IBM01142                   [IBM EBCDIC Denmark-Norway (20277 + Euro symbol); IBM EBCDIC (Denmark-Norway-Euro)]
#define \
   CODEPAGE_1143      _S("1143")    // IBM01143                   [IBM EBCDIC Finland-Sweden (20278 + Euro symbol); IBM EBCDIC (Finland-Sweden-Euro)]
#define \
   CODEPAGE_1144      _S("1144")    // IBM01144                   [IBM EBCDIC Italy (20280 + Euro symbol); IBM EBCDIC (Italy-Euro)]
#define \
   CODEPAGE_1145      _S("1145")    // IBM01145                   [IBM EBCDIC Latin America-Spain (20284 + Euro symbol); IBM EBCDIC (Spain-Euro)]
#define \
   CODEPAGE_1146      _S("1146")    // IBM01146                   [IBM EBCDIC United Kingdom (20285 + Euro symbol); IBM EBCDIC (UK-Euro)]
#define \
   CODEPAGE_1147      _S("1147")    // IBM01147                   [IBM EBCDIC France (20297 + Euro symbol); IBM EBCDIC (France-Euro)]
#define \
   CODEPAGE_1148      _S("1148")    // IBM01148                   [IBM EBCDIC International (500 + Euro symbol); IBM EBCDIC (International-Euro)]
#define \
   CODEPAGE_1149      _S("1149")    // IBM01149                   [IBM EBCDIC Icelandic (20871 + Euro symbol); IBM EBCDIC (Icelandic-Euro)]
// CODEPAGE_1200      _S("1200")    // utf-16                     [Unicode UTF-16, little endian byte order (BMP of ISO 10646); available only to managed applications]
// CODEPAGE_1201      _S("1201")    // unicodeFFFE                [Unicode UTF-16, big endian byte order; available only to managed applications]
#define \
   CODEPAGE_1250      _S("1250")    // windows-1250               [ANSI Central European; Central European (Windows)]
#define \
   CODEPAGE_1251      _S("1251")    // windows-1251               [ANSI Cyrillic; Cyrillic (Windows)]
#define \
   CODEPAGE_1252      _S("1252")    // windows-1252               [ANSI Latin 1; Western European (Windows)]
#define \
   CODEPAGE_1253      _S("1253")    // windows-1253               [ANSI Greek; Greek (Windows)]
#define \
   CODEPAGE_1254      _S("1254")    // windows-1254               [ANSI Turkish; Turkish (Windows)]
#define \
   CODEPAGE_1255      _S("1255")    // windows-1255               [ANSI Hebrew; Hebrew (Windows)]
#define \
   CODEPAGE_1256      _S("1256")    // windows-1256               [ANSI Arabic; Arabic (Windows)]
#define \
   CODEPAGE_1257      _S("1257")    // windows-1257               [ANSI Baltic; Baltic (Windows)]
#define \
   CODEPAGE_1258      _S("1258")    // windows-1258               [ANSI/OEM Vietnamese; Vietnamese (Windows)]
#define \
   CODEPAGE_1361      _S("1361")    // Johab                      [Korean (Johab)]
#define \
   CODEPAGE_10000     _S("10000")   // macintosh                  [MAC Roman; Western European (Mac)]
// CODEPAGE_10001     _S("10001")   // x-mac-japanese             [Japanese (Mac)]
// CODEPAGE_10002     _S("10002")   // x-mac-chinesetrad          [MAC Traditional Chinese (Big5); Chinese Traditional (Mac)]
// CODEPAGE_10003     _S("10003")   // x-mac-korean               [Korean (Mac)]
// CODEPAGE_10004     _S("10004")   // x-mac-arabic               [Arabic (Mac)]
// CODEPAGE_10005     _S("10005")   // x-mac-hebrew               [Hebrew (Mac)]
// CODEPAGE_10006     _S("10006")   // x-mac-greek                [Greek (Mac)]
#define \
   CODEPAGE_10007     _S("10007")   // x-mac-cyrillic             [Cyrillic (Mac)]
// CODEPAGE_10008     _S("10008")   // x-mac-chinesesimp          [MAC Simplified Chinese (GB 2312); Chinese Simplified (Mac)]
// CODEPAGE_10010     _S("10010")   // x-mac-romanian             [Romanian (Mac)]
#define \
   CODEPAGE_10017     _S("10017")   // x-mac-ukrainian            [Ukrainian (Mac)]
// CODEPAGE_10021     _S("10021")   // x-mac-thai                 [Thai (Mac)]
#define \
   CODEPAGE_10029     _S("10029")   // x-mac-ce                   [MAC Latin 2; Central European (Mac)]
// CODEPAGE_10079     _S("10079")   // x-mac-icelandic            [Icelandic (Mac)]
// CODEPAGE_10081     _S("10081")   // x-mac-turkish              [Turkish (Mac)]
// CODEPAGE_10082     _S("10082")   // x-mac-croatian             [Croatian (Mac)]
// CODEPAGE_12000     _S("12000")   // utf-32                     [Unicode UTF-32, little endian byte order; available only to managed applications]
// CODEPAGE_12001     _S("12001")   // utf-32BE                   [Unicode UTF-32, big endian byte order; available only to managed applications]
// CODEPAGE_20000     _S("20000")   // x-Chinese_CNS              [CNS Taiwan; Chinese Traditional (CNS)]
// CODEPAGE_20001     _S("20001")   // x-cp20001                  [TCA Taiwan]
// CODEPAGE_20002     _S("20002")   // x_Chinese-Eten             [Eten Taiwan; Chinese Traditional (Eten)]
// CODEPAGE_20003     _S("20003")   // x-cp20003                  [IBM5550 Taiwan]
// CODEPAGE_20004     _S("20004")   // x-cp20004                  [TeleText Taiwan]
// CODEPAGE_20005     _S("20005")   // x-cp20005                  [Wang Taiwan]
// CODEPAGE_20105     _S("20105")   // x-IA5                      [IA5 (IRV International Alphabet No. 5, 7-bit); Western European (IA5)]
// CODEPAGE_20106     _S("20106")   // x-IA5-German               [IA5 German (7-bit)]
// CODEPAGE_20107     _S("20107")   // x-IA5-Swedish              [IA5 Swedish (7-bit)]
// CODEPAGE_20108     _S("20108")   // x-IA5-Norwegian            [IA5 Norwegian (7-bit)]
#define \
   CODEPAGE_20127     _S("20127")   // us-ascii                   [US-ASCII (7-bit)]
#define \
   CODEPAGE_20261     _S("20261")   // x-cp20261                  [T.61]
// CODEPAGE_20269     _S("20269")   // x-cp20269                  [ISO 6937 Non-Spacing Accent]
#define \
   CODEPAGE_273       _S("20273")   // IBM273                     [IBM EBCDIC Germany]
#define \
   CODEPAGE_277       _S("20277")   // IBM277                     [IBM EBCDIC Denmark-Norway]
#define \
   CODEPAGE_278       _S("20278")   // IBM278                     [IBM EBCDIC Finland-Sweden]
#define \
   CODEPAGE_280       _S("20280")   // IBM280                     [IBM EBCDIC Italy]
#define \
   CODEPAGE_284       _S("20284")   // IBM284                     [IBM EBCDIC Latin America-Spain]
#define \
   CODEPAGE_285       _S("20285")   // IBM285                     [IBM EBCDIC United Kingdom]
#define \
   CODEPAGE_290       _S("20290")   // IBM290                     [IBM EBCDIC Japanese Katakana Extended]
#define \
   CODEPAGE_297       _S("20297")   // IBM297                     [IBM EBCDIC France]
#define \
   CODEPAGE_420       _S("20420")   // IBM420                     [IBM EBCDIC Arabic]
#define \
   CODEPAGE_423       _S("20423")   // IBM423                     [IBM EBCDIC Greek]
#define \
   CODEPAGE_424       _S("20424")   // IBM424                     [IBM EBCDIC Hebrew]
// CODEPAGE_20833     _S("20833")   // x-EBCDIC-KoreanExtended    [IBM EBCDIC Korean Extended]
// CODEPAGE_20838     _S("20838")   // IBM-Thai                   [IBM EBCDIC Thai]
#define \
   CODEPAGE_866_1     _S("20866")   // koi8-r                     [Russian (KOI8-R); Cyrillic (KOI8-R)]
#define \
   CODEPAGE_871       _S("20871")   // IBM871                     [IBM EBCDIC Icelandic]
#define \
   CODEPAGE_880       _S("20880")   // IBM880                     [IBM EBCDIC Cyrillic Russian]
#define \
   CODEPAGE_905       _S("20905")   // IBM905                     [IBM EBCDIC Turkish]
// CODEPAGE_20924     _S("20924")   // IBM00924                   [IBM EBCDIC Latin 1/Open System (1047 + Euro symbol)]
// CODEPAGE_20932     _S("20932")   // EUC-JP                     [Japanese (JIS 0208-1990 and 0212-1990)]
// CODEPAGE_20936     _S("20936")   // x-cp20936                  [Simplified Chinese (GB2312); Chinese Simplified (GB2312-80)]
// CODEPAGE_20949     _S("20949")   // x-cp20949                  [Korean Wansung]
#define \
   CODEPAGE_1025      _S("21025")   // cp1025                     [IBM EBCDIC Cyrillic Serbian-Bulgarian]
// CODEPAGE_21027     _S("21027")   //                            [(deprecated)]
#define \
   CODEPAGE_866_2     _S("21866")   // koi8-u                     [Ukrainian (KOI8-U); Cyrillic (KOI8-U)]
#define \
   CODEPAGE_8859_1    _S("28591")   // iso-8859-1                 [ISO 8859-1 Latin 1; Western European (ISO)]
#define \
   CODEPAGE_8859_2    _S("28592")   // iso-8859-2                 [ISO 8859-2 Central European; Central European (ISO)]
#define \
   CODEPAGE_8859_3    _S("28593")   // iso-8859-3                 [ISO 8859-3 Latin 3]
#define \
   CODEPAGE_8859_4    _S("28594")   // iso-8859-4                 [ISO 8859-4 Baltic]
#define \
   CODEPAGE_8859_5    _S("28595")   // iso-8859-5                 [ISO 8859-5 Cyrillic]
#define \
   CODEPAGE_8859_6    _S("28596")   // iso-8859-6                 [ISO 8859-6 Arabic]
#define \
   CODEPAGE_8859_7    _S("28597")   // iso-8859-7                 [ISO 8859-7 Greek]
#define \
   CODEPAGE_8859_8    _S("28598")   // iso-8859-8                 [ISO 8859-8 Hebrew; Hebrew (ISO-Visual)]
#define \
   CODEPAGE_8859_9    _S("28599")   // iso-8859-9                 [ISO 8859-9 Turkish]
#define \
   CODEPAGE_8859_13   _S("28603")   // iso-8859-13                [ISO 8859-13 Estonian]
#define \
   CODEPAGE_8859_15   _S("28605")   // iso-8859-15                [ISO 8859-15 Latin 9]
// CODEPAGE_29001     _S("29001")   // x-Europa                   [Europa 3]
// CODEPAGE_38598     _S("38598")   // iso-8859-8-i               [ISO 8859-8 Hebrew; Hebrew (ISO-Logical)]
#define \
   CODEPAGE_2022_0    _S("50220")   // iso-2022-jp                [ISO 2022 Japanese with no halfwidth Katakana; Japanese (JIS)]
#define \
   CODEPAGE_2022_1    _S("50221")   // csISO2022JP                [ISO 2022 Japanese with halfwidth Katakana; Japanese (JIS-Allow 1 byte Kana)]
#define \
   CODEPAGE_2022_2    _S("50222")   // iso-2022-jp                [ISO 2022 Japanese JIS X 0201-1989; Japanese (JIS-Allow 1 byte Kana - SO/SI)]
#define \
   CODEPAGE_2022_5    _S("50225")   // iso-2022-kr                [ISO 2022 Korean]
#define \
   CODEPAGE_2022_7    _S("50227")   // x-cp50227                  [ISO 2022 Simplified Chinese; Chinese Simplified (ISO 2022)]
#define \
   CODEPAGE_2022_9    _S("50229")   //                            [ISO 2022 Traditional Chinese]
#define \
   CODEPAGE_930       _S("50930")   //                            [EBCDIC Japanese (Katakana) Extended]
// CODEPAGE_50931     _S("50931")   //                            [EBCDIC US-Canada and Japanese]
#define \
   CODEPAGE_933       _S("50933")   //                            [EBCDIC Korean Extended and Korean]
#define \
   CODEPAGE_935       _S("50935")   //                            [EBCDIC Simplified Chinese Extended and Simplified Chinese]
// CODEPAGE_50936     _S("50936")   //                            [EBCDIC Simplified Chinese]
#define \
   CODEPAGE_937       _S("50937")   //                            [EBCDIC US-Canada and Traditional Chinese]
#define \
   CODEPAGE_939       _S("50939")   //                            [EBCDIC Japanese (Latin) Extended and Japanese]
#define \
   CODEPAGE_932_1     _S("51932")   // euc-jp                     [EUC Japanese]
#define \
   CODEPAGE_936_1     _S("51936")   // EUC-CN                     [EUC Simplified Chinese; Chinese Simplified (EUC)]
#define \
   CODEPAGE_949_1     _S("51949")   // euc-kr                     [EUC Korean]
#define \
   CODEPAGE_950_1     _S("51950")   //                            [EUC Traditional Chinese]
// CODEPAGE_52936     _S("52936")   // hz-gb-2312                 [HZ-GB2312 Simplified Chinese; Chinese Simplified (HZ)]
#define \
   CODEPAGE_18030     _S("54936")   // GB18030                    [Windows XP and later: GB18030 Simplified Chinese (4 byte); Chinese Simplified (GB18030)]
// CODEPAGE_57002     _S("57002")   // x-iscii-de                 [ISCII Devanagari]
// CODEPAGE_57003     _S("57003")   // x-iscii-be                 [ISCII Bengali]
// CODEPAGE_57004     _S("57004")   // x-iscii-ta                 [ISCII Tamil]
// CODEPAGE_57005     _S("57005")   // x-iscii-te                 [ISCII Telugu]
// CODEPAGE_57006     _S("57006")   // x-iscii-as                 [ISCII Assamese]
// CODEPAGE_57007     _S("57007")   // x-iscii-or                 [ISCII Oriya]
// CODEPAGE_57008     _S("57008")   // x-iscii-ka                 [ISCII Kannada]
// CODEPAGE_57009     _S("57009")   // x-iscii-ma                 [ISCII Malayalam]
// CODEPAGE_57010     _S("57010")   // x-iscii-gu                 [ISCII Gujarati]
// CODEPAGE_57011     _S("57011")   // x-iscii-pa                 [ISCII Punjabi]
// CODEPAGE_65000     _S("65000")   // utf-7                      [Unicode (UTF-7)]
// CODEPAGE_65001     _S("65001")   // utf-8                      [Unicode (UTF-8)]

//------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
