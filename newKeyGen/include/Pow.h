#ifndef POW_H_
#define POW_H_

template <typename T>
T& pow(const T& base, const T& power)
{
	T result = 1;

	for(T i = 0; i < power; i++)
		result *= base;

	return result;
}

#endif