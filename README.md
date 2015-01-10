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

## Compiler:

1. Windows

	Visual Studio Express 2013

1. Linux

	g++-4.9

## Building

Install the necessary softwares in the production machine.
Assume the building directory has the same level of directory GKC.

### Windows

1. CMake:
	* x86 machine:
	```
cmake ../GKC -G"Visual Studio 12"
```

	* x64 machine:
	```
cmake ../GKC -G"Visual Studio 12 Win64"
```

1. make:
	```
MSBuild ALL_BUILD.vcxproj /t:build /p:Configuration=Release
```

### Linux

1. Install libuuid:
	```
sudo apt-get install uuid-dev
```

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

## Latex

### Windows

Install MikTex, and use TexWorks. Install other packages through Maintenance UI such as CJK packages.
Two patterns, pdfLaTeX+MakeIndex+BibTeX and XeLaTeX+MakeIndex+BibTeX can be used for composing
papers, books and slides.

### Linux

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
	Install TexStudio. Select the menu `Options->Configure Texstudio->Commands`,
change the item `XeLaTeX` to:
	```
xelatex -synctex=1 -interaction=nonstopmode %.tex
```

	Select the menu `Options->Configure Texstudio->Build`,
change the item `Default Compiler` to `XeLaTeX`.
