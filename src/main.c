#include "match.h"
#include "res.h"
#include "score.h"
#include "stats.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/resource.h>

#define HELP_MSG                                                           \
  "pf [OPTION] ... EXPR ...\n"                                             \
  "Find path(s) best matching EXPR using substring matches.\n"             \
  "\n"                                                                     \
  "General options:\n"                                                     \
  "-h, --help     Show help\n"                                             \
  "-v, --verbose  Print errors to stderr\n"                                \
  "-,  --         Mark end of arguments, useful for providing EXPR "       \
  "starting with \"-\"\n"                                                  \
  "\n"                                                                     \
  "Traverse Options:\n"                                                    \
  "-d, --depth  Max depth to search down the tree (default 5)\n"           \
  "-f, --file   Get paths from file instead of traversing (use \"-\" for " \
  "stdin)\n"                                                               \
  "-M, --mount  Don't cross device boundaries\n"                           \
  "-D, --dir    Start traversing from specified dir (default \".\")\n"     \
  "\n"                                                                     \
  "Filter Options:\n"                                                      \
  "-t, --types  Types of entities considered when searching,\n"            \
  "             represented by by string of letters\n"                     \
  "             (a-all, d-dir, f-file, l-link, D-link to dir, F-link "     \
  "to file)\n"                                                             \
  "             (default dfl)\n"                                           \
  "\n"                                                                     \
  "Print Options:\n"                                                       \
  "-i, --interactive  Possible options: always, auto, never (default "     \
  "auto)\n"                                                                \
  "-m, --matches      Number of matches to print (default 9)\n"            \
  "-r, --reverse      Reverse display order\n"

#define OPTS_FLAG            \
  X('v', "verbose", verbose) \
  X('r', "reverse", reverse) \
  X('M', "mount", mount)

#define OPTS_VOID X('h', "help", opts_help)

#define OPTS_STR                          \
  X('i', "interactive", opts_interactive) \
  X('t', "types", opts_types)             \
  X('f', "file", opts_file)               \
  X('D', "dir", opts_dir)

#define OPTS_NUM                  \
  X('m', "matches", opts_matches) \
  X('d', "depth", opts_depth)

#define DIR_SIZE 512

typedef enum {
  TYPE_FILTER_ALL = 1,
  TYPE_FILTER_DIRS = 2,
  TYPE_FILTER_FILES = 4,
  TYPE_FILTER_LINKS = 8,
  TYPE_FILTER_DIR_LINKS = 16,
  TYPE_FILTER_FILE_LINKS = 32,
} TYPE_FILTER;

typedef enum {
  INTERACTIVE_AUTO,
  INTERACTIVE_ALWAYS,
  INTERACTIVE_NEVER,
} interactive_t;

TYPE_FILTER type_filter =
  TYPE_FILTER_DIRS | TYPE_FILTER_FILES | TYPE_FILTER_LINKS;
bool unlimited;
interactive_t interactive;
bool verbose;
bool reverse;
bool mount;
uint depth = 5;
const char *file = NULL;
char dir[DIR_SIZE] = ".";

uint *ranges;
struct stat sstat;
stats_t st;
resl_t list;
resa_t arr = {.size = 0, .capacity = 9};

static void cleanup(void) {
#ifndef NDEBUG
  if(unlimited) {
    resl_free(&list);
  } else {
    resa_free(&arr);
  }
  free(ranges);
#endif
}

CONST static uint node_count(const char *const *expr, uint len) {
  uint res = len;
  for(uint i = 0; i < len; i++) {
    for(uint j = 0; expr[i][j]; j++) {
      if(expr[i][j] == '/') res++;
    }
  }
  return res;
}

static uint addinf(const char *restrict str, const char *restrict const *expr,
                   uint count) {
  resn_t *new;
  stats_t *s;
  new = malloc(sizeof(resn_t));
#ifndef NDEBUG
  s = &new->stats;
#else
  s = &st;
#endif
  resn_alloc(new, strlen(str) + 1, count);
  strcpy(new->path, str);
  stats(s, ranges, count * 2, expr, str);
  new->score = score(s, count);
  resl_add(&list, new);
  return new->score;
  return 0;
}

