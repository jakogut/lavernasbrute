#ifndef INT128_H_
#define INT128_H_

// Custom 128-bit datatype used to store and compare the results of a weakened hash operation.
struct int128
{
	// We don't need a unified number. Four 32-bit unsigned integers work just fine.
	unsigned int storage[4];

	/* Assignment operation that takes a 32-bit integer array of four elements.
	This makes assignment of values a shorter and less painful operation. */
	void operator=(const unsigned int input[4])
	{
		for(int i = 0; i < 4; i++)
			storage[i] = input[i];
	}
};

bool operator==(const int128 o1, const int128 o2)
{
	/* Here we use XOR to compare values. If an XOR of two numbers returns zero, we know they're equal.
	Using a bitwise operation to do this should be pretty fast. Also, nesting the comparisons of each integer
	should improve speed by ensuring that no unnecessary comparisons are made. */
	if((o1.storage[0] ^ o2.storage[0]) == 0)
		if((o1.storage[1] ^ o2.storage[1]) == 0)
			if((o1.storage[2]^ o2.storage[2]) == 0)
				if((o1.storage[3] ^ o2.storage[3]) == 0)
					return true;

	return false;
}

std::size_t hash_value(int128 const &input)
{
	boost::hash<unsigned long long> hasher;
	unsigned long long hashVal = input.storage[0];

	for(int i = 1; i < 3; i++)
	{
		hashVal *= 37;
		hashVal += input.storage[1];
	}

	return hasher(hashVal);
}

#endif