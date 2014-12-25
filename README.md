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

2. Linux

	g++-4.9

## Building

Install the necessary softwares in the production machine.

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

2. make
	```
MSBuild ALL_BUILD.vcxproj /t:build /p:Configuration=Release
```

### Linux

1. Install libuuid:
	```
sudo apt-get install uuid-dev
```

2. CMake:
	* x86 machine:
	```
cmake ../GKC -DCMAKE_BUILD_TYPE=Release -G"Unix Makefiles"
```

	* x64 machine:
	```
cmake ../GKC -DCMAKE_BUILD_TYPE=Release -DUSE_64BITS=1 -G"Unix Makefiles"
```

3. make
	```
make
```
