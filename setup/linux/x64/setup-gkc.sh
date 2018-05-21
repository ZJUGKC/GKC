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

#package name
strPackageName = "GKC-1.0.1-Linux.sh"
#environment variables
strEnvSystemRoot = "GKC_SYSTEM_ROOT"
strEnvLws = "GKC_LOCAL_WORKSPACE"
strEnvUws = "GKC_UNIFIED_WORKSPACE"

#command parameters
if [ $# -lt 1 -o $# -gt 7 ]; then
	echo "Usage: sudo sh setup-gkc.sh <-c|-d|-s> [-i Installation-directory] [-l Local-workspace-directory] [-u Unified-workspace-directory]"
	exit 1
fi

iSetupMode = -1
if [ $1 = "-c" ]; then
	iSetupMode = 0
elif [ $1 = "-d" ]; then
	iSetupMode = 1
elif [ $1 = "-s" ]; then
	iSetupMode = 2
else
	echo "Command error: unknown option!"
	exit 1
fi

strTargetDir = "/usr"
strLwsDir = "${strTargetDir}/.GKC/LWS"
strUwsDir = "${strTargetDir}/.GKC/UWS"
iIndex = 2
while [ $iIndex -le $# ];
do
	eval strTemp = "\$${iIndex}"
	iIndexNext = $(( $iIndex + 1 ))
	if [ ${strTemp} = "-i" ]; then
		if [ $iIndex -ne 2 ]; then
			echo "Command error: the installation directory should be specified at first!"
			exit 1
		fi
		if [ $iIndexNext -gt $# ]; then
			echo "Command error: the installation directory should be specified!"
			exit 1
		fi
		eval strTargetDir = "\$${iIndexNext}"
		strTargetDir = ${strTargetDir%*/}
		strLwsDir = "${strTargetDir}/.GKC/LWS"
		strUwsDir = "${strTargetDir}/.GKC/UWS"
	elif [ ${strTemp} = "-l" ]; then
		if [ $iIndexNext -gt $# ]; then
			echo "Command error: the local workspace directory should be specified!"
			exit 1
		fi
		eval strLwsDir = "\$${iIndexNext}"
	elif [ ${strTemp} = "-u" ]; then
		if [ $iIndexNext -gt $# ]; then
			echo "Command error: the unified workspace directory should be specified!"
			exit 1
		fi
		eval strUwsDir = "\$${iIndexNext}"
	else
		echo "Command error: unknown option for directory!"
		exit 1
	fi
	iIndex = $(( $iIndexNext + 1 ))
done

#install
echo "Install..."

if [ ! -x "${strPackageName}" ]; then
	echo "Error: The package file does not exist!"
	exit 1
fi

strDest = "${strTargetDir}/.GKC/SYSTEM"
if [ -d "${strDest}" ]; then
	echo "Error: The software is not uninstalled!"
	exit 1
fi

if eval [[ -n "\$${strEnvSystemRoot}" ]]; then
	echo "Error: The software is not uninstalled!"
	exit 1
fi
if eval [[ -n "\$${strEnvLws}" ]]; then
	echo "Error: The software is not uninstalled!"
	exit 1
fi
if eval [[ -n "\$${strEnvUws}" ]]; then
	echo "Error: The software is not uninstalled!"
	exit 1
fi

mkdir "${strTargetDir}/.GKC"
mkdir "${strDest}"
./${strPackageName} --prefix=${strDest} --exclude-subdir
sleep 0.5

if [ iSetupMode -eq 0 ]; then
	rm -r ${strDest}/core/services
	rm -r ${strDest}/development
elif [ iSetupMode -eq 1 ]; then
	rm -r ${strDest}/core/services
	rm -r ${strDest}/client
	rm -r -f ${strDest}/software
elif [ iSetupMode -eq 2 ]; then
	rm -r ${strDest}/core/tools
	rm -r ${strDest}/development
	rm -r ${strDest}/client
fi

if [ ! -d "${strLwsDir}" ]; then
	mkdir -p "${strLwsDir}"
fi
if [ ! -d "${strUwsDir}" ]; then
	mkdir -p "${strUwsDir}"
fi

#environment
strFile = "/etc/environment"
cat ${strFile} | while read strLine
do
	echo "${strLine}" >> ${strFile}_tmp
done
echo "${strEnvSystemRoot}=\"${strDest}\"" >> ${strFile}_tmp
echo "${strEnvLws}=\"${strLwsDir}\"" >> ${strFile}_tmp
echo "${strEnvUws}=\"${strUwsDir}\"" >> ${strFile}_tmp
rm ${strFile}
sleep 0.5
mv ${strFile}_tmp ${strFile}
source ${strFile}

echo "${strDest}/core/assemblies" > /etc/ld.so.conf.d/GKC.conf
ldconfig

strFile = "/etc/profile.d/GKC.sh"
strTemp = "export PATH=\$PATH"
if [ iSetupMode -ne 2 ]; then
	strTemp = "${strTemp}:${strDest}/core/tools"
fi
if [ iSetupMode -eq 0 ]; then
	strTemp = "${strTemp}:${strDest}/client"
fi
if [ iSetupMode -eq 1 ]; then
	strTemp = "${strTemp}:${strDest}/development/tools"
fi
echo "#!/bin/sh" > ${strFile}
echo "${strTemp}" >> ${strFile}
chmod ugoa+x ${strFile}
source /etc/profile

#services
if [ iSetupMode -eq 2 ]; then
	strTemp = "${strDest}/core/services/GkcHostSvc"
	sh "${strDest}/core/services/ghs-setup.sh" "${strTemp}"
	sleep 0.5
fi

#done
echo "Done"

#end
