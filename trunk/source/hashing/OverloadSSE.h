#ifndef OVERLOADSSE_H_
#define OVERLOADSSE_H_

#include <emmintrin.h>

// Overloaded operators for dealing with the 128-bit sse datatype
inline __m128i operator +(const __m128i &a, const __m128i &b)
{
	return _mm_add_epi32(a, b);
}

inline __m128i operator +(const __m128i &a, const unsigned int b_sd[4])
{
	__m128i b = _mm_set_epi32(b_sd[3], b_sd[2], b_sd[1], b_sd[0]);
	return _mm_add_epi32(a, b);
}

inline __m128i operator +(const __m128i &a, const unsigned int b_scalar)
{
	__m128i b = _mm_set1_epi32(b_scalar);
	return _mm_add_epi32(a, b);
}

inline __m128i operator +=(__m128i &a, const __m128i &b)
{
	return a = a + b;
}

inline __m128i operator +=(__m128i &a, const int b_scalar)
{
	return a = a + b_scalar;
}

inline __m128i operator -(const __m128i &a, const __m128i &b)
{
	return _mm_sub_epi32(a, b);
}

inline __m128i operator ^(const __m128i &a, const __m128i &b)
{
	return _mm_xor_si128(a, b);
}

inline __m128i operator |(const __m128i &a, const __m128i &b)
{
	return _mm_or_si128(a, b);
}

inline __m128i operator &(const __m128i &a, const __m128i &b)
{
	return _mm_and_si128(a, b);
}

// SSE rotate function
inline __m128i ROTL_md(const __m128i &input, const int places)
{
	return _mm_or_si128(_mm_slli_epi32(input, places), _mm_srli_epi32(input, (32 - places)));
}

#endif