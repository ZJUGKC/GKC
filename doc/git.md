<!--
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
-->

# Set the git client for windows using GitHub

Install the GitHub for windows from [GitHub](https://github.com "GitHub"),
with the typical folder
```
C:\Users\<USER>\AppData\Local\GitHub
```

noted as `<GitHubInstallDir>`.

1. GIT BASH

	Create a shortcut on desktop with the name `GIT BASH`.
Right-click the shortcut and select property in popup menu,
then edit the destination as:
	```
"<GitHubInstallDir>\PortableGit_<LatestVersionNumber>\bin\sh.exe" --login –i
```

	The `<LatestVersionNumber>` should be found in `<GitHubInstallDir>`.

	The start location should be written as:
	```
"<GitHubInstallDir>\PortableGit_<LatestVersionNumber>\bin"
```

	Save the shortcut.

1. GIT GUI

	Create a shortcut on desktop with the name `GIT GUI`.
Right-click the shortcut and select property in popup menu,
then edit the destination as:
	```
"<GitHubInstallDir>\PortableGit_<LatestVersionNumber>\bin\wish.exe" "<GitHubInstallDir>\PortableGit_<LatestVersionNumber>\libexec\git-core\git-gui"
```

	The start location should be written as:
	```
"<GitHubInstallDir>\PortableGit_<LatestVersionNumber>\bin"
```

	Save the shortcut.
