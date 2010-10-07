// Part of Laverna's Brute

#ifndef KEYGENERATOR_H_
#define KEYGENERATOR_H_

#include <vector>
#include <cstring>
#include <limits.h>

#include "CharacterSet.h"

////////////////////////////////////////////
// Plaintext key generator /////////////////
////////////////////////////////////////////

// This key generator operates on plaintext keys, and can be adapted to create the input for any hashing function.

class keyGenerator
{

protected:

	// Allow classes inheriting from the keygen to use their own constructor.
	keyGenerator()
	{
	}

public:

	keyGenerator(unsigned long long location, characterSet* charset)
		: location(location), charset(charset)
	{
		memset(key, 0, 16);
		integerToKey();
	}

	~keyGenerator()
	{
	}

	inline void incrementKey()
	{
		register size_t keyLength = strlen(key);

		for(register char* place = key; *place != 0; place++)
		{
			if(*place == charset->maxChar)
			{
				// Overflow, reset char at place
				*place = charset->minChar;

				if(!*(place+1))
				{
					// Carry, no space, insert char
					*(place+1) = charset->minChar;
					++keyLength;

					break;
				}
				else
				{
					continue;
				}
			}
			else
			{
				// Space available, increment char at place
				if(*place == charset->charSecEnd[0]) *place = charset->charSecBegin[0];
				else if(*place == charset->charSecEnd[1]) *place = charset->charSecBegin[1];

				(*place)++;

				break;
			}
		}
	}

    inline char* operator++() // Pre-increment
    {
            incrementKey();
            return key;
    }

    inline char* operator++(int) // Post-increment
    {
            memcpy(postIncrementRetval, key, 16);
            incrementKey();

            return postIncrementRetval;
    }

	void integerToKey()
	{
		register unsigned long long num = location;

		if(!num)
		{
			key[0] = charset->str[0];
		}
		else
		{
			num++;

			while(num)
			{
				num--;
				unsigned int remainder = num % charset->length;
				num /= charset->length;

				key[strlen(key)] = charset->str[remainder];
			}
		}
	}

	unsigned long long keyToInteger(char* input)
	{
		std::string stl_str = charset->str;

		unsigned long long retval = 0;
		unsigned int numChars = (unsigned int)strlen(input);

		for(unsigned int i = 0; i < numChars; i++)
		{
			unsigned long long temp = 1;

			for(unsigned int j = 0; j < i; j++)
					temp *= charset->length;

			retval += (temp * (stl_str.find(input[i]) + 1));
		}

		return retval - 1;
	}

	inline unsigned long long keyToInteger()
	{
		return keyToInteger(key);
	}

	inline char* getKey()
	{
		return key;
	}

private:

	unsigned long long location;

	characterSet* charset;

	char key[16];

	// We need a place to store the key for the post increment operation.
	char postIncrementRetval[16];
};

#endif

