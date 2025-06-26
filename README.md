# SWG - Simple Wordlist Generator

![screenshot](https://github.com/sleepymincy/sleepymincy/blob/main/.gitfiles/repos/images/swg.gif)

Simple, fast and NOT memory safe wordlist generator!~

## Quick Links:
- [Quick start](#quick-start)
- [Help / Example usage](#help-example--usage)
- [License information](#license-information)

## Quick start:

How to install:
- `git clone https://github.com/sleepymincy/swg`
- `cd swg && sudo make install`

How to uninstall:
- `sudo make uninstall && sudo make clean` (from cloned directory)

## Help / Example usage:
```
Usage: swg [args]

Required arguments (either/or, but not both):
  -w, --words    WORD1,WORD2     Read comma-separated list of words
  -f, --file     PATH            Read one word per line from file

Optional arguments:
  -o, --output   PATH            Write the wordlist to specified PATH, otherwise will output to STDOUT
  -d, --depth    NUM             Custom depth of a wordlist (how long the generated passwords should be)
  -q, --quiet                    Aside from critical errors, informational messages won't be printed
  -e, --estimate                 Evaluate the amount of lines you'll generate, without running the code
  -h, --help                     Show this help message
  -v, --version                  Show current version
```

- `swg -w 1,2,3,4,5` will generate:
```
1
2
...
55554
55555
```
- `swg -w foo,bar,baz,qux,quux -d 3` will generate:
```
foo
bar
baz
qux
quux
foofoo
foobar
foobaz
...
quuxquuxbaz
quuxquuxqux
quuxquuxquux
```

## License information:
This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <https://unlicense.org>
