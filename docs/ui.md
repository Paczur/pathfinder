## Options
* -h, --help = Show help
* -m, --max-matches = number of matches to print (default 1)

### Scoring options
* --ignore-dotfiles = Skip dotfiles
* -M, --max-depth = max depth to search down the tree (default 10)
* -r, --reverse-sort = Reverse result of sorting

## Syntax
pf [OPTIONS] path [path...]

Example:
* pf foo bar - find bar (in)directly inside foo
* pf foo/bar - find bar directly inside foo
* pf / foo - find foo (in)directly in root
* pf /foo - find foo directly in root
