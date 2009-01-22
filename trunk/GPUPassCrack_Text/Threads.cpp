#include "Threads.h"
#include "MasterThread.h"

void threads::operator()()
{
	while (!masterThread::getSuccess())
	{
		generatePasswdString(getPasswd());

		if(passwdNumericalString == getPasswd())
		{
			masterThread::setSuccess(true);
			break;
		}
		else
		{
			incrementIterations();

			if(!masterThread::getSilent())
			{
				if(threads::getIterations() % masterThread::getInterval() == 0)
				{
					masterThread::writeIterations();
				}
			}
		}
	}
}

void threads::generatePasswdString(string passwd)
{
	setPasswdNumericalString(generateRandString(passwd.length()));
	setPasswdNumericalString(parseNumericalString(getPasswdNumericalString()));
}

//We're only reading the variable here, so there's no need to lock it. It's not imperative that we have
//the variable returned every time either. That's good, because sometimes the variable won't be returned
//as a result of being locked by the incrementIterations function.
__int64 threads::getIterations()
{
	//boost::mutex::scoped_lock scoped_lock(IterationsMutex);
	return iterations;
}

void threads::incrementIterations()
{
	//boost::mutex::scoped_lock lock(IterationsMutex);
	iterations++;
}

string threads::getPasswdNumericalString()
{
	//boost::recursive_mutex::scoped_lock scoped_lock(PasswdNumericalStringMutex);
	return passwdNumericalString;
}

void threads::setPasswdNumericalString(string write)
{
	boost::recursive_mutex::scoped_lock scoped_lock(PasswdNumericalStringMutex);
	passwdNumericalString = write;
}

string threads::getPasswd()
{
	boost::recursive_mutex::scoped_lock scoped_lock(PasswdMutex);
	return passwd;
}

void threads::writePasswd(string write)
{
	boost::recursive_mutex::scoped_lock scoped_lock(PasswdMutex);
	passwd = write;
}

//Replace 'a' with 'b' in string "source," the characters replaced are controlled with length.
string threads::replaceAll(string source, string a, string b)
{
	string result = source;
	int position = source.find(a);
	int length = a.length();

	while (position != string::npos) 
	{
		result.replace( position, length, b );
		position = result.find(a, position + 1);
	} 

	return result;
}

int threads::roundToNearestTen(int round)
{
	round = (round / 10);
	return round * 10;
}

