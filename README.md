## Pathfinder
Program that filters and scores paths based on expression provided by user.
Built to try out TDD. Primarly designed for combining with cd for quick system naviagtion.

### Usage
```
  Find path(s) best matching EXPR using substring matches.

  General options:
  -h, --help            Show help
  -v, --verbose         Print errors to stderr
  -m, --max-matches     Number of matches to print (default 9)
  -r, --reverse         Reverse display order
  -i, --non-interactive Print  paths immediatly, don't wait for user choice

  Search Options:
  -I, --ignore-dotfiles   Skip directories and symlinks beginning with "."
  -M, --max-depth         Max depth to search down the tree (default 5)
  -t, --types             Types of filesystem object considered when
                          searching, represented by by string of letters
                          (a-all, d-dir, f-file, l-link, D-link to dir, F-link to file)
                          (default dfl)
```

### Recommended function to put in .bashrc
```
cdd() {
  local path="$(pf -t dD $@)"
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
