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

# Set the configuration for SciTE

Edit the file `SciTEGlobal.properties`:

1. Change `split.vertical=1` to `split.vertical=0`.

1. Cancel comment of `statusbar.visible=1`.

1. Cancel comment of `line.margin.visible=1`.

1. Cancel comment of `load.on.activate=1`.

1. Cancel comment of `are.you.sure.on.reload=1`.

1. Change `tabsize=8` to `tabsize=4`.

1. Change `indent.size=8` to `indent.size=4`.

1. Change `find.files=*.c *.cxx *.h` to `find.files=*.c *.cxx *.h *.cpp *.hpp *.hxx *.cc`.

1. Change `code.page=0` to `code.page=936`,
and add `character.set=134` for chinese input.

1. Change the value of `font.base` and so on to `$(font.monospace)` in section PLAT_WIN.

1. Change the size of font monospace to 11.
