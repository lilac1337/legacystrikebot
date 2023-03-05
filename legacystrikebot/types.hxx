#pragma once

#include <stdint.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef int_fast8_t if8;
typedef int_fast16_t if16;
typedef int_fast32_t if32;
typedef int_fast64_t if64;

typedef int_least8_t il8;
typedef int_least16_t il6;
typedef int_least32_t il32;
typedef int_least64_t il64;

typedef intmax_t im;

typedef intptr_t iptr;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef uint_fast8_t uf8t;
typedef uint_fast16_t uf16;
typedef uint_fast32_t uf32;
typedef uint_fast64_t uf64;

typedef uint_least8_t ul8;
typedef uint_least16_t ul16;
typedef uint_least32_t ul32;
typedef uint_least64_t ul64;

typedef uintmax_t um;

typedef uintptr_t uptr;

enum Types {
	DiscordId,
	SteamId,
	Ticket
};