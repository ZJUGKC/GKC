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

//Linux

//------------------------------------------------------------------------------
// tools

// get_current_locale
//   this function is not thread-safe
inline void get_current_locale(OUT locale_info& info, OUT code_page& cp) throw()
{
	info.szLanguageName[0] = 0;
	info.uLanguageLength = 0;
	info.szRegionName[0] = 0;
	info.uRegionLength = 0;
	cp.szName[0] = 0;
	cp.uLength = 0;
	//locale
	//  language[_territory][.codeset][@modifier]
	char* sz = ::setlocale(LC_ALL, NULL);  //LC_CTYPE
	assert( sz != NULL );
	char* szT;
	char* szE = sz + calc_string_length(sz);
	//find '@'
	szT = find_string_char(sz, '@');
	if( szT != NULL )
		szE = szT;
	//find '.'
	szT = find_string_char(sz, '.');
	if( szT != NULL ) {
		assert( szT < szE );
		//codeset
		cp.uLength = (uint)(uintptr)(szE - (szT + 1));
		assert( cp.uLength < code_page::NAME_LEN );
		mem_copy(szT + 1, cp.uLength * sizeof(char), cp.szName);
		cp.szName[cp.uLength] = 0;
		szE = szT;
	}
	//find '_'
	szT = find_string_char(sz, '_');
	if( szT != NULL ) {
		assert( szT < szE );
		//territory
		info.uRegionLength = (uint)(uintptr)(szE - (szT + 1));
		assert( info.uRegionLength < locale_info::REGION_LEN );
		mem_copy(szT + 1, info.uRegionLength * sizeof(char), info.szRegionName);
		info.szRegionName[info.uRegionLength] = 0;
		szE = szT;
	}
	//language
	info.uLanguageLength = (uint)(uintptr)(szE - sz);
	assert( info.uLanguageLength < locale_info::LANGUAGE_LEN );
	mem_copy(sz, info.uLanguageLength * sizeof(char), info.szLanguageName);
	info.szLanguageName[info.uLanguageLength] = 0;
}

//------------------------------------------------------------------------------
// constant strings for code page

// CODEPAGE_500V1                    _S("500V1")                    // 500V1
// CODEPAGE_851                      _S("851")                      // 851
// CODEPAGE_856                      _S("856")                      // 856
// CODEPAGE_866NAV                   _S("866NAV")                   // 866NAV
// CODEPAGE_904                      _S("904")                      // 904
// CODEPAGE_1046                     _S("1046")                     // 1046
#define \
   CODEPAGE_8859_1                   _S("8859_1")                   // 8859_1
#define \
   CODEPAGE_8859_2                   _S("8859_2")                   // 8859_2
#define \
   CODEPAGE_8859_3                   _S("8859_3")                   // 8859_3
#define \
   CODEPAGE_8859_4                   _S("8859_4")                   // 8859_4
#define \
   CODEPAGE_8859_5                   _S("8859_5")                   // 8859_5
#define \
   CODEPAGE_8859_6                   _S("8859_6")                   // 8859_6
#define \
   CODEPAGE_8859_7                   _S("8859_7")                   // 8859_7
#define \
   CODEPAGE_8859_8                   _S("8859_8")                   // 8859_8
#define \
   CODEPAGE_8859_9                   _S("8859_9")                   // 8859_9
// CODEPAGE_10646_1_1993             _S("10646-1:1993")             // 10646-1:1993
// CODEPAGE_10646_1_1993_UCS4        _S("10646-1:1993/UCS4")        // 10646-1:1993/UCS4
// CODEPAGE_ANSI_X3_4_1968           _S("ANSI_X3.4-1968")           // ANSI_X3.4-1968
// CODEPAGE_ANSI_X3_4_1986           _S("ANSI_X3.4-1986")           // ANSI_X3.4-1986
// CODEPAGE_ANSI_X3_4                _S("ANSI_X3.4")                // ANSI_X3.4
// CODEPAGE_ANSI_X3_110_1983         _S("ANSI_X3.110-1983")         // ANSI_X3.110-1983
// CODEPAGE_ANSI_X3_110              _S("ANSI_X3.110")              // ANSI_X3.110
// CODEPAGE_ARABIC                   _S("ARABIC")                   // ARABIC
// CODEPAGE_ARABIC7                  _S("ARABIC7")                  // ARABIC7
// CODEPAGE_ARMSCII_8                _S("ARMSCII-8")                // ARMSCII-8
// CODEPAGE_ASCII                    _S("ASCII")                    // ASCII
// CODEPAGE_ASMO_708                 _S("ASMO-708")                 // ASMO-708
// CODEPAGE_ASMO_449                 _S("ASMO_449")                 // ASMO_449
// CODEPAGE_BALTIC                   _S("BALTIC")                   // BALTIC
#define \
   CODEPAGE_950                      _S("BIG-5")                    // BIG-5
// CODEPAGE_BIG_FIVE                 _S("BIG-FIVE")                 // BIG-FIVE
// CODEPAGE_BIG5_HKSCS               _S("BIG5-HKSCS")               // BIG5-HKSCS
// CODEPAGE_BIG5                     _S("BIG5")                     // BIG5
// CODEPAGE_BIG5HKSCS                _S("BIG5HKSCS")                // BIG5HKSCS
// CODEPAGE_BIGFIVE                  _S("BIGFIVE")                  // BIGFIVE
// CODEPAGE_BRF                      _S("BRF")                      // BRF
// CODEPAGE_BS_4730                  _S("BS_4730")                  // BS_4730
// CODEPAGE_CA                       _S("CA")                       // CA
// CODEPAGE_CN_BIG5                  _S("CN-BIG5")                  // CN-BIG5
// CODEPAGE_CN_GB                    _S("CN-GB")                    // CN-GB
// CODEPAGE_CN                       _S("CN")                       // CN
// CODEPAGE_CP_AR                    _S("CP-AR")                    // CP-AR
// CODEPAGE_CP_GR                    _S("CP-GR")                    // CP-GR
// CODEPAGE_CP_HU                    _S("CP-HU")                    // CP-HU
#define \
   CODEPAGE_037                      _S("CP037")                    // CP037
// CODEPAGE_CP038                    _S("CP038")                    // CP038
#define \
   CODEPAGE_273                      _S("CP273")                    // CP273
// CODEPAGE_CP274                    _S("CP274")                    // CP274
// CODEPAGE_CP275                    _S("CP275")                    // CP275
#define \
   CODEPAGE_278                      _S("CP278")                    // CP278
#define \
   CODEPAGE_280                      _S("CP280")                    // CP280
// CODEPAGE_CP281                    _S("CP281")                    // CP281
// CODEPAGE_CP282                    _S("CP282")                    // CP282
#define \
   CODEPAGE_284                      _S("CP284")                    // CP284
#define \
   CODEPAGE_285                      _S("CP285")                    // CP285
#define \
   CODEPAGE_290                      _S("CP290")                    // CP290
#define \
   CODEPAGE_297                      _S("CP297")                    // CP297
// CODEPAGE_CP367                    _S("CP367")                    // CP367
#define \
   CODEPAGE_420                      _S("CP420")                    // CP420
#define \
   CODEPAGE_423                      _S("CP423")                    // CP423
#define \
   CODEPAGE_424                      _S("CP424")                    // CP424
// CODEPAGE_437                      _S("CP437")                    // CP437
// CODEPAGE_500                      _S("CP500")                    // CP500
#define \
   CODEPAGE_737                      _S("CP737")                    // CP737
// CODEPAGE_CP770                    _S("CP770")                    // CP770
// CODEPAGE_CP771                    _S("CP771")                    // CP771
// CODEPAGE_CP772                    _S("CP772")                    // CP772
// CODEPAGE_CP773                    _S("CP773")                    // CP773
// CODEPAGE_CP774                    _S("CP774")                    // CP774
#define \
   CODEPAGE_775                      _S("CP775")                    // CP775
// CODEPAGE_CP803                    _S("CP803")                    // CP803
// CODEPAGE_CP813                    _S("CP813")                    // CP813
// CODEPAGE_CP819                    _S("CP819")                    // CP819
// CODEPAGE_CP850                    _S("CP850")                    // CP850
// CODEPAGE_CP851                    _S("CP851")                    // CP851
// CODEPAGE_CP852                    _S("CP852")                    // CP852
// CODEPAGE_CP855                    _S("CP855")                    // CP855
// CODEPAGE_CP856                    _S("CP856")                    // CP856
// CODEPAGE_CP857                    _S("CP857")                    // CP857
// CODEPAGE_CP860                    _S("CP860")                    // CP860
// CODEPAGE_CP861                    _S("CP861")                    // CP861
// CODEPAGE_CP862                    _S("CP862")                    // CP862
// CODEPAGE_CP863                    _S("CP863")                    // CP863
// CODEPAGE_CP864                    _S("CP864")                    // CP864
// CODEPAGE_CP865                    _S("CP865")                    // CP865
// CODEPAGE_CP866                    _S("CP866")                    // CP866
// CODEPAGE_CP866NAV                 _S("CP866NAV")                 // CP866NAV
// CODEPAGE_CP868                    _S("CP868")                    // CP868
// CODEPAGE_CP869                    _S("CP869")                    // CP869
#define \
   CODEPAGE_870                      _S("CP870")                    // CP870
#define \
   CODEPAGE_871                      _S("CP871")                    // CP871
// CODEPAGE_CP874                    _S("CP874")                    // CP874
#define \
   CODEPAGE_875                      _S("CP875")                    // CP875
#define \
   CODEPAGE_880                      _S("CP880")                    // CP880
// CODEPAGE_CP891                    _S("CP891")                    // CP891
// CODEPAGE_CP901                    _S("CP901")                    // CP901
// CODEPAGE_CP902                    _S("CP902")                    // CP902
// CODEPAGE_CP903                    _S("CP903")                    // CP903
// CODEPAGE_CP904                    _S("CP904")                    // CP904
#define \
   CODEPAGE_905                      _S("CP905")                    // CP905
// CODEPAGE_CP912                    _S("CP912")                    // CP912
// CODEPAGE_CP915                    _S("CP915")                    // CP915
// CODEPAGE_CP916                    _S("CP916")                    // CP916
// CODEPAGE_CP918                    _S("CP918")                    // CP918
// CODEPAGE_CP920                    _S("CP920")                    // CP920
// CODEPAGE_CP921                    _S("CP921")                    // CP921
// CODEPAGE_CP922                    _S("CP922")                    // CP922
#define \
   CODEPAGE_930                      _S("CP930")                    // CP930
#define \
   CODEPAGE_932                      _S("CP932")                    // CP932
#define \
   CODEPAGE_933                      _S("CP933")                    // CP933
#define \
   CODEPAGE_935                      _S("CP935")                    // CP935
#define \
   CODEPAGE_936                      _S("CP936")                    // CP936
#define \
   CODEPAGE_937                      _S("CP937")                    // CP937
#define \
   CODEPAGE_939                      _S("CP939")                    // CP939
#define \
   CODEPAGE_949                      _S("CP949")                    // CP949
// CODEPAGE_CP950                    _S("CP950")                    // CP950
// CODEPAGE_CP1004                   _S("CP1004")                   // CP1004
// CODEPAGE_CP1008                   _S("CP1008")                   // CP1008
#define \
   CODEPAGE_1025                     _S("CP1025")                   // CP1025
// CODEPAGE_CP1026                   _S("CP1026")                   // CP1026
// CODEPAGE_CP1046                   _S("CP1046")                   // CP1046
// CODEPAGE_CP1047                   _S("CP1047")                   // CP1047
// CODEPAGE_CP1070                   _S("CP1070")                   // CP1070
// CODEPAGE_CP1079                   _S("CP1079")                   // CP1079
// CODEPAGE_CP1081                   _S("CP1081")                   // CP1081
// CODEPAGE_CP1084                   _S("CP1084")                   // CP1084
// CODEPAGE_CP1089                   _S("CP1089")                   // CP1089
// CODEPAGE_CP1097                   _S("CP1097")                   // CP1097
// CODEPAGE_CP1112                   _S("CP1112")                   // CP1112
// CODEPAGE_CP1122                   _S("CP1122")                   // CP1122
// CODEPAGE_CP1123                   _S("CP1123")                   // CP1123
// CODEPAGE_CP1124                   _S("CP1124")                   // CP1124
// CODEPAGE_CP1125                   _S("CP1125")                   // CP1125
// CODEPAGE_CP1129                   _S("CP1129")                   // CP1129
// CODEPAGE_CP1130                   _S("CP1130")                   // CP1130
// CODEPAGE_CP1132                   _S("CP1132")                   // CP1132
// CODEPAGE_CP1133                   _S("CP1133")                   // CP1133
// CODEPAGE_CP1137                   _S("CP1137")                   // CP1137
#define \
   CODEPAGE_1140                     _S("CP1140")                   // CP1140
#define \
   CODEPAGE_1141                     _S("CP1141")                   // CP1141
#define \
   CODEPAGE_1142                     _S("CP1142")                   // CP1142
#define \
   CODEPAGE_1143                     _S("CP1143")                   // CP1143
#define \
   CODEPAGE_1144                     _S("CP1144")                   // CP1144
#define \
   CODEPAGE_1145                     _S("CP1145")                   // CP1145
#define \
   CODEPAGE_1146                     _S("CP1146")                   // CP1146
#define \
   CODEPAGE_1147                     _S("CP1147")                   // CP1147
#define \
   CODEPAGE_1148                     _S("CP1148")                   // CP1148
#define \
   CODEPAGE_1149                     _S("CP1149")                   // CP1149
