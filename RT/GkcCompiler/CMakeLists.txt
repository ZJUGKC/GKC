#
# Copyright (c) 2016, Xin YUAN, courses of Zhejiang University
# All rights reserved.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the 2-Clause BSD License.
#
# Author contact information:
#   yxxinyuan@zju.edu.cn
#

INCLUDE_DIRECTORIES(
	"${CMAKE_CURRENT_BINARY_DIR}"
	"${CMAKE_CURRENT_SOURCE_DIR}"
	"${PROJECT_SOURCE_DIR}/RT/GkcCompiler/public"
	"${PROJECT_SOURCE_DIR}/RT/GkcCompiler/include"
	"${PROJECT_SOURCE_DIR}/RT/GkcSys/public"
	"${PROJECT_SOURCE_DIR}/public/include")
SET(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")
SET(SRC_PCH_LIST "src/PreComp.cpp")
SET(SRC_LIST "src/Main.cpp" "src/Public.cpp" "src/Action.cpp" "src/Analyzer.cpp" "src/Rdp.cpp"
	"src/action/CommentStartAction.cpp" "src/action/LineCommentStartAction.cpp" "src/action/SpaceAction.cpp" "src/action/ReturnAction.cpp"
	"src/ldf/base/LdfFsa.cpp"
	"src/ldf/lex/LdfLexPda.cpp" "src/ldf/lex/MacroTokenAction.cpp" "src/ldf/lex/DoIdTokenMacroAction.cpp"
	"src/ldf/regex/LdfRegexFsa.cpp" "src/ldf/regex/LdfRegexPda.cpp" 
	"src/ldf/regex/RegexCharAction.cpp" "src/ldf/regex/RegexDoCharAction.cpp" "src/ldf/regex/RegexDoCharSAction.cpp" "src/ldf/regex/RegexDoCharRangeAction.cpp" "src/ldf/regex/RegexDoCharItemCharEAction.cpp" "src/ldf/regex/RegexDoCharItemItemCharEAction.cpp"
	"src/ldf/regex/RegexDoCharSetAction.cpp" "src/ldf/regex/RegexDoCharSetUpAction.cpp" "src/ldf/regex/RegexDoFactorCharSetAction.cpp" "src/ldf/regex/RegexDoFactorCharAction.cpp" "src/ldf/regex/RegexDoFactorParenExpAction.cpp" "src/ldf/regex/RegexDoFactor1QuestionAction.cpp" "src/ldf/regex/RegexDoFactor1StarAction.cpp" "src/ldf/regex/RegexDoFactor1PlusAction.cpp" "src/ldf/regex/RegexDoTermTermFactor1Action.cpp" "src/ldf/regex/RegexDoExpExpTermAction.cpp"
	"src/ldf/regex/RegexGrammarAccepted.cpp"
	"src/ldf/regex/RegexCharSymbolData.cpp" "src/ldf/regex/RegexCharSymbolData_Factory.cpp" "src/ldf/regex/RegexCharSetSymbolData.cpp" "src/ldf/regex/RegexCharSetSymbolData_Factory.cpp" "src/ldf/regex/RegexPositionSymbolData.cpp" "src/ldf/regex/RegexPositionSymbolData_Factory.cpp"
	"src/ldf/gra/LdfGraPda.cpp"
	"src/ldf/gra/GraDoIdTokenAction.cpp" "src/ldf/gra/GraDoIdMacroAction.cpp" "src/ldf/gra/GraDoIdAction.cpp" "src/ldf/gra/GraDoIdBlockAction.cpp" "src/ldf/gra/GraDoRightIdBlockAction.cpp" "src/ldf/gra/GraDoRightAction.cpp" "src/ldf/gra/GraDoRightBlockAction.cpp" "src/ldf/gra/GraDoRuleRightBlockAction.cpp"
	"src/ldf/gra/GraTokenSymbolData.cpp" "src/ldf/gra/GraTokenSymbolData_Factory.cpp" "src/ldf/gra/GraPositionSymbolData.cpp" "src/ldf/gra/GraPositionSymbolData_Factory.cpp"
	"src/ldf/RegexAction.cpp" "src/ldf/LexAction.cpp" "src/ldf/GraAction.cpp" "src/ldf/LexAnalyzer.cpp" "src/ldf/GraAnalyzer.cpp"
	"src/analyzer/LexerTables.cpp" "src/analyzer/LexerAnalyzer.cpp" "src/analyzer/GrammarTables.cpp" "src/analyzer/GrammarAnalyzer.cpp"
	"src/analyzer/BasicSymbolData.cpp" "src/analyzer/BasicSymbolData_Factory.cpp"
	"src/meta/CplMetaData.cpp"
	"src/meta/CplMetaDataPositionSymbolData.cpp" "src/meta/CplMetaDataPositionSymbolData_Factory.cpp"
	"src/rdp/ldf/token/TkAction.cpp" "src/rdp/ldf/token/NtAction.cpp" "src/rdp/ldf/token/TkidAction.cpp" "src/rdp/ldf/token/ActidAction.cpp"
	"src/rdp/ldf/scanner/DoIdAction.cpp"
	"src/rdp/ldf/parser/DoRuleItemEndAction.cpp" "src/rdp/ldf/parser/DoNtAction.cpp" "src/rdp/ldf/parser/DoVertAction.cpp" "src/rdp/ldf/parser/DoActAction.cpp" "src/rdp/ldf/parser/DoEpsAction.cpp" "src/rdp/ldf/parser/DoTkAction.cpp"
	"src/rdp/ldf/TokenAction.cpp" "src/rdp/ldf/ScannerAction.cpp" "src/rdp/ldf/ParserAction.cpp"
	"src/rdp/RdScannerTables.cpp" "src/rdp/RdScanner.cpp" "src/rdp/RdParserTables.cpp" "src/rdp/RdParser.cpp")
ADD_LIBRARY(GkcCompiler SHARED ${SRC_PCH_LIST} ${SRC_LIST})
SET_PCH(GkcCompiler "shared" "PreComp.h" ${SRC_PCH_LIST} ${SRC_LIST})
SET_TARGET_SUBSYSTEM("Background")
SET_TARGET_PROPERTIES(GkcCompiler PROPERTIES OUTPUT_NAME "GkcCompiler"
	VERSION 1.0 SOVERSION 1
	LINK_FLAGS "${STRING_LINK_FLAGS_TARGET}")
TARGET_LINK_LIBRARIES(GkcCompiler GkcSys ${STRING_LINK_EXTRA_LIBS})

INSTALL(TARGETS GkcCompiler
	RUNTIME DESTINATION core/assemblies COMPONENT runtime
	LIBRARY DESTINATION core/assemblies COMPONENT runtime
	ARCHIVE DESTINATION development/lib COMPONENT development)

INSTALL(DIRECTORY "${PROJECT_SOURCE_DIR}/RT/GkcCompiler/public/"
	DESTINATION development/include/compiler COMPONENT development)

#end
