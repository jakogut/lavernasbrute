#ifndef OVERLOADSSE_H_
#define OVERLOADSSE_H_

#include <emmintrin.h>

// SSE rotate macro
#define ROTL_md(num, places) (_mm_or_si128(_mm_slli_epi32(num, places), _mm_srli_epi32(num, (32 - places))))

#ifdef WIN32
// Overloaded operators for dealing with the 128-bit sse datatype
inline __m128i operator +(const __m128i &a, const __m128i &b)
{
	return _mm_add_epi32(a, b);
}

inline __m128i operator +(const __m128i &a, const unsigned int b_sd[4])
{
	return _mm_add_epi32(a, _mm_set_epi32(b_sd[3], b_sd[2], b_sd[1], b_sd[0]));
}

inline __m128i operator +=(__m128i &a, const __m128i &b)
{
	return a = _mm_add_epi32(a, b);
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
#endif

#endif