// CODEPAGE_CP1153                   _S("CP1153")                   // CP1153
// CODEPAGE_CP1154                   _S("CP1154")                   // CP1154
// CODEPAGE_CP1155                   _S("CP1155")                   // CP1155
// CODEPAGE_CP1156                   _S("CP1156")                   // CP1156
// CODEPAGE_CP1157                   _S("CP1157")                   // CP1157
// CODEPAGE_CP1158                   _S("CP1158")                   // CP1158
// CODEPAGE_CP1160                   _S("CP1160")                   // CP1160
// CODEPAGE_CP1161                   _S("CP1161")                   // CP1161
// CODEPAGE_CP1162                   _S("CP1162")                   // CP1162
// CODEPAGE_CP1163                   _S("CP1163")                   // CP1163
// CODEPAGE_CP1164                   _S("CP1164")                   // CP1164
// CODEPAGE_CP1166                   _S("CP1166")                   // CP1166
// CODEPAGE_CP1167                   _S("CP1167")                   // CP1167
#define \
   CODEPAGE_1250                     _S("CP1250")                   // CP1250
#define \
   CODEPAGE_1251                     _S("CP1251")                   // CP1251
#define \
   CODEPAGE_1252                     _S("CP1252")                   // CP1252
#define \
   CODEPAGE_1253                     _S("CP1253")                   // CP1253
#define \
   CODEPAGE_1254                     _S("CP1254")                   // CP1254
#define \
   CODEPAGE_1255                     _S("CP1255")                   // CP1255
#define \
   CODEPAGE_1256                     _S("CP1256")                   // CP1256
#define \
   CODEPAGE_1257                     _S("CP1257")                   // CP1257
#define \
   CODEPAGE_1258                     _S("CP1258")                   // CP1258
// CODEPAGE_CP1282                   _S("CP1282")                   // CP1282
#define \
   CODEPAGE_1361                     _S("CP1361")                   // CP1361
