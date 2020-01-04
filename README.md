# AttributeParser

## Description
Parses HTML-like syntactic tags and attributes into in-memory data structures to assist with querying and editing.
Initially, this project was developed in Microsoft Visual Studio Community 2017, will port to Linux (soon) and add the appropriate Makefiles.
Inspired originally by HackerRank problem: https://www.hackerrank.com/challenges/attribute-parser/problem.

The parser currently reads from the file specified in `main()` which should contain tags. These tags can be nested within other tags, and multiple tags may be placed on the same level. Each tag *MUST* be on a separate line, same for tag endings. Example:

Correct :heavy_check_mark:
```
<tag1>
</tag1>
```
Incorrect :x:
```
<tag1></tag1>
```

Currently, support for self-contained tags (i.e. `<tag1/>`) is not finished.

The tags' format is specified as follows:
- `<tagName>` if the tag has no attributes
- `<tagName attributeName = "attributeValue">` if the tag has attributes
    - Whitespace space must appear before and after the `=`, and the closing `>` must be directly following the final attribute.

**NOTE**: This does NOT support true HTML yet, much work has to be done to support even basic syntax.
