## Options
* -h, --help = Show help

### Scoring options
* --ignore-dotfiles = Skip dotfiles
* -D, --normal-dotfiles = ignore dot in dotfile name
* -i, --case-insensitive = don't score based on case
* -M, --max-depth = max depth to search down the tree (default 10)

### Sort options
* -r, --reverse-sort = Reverse result of sorting
* -d, --dirs-first = Dirs first
* -f, --files-first = Files first

### Interactive options
* -i = Enables interactive choice
* -m, --max-matches = number of matches to print (default 30)

## Syntax
pf [OPTIONS] path [path...]

Example:
* pf foo bar - find bar (in)directly inside foo
* pf foo/bar - find bar directly inside foo
* pf / foo - find foo (in)directly in root
* pf /foo - find foo directly in root
