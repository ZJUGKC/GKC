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
