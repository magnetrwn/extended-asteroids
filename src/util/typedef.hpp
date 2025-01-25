#ifndef TYPEDEF_HPP_
#define TYPEDEF_HPP_

#include <cstdint>
#include <limits>
#include <raylib.h>

typedef std::uint8_t u8;
typedef std::uint16_t u16;
typedef std::uint32_t u32;
typedef std::uint64_t u64;
typedef std::size_t usize;

typedef std::int8_t i8;
typedef std::int16_t i16;
typedef std::int32_t i32;
typedef std::int64_t i64;

typedef signed long isize;

typedef float f32;
typedef double f64;

typedef Vector2 f32_2;
typedef Rectangle f32_4;

constexpr u8 U8MAX = std::numeric_limits<u8>::max();
constexpr u16 U16MAX = std::numeric_limits<u16>::max();
constexpr u32 U32MAX = std::numeric_limits<u32>::max();
constexpr u64 U64MAX = std::numeric_limits<u64>::max();
constexpr usize USIZEMAX = std::numeric_limits<usize>::max();

constexpr f32_2 VECTOR2ZERO = { 0.0f, 0.0f };

#endif