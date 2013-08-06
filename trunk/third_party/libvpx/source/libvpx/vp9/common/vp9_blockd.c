/*
 *  Copyright (c) 2010 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */


#include "vp9/common/vp9_blockd.h"
#include "vpx_mem/vpx_mem.h"

const uint8_t vp9_block2left[TX_SIZE_MAX_MB][24] = {
  { 0, 0, 0, 0,
    1, 1, 1, 1,
    2, 2, 2, 2,
    3, 3, 3, 3,
    4, 4,
    5, 5,
    6, 6,
    7, 7 },
  { 0, 0, 0, 0,
    0, 0, 0, 0,
    2, 2, 2, 2,
    2, 2, 2, 2,
    4, 4,
    4, 4,
    6, 6,
    6, 6 },
  { 0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0 },
};
const uint8_t vp9_block2above[TX_SIZE_MAX_MB][24] = {
  { 0, 1, 2, 3,
    0, 1, 2, 3,
    0, 1, 2, 3,
    0, 1, 2, 3,
    4, 5,
    4, 5,
    6, 7,
    6, 7 },
  { 0, 0, 0, 0,
    2, 2, 2, 2,
    0, 0, 0, 0,
    2, 2, 2, 2,
    4, 4,
    4, 4,
    6, 6,
    6, 6 },
  { 0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0 },
};

#define S(x) x + sizeof(ENTROPY_CONTEXT_PLANES) / sizeof(ENTROPY_CONTEXT)
const uint8_t vp9_block2left_sb[TX_SIZE_MAX_SB][96] = {
  { 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2,
    3, 3, 3, 3, 3, 3, 3, 3,
    S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0),
    S(1), S(1), S(1), S(1), S(1), S(1), S(1), S(1),
    S(2), S(2), S(2), S(2), S(2), S(2), S(2), S(2),
    S(3), S(3), S(3), S(3), S(3), S(3), S(3), S(3),
    4, 4, 4, 4,
    5, 5, 5, 5,
    S(4), S(4), S(4), S(4),
    S(5), S(5), S(5), S(5),
    6, 6, 6, 6,
    7, 7, 7, 7,
    S(6), S(6), S(6), S(6),
    S(7), S(7), S(7), S(7) },
  { 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0),
    S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0),
    S(2), S(2), S(2), S(2), S(2), S(2), S(2), S(2),
    S(2), S(2), S(2), S(2), S(2), S(2), S(2), S(2),
    4, 4, 4, 4,
    4, 4, 4, 4,
    S(4), S(4), S(4), S(4),
    S(4), S(4), S(4), S(4),
    6, 6, 6, 6,
    6, 6, 6, 6,
    S(6), S(6), S(6), S(6),
    S(6), S(6), S(6), S(6) },
  { 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0),
    S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0),
    S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0),
    S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0),
    4, 4, 4, 4,
    4, 4, 4, 4,
    4, 4, 4, 4,
    4, 4, 4, 4,
    6, 6, 6, 6,
    6, 6, 6, 6,
    6, 6, 6, 6,
    6, 6, 6, 6 },
  { 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0 },
};
const uint8_t vp9_block2above_sb[TX_SIZE_MAX_SB][96] = {
  { 0, 1, 2, 3, S(0), S(1), S(2), S(3),
    0, 1, 2, 3, S(0), S(1), S(2), S(3),
    0, 1, 2, 3, S(0), S(1), S(2), S(3),
    0, 1, 2, 3, S(0), S(1), S(2), S(3),
    0, 1, 2, 3, S(0), S(1), S(2), S(3),
    0, 1, 2, 3, S(0), S(1), S(2), S(3),
    0, 1, 2, 3, S(0), S(1), S(2), S(3),
    0, 1, 2, 3, S(0), S(1), S(2), S(3),
    4, 5, S(4), S(5),
    4, 5, S(4), S(5),
    4, 5, S(4), S(5),
    4, 5, S(4), S(5),
    6, 7, S(6), S(7),
    6, 7, S(6), S(7),
    6, 7, S(6), S(7),
    6, 7, S(6), S(7) },
  { 0, 0, 0, 0, 2, 2, 2, 2,
    S(0), S(0), S(0), S(0), S(2), S(2), S(2), S(2),
    0, 0, 0, 0, 2, 2, 2, 2,
    S(0), S(0), S(0), S(0), S(2), S(2), S(2), S(2),
    0, 0, 0, 0, 2, 2, 2, 2,
    S(0), S(0), S(0), S(0), S(2), S(2), S(2), S(2),
    0, 0, 0, 0, 2, 2, 2, 2,
    S(0), S(0), S(0), S(0), S(2), S(2), S(2), S(2),
    4, 4, 4, 4,
    S(4), S(4), S(4), S(4),
    4, 4, 4, 4,
    S(4), S(4), S(4), S(4),
    6, 6, 6, 6,
    S(6), S(6), S(6), S(6),
    6, 6, 6, 6,
    S(6), S(6), S(6), S(6) },
  { 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0),
    S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0),
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0),
    S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0),
    4, 4, 4, 4,
    4, 4, 4, 4,
    4, 4, 4, 4,
    4, 4, 4, 4,
    6, 6, 6, 6,
    6, 6, 6, 6,
    6, 6, 6, 6,
    6, 6, 6, 6 },
  { 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0 },
};

