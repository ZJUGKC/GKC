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
	"${PROJECT_SOURCE_DIR}/RT/GkcSys/include"
	"${PROJECT_SOURCE_DIR}/public/include")
SET(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")
SET(SRC_PCH_LIST "src/precomp.cpp")
SET(SRC_LIST "src/main.cpp" "src/globals.cpp" "src/spb_pool.cpp")
ADD_LIBRARY(GkcSys SHARED ${SRC_PCH_LIST} ${SRC_LIST})
SET_PCH(GkcSys "shared" "precomp.h" ${SRC_PCH_LIST} ${SRC_LIST})
SET_TARGET_SUBSYSTEM("Background")
SET_TARGET_PROPERTIES(GkcSys PROPERTIES OUTPUT_NAME "GkcSys-1-0"
	VERSION 1.0 SOVERSION 1
	LINK_FLAGS "${STRING_LINK_FLAGS_TARGET}")

#end
