#ifndef Analizator_h
#define Analizator_h

#include "Database.h"
#include <exception>

#include "Dictionary.h"
#include "Canvas.h"
#include "Plot.h"

class Analizator
{
private:
	Dictionary* dict;//словарь ключевых слов и разделителей
	Database* database;
	CCanvas* canvas;
public:
	string debug_string;
	
	Analizator(Database *_database, Dictionary* _dict, CCanvas* _canvas)
	{
		database = _database;
        dict = _dict;
		canvas = _canvas;
    }
//-------------------------------------------------------------------------------------------------------------
	string Run(string code)
	{
		 vector<string> elements = BuildElementsVector(code);
		 return BuildAnswer(elements);
	}
	//-------------------------------------------------------------------------------------------------------------	
private:

	//-------------------------------------------------------------------------------------------------------------
	vector<string> BuildElementsVector(string code)//разбиваем строку на элементы
    {
		//elements.clear();
		vector<string> elements;//разбитие строки на элементы, для прорисовки и дальнейшей обработки

		string word="";
	
        for(size_t i=0; i<code.length(); i++)//посимвольно
        {
			if (code[i] == '\"')
			{
				int j = (int)code.find('\"', i + 1);
				if (j == -1)
					throw runtime_error("Ошибка синтаксиса: кавычки должны быть закрыты.");
				string temp = code.substr(i+1, j-i-1);
				i = j;

				if (!word.empty())
					elements.push_back(word);//сохраняем и слово
				elements.push_back(temp);
				
			}
			else if(code[i]==' ' || dict->operators.count(ToString(code[i]))>0)//проверяем пробел или любой разделительный символ
            {
				if(!word.empty())
					elements.push_back(word);//сохраняем и слово
				if(code[i]!=' ')
					elements.push_back(ToString(code[i]));//и разделительный символ
                word.clear();
            }
            else//копим символы в слове
            {
				word.append(ToString(code[i]));
                if(i==code.length()-1)
                    elements.push_back(word);
			}
		}
		return elements;
    }
//-------------------------------------------------------------------------------------------------------------
    
public:
	string BuildAnswer(vector<string>& elements)
	{

		string output;
		if(elements.empty())
		{
			return "";
		}
		else if(elements.size()==1)
		{
			string var = elements[0];
			if(is_number(var))
				output = ToString(var);
			else if (database->isVariableExists(var))
			{
				output = ToString(database->getVariable(var));
				string temp = output;
			}
			else
				throw runtime_error("Переменная '"+var+"' не найдена.");
		}
		else if(isFunction(elements))
		{
			output = Function(elements);
		}
		else if (elements[1] == "=")
		{
			output = "";
			debug_string = Equation(elements);
		}
        else
        {
			Variable result(0.);
			result = Computation(elements);
			output = ToString(result);
		}
		return output;
    }
    //------------------------------------------------------------------------------------
private:
	bool isFunction(const vector<string>& elements)
	{
        if(elements[1]!="(") return false;
		//Функция имеет название, открывающиеся скобки и закрывающиеся
		if(dict->functions.count(elements[0])<=0)
			throw runtime_error("Функция '"+elements[0]+"' не найдена");
		if(elements[1]!="(" || elements[elements.size()-1]!=")")
			throw runtime_error("Синтаксическая ошибка написания функции '"+elements[0]+"'");
		return true;
	}
	//------------------------------------------------------------------------------------
	string Function( vector<string>& elements)
	{
		Plot* plot= new Plot();
		plot->Run(database, elements);
		plot->DrawPlot();
		string name = canvas->AddPlot(plot);
		
		
		/*switch(dict->functions[elements[0]])
		{
			case 1://ГРАФИК
				m_pPlot.Run(variables, elements);
				break;
		}*/
		name = "'" + name + "'";
		return name;//"Выполнена функция '"+elements[0]+"'";
	}