static uint addfin(const char *restrict str, const char *restrict const *expr,
                   uint count) {
  resv_t new;
  stats_t *s;
#ifndef NDEBUG
  s = &new.stats;
#else
  s = &st;
#endif
  resv_alloc(&new, strlen(str) + 1, count);
  strcpy(new.path, str);
  stats(s, ranges, count * 2, expr, str);
  new.score = score(s, count);
  if(!resa_add(&arr, &new)) {
    resv_free(&new);
  } else {
    return new.score;
  }
  return 0;
}

static uint add(const char *restrict str, const char *restrict const *expr,
                uint count) {
  return (unlimited) ? addinf(str, expr, count) : addfin(str, expr, count);
}

PURE static bool de_type_matches(const struct dirent *de) {
  return type_filter == TYPE_FILTER_ALL ||
         (type_filter & TYPE_FILTER_FILES && de->d_type == DT_REG) ||
         (type_filter & TYPE_FILTER_DIRS && de->d_type == DT_DIR) ||
         (type_filter & (TYPE_FILTER_LINKS | TYPE_FILTER_DIR_LINKS |
                         TYPE_FILTER_FILE_LINKS) &&
          de->d_type == DT_LNK);
}

CONST static bool de_useless(const struct dirent *de) {
  return (de->d_name[0] == '.' &&
          (de->d_name[1] == 0 ||
           (de->d_name[1] == '.' && de->d_name[2] == 0))) ||
         (de->d_type != DT_DIR && !de_type_matches(de));
}

static int filter_add(const char *restrict path, const struct dirent *de,
                      uint *ranges, const char *restrict const *expr, uint len,
                      uint count) {
  struct stat lstats;
  uchar type = 0;
  if(type_filter != TYPE_FILTER_ALL) {
    if(!de || de->d_type == DT_UNKNOWN) {
      lstat(path, &lstats);
      type = S_ISDIR(lstats.st_mode)   ? DT_DIR
             : S_ISREG(lstats.st_mode) ? DT_REG
             : S_ISLNK(lstats.st_mode) ? DT_LNK
                                       : DT_BLK;
    } else {
      type = de->d_type;
    }
  }

  if(type_filter == TYPE_FILTER_ALL ||
     (type_filter & TYPE_FILTER_FILES && type == DT_REG) ||
     (type_filter & TYPE_FILTER_DIRS && type == DT_DIR) ||
     (type_filter & TYPE_FILTER_LINKS && type == DT_LNK)) {
    if(matches(ranges, path, expr, len, count)) add(path, expr, count);
  } else if(type == DT_LNK &&
            type_filter & (TYPE_FILTER_DIR_LINKS | TYPE_FILTER_FILE_LINKS)) {
    if(matches(ranges, path, expr, len, count)) {
      if(stat(path, &sstat)) {
        if(verbose) fprintf(stderr, "Couldn't stat directory: %s\n", path);
        return -1;
      }
      if((type_filter & TYPE_FILTER_DIR_LINKS && S_ISDIR(sstat.st_mode)) ||
         (type_filter & TYPE_FILTER_FILE_LINKS && S_ISREG(sstat.st_mode))) {
        add(path, expr, count);
      }
      return 1;
    }
  }
  return 0;
}

static void file_paths(const char *const *expr, uint len, uint count) {
  FILE *f;
  size_t line_size = 512;
  char *line = malloc(line_size);
  ssize_t line_len;
  if(!strcmp("-", file)) {
    f = stdin;
  } else {
    f = fopen(file, "r");
  }
  while((line_len = getline(&line, &line_size, f)) > 0) {
    for(uint i = 0; i < line_len && i < line_size - 1; i++) {
      if(line[i] == '\n') {
        line[i] = 0;
        filter_add(line, NULL, ranges, expr, len, count);
      }
    }
  }
}