// CODEPAGE_CP1364                   _S("CP1364")                   // CP1364
// CODEPAGE_CP1371                   _S("CP1371")                   // CP1371
// CODEPAGE_CP1388                   _S("CP1388")                   // CP1388
// CODEPAGE_CP1390                   _S("CP1390")                   // CP1390
// CODEPAGE_CP1399                   _S("CP1399")                   // CP1399
// CODEPAGE_CP4517                   _S("CP4517")                   // CP4517
// CODEPAGE_CP4899                   _S("CP4899")                   // CP4899
// CODEPAGE_CP4909                   _S("CP4909")                   // CP4909
// CODEPAGE_CP4971                   _S("CP4971")                   // CP4971
// CODEPAGE_CP5347                   _S("CP5347")                   // CP5347
// CODEPAGE_CP9030                   _S("CP9030")                   // CP9030
// CODEPAGE_CP9066                   _S("CP9066")                   // CP9066
// CODEPAGE_CP9448                   _S("CP9448")                   // CP9448
// CODEPAGE_CP10007                  _S("CP10007")                  // CP10007
// CODEPAGE_CP12712                  _S("CP12712")                  // CP12712
// CODEPAGE_CP16804                  _S("CP16804")                  // CP16804
// CODEPAGE_CPIBM861                 _S("CPIBM861")                 // CPIBM861
// CODEPAGE_CSA7_1                   _S("CSA7-1")                   // CSA7-1
// CODEPAGE_CSA7_2                   _S("CSA7-2")                   // CSA7-2
// CODEPAGE_CSASCII                  _S("CSASCII")                  // CSASCII
// CODEPAGE_CSA_T500_1983            _S("CSA_T500-1983")            // CSA_T500-1983
// CODEPAGE_CSA_T500                 _S("CSA_T500")                 // CSA_T500
// CODEPAGE_CSA_Z243_4_1985_1        _S("CSA_Z243.4-1985-1")        // CSA_Z243.4-1985-1
// CODEPAGE_CSA_Z243_4_1985_2        _S("CSA_Z243.4-1985-2")        // CSA_Z243.4-1985-2
// CODEPAGE_CSA_Z243_419851          _S("CSA_Z243.419851")          // CSA_Z243.419851
// CODEPAGE_CSA_Z243_419852          _S("CSA_Z243.419852")          // CSA_Z243.419852
// CODEPAGE_CSDECMCS                 _S("CSDECMCS")                 // CSDECMCS
// CODEPAGE_CSEBCDICATDE             _S("CSEBCDICATDE")             // CSEBCDICATDE
// CODEPAGE_CSEBCDICATDEA            _S("CSEBCDICATDEA")            // CSEBCDICATDEA
// CODEPAGE_CSEBCDICCAFR             _S("CSEBCDICCAFR")             // CSEBCDICCAFR
// CODEPAGE_CSEBCDICDKNO             _S("CSEBCDICDKNO")             // CSEBCDICDKNO
// CODEPAGE_CSEBCDICDKNOA            _S("CSEBCDICDKNOA")            // CSEBCDICDKNOA
// CODEPAGE_CSEBCDICES               _S("CSEBCDICES")               // CSEBCDICES
// CODEPAGE_CSEBCDICESA              _S("CSEBCDICESA")              // CSEBCDICESA
// CODEPAGE_CSEBCDICESS              _S("CSEBCDICESS")              // CSEBCDICESS
// CODEPAGE_CSEBCDICFISE             _S("CSEBCDICFISE")             // CSEBCDICFISE
// CODEPAGE_CSEBCDICFISEA            _S("CSEBCDICFISEA")            // CSEBCDICFISEA
// CODEPAGE_CSEBCDICFR               _S("CSEBCDICFR")               // CSEBCDICFR
// CODEPAGE_CSEBCDICIT               _S("CSEBCDICIT")               // CSEBCDICIT
// CODEPAGE_CSEBCDICPT               _S("CSEBCDICPT")               // CSEBCDICPT
// CODEPAGE_CSEBCDICUK               _S("CSEBCDICUK")               // CSEBCDICUK
// CODEPAGE_CSEBCDICUS               _S("CSEBCDICUS")               // CSEBCDICUS
// CODEPAGE_CSEUCKR                  _S("CSEUCKR")                  // CSEUCKR
// CODEPAGE_CSEUCPKDFMTJAPANESE      _S("CSEUCPKDFMTJAPANESE")      // CSEUCPKDFMTJAPANESE
// CODEPAGE_CSGB2312                 _S("CSGB2312")                 // CSGB2312
// CODEPAGE_CSHPROMAN8               _S("CSHPROMAN8")               // CSHPROMAN8
// CODEPAGE_CSIBM037                 _S("CSIBM037")                 // CSIBM037
// CODEPAGE_CSIBM038                 _S("CSIBM038")                 // CSIBM038
// CODEPAGE_CSIBM273                 _S("CSIBM273")                 // CSIBM273
// CODEPAGE_CSIBM274                 _S("CSIBM274")                 // CSIBM274
// CODEPAGE_CSIBM275                 _S("CSIBM275")                 // CSIBM275
// CODEPAGE_CSIBM277                 _S("CSIBM277")                 // CSIBM277
// CODEPAGE_CSIBM278                 _S("CSIBM278")                 // CSIBM278
// CODEPAGE_CSIBM280                 _S("CSIBM280")                 // CSIBM280
// CODEPAGE_CSIBM281                 _S("CSIBM281")                 // CSIBM281
// CODEPAGE_CSIBM284                 _S("CSIBM284")                 // CSIBM284
// CODEPAGE_CSIBM285                 _S("CSIBM285")                 // CSIBM285
// CODEPAGE_CSIBM290                 _S("CSIBM290")                 // CSIBM290
// CODEPAGE_CSIBM297                 _S("CSIBM297")                 // CSIBM297
// CODEPAGE_CSIBM420                 _S("CSIBM420")                 // CSIBM420
// CODEPAGE_CSIBM423                 _S("CSIBM423")                 // CSIBM423
// CODEPAGE_CSIBM424                 _S("CSIBM424")                 // CSIBM424
// CODEPAGE_CSIBM500                 _S("CSIBM500")                 // CSIBM500
// CODEPAGE_CSIBM803                 _S("CSIBM803")                 // CSIBM803
// CODEPAGE_CSIBM851                 _S("CSIBM851")                 // CSIBM851
// CODEPAGE_CSIBM855                 _S("CSIBM855")                 // CSIBM855
// CODEPAGE_CSIBM856                 _S("CSIBM856")                 // CSIBM856
// CODEPAGE_CSIBM857                 _S("CSIBM857")                 // CSIBM857
// CODEPAGE_CSIBM860                 _S("CSIBM860")                 // CSIBM860
// CODEPAGE_CSIBM863                 _S("CSIBM863")                 // CSIBM863
// CODEPAGE_CSIBM864                 _S("CSIBM864")                 // CSIBM864
// CODEPAGE_CSIBM865                 _S("CSIBM865")                 // CSIBM865
// CODEPAGE_CSIBM866                 _S("CSIBM866")                 // CSIBM866
// CODEPAGE_CSIBM868                 _S("CSIBM868")                 // CSIBM868
// CODEPAGE_CSIBM869                 _S("CSIBM869")                 // CSIBM869
// CODEPAGE_CSIBM870                 _S("CSIBM870")                 // CSIBM870
// CODEPAGE_CSIBM871                 _S("CSIBM871")                 // CSIBM871
// CODEPAGE_CSIBM880                 _S("CSIBM880")                 // CSIBM880
// CODEPAGE_CSIBM891                 _S("CSIBM891")                 // CSIBM891
// CODEPAGE_CSIBM901                 _S("CSIBM901")                 // CSIBM901
// CODEPAGE_CSIBM902                 _S("CSIBM902")                 // CSIBM902
// CODEPAGE_CSIBM903                 _S("CSIBM903")                 // CSIBM903
// CODEPAGE_CSIBM904                 _S("CSIBM904")                 // CSIBM904
// CODEPAGE_CSIBM905                 _S("CSIBM905")                 // CSIBM905
// CODEPAGE_CSIBM918                 _S("CSIBM918")                 // CSIBM918
// CODEPAGE_CSIBM921                 _S("CSIBM921")                 // CSIBM921
// CODEPAGE_CSIBM922                 _S("CSIBM922")                 // CSIBM922
// CODEPAGE_CSIBM930                 _S("CSIBM930")                 // CSIBM930
// CODEPAGE_CSIBM932                 _S("CSIBM932")                 // CSIBM932
// CODEPAGE_CSIBM933                 _S("CSIBM933")                 // CSIBM933
// CODEPAGE_CSIBM935                 _S("CSIBM935")                 // CSIBM935
// CODEPAGE_CSIBM937                 _S("CSIBM937")                 // CSIBM937
// CODEPAGE_CSIBM939                 _S("CSIBM939")                 // CSIBM939
// CODEPAGE_CSIBM943                 _S("CSIBM943")                 // CSIBM943
// CODEPAGE_CSIBM1008                _S("CSIBM1008")                // CSIBM1008
// CODEPAGE_CSIBM1025                _S("CSIBM1025")                // CSIBM1025
// CODEPAGE_CSIBM1026                _S("CSIBM1026")                // CSIBM1026
// CODEPAGE_CSIBM1097                _S("CSIBM1097")                // CSIBM1097
// CODEPAGE_CSIBM1112                _S("CSIBM1112")                // CSIBM1112
// CODEPAGE_CSIBM1122                _S("CSIBM1122")                // CSIBM1122
// CODEPAGE_CSIBM1123                _S("CSIBM1123")                // CSIBM1123
// CODEPAGE_CSIBM1124                _S("CSIBM1124")                // CSIBM1124
// CODEPAGE_CSIBM1129                _S("CSIBM1129")                // CSIBM1129
// CODEPAGE_CSIBM1130                _S("CSIBM1130")                // CSIBM1130
// CODEPAGE_CSIBM1132                _S("CSIBM1132")                // CSIBM1132
// CODEPAGE_CSIBM1133                _S("CSIBM1133")                // CSIBM1133
// CODEPAGE_CSIBM1137                _S("CSIBM1137")                // CSIBM1137
// CODEPAGE_CSIBM1140                _S("CSIBM1140")                // CSIBM1140
// CODEPAGE_CSIBM1141                _S("CSIBM1141")                // CSIBM1141
// CODEPAGE_CSIBM1142                _S("CSIBM1142")                // CSIBM1142
// CODEPAGE_CSIBM1143                _S("CSIBM1143")                // CSIBM1143
// CODEPAGE_CSIBM1144                _S("CSIBM1144")                // CSIBM1144
// CODEPAGE_CSIBM1145                _S("CSIBM1145")                // CSIBM1145
// CODEPAGE_CSIBM1146                _S("CSIBM1146")                // CSIBM1146
// CODEPAGE_CSIBM1147                _S("CSIBM1147")                // CSIBM1147
// CODEPAGE_CSIBM1148                _S("CSIBM1148")                // CSIBM1148
// CODEPAGE_CSIBM1149                _S("CSIBM1149")                // CSIBM1149
// CODEPAGE_CSIBM1153                _S("CSIBM1153")                // CSIBM1153
// CODEPAGE_CSIBM1154                _S("CSIBM1154")                // CSIBM1154
// CODEPAGE_CSIBM1155                _S("CSIBM1155")                // CSIBM1155
// CODEPAGE_CSIBM1156                _S("CSIBM1156")                // CSIBM1156
// CODEPAGE_CSIBM1157                _S("CSIBM1157")                // CSIBM1157
// CODEPAGE_CSIBM1158                _S("CSIBM1158")                // CSIBM1158
// CODEPAGE_CSIBM1160                _S("CSIBM1160")                // CSIBM1160
// CODEPAGE_CSIBM1161                _S("CSIBM1161")                // CSIBM1161
// CODEPAGE_CSIBM1163                _S("CSIBM1163")                // CSIBM1163
// CODEPAGE_CSIBM1164                _S("CSIBM1164")                // CSIBM1164
// CODEPAGE_CSIBM1166                _S("CSIBM1166")                // CSIBM1166
// CODEPAGE_CSIBM1167                _S("CSIBM1167")                // CSIBM1167
// CODEPAGE_CSIBM1364                _S("CSIBM1364")                // CSIBM1364
// CODEPAGE_CSIBM1371                _S("CSIBM1371")                // CSIBM1371
// CODEPAGE_CSIBM1388                _S("CSIBM1388")                // CSIBM1388
// CODEPAGE_CSIBM1390                _S("CSIBM1390")                // CSIBM1390
// CODEPAGE_CSIBM1399                _S("CSIBM1399")                // CSIBM1399
// CODEPAGE_CSIBM4517                _S("CSIBM4517")                // CSIBM4517
// CODEPAGE_CSIBM4899                _S("CSIBM4899")                // CSIBM4899
// CODEPAGE_CSIBM4909                _S("CSIBM4909")                // CSIBM4909
// CODEPAGE_CSIBM4971                _S("CSIBM4971")                // CSIBM4971
// CODEPAGE_CSIBM5347                _S("CSIBM5347")                // CSIBM5347
// CODEPAGE_CSIBM9030                _S("CSIBM9030")                // CSIBM9030
// CODEPAGE_CSIBM9066                _S("CSIBM9066")                // CSIBM9066
// CODEPAGE_CSIBM9448                _S("CSIBM9448")                // CSIBM9448
// CODEPAGE_CSIBM12712               _S("CSIBM12712")               // CSIBM12712
// CODEPAGE_CSIBM16804               _S("CSIBM16804")               // CSIBM16804
// CODEPAGE_CSIBM11621162            _S("CSIBM11621162")            // CSIBM11621162
// CODEPAGE_CSISO4UNITEDKINGDOM      _S("CSISO4UNITEDKINGDOM")      // CSISO4UNITEDKINGDOM
// CODEPAGE_CSISO10SWEDISH           _S("CSISO10SWEDISH")           // CSISO10SWEDISH
// CODEPAGE_CSISO11SWEDISHFORNAMES   _S("CSISO11SWEDISHFORNAMES")   // CSISO11SWEDISHFORNAMES
// CODEPAGE_CSISO14JISC6220RO        _S("CSISO14JISC6220RO")        // CSISO14JISC6220RO
// CODEPAGE_CSISO15ITALIAN           _S("CSISO15ITALIAN")           // CSISO15ITALIAN
// CODEPAGE_CSISO16PORTUGESE         _S("CSISO16PORTUGESE")         // CSISO16PORTUGESE
// CODEPAGE_CSISO17SPANISH           _S("CSISO17SPANISH")           // CSISO17SPANISH
// CODEPAGE_CSISO18GREEK7OLD         _S("CSISO18GREEK7OLD")         // CSISO18GREEK7OLD
// CODEPAGE_CSISO19LATINGREEK        _S("CSISO19LATINGREEK")        // CSISO19LATINGREEK
// CODEPAGE_CSISO21GERMAN            _S("CSISO21GERMAN")            // CSISO21GERMAN
// CODEPAGE_CSISO25FRENCH            _S("CSISO25FRENCH")            // CSISO25FRENCH
// CODEPAGE_CSISO27LATINGREEK1       _S("CSISO27LATINGREEK1")       // CSISO27LATINGREEK1
// CODEPAGE_CSISO49INIS              _S("CSISO49INIS")              // CSISO49INIS
// CODEPAGE_CSISO50INIS8             _S("CSISO50INIS8")             // CSISO50INIS8
// CODEPAGE_CSISO51INISCYRILLIC      _S("CSISO51INISCYRILLIC")      // CSISO51INISCYRILLIC
// CODEPAGE_CSISO58GB1988            _S("CSISO58GB1988")            // CSISO58GB1988
// CODEPAGE_CSISO60DANISHNORWEGIAN   _S("CSISO60DANISHNORWEGIAN")   // CSISO60DANISHNORWEGIAN
// CODEPAGE_CSISO60NORWEGIAN1        _S("CSISO60NORWEGIAN1")        // CSISO60NORWEGIAN1
// CODEPAGE_CSISO61NORWEGIAN2        _S("CSISO61NORWEGIAN2")        // CSISO61NORWEGIAN2
// CODEPAGE_CSISO69FRENCH            _S("CSISO69FRENCH")            // CSISO69FRENCH
// CODEPAGE_CSISO84PORTUGUESE2       _S("CSISO84PORTUGUESE2")       // CSISO84PORTUGUESE2
// CODEPAGE_CSISO85SPANISH2          _S("CSISO85SPANISH2")          // CSISO85SPANISH2
// CODEPAGE_CSISO86HUNGARIAN         _S("CSISO86HUNGARIAN")         // CSISO86HUNGARIAN
// CODEPAGE_CSISO88GREEK7            _S("CSISO88GREEK7")            // CSISO88GREEK7
// CODEPAGE_CSISO89ASMO449           _S("CSISO89ASMO449")           // CSISO89ASMO449
// CODEPAGE_CSISO90                  _S("CSISO90")                  // CSISO90
// CODEPAGE_CSISO92JISC62991984B     _S("CSISO92JISC62991984B")     // CSISO92JISC62991984B
// CODEPAGE_CSISO99NAPLPS            _S("CSISO99NAPLPS")            // CSISO99NAPLPS
// CODEPAGE_CSISO103T618BIT          _S("CSISO103T618BIT")          // CSISO103T618BIT
// CODEPAGE_CSISO111ECMACYRILLIC     _S("CSISO111ECMACYRILLIC")     // CSISO111ECMACYRILLIC
// CODEPAGE_CSISO121CANADIAN1        _S("CSISO121CANADIAN1")        // CSISO121CANADIAN1
// CODEPAGE_CSISO122CANADIAN2        _S("CSISO122CANADIAN2")        // CSISO122CANADIAN2
// CODEPAGE_CSISO139CSN369103        _S("CSISO139CSN369103")        // CSISO139CSN369103
// CODEPAGE_CSISO141JUSIB1002        _S("CSISO141JUSIB1002")        // CSISO141JUSIB1002
// CODEPAGE_CSISO143IECP271          _S("CSISO143IECP271")          // CSISO143IECP271
// CODEPAGE_CSISO150                 _S("CSISO150")                 // CSISO150
// CODEPAGE_CSISO150GREEKCCITT       _S("CSISO150GREEKCCITT")       // CSISO150GREEKCCITT
// CODEPAGE_CSISO151CUBA             _S("CSISO151CUBA")             // CSISO151CUBA
// CODEPAGE_CSISO153GOST1976874      _S("CSISO153GOST1976874")      // CSISO153GOST1976874
// CODEPAGE_CSISO646DANISH           _S("CSISO646DANISH")           // CSISO646DANISH
// CODEPAGE_CSISO2022CN              _S("CSISO2022CN")              // CSISO2022CN
// CODEPAGE_CSISO2022JP              _S("CSISO2022JP")              // CSISO2022JP
// CODEPAGE_CSISO2022JP2             _S("CSISO2022JP2")             // CSISO2022JP2
// CODEPAGE_CSISO2022KR              _S("CSISO2022KR")              // CSISO2022KR
// CODEPAGE_CSISO2033                _S("CSISO2033")                // CSISO2033
// CODEPAGE_CSISO5427CYRILLIC        _S("CSISO5427CYRILLIC")        // CSISO5427CYRILLIC
// CODEPAGE_CSISO5427CYRILLIC1981    _S("CSISO5427CYRILLIC1981")    // CSISO5427CYRILLIC1981
// CODEPAGE_CSISO5428GREEK           _S("CSISO5428GREEK")           // CSISO5428GREEK
// CODEPAGE_CSISO10367BOX            _S("CSISO10367BOX")            // CSISO10367BOX
// CODEPAGE_CSISOLATIN1              _S("CSISOLATIN1")              // CSISOLATIN1
// CODEPAGE_CSISOLATIN2              _S("CSISOLATIN2")              // CSISOLATIN2
// CODEPAGE_CSISOLATIN3              _S("CSISOLATIN3")              // CSISOLATIN3
// CODEPAGE_CSISOLATIN4              _S("CSISOLATIN4")              // CSISOLATIN4
// CODEPAGE_CSISOLATIN5              _S("CSISOLATIN5")              // CSISOLATIN5
// CODEPAGE_CSISOLATIN6              _S("CSISOLATIN6")              // CSISOLATIN6
// CODEPAGE_CSISOLATINARABIC         _S("CSISOLATINARABIC")         // CSISOLATINARABIC
// CODEPAGE_CSISOLATINCYRILLIC       _S("CSISOLATINCYRILLIC")       // CSISOLATINCYRILLIC
// CODEPAGE_CSISOLATINGREEK          _S("CSISOLATINGREEK")          // CSISOLATINGREEK
// CODEPAGE_CSISOLATINHEBREW         _S("CSISOLATINHEBREW")         // CSISOLATINHEBREW
// CODEPAGE_CSKOI8R                  _S("CSKOI8R")                  // CSKOI8R
// CODEPAGE_CSKSC5636                _S("CSKSC5636")                // CSKSC5636
// CODEPAGE_CSMACINTOSH              _S("CSMACINTOSH")              // CSMACINTOSH
// CODEPAGE_CSNATSDANO               _S("CSNATSDANO")               // CSNATSDANO
// CODEPAGE_CSNATSSEFI               _S("CSNATSSEFI")               // CSNATSSEFI
// CODEPAGE_CSN_369103               _S("CSN_369103")               // CSN_369103
// CODEPAGE_CSPC8CODEPAGE437         _S("CSPC8CODEPAGE437")         // CSPC8CODEPAGE437
// CODEPAGE_CSPC775BALTIC            _S("CSPC775BALTIC")            // CSPC775BALTIC
// CODEPAGE_CSPC850MULTILINGUAL      _S("CSPC850MULTILINGUAL")      // CSPC850MULTILINGUAL
// CODEPAGE_CSPC862LATINHEBREW       _S("CSPC862LATINHEBREW")       // CSPC862LATINHEBREW
// CODEPAGE_CSPCP852                 _S("CSPCP852")                 // CSPCP852
// CODEPAGE_CSSHIFTJIS               _S("CSSHIFTJIS")               // CSSHIFTJIS
// CODEPAGE_CSUCS4                   _S("CSUCS4")                   // CSUCS4
// CODEPAGE_CSUNICODE                _S("CSUNICODE")                // CSUNICODE
// CODEPAGE_CSWINDOWS31J             _S("CSWINDOWS31J")             // CSWINDOWS31J
// CODEPAGE_CUBA                     _S("CUBA")                     // CUBA
// CODEPAGE_CWI_2                    _S("CWI-2")                    // CWI-2
// CODEPAGE_CWI                      _S("CWI")                      // CWI
// CODEPAGE_CYRILLIC                 _S("CYRILLIC")                 // CYRILLIC
// CODEPAGE_DE                       _S("DE")                       // DE
// CODEPAGE_DEC_MCS                  _S("DEC-MCS")                  // DEC-MCS
// CODEPAGE_DEC                      _S("DEC")                      // DEC
// CODEPAGE_DECMCS                   _S("DECMCS")                   // DECMCS
// CODEPAGE_DIN_66003                _S("DIN_66003")                // DIN_66003
// CODEPAGE_DK                       _S("DK")                       // DK
// CODEPAGE_DS2089                   _S("DS2089")                   // DS2089
// CODEPAGE_DS_2089                  _S("DS_2089")                  // DS_2089
// CODEPAGE_E13B                     _S("E13B")                     // E13B
// CODEPAGE_EBCDIC_AT_DE_A           _S("EBCDIC-AT-DE-A")           // EBCDIC-AT-DE-A
// CODEPAGE_EBCDIC_AT_DE             _S("EBCDIC-AT-DE")             // EBCDIC-AT-DE
// CODEPAGE_EBCDIC_BE                _S("EBCDIC-BE")                // EBCDIC-BE
// CODEPAGE_EBCDIC_BR                _S("EBCDIC-BR")                // EBCDIC-BR
// CODEPAGE_EBCDIC_CA_FR             _S("EBCDIC-CA-FR")             // EBCDIC-CA-FR
// CODEPAGE_EBCDIC_CP_AR1            _S("EBCDIC-CP-AR1")            // EBCDIC-CP-AR1
// CODEPAGE_EBCDIC_CP_AR2            _S("EBCDIC-CP-AR2")            // EBCDIC-CP-AR2
// CODEPAGE_EBCDIC_CP_BE             _S("EBCDIC-CP-BE")             // EBCDIC-CP-BE
// CODEPAGE_EBCDIC_CP_CA             _S("EBCDIC-CP-CA")             // EBCDIC-CP-CA
// CODEPAGE_EBCDIC_CP_CH             _S("EBCDIC-CP-CH")             // EBCDIC-CP-CH
// CODEPAGE_EBCDIC_CP_DK             _S("EBCDIC-CP-DK")             // EBCDIC-CP-DK
// CODEPAGE_EBCDIC_CP_ES             _S("EBCDIC-CP-ES")             // EBCDIC-CP-ES
// CODEPAGE_EBCDIC_CP_FI             _S("EBCDIC-CP-FI")             // EBCDIC-CP-FI
// CODEPAGE_EBCDIC_CP_FR             _S("EBCDIC-CP-FR")             // EBCDIC-CP-FR
// CODEPAGE_EBCDIC_CP_GB             _S("EBCDIC-CP-GB")             // EBCDIC-CP-GB
// CODEPAGE_EBCDIC_CP_GR             _S("EBCDIC-CP-GR")             // EBCDIC-CP-GR
// CODEPAGE_EBCDIC_CP_HE             _S("EBCDIC-CP-HE")             // EBCDIC-CP-HE
// CODEPAGE_EBCDIC_CP_IS             _S("EBCDIC-CP-IS")             // EBCDIC-CP-IS
// CODEPAGE_EBCDIC_CP_IT             _S("EBCDIC-CP-IT")             // EBCDIC-CP-IT
// CODEPAGE_EBCDIC_CP_NL             _S("EBCDIC-CP-NL")             // EBCDIC-CP-NL
// CODEPAGE_EBCDIC_CP_NO             _S("EBCDIC-CP-NO")             // EBCDIC-CP-NO
// CODEPAGE_EBCDIC_CP_ROECE          _S("EBCDIC-CP-ROECE")          // EBCDIC-CP-ROECE
// CODEPAGE_EBCDIC_CP_SE             _S("EBCDIC-CP-SE")             // EBCDIC-CP-SE
// CODEPAGE_EBCDIC_CP_TR             _S("EBCDIC-CP-TR")             // EBCDIC-CP-TR
// CODEPAGE_EBCDIC_CP_US             _S("EBCDIC-CP-US")             // EBCDIC-CP-US
// CODEPAGE_EBCDIC_CP_WT             _S("EBCDIC-CP-WT")             // EBCDIC-CP-WT
// CODEPAGE_EBCDIC_CP_YU             _S("EBCDIC-CP-YU")             // EBCDIC-CP-YU
// CODEPAGE_EBCDIC_CYRILLIC          _S("EBCDIC-CYRILLIC")          // EBCDIC-CYRILLIC
// CODEPAGE_EBCDIC_DK_NO_A           _S("EBCDIC-DK-NO-A")           // EBCDIC-DK-NO-A
// CODEPAGE_EBCDIC_DK_NO             _S("EBCDIC-DK-NO")             // EBCDIC-DK-NO
// CODEPAGE_EBCDIC_ES_A              _S("EBCDIC-ES-A")              // EBCDIC-ES-A
// CODEPAGE_EBCDIC_ES_S              _S("EBCDIC-ES-S")              // EBCDIC-ES-S
// CODEPAGE_EBCDIC_ES                _S("EBCDIC-ES")                // EBCDIC-ES
// CODEPAGE_EBCDIC_FI_SE_A           _S("EBCDIC-FI-SE-A")           // EBCDIC-FI-SE-A
// CODEPAGE_EBCDIC_FI_SE             _S("EBCDIC-FI-SE")             // EBCDIC-FI-SE
// CODEPAGE_EBCDIC_FR                _S("EBCDIC-FR")                // EBCDIC-FR
// CODEPAGE_EBCDIC_GREEK             _S("EBCDIC-GREEK")             // EBCDIC-GREEK
// CODEPAGE_EBCDIC_INT               _S("EBCDIC-INT")               // EBCDIC-INT
// CODEPAGE_EBCDIC_INT1              _S("EBCDIC-INT1")              // EBCDIC-INT1
// CODEPAGE_EBCDIC_IS_FRISS          _S("EBCDIC-IS-FRISS")          // EBCDIC-IS-FRISS
// CODEPAGE_EBCDIC_IT                _S("EBCDIC-IT")                // EBCDIC-IT
// CODEPAGE_EBCDIC_JP_E              _S("EBCDIC-JP-E")              // EBCDIC-JP-E
// CODEPAGE_EBCDIC_JP_KANA           _S("EBCDIC-JP-KANA")           // EBCDIC-JP-KANA
// CODEPAGE_EBCDIC_PT                _S("EBCDIC-PT")                // EBCDIC-PT
// CODEPAGE_EBCDIC_UK                _S("EBCDIC-UK")                // EBCDIC-UK
// CODEPAGE_EBCDIC_US                _S("EBCDIC-US")                // EBCDIC-US
// CODEPAGE_EBCDICATDE               _S("EBCDICATDE")               // EBCDICATDE
// CODEPAGE_EBCDICATDEA              _S("EBCDICATDEA")              // EBCDICATDEA
// CODEPAGE_EBCDICCAFR               _S("EBCDICCAFR")               // EBCDICCAFR
// CODEPAGE_EBCDICDKNO               _S("EBCDICDKNO")               // EBCDICDKNO
// CODEPAGE_EBCDICDKNOA              _S("EBCDICDKNOA")              // EBCDICDKNOA
// CODEPAGE_EBCDICES                 _S("EBCDICES")                 // EBCDICES
// CODEPAGE_EBCDICESA                _S("EBCDICESA")                // EBCDICESA
// CODEPAGE_EBCDICESS                _S("EBCDICESS")                // EBCDICESS
// CODEPAGE_EBCDICFISE               _S("EBCDICFISE")               // EBCDICFISE
// CODEPAGE_EBCDICFISEA              _S("EBCDICFISEA")              // EBCDICFISEA
// CODEPAGE_EBCDICFR                 _S("EBCDICFR")                 // EBCDICFR
// CODEPAGE_EBCDICISFRISS            _S("EBCDICISFRISS")            // EBCDICISFRISS
// CODEPAGE_EBCDICIT                 _S("EBCDICIT")                 // EBCDICIT
// CODEPAGE_EBCDICPT                 _S("EBCDICPT")                 // EBCDICPT
// CODEPAGE_EBCDICUK                 _S("EBCDICUK")                 // EBCDICUK
// CODEPAGE_EBCDICUS                 _S("EBCDICUS")                 // EBCDICUS
// CODEPAGE_ECMA_114                 _S("ECMA-114")                 // ECMA-114
// CODEPAGE_ECMA_118                 _S("ECMA-118")                 // ECMA-118
// CODEPAGE_ECMA_128                 _S("ECMA-128")                 // ECMA-128
// CODEPAGE_ECMA_CYRILLIC            _S("ECMA-CYRILLIC")            // ECMA-CYRILLIC
// CODEPAGE_ECMACYRILLIC             _S("ECMACYRILLIC")             // ECMACYRILLIC
// CODEPAGE_ELOT_928                 _S("ELOT_928")                 // ELOT_928
// CODEPAGE_ES                       _S("ES")                       // ES
// CODEPAGE_ES2                      _S("ES2")                      // ES2
#define \
   CODEPAGE_936_1                    _S("EUC-CN")                   // EUC-CN
