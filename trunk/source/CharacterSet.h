#ifndef CHARACTERSET_H_
#define CHARACTERSET_H_

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

#endif