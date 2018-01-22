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

#environment variables
strEnvSystemRoot = "GKC_SYSTEM_ROOT"
strEnvLws = "GKC_LOCAL_WORKSPACE"
strEnvUws = "GKC_UNIFIED_WORKSPACE"

#command parameters
if [ $# -gt 1 ]; then
	echo "Usage: sudo sh uninstall-gkc.sh [-a]"
	exit 1
fi

iUninstallMode = 0
if [ $# -eq 1 ]; then
	if [ $1 = "-a" ]; then
		iUninstallMode = 1
	else
		echo "Command error: unknown option!"
		exit 1
	fi
fi

#uninstall
echo "Uninstall..."

#environment
eval strInstallDir = "\$${strEnvSystemRoot}"
if [[ -z ${strInstallDir} ]]; then
	echo "Error: The software is not installed!"
	exit 1
fi
eval strLwsDir = "\$${strEnvLws}"
if [[ -z ${strLwsDir} ]]; then
	echo "Error: The software is not installed!"
	exit 1
fi
eval strUwsDir = "\$${strEnvUws}"
if [[ -z ${strUwsDir} ]]; then
	echo "Error: The software is not installed!"
	exit 1
fi

if [ ! -d "${strInstallDir}" ]; then
	echo "Error: The software is not installed!"
	exit 1
fi
if [ ! -d "${strLwsDir}" ]; then
	echo "Error: The software is not installed!"
	exit 1
fi
if [ ! -d "${strUwsDir}" ]; then
	echo "Error: The software is not installed!"
	exit 1
fi

#services
strTemp = "${strInstallDir}/core/services"
if [ -d "${strTemp}" ]; then
	sh "${strTemp}/ghs-uninstall.sh"
	sleep 0.5
fi

#environment
rm /etc/profile.d/GKC.sh
source /etc/profile

rm /etc/ld.so.conf.d/GKC.conf
ldconfig

strFile = "/etc/environment"
cat ${strFile} | while read strLine
do
	if [[ ! $strLine == *$strEnvSystemRoot* ]] && [[ ! $strLine == *$strEnvLws* ]] && [[ ! $strLine == *$strEnvUws* ]]; then
		echo "${strLine}" >> ${strFile}_tmpd
	fi
done
rm ${strFile}
sleep 0.5
mv ${strFile}_tmpd ${strFile}
source ${strFile}

#folders
rm -r "${strInstallDir}"
if [ $iUninstallMode -eq 1 ]; then
	rm -r "${strLwsDir}"
	rm -r "${strUwsDir}"
	strTemp = ${strInstallDir%/*}
	rm -r "${strTemp}"
fi

#done
echo "done."

#end
