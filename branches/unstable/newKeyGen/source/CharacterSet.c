#include "CharacterSet.h"

characterSet* createCharacterSet(char min, char max, char charsec0, char charsec1, char charsec2, char charsec3)
{
	int i, j, chr;

	characterSet* newCharset = (characterSet*)malloc(sizeof(characterSet));
	newCharset->str = (char*)malloc(sizeof(char) * 256);

	// Set the limits of the character set
	newCharset->minChar = min;
	newCharset->maxChar = max;

	// Character set sections are used to specify sections of the character set which are not contiguous
	newCharset->charSecEnd[0] = charsec0;
	newCharset->charSecBegin[0] = charsec1;
	newCharset->charSecEnd[1] = charsec2;
	newCharset->charSecBegin[1] = charsec3;

	// Generate the plaintext string version of the character set
	for(chr = newCharset->minChar, i = 0; chr != (newCharset->maxChar + 1); chr++, i++)
	{
		for(j = 0; j < 2; j++) if(chr == newCharset->charSecEnd[j]) chr = newCharset->charSecBegin[j];
		newCharset->str[i] = chr;
	}

	newCharset->str[i] = 0;

	// Set the length of the character set
	newCharset->length = strlen(newCharset->str);

	return newCharset;
}

void destroyCharacterSet(characterSet* charset)
{
	free(charset->str);
	free(charset);
}
