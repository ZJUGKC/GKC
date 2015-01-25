#!/bin/sh

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

#install
mkdir /usr/.GKC
./GKC-1.0.1-Linux.sh --prefix=/usr/.GKC --exclude-subdir
#path
echo "/usr/.GKC/public/assemblies" > /etc/ld.so.conf.d/GKC.conf
ldconfig
