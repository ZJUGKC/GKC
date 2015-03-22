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

#services
chmod ugoa+x /usr/.GKC/public/assemblies/ghs-uninstall.sh
/usr/.GKC/public/assemblies/ghs-uninstall.sh
#uninstall
rm -r /usr/.GKC
#path
rm /etc/ld.so.conf.d/GKC.conf
ldconfig

echo "uninstall done."

#end