// CODEPAGE_EUC_JISX0213             _S("EUC-JISX0213")             // EUC-JISX0213
// CODEPAGE_EUC_JP_MS                _S("EUC-JP-MS")                // EUC-JP-MS
#define \
   CODEPAGE_932_1                    _S("EUC-JP")                   // EUC-JP
#define \
   CODEPAGE_949_1                    _S("EUC-KR")                   // EUC-KR
#define \
   CODEPAGE_950_1                    _S("EUC-TW")                   // EUC-TW
// CODEPAGE_EUCCN                    _S("EUCCN")                    // EUCCN
// CODEPAGE_EUCJP_MS                 _S("EUCJP-MS")                 // EUCJP-MS
// CODEPAGE_EUCJP_OPEN               _S("EUCJP-OPEN")               // EUCJP-OPEN
// CODEPAGE_EUCJP_WIN                _S("EUCJP-WIN")                // EUCJP-WIN
// CODEPAGE_EUCJP                    _S("EUCJP")                    // EUCJP
// CODEPAGE_EUCKR                    _S("EUCKR")                    // EUCKR
// CODEPAGE_EUCTW                    _S("EUCTW")                    // EUCTW
// CODEPAGE_FI                       _S("FI")                       // FI
// CODEPAGE_FR                       _S("FR")                       // FR
// CODEPAGE_GB                       _S("GB")                       // GB
// CODEPAGE_GB2312                   _S("GB2312")                   // GB2312
// CODEPAGE_GB13000                  _S("GB13000")                  // GB13000
#define \
   CODEPAGE_18030                    _S("GB18030")                  // GB18030
// CODEPAGE_GBK                      _S("GBK")                      // GBK
// CODEPAGE_GB_1988_80               _S("GB_1988-80")               // GB_1988-80
// CODEPAGE_GB_198880                _S("GB_198880")                // GB_198880
// CODEPAGE_GEORGIAN_ACADEMY         _S("GEORGIAN-ACADEMY")         // GEORGIAN-ACADEMY
// CODEPAGE_GEORGIAN_PS              _S("GEORGIAN-PS")              // GEORGIAN-PS
// CODEPAGE_GOST_19768_74            _S("GOST_19768-74")            // GOST_19768-74
// CODEPAGE_GOST_19768               _S("GOST_19768")               // GOST_19768
// CODEPAGE_GOST_1976874             _S("GOST_1976874")             // GOST_1976874
// CODEPAGE_GREEK_CCITT              _S("GREEK-CCITT")              // GREEK-CCITT
// CODEPAGE_GREEK                    _S("GREEK")                    // GREEK
// CODEPAGE_GREEK7_OLD               _S("GREEK7-OLD")               // GREEK7-OLD
// CODEPAGE_GREEK7                   _S("GREEK7")                   // GREEK7
// CODEPAGE_GREEK7OLD                _S("GREEK7OLD")                // GREEK7OLD
// CODEPAGE_GREEK8                   _S("GREEK8")                   // GREEK8
// CODEPAGE_GREEKCCITT               _S("GREEKCCITT")               // GREEKCCITT
// CODEPAGE_HEBREW                   _S("HEBREW")                   // HEBREW
// CODEPAGE_HP_GREEK8                _S("HP-GREEK8")                // HP-GREEK8
// CODEPAGE_HP_ROMAN8                _S("HP-ROMAN8")                // HP-ROMAN8
// CODEPAGE_HP_ROMAN9                _S("HP-ROMAN9")                // HP-ROMAN9
// CODEPAGE_HP_THAI8                 _S("HP-THAI8")                 // HP-THAI8
// CODEPAGE_HP_TURKISH8              _S("HP-TURKISH8")              // HP-TURKISH8
// CODEPAGE_HPGREEK8                 _S("HPGREEK8")                 // HPGREEK8
// CODEPAGE_HPROMAN8                 _S("HPROMAN8")                 // HPROMAN8
// CODEPAGE_HPROMAN9                 _S("HPROMAN9")                 // HPROMAN9
// CODEPAGE_HPTHAI8                  _S("HPTHAI8")                  // HPTHAI8
// CODEPAGE_HPTURKISH8               _S("HPTURKISH8")               // HPTURKISH8
// CODEPAGE_HU                       _S("HU")                       // HU
// CODEPAGE_IBM_803                  _S("IBM-803")                  // IBM-803
// CODEPAGE_IBM_856                  _S("IBM-856")                  // IBM-856
// CODEPAGE_IBM_901                  _S("IBM-901")                  // IBM-901
// CODEPAGE_IBM_902                  _S("IBM-902")                  // IBM-902
// CODEPAGE_IBM_921                  _S("IBM-921")                  // IBM-921
// CODEPAGE_IBM_922                  _S("IBM-922")                  // IBM-922
// CODEPAGE_IBM_930                  _S("IBM-930")                  // IBM-930
// CODEPAGE_IBM_932                  _S("IBM-932")                  // IBM-932
// CODEPAGE_IBM_933                  _S("IBM-933")                  // IBM-933
// CODEPAGE_IBM_935                  _S("IBM-935")                  // IBM-935
// CODEPAGE_IBM_937                  _S("IBM-937")                  // IBM-937
// CODEPAGE_IBM_939                  _S("IBM-939")                  // IBM-939
// CODEPAGE_IBM_943                  _S("IBM-943")                  // IBM-943
// CODEPAGE_IBM_1008                 _S("IBM-1008")                 // IBM-1008
// CODEPAGE_IBM_1025                 _S("IBM-1025")                 // IBM-1025
// CODEPAGE_IBM_1046                 _S("IBM-1046")                 // IBM-1046
// CODEPAGE_IBM_1047                 _S("IBM-1047")                 // IBM-1047
// CODEPAGE_IBM_1097                 _S("IBM-1097")                 // IBM-1097
// CODEPAGE_IBM_1112                 _S("IBM-1112")                 // IBM-1112
// CODEPAGE_IBM_1122                 _S("IBM-1122")                 // IBM-1122
// CODEPAGE_IBM_1123                 _S("IBM-1123")                 // IBM-1123
// CODEPAGE_IBM_1124                 _S("IBM-1124")                 // IBM-1124
// CODEPAGE_IBM_1129                 _S("IBM-1129")                 // IBM-1129
// CODEPAGE_IBM_1130                 _S("IBM-1130")                 // IBM-1130
// CODEPAGE_IBM_1132                 _S("IBM-1132")                 // IBM-1132
// CODEPAGE_IBM_1133                 _S("IBM-1133")                 // IBM-1133
// CODEPAGE_IBM_1137                 _S("IBM-1137")                 // IBM-1137
// CODEPAGE_IBM_1140                 _S("IBM-1140")                 // IBM-1140
// CODEPAGE_IBM_1141                 _S("IBM-1141")                 // IBM-1141
// CODEPAGE_IBM_1142                 _S("IBM-1142")                 // IBM-1142
// CODEPAGE_IBM_1143                 _S("IBM-1143")                 // IBM-1143
// CODEPAGE_IBM_1144                 _S("IBM-1144")                 // IBM-1144
// CODEPAGE_IBM_1145                 _S("IBM-1145")                 // IBM-1145
// CODEPAGE_IBM_1146                 _S("IBM-1146")                 // IBM-1146
// CODEPAGE_IBM_1147                 _S("IBM-1147")                 // IBM-1147
// CODEPAGE_IBM_1148                 _S("IBM-1148")                 // IBM-1148
// CODEPAGE_IBM_1149                 _S("IBM-1149")                 // IBM-1149
// CODEPAGE_IBM_1153                 _S("IBM-1153")                 // IBM-1153
// CODEPAGE_IBM_1154                 _S("IBM-1154")                 // IBM-1154
// CODEPAGE_IBM_1155                 _S("IBM-1155")                 // IBM-1155
// CODEPAGE_IBM_1156                 _S("IBM-1156")                 // IBM-1156
// CODEPAGE_IBM_1157                 _S("IBM-1157")                 // IBM-1157
// CODEPAGE_IBM_1158                 _S("IBM-1158")                 // IBM-1158
// CODEPAGE_IBM_1160                 _S("IBM-1160")                 // IBM-1160
// CODEPAGE_IBM_1161                 _S("IBM-1161")                 // IBM-1161
// CODEPAGE_IBM_1162                 _S("IBM-1162")                 // IBM-1162
// CODEPAGE_IBM_1163                 _S("IBM-1163")                 // IBM-1163
// CODEPAGE_IBM_1164                 _S("IBM-1164")                 // IBM-1164
// CODEPAGE_IBM_1166                 _S("IBM-1166")                 // IBM-1166
// CODEPAGE_IBM_1167                 _S("IBM-1167")                 // IBM-1167
// CODEPAGE_IBM_1364                 _S("IBM-1364")                 // IBM-1364
// CODEPAGE_IBM_1371                 _S("IBM-1371")                 // IBM-1371
// CODEPAGE_IBM_1388                 _S("IBM-1388")                 // IBM-1388
// CODEPAGE_IBM_1390                 _S("IBM-1390")                 // IBM-1390
// CODEPAGE_IBM_1399                 _S("IBM-1399")                 // IBM-1399
// CODEPAGE_IBM_4517                 _S("IBM-4517")                 // IBM-4517
// CODEPAGE_IBM_4899                 _S("IBM-4899")                 // IBM-4899
// CODEPAGE_IBM_4909                 _S("IBM-4909")                 // IBM-4909
// CODEPAGE_IBM_4971                 _S("IBM-4971")                 // IBM-4971
// CODEPAGE_IBM_5347                 _S("IBM-5347")                 // IBM-5347
// CODEPAGE_IBM_9030                 _S("IBM-9030")                 // IBM-9030
// CODEPAGE_IBM_9066                 _S("IBM-9066")                 // IBM-9066
// CODEPAGE_IBM_9448                 _S("IBM-9448")                 // IBM-9448
// CODEPAGE_IBM_12712                _S("IBM-12712")                // IBM-12712
// CODEPAGE_IBM_16804                _S("IBM-16804")                // IBM-16804
// CODEPAGE_IBM037                   _S("IBM037")                   // IBM037
// CODEPAGE_IBM038                   _S("IBM038")                   // IBM038
// CODEPAGE_IBM256                   _S("IBM256")                   // IBM256
// CODEPAGE_IBM273                   _S("IBM273")                   // IBM273
// CODEPAGE_IBM274                   _S("IBM274")                   // IBM274
// CODEPAGE_IBM275                   _S("IBM275")                   // IBM275
#define \
   CODEPAGE_277                      _S("IBM277")                   // IBM277
