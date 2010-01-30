// Part of Laverna's Brute

#ifndef KEYGENERATOR_H_
#define KEYGENERATOR_H_

class keyGenerator
{
public:

	keyGenerator(unsigned long long location, char* charset)
		: charset(charset), location(location)
	{
		charsetLength = strlen(charset);
		integerToKey();
	}

	~keyGenerator()
	{
	}

	void incrementKey()
	{
		for(unsigned int place = 0; place < key.length(); place++)
		{
			if(key[place] == charset[charsetLength - 1])
			{
				// Overflow, reset char at place
				key[place] = charset[0];
				keyIndices[place] = 0;

				if((key.length() - 1) < (place + 1))
				{
					// Carry, no space, insert char
					key.insert(key.begin(), charset[0]);
					keyIndices.insert(keyIndices.begin(), 0);
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
				key[place] = charset[keyIndices[place] + 1];
				keyIndices[place] = keyIndices[place] + 1;
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
		unsigned long long num = location;
		unsigned int remainder;

		key.clear();

		do
		{
			remainder = num % charsetLength;

			key.insert(key.begin(), charset[remainder]);
			keyIndices.insert(keyIndices.begin(), remainder);

		} while(num /= charsetLength);
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

	inline void getMultipleKeys(std::string* output, int num)
	{
		for(int i = 0; i < num; i++)
		{
			output[i] = key;

			incrementKey();
		}
	}

protected:

	char* charset;
	unsigned int charsetLength;

	std::string key;
	std::vector<unsigned int> keyIndices;

	unsigned long long location;
};

#endif