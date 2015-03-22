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
cp /usr/.GKC/public/assemblies/GkcHostSvc /etc/init.d/GkcHostSvc
#right
chmod ugoa+x /etc/init.d/GkcHostSvc
#runlevels
update-rc.d GkcHostSvc defaults
#/sbin/chkconfig --add GkcHostSvc
#start
service GkcHostSvc start
#status
service GkcHostSvc status

#end
