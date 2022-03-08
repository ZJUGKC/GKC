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

CMAKE_MINIMUM_REQUIRED(VERSION 3.14)
INCLUDE("common.cmk")

PROJECT(GKC)

CHECK_BUILDING_SYSTEM()
CHECK_BUILDING_DIR()

#sub-directories
ADD_SUBDIRECTORY(RT RT)
ADD_SUBDIRECTORY(tools tools)
ADD_SUBDIRECTORY(Compiler/wlc Compiler/wlc)
ADD_SUBDIRECTORY(Compiler/file2code Compiler/file2code)
ADD_SUBDIRECTORY(test test)

#pack
SET(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")
SET(CPACK_PACKAGE_NAME "GKC")
SET(CPACK_PACKAGE_VENDER "ZJU-GKC")
SET(CPACK_PACKAGE_DIRECTORY "${PROJECT_BINARY_DIR}/_pack")
SET(CPACK_PACKAGE_VERSION_MAJOR "1")
SET(CPACK_PACKAGE_VERSION_MINOR "0")
SET(CPACK_PACKAGE_VERSION_PATCH "1")
INCLUDE(CPack)

#end
