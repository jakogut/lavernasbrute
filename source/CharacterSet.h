#ifndef CHARACTERSET_H_
#define CHARACTERSET_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <string.h>

typedef struct
{
	int minChar, maxChar;

	// charSec = character set section
	char charSecEnd[2], charSecBegin[2];

	// Plaintext string of the character set
	char* str;

	// Character set length
	unsigned int length;
} characterSet;

characterSet* createCharacterSet(char min, char max, char charsec0, char charsec1, char charsec2, char charsec3);

void destroyCharacterSet(characterSet* charset);

#ifdef __cplusplus
}
#endif

#endif