static void dfs_paths(const char *const *expr, uint len, uint count) {
  struct dirent *de;
  dev_t id = 0;
  int ret;
  uint *null_stack;
  bool stated;
  uint new_null = 1;
  DIR **dr_stack = malloc(depth * sizeof(DIR *));
  uint stack_i = 0;
  dr_stack[0] = opendir(dir);
  if(!dr_stack[0]) {
    if(verbose) fprintf(stderr, "Couldn't open directory: %s\n", dir);
    free(dr_stack);
    return;
  }
  if(mount) {
    if(stat(dir, &sstat)) {
      if(verbose) fprintf(stderr, "Couldn't stat directory: %s\n", dir);
      return;
    }
    id = sstat.st_dev;
  }
  null_stack = malloc(depth * sizeof(uint));
  if(dir[0] == '.' && !dir[1]) dir[0] = 0;

  null_stack[0] = strnlen(dir, DIR_SIZE);
  while(1) {
    stated = false;
    de = readdir(dr_stack[stack_i]);
    if(!de) {
      closedir(dr_stack[stack_i]);
      if(stack_i == 0) break;
      stack_i--;
      continue;
    }
    if(de_useless(de)) continue;

    if(null_stack[stack_i] > 0 && dir[null_stack[stack_i] - 1] != '/') {
      dir[null_stack[stack_i]] = '/';
      new_null = stpcpy(dir + null_stack[stack_i] + 1, de->d_name) - dir;
    } else {
      new_null = stpcpy(dir + null_stack[stack_i], de->d_name) - dir;
    }

    ret = filter_add(dir, de, ranges, expr, len, count);
    if(ret == -1) continue;
    if(ret == 1) stated = true;

    if(de->d_type == DT_DIR && stack_i + 1 < depth) {
      if(mount && !stated && stat(dir, &sstat)) {
        if(verbose) fprintf(stderr, "Couldn't stat directory: %s\n", dir);
        continue;
      }
      if(!mount || sstat.st_dev == id) {
        stack_i++;
        dr_stack[stack_i] = opendir(dir);
        if(!dr_stack[stack_i]) {
          stack_i--;
        } else {
          null_stack[stack_i] = new_null;
        }
      }
    }
  }
  free(dr_stack);
  free(null_stack);
}

static void exit_clean(void) {
  cleanup();
  exit(0);
}

static void exit_error(void) {
  fputs(HELP_MSG "\n", stderr);
  cleanup();
  exit(1);
}

static void opts_help(void) {
  puts(HELP_MSG);
  exit_clean();
}

static void opts_matches(long long n) {
  unlimited = n < 1;
  arr.capacity = (n < 1) ? 0 : (uint)n;
}

static void opts_depth(long long n) {
  if(n < 0) exit_error();
  depth = n;
}

static void opts_interactive(const char *str) {
  if(!strcmp(str, "always")) {
    interactive = INTERACTIVE_ALWAYS;
  } else if(!strcmp(str, "auto")) {
    interactive = INTERACTIVE_AUTO;
  } else if(!strcmp(str, "never")) {
    interactive = INTERACTIVE_NEVER;
  } else {
    exit_error();
  }
}

static void opts_types(const char *str) {
  type_filter = 0;
  for(uint i = 0; str[i] != 0; i++) {
    switch(str[i]) {
    case 'f':
      type_filter |= TYPE_FILTER_FILES;
      break;
    case 'd':
      type_filter |= TYPE_FILTER_DIRS;
      break;
    case 'l':
      type_filter |= TYPE_FILTER_LINKS;
      break;
    case 'F':
      type_filter |= TYPE_FILTER_FILE_LINKS;
      break;
    case 'D':
      type_filter |= TYPE_FILTER_DIR_LINKS;
      break;
    case 'a':
      type_filter |= TYPE_FILTER_ALL;
      break;
    default:
      exit_error();
      break;
    }
  }
}

static void opts_file(const char *str) { file = str; }

static void opts_dir(const char *str) { strncpy(dir, str, DIR_SIZE); }

