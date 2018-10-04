#ifndef Highlighter_h
#define Highlighter_h

#include "Dictionary.h"

using namespace std;



class Highlighter
{
	CRichEditCtrl *Editor;
    Dictionary dict;
	bool fullredraw;
	UINT_PTR timer;

public:
	Highlighter()
	{
	   
	   Editor = nullptr;
	   fullredraw = true;
	   timer = 0;
	}
	Highlighter(CRichEditCtrl *_editor)
	{
		
		Editor = _editor;
		fullredraw = true;
	};

	void RestartTimer(HWND& hwnd)
	{
		StopTimer(hwnd);
		timer = SetTimer(hwnd,(int)2, 1000, NULL);
		
	}

	void StopTimer(HWND& hwnd)
	{
		if (timer != 0)
			KillTimer(hwnd, timer);
	}

	void Run()
	{
		if (!Editor) return;

		int len = Editor->GetTextLength() + 1;
		char * buf = new char[len + 1];
		Editor->GetWindowTextA(buf, len);
		string text = buf;
		delete[] buf;


		size_t f = -1;
		while (1)
		{
			f = text.find("\r\n");
			if (f == std::string::npos)
				break;
			text.replace(f, 2, " ");
		}
		

		CHARFORMAT charformat;
		charformat.cbSize = sizeof(charformat);
		charformat.dwMask = CFM_COLOR;
		charformat.dwEffects = CFE_ITALIC | ~CFE_AUTOCOLOR;



		CHARRANGE crBefore;
		Editor->GetSel(crBefore);

		CHARRANGE all = { 0, (LONG)text.length() };
		SetNormal(all);
		//
		int shift = 0;
		size_t found = 0;
		size_t sstart = 0;
		int c=0;
		while (1)
		{
			size_t start = text.find("{", found);
			if (start == std::string::npos)
				break;
			size_t end = text.find("}", start + 1);
			if (end == std::string::npos)
				break;
			
			//if (found < 0) found = 0;
			int length = start - found;

			string subtext = "";
			if(length>0) subtext = text.substr(found, length);
			string code = text.substr(start + 1, end - start - 1);

			
			
			charformat.crTextColor = getColor(5);
			Editor->SetSel(found, found+length);
			Editor->SetSelectionCharFormat(charformat);
			//charformat.crTextColor = getColor(100);


			shift += subtext.length()+1;
			HighliteCode(code, shift, charformat);
			

			found = end+1;

		}
		charformat.crTextColor = getColor(5);
		Editor->SetSel(found, text.length());
		Editor->SetSelectionCharFormat(charformat);




		SetNormal(crBefore);
	}

	

	int count(string& text, string s)
	{
		int count = 0;
		size_t found = -1;
		while (1)
		{
			found = text.find(s, found + 1);
			if (found == std::string::npos)
				break;
			count++;
		}
		return count;
	}
	
	void HighliteCode(string& text, int& shift, CHARFORMAT& charformat)
	{
		stringstream ss(text);
		string line;
		
		
		while (getline(ss, line, '\n'))
		{
			size_t found = 0;
			while (1)
			{
				size_t start = text.find("\"", found);
				if (start == std::string::npos)
					break;
				size_t end = text.find("\"", start + 1);
				if (end == std::string::npos)
					break;
				found = end + 1;
				charformat.crTextColor = getColor(2);
				Editor->SetSel(shift+start, shift+end);
				Editor->SetSelectionCharFormat(charformat);

			}
			//подсвечиваем операторы
			for (auto& word : dict.operators)
				HighlightLine(line, word.first, word.second, charformat, shift);
			
			//подсвечиваем функции
			for (auto& word : dict.functions)
				HighlightLine(line, word.first, 4, charformat, shift);

			shift += line.length()+1;
		}
		
	
	}

	void SetNormal(CHARRANGE& cr)
	{
		
		
		CHARFORMAT normal;
		memset(&normal, 0, sizeof(normal));
		normal.cbSize = sizeof(normal);
		normal.dwMask = CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE | CFM_COLOR;// | CFM_FACE | CFM_SIZE;
		normal.dwEffects = 0;
		Editor->SetSel(cr);
		Editor->SetSelectionCharFormat(normal);
	}
	void HighlightLine(string line, string word, int colortype, CHARFORMAT& charformat, int shift)
	{
        size_t found = -1;
        while(1)
		{
			found = line.find(word, found+1);
			if (found==std::string::npos)
			{
				break;
			}
			
			
			charformat.crTextColor = getColor(colortype);
			Editor->SetSel(found+shift, found+shift+(int)word.length());
			Editor->SetSelectionCharFormat(charformat);


			
		}
	}

	COLORREF getColor(int type)
	{
		switch(type)
		{
			/*case 1: return RGB(255,0,126);//_operator
			case 2: return RGB(110, 15, 190);//keyword
			case 3: return RGB(0,0,255);//_variable
			case 4: return RGB(70,130,0);//_function - темно зеленый
			case 5: return RGB(95, 95, 95);//текст*/

		case 1: return RGB(255, 0, 126);//_operator
		case 2: return RGB(75, 75, 75);//keyword
		case 3: return RGB(0, 0, 255);//_variable
		case 4: return RGB(110, 15, 190);//_function - темно зеленый
		case 5: return RGB(70, 130, 0);//текст

		}
		return RGB(0, 0, 0);
	}


	wstring StringToWString(string str)
	{
		//CP_ACP
		DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
		//выделяем память под wide char строку:
		wchar_t *pwText;
		pwText = new wchar_t[dwNum];
		MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, pwText, dwNum);
		wstring w =  pwText;
		delete [] pwText;
		return w;
	}

//-------------------------------------------------------------------------------------------------------------




};



#endif


