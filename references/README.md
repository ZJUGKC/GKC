
1. Install GTK+3:

	```
	sudo apt-get install libgtk-3-dev
	sudo apt-get install pkg-config
	```

### Testing

#### Windows

1. CMake:
...
	* x64 machine:

		```
		cmake ../GKC -DUSE_64BITS=1 -G "Visual Studio 17 2022" -A x64
		```

#### Linux

1. CMake:
...
	* x86 machine:

		```
		cmake ../GKC -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles"
		```

	* x64 machine:

		```
		cmake ../GKC -DCMAKE_BUILD_TYPE=Debug -DUSE_64BITS=1 -G "Unix Makefiles"
		```

### Release

#### Windows

1. CMake:
...
	* x64 machine:

		```
		cmake ../GKC -DUSE_64BITS=1 -G "Visual Studio 17 2022" -A x64
		```

#### Linux

1. CMake:
...
	* x86 machine:

		```
		cmake ../GKC -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles"
		```

	* x64 machine:

		```
		cmake ../GKC -DCMAKE_BUILD_TYPE=Release -DUSE_64BITS=1 -G "Unix Makefiles"
		```

## Install

Install GKC in the computers on client end or server end for production and running infrastructure.

* Windows

	Run the following command as administrator:

	```
	cscript setup-gkc.vbs <-c|-d|-s> [-i Installation-directory] [-l Local-workspace-directory] [-u Unified-workspace-directory] [-si <user:pwd>]
	```

* Linux

	```
	sudo bash setup-gkc.sh <-c|-d|-s> [-i Installation-directory] [-l Local-workspace-directory] [-u Unified-workspace-directory]
	```

The options are listed as follows:

| Options | Notes |
|:-------:|:------|
| -c      | This software is installed as client on client end |
| -d      | This software is installed as development on production machine |
| -s      | This software is installed as server on server end |

The user which must be an administrator is used for connecting UWS.

The directories are listed as follows:

| Directories | Notes |
|:------------|:------|
| Installation-directory      | Specify the installation directory |
| Local-workspace-directory   | Specify the directory of local workspace |
| Unified-workspace-directory | Specify the directory of unified workspace |

## Uninstall

* Windows

	Run the following command as administrator:

	```
	cscript uninstall-gkc.vbs [-a]
	```

* Linux

	```
	sudo bash uninstall-gkc.sh [-a]
	```

`-a` means the directories for local workspace and unified workspace will be deleted.
