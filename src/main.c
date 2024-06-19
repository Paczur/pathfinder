#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "match.h"
#include "stat.h"
#include "score.h"

uchar *ranges;

void handle(const char *str, const char *const *expr, uchar len) {
  stats_t stats;
  stats_alloc(&stats, len);
  if(matches(ranges, str, expr, len)) {
    stat(&stats, ranges, len * 2, expr, str);
    printf("%u", score(&stats));
  }
  stats_free(&stats);
}

int main(int argc, char *argv[]) {
  DIR *dr;
  struct dirent *de;
  if(argc < 2) {
    puts("HELP MSG HERE");
    return 1;
  }
  dr = opendir(".");
  if(!dr) {
    puts("Couldn't open directory");
    return 1;
  }

  ranges = malloc((argc - 1) * 2 * sizeof(uchar));

  while((de = readdir(dr))) {
    if(!strcmp(de->d_name, "..") || !strcmp(de->d_name, ".")) continue;
    if(de->d_type == DT_DIR) {
      printf("%s ", de->d_name);
      handle(de->d_name, (const char *const *)argv + 1, argc - 1);
      puts("");
    }
  }
  closedir(dr);
  return 0;
}
