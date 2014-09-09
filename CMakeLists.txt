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

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
INCLUDE("common.cmk")
PROJECT(GKC)
ADD_SUBDIRECTORY(RT/GkcSys RT/GkcSys)
ADD_SUBDIRECTORY(Compiler/wlc Compiler/wlc)
ADD_SUBDIRECTORY(test/BasicTest test/BasicTest)

#end
