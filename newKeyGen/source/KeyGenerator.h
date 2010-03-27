// Part of Laverna's Brute

#ifndef KEYGENERATOR_H_
#define KEYGENERATOR_H_

#include <vector>
#include <cstring>
#include <limits.h>

#include "Pow.h"

// This code was inspired by Daniel Niggebrugge's method of handling character sets in EnTibr
// His blog can be found here: http://blog.distracted.nl/
struct characterSet
{
	characterSet()
	{
	}

	characterSet(unsigned int len, int min, int max, int charsec0, int charsec1, int charsec2, int charsec3)
	{
		init(length, min, max, charsec0, charsec1, charsec2, charsec3);
	}

	void init(unsigned int len, int min, int max, int charsec0, int charsec1, int charsec2, int charsec3)
	{
		length = len;
		minChar = min;
		maxChar = max;

		charSecEnd[0] = charsec0;
		charSecBegin[0] = charsec1;
		charSecEnd[1] = charsec2;
		charSecBegin[1] = charsec3;
	}

	std::string getCharsetStr()
	{
		std::string retval;

		for(int chr = minChar; chr != maxChar; chr++)
		{
			for(int i = 0; i < 2; i++) if(chr == charSecEnd[i]) chr = charSecBegin[i];
			retval += chr;
		}

		return retval;
	}

	int minChar, maxChar;

	// charSec = character set section
	int charSecEnd[2], charSecBegin[2];

	unsigned int length;
};

class keyGenerator
{
public:

        keyGenerator(unsigned long long location, characterSet* charset)
                : charset(charset), location(location)
        {
			// Fourteen characters is just about the limit of what is feasible to crack.
			key = new char[15];
			
			for(int i = 0; i < 15; i++)
				key[i] = 0;

			charsetStr = charset->getCharsetStr();
			integerToKey();
        }

        ~keyGenerator()
        {
			delete [] key;
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

					if(*(place+1) == 0)
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

        inline std::string operator++()
        {
                incrementKey();
                return getKey();
        }

        inline std::string operator++(int)
        {
                std::string retval = key;
                incrementKey();

                return retval;
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

	inline std::string getKey()
	{
		return key;
	}

	inline void getMultipleKeys(register std::string* output, int num)
	{
		for(int i = 0; i < num; i++)
		{
			output[i] = key;
			incrementKey();
		}
	}

protected:

	characterSet* charset;
	std::string charsetStr;

    char* key;

	unsigned long long location;
};

#endif

