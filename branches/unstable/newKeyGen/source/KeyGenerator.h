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
		unsigned long long retval = 0;
		unsigned int numChars = (unsigned int)strlen(input);

		for(unsigned int i = 0; i < numChars; i++)
		{
			unsigned long long temp = 1;

			for(unsigned int j = 0; j < i; j++)
					temp *= charset->length;

			retval += (temp * (charset->str.find(input[i]) + 1));
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

////////////////////////////////////////////
// Tailored key generators /////////////////
////////////////////////////////////////////

/* The key generators tailored to specific algorithms work in mostly the same way as the standard plaintext keygen.
The main difference is that tailored key generators operate on messages which are used directly as input to the 
hashing function, rather than plaintext keys. A tailored key generator is much more efficient, but one must be 
written for each algorithm. */

class keyGenerator_NTLM : protected keyGenerator
{
public:

	keyGenerator_NTLM(unsigned long long location, characterSet* charset)
		: location(location), charset(charset)
	{
		memset(message.uint32, 0, 16*4);

		integerToMessage();
	}

	~keyGenerator_NTLM()
	{
	}

	unsigned int findMessageLength()
	{
		unsigned int retval = 0;

		for(int i = 0; i < 32; i++)
		{
			if((message.uint16[i] & 0x800000) == 0x800000 || message.uint16[i] == 0x80)
				retval = i + 1;
		}

		return retval;
	}

	void incrementMessage()
	{
		register size_t messageLength = findMessageLength();

		// Remove padding
		if(messageLength % 2 == 1)
			message.uint32[messageLength / 2 - 1] ^= 0x800000;
		else if(message.uint32[messageLength / 2 - 1] == 0x80)
			message.uint32[messageLength / 2 - 1] = 0;

		// Wide character pointer, used as an iterator
		wchar_t* place = (wchar_t*)message.uint16;

		for(; *place != 0; place++)
		{
			if(*place == (wchar_t)charset->maxChar)
			{
				// Overflow, reset char at place
				*place = (wchar_t)charset->minChar;

				if(!*(place+1))
				{
					// Carry, no space, insert char
					*(place+1) = (wchar_t)charset->minChar;
					++messageLength;

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
				if(*place == (wchar_t)charset->charSecEnd[0]) *place = (wchar_t)charset->charSecBegin[0];
				else if(*place == (wchar_t)charset->charSecEnd[1]) *place = (wchar_t)charset->charSecBegin[1];

				(*place)++;

				break;
			}
		}

		// Padding
		/*if(messageLength % 2 == 1)
			message.uint16[(messageLength / 2) - 1] |= 0x800000;
		else 
			message.uint16[(messageLength / 2) - 1] = 0x80;*/

		// Append the length to the message
		message.uint32[14] = (unsigned int)messageLength << 4;
	}

	unsigned int* operator++()
	{
		incrementMessage();

		return message.uint32;
	}

	unsigned int* operator++(int)
	{
		memcpy(postIncrementRetval, message.uint32, 16*4);
		incrementMessage();

		return postIncrementRetval;
	}

	void integerToMessage()
	{
		memset(key, 0, 16);

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

		int i=0;
		int length=(int)(strlen(key));

		memset(message.uint32,0,16*4);
		
		for(;i<length/2;i++)	
			message.uint32[i] = key[2*i] | (key[2*i+1]<<16);
	 
		if(length%2==1)
			message.uint32[i] = key[length-1] | 0x800000;
		else
			message.uint32[i]=0x80;

		message.uint32[14] = length << 4;
	}

protected:

	unsigned long long location;

	characterSet* charset;

	// The message is the input to the hashing function.
	union
	{
		unsigned int uint32[16];
		unsigned short uint16[32];
	} message;

	// The key is the text string that becomes the message.
	char key[16];

	unsigned int postIncrementRetval[16];
};

#endif

