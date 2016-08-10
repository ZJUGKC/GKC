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
	"src/wlang/WlangGrammarError.cpp" "src/wlang/WlangParser.cpp")
ADD_LIBRARY(GkcParser SHARED ${SRC_PCH_LIST} ${SRC_LIST})
SET_PCH(GkcParser "shared" "PreComp.h" ${SRC_PCH_LIST} ${SRC_LIST})
SET_TARGET_SUBSYSTEM("Background")
SET_TARGET_PROPERTIES(GkcParser PROPERTIES OUTPUT_NAME "GkcParser"
	VERSION 1.0 SOVERSION 1
	LINK_FLAGS "${STRING_LINK_FLAGS_TARGET}")
TARGET_LINK_LIBRARIES(GkcParser GkcCompiler ${STRING_LINK_EXTRA_LIBS})

INSTALL(TARGETS GkcParser
	RUNTIME DESTINATION public/assemblies COMPONENT runtime
	LIBRARY DESTINATION public/assemblies COMPONENT runtime
	ARCHIVE DESTINATION public/lib COMPONENT development)

INSTALL(DIRECTORY "${PROJECT_SOURCE_DIR}/RT/GkcParser/public/"
	DESTINATION public/include/parser COMPONENT development)

#end
