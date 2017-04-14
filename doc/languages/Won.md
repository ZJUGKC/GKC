<!--
#
# Copyright (c) 2017, Xin YUAN, courses of Zhejiang University
# All rights reserved.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the 2-Clause BSD License.
#
# Author contact information:
#   yxxinyuan@zju.edu.cn
#
-->

# Introduction

Won (Wlang Object Notation) language is a subset of WLang language like JSON.

# Syntax

A won file can include comments surrounded by `/*` and `*/`.

The whole won file represents a `value` with the following types:

1. `string`

	A string contains zero or more characters between a pair of quotation marks (`"`).

	A character can be any UNICODE character except `"` or `\` or control character.

	The escape characters are listed as follows:

	| `\"`          | quotation mark            |
	|  \\           | reverse solidus           |
	|  \/           | solidus                   |
	|  \b           | backspace                 |
	|  \f           | formfeed                  |
	|  \n           | newline                   |
	|  \r           | carriage return           |
	|  \t           | horizontal tab            |
	|  \uDD, \uDDDD | 2 or 4 hexadecimal digits |

1. `number`

	A number contains integer part and optional decimal part and exponent part like C.

	The integer part can include a sign.

	A hexadecimal interger starts with two characters `0x` followed by 2, 4, 6, 8, 10, 12, 14, 16 hexadecimal digits.

1. `object`

	An object is an unordered set of name/value pairs.
	An object begins with `{` (left brace) and ends with `}` (right brace).
	Each name is followed by `:` (colon) and the name/value pairs are separated by `,` (comma).

1. `array`

	An array is an ordered collection of values.
	An array begins with `[` (left bracket) and ends with `]` (right bracket).
	Values are separated by `,` (comma).

1. Boolean

	Two keyword `true` and `false` represent the boolean values.

1. Null

	The keyword `null` represents the null object.

# Example

```
{
	"name" : "John",
	"age" : 10,
	"run" : [8.0, 7.8, 9.0, 11.2, 9.1],
	"height" : 153.2,
	"score" : 
	{
		"Math" : 100,
		"English" : 99,
		"Physics" : 70,
		"Chemistry" : 90,
		"History" :78
	},
	"payment" : true
}
```
