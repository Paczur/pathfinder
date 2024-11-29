#include "res.h"
#include <res/res.h>

CTF_TEST(resa_add_empty) {
  resa_t arr = RESA_INIT(1);
  resv_t val = {.score = 1};
  resa_add(&arr, &val);
  expect_uint_eq(arr.arr[0].score, 1);
}

CTF_TEST(resa_add_score_higher) {
  resa_t arr = RESA_INIT(2);
  arr.size = 1;
  resv_t val1 = {.score = 0};
  resv_t val2 = {.score = 1};
  resa_add(&arr, &val1);
  resa_add(&arr, &val2);
  expect_uint_eq(arr.arr[0].score, 1);
  expect_uint_eq(arr.arr[1].score, 0);
}

CTF_TEST(resa_add_score_lower) {
  resa_t arr = RESA_INIT(2);
  arr.size = 1;
  resv_t val1 = {.score = 1};
  resv_t val2 = {.score = 0};
  resa_add(&arr, &val1);
  resa_add(&arr, &val2);
  expect_uint_eq(arr.arr[0].score, 1);
  expect_uint_eq(arr.arr[1].score, 0);
}

CTF_TEST(resa_add_score_middle) {
  resa_t arr = RESA_INIT(3);
  arr.size = 1;
  resv_t val1 = {.score = 1};
  resv_t val2 = {.score = 2};
  resv_t val3 = {.score = 3};
  resa_add(&arr, &val1);
  resa_add(&arr, &val3);
  resa_add(&arr, &val2);
  expect_uint_eq(arr.arr[0].score, 3);
  expect_uint_eq(arr.arr[1].score, 2);
  expect_uint_eq(arr.arr[2].score, 1);
}

CTF_TEST(resa_add_full_higher) {
  resa_t arr = RESA_INIT(1);
  arr.size = 1;
  resv_t val1 = {.score = 1};
  resv_t val2 = {.score = 2};
  resa_add(&arr, &val1);
  resa_add(&arr, &val2);
  expect_uint_eq(arr.arr[0].score, 2);
}

CTF_TEST(resa_add_full_lower) {
  resa_t arr = RESA_INIT(1);
  arr.size = 1;
  resv_t val1 = {.score = 1};
  resv_t val2 = {.score = 2};
  resa_add(&arr, &val2);
  resa_add(&arr, &val1);
  expect_uint_eq(arr.arr[0].score, 2);
}

CTF_TEST(resl_add_empty_head) {
  resl_t list = {0};
  resn_t node = {.score = 1};
  resl_add(&list, &node);
  expect_non_null(list.head);
  expect_uint_eq(list.head->score, 1);
}

CTF_TEST(resl_add_empty_tail) {
  resl_t list = {0};
  resn_t node = {.score = 1};
  resl_add(&list, &node);
  expect_non_null(list.tail);
  expect_uint_eq(list.tail->score, 1);
}

CTF_TEST(resl_add_second_next) {
  resl_t list = {0};
  resn_t node = {.score = 2};
  resn_t node2 = {.score = 1};
  resl_add(&list, &node);
  resl_add(&list, &node2);
  assert_non_null(list.head);
  assert_non_null(list.head->next);
  expect_uint_eq(list.head->next->score, 1);
}

CTF_TEST(resl_add_second_tail) {
  resl_t list = {0};
  resn_t node = {.score = 2};
  resn_t node2 = {.score = 1};
  resl_add(&list, &node);
  resl_add(&list, &node2);
  expect_non_null(list.tail);
  expect_uint_eq(list.tail->score, 1);
}

CTF_TEST(resl_add_score_higher) {
  resl_t list = {0};
  resn_t node = {.score = 1};
  resn_t node2 = {.score = 2};
  resl_add(&list, &node);
  resl_add(&list, &node2);
  expect_uint_eq(list.head->score, 2);
  expect_uint_eq(list.head->next->score, 1);
}

CTF_TEST(resl_add_score_lower) {
  resl_t list = {0};
  resn_t node = {.score = 2};
  resn_t node2 = {.score = 1};
  resl_add(&list, &node);
  resl_add(&list, &node2);
  expect_uint_eq(list.head->score, 2);
  expect_uint_eq(list.head->next->score, 1);
}

CTF_TEST(resl_add_score_middle) {
  resl_t list = {0};
  resn_t node = {.score = 1};
  resn_t node2 = {.score = 2};
  resn_t node3 = {.score = 3};
  resl_add(&list, &node);
  resl_add(&list, &node3);
  resl_add(&list, &node2);
  expect_uint_eq(list.head->score, 3);
  expect_uint_eq(list.head->next->score, 2);
  expect_uint_eq(list.head->next->next->score, 1);
}

CTF_GROUP(res_group) = {
  resa_add_empty,        resa_add_score_higher, resa_add_score_lower,
  resa_add_score_middle, resa_add_full_higher,  resa_add_full_lower,
  resl_add_empty_tail,   resl_add_empty_head,   resl_add_second_next,
  resl_add_second_tail,  resl_add_score_higher, resl_add_score_lower,
  resl_add_score_middle,
};

