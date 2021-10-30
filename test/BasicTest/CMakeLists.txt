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
	"${PROJECT_SOURCE_DIR}/test/BasicTest/include"
	"${PROJECT_SOURCE_DIR}/public/include")
LINK_DIRECTORIES("${PROJECT_BINARY_DIR}/bin")
SET(STRING_CURRENT_CONFIG "$<CONFIG>")
IF(${STRING_CURRENT_CONFIG} STREQUAL "Debug")
	SET(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")
ELSE()
	SET(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/test-bin")
ENDIF()
SET(SRC_PCH_LIST "src/PreComp.cpp")
SET(SRC_LIST "src/Main.cpp" "src/ShareTest.cpp" "src/StreamTest.cpp" "src/PkzTest.cpp")
ADD_EXECUTABLE(BasicTest ${SRC_PCH_LIST} ${SRC_LIST})
SET_PCH(BasicTest "exe" "PreComp.h" ${SRC_PCH_LIST} ${SRC_LIST})
SET_TARGET_SUBSYSTEM("Foreground")
SET_TARGET_PROPERTIES(BasicTest PROPERTIES OUTPUT_NAME "BasicTest"
	LINK_FLAGS "${STRING_LINK_FLAGS_TARGET}")
TARGET_LINK_LIBRARIES(BasicTest GkcSys ${STRING_LINK_EXTRA_LIBS})

#end
