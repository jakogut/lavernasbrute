// Part of Laverna's Brute

#ifndef BINARYSEARCH_H_
#define BINARYSEARCH_H_

template < typename T, typename iterT >
iterT binarySearch(iterT begin, iterT end, const T* target)
{
	iterT arrayEnd = end;

	while(begin < end)
	{
		iterT mid = ((end - begin - 1) / 2) + begin;

		if(*target < *mid) end = mid;
		else if(*mid < *target) begin = mid + 1;
		else return mid;
	}

	return arrayEnd; 
}

#endif
