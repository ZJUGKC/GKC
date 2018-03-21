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

# Comment

The comment begins with `<!--` and ends with `-->`.

# Special Characters

1. Copyright

	If you want to include a copyright symbol in your article, you can write `&copy;`.

1. `<`, `>` and `&`

	They would be translated to `&lt;`, `&gt;` and `&amp;`.

# Paragrahs

A paragraph is simply one or more consecutive lines of text, separated by one or more blank lines.
A blank line is any line that looks like a blank line - a line containing nothing but spaces or tabs is considered blank.
Normal paragraphs should not be indented with spaces or tabs.

When you do want to insert a `<br />` break tag using Markdown, you end a line with two or more spaces, then type return.

# Headers

Atx-style headers use 1-6 hash characters at the start of the line, corresponding to header levels 1-6.

# Blockquotes

Markdown uses email-style `>` characters for blockquoting.
It looks best if you hard wrap the text and put a `>` before every line or before the first line of a hard-wrapped paragraph.
Blockquotes can be nested (i.e. a blockquote-in-a-blockquote) by adding additional levels of `>`.
Blockquotes can contain other Markdown elements, including headers, lists, and code blocks.
(Translated to `<blockquote><p>...</p></blockquote>`)

# Lists

Unordered lists use asterisks, pluses, and hyphens - interchangably - as list markers (translated to `<ul><li>...</li>...</ul>`).
Ordered lists use numbers followed by periods (translated to `<ol><li>...</li>...</ol>`).
List markers typically start at the left margin, but may be indented by up to three spaces. List markers must be followed by one or more spaces or a tab.
If list items are separated by blank lines, Markdown will wrap the items in `<p>` tags in the HTML output.
List items may consist of multiple paragraphs. Each subsequent paragraph in a list item must be indented by either 4 spaces or one tab.
To put a blockquote within a list item, the blockquote's `>` delimiters need to be indented.
To put a code block within a list item, the code block needs to be indented twice - 8 spaces or two tabs.
You can backslash-escape the period.

# Code Blocks

To produce a code block in Markdown, simply indent every line of the block by at least 4 spaces or 1 tab (`<pre><code>...</code></pre>`).
One level of indentation - 4 spaces or 1 tab - is removed from each line of the code block.
A code block continues until it reaches a line that is not indented (or the end of the article).

# Horizontal Rules

You can produce a horizontal rule tag (`<hr />`) by placing three or more hyphens, asterisks, or underscores on a line by themselves.
If you wish, you may use spaces between the hyphens or asterisks.

# Links

The link text is delimited by [square brackets].
To create an inline link, use a set of regular parentheses immediately after the link text's closing square bracket. Inside the parentheses, put the URL where you want the link to point, along with an optional title for the link, surrounded in quotes.
Reference-style links use a second set of square brackets, inside which you place a label of your choosing to identify the link.
You can optionally use a space to separate the sets of brackets.
Then, anywhere in the document, you define your link label like this, on a line by itself.
That is:

1. Square brackets containing the link identifier (optionally indented from the left margin using up to three spaces);
1. followed by a colon;
1. followed by one or more spaces (or tabs);
1. followed by the URL for the link;
1. optionally followed by a title attribute for the link, enclosed in double or single quotes, or enclosed in parentheses.

You can put the title attribute on the next line and use extra spaces or tabs for padding, which tends to look better with longer URLs.
Link definition names may consist of letters, numbers, spaces, and punctuation - but they are not case sensitive.
The implicit link name shortcut allows you to omit the name of the link, in which case the link text itself is used as the name. Just use an empty set of square brackets.
Link definitions can be placed anywhere in your Markdown document.

# Emphasis

Markdown treats asterisks (*) and underscores (_) as indicators of emphasis. Text wrapped with one * or _ will be wrapped with an HTML `<em>` tag; double *'s or _'s will be wrapped with an HTML `<strong>` tag.
But if you surround an * or _ with spaces, it'll be treated as a literal asterisk or underscore.
To produce a literal asterisk or underscore at a position where it would otherwise be used as an emphasis delimiter, you can backslash escape it.

# Code

To indicate a span of code, wrap it with backtick quotes (\`) (translated to `<code>...</code>`).
To include a literal backtick character within a code span, you can use multiple backticks as the opening and closing delimiters.
The backtick delimiters surrounding a code span may include spaces - one after the opening, one before the closing. This allows you to place literal backtick characters at the beginning or end of a code span.
Write codes between two \`\`\` lines.

# Images

Inline image syntax:

1. An exclamation mark: !;
1. followed by a set of square brackets, containing the alt attribute text for the image;
1. followed by a set of parentheses, containing the URL or path to the image, and an optional title attribute enclosed in double or single quotes.

Reference-style image syntax, [id]
Where "id" is the name of a defined image reference. Image references are defined using syntax identical to link references.

# Backslash Escapes

Markdown provides backslash escapes for the following characters:

```
\   backslash
`   backtick
*   asterisk
_   underscore
{}  curly braces
[]  square brackets
()  parentheses
#   hash mark
+   plus sign
-   minus sign (hyphen)
.   dot
!   exclamation mark
```

# Footnode

```
hello [^he]

[^he]: he
```

# Deleted line

```
~~...~~
```

# Formula

```
$ ... $
$$ ... $$
```

# Tables

```
| ... | ... |...|...|
|---|:---|:---:|---:|
| ... | ... |...|...|

|| ... || ... ||...||...||
||---||:---||:---:||---:||
|| ... || ... ||...||...||
```

# Specification

* CommonMark (http://commonmark.org)
* MultiMarkdown (http://fletcherpenney.net/multimarkdown)
