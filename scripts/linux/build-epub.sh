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
	sleep 1
fi
mkdir -p "${strWorkspacePath}/$strItem"

strScriptRoot=$(dirname $(readlink -f "$0"))
cd "$strScriptRoot"
cd ../bin

echo "*** generate ***"
./mdp -e "$4" "$strProjectFile" "${strWorkspacePath}/$strItem"
if [ $? -ne 0 ]; then
	echo "Error: mdp error."
	exit 2
fi
sleep 2

echo "*** execute ***"
cd "${strWorkspacePath}/$strItem/epub"
bash build.sh
if [ $? -ne 0 ]; then
	echo "Error: build error."
	exit 3
fi
sleep 2

exit 0