	string Equation(vector<string>& elements_copy)
	{
        string var = elements_copy[0];
        if(elements_copy.size()<3)
			throw runtime_error("Выражение написано не полностью");
        if(is_number(var))
			throw runtime_error("Действительное число нельзя определить как переменную");

        elements_copy.erase(elements_copy.begin());//удаляем название новой переменной
        elements_copy.erase(elements_copy.begin());//удаляем равно
        Variable result=0;
		result = Computation(elements_copy);
		string msg = "Инициализирована переменная '"+var+"' со значением ";
		if (database->isVariableExists(var))
			msg = "Переменная '"+var+"' равна ";
		if(database->isVariableExists(var) && database->getVariable(var).isConst())
			throw runtime_error("Нельзя изменить константную переменную");

		database->SetVariable(var, result);//m_pDatabase->variables[var] = result;//все хорошо, создаем новую запись в словаре переменных
		return msg + ToString(result);
    }
    //------------------------------------------------------------------------------------
	Variable Computation(vector<string>& elements_copy)
    {
        if(elements_copy.size()==1)
		{
			return getValue(elements_copy[0]);
        }

        //если значений больше, тогда создаем карту выражения
        bool operand=true;//индикатор что должно сейчас идти. Если true - операнд, false - оператор.
        //если вместо операнда идет оператор, тогда это знак для операнда
        int sign=1;//по умолчанию знак +1, принимает значение +1 или -1.

        vector<Variable>operands;
        vector<int>operators;
        vector<string>str_operators;

        int rank=1;
		for(size_t i=0; i<elements_copy.size(); ++i)
		{
			const string& e = elements_copy[i];
            if(e=="+" || e=="-")
            {
                if(operand)//то есть очередь операнда, тогда надо присвоить знак
                    sign = (e=="+")?1:(-1);
                else
                {
                    operators.push_back(1+rank*2);
                    str_operators.push_back(e);
                    operand=!operand;
                }

            }
            else if(e=="*" || e=="/")
            {
                if(operand)//то есть очередь операнда, тогда надо присвоить знак
					throw runtime_error("Ошибка написания выражения");

                operators.push_back(2+rank*2);
                str_operators.push_back(e);
                operand=!operand;

            }
            else if(e=="(")
            {
                rank++;
            }
            else if(e==")")
            {
                rank--;
            }
            else//если приходит операнд
            {
                if(!operand)//если идет очередь оператора
					throw runtime_error("Ошибка написания выражения");

                Variable temp=0;
				temp = getValue(e);
				temp = temp*sign;
                operands.push_back(temp);

                sign=1;
                operand=!operand;

            }
        }



        int count_operations = (int)operators.size();
        for(int i=0; i<count_operations; i++)
        {

			int pos = max_element(operators.begin(), operators.end()) - operators.begin();
            cout << operands[pos] << str_operators[pos] << operands[pos+1];

            try {
                if(str_operators[pos]=="+")
                    operands[pos] = operands[pos] + operands[pos+1];
                else if(str_operators[pos]=="-")
                    operands[pos] = operands[pos] - operands[pos + 1];//operands[pos] + (operands[pos+1]*(-1));
                else if(str_operators[pos]=="*")
                    operands[pos] = operands[pos] * operands[pos+1];
                else if(str_operators[pos]=="/")
                    operands[pos] = operands[pos] / operands[pos+1];
            } catch (...)
            {
				throw runtime_error("Размеры векторов не равны");
            }

            operators.erase(operators.begin()+pos);
            str_operators.erase(str_operators.begin()+pos);
            operands.erase(operands.begin()+pos+1);


        }
        cout << endl;
		return operands[0];
    }
    //------------------------------------------------------------------------------------
	Variable getValue(const string& element)
	{

        if(!is_number(element)) {
            if(!database->isVariableExists(element))
				throw runtime_error("Переменная '"+element+"' не найдена.");
			else
			{
				return database->getVariable(element);
			}
		}
		else
		{
			return atof(element.c_str());//ToDouble(element);
		}
	}


	bool is_number(const std::string& str)
	{
		if (str.empty()) return false;
		int points = 0;//для счета количества точек
					   //for(const char& c:s)

		string symb = "0123456789.";
		for (const auto& c:str)
		{
			bool temp = false;
			for (const auto& s : symb)
			{
				if (c == s)
					temp=true;
				if (c == '.')
				{
					if (points == 0)
						points++;
					else//если точек больше 1, то это ошибка записи
						return false;
				}
			}
			if (!temp) return false;
			
		}
		return true;
	}

    //------------------------------------------------------------------------------------
};


#endif