// CODEPAGE_IBM278                   _S("IBM278")                   // IBM278
// CODEPAGE_IBM280                   _S("IBM280")                   // IBM280
// CODEPAGE_IBM281                   _S("IBM281")                   // IBM281
// CODEPAGE_IBM284                   _S("IBM284")                   // IBM284
// CODEPAGE_IBM285                   _S("IBM285")                   // IBM285
// CODEPAGE_IBM290                   _S("IBM290")                   // IBM290
// CODEPAGE_IBM297                   _S("IBM297")                   // IBM297
// CODEPAGE_IBM367                   _S("IBM367")                   // IBM367
// CODEPAGE_IBM420                   _S("IBM420")                   // IBM420
// CODEPAGE_IBM423                   _S("IBM423")                   // IBM423
// CODEPAGE_IBM424                   _S("IBM424")                   // IBM424
// CODEPAGE_IBM437                   _S("IBM437")                   // IBM437
// CODEPAGE_IBM500                   _S("IBM500")                   // IBM500
// CODEPAGE_IBM775                   _S("IBM775")                   // IBM775
// CODEPAGE_IBM803                   _S("IBM803")                   // IBM803
// CODEPAGE_IBM813                   _S("IBM813")                   // IBM813
// CODEPAGE_IBM819                   _S("IBM819")                   // IBM819
// CODEPAGE_IBM848                   _S("IBM848")                   // IBM848
// CODEPAGE_IBM850                   _S("IBM850")                   // IBM850
// CODEPAGE_IBM851                   _S("IBM851")                   // IBM851
// CODEPAGE_IBM852                   _S("IBM852")                   // IBM852
// CODEPAGE_IBM855                   _S("IBM855")                   // IBM855
// CODEPAGE_IBM856                   _S("IBM856")                   // IBM856
// CODEPAGE_IBM857                   _S("IBM857")                   // IBM857
// CODEPAGE_IBM860                   _S("IBM860")                   // IBM860
// CODEPAGE_IBM861                   _S("IBM861")                   // IBM861
// CODEPAGE_IBM862                   _S("IBM862")                   // IBM862
// CODEPAGE_IBM863                   _S("IBM863")                   // IBM863
// CODEPAGE_IBM864                   _S("IBM864")                   // IBM864
// CODEPAGE_IBM865                   _S("IBM865")                   // IBM865
// CODEPAGE_IBM866                   _S("IBM866")                   // IBM866
// CODEPAGE_IBM866NAV                _S("IBM866NAV")                // IBM866NAV
// CODEPAGE_IBM868                   _S("IBM868")                   // IBM868
// CODEPAGE_IBM869                   _S("IBM869")                   // IBM869
// CODEPAGE_IBM870                   _S("IBM870")                   // IBM870
// CODEPAGE_IBM871                   _S("IBM871")                   // IBM871
// CODEPAGE_IBM874                   _S("IBM874")                   // IBM874
// CODEPAGE_IBM875                   _S("IBM875")                   // IBM875
// CODEPAGE_IBM880                   _S("IBM880")                   // IBM880
// CODEPAGE_IBM891                   _S("IBM891")                   // IBM891
// CODEPAGE_IBM901                   _S("IBM901")                   // IBM901
// CODEPAGE_IBM902                   _S("IBM902")                   // IBM902
// CODEPAGE_IBM903                   _S("IBM903")                   // IBM903
// CODEPAGE_IBM904                   _S("IBM904")                   // IBM904
// CODEPAGE_IBM905                   _S("IBM905")                   // IBM905
// CODEPAGE_IBM912                   _S("IBM912")                   // IBM912
// CODEPAGE_IBM915                   _S("IBM915")                   // IBM915
// CODEPAGE_IBM916                   _S("IBM916")                   // IBM916
// CODEPAGE_IBM918                   _S("IBM918")                   // IBM918
// CODEPAGE_IBM920                   _S("IBM920")                   // IBM920
// CODEPAGE_IBM921                   _S("IBM921")                   // IBM921
// CODEPAGE_IBM922                   _S("IBM922")                   // IBM922
// CODEPAGE_IBM930                   _S("IBM930")                   // IBM930
// CODEPAGE_IBM932                   _S("IBM932")                   // IBM932
// CODEPAGE_IBM933                   _S("IBM933")                   // IBM933
// CODEPAGE_IBM935                   _S("IBM935")                   // IBM935
// CODEPAGE_IBM937                   _S("IBM937")                   // IBM937
// CODEPAGE_IBM939                   _S("IBM939")                   // IBM939
// CODEPAGE_IBM943                   _S("IBM943")                   // IBM943
// CODEPAGE_IBM1004                  _S("IBM1004")                  // IBM1004
// CODEPAGE_IBM1008                  _S("IBM1008")                  // IBM1008
// CODEPAGE_IBM1025                  _S("IBM1025")                  // IBM1025
// CODEPAGE_IBM1026                  _S("IBM1026")                  // IBM1026
// CODEPAGE_IBM1046                  _S("IBM1046")                  // IBM1046
// CODEPAGE_IBM1047                  _S("IBM1047")                  // IBM1047
// CODEPAGE_IBM1089                  _S("IBM1089")                  // IBM1089
// CODEPAGE_IBM1097                  _S("IBM1097")                  // IBM1097
// CODEPAGE_IBM1112                  _S("IBM1112")                  // IBM1112
// CODEPAGE_IBM1122                  _S("IBM1122")                  // IBM1122
// CODEPAGE_IBM1123                  _S("IBM1123")                  // IBM1123
// CODEPAGE_IBM1124                  _S("IBM1124")                  // IBM1124
// CODEPAGE_IBM1129                  _S("IBM1129")                  // IBM1129
// CODEPAGE_IBM1130                  _S("IBM1130")                  // IBM1130
// CODEPAGE_IBM1132                  _S("IBM1132")                  // IBM1132
// CODEPAGE_IBM1133                  _S("IBM1133")                  // IBM1133
// CODEPAGE_IBM1137                  _S("IBM1137")                  // IBM1137
// CODEPAGE_IBM1140                  _S("IBM1140")                  // IBM1140
// CODEPAGE_IBM1141                  _S("IBM1141")                  // IBM1141
// CODEPAGE_IBM1142                  _S("IBM1142")                  // IBM1142
// CODEPAGE_IBM1143                  _S("IBM1143")                  // IBM1143
// CODEPAGE_IBM1144                  _S("IBM1144")                  // IBM1144
// CODEPAGE_IBM1145                  _S("IBM1145")                  // IBM1145
// CODEPAGE_IBM1146                  _S("IBM1146")                  // IBM1146
// CODEPAGE_IBM1147                  _S("IBM1147")                  // IBM1147
// CODEPAGE_IBM1148                  _S("IBM1148")                  // IBM1148
// CODEPAGE_IBM1149                  _S("IBM1149")                  // IBM1149
// CODEPAGE_IBM1153                  _S("IBM1153")                  // IBM1153
// CODEPAGE_IBM1154                  _S("IBM1154")                  // IBM1154
// CODEPAGE_IBM1155                  _S("IBM1155")                  // IBM1155
// CODEPAGE_IBM1156                  _S("IBM1156")                  // IBM1156
// CODEPAGE_IBM1157                  _S("IBM1157")                  // IBM1157
// CODEPAGE_IBM1158                  _S("IBM1158")                  // IBM1158
// CODEPAGE_IBM1160                  _S("IBM1160")                  // IBM1160
// CODEPAGE_IBM1161                  _S("IBM1161")                  // IBM1161
// CODEPAGE_IBM1162                  _S("IBM1162")                  // IBM1162
// CODEPAGE_IBM1163                  _S("IBM1163")                  // IBM1163
// CODEPAGE_IBM1164                  _S("IBM1164")                  // IBM1164
// CODEPAGE_IBM1166                  _S("IBM1166")                  // IBM1166
// CODEPAGE_IBM1167                  _S("IBM1167")                  // IBM1167
// CODEPAGE_IBM1364                  _S("IBM1364")                  // IBM1364
// CODEPAGE_IBM1371                  _S("IBM1371")                  // IBM1371
// CODEPAGE_IBM1388                  _S("IBM1388")                  // IBM1388
// CODEPAGE_IBM1390                  _S("IBM1390")                  // IBM1390
// CODEPAGE_IBM1399                  _S("IBM1399")                  // IBM1399
// CODEPAGE_IBM4517                  _S("IBM4517")                  // IBM4517
// CODEPAGE_IBM4899                  _S("IBM4899")                  // IBM4899
// CODEPAGE_IBM4909                  _S("IBM4909")                  // IBM4909
// CODEPAGE_IBM4971                  _S("IBM4971")                  // IBM4971
// CODEPAGE_IBM5347                  _S("IBM5347")                  // IBM5347
// CODEPAGE_IBM9030                  _S("IBM9030")                  // IBM9030
// CODEPAGE_IBM9066                  _S("IBM9066")                  // IBM9066
// CODEPAGE_IBM9448                  _S("IBM9448")                  // IBM9448
// CODEPAGE_IBM12712                 _S("IBM12712")                 // IBM12712
// CODEPAGE_IBM16804                 _S("IBM16804")                 // IBM16804
// CODEPAGE_IEC_P27_1                _S("IEC_P27-1")                // IEC_P27-1
// CODEPAGE_IEC_P271                 _S("IEC_P271")                 // IEC_P271
// CODEPAGE_INIS_8                   _S("INIS-8")                   // INIS-8
// CODEPAGE_INIS_CYRILLIC            _S("INIS-CYRILLIC")            // INIS-CYRILLIC
// CODEPAGE_INIS                     _S("INIS")                     // INIS
// CODEPAGE_INIS8                    _S("INIS8")                    // INIS8
// CODEPAGE_INISCYRILLIC             _S("INISCYRILLIC")             // INISCYRILLIC
// CODEPAGE_ISIRI_3342               _S("ISIRI-3342")               // ISIRI-3342
// CODEPAGE_ISIRI3342                _S("ISIRI3342")                // ISIRI3342
#define \
   CODEPAGE_2022_9                   _S("ISO-2022-CN-EXT")          // ISO-2022-CN-EXT
#define \
   CODEPAGE_2022_7                   _S("ISO-2022-CN")              // ISO-2022-CN
#define \
   CODEPAGE_2022_1                   _S("ISO-2022-JP-2")            // ISO-2022-JP-2
#define \
   CODEPAGE_2022_2                   _S("ISO-2022-JP-3")            // ISO-2022-JP-3
#define \
   CODEPAGE_2022_0                   _S("ISO-2022-JP")              // ISO-2022-JP
#define \
   CODEPAGE_2022_5                   _S("ISO-2022-KR")              // ISO-2022-KR
