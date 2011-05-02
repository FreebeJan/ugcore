// author: Andreas Vogel

#include "string_util.h"
#include <algorithm>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cctype>
#include "common/common.h"

namespace ug{

void RemoveWhitespaceFromString(std::string& string)
{
	string.erase(std::remove_if(string.begin(), string.end(), isspace), string.end());
}

// help function to tokenize the parameter string
void TokenizeString(const std::string& str, std::vector<std::string>& tokens, const char delimiter)
{
	tokens.clear();
	std::stringstream tokenstream;
	tokenstream << str;
	std::string token;

	while ( std::getline (tokenstream, token, delimiter ) )
	{
			tokens.push_back(token);
	}
}

std::string TrimString(const std::string& str)
{
	const size_t start = str.find_first_not_of(" \t");
	const size_t end = str.find_last_not_of(" \t");
	if(start == std::string::npos || end == std::string::npos) return "";
	return str.substr(start, end - start + 1);
}

/// returns the number of digits of an integer (expressed with base 10)
int NumberOfDigits(int n)
{
//	a 0 has 1 digit
	if (n == 0) return 1;

//	divide by 10 until
    int cnt = 0;
    while (n > 0)
    {
    	++cnt;
    	n /= 10;
    }

//	return number of digits
    return cnt;
}

///	appends a counter number to a string
void AppendCounterToString(std::string& str, std::string indicator,
                           int counter, int maxCounter)
{
//	check correct usage
	if(maxCounter >= 0)
		UG_ASSERT(counter <= maxCounter, "Wrong usage of function");

//	get number of digits needed for counter field (default is 4)
	int numDigits = 4;
	if(maxCounter >= 0) numDigits = NumberOfDigits(maxCounter);

//	create stringstream
	std::stringstream ss;

//	print stringstream
	ss << indicator << std::setfill ('0') << std::setw (numDigits) << counter;

//	append to string
	str.append(ss.str());
}


//sreiter - Implementation is copied from some book or website. Can't remember...
template <> unsigned long hash_key(const std::string& key)
{

	unsigned long hash = 5381;
	const char* str = key.c_str();
	int c;

	while((c = *str)){
		hash = hash * 33 + c;
		++str;
	}

	return hash;
}

}

