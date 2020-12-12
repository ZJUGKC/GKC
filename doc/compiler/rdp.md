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

# Recursive Descent Parser

## Parser

1. Scanner
1. Push-down parser

## Language Definition File

A scanner file contains two sections as follows:

```
%%
zero or more TK_*
%%
zero or more ACT_*
```

A grammar file contains only one section as follows:

```
%%
Rule Block
```

For example:

```
%%
program : { ACT_program } line TK_RETURN { ACT_RETURN }
	| { ACT_program_SEP } TK_SEP expr
	;
expr : { ACT_expr } epsilon
	;
```
