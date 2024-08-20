## Pathfinder
Program that filters and scores paths based on expression provided by user.
Built to try out TDD. Primarly designed for combining with cd for quick system naviagtion.

### Usage
```
Usage: pf [OPTION]... [EXPR]...
Find path(s) best matching EXPR using substring matches.
General options:
-h, --help          Show help
-v, --verbose       Print errors to stderr
-m, --max-matches   Number of matches to print (default 9)
-r, --reverse       Reverse display order
-i, --non-interactive Print  paths immediatly, don't wait for user choice
Search Options:
--ignore-dotfiles   Skip directories and symlinks beginning with "."
-M, --max-depth     Max depth to search down the tree (default 5)
```

### Recommended function to put in .bashrc
```
cdd() {
  local path="$(pf "$1")"
  if [ -n "$path" ]; then
    cd "$path"
  fi
}
```

### Installation instructions
```
make && sudo make install
```

### Dependencies
Tests require cmocka.
Release build requires only stdlib.
Tested with GCC.
