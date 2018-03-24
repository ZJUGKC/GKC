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
	"${PROJECT_SOURCE_DIR}/tools/mdp/include"
	"${PROJECT_SOURCE_DIR}/RT/GkcParser/public"
	"${PROJECT_SOURCE_DIR}/RT/GkcCompiler/public"
	"${PROJECT_SOURCE_DIR}/RT/GkcSys/public"
	"${PROJECT_SOURCE_DIR}/public/include")
LINK_DIRECTORIES("${PROJECT_BINARY_DIR}/bin")
SET(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")
SET(SRC_PCH_LIST "src/PreComp.cpp")
SET(SRC_LIST "src/Main.cpp" "src/Global.cpp")
SET_PCH(mdp "exe" "PreComp.h" ${SRC_PCH_LIST} ${SRC_LIST})
SET_TARGET_SUBSYSTEM("Foreground")
SET_TARGET_PROPERTIES(mdp PROPERTIES OUTPUT_NAME "mdp"
	LINK_FLAGS "${STRING_LINK_FLAGS_TARGET}")
TARGET_LINK_LIBRARIES(mdp GkcSys GkcCompiler GkcParser ${STRING_LINK_EXTRA_LIBS})

INSTALL(TARGETS mdp
	RUNTIME DESTINATION core/tools COMPONENT runtime)

#end
