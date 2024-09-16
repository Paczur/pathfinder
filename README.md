## Pathfinder
Program that filters and scores paths based on expression provided by user.
Primarly designed for combining with cd for quick system naviagtion.

### Usage
```
  Find path(s) best matching EXPR using substring matches.

  General options:
  -h, --help            Show help
  -v, --verbose         Print errors to stderr
  -,  --                Mark end of arguments, useful for providing EXPR starting with "-"

  Traverse Options:
  -d, --depth        Max depth to search down the tree (default 5)
  -f, --file         Get paths from file instead of traversing (use "-" for stdin)
  -M, --mount        Don't cross device boundaries

  Filter Options:
  -t, --types        Types of entities considered when searching,
                     represented by by string of letters
                     (a-all, d-dir, f-file, l-link, D-link to dir, F-link to file)
                     (default dfl)

  Print Options:
  -i, --interactive     Possible options: always, auto, never (default auto)
  -m, --matches         Number of matches to print (default 9)
  -r, --reverse         Reverse display order
```

### Recommended function to put in .bashrc
```
cdd() {
  local path="$(pf -t dD "$@")"
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