// CODEPAGE_ISO_8859_1               _S("ISO-8859-1")               // ISO-8859-1
// CODEPAGE_ISO_8859_2               _S("ISO-8859-2")               // ISO-8859-2
// CODEPAGE_ISO_8859_3               _S("ISO-8859-3")               // ISO-8859-3
// CODEPAGE_ISO_8859_4               _S("ISO-8859-4")               // ISO-8859-4
// CODEPAGE_ISO_8859_5               _S("ISO-8859-5")               // ISO-8859-5
// CODEPAGE_ISO_8859_6               _S("ISO-8859-6")               // ISO-8859-6
// CODEPAGE_ISO_8859_7               _S("ISO-8859-7")               // ISO-8859-7
// CODEPAGE_ISO_8859_8               _S("ISO-8859-8")               // ISO-8859-8
// CODEPAGE_ISO_8859_9               _S("ISO-8859-9")               // ISO-8859-9
// CODEPAGE_ISO_8859_9E              _S("ISO-8859-9E")              // ISO-8859-9E
// CODEPAGE_ISO_8859_10              _S("ISO-8859-10")              // ISO-8859-10
// CODEPAGE_ISO_8859_11              _S("ISO-8859-11")              // ISO-8859-11
#define \
   CODEPAGE_8859_13                  _S("ISO-8859-13")              // ISO-8859-13
// CODEPAGE_ISO_8859_14              _S("ISO-8859-14")              // ISO-8859-14
#define \
   CODEPAGE_8859_15                  _S("ISO-8859-15")              // ISO-8859-15
// CODEPAGE_ISO_8859_16              _S("ISO-8859-16")              // ISO-8859-16
// CODEPAGE_ISO_10646                _S("ISO-10646")                // ISO-10646
// CODEPAGE_ISO_10646_UCS2           _S("ISO-10646/UCS2")           // ISO-10646/UCS2
// CODEPAGE_ISO_10646_UCS4           _S("ISO-10646/UCS4")           // ISO-10646/UCS4
// CODEPAGE_ISO_10646_UTF_8          _S("ISO-10646/UTF-8")          // ISO-10646/UTF-8
// CODEPAGE_ISO_10646_UTF8           _S("ISO-10646/UTF8")           // ISO-10646/UTF8
// CODEPAGE_ISO_CELTIC               _S("ISO-CELTIC")               // ISO-CELTIC
// CODEPAGE_ISO_IR_4                 _S("ISO-IR-4")                 // ISO-IR-4
// CODEPAGE_ISO_IR_6                 _S("ISO-IR-6")                 // ISO-IR-6
// CODEPAGE_ISO_IR_8_1               _S("ISO-IR-8-1")               // ISO-IR-8-1
// CODEPAGE_ISO_IR_9_1               _S("ISO-IR-9-1")               // ISO-IR-9-1
// CODEPAGE_ISO_IR_10                _S("ISO-IR-10")                // ISO-IR-10
// CODEPAGE_ISO_IR_11                _S("ISO-IR-11")                // ISO-IR-11
// CODEPAGE_ISO_IR_14                _S("ISO-IR-14")                // ISO-IR-14
// CODEPAGE_ISO_IR_15                _S("ISO-IR-15")                // ISO-IR-15
// CODEPAGE_ISO_IR_16                _S("ISO-IR-16")                // ISO-IR-16
// CODEPAGE_ISO_IR_17                _S("ISO-IR-17")                // ISO-IR-17
// CODEPAGE_ISO_IR_18                _S("ISO-IR-18")                // ISO-IR-18
// CODEPAGE_ISO_IR_19                _S("ISO-IR-19")                // ISO-IR-19
// CODEPAGE_ISO_IR_21                _S("ISO-IR-21")                // ISO-IR-21
// CODEPAGE_ISO_IR_25                _S("ISO-IR-25")                // ISO-IR-25
// CODEPAGE_ISO_IR_27                _S("ISO-IR-27")                // ISO-IR-27
// CODEPAGE_ISO_IR_37                _S("ISO-IR-37")                // ISO-IR-37
// CODEPAGE_ISO_IR_49                _S("ISO-IR-49")                // ISO-IR-49
// CODEPAGE_ISO_IR_50                _S("ISO-IR-50")                // ISO-IR-50
// CODEPAGE_ISO_IR_51                _S("ISO-IR-51")                // ISO-IR-51
// CODEPAGE_ISO_IR_54                _S("ISO-IR-54")                // ISO-IR-54
// CODEPAGE_ISO_IR_55                _S("ISO-IR-55")                // ISO-IR-55
// CODEPAGE_ISO_IR_57                _S("ISO-IR-57")                // ISO-IR-57
// CODEPAGE_ISO_IR_60                _S("ISO-IR-60")                // ISO-IR-60
// CODEPAGE_ISO_IR_61                _S("ISO-IR-61")                // ISO-IR-61
// CODEPAGE_ISO_IR_69                _S("ISO-IR-69")                // ISO-IR-69
// CODEPAGE_ISO_IR_84                _S("ISO-IR-84")                // ISO-IR-84
// CODEPAGE_ISO_IR_85                _S("ISO-IR-85")                // ISO-IR-85
// CODEPAGE_ISO_IR_86                _S("ISO-IR-86")                // ISO-IR-86
// CODEPAGE_ISO_IR_88                _S("ISO-IR-88")                // ISO-IR-88
// CODEPAGE_ISO_IR_89                _S("ISO-IR-89")                // ISO-IR-89
// CODEPAGE_ISO_IR_90                _S("ISO-IR-90")                // ISO-IR-90
// CODEPAGE_ISO_IR_92                _S("ISO-IR-92")                // ISO-IR-92
// CODEPAGE_ISO_IR_98                _S("ISO-IR-98")                // ISO-IR-98
// CODEPAGE_ISO_IR_99                _S("ISO-IR-99")                // ISO-IR-99
// CODEPAGE_ISO_IR_100               _S("ISO-IR-100")               // ISO-IR-100
// CODEPAGE_ISO_IR_101               _S("ISO-IR-101")               // ISO-IR-101
// CODEPAGE_ISO_IR_103               _S("ISO-IR-103")               // ISO-IR-103
// CODEPAGE_ISO_IR_109               _S("ISO-IR-109")               // ISO-IR-109
// CODEPAGE_ISO_IR_110               _S("ISO-IR-110")               // ISO-IR-110
// CODEPAGE_ISO_IR_111               _S("ISO-IR-111")               // ISO-IR-111
// CODEPAGE_ISO_IR_121               _S("ISO-IR-121")               // ISO-IR-121
// CODEPAGE_ISO_IR_122               _S("ISO-IR-122")               // ISO-IR-122
// CODEPAGE_ISO_IR_126               _S("ISO-IR-126")               // ISO-IR-126
// CODEPAGE_ISO_IR_127               _S("ISO-IR-127")               // ISO-IR-127
// CODEPAGE_ISO_IR_138               _S("ISO-IR-138")               // ISO-IR-138
// CODEPAGE_ISO_IR_139               _S("ISO-IR-139")               // ISO-IR-139
// CODEPAGE_ISO_IR_141               _S("ISO-IR-141")               // ISO-IR-141
// CODEPAGE_ISO_IR_143               _S("ISO-IR-143")               // ISO-IR-143
// CODEPAGE_ISO_IR_144               _S("ISO-IR-144")               // ISO-IR-144
// CODEPAGE_ISO_IR_148               _S("ISO-IR-148")               // ISO-IR-148
// CODEPAGE_ISO_IR_150               _S("ISO-IR-150")               // ISO-IR-150
// CODEPAGE_ISO_IR_151               _S("ISO-IR-151")               // ISO-IR-151
// CODEPAGE_ISO_IR_153               _S("ISO-IR-153")               // ISO-IR-153
// CODEPAGE_ISO_IR_155               _S("ISO-IR-155")               // ISO-IR-155
// CODEPAGE_ISO_IR_156               _S("ISO-IR-156")               // ISO-IR-156
// CODEPAGE_ISO_IR_157               _S("ISO-IR-157")               // ISO-IR-157
// CODEPAGE_ISO_IR_166               _S("ISO-IR-166")               // ISO-IR-166
// CODEPAGE_ISO_IR_179               _S("ISO-IR-179")               // ISO-IR-179
// CODEPAGE_ISO_IR_193               _S("ISO-IR-193")               // ISO-IR-193
// CODEPAGE_ISO_IR_197               _S("ISO-IR-197")               // ISO-IR-197
// CODEPAGE_ISO_IR_199               _S("ISO-IR-199")               // ISO-IR-199
// CODEPAGE_ISO_IR_203               _S("ISO-IR-203")               // ISO-IR-203
// CODEPAGE_ISO_IR_209               _S("ISO-IR-209")               // ISO-IR-209
// CODEPAGE_ISO_IR_226               _S("ISO-IR-226")               // ISO-IR-226
// CODEPAGE_ISO_TR_11548_1           _S("ISO/TR_11548-1")           // ISO/TR_11548-1
// CODEPAGE_ISO646_CA                _S("ISO646-CA")                // ISO646-CA
// CODEPAGE_ISO646_CA2               _S("ISO646-CA2")               // ISO646-CA2
// CODEPAGE_ISO646_CN                _S("ISO646-CN")                // ISO646-CN
// CODEPAGE_ISO646_CU                _S("ISO646-CU")                // ISO646-CU
// CODEPAGE_ISO646_DE                _S("ISO646-DE")                // ISO646-DE
// CODEPAGE_ISO646_DK                _S("ISO646-DK")                // ISO646-DK
// CODEPAGE_ISO646_ES                _S("ISO646-ES")                // ISO646-ES
// CODEPAGE_ISO646_ES2               _S("ISO646-ES2")               // ISO646-ES2
// CODEPAGE_ISO646_FI                _S("ISO646-FI")                // ISO646-FI
// CODEPAGE_ISO646_FR                _S("ISO646-FR")                // ISO646-FR
// CODEPAGE_ISO646_FR1               _S("ISO646-FR1")               // ISO646-FR1
// CODEPAGE_ISO646_GB                _S("ISO646-GB")                // ISO646-GB
// CODEPAGE_ISO646_HU                _S("ISO646-HU")                // ISO646-HU
// CODEPAGE_ISO646_IT                _S("ISO646-IT")                // ISO646-IT
// CODEPAGE_ISO646_JP_OCR_B          _S("ISO646-JP-OCR-B")          // ISO646-JP-OCR-B
// CODEPAGE_ISO646_JP                _S("ISO646-JP")                // ISO646-JP
// CODEPAGE_ISO646_KR                _S("ISO646-KR")                // ISO646-KR
// CODEPAGE_ISO646_NO                _S("ISO646-NO")                // ISO646-NO
// CODEPAGE_ISO646_NO2               _S("ISO646-NO2")               // ISO646-NO2
// CODEPAGE_ISO646_PT                _S("ISO646-PT")                // ISO646-PT
// CODEPAGE_ISO646_PT2               _S("ISO646-PT2")               // ISO646-PT2
// CODEPAGE_ISO646_SE                _S("ISO646-SE")                // ISO646-SE
// CODEPAGE_ISO646_SE2               _S("ISO646-SE2")               // ISO646-SE2
// CODEPAGE_ISO646_US                _S("ISO646-US")                // ISO646-US
// CODEPAGE_ISO646_YU                _S("ISO646-YU")                // ISO646-YU
// CODEPAGE_ISO2022CN                _S("ISO2022CN")                // ISO2022CN
// CODEPAGE_ISO2022CNEXT             _S("ISO2022CNEXT")             // ISO2022CNEXT
// CODEPAGE_ISO2022JP                _S("ISO2022JP")                // ISO2022JP
// CODEPAGE_ISO2022JP2               _S("ISO2022JP2")               // ISO2022JP2
// CODEPAGE_ISO2022KR                _S("ISO2022KR")                // ISO2022KR
// CODEPAGE_ISO6937                  _S("ISO6937")                  // ISO6937
// CODEPAGE_ISO8859_1                _S("ISO8859-1")                // ISO8859-1
// CODEPAGE_ISO8859_2                _S("ISO8859-2")                // ISO8859-2
// CODEPAGE_ISO8859_3                _S("ISO8859-3")                // ISO8859-3
// CODEPAGE_ISO8859_4                _S("ISO8859-4")                // ISO8859-4
// CODEPAGE_ISO8859_5                _S("ISO8859-5")                // ISO8859-5
// CODEPAGE_ISO8859_6                _S("ISO8859-6")                // ISO8859-6
// CODEPAGE_ISO8859_7                _S("ISO8859-7")                // ISO8859-7
// CODEPAGE_ISO8859_8                _S("ISO8859-8")                // ISO8859-8
// CODEPAGE_ISO8859_9                _S("ISO8859-9")                // ISO8859-9
// CODEPAGE_ISO8859_9E               _S("ISO8859-9E")               // ISO8859-9E
// CODEPAGE_ISO8859_10               _S("ISO8859-10")               // ISO8859-10
// CODEPAGE_ISO8859_11               _S("ISO8859-11")               // ISO8859-11
// CODEPAGE_ISO8859_13               _S("ISO8859-13")               // ISO8859-13
// CODEPAGE_ISO8859_14               _S("ISO8859-14")               // ISO8859-14
// CODEPAGE_ISO8859_15               _S("ISO8859-15")               // ISO8859-15
// CODEPAGE_ISO8859_16               _S("ISO8859-16")               // ISO8859-16
// CODEPAGE_ISO11548_1               _S("ISO11548-1")               // ISO11548-1
// CODEPAGE_ISO88591                 _S("ISO88591")                 // ISO88591
// CODEPAGE_ISO88592                 _S("ISO88592")                 // ISO88592
// CODEPAGE_ISO88593                 _S("ISO88593")                 // ISO88593
// CODEPAGE_ISO88594                 _S("ISO88594")                 // ISO88594
// CODEPAGE_ISO88595                 _S("ISO88595")                 // ISO88595
// CODEPAGE_ISO88596                 _S("ISO88596")                 // ISO88596
// CODEPAGE_ISO88597                 _S("ISO88597")                 // ISO88597
// CODEPAGE_ISO88598                 _S("ISO88598")                 // ISO88598
// CODEPAGE_ISO88599                 _S("ISO88599")                 // ISO88599
// CODEPAGE_ISO88599E                _S("ISO88599E")                // ISO88599E
// CODEPAGE_ISO885910                _S("ISO885910")                // ISO885910
// CODEPAGE_ISO885911                _S("ISO885911")                // ISO885911
// CODEPAGE_ISO885913                _S("ISO885913")                // ISO885913
// CODEPAGE_ISO885914                _S("ISO885914")                // ISO885914
// CODEPAGE_ISO885915                _S("ISO885915")                // ISO885915
// CODEPAGE_ISO885916                _S("ISO885916")                // ISO885916
// CODEPAGE_ISO_646_IRV_1991         _S("ISO_646.IRV:1991")         // ISO_646.IRV:1991
// CODEPAGE_ISO_2033_1983            _S("ISO_2033-1983")            // ISO_2033-1983
// CODEPAGE_ISO_2033                 _S("ISO_2033")                 // ISO_2033
// CODEPAGE_ISO_5427_EXT             _S("ISO_5427-EXT")             // ISO_5427-EXT
// CODEPAGE_ISO_5427                 _S("ISO_5427")                 // ISO_5427
// CODEPAGE_ISO_5427_1981            _S("ISO_5427:1981")            // ISO_5427:1981
// CODEPAGE_ISO_5427EXT              _S("ISO_5427EXT")              // ISO_5427EXT
// CODEPAGE_ISO_5428                 _S("ISO_5428")                 // ISO_5428
// CODEPAGE_ISO_5428_1980            _S("ISO_5428:1980")            // ISO_5428:1980
// CODEPAGE_ISO_6937_2               _S("ISO_6937-2")               // ISO_6937-2
// CODEPAGE_ISO_6937_2_1983          _S("ISO_6937-2:1983")          // ISO_6937-2:1983
// CODEPAGE_ISO_6937                 _S("ISO_6937")                 // ISO_6937
// CODEPAGE_ISO_6937_1992            _S("ISO_6937:1992")            // ISO_6937:1992
// CODEPAGE_ISO_8859_1               _S("ISO_8859-1")               // ISO_8859-1
// CODEPAGE_ISO_8859_1_1987          _S("ISO_8859-1:1987")          // ISO_8859-1:1987
// CODEPAGE_ISO_8859_2               _S("ISO_8859-2")               // ISO_8859-2
// CODEPAGE_ISO_8859_2_1987          _S("ISO_8859-2:1987")          // ISO_8859-2:1987
// CODEPAGE_ISO_8859_3               _S("ISO_8859-3")               // ISO_8859-3
// CODEPAGE_ISO_8859_3_1988          _S("ISO_8859-3:1988")          // ISO_8859-3:1988
// CODEPAGE_ISO_8859_4               _S("ISO_8859-4")               // ISO_8859-4
// CODEPAGE_ISO_8859_4_1988          _S("ISO_8859-4:1988")          // ISO_8859-4:1988
// CODEPAGE_ISO_8859_5               _S("ISO_8859-5")               // ISO_8859-5
// CODEPAGE_ISO_8859_5_1988          _S("ISO_8859-5:1988")          // ISO_8859-5:1988
// CODEPAGE_ISO_8859_6               _S("ISO_8859-6")               // ISO_8859-6
// CODEPAGE_ISO_8859_6_1987          _S("ISO_8859-6:1987")          // ISO_8859-6:1987
// CODEPAGE_ISO_8859_7               _S("ISO_8859-7")               // ISO_8859-7
// CODEPAGE_ISO_8859_7_1987          _S("ISO_8859-7:1987")          // ISO_8859-7:1987
// CODEPAGE_ISO_8859_7_2003          _S("ISO_8859-7:2003")          // ISO_8859-7:2003
// CODEPAGE_ISO_8859_8               _S("ISO_8859-8")               // ISO_8859-8
// CODEPAGE_ISO_8859_8_1988          _S("ISO_8859-8:1988")          // ISO_8859-8:1988
// CODEPAGE_ISO_8859_9               _S("ISO_8859-9")               // ISO_8859-9
// CODEPAGE_ISO_8859_9_1989          _S("ISO_8859-9:1989")          // ISO_8859-9:1989
// CODEPAGE_ISO_8859_9E              _S("ISO_8859-9E")              // ISO_8859-9E
// CODEPAGE_ISO_8859_10              _S("ISO_8859-10")              // ISO_8859-10
// CODEPAGE_ISO_8859_10_1992         _S("ISO_8859-10:1992")         // ISO_8859-10:1992
// CODEPAGE_ISO_8859_14              _S("ISO_8859-14")              // ISO_8859-14
// CODEPAGE_ISO_8859_14_1998         _S("ISO_8859-14:1998")         // ISO_8859-14:1998
// CODEPAGE_ISO_8859_15              _S("ISO_8859-15")              // ISO_8859-15
// CODEPAGE_ISO_8859_15_1998         _S("ISO_8859-15:1998")         // ISO_8859-15:1998
// CODEPAGE_ISO_8859_16              _S("ISO_8859-16")              // ISO_8859-16
// CODEPAGE_ISO_8859_16_2001         _S("ISO_8859-16:2001")         // ISO_8859-16:2001
// CODEPAGE_ISO_9036                 _S("ISO_9036")                 // ISO_9036
// CODEPAGE_ISO_10367_BOX            _S("ISO_10367-BOX")            // ISO_10367-BOX
// CODEPAGE_ISO_10367BOX             _S("ISO_10367BOX")             // ISO_10367BOX
// CODEPAGE_ISO_11548_1              _S("ISO_11548-1")              // ISO_11548-1
// CODEPAGE_ISO_69372                _S("ISO_69372")                // ISO_69372
// CODEPAGE_IT                       _S("IT")                       // IT
// CODEPAGE_JIS_C6220_1969_RO        _S("JIS_C6220-1969-RO")        // JIS_C6220-1969-RO
// CODEPAGE_JIS_C6229_1984_B         _S("JIS_C6229-1984-B")         // JIS_C6229-1984-B
// CODEPAGE_JIS_C62201969RO          _S("JIS_C62201969RO")          // JIS_C62201969RO
// CODEPAGE_JIS_C62291984B           _S("JIS_C62291984B")           // JIS_C62291984B
// CODEPAGE_JOHAB                    _S("JOHAB")                    // JOHAB
// CODEPAGE_JP_OCR_B                 _S("JP-OCR-B")                 // JP-OCR-B
// CODEPAGE_JP                       _S("JP")                       // JP
// CODEPAGE_JS                       _S("JS")                       // JS
// CODEPAGE_JUS_I_B1_002             _S("JUS_I.B1.002")             // JUS_I.B1.002
// CODEPAGE_KOI_7                    _S("KOI-7")                    // KOI-7
// CODEPAGE_KOI_8                    _S("KOI-8")                    // KOI-8
#define \
   CODEPAGE_866_1                    _S("KOI8-R")                   // KOI8-R
