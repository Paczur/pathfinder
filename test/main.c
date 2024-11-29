#include <ctf/ctf.h>

#include "match/match.h"
#include "score/score.h"
#include "res/res.h"
#include "stats/stats.h"

int main(void) {
  ctf_groups_run(match_group, score_group, res_group, stats_group);
  return ctf_exit_code;
}
