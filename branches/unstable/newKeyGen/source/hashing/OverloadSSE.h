#ifndef OVERLOADSSE_H_
#define OVERLOADSSE_H_

#include <emmintrin.h>

#ifdef WIN32
// Overloaded operators for dealing with the 128-bit sse datatype
inline __m128i& operator +(const __m128i &a, const __m128i &b)
{
	return _mm_add_epi32(a, b);
}

inline __m128i& operator +=(__m128i &a, const __m128i &b)
{
	return a = _mm_add_epi32(a, b);
}

inline __m128i& operator -(const __m128i &a, const __m128i &b)
{
	return _mm_sub_epi32(a, b);
}

inline __m128i& operator ^(const __m128i &a, const __m128i &b)
{
	return _mm_xor_si128(a, b);
}

inline __m128i& operator |(const __m128i &a, const __m128i &b)
{
	return _mm_or_si128(a, b);
}

inline __m128i& operator &(const __m128i &a, const __m128i &b)
{
	return _mm_and_si128(a, b);
}
#endif

#endif