// CODEPAGE_KOI8_RU                  _S("KOI8-RU")                  // KOI8-RU
// CODEPAGE_KOI8_T                   _S("KOI8-T")                   // KOI8-T
#define \
   CODEPAGE_866_2                    _S("KOI8-U")                   // KOI8-U
// CODEPAGE_KOI8                     _S("KOI8")                     // KOI8
// CODEPAGE_KOI8R                    _S("KOI8R")                    // KOI8R
// CODEPAGE_KOI8U                    _S("KOI8U")                    // KOI8U
// CODEPAGE_KSC5636                  _S("KSC5636")                  // KSC5636
// CODEPAGE_L1                       _S("L1")                       // L1
// CODEPAGE_L2                       _S("L2")                       // L2
// CODEPAGE_L3                       _S("L3")                       // L3
// CODEPAGE_L4                       _S("L4")                       // L4
// CODEPAGE_L5                       _S("L5")                       // L5
// CODEPAGE_L6                       _S("L6")                       // L6
// CODEPAGE_L7                       _S("L7")                       // L7
// CODEPAGE_L8                       _S("L8")                       // L8
// CODEPAGE_L10                      _S("L10")                      // L10
// CODEPAGE_LATIN_9                  _S("LATIN-9")                  // LATIN-9
// CODEPAGE_LATIN_GREEK_1            _S("LATIN-GREEK-1")            // LATIN-GREEK-1
// CODEPAGE_LATIN_GREEK              _S("LATIN-GREEK")              // LATIN-GREEK
// CODEPAGE_LATIN1                   _S("LATIN1")                   // LATIN1
// CODEPAGE_LATIN2                   _S("LATIN2")                   // LATIN2
// CODEPAGE_LATIN3                   _S("LATIN3")                   // LATIN3
// CODEPAGE_LATIN4                   _S("LATIN4")                   // LATIN4
// CODEPAGE_LATIN5                   _S("LATIN5")                   // LATIN5
// CODEPAGE_LATIN6                   _S("LATIN6")                   // LATIN6
// CODEPAGE_LATIN7                   _S("LATIN7")                   // LATIN7
// CODEPAGE_LATIN8                   _S("LATIN8")                   // LATIN8
// CODEPAGE_LATIN9                   _S("LATIN9")                   // LATIN9
// CODEPAGE_LATIN10                  _S("LATIN10")                  // LATIN10
// CODEPAGE_LATINGREEK               _S("LATINGREEK")               // LATINGREEK
// CODEPAGE_LATINGREEK1              _S("LATINGREEK1")              // LATINGREEK1
#define \
   CODEPAGE_10029                    _S("MAC-CENTRALEUROPE")        // MAC-CENTRALEUROPE
#define \
   CODEPAGE_10007                    _S("MAC-CYRILLIC")             // MAC-CYRILLIC
// CODEPAGE_MAC_IS                   _S("MAC-IS")                   // MAC-IS
// CODEPAGE_MAC_SAMI                 _S("MAC-SAMI")                 // MAC-SAMI
// CODEPAGE_MAC_UK                   _S("MAC-UK")                   // MAC-UK
// CODEPAGE_MAC                      _S("MAC")                      // MAC
// CODEPAGE_MACCYRILLIC              _S("MACCYRILLIC")              // MACCYRILLIC
#define \
   CODEPAGE_10000                    _S("MACINTOSH")                // MACINTOSH
// CODEPAGE_MACIS                    _S("MACIS")                    // MACIS
// CODEPAGE_MACUK                    _S("MACUK")                    // MACUK
#define \
   CODEPAGE_10017                    _S("MACUKRAINIAN")             // MACUKRAINIAN
