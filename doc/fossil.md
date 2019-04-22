<!--
#
# Copyright (c) 2018, Xin YUAN, courses of Zhejiang University
# All rights reserved.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the 2-Clause BSD License.
#
# Author contact information:
#   yxxinyuan@zju.edu.cn
#
-->

# Fossil

## Compiling

Download the source code from `http://www.fossil-scm.org`.
Edit the file `autosetup/cc.tcl`, then find the section containing `-g -O2`
and remove the `-g` flag.

Compile the source code by the following commands:

```
./configure
make
```

Copy the executable file `fossil` to `/usr/bin`.

The source code can be compiled under `Cygwin`.
Select `autoconf`, `make`, `gcc`, `zlib-devel`, `openssl-devel`,
and `mingw64` libraries in `devel` catalog when installing `Cygwin`.
The generated exectuable file can run in `Cygwin` command window.

The DLL files of `Cygwin` runtime including cygcrypto-1.0.0.dll, cygssl-1.0.0.dll,
cygwin1.dll and cygz.dll should be copied to the same directory
containing the generated executable file `fossil.exe`,
then `fossil.exe` can run under Windows.

## Server

Establish the work directory:

```
sudo mkdir /home/fossil
sudo chmod -R go+rw /home/fossil
cd /home/fossil
```

Create a new repository:

```
fossil new [-A <admin-username>] <repo-name>.fossil
fossil setting autosync off -R <repo-name>.fossil
fossil setting crnl-glob *, -R <repo-name>.fossil
fossil setting encoding-glob *, -R <repo-name>.fossil
```

These are commands about user:

```
fossil user password <username> <password> -R <repo-name>.fossil (Change the password)
fossil user new <username> <contact-info> <password> -R <repo-name>.fossil
fossil user list -R <repo-name>.fossil
fossil user default <username> -R <repo-name>.fossil (Change the default user)
```

Change the capabilities for user:

```
fossil user capabilities <username> <capabilities> -R <repo-name>.fossil
```

All capabilities can be found [here](https://fossil-scm.org/fossil/setup_ucap_list "capabilities").

Four categories are "anonymous", "developer", "nobody" and "reader"
whose capabilities can be modified for special cases like private repositories.

The development team includes managers, programmers, testers and operators.

The manual for running Fossil as a CGI program can be found [here](http://www.fossil-scm.org/index.html/doc/trunk/www/server.wiki "CGI").

The manual for running Fossil as a SCGI program can be found
in [this page](http://www.fossil-scm.org/index.html/doc/trunk/www/server.wiki "CGI")
and in [that page](http://www.fossil-scm.org/index.html/doc/trunk/www/scgi.wiki "SCGI").

For example, add the following commands to the file `/etc/backend-rc.local` before `exit 0`:

```
#!/bin/sh -e
#
nohup fossil server --localhost --port 9000 --scgi /home/fossil &
```

and modify the Nginx configuration:

```
location ~ ^/fossil-repo/ {
    include scgi_params;
    scgi_pass localhost:9000;
    scgi_param SCRIPT_NAME "/fossil-repo";
}
```

Create a file `/etc/systemd/system/backend-rc.service` (using `\n`):

```
[Unit]
Description=/etc/backend-rc.local Compatibility
ConditionPathExists=/etc/backend-rc.local
After=network.target

[Service]
Type=forking
ExecStart=/etc/backend-rc.local start
TimeoutSec=0
StandardOutput=tty
RemainAfterExit=yes
SysVStartPriority=99

[Install]
WantedBy=multi-user.target
#Alias=backend-rc-local.service
```

Enable the service:

```
sudo systemctl enable backend-rc.service
```

## Client

Create a new repository in local machine:

```
fossil new [-A <admin-username>] <repo-name>.fossil
fossil setting autosync off -R <repo-name>.fossil
fossil setting crnl-glob *, -R <repo-name>.fossil
fossil setting encoding-glob *, -R <repo-name>.fossil
```

Visit the website for configuring the respository:

```
http(s)://<host>/fossil-repo/<repo-name>
```

Clone the repository:

```
fossil clone [-A <admin-username>] http(s)://<user>:<password>@<host>/fossil-repo/<repo-name> <repo-name>.fossil
fossil setting autosync off -R <repo-name>.fossil
fossil setting crnl-glob *, -R <repo-name>.fossil
fossil setting encoding-glob *, -R <repo-name>.fossil
fossil setting binary-glob *, -R <repo-name>.fossil
```

Open a repository:

```
fossil open <repo-name>.fossil
```

Close the repository:

```
fossil close
```

Check the extra files:

```
fossil extras [--dotfiles]
```

Check the changed files:

```
fossil changes [--dotfiles]
```

List the files:

```
fossil ls
```

Add the files or directories:

```
fossil add [--dotfiles] <FILE1> [<FILE2> ...]
fossil add [--dotfiles] .
```

Remove the files or directories:

```
fossil rm [--hard] <FILE1> [<FILE2> ...]
```

Rename the files or directories:

```
fossil mv [--hard] <OLDFILE> <NEWFILE>
fossil mv [--hard] <OLDFILE>... <DIR>
```

Show the difference between the current version of files
and that same file as it was checked out:

```
fossil diff [--tk] [<FILE1>] [<FILE2> ...]
```

The option `--tk` means a Tcl/Tk GUI for display. ActiveTcl can be used under Windows.

Commit the changes to a new version:

```
fossil commit -m <comment-message> [<FILE> ...]
```

List the timeline of current repository:

```
fossil timeline [-n <number>] (e.g., number =0, >0, <0)
```

Show the information about the object.

```
fossil info [<VERSION>]
```

Push the local changes to remote repository:

```
fossil push [<URL>]
```

Pull the changes from remote repository:

```
fossil pull [<URL>]
```

Update the local tree after pulling:

```
fossil update [<VERSION>]
```

Undo the changes to the working checkout caused by "update", "merge", "revert" and so on.

```
fossil undo
```

Revert the local files:

```
fossil revert [-r <REVISION>] [<FILE> ...]
```

These are branch commands:

```
fossil branch list
fossil branch current
fossil branch info <branch-name>
fossil branch new <branch-name> <BASIS> [--private]
```

Check out any version:

```
fossil checkout [<VERSION> | --latest] [--force]
```

Merge the version to the current checkout:

```
fossil merge [<VERSION>]
```

These are tag commands:

```
fossil tag add <TAG-NAME> <CHECK-IN> [<value>] (e.g., tag-value pairs: [tag:release-X.X][], ...)
fossil tag add --raw closed <CHECK-IN>
fossil tag list [<CHECK-IN>]
```

Open the web interface:

```
fossil ui
```

In web-browser, two versions which can be located on different branches
can be clicked for viewing the changes.
This function can be used as code-review.
Tickets are also edited in browser.

Another GUI program can be found [here](http://fuelscm.org/ "GUI").
