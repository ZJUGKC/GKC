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
	"${PROJECT_SOURCE_DIR}/RT/GkcParser/public"
	"${PROJECT_SOURCE_DIR}/RT/GkcParser/include"
	"${PROJECT_SOURCE_DIR}/RT/GkcCompiler/public"
	"${PROJECT_SOURCE_DIR}/RT/GkcSys/public"
	"${PROJECT_SOURCE_DIR}/public/include")
SET(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")
SET(SRC_PCH_LIST "src/PreComp.cpp")
SET(SRC_LIST "src/Main.cpp" "src/Public.cpp" "src/Parser.cpp"
	"src/wlang/action/WlangGrammarError.cpp" "src/wlang/action/WlangGrammarAccepted.cpp"
	"src/wlang/action/WlangDoNsBodyAction.cpp" "src/wlang/action/WlangDoBodySemiAction.cpp"
	"src/wlang/WlangAction.cpp" "src/wlang/WlangUtility.cpp" "src/wlang/WlangParser.cpp"
	"src/won/action/WonDoCommonAction.cpp" "src/won/action/WonDoStringCatenationAction.cpp" "src/won/action/WonDoStringCatenationStringAction.cpp" "src/won/action/WonDoValueStringCatenationAction.cpp"
	"src/won/WonUtility.cpp" "src/won/WonParser.cpp"
	"src/wmark/action/WmarkGrammarAccepted.cpp"
	"src/wmark/action/WmarkReturnAction.cpp" "src/wmark/action/WmarkCommentStartAction.cpp"
	"src/wmark/action/WmarkDoDocRsBlockListAction.cpp" "src/wmark/action/WmarkDoBlockListListBlockAction.cpp" "src/wmark/action/WmarkDoBlockBlockBodyAction.cpp" "src/wmark/action/WmarkDoBlockBodyCommentAction.cpp" "src/wmark/action/WmarkDoBlockBodyIndentAction.cpp" "src/wmark/action/WmarkDoBlockBodyTextAction.cpp"
	"src/wmark/WmarkAction.cpp" "src/wmark/WmarkUtility.cpp" "src/wmark/WmarkParser.cpp")
ADD_LIBRARY(GkcParser SHARED ${SRC_PCH_LIST} ${SRC_LIST})
SET_PCH(GkcParser "shared" "PreComp.h" ${SRC_PCH_LIST} ${SRC_LIST})
SET_TARGET_TYPE("shared")
SET_TARGET_PROPERTIES(GkcParser PROPERTIES OUTPUT_NAME "GkcParser"
	VERSION 1.0 SOVERSION 1
	LINK_FLAGS "${STRING_LINK_FLAGS_TARGET}")
TARGET_LINK_LIBRARIES(GkcParser GkcCompiler ${STRING_LINK_EXTRA_LIBS})

INSTALL(TARGETS GkcParser
	RUNTIME DESTINATION core/assemblies COMPONENT runtime
	LIBRARY DESTINATION core/assemblies COMPONENT runtime
	ARCHIVE DESTINATION development/lib COMPONENT development)

INSTALL(DIRECTORY "${PROJECT_SOURCE_DIR}/RT/GkcParser/public/"
	DESTINATION development/include/parser COMPONENT development)

#end
