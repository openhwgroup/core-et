/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _FIND_VECTORS_SMALL_H_
#define _FIND_VECTORS_SMALL_H_

void find_vectors_small(volatile int32_t ** lengths, volatile int64_t ** indices, volatile float ** weights, 
                        volatile uint8_t ** data, uint64_t * out, uint32_t shire_id, uint32_t outTensorRowSize);

#endif