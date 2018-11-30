// keccak.h
// 19-Nov-11  Markku-Juhani O. Saarinen <mjos@iki.fi>
#pragma once

#include <stdint.h>
#include <string.h>

#if defined(__cplusplus)
extern "C" {
#endif

#ifndef KECCAK_ROUNDS
#define KECCAK_ROUNDS 24
#endif

#ifndef ROTL64
#define ROTL64(x, y) (((x) << (y)) | ((x) >> (64 - (y))))
#endif


//# copied from hash-ops.h and hash-impl.h of bcndev cryptonote project
#pragma pack(push, 1)
union hash_state {
  uint8_t b[200];
  uint64_t w[25];
};
#pragma pack(pop)

enum {
  HASH_SIZE = 32,
  HASH_DATA_AREA = 136,
  SLOW_HASH_CONTEXT_SIZE = 2097552
};
//------------------------------------------------------------------------------------

// compute a keccak hash (md) of given byte length from "in"
int keccak(const uint8_t *in, int inlen, uint8_t *md, int mdlen);

// update the state
void keccakf(uint64_t st[25], int norounds);

void keccak1600(const uint8_t *in, int inlen, uint8_t *md);

#if defined(__cplusplus)
}
#endif

