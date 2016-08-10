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
SET(SRC_LIST "src/Main.cpp" "src/Public.cpp" "src/Action.cpp" "src/Analyzer.cpp"
	"src/action/CommentStartAction.cpp" "src/action/LineCommentStartAction.cpp" "src/action/SpaceAction.cpp" "src/action/ReturnAction.cpp"
	"src/ldf/LdfFsa.cpp" "src/ldf/LdfLexPda.cpp" "src/ldf/LdfRegexFsa.cpp" "src/ldf/LdfRegexPda.cpp" "src/ldf/LdfGraPda.cpp"
	"src/ldf/MacroTokenAction.cpp" "src/ldf/DoIdTokenMacroAction.cpp" "src/ldf/RegexCharAction.cpp" "src/ldf/RegexDoCharAction.cpp"
	"src/ldf/RegexCharSymbolData.cpp" "src/ldf/RegexCharSymbolData_Factory.cpp"
	"src/analyzer/LexerTables.cpp" "src/analyzer/LexerAnalyzer.cpp" "src/analyzer/GrammarTables.cpp" "src/analyzer/GrammarAnalyzer.cpp"
	"src/analyzer/BasicSymbolData.cpp" "src/analyzer/BasicSymbolData_Factory.cpp")
ADD_LIBRARY(GkcCompiler SHARED ${SRC_PCH_LIST} ${SRC_LIST})
SET_PCH(GkcCompiler "shared" "PreComp.h" ${SRC_PCH_LIST} ${SRC_LIST})
SET_TARGET_SUBSYSTEM("Background")
SET_TARGET_PROPERTIES(GkcCompiler PROPERTIES OUTPUT_NAME "GkcCompiler"
	VERSION 1.0 SOVERSION 1
	LINK_FLAGS "${STRING_LINK_FLAGS_TARGET}")
TARGET_LINK_LIBRARIES(GkcCompiler GkcSys ${STRING_LINK_EXTRA_LIBS})

INSTALL(TARGETS GkcCompiler
	RUNTIME DESTINATION public/assemblies COMPONENT runtime
	LIBRARY DESTINATION public/assemblies COMPONENT runtime
	ARCHIVE DESTINATION public/lib COMPONENT development)

INSTALL(DIRECTORY "${PROJECT_SOURCE_DIR}/RT/GkcCompiler/public/"
	DESTINATION public/include/compiler COMPONENT development)

#end
