#
# Copyright (c) 2015, Xin YUAN, courses of Zhejiang University
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
	"${PROJECT_SOURCE_DIR}/RT/GkcScheduleSvc/include"
	"${PROJECT_SOURCE_DIR}/public/include")
LINK_DIRECTORIES("${PROJECT_BINARY_DIR}/bin")
SET(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")
SET(SRC_PCH_LIST "src/precomp.cpp")
SET(SRC_LIST "src/main.cpp")
ADD_EXECUTABLE(GkcScheduleSvc ${SRC_PCH_LIST} ${SRC_LIST})
SET_PCH(GkcScheduleSvc "exe" "precomp.h" ${SRC_PCH_LIST} ${SRC_LIST})
SET_TARGET_SUBSYSTEM("Background")
SET_TARGET_PROPERTIES(GkcScheduleSvc PROPERTIES OUTPUT_NAME "GkcScheduleSvc-1-0"
	LINK_FLAGS "${STRING_LINK_FLAGS_TARGET}")
TARGET_LINK_LIBRARIES(GkcScheduleSvc GkcSys)

#copy
SET_POSTBUILD_PATH_COPY(GkcScheduleSvc "platform" "machine" "config" "${PROJECT_SOURCE_DIR}/RT/GkcScheduleSvc/setup" "*" "${EXECUTABLE_OUTPUT_PATH}")

INSTALL(TARGETS GkcScheduleSvc
	RUNTIME DESTINATION public/assemblies COMPONENT runtime)
SET_POSTBUILD_SRC_PATH("platform" "machine" "${PROJECT_SOURCE_DIR}/RT/GkcScheduleSvc/setup")
INSTALL(DIRECTORY "${POSTBUILD_SRC_PATH}/"
	DESTINATION public/assemblies COMPONENT runtime)

#end
