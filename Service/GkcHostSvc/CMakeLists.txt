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
	"${PROJECT_SOURCE_DIR}/Service/GkcHostSvc/include"
	"${PROJECT_SOURCE_DIR}/public/include")
LINK_DIRECTORIES("${PROJECT_BINARY_DIR}/bin")
SET(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")
SET(SRC_PCH_LIST "src/PreComp.cpp")
SET(SRC_LIST "src/Main.cpp")
ADD_EXECUTABLE(GkcHostSvc ${SRC_PCH_LIST} ${SRC_LIST})
SET_PCH(GkcHostSvc "exe" "PreComp.h" ${SRC_PCH_LIST} ${SRC_LIST})
SET_TARGET_SUBSYSTEM("Background")
SET_TARGET_PROPERTIES(GkcHostSvc PROPERTIES OUTPUT_NAME "GkcHostSvc"
	LINK_FLAGS "${STRING_LINK_FLAGS_TARGET}")
TARGET_LINK_LIBRARIES(GkcHostSvc GkcSys ${STRING_LINK_EXTRA_LIBS})

#copy
SET_TARGET_PATH_COPY(GkcHostSvc TRUE "platform" "machine" "no-config" "config" "${PROJECT_SOURCE_DIR}/Service/GkcHostSvc/setup" "*" "${EXECUTABLE_OUTPUT_PATH}")

INSTALL(TARGETS GkcHostSvc
	RUNTIME DESTINATION core/services COMPONENT runtime)

SET_TARGET_SRC_PATH("platform" "machine" "no-config" "" "${PROJECT_SOURCE_DIR}/Service/GkcHostSvc/setup")
INSTALL(DIRECTORY "${TARGET_SRC_PATH}/"
	DESTINATION core/services COMPONENT runtime)

#end
