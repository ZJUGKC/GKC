#!/bin/sh

# parameters: <workspace-path> <item-name> <project-file> [-l]

if [ $# -lt 3 -o $# -gt 4 ]; then
	echo "Usage: bash build-epub.sh <workspace-path> <item-name> <project-file> [-l]"
	exit 1
fi

strWorkspacePath=$1
strItem=$2
strProjectFile=$3

if [ -d "${strWorkspacePath}/$strItem" ]; then
	rm -r "${strWorkspacePath}/$strItem"
	sleep 0.05
fi
mkdir -p "${strWorkspacePath}/$strItem"
if [ $? -ne 0 ]; then
	echo "Error: cannot create directory."
	exit 2
fi

strScriptRoot=$(dirname $(readlink -f "$0"))
cd "$strScriptRoot"
cd ../bin
strBinPath=`pwd`

echo "*** generate ***"
# Do not use "$4" (quoted) when $4 is a null string
./mdp -e $4 "$strProjectFile" "${strWorkspacePath}/$strItem"
if [ $? -ne 0 ]; then
	echo "Error: mdp error."
	exit 3
fi
sleep 1

echo "*** execute ***"
export PATH="$PATH:$strBinPath"
cd "${strWorkspacePath}/$strItem/epub"
bash build.sh
if [ $? -ne 0 ]; then
	echo "Error: build error."
	exit 4
fi

exit 0
