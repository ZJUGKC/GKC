<!--
#
# Copyright (c) 2020, Xin YUAN, courses of Zhejiang University
# All rights reserved.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the 2-Clause BSD License.
#
# Author contact information:
#   yxxinyuan@zju.edu.cn
#
-->

# Setup

The compiling destination of GKC project is a ZIP package with setup/uninstall scripts.
Use setup script with specified parameters to install the package,
and some environment variables are established.
Three environment variables indicate three directories which form a virtual file system.

| Directory | Description       |
|:----------|:------------------|
| /system   | system files      |
| /lws      | software packages |
| /uws      | user data (may be shared directory) |

Use `Wlang` compiling tools in command line or shell.