string threads::parseNumericalString(string passwdNumericalString)
{     
	string copyString = passwdNumericalString;

	//If our loop begins at zero, and the string is 3 characters or less, it throws an exception
	//due to division by zero. This little hack, while messy, fixes that.
	for(int i = 0; i < (passwdNumericalString.length() / 3); i++)
	{
		string stringKey = passwdNumericalString.substr((i * 3), 3);
		int key = atoi(stringKey.c_str());

		switch(key)
		{
			//Lowercase Alphabet
			case 110:
				copyString = replaceAll(copyString, "110", "a");
				break;

			case 120:
				copyString = replaceAll(copyString, "120", "b");
				break;

			case 130:
				copyString = replaceAll(copyString, "130", "c");
				break;

			case 140:
				copyString = replaceAll(copyString, "140", "d");
				break;

			case 150:
				copyString = replaceAll(copyString, "150", "e");
				break;

			case 160:
				copyString = replaceAll(copyString, "160", "f");
				break;

			case 170:
				copyString = replaceAll(copyString, "170", "g");
				break;

			case 180:
				copyString = replaceAll(copyString, "180", "h");
				break;

			case 190:
				copyString = replaceAll(copyString, "190", "i");
				break;

			case 210:
				copyString = replaceAll(copyString, "210", "j");
				break;

			case 220:
				copyString = replaceAll(copyString, "220", "k");
				break;

			case 230:
				copyString = replaceAll(copyString, "230", "l");
				break;

			case 240:
				copyString = replaceAll(copyString, "240", "m");
				break;

			case 250:
				copyString = replaceAll(copyString, "250", "n");
				break;

			case 260:
				copyString = replaceAll(copyString, "260", "o");
				break;

			case 270:
				copyString = replaceAll(copyString, "270", "p");
				break;

			case 280:
				copyString = replaceAll(copyString, "280", "q");
				break;

			case 290:
				copyString = replaceAll(copyString, "290", "r");
				break;

			case 310:
				copyString = replaceAll(copyString, "310", "s");
				break;

			case 320:
				copyString = replaceAll(copyString, "320", "t");
				break;

			case 330:
				copyString = replaceAll(copyString, "330", "u");
				break;

			case 340:
				copyString = replaceAll(copyString, "340", "v");
				break;

			case 350:
				copyString = replaceAll(copyString, "350", "w");
				break;

			case 360:
				copyString = replaceAll(copyString, "360", "x");
				break;

			case 370:
				copyString = replaceAll(copyString, "370", "y");
				break;

			case 380:
				copyString = replaceAll(copyString, "380", "z");
				break;

			//Uppercase Alphabet
			case 390:
				copyString = replaceAll(copyString, "390", "A");
				break;

			case 410:
				copyString = replaceAll(copyString, "410", "B");
				break;

			case 420:
				copyString = replaceAll(copyString, "420", "C");
				break;

			case 430:
				copyString = replaceAll(copyString, "430", "D");
				break;

			case 440:
				copyString = replaceAll(copyString, "440", "E");
				break;

			case 450:
				copyString = replaceAll(copyString, "450", "F");
				break;

			case 460:
				copyString = replaceAll(copyString, "460", "G");
				break;

			case 470:
				copyString = replaceAll(copyString, "470", "H");
				break;

			case 480:
				copyString = replaceAll(copyString, "480", "I");
				break;

			case 490:
				copyString = replaceAll(copyString, "490", "J");
				break;

			case 510:
				copyString = replaceAll(copyString, "510", "K");
				break;

			case 520:
				copyString = replaceAll(copyString, "520", "L");
				break;

			case 530:
				copyString = replaceAll(copyString, "530", "M");
				break;

			case 540:
				copyString = replaceAll(copyString, "540", "N");
				break;

			case 550:
				copyString = replaceAll(copyString, "550", "O");
				break;

			case 560:
				copyString = replaceAll(copyString, "560", "P");
				break;

			case 570:
				copyString = replaceAll(copyString, "570", "Q");
				break;

			case 580:
				copyString = replaceAll(copyString, "580", "R");
				break;

			case 590:
				copyString = replaceAll(copyString, "590", "S");
				break;

			case 610:
				copyString = replaceAll(copyString, "610", "T");
				break;

			case 620:
				copyString = replaceAll(copyString, "620", "U");
				break;

			case 630:
				copyString = replaceAll(copyString, "630", "V");
				break;

			case 640:
				copyString = replaceAll(copyString, "640", "W");
				break;

			case 650:
				copyString = replaceAll(copyString, "650", "X");
				break;

			case 660:
				copyString = replaceAll(copyString, "660", "Y");
				break;

			case 670:
				copyString = replaceAll(copyString, "670", "Z");
				break;

			//Numbers
			case 680:
				copyString = replaceAll(copyString, "680", "0");
				break;

			case 690:
				copyString = replaceAll(copyString, "690", "1");
				break;

			case 710:
				copyString = replaceAll(copyString, "710", "2");
				break;

			case 720:
				copyString = replaceAll(copyString, "720", "3");
				break;

			case 730:
				copyString = replaceAll(copyString, "730", "4");
				break;

			case 740:
				copyString = replaceAll(copyString, "740", "5");
				break;

			case 750:
				copyString = replaceAll(copyString, "750", "6");
				break;

			case 760:
				copyString = replaceAll(copyString, "760", "7");
				break;

			case 770:
				copyString = replaceAll(copyString, "770", "8");
				break;

			case 780:
				copyString = replaceAll(copyString, "780", "9");
				break;
		}
	}

	return copyString;
}

//Generate random string of numbers to be converted to text
string threads::generateRandString(int length)
{
	string passwdNumericalString;
	char* bufferString = new char[length];
	int randTemp;
 
	while((unsigned)(passwdNumericalString.length()) < (unsigned)(length * 3))
	{
		randTemp = random();
		randTemp = roundToNearestTen(randTemp);

		//Safeguard for if the number sequence representing characters goes out of scope.
		if(randTemp < 110 || randTemp == 200 || randTemp == 300 || randTemp == 400 || randTemp == 500 || randTemp == 600 || randTemp == 700 || randTemp > 720)
		{
			continue;
		}
		else
		{
			itoa(randTemp, bufferString, 10);
			passwdNumericalString.append(bufferString);
		}
	}

	//Free the memory used by our buffer string. Otherwise we get a memory leak. 
	delete[] bufferString;

	return passwdNumericalString;
}