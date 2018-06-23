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
strEnvSystemRoot="GKC_SYSTEM_ROOT="
strEnvLws="GKC_LOCAL_WORKSPACE="
strEnvUws="GKC_UNIFIED_WORKSPACE="

cat $1 | while read -rs strLine;
do
	if [[ $strLine == *$strEnvSystemRoot* ]]; then
		echo "${strLine}\"$2\"" >> /etc/init.d/GkcHostSvc
	elif [[ $strLine == *$strEnvLws* ]]; then
		echo "${strLine}\"$3\"" >> /etc/init.d/GkcHostSvc
	elif [[ $strLine == *$strEnvUws* ]]; then
		echo "${strLine}\"$4\"" >> /etc/init.d/GkcHostSvc
	else
		echo "${strLine}" >> /etc/init.d/GkcHostSvc
	fi
done
sleep 0.5

#right
chmod ugoa+x /etc/init.d/GkcHostSvc
#runlevels
update-rc.d GkcHostSvc defaults
#/sbin/chkconfig --add GkcHostSvc
#start
service GkcHostSvc start
#status
#service GkcHostSvc status

#end
