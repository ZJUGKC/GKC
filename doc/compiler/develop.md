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

## Parser

1. Finite State Automata (FSA)

Implement a finite state automata named FiniteStateAutomata.
It uses the matching algorithm of maximum length.

1. Push-down Automata (PDA)

Implement a push-down automata named PushDownAutomata.
It uses the symbol stack to process states and terminals/nonterminals.

1. Parsers

Implement lexer parser and grammar parser.

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
| x | Visible ASCII character, 0x21-0x7E (remove space - 0x20) |
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
| R1`|`R2 | R1 or R2 |

1. Others:

| Symbols | Description |
|-----|-----|
| (R) | Grouped by R |
| {macro} | Quote the macro defined in definition block (it cannot be nested) |

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

The basic principles are shown as follows:

1. Match the string with the maximum length.
1. Match the first regular expression for conflicts.

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

1. Define grammar by left recursion as much as possible.
1. Use the first rule for reduction-reduction conflicts.
1. Use the shift operation for shift-reduction conflicts.
1. Do not use nullable nonterminals and nullable production rules.
1. Do not design ambiguous grammar, i.e., the nonterminal should not appear two or more times in right-hand side.
1. The priority order is from top to bottom with low level to high level.
1. The first nonterminal in the left-hand side of the first production rule is considered as the start nonterminal.
The PDA generator would add a special production rule `S->start_nonterminal $` implicitly.

The grammar file for WLANG is named as `wlang.gra`.

## Analyze the Language Definition File

The lexical elements of lex and grammar files can be described as the following lex file:

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
TK_COLON    :
TK_VERT     \|
TK_SEMI     ;
```

The corresponding FSA tables are shown in `ldf-fsa.odg` and `_ldf_base.h`.

The syntax of lex file can be described as the following grammar file:

```
%%
lex_def : TK_SEP rule_block  { do_def }
	;
rule_block : rule_block id { do_rule_block_id }
	| id { do_rule_id }
	;
id : TK_TOKEN  { do_id_token }
	| TK_MACRO  { do_id_macro }
	;
```

The corresponding PDA tables are shown in `ldf-lex-pda.odg` and `_ldf_base.h`.

The lexical elements of regular expression can be described as the following lex file:

```
%%
TK_CHAR_BACKSLASH       \\\\
TK_CHAR_HEX             \\x[0-9A-Fa-f][0-9A-Fa-f]
TK_CHAR_CR              \\r
TK_CHAR_LN              \\n
TK_CHAR_TAB             \\t
TK_CHAR_SPACE           \\s
TK_CHAR_LBRACKET        \\\[
TK_CHAR_RBRACKET        \\\]
TK_CHAR_LPAREN          \\\(
TK_CHAR_RPAREN          \\\)
TK_CHAR_LCURLY          \\\{
TK_CHAR_RCURLY          \\\}
TK_CHAR_QUESTION        \\\?
TK_CHAR_STAR            \\\*
TK_CHAR_PLUS            \\\+
TK_CHAR_MINUS           \\\-
TK_CHAR_UPARROW         \\\^
TK_CHAR_VERT            \\\|
TK_LBRACKET             \[
TK_RBRACKET             \]
TK_MINUS                \-
TK_UPARROW              \^
TK_QUESTION             \?
TK_STAR                 \*
TK_PLUS                 \+
TK_VERT                 \|
TK_LPAREN               \(
TK_RPAREN               \)
TK_CHAR                 [\x21-\x7E]
```

The corresponding FSA tables are shown in `ldf-regex-fsa.odg` and `_ldf_base.h`.

The syntax of regular expression can be described as the following grammar file:

```
%%
regex_exp : regex_exp TK_VERT regex_term  { do_exp_exp_term }
	| regex_term  { do_exp_term }
	;
regex_term : regex_term regex_factor_1  { do_term_term_factor_1 }
	| regex_factor_1  { do_term_factor_1 }
	;
regex_factor_1 : regex_factor_1 TK_PLUS  { do_factor_1_plus }
	| regex_factor_1 TK_STAR  { do_factor_1_star }
	| regex_factor_1 TK_QUESTION  { do_factor_1_question }
	| regex_factor  { do_factor_1_factor }
	;
regex_factor : TK_LPAREN regex_exp TK_RPAREN  { do_factor_paren_exp }
	| regex_char  { do_factor_char }
	| regex_char_set  { do_factor_char_set }
	;
regex_char_set : TK_LBRACKET regex_char_item TK_RLBRACKET  { do_char_set }
	| TK_LBRACKET TK_UPARROW regex_char_item TK_RLBRACKET  { do_char_set_up }
	;
regex_char_item : regex_char_item regex_char_e  { do_char_item_item_char_e }
	| regex_char_e  { do_char_item_char_e }
	;
regex_char_e : regex_char_range  { do_char_e_range }
	| regex_char  { do_char_e_char }
	;
regex_char_range : regex_char TK_MINUS regex_char_s  { do_char_range }
	;
regex_char_s : regex_char  { do_char_s }
	;
regex_char : TK_CHAR  { do_char }
	;
```

The corresponding PDA tables are shown in `ldf-regex-pda.odg` and `_ldf_base.h`.

The syntax of grammar file can be described as the following grammar file:

```
%%
gra_def : TK_SEP rule_block  { do_ref }
	;
rule_block : rule_block rule  { do_rule_block_rule }
	| rule  { do_rule }
	;
rule : TK_MACRO rule_right_block TK_SEMI  { do_rule_right_block }
	;
rule_right_block : rule_right_block TK_VERT rule_right  { do_right_block }
	| rule_right  { do_right }
	;
rule_right : rule_right id  { do_right_id }
	| id  { do_id }
	;
id : TK_TOKEN  { do_id_token }
	| TK_MACRO  { do_id_macro }
	;
```

The corresponding PDA tables are shown in `ldf-gra-pda.odg` and `_ldf_base.h`.