static bool opts_short(const char *opts, const char *val) {
  long long n;

  for(const char *o = opts; *o; o++) {
    switch(*o) {
#define X(s, l, f) \
  case s:          \
    f = true;      \
    break;
      OPTS_FLAG
#undef X
#define X(s, l, f) \
  case s:          \
    f();           \
    break;
      OPTS_VOID
#undef X
#define X(s, l, f)                          \
  case s:                                   \
    if(*(o + 1) != 0 || !val) exit_error(); \
    f(val);                                 \
    return true;                            \
    break;
      OPTS_STR
#undef X
#define X(s, l, f)                                                      \
  case s:                                                               \
    if(*(o + 1) != 0 || !val || !sscanf(val, "%lld", &n)) exit_error(); \
    f(n);                                                               \
    return true;                                                        \
    break;
      OPTS_NUM
#undef X
    default:
      exit_error();
      break;
    }
  }
  return false;
}

static bool opts_long(const char *opts, const char *val) {
  long long n;
  if(0) {
  }
#define X(s, l, f)            \
  else if(!strcmp(opts, l)) { \
    f = true;                 \
  }
  OPTS_FLAG
#undef X
#define X(s, l, f)            \
  else if(!strcmp(opts, l)) { \
    f();                      \
  }
  OPTS_VOID
#undef X
#define X(s, l, f)            \
  else if(!strcmp(opts, l)) { \
    if(!val) exit_error();    \
    f(val);                   \
    return true;              \
  }
  OPTS_STR
#undef X
#define X(s, l, f)                                     \
  else if(!strcmp(opts, l)) {                          \
    if(!val || !sscanf(val, "%lld", &n)) exit_error(); \
    f(n);                                              \
    return true;                                       \
  }
  OPTS_NUM
#undef X
  else exit_error();
  return false;
}

static uint opts(int argc, const char *const argv[]) {
  uint off = 1;
  const char *v;
  if(argc < 2) exit_error();
  for(; off < (uint)argc; off++) {
    if(argv[off][0] != '-') return off;
    v = (off + 1 < (uint)argc) ? argv[off + 1] : NULL;
    if(argv[off][1] == '-') {
      if(!argv[off][2]) return off + 1;
      off += opts_long(argv[off] + 2, v);
    } else {
      if(!argv[off][1]) return off + 1;
      off += opts_short(argv[off] + 1, v);
    }
  }
  return off;
}

static void print(void) {
  uint n;
  if((unlimited && !list.tail) || (!unlimited && !arr.size)) exit_clean();

  if(interactive == INTERACTIVE_ALWAYS ||
     (interactive == INTERACTIVE_AUTO &&
      ((unlimited && resl_length(&list) > 1) ||
       (!unlimited && resa_length(&arr) > 1)))) {
    if(unlimited) {
      if(reverse) {
        resl_numbered_path_print(&list);
      } else {
        resl_reverse_numbered_path_print(&list);
      }
    } else {
      if(reverse) {
        resa_numbered_path_print(&arr);
      } else {
        resa_reverse_numbered_path_print(&arr);
      }
    }
    fputs("Choice: ", stderr);
    if(!scanf("%u", &n)) exit_error();
    if(unlimited) {
      resl_i_path_print(&list, n);
    } else {
      resa_i_path_print(&arr, n);
    }
  } else {
    if(unlimited) {
      if(reverse) {
        resl_reverse_path_print(&list);
      } else {
        resl_path_print(&list);
      }
    } else {
      if(reverse) {
        resa_reverse_path_print(&arr);
      } else {
        resa_path_print(&arr);
      }
    }
  }
}

int main(int argc, const char *const argv[]) {
  uint nc;
  uint off = opts(argc, argv);
  if(off >= (uint)argc) exit_error();
  nc = node_count(argv + off, argc - off);

  ranges = malloc(nc * 2 * sizeof(uint));
  if(!unlimited) resa_alloc(&arr);
#ifdef NDEBUG
  stats_alloc(&st, nc);
#endif

  if(file) {
    file_paths(argv + off, argc - off, nc);
  } else {
    dfs_paths(argv + off, argc - off, nc);
  }

#ifdef NDEBUG
  stats_free(&st);
#endif

#ifndef NDEBUG
  if(unlimited) {
    resl_print(&list, nc);
  } else {
    resa_print(&arr, nc);
  }
  puts("");
#endif

  print();

  exit_clean();
}
