#
# Copyright (c) 2013, Xin YUAN, courses of Zhejiang University
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
	"${PROJECT_SOURCE_DIR}/RT/GkcSys/public"
	"${PROJECT_SOURCE_DIR}/RT/GkcSys/include"
	"${PROJECT_SOURCE_DIR}/public/include")
SET(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")
SET(SRC_PCH_LIST "src/PreComp.cpp")
SET(SRC_LIST "src/Main.cpp" "src/Public.cpp" "src/Globals.cpp" "src/SobPool.cpp" "src/MemMgr.cpp"
	"src/Stream.cpp" "src/ComClient.cpp" "src/HelpAuthoring.cpp" "src/Compression.cpp" "src/LiteCom.cpp" "src/Logger.cpp"
	"src/stream/FileStream.cpp" "src/stream/MemoryStream.cpp" "src/stream/MemoryStreamU.cpp" "src/stream/BufferStream.cpp"
	"src/stream/_TxtStream.cpp" "src/stream/TextStream.cpp" "src/stream/TextStreamU.cpp"
	"src/compress/_PkCompress.cpp" "src/compress/PkzCompressor.cpp" "src/compress/PkzDecompressor.cpp")
ADD_LIBRARY(GkcSys SHARED ${SRC_PCH_LIST} ${SRC_LIST})
SET_PCH(GkcSys "shared" "PreComp.h" ${SRC_PCH_LIST} ${SRC_LIST})
SET_TARGET_TYPE("shared")
SET_TARGET_PROPERTIES(GkcSys PROPERTIES OUTPUT_NAME "GkcSys"
	VERSION 1.0 SOVERSION 1
	LINK_FLAGS "${STRING_LINK_FLAGS_TARGET}")

INSTALL(TARGETS GkcSys
	RUNTIME DESTINATION core/assemblies COMPONENT runtime
	LIBRARY DESTINATION core/assemblies COMPONENT runtime
	ARCHIVE DESTINATION development/lib COMPONENT development)

INSTALL(DIRECTORY "${PROJECT_SOURCE_DIR}/public/include/"
	DESTINATION development/include COMPONENT development)
INSTALL(DIRECTORY "${PROJECT_SOURCE_DIR}/RT/GkcSys/public/"
	DESTINATION development/include/sys COMPONENT development)
INSTALL(FILES "${PROJECT_SOURCE_DIR}/common.cmk"
	DESTINATION development/cmake COMPONENT development)
INSTALL(FILES "${PROJECT_SOURCE_DIR}/ftsc.cmk"
	DESTINATION development/cmake COMPONENT development)

INSTALL(FILES "${PROJECT_SOURCE_DIR}/LICENSE"
	DESTINATION core COMPONENT runtime)
INSTALL(DIRECTORY "${PROJECT_SOURCE_DIR}/licenses/"
	DESTINATION core/licenses COMPONENT runtime)

#end
