// Part of Laverna's Brute

#ifndef KEYGENERATOR_H_
#define KEYGENERATOR_H_

#include <vector>
#include <cstring>
#include <limits.h>

#include "Pow.h"
#include "CharacterSet.h"

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
		for(int i = 0; i < 16; i++)
			key[i] = 0;

		charsetStr = charset->getCharsetStr();
		integerToKey();
	}

	~keyGenerator()
	{
	}

	inline void incrementKey()
	{
		register size_t keyLength = strlen(key);

		for(register char* place = key; place; place++)
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
			key[0] = charsetStr[0];
		}
		else
		{
			num++;

			while(num)
			{
				num--;
				unsigned int remainder = num % charset->length;
				num /= charset->length;

				key[strlen(key)] = charsetStr[remainder];
			}
		}
	}

	inline unsigned long long keyToInteger()
	{
		// TODO
		return 0;
	}

	inline char* getKey()
	{
		return key;
	}

private:

	unsigned long long location;

	characterSet* charset;
	std::string charsetStr;

	char key[16];

	// We need a place to store the key for the post increment operation.
	char postIncrementRetval[16];
};

#endif

