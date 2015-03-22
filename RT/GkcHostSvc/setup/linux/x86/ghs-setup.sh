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

#copy
cp /usr/.GKC/public/assemblies/GkcScheduleSvc /etc/init.d/GkcScheduleSvc
#right
chmod ugoa+x /etc/init.d/GkcScheduleSvc
#runlevels
update-rc.d GkcScheduleSvc defaults
#/sbin/chkconfig --add GkcScheduleSvc
#start
service GkcScheduleSvc start
#status
service GkcScheduleSvc status

#end
