#ifndef MESSAGEGENERATOR_NTLM_H_
#define MESSAGEGENERATOR_NTLM_H_

#include "KeyGenerator.h"
#include "hashing/MD4.h"

class messageGenerator_NTLM : protected keyGenerator

{

public:

	messageGenerator_NTLM(unsigned long long location, characterSet* charset)
		: location(location), charset(charset)
	{
		memset(ctx.message.uint32, 0, 16*4);

		integerToMessage();
	}

	~messageGenerator_NTLM()
	{
	}

	inline unsigned int findMessageLength()
	{
		for(register unsigned int padByte = 0; padByte < 32; ++padByte)
			if(ctx.message.uint8[padByte << 1] == 0x80)
				return padByte;

		return 0;
	}

	char* messageToKey()
	{
		for(unsigned int i = 0; i < findMessageLength(); i++)
			key[i] = ctx.message.uint8[i << 1];

		return key;
	}

	inline void incrementMessage()
	{
		register unsigned int messageLength;

		for(messageLength = 0; messageLength < 32; ++messageLength)
			if(ctx.message.uint8[messageLength << 1] == 0x80)
				break;

		register unsigned int i;

		for(i = 0; i < 32; ++i)
		{
			if(ctx.message.uint8[i << 1] == charset->maxChar)
			{
				// Overflow, reset char at place
				ctx.message.uint8[i << 1] = charset->minChar;

				if(ctx.message.uint8[(i + 1) << 1] == 0x80)
				{
					// Carry, no space, insert char
					ctx.message.uint8[(i + 1) << 1] = charset->minChar;

					// Move the padding byte forward
					ctx.message.uint8[(i + 2) << 1] = 0x80;

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
				if(ctx.message.uint8[i << 1] == charset->charSecEnd[0]) ctx.message.uint8[i << 1] = charset->charSecBegin[0];
				else if(ctx.message.uint8[i << 1] == charset->charSecEnd[1]) ctx.message.uint8[i << 1] = charset->charSecBegin[1];

				ctx.message.uint8[i << 1]++;

				break;
			}
		}

		// Append the length
		ctx.message.uint32[14] = messageLength << 4;
	}

	inline hashContext* operator++()
	{
		incrementMessage();
		return &ctx;
	}

	inline hashContext* operator++(int)
	{
		// Too lazy to reimplement this properly.
		return ++*this;
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

		memset(ctx.message.uint32, 0, 16*4);

		for(;i<length/2;i++)
			ctx.message.uint32[i] = key[2*i] | (key[2*i+1]<<16);

		if(length%2==1)
			ctx.message.uint32[i] = key[length-1] | 0x800000;
		else
			ctx.message.uint32[i]=0x80;

		ctx.message.uint32[14] = length << 4;

	}

protected:

	unsigned long long location;

	characterSet* charset;

	hashContext ctx;

	char key[16];

};

#endif