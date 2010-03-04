#ifndef POW_H_
#define POW_H_

template <typename T>
void pow(const T& base, const T& power, T& output)
{
	output = 1;

	for(T i = 0; i < power; i++)
		output *= base;
}

#endif