// CODEPAGE_MIK                      _S("MIK")                      // MIK
// CODEPAGE_MS_ANSI                  _S("MS-ANSI")                  // MS-ANSI
// CODEPAGE_MS_ARAB                  _S("MS-ARAB")                  // MS-ARAB
// CODEPAGE_MS_CYRL                  _S("MS-CYRL")                  // MS-CYRL
// CODEPAGE_MS_EE                    _S("MS-EE")                    // MS-EE
// CODEPAGE_MS_GREEK                 _S("MS-GREEK")                 // MS-GREEK
// CODEPAGE_MS_HEBR                  _S("MS-HEBR")                  // MS-HEBR
// CODEPAGE_MS_MAC_CYRILLIC          _S("MS-MAC-CYRILLIC")          // MS-MAC-CYRILLIC
// CODEPAGE_MS_TURK                  _S("MS-TURK")                  // MS-TURK
// CODEPAGE_MS932                    _S("MS932")                    // MS932
// CODEPAGE_MS936                    _S("MS936")                    // MS936
// CODEPAGE_MSCP949                  _S("MSCP949")                  // MSCP949
// CODEPAGE_MSCP1361                 _S("MSCP1361")                 // MSCP1361
// CODEPAGE_MSMACCYRILLIC            _S("MSMACCYRILLIC")            // MSMACCYRILLIC
// CODEPAGE_MSZ_7795_3               _S("MSZ_7795.3")               // MSZ_7795.3
// CODEPAGE_MS_KANJI                 _S("MS_KANJI")                 // MS_KANJI
// CODEPAGE_NAPLPS                   _S("NAPLPS")                   // NAPLPS
// CODEPAGE_NATS_DANO                _S("NATS-DANO")                // NATS-DANO
// CODEPAGE_NATS_SEFI                _S("NATS-SEFI")                // NATS-SEFI
// CODEPAGE_NATSDANO                 _S("NATSDANO")                 // NATSDANO
// CODEPAGE_NATSSEFI                 _S("NATSSEFI")                 // NATSSEFI
// CODEPAGE_NC_NC0010                _S("NC_NC0010")                // NC_NC0010
// CODEPAGE_NC_NC00_10               _S("NC_NC00-10")               // NC_NC00-10
// CODEPAGE_NC_NC00_10_81            _S("NC_NC00-10:81")            // NC_NC00-10:81
// CODEPAGE_NF_Z_62_010              _S("NF_Z_62-010")              // NF_Z_62-010
// CODEPAGE_NF_Z_62_010__1973_       _S("NF_Z_62-010_(1973)")       // NF_Z_62-010_(1973)
// CODEPAGE_NF_Z_62_010_1973         _S("NF_Z_62-010_1973")         // NF_Z_62-010_1973
// CODEPAGE_NF_Z_62010               _S("NF_Z_62010")               // NF_Z_62010
// CODEPAGE_NF_Z_62010_1973          _S("NF_Z_62010_1973")          // NF_Z_62010_1973
// CODEPAGE_NO                       _S("NO")                       // NO
// CODEPAGE_NO2                      _S("NO2")                      // NO2
// CODEPAGE_NS_4551_1                _S("NS_4551-1")                // NS_4551-1
// CODEPAGE_NS_4551_2                _S("NS_4551-2")                // NS_4551-2
// CODEPAGE_NS_45511                 _S("NS_45511")                 // NS_45511
// CODEPAGE_NS_45512                 _S("NS_45512")                 // NS_45512
// CODEPAGE_OS2LATIN1                _S("OS2LATIN1")                // OS2LATIN1
// CODEPAGE_OSF00010001              _S("OSF00010001")              // OSF00010001
// CODEPAGE_OSF00010002              _S("OSF00010002")              // OSF00010002
// CODEPAGE_OSF00010003              _S("OSF00010003")              // OSF00010003
// CODEPAGE_OSF00010004              _S("OSF00010004")              // OSF00010004
// CODEPAGE_OSF00010005              _S("OSF00010005")              // OSF00010005
// CODEPAGE_OSF00010006              _S("OSF00010006")              // OSF00010006
// CODEPAGE_OSF00010007              _S("OSF00010007")              // OSF00010007
// CODEPAGE_OSF00010008              _S("OSF00010008")              // OSF00010008
// CODEPAGE_OSF00010009              _S("OSF00010009")              // OSF00010009
// CODEPAGE_OSF0001000A              _S("OSF0001000A")              // OSF0001000A
// CODEPAGE_OSF00010020              _S("OSF00010020")              // OSF00010020
// CODEPAGE_OSF00010100              _S("OSF00010100")              // OSF00010100
// CODEPAGE_OSF00010101              _S("OSF00010101")              // OSF00010101
// CODEPAGE_OSF00010102              _S("OSF00010102")              // OSF00010102
// CODEPAGE_OSF00010104              _S("OSF00010104")              // OSF00010104
// CODEPAGE_OSF00010105              _S("OSF00010105")              // OSF00010105
// CODEPAGE_OSF00010106              _S("OSF00010106")              // OSF00010106
// CODEPAGE_OSF00030010              _S("OSF00030010")              // OSF00030010
// CODEPAGE_OSF0004000A              _S("OSF0004000A")              // OSF0004000A
// CODEPAGE_OSF0005000A              _S("OSF0005000A")              // OSF0005000A
// CODEPAGE_OSF05010001              _S("OSF05010001")              // OSF05010001
// CODEPAGE_OSF100201A4              _S("OSF100201A4")              // OSF100201A4
// CODEPAGE_OSF100201A8              _S("OSF100201A8")              // OSF100201A8
// CODEPAGE_OSF100201B5              _S("OSF100201B5")              // OSF100201B5
// CODEPAGE_OSF100201F4              _S("OSF100201F4")              // OSF100201F4
// CODEPAGE_OSF100203B5              _S("OSF100203B5")              // OSF100203B5
// CODEPAGE_OSF1002011C              _S("OSF1002011C")              // OSF1002011C
// CODEPAGE_OSF1002011D              _S("OSF1002011D")              // OSF1002011D
// CODEPAGE_OSF1002035D              _S("OSF1002035D")              // OSF1002035D
// CODEPAGE_OSF1002035E              _S("OSF1002035E")              // OSF1002035E
// CODEPAGE_OSF1002035F              _S("OSF1002035F")              // OSF1002035F
// CODEPAGE_OSF1002036B              _S("OSF1002036B")              // OSF1002036B
// CODEPAGE_OSF1002037B              _S("OSF1002037B")              // OSF1002037B
// CODEPAGE_OSF10010001              _S("OSF10010001")              // OSF10010001
// CODEPAGE_OSF10010004              _S("OSF10010004")              // OSF10010004
// CODEPAGE_OSF10010006              _S("OSF10010006")              // OSF10010006
// CODEPAGE_OSF10020025              _S("OSF10020025")              // OSF10020025
// CODEPAGE_OSF10020111              _S("OSF10020111")              // OSF10020111
// CODEPAGE_OSF10020115              _S("OSF10020115")              // OSF10020115
// CODEPAGE_OSF10020116              _S("OSF10020116")              // OSF10020116
// CODEPAGE_OSF10020118              _S("OSF10020118")              // OSF10020118
// CODEPAGE_OSF10020122              _S("OSF10020122")              // OSF10020122
// CODEPAGE_OSF10020129              _S("OSF10020129")              // OSF10020129
// CODEPAGE_OSF10020352              _S("OSF10020352")              // OSF10020352
// CODEPAGE_OSF10020354              _S("OSF10020354")              // OSF10020354
// CODEPAGE_OSF10020357              _S("OSF10020357")              // OSF10020357
// CODEPAGE_OSF10020359              _S("OSF10020359")              // OSF10020359
// CODEPAGE_OSF10020360              _S("OSF10020360")              // OSF10020360
// CODEPAGE_OSF10020364              _S("OSF10020364")              // OSF10020364
// CODEPAGE_OSF10020365              _S("OSF10020365")              // OSF10020365
// CODEPAGE_OSF10020366              _S("OSF10020366")              // OSF10020366
// CODEPAGE_OSF10020367              _S("OSF10020367")              // OSF10020367
// CODEPAGE_OSF10020370              _S("OSF10020370")              // OSF10020370
// CODEPAGE_OSF10020387              _S("OSF10020387")              // OSF10020387
// CODEPAGE_OSF10020388              _S("OSF10020388")              // OSF10020388
// CODEPAGE_OSF10020396              _S("OSF10020396")              // OSF10020396
// CODEPAGE_OSF10020402              _S("OSF10020402")              // OSF10020402
// CODEPAGE_OSF10020417              _S("OSF10020417")              // OSF10020417
// CODEPAGE_PT                       _S("PT")                       // PT
// CODEPAGE_PT2                      _S("PT2")                      // PT2
// CODEPAGE_PT154                    _S("PT154")                    // PT154
// CODEPAGE_R8                       _S("R8")                       // R8
// CODEPAGE_R9                       _S("R9")                       // R9
// CODEPAGE_RK1048                   _S("RK1048")                   // RK1048
// CODEPAGE_ROMAN8                   _S("ROMAN8")                   // ROMAN8
// CODEPAGE_ROMAN9                   _S("ROMAN9")                   // ROMAN9
// CODEPAGE_RUSCII                   _S("RUSCII")                   // RUSCII
// CODEPAGE_SE                       _S("SE")                       // SE
// CODEPAGE_SE2                      _S("SE2")                      // SE2
// CODEPAGE_SEN_850200_B             _S("SEN_850200_B")             // SEN_850200_B
// CODEPAGE_SEN_850200_C             _S("SEN_850200_C")             // SEN_850200_C
// CODEPAGE_SHIFT__JIS               _S("SHIFT-JIS")                // SHIFT-JIS
// CODEPAGE_SHIFT_JIS                _S("SHIFT_JIS")                // SHIFT_JIS
// CODEPAGE_SHIFT_JISX0213           _S("SHIFT_JISX0213")           // SHIFT_JISX0213
// CODEPAGE_SJIS_OPEN                _S("SJIS-OPEN")                // SJIS-OPEN
// CODEPAGE_SJIS_WIN                 _S("SJIS-WIN")                 // SJIS-WIN
// CODEPAGE_SJIS                     _S("SJIS")                     // SJIS
// CODEPAGE_SS636127                 _S("SS636127")                 // SS636127
// CODEPAGE_STRK1048_2002            _S("STRK1048-2002")            // STRK1048-2002
// CODEPAGE_ST_SEV_358_88            _S("ST_SEV_358-88")            // ST_SEV_358-88
// CODEPAGE_T_61_8BIT                _S("T.61-8BIT")                // T.61-8BIT
#define \
   CODEPAGE_20261                    _S("T.61")                     // T.61
// CODEPAGE_T_618BIT                 _S("T.618BIT")                 // T.618BIT
// CODEPAGE_TCVN_5712                _S("TCVN-5712")                // TCVN-5712
// CODEPAGE_TCVN                     _S("TCVN")                     // TCVN
// CODEPAGE_TCVN5712_1               _S("TCVN5712-1")               // TCVN5712-1
// CODEPAGE_TCVN5712_1_1993          _S("TCVN5712-1:1993")          // TCVN5712-1:1993
// CODEPAGE_THAI8                    _S("THAI8")                    // THAI8
// CODEPAGE_TIS_620                  _S("TIS-620")                  // TIS-620
// CODEPAGE_TIS620_0                 _S("TIS620-0")                 // TIS620-0
// CODEPAGE_TIS620_2529_1            _S("TIS620.2529-1")            // TIS620.2529-1
// CODEPAGE_TIS620_2533_0            _S("TIS620.2533-0")            // TIS620.2533-0
// CODEPAGE_TIS620                   _S("TIS620")                   // TIS620
// CODEPAGE_TS_5881                  _S("TS-5881")                  // TS-5881
// CODEPAGE_TSCII                    _S("TSCII")                    // TSCII
// CODEPAGE_TURKISH8                 _S("TURKISH8")                 // TURKISH8
// CODEPAGE_UCS_2                    _S("UCS-2")                    // UCS-2
// CODEPAGE_UCS_2BE                  _S("UCS-2BE")                  // UCS-2BE
// CODEPAGE_UCS_2LE                  _S("UCS-2LE")                  // UCS-2LE
// CODEPAGE_UCS_4                    _S("UCS-4")                    // UCS-4
// CODEPAGE_UCS_4BE                  _S("UCS-4BE")                  // UCS-4BE
// CODEPAGE_UCS_4LE                  _S("UCS-4LE")                  // UCS-4LE
// CODEPAGE_UCS2                     _S("UCS2")                     // UCS2
// CODEPAGE_UCS4                     _S("UCS4")                     // UCS4
// CODEPAGE_UHC                      _S("UHC")                      // UHC
// CODEPAGE_UJIS                     _S("UJIS")                     // UJIS
// CODEPAGE_UK                       _S("UK")                       // UK
// CODEPAGE_UNICODE                  _S("UNICODE")                  // UNICODE
// CODEPAGE_UNICODEBIG               _S("UNICODEBIG")               // UNICODEBIG
// CODEPAGE_UNICODELITTLE            _S("UNICODELITTLE")            // UNICODELITTLE
#define \
   CODEPAGE_20127                    _S("US-ASCII")                 // US-ASCII
// CODEPAGE_US                       _S("US")                       // US
// CODEPAGE_UTF_7                    _S("UTF-7")                    // UTF-7
// CODEPAGE_UTF_8                    _S("UTF-8")                    // UTF-8
// CODEPAGE_UTF_16                   _S("UTF-16")                   // UTF-16
// CODEPAGE_UTF_16BE                 _S("UTF-16BE")                 // UTF-16BE
// CODEPAGE_UTF_16LE                 _S("UTF-16LE")                 // UTF-16LE
// CODEPAGE_UTF_32                   _S("UTF-32")                   // UTF-32
// CODEPAGE_UTF_32BE                 _S("UTF-32BE")                 // UTF-32BE
// CODEPAGE_UTF_32LE                 _S("UTF-32LE")                 // UTF-32LE
// CODEPAGE_UTF7                     _S("UTF7")                     // UTF7
// CODEPAGE_UTF8                     _S("UTF8")                     // UTF8
// CODEPAGE_UTF16                    _S("UTF16")                    // UTF16
// CODEPAGE_UTF16BE                  _S("UTF16BE")                  // UTF16BE
// CODEPAGE_UTF16LE                  _S("UTF16LE")                  // UTF16LE
// CODEPAGE_UTF32                    _S("UTF32")                    // UTF32
// CODEPAGE_UTF32BE                  _S("UTF32BE")                  // UTF32BE
// CODEPAGE_UTF32LE                  _S("UTF32LE")                  // UTF32LE
// CODEPAGE_VISCII                   _S("VISCII")                   // VISCII
// CODEPAGE_WCHAR_T                  _S("WCHAR_T")                  // WCHAR_T
// CODEPAGE_WIN_SAMI_2               _S("WIN-SAMI-2")               // WIN-SAMI-2
// CODEPAGE_WINBALTRIM               _S("WINBALTRIM")               // WINBALTRIM
// CODEPAGE_WINDOWS_31J              _S("WINDOWS-31J")              // WINDOWS-31J
// CODEPAGE_WINDOWS_874              _S("WINDOWS-874")              // WINDOWS-874
// CODEPAGE_WINDOWS_936              _S("WINDOWS-936")              // WINDOWS-936
// CODEPAGE_WINDOWS_1250             _S("WINDOWS-1250")             // WINDOWS-1250
// CODEPAGE_WINDOWS_1251             _S("WINDOWS-1251")             // WINDOWS-1251
// CODEPAGE_WINDOWS_1252             _S("WINDOWS-1252")             // WINDOWS-1252
// CODEPAGE_WINDOWS_1253             _S("WINDOWS-1253")             // WINDOWS-1253
// CODEPAGE_WINDOWS_1254             _S("WINDOWS-1254")             // WINDOWS-1254
// CODEPAGE_WINDOWS_1255             _S("WINDOWS-1255")             // WINDOWS-1255
// CODEPAGE_WINDOWS_1256             _S("WINDOWS-1256")             // WINDOWS-1256
// CODEPAGE_WINDOWS_1257             _S("WINDOWS-1257")             // WINDOWS-1257
// CODEPAGE_WINDOWS_1258             _S("WINDOWS-1258")             // WINDOWS-1258
// CODEPAGE_WINSAMI2                 _S("WINSAMI2")                 // WINSAMI2
// CODEPAGE_WS2                      _S("WS2")                      // WS2
// CODEPAGE_YU                       _S("YU")                       // YU

//------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
