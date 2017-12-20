#ifndef string_extentions_hpp
#define string_extentions_hpp

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



/*double _ToDouble(string str)
{
    stringstream ss(str);
    double temp=0;
    ss >> temp;
    return temp;
}*/



#endif /* string_extentions_hpp */
