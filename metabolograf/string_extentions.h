#ifndef string_extentions_h
#define string_extentions_h

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace std;
template<class T>
string operator+(const string& str, const T& n)
{
    stringstream ss("");
    ss<< str << n;
    return ss.str();

}

template<class T>
string ToString(const T& value, int count=0)
{
	stringstream ss("");
	if(count<=0)
		ss << value;
	else
		ss << setfill('0')<<setw(count)<< value;
    return ss.str();
}




#endif /* string_extentions_h */
