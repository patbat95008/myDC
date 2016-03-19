// Patrick Russell pcrussel
// $Id: bigint.h,v 1.7 2014-01-24 18:33:47-08 - - $

#ifndef __BIGINT_H__
#define __BIGINT_H__

#include <stdbool.h>

typedef struct bigint bigint;

typedef bigint *(*bigint_binop) (bigint*, bigint*);

bigint *new_bigint (size_t capacity);

bigint *new_string_bigint (char *string);

void free_bigint (bigint*);

void print_bigint (bigint*, FILE*);

bigint *add_bigint (bigint*, bigint*);

bigint *sub_bigint (bigint*, bigint*);

bigint *mul_bigint (bigint*, bigint*);

void show_bigint (bigint*);

#endif

