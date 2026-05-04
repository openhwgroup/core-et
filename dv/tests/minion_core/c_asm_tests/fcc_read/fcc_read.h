/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _FCC_READ_H_
#define _FCC_READ_H_
#define NR_COUNTERS 2
#define ITERATIONS sizeof(test_config)/sizeof(iter_config_t)

typedef struct {
  int counter;
  uint64_t producer;
  uint64_t nr_credits;
}iter_config_t;

static iter_config_t test_config [] = {
{ 0, 1, 2},
{ 1, 1, 12},
{ 1, 0, 14},
{ 1, 0, 6},
{ 1, 0, 6},
{ 1, 1, 9},
{ 1, 0, 7},
{ 0, 1, 6},
{ 1, 1, 14},
{ 0, 0, 4},
{ 1, 0, 12},
{ 0, 0, 15},
{ 0, 0, 6},
{ 0, 1, 14},
{ 0, 0, 5},
{ 1, 0, 12},
{ 1, 0, 12},
{ 0, 1, 14},
{ 1, 1, 7},
{ 1, 1, 10},
{ 0, 1, 3},
{ 1, 0, 7},
{ 0, 1, 6},
{ 0, 0, 15},
{ 1, 1, 13},
{ 0, 1, 10},
{ 0, 1, 15},
{ 1, 1, 9},
{ 0, 1, 9},
{ 0, 0, 14},
{ 0, 1, 11},
{ 1, 0, 10},
{ 1, 0, 1},
{ 1, 1, 2},
{ 0, 0, 11},
{ 0, 1, 3},
{ 0, 0, 13},
{ 1, 0, 8},
{ 0, 0, 11},
{ 0, 1, 8},
{ 1, 1, 2},
{ 0, 0, 13},
{ 1, 0, 13},
{ 1, 1, 2},
{ 1, 1, 14},
{ 1, 1, 15},
{ 1, 0, 7},
{ 1, 1, 9},
{ 0, 1, 3},
{ 1, 1, 15}
};
#endif
