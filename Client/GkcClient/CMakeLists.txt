#
# Copyright (c) 2018, Xin YUAN, courses of Zhejiang University
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
	"${PROJECT_SOURCE_DIR}/Client/GkcClient/include"
	"${PROJECT_SOURCE_DIR}/RT/GkcSys/public"
	"${PROJECT_SOURCE_DIR}/public/include")
LINK_DIRECTORIES("${PROJECT_BINARY_DIR}/bin")
SET(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")
SET(SRC_PCH_LIST "src/PreComp.cpp")
SET(SRC_LIST "src/Main.cpp")
ADD_EXECUTABLE(GkcClient ${SRC_PCH_LIST} ${SRC_LIST})
SET_PCH(GkcClient "exe" "PreComp.h" ${SRC_PCH_LIST} ${SRC_LIST})
SET_TARGET_SUBSYSTEM("Background")
SET_TARGET_PROPERTIES(GkcClient PROPERTIES OUTPUT_NAME "GkcClient"
	LINK_FLAGS "${STRING_LINK_FLAGS_TARGET}")
TARGET_LINK_LIBRARIES(GkcClient GkcSys ${STRING_LINK_EXTRA_LIBS})

INSTALL(TARGETS GkcClient
	RUNTIME DESTINATION client COMPONENT runtime)

#end
