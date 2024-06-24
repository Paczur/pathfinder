#include "res.c"
#include "stats.c"
#include "test.h"

TEST(resa_add, empty) {
  resa_t arr = RESA_INIT(1);
  resv_t val = {.score = 1};
  resa_add(&arr, &val);
  assert_int_equal(arr.arr[0].score, 1);
}

TEST(resa_add, score_higher) {
  resa_t arr = RESA_INIT(2);
  arr.size = 1;
  resv_t val1 = {.score = 0};
  resv_t val2 = {.score = 1};
  resa_add(&arr, &val1);
  resa_add(&arr, &val2);
  assert_int_equal(arr.arr[0].score, 1);
  assert_int_equal(arr.arr[1].score, 0);
}

TEST(resa_add, score_lower) {
  resa_t arr = RESA_INIT(2);
  arr.size = 1;
  resv_t val1 = {.score = 1};
  resv_t val2 = {.score = 0};
  resa_add(&arr, &val1);
  resa_add(&arr, &val2);
  assert_int_equal(arr.arr[0].score, 1);
  assert_int_equal(arr.arr[1].score, 0);
}

TEST(resa_add, score_middle) {
  resa_t arr = RESA_INIT(3);
  arr.size = 1;
  resv_t val1 = {.score = 1};
  resv_t val2 = {.score = 2};
  resv_t val3 = {.score = 3};
  resa_add(&arr, &val1);
  resa_add(&arr, &val3);
  resa_add(&arr, &val2);
  assert_int_equal(arr.arr[0].score, 3);
  assert_int_equal(arr.arr[1].score, 2);
  assert_int_equal(arr.arr[2].score, 1);
}

TEST(resa_add, full_higher) {
  resa_t arr = RESA_INIT(1);
  arr.size = 1;
  resv_t val1 = {.score = 1};
  resv_t val2 = {.score = 2};
  resa_add(&arr, &val1);
  resa_add(&arr, &val2);
  assert_int_equal(arr.arr[0].score, 2);
}

TEST(resa_add, full_lower) {
  resa_t arr = RESA_INIT(1);
  arr.size = 1;
  resv_t val1 = {.score = 1};
  resv_t val2 = {.score = 2};
  resa_add(&arr, &val2);
  resa_add(&arr, &val1);
  assert_int_equal(arr.arr[0].score, 2);
}

TEST(resl_add, empty_head) {
  resl_t list;
  resn_t node = {.score = 1};
  resl_add(&list, &node);
  assert_non_null(list.head);
  assert_int_equal(list.head->score, 1);
}

TEST(resl_add, empty_tail) {
  resl_t list;
  resn_t node = {.score = 1};
  resl_add(&list, &node);
  assert_non_null(list.tail);
  assert_int_equal(list.tail->score, 1);
}

TEST(resl_add, second_next) {
  resl_t list;
  resn_t node = {.score = 2};
  resn_t node2 = {.score = 1};
  resl_add(&list, &node);
  resl_add(&list, &node2);
  assert_non_null(list.head->next);
  assert_int_equal(list.head->next->score, 1);
}

TEST(resl_add, second_tail) {
  resl_t list;
  resn_t node = {.score = 2};
  resn_t node2 = {.score = 1};
  resl_add(&list, &node);
  resl_add(&list, &node2);
  assert_non_null(list.tail);
  assert_int_equal(list.tail->score, 1);
}

TEST(resl_add, score_higher) {
  resl_t list;
  resn_t node = {.score = 1};
  resn_t node2 = {.score = 2};
  resl_add(&list, &node);
  resl_add(&list, &node2);
  assert_int_equal(list.head->score, 2);
  assert_int_equal(list.head->next->score, 1);
}

TEST(resl_add, score_lower) {
  resl_t list;
  resn_t node = {.score = 2};
  resn_t node2 = {.score = 1};
  resl_add(&list, &node);
  resl_add(&list, &node2);
  assert_int_equal(list.head->score, 2);
  assert_int_equal(list.head->next->score, 1);
}

TEST(resl_add, score_middle) {
  resl_t list;
  resn_t node = {.score = 1};
  resn_t node2 = {.score = 2};
  resn_t node3 = {.score = 3};
  resl_add(&list, &node);
  resl_add(&list, &node3);
  resl_add(&list, &node2);
  assert_int_equal(list.head->score, 3);
  assert_int_equal(list.head->next->score, 2);
  assert_int_equal(list.head->next->next->score, 1);
}

int main(void) {
  const struct CMUnitTest tests[] = {
    ADD(resa_add, empty),        ADD(resa_add, score_higher),
    ADD(resa_add, score_lower),  ADD(resa_add, score_middle),
    ADD(resa_add, full_higher),  ADD(resa_add, full_lower),
    ADD(resl_add, empty_tail),   ADD(resl_add, empty_head),
    ADD(resl_add, second_next),  ADD(resl_add, second_tail),
    ADD(resl_add, score_higher), ADD(resl_add, score_lower),
    ADD(resl_add, score_middle),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
