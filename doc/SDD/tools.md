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

# Specification

All Software Development Documents (SDD) should located in a directory.
All documents use markdown format saved as UTF8 encoding with BOM.
A directory has several sub-directories, several markdown files and other files (e.g., image files).
All directories and files form a tree structure.
Each directory has a file named `index.md` as the default page for this directory.
If a directory does not including that file, the directory will not appear in the tree of final chm book.
The names of directories and files can use a prefix with the format `XXX-`, where `XXX` is a number with leading zeros which will be removed in final chm book.

# Kernel Tool

The kernel tool is CMark (https://github.com/jgm/cmark).
Download it and compile it under Windows and Linux.

# Windows

1. Copy `cmark.exe` and `cmark.dll` to `c:\program files\.GKC\GKC-X.X.X-winXX\public\assemblies`.

1. Execute:

```
mdc <source directory> <output directory>
```

1. Install `HTML Help Workshop`, execute `hhc XXX.hhp`.

# Linux

1. Copy `cmark`, `libcmark.so` and `libcmark.so.0.22.0` to `/usr/.GKC/public/assemblies`.

1. Execute:

```
mdc <source directory> <output directory>
```

1. Download chmc (http://sourceforge.net/projects/chmc/) and compile it. Execute:

```
chmc -c XXX.conf -o XXX.chm <source directory>
```