#define T(x) x + 2 * (sizeof(ENTROPY_CONTEXT_PLANES) / sizeof(ENTROPY_CONTEXT))
#define U(x) x + 3 * (sizeof(ENTROPY_CONTEXT_PLANES) / sizeof(ENTROPY_CONTEXT))
const uint8_t vp9_block2left_sb64[TX_SIZE_MAX_SB][384] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0),
    S(1), S(1), S(1), S(1), S(1), S(1), S(1), S(1), S(1), S(1), S(1), S(1), S(1), S(1), S(1), S(1),
    S(2), S(2), S(2), S(2), S(2), S(2), S(2), S(2), S(2), S(2), S(2), S(2), S(2), S(2), S(2), S(2),
    S(3), S(3), S(3), S(3), S(3), S(3), S(3), S(3), S(3), S(3), S(3), S(3), S(3), S(3), S(3), S(3),
    T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0),
    T(1), T(1), T(1), T(1), T(1), T(1), T(1), T(1), T(1), T(1), T(1), T(1), T(1), T(1), T(1), T(1),
    T(2), T(2), T(2), T(2), T(2), T(2), T(2), T(2), T(2), T(2), T(2), T(2), T(2), T(2), T(2), T(2),
    T(3), T(3), T(3), T(3), T(3), T(3), T(3), T(3), T(3), T(3), T(3), T(3), T(3), T(3), T(3), T(3),
    U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0),
    U(1), U(1), U(1), U(1), U(1), U(1), U(1), U(1), U(1), U(1), U(1), U(1), U(1), U(1), U(1), U(1),
    U(2), U(2), U(2), U(2), U(2), U(2), U(2), U(2), U(2), U(2), U(2), U(2), U(2), U(2), U(2), U(2),
    U(3), U(3), U(3), U(3), U(3), U(3), U(3), U(3), U(3), U(3), U(3), U(3), U(3), U(3), U(3), U(3),
    4, 4, 4, 4, 4, 4, 4, 4,
    5, 5, 5, 5, 5, 5, 5, 5,
    S(4), S(4), S(4), S(4), S(4), S(4), S(4), S(4),
    S(5), S(5), S(5), S(5), S(5), S(5), S(5), S(5),
    T(4), T(4), T(4), T(4), T(4), T(4), T(4), T(4),
    T(5), T(5), T(5), T(5), T(5), T(5), T(5), T(5),
    U(4), U(4), U(4), U(4), U(4), U(4), U(4), U(4),
    U(5), U(5), U(5), U(5), U(5), U(5), U(5), U(5),
    6, 6, 6, 6, 6, 6, 6, 6,
    7, 7, 7, 7, 7, 7, 7, 7,
    S(6), S(6), S(6), S(6), S(6), S(6), S(6), S(6),
    S(7), S(7), S(7), S(7), S(7), S(7), S(7), S(7),
    T(6), T(6), T(6), T(6), T(6), T(6), T(6), T(6),
    T(7), T(7), T(7), T(7), T(7), T(7), T(7), T(7),
    U(6), U(6), U(6), U(6), U(6), U(6), U(6), U(6),
    U(7), U(7), U(7), U(7), U(7), U(7), U(7), U(7) },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0),
    S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0),
    S(2), S(2), S(2), S(2), S(2), S(2), S(2), S(2), S(2), S(2), S(2), S(2), S(2), S(2), S(2), S(2),
    S(2), S(2), S(2), S(2), S(2), S(2), S(2), S(2), S(2), S(2), S(2), S(2), S(2), S(2), S(2), S(2),
    T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0),
    T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0),
    T(2), T(2), T(2), T(2), T(2), T(2), T(2), T(2), T(2), T(2), T(2), T(2), T(2), T(2), T(2), T(2),
    T(2), T(2), T(2), T(2), T(2), T(2), T(2), T(2), T(2), T(2), T(2), T(2), T(2), T(2), T(2), T(2),
    U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0),
    U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0),
    U(2), U(2), U(2), U(2), U(2), U(2), U(2), U(2), U(2), U(2), U(2), U(2), U(2), U(2), U(2), U(2),
    U(2), U(2), U(2), U(2), U(2), U(2), U(2), U(2), U(2), U(2), U(2), U(2), U(2), U(2), U(2), U(2),
    4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4,
    S(4), S(4), S(4), S(4), S(4), S(4), S(4), S(4),
    S(4), S(4), S(4), S(4), S(4), S(4), S(4), S(4),
    T(4), T(4), T(4), T(4), T(4), T(4), T(4), T(4),
    T(4), T(4), T(4), T(4), T(4), T(4), T(4), T(4),
    U(4), U(4), U(4), U(4), U(4), U(4), U(4), U(4),
    U(4), U(4), U(4), U(4), U(4), U(4), U(4), U(4),
    6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6,
    S(6), S(6), S(6), S(6), S(6), S(6), S(6), S(6),
    S(6), S(6), S(6), S(6), S(6), S(6), S(6), S(6),
    T(6), T(6), T(6), T(6), T(6), T(6), T(6), T(6),
    T(6), T(6), T(6), T(6), T(6), T(6), T(6), T(6),
    U(6), U(6), U(6), U(6), U(6), U(6), U(6), U(6),
    U(6), U(6), U(6), U(6), U(6), U(6), U(6), U(6) },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0),
    S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0),
    S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0),
    S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0),
    T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0),
    T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0),
    T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0),
    T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0),
    U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0),
    U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0),
    U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0),
    U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0),
    4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4,
    T(4), T(4), T(4), T(4), T(4), T(4), T(4), T(4),
    T(4), T(4), T(4), T(4), T(4), T(4), T(4), T(4),
    T(4), T(4), T(4), T(4), T(4), T(4), T(4), T(4),
    T(4), T(4), T(4), T(4), T(4), T(4), T(4), T(4),
    6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6,
    T(6), T(6), T(6), T(6), T(6), T(6), T(6), T(6),
    T(6), T(6), T(6), T(6), T(6), T(6), T(6), T(6),
    T(6), T(6), T(6), T(6), T(6), T(6), T(6), T(6),
    T(6), T(6), T(6), T(6), T(6), T(6), T(6), T(6) },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4,
    6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6 },
};
const uint8_t vp9_block2above_sb64[TX_SIZE_MAX_SB][384] = {
  { 0, 1, 2, 3, S(0), S(1), S(2), S(3), T(0), T(1), T(2), T(3), U(0), U(1), U(2), U(3),
    0, 1, 2, 3, S(0), S(1), S(2), S(3), T(0), T(1), T(2), T(3), U(0), U(1), U(2), U(3),
    0, 1, 2, 3, S(0), S(1), S(2), S(3), T(0), T(1), T(2), T(3), U(0), U(1), U(2), U(3),
    0, 1, 2, 3, S(0), S(1), S(2), S(3), T(0), T(1), T(2), T(3), U(0), U(1), U(2), U(3),
    0, 1, 2, 3, S(0), S(1), S(2), S(3), T(0), T(1), T(2), T(3), U(0), U(1), U(2), U(3),
    0, 1, 2, 3, S(0), S(1), S(2), S(3), T(0), T(1), T(2), T(3), U(0), U(1), U(2), U(3),
    0, 1, 2, 3, S(0), S(1), S(2), S(3), T(0), T(1), T(2), T(3), U(0), U(1), U(2), U(3),
    0, 1, 2, 3, S(0), S(1), S(2), S(3), T(0), T(1), T(2), T(3), U(0), U(1), U(2), U(3),
    0, 1, 2, 3, S(0), S(1), S(2), S(3), T(0), T(1), T(2), T(3), U(0), U(1), U(2), U(3),
    0, 1, 2, 3, S(0), S(1), S(2), S(3), T(0), T(1), T(2), T(3), U(0), U(1), U(2), U(3),
    0, 1, 2, 3, S(0), S(1), S(2), S(3), T(0), T(1), T(2), T(3), U(0), U(1), U(2), U(3),
    0, 1, 2, 3, S(0), S(1), S(2), S(3), T(0), T(1), T(2), T(3), U(0), U(1), U(2), U(3),
    0, 1, 2, 3, S(0), S(1), S(2), S(3), T(0), T(1), T(2), T(3), U(0), U(1), U(2), U(3),
    0, 1, 2, 3, S(0), S(1), S(2), S(3), T(0), T(1), T(2), T(3), U(0), U(1), U(2), U(3),
    0, 1, 2, 3, S(0), S(1), S(2), S(3), T(0), T(1), T(2), T(3), U(0), U(1), U(2), U(3),
    0, 1, 2, 3, S(0), S(1), S(2), S(3), T(0), T(1), T(2), T(3), U(0), U(1), U(2), U(3),
    4, 5, S(4), S(5), T(4), T(5), U(4), U(5),
    4, 5, S(4), S(5), T(4), T(5), U(4), U(5),
    4, 5, S(4), S(5), T(4), T(5), U(4), U(5),
    4, 5, S(4), S(5), T(4), T(5), U(4), U(5),
    4, 5, S(4), S(5), T(4), T(5), U(4), U(5),
    4, 5, S(4), S(5), T(4), T(5), U(4), U(5),
    4, 5, S(4), S(5), T(4), T(5), U(4), U(5),
    4, 5, S(4), S(5), T(4), T(5), U(4), U(5),
    6, 7, S(6), S(7), T(6), T(7), U(6), U(7),
    6, 7, S(6), S(7), T(6), T(7), U(6), U(7),
    6, 7, S(6), S(7), T(6), T(7), U(6), U(7),
    6, 7, S(6), S(7), T(6), T(7), U(6), U(7),
    6, 7, S(6), S(7), T(6), T(7), U(6), U(7),
    6, 7, S(6), S(7), T(6), T(7), U(6), U(7),
    6, 7, S(6), S(7), T(6), T(7), U(6), U(7),
    6, 7, S(6), S(7), T(6), T(7), U(6), U(7) },
  { 0, 0, 0, 0, 2, 2, 2, 2, S(0), S(0), S(0), S(0), S(2), S(2), S(2), S(2),
    T(0), T(0), T(0), T(0), T(2), T(2), T(2), T(2), U(0), U(0), U(0), U(0), U(2), U(2), U(2), U(2),
    0, 0, 0, 0, 2, 2, 2, 2, S(0), S(0), S(0), S(0), S(2), S(2), S(2), S(2),
    T(0), T(0), T(0), T(0), T(2), T(2), T(2), T(2), U(0), U(0), U(0), U(0), U(2), U(2), U(2), U(2),
    0, 0, 0, 0, 2, 2, 2, 2, S(0), S(0), S(0), S(0), S(2), S(2), S(2), S(2),
    T(0), T(0), T(0), T(0), T(2), T(2), T(2), T(2), U(0), U(0), U(0), U(0), U(2), U(2), U(2), U(2),
    0, 0, 0, 0, 2, 2, 2, 2, S(0), S(0), S(0), S(0), S(2), S(2), S(2), S(2),
    T(0), T(0), T(0), T(0), T(2), T(2), T(2), T(2), U(0), U(0), U(0), U(0), U(2), U(2), U(2), U(2),
    0, 0, 0, 0, 2, 2, 2, 2, S(0), S(0), S(0), S(0), S(2), S(2), S(2), S(2),
    T(0), T(0), T(0), T(0), T(2), T(2), T(2), T(2), U(0), U(0), U(0), U(0), U(2), U(2), U(2), U(2),
    0, 0, 0, 0, 2, 2, 2, 2, S(0), S(0), S(0), S(0), S(2), S(2), S(2), S(2),
    T(0), T(0), T(0), T(0), T(2), T(2), T(2), T(2), U(0), U(0), U(0), U(0), U(2), U(2), U(2), U(2),
    0, 0, 0, 0, 2, 2, 2, 2, S(0), S(0), S(0), S(0), S(2), S(2), S(2), S(2),
    T(0), T(0), T(0), T(0), T(2), T(2), T(2), T(2), U(0), U(0), U(0), U(0), U(2), U(2), U(2), U(2),
    0, 0, 0, 0, 2, 2, 2, 2, S(0), S(0), S(0), S(0), S(2), S(2), S(2), S(2),
    T(0), T(0), T(0), T(0), T(2), T(2), T(2), T(2), U(0), U(0), U(0), U(0), U(2), U(2), U(2), U(2),
    4, 4, 4, 4, S(4), S(4), S(4), S(4),
    T(4), T(4), T(4), T(4), U(4), U(4), U(4), U(4),
    4, 4, 4, 4, S(4), S(4), S(4), S(4),
    T(4), T(4), T(4), T(4), U(4), U(4), U(4), U(4),
    4, 4, 4, 4, S(4), S(4), S(4), S(4),
    T(4), T(4), T(4), T(4), U(4), U(4), U(4), U(4),
    4, 4, 4, 4, S(4), S(4), S(4), S(4),
    T(4), T(4), T(4), T(4), U(4), U(4), U(4), U(4),
    6, 6, 6, 6, S(6), S(6), S(6), S(6),
    T(6), T(6), T(6), T(6), U(6), U(6), U(6), U(6),
    6, 6, 6, 6, S(6), S(6), S(6), S(6),
    T(6), T(6), T(6), T(6), U(6), U(6), U(6), U(6),
    6, 6, 6, 6, S(6), S(6), S(6), S(6),
    T(6), T(6), T(6), T(6), U(6), U(6), U(6), U(6),
    6, 6, 6, 6, S(6), S(6), S(6), S(6),
    T(6), T(6), T(6), T(6), U(6), U(6), U(6), U(6) },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0),
    T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0),
    U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0),
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0),
    T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0),
    U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0),
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0),
    T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0),
    U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0),
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0), S(0),
    T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0),
    U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0), U(0),
    4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4,
    T(4), T(4), T(4), T(4), T(4), T(4), T(4), T(4),
    T(4), T(4), T(4), T(4), T(4), T(4), T(4), T(4),
    4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4,
    T(4), T(4), T(4), T(4), T(4), T(4), T(4), T(4),
    T(4), T(4), T(4), T(4), T(4), T(4), T(4), T(4),
    6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6,
    T(6), T(6), T(6), T(6), T(6), T(6), T(6), T(6),
    T(6), T(6), T(6), T(6), T(6), T(6), T(6), T(6),
    6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6,
    T(6), T(6), T(6), T(6), T(6), T(6), T(6), T(6),
    T(6), T(6), T(6), T(6), T(6), T(6), T(6), T(6) },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4,
    6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6 },
};
#undef U
#undef T
#undef S