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

#stop
service GkcHostSvc stop
#delete
rm /etc/init.d/GkcHostSvc
rm /etc/rc0.d/K20GkcHostSvc
rm /etc/rc1.d/K20GkcHostSvc
rm /etc/rc2.d/S20GkcHostSvc
rm /etc/rc3.d/S20GkcHostSvc
rm /etc/rc4.d/S20GkcHostSvc
rm /etc/rc5.d/S20GkcHostSvc
rm /etc/rc6.d/K20GkcHostSvc

#end
