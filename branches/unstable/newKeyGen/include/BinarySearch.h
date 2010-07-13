// Part of Laverna's Brute

#ifndef BINARYSEARCH_H_
#define BINARYSEARCH_H_

// The compare function type used in this binary search is compatible with any algorithm that works with qsort
typedef int (*compareFunc)(const void*, const void*);

template <class T>
unsigned int binarySearch(T* a, size_t low, size_t high, T target, compareFunc compare)
{
	while(low <= high)
	{
		int mid = ((high - low) / 2) + low;

		int compareResult = compare((const void*)&target, (const void*)&a[mid]);
		(compareResult > 0) ? compareResult = 1 : ((compareResult < 0) ? compareResult = -1 : compareResult = 0);

		switch(compareResult)
		{
		case -1:
			high = mid - 1;
			break;
		case 1:
			low = mid + 1;
			break;
		default:
			return mid;
		}
	}

	return -1;
}

#endif
