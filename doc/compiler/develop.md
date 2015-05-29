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

# The development process

## Finite State Automata (FSA)

Implement a finite state automata named FiniteStateAutomata.
It uses the matching algorithm of maximum length.

## Language Definition File

The definition of language is written in a lex file and a grammar file.

A lex file consists of only one section as follows:

```
%%
Definition & Rule Block
```

For example,

```
%%
TK_INTEGER [0-9]+
TK_OTHER   [^\n]
```

The syntax of regular expression is shown in the following tables:

1. The basic operations:

| Symbols | Description |
|-----|-----|
| x | Visible ASCII character, 0x20-0x7E |
| \x | Escape character |
| [xyz...] or [...x-z...] or [^...] | Define a character set |

1. Repeat operators (high priority level, the priority order is from top to bottom):

| Symbols | Description |
|-----|-----|
| R? | zero or one R |
| R* | zero or more R |
| R+ | one or more R |

1. Combination operators (low priority level, the priority order is from top to bottom):

| Symbols | Description |
|-----|-----|
| R1R2 |  R1 followed by R2 |
| R1|R2 | R1 or R2 |

1. Others:

| Symbols | Description |
|-----|-----|
| (R) | Grouped by R |
| {macro} | Quote the macro defined in definition block |

1. Escape characters:

| Symbols | Description |
|-----|-----|
| \\ | '\' |
| \xDD | Hexadecimal number |
| \r | Carriage return |
| \n | Line break |
| \t | Tab |
| \s | Space |
| \[, \] | Brackets |
| \(, \) | Parentheses |
| \{, \} | Curly braces |
| \?, \*, \+, \- | Question, Star, Plus, Minus |
| \^, `\|` | UpArrow, Vertical Line |

The lex file for WLANG is named as `wlang.lex`.

A grammar file has only one section as follows:

```
%%
Rule Block
```

For example,

```
%%
line : expr TK_RETURN  { do_line_expr }
	;
expr : expr TK_PLUS term  { do_expr_expr_plus_term }
	| term  { do_expr_term }
	;
term : term TK_STAR factor  { do_term_term_star_factor }
	| factor { do_term_factor }
	;
factor : TK_LPAREN expr TK_RPAREN  { do_factor_expr }
	| TK_DIGIT { do_factor_digit }
	;
```

The basic principles are shown as follows:

1. Define grammar by left recursion.
1. Use the first rule for reduction-reduction conflicts.
1. Use the shift operation for shift-reduction conflicts.

The grammar file for WLANG is named as `wlang.gra`.

## Analyze the Language Definition File

A lex file can be described as the following lex and grammar files:

```
%%
TK_COMMENT_START  /\*
TK_SPACE    [\s\t]+
TK_RETURN   \r|\n|\r\n
TK_SEP      %%
TK_ACTION   do_[a-z]([a-z_]|[0-9])*
TK_MACRO    [a-z]([a-z_]|[0-9])*
TK_TOKEN    TK_[A-Z]([A-Z_]|[0-9])*
TK_LCURLY   \{
TK_RCURLY   \}
```

```
%%
lex_def : TK_COMMENT_START rule_block TK_SEP action_block  { do_comment_def }
	| rule_block TK_SEP action_block  { do_def }
	;
rule_block : rule_block TK_TOKEN  { do_rule_block_token }
	| TK_TOKEN  { do_rule_token }
	;
action_block : action_block action_item  { do_action_block_item }
	| action_item  { do_action_item }
	;
action_item : TK_TOKEN TK_LCURLY TK_ACTION TK_RCURLY  { do_item }
	;
```

The corresponding FSA tables are shown in `lex-parser.odg`.
