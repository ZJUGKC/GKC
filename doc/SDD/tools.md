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

## Basic Structures

All Software Development Documents (SDD) should be located in a directory called *document directory*.
All documents use markdown format saved as UTF8 encoding with BOM.
A directory contains several markdown files and sub-directories.
If the directory is named as `<md-filename>-files`, it will only contain the accessory files (e.g., image files) for the markdown file `<md-filename>.md`.
One of the markdown files under the directory should be used as *summary file* for that directory.
The directory named as `files` under the *document directory* contains the accessory files for the entire documents.
All directories and files form a tree structure and should be named in English without spaces.

The image formats can be JPEG, PNG and GIF.

## Special Files

These four types of files under the *document directory* are listed as follows:

| Type      | Description  |
|:---------:|:-------------|
| Cover     | Image file   |
| Title     | Mardown file |
| Content   | Mardown file, can use *summary file* |
| Copyright | Mardown file |

## Project File

A project file with postfix `.mdp` is located in a directory which contains the *document directory*.
The project file uses WON format, and the whole file is an object containing the following key-value pairs:

| Key                  | Value                                            |
|:--------------------:|:-------------------------------------------------|
| "project-name"       | Specify the project name                         |
| "document-directory" | Specify the *document directory*                 |
| "language"           | Specify the language ID                          |
| "command"            | Specify the compiling commands for markdown file |
| "cover"              | Specify the cover image file                     |
| "cover-name"         | Specify the cover page name                      |
| "tree"               | Specify the file tree                            |

The optional key-value pairs are listed as follows:

| Key           | Value                        |
|:-------------:|:-----------------------------|
| "topic"       | Specify the topic            |
| "author"      | Specify the author           |
| "description" | Specify the description      |
| "date"        | Specify the date             |
| "contributor" | Specify the contributor      |
| "publisher"   | Specify the publisher        |
| "subject"     | Specify the subject          |
| "rights"      | Specify the rights           |
| "identifier"  | Specify the identifier       |

The file tree is an array with the object elements. Each object contains the following key-value pairs:

| Key    | Value                                                  |
|:------:|:-------------------------------------------------------|
| "name" | Specify the node name                                  |
| "file" | Specify the file name, which can use anchor optionally |

The optional key-value pair of file tree is `"tree"` and the corresponding file tree of the sub-level.

Another optional key-value pair of file tree is `"guide"`
and the value listed as follows:

| Value         | Description                       |
|:-------------:|:----------------------------------|
| "title"       | Specify the title page file       |
| "content"     | Specify the content file          |
| "copyright"   | Specify the copyright page file   |

The language IDs are listed as follows:

| ID   | Hexadecimal Number | Description             | Code Page     | Short String | Charset     |
|:----:|:------------------:|:------------------------|:-------------:|:------------:|:------------|
| 1033 | 0x409              | English (United States) | CODEPAGE_1252 | en-US        | US-ASCII    |
| 1025 | 0x401              | Arabic                  | CODEPAGE_1256 | ar-SA        | Arabic      |
| 2052 | 0x804              | Chinese (Simplified)    | CODEPAGE_936  | zh-CN        | GB2312      |
| 1028 | 0x404              | Chinese (Traditional)   | CODEPAGE_950  | zh-TW        | Big5        |
| 1029 | 0x405              | Czech                   | CODEPAGE_1250 | cs-CZ        | ISO-8859-2  |
| 1032 | 0x408              | Greek                   | CODEPAGE_1253 | el-GR        | Greek       |
| 1037 | 0x40D              | Hebrew                  | CODEPAGE_1255 | he-IL        | Hebrew      |
| 1038 | 0x40E              | Hungarian               | CODEPAGE_1250 | hu-HU        | ISO-8859-2  |
| 1041 | 0x411              | Japanese                | CODEPAGE_932  | ja-JP        | Shift-JIS   |
| 1042 | 0x412              | Korean                  | CODEPAGE_949  | ko-KR        | ISO-2022-KR |
| 1045 | 0x415              | Polish                  | CODEPAGE_1250 | pl-PL        | ISO-8859-2  |
| 1049 | 0x419              | Russian                 | CODEPAGE_1251 | ru-RU        | ISO-8859-5  |
| 1051 | 0x41B              | Slovakian               | CODEPAGE_1250 | sk-SK        | ISO-8859-2  |
| 1060 | 0x424              | Slovenian               | CODEPAGE_1250 | sl-SI        | ISO-8859-2  |
| 1055 | 0x41F              | Turkish                 | CODEPAGE_1254 | tr-TR        | ISO-8859-9  |
| 1026 | 0x402              | Bulgarian               | CODEPAGE_1251 | bg-BG        | ISO-8859-5  |

The compiling commands is an array containing one or more command strings for processing the markdown file.
The macro variables, `$SRC$` and `$DEST$`, can be used in command string for indicating the source file and the destination file.
The macro variable `$DEL$` is used as the delete command for windows and linux.
The macro variables cannot be surrounded by quotation marks.

# Viewer

* Windows

	A `.chm` file can be opened directly under Windows.
	A tool called SumatraPDF (https://github.com/sumatrapdfreader/sumatrapdf) can be used
	for browsing `chm` files and `epub` files.

* Linux

	A tool called `KchmViewer` can be used for browsing `chm` files and `epub` files.
	Use the following command to install it:

	```
	sudo apt-get install kchmviewer
	```

# Processing Tools

## Project

Execute the command as follows:

```
mdp <options> <project file> <output directory>
```

The options are listed as follows:

| Options | Description                  |
|:-------:|:-----------------------------|
| -m      | Generate the script for chm  |
| -e      | Generate the script for epub |

## Compilers

There are several compilers used for processing markdown files.
`Multi-Markdown` and `CMark` are two command-line tools.

### Multi-Markdown

Download the software from this [page](http://fletcherpenney.net/multimarkdown "mmd")
and use it under Windows:

```
multimarkdown -o <output file> <source file>
```

### CMark

The kernel tool of `commonmark` is CMark (https://github.com/jgm/cmark).
Download it and compile it under Windows and Linux.

* Windows

	Copy `cmark.dll` to `%GKC_SYSTEM_ROOT%\core\assemblies`,
	and copy `cmark.exe` to `%GKC_SYSTEM_ROOT%\core\tools`.

* Linux

	Copy `libcmark.so` and `libcmark.so.0.XX.X` to `${GKC_SYSTEM_ROOT}/core/assemblies` with option `-d`,
	and copy `cmark` to `${GKC_SYSTEM_ROOT}/core/tools`.

Execute the following command:

```
cmark <source file> -t html > <output file>
```

## Generator

### CHM

* Windows

	1. Install `HTML Help Workshop`, execute `hhc XXX.hhp`.

	1. Double-click `XXX.chm` and view it.

* Linux

	1. Download Free Pascal Compiler (https://www.freepascal.org/) and install it.

	1. Execute:

		```
		chmcmd XXX.hhp
		```

	1. Install the package `kchmviewer` and view `XXX.chm`.

### EPUB

* Windows

	Install 7-zip tool, and execute:

	```
	7z a -mx0 -tzip <output file> mimetype
	7z a -mx9 -r -tzip <output file> META-INF OEBPS
	```

* Linux

	Install zip tool, and execute:

	```
	zip -0Xq <output file> mimetype
	zip -Xr9Dq <output file> META-INF OEBPS
	```
