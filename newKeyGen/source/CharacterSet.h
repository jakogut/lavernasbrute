#ifndef CHARACTERSET_H_
#define CHARACTERSET_H_

// This code was inspired by Daniel Niggebrugge's method of handling character sets in EnTibr
// His blog can be found here: http://blog.distracted.nl/
struct characterSet
{
	characterSet()
	{
	}

	void init(char min, char max, char charsec0, char charsec1, char charsec2, char charsec3)
	{
		// Set the limits of the character set
		minChar = min;
		maxChar = max;

		// Character set sections are used to specify sections of the character set which are not contiguous
		charSecEnd[0] = charsec0;
		charSecBegin[0] = charsec1;
		charSecEnd[1] = charsec2;
		charSecBegin[1] = charsec3;

		// Generate the plaintext string version of the character set
		for(int chr = minChar; chr != (maxChar + 1); chr++)
		{
			for(int i = 0; i < 2; i++) if(chr == charSecEnd[i]) chr = charSecBegin[i];
			str += chr;
		}

		// Set the length of the character set
		length = str.length();
	}

	int minChar, maxChar;

	// charSec = character set section
	char charSecEnd[2], charSecBegin[2];

	// Plaintext string of the character set
	std::string str;

	// Character set length
	int length;
};

#endif