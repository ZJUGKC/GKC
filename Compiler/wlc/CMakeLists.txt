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
	"${PROJECT_SOURCE_DIR}/Compiler/wlc/include"
	"${PROJECT_SOURCE_DIR}/public/include")
LINK_DIRECTORIES("${PROJECT_BINARY_DIR}/bin")
SET(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")
SET(SRC_PCH_LIST "src/precomp.cpp")
SET(SRC_LIST "src/main.cpp")
ADD_EXECUTABLE(wlc ${SRC_PCH_LIST} ${SRC_LIST})
SET_PCH(wlc "exe" "precomp.h" ${SRC_PCH_LIST} ${SRC_LIST})
SET_TARGET_SUBSYSTEM("Foreground")
SET_TARGET_PROPERTIES(wlc PROPERTIES OUTPUT_NAME "wlc-1-0"
	LINK_FLAGS "${STRING_LINK_FLAGS_TARGET}")
TARGET_LINK_LIBRARIES(wlc GkcSys)

#end
