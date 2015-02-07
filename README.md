<!--
#
# Copyright (c) 2013, Xin YUAN, courses of Zhejiang University
# All rights reserved.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the 2-Clause BSD License.
#
# Author contact information:
#   yxxinyuan@zju.edu.cn
#
-->

# GKC

General Kind C++

## Production

Install the necessary softwares in the production machine.
Assume the building directory has the same level of directory GKC.

### Preparation

#### Windows

##### Compiler

Visual Studio Express 2013

##### Latex

Install MikTex, and use TexWorks. Install other packages through Maintenance UI such as CJK packages.
Two patterns, `pdfLaTeX+MakeIndex+BibTeX` and `XeLaTeX+MakeIndex+BibTeX` can be used for composing
papers, books and slides.

#### Linux

##### Compiler

g++-4.9

Install libuuid:
```
sudo apt-get install uuid-dev
```

##### Latex
1. Install TexLive:
	```
sudo apt-get install texlive-full
```

1. Install fonts:
	```
sudo apt-get install ttf-mscorefonts-installer
sudo apt-get install xfonts-wqy ttf-wqy-microhei ttf-wqy-zenhei
fc-cache
fc-list
```

1. Install TexStudio:

	Install TexStudio.

	Select the menu `Options->Configure Texstudio->Commands`,
change the item `XeLaTeX` to:
	```
xelatex -synctex=1 -interaction=nonstopmode %.tex
```

	Select the menu `Options->Configure Texstudio->Build`,
change the item `Default Compiler` to `XeLaTeX`.

### Testing

#### Windows

1. CMake:
	* x86 machine:
	```
cmake ../GKC -G"Visual Studio 12"
```

	* x64 machine:
	```
cmake ../GKC -DUSE_64BITS=1 -G"Visual Studio 12 Win64"
```

1. make:
	```
MSBuild ALL_BUILD.vcxproj /t:build /p:Configuration=Debug
```

1. test:
	```
cd test
ctest -C "Debug"
```

#### Linux

1. CMake:
	* x86 machine:
	```
cmake ../GKC -DCMAKE_BUILD_TYPE=Debug -G"Unix Makefiles"
```

	* x64 machine:
	```
cmake ../GKC -DCMAKE_BUILD_TYPE=Debug -DUSE_64BITS=1 -G"Unix Makefiles"
```

1. make:
	```
make
```

1. test:
	```
cd test
ctest
```

### Release

#### Windows

1. CMake:
	* x86 machine:
	```
cmake ../GKC -G"Visual Studio 12"
```

	* x64 machine:
	```
cmake ../GKC -DUSE_64BITS=1 -G"Visual Studio 12 Win64"
```

1. make:
	```
MSBuild ALL_BUILD.vcxproj /t:build /p:Configuration=Release
```

1. pack:
	```
cpack -D CPACK_GENERATOR="ZIP;TGZ"
```

	Compress the files, setup-gkc.vbs, uninstall-gkc.vbs and GKC-1.0.1-win32.zip (or GKC-1.0.1-win64.zip),
into a zip file and redistribute it.

#### Linux

1. CMake:
	* x86 machine:
	```
cmake ../GKC -DCMAKE_BUILD_TYPE=Release -G"Unix Makefiles"
```

	* x64 machine:
	```
cmake ../GKC -DCMAKE_BUILD_TYPE=Release -DUSE_64BITS=1 -G"Unix Makefiles"
```

1. make:
	```
make
```

1. pack:
	```
cpack
```

	Compress the files, setup-gkc.sh, uninstall-gkc.sh, GKC-1.0.1-Linux.sh and GKC-1.0.1-Linux.tar.gz,
into a zip file and redistribute it.

## Install

Install GKC in the computers on client end or server end for production and running infrastructure.

### Windows

```
cscript setup-gkc.vbs
```

### Linux

```
sudo sh setup-gkc.sh
```

## Uninstall

### Windows

```
cscript uninstall-gkc.vbs
```

### Linux

```
sudo sh uninstall-gkc.sh
```
