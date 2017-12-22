
#pragma once
#include <string>
#include <sstream>
#include <Windows.h>
using std::string;
using std::stringstream;

class Ini
{
private:
	string file_name_;
public:
	Ini(string file_name)
	{	
		char buffer[MAX_PATH];
		GetCurrentDirectory(sizeof(buffer), buffer);
		file_name_ = (string)buffer + (string)"\\" + file_name;
	}

	void Delete()
	{
		DeleteFile(file_name_.c_str());
	}

	bool IsExists()
	{
		return PathFileExists(file_name_.c_str());
	}

	void Write(const string& section, const string& field, const string& value)
	{
		WritePrivateProfileString(section.c_str(), field.c_str(), value.c_str(), file_name_.c_str());
	}

	template <typename T>
	void Write(const string& section, const string& field, const T& value)
	{
		stringstream ss;
		ss << value;
		WritePrivateProfileString(section.c_str(), field.c_str(), ss.str().c_str(), file_name_.c_str());
	}

	string Read(const string& section, const string& field, const string& default_value)
	{
		char out[256];
		
		DWORD dd = GetPrivateProfileString(
				section.c_str(), field.c_str(), default_value.c_str(),
				out,
				256,
				file_name_.c_str());
		return string(out);
	}

	
	template <typename T>
	T Read(const string& section, const string& field, const T& default_value)
	{
		stringstream ss;
		ss << default_value;// конвертируем T в строку 
		string str_result = Read(section, field, ss.str());
		if (ss.str() == str_result) return default_value;//если совпало

		ss = stringstream(str_result);
		double result;
		ss >> result;// string to T
		if (ss)// если без ошибок
			return result;
		else return default_value;//если с ошибкой, то возвращаем значение по умолчанию
	}
};