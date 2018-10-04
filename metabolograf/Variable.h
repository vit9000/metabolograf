#ifndef Variable_h
#define Variable_h

#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <exception>
#include <algorithm>

using namespace std;

#define Scalar 0
#define Vector 1

template<class V>
class Variable_
{
public:
	Variable_()
	{
		SetType(Vector);
	};
	Variable_(size_t size, const V& fill)
	{
	   values.clear();
	   values.resize(size);
	   for(size_t i=0; i<size; i++)
            values[i] = fill;
	   if(size>1)
		   SetType(Scalar);
	   else 
		   SetType(Vector);
	};
	Variable_(const Variable_& var)
	{
		values.clear();
		values.insert(values.begin(), var.values.begin(), var.values.end());
		SetType(Vector);
	};


	void clear()
	{
      values.clear();
	}
	void append(const V& v)
	{
		values.push_back(v);
	}
	typename vector<V>::iterator begin()
	{
		return values.begin();
	}

	typename vector<V>::iterator end()
	{
		return values.end();
	}

	Variable_(const V& v)
    {
        values.push_back(v);
		SetType(Scalar);
	}
	//--------------------------------------------------------------------------
	template<typename T>
	Variable_(const vector<T>& v)
    {
		for(int i=0; i<v.size();i++)
		{
			values.push_back(static_cast<V>(v[i]));
		}
		SetType((v.size()>1)?Vector:Scalar);

    }
	//--------------------------------------------------------------------------
	template<typename T>
	Variable_(T* v, int size)
	{
		for(int i=0; i<size; i++)
			values.push_back(static_cast<V>(v[i]));
		SetType(size);
	}
	//--------------------------------------------------------------------------
	double GetSD()
	{
		double temp = 0;
		for (const double& val : v)
			temp += val;
		temp = static_cast<int>(temp / static_cast<int>(hr.size()));
		return temp;
	}


	V& operator[](size_t i)
	{
		if (i<0)
			throw exception();
		if (i >= values.size())
		{
			values.resize(i + 1);
		}

		V& val = (values[i]);
		return val;
	}

	const V& operator[](size_t i) const
	{
		return values.at(i);
	}
	//--------------------------------------------------------------------------
	
	
    
	friend Variable_ operator*(const Variable_& a, const Variable_& b)
    {
        if(a.values.size()==1) return operator*(b,a.values[0]);
        if(b.values.size()==1) return operator*(a,b.values[0]);
        Variable_ result;

        if(a.values.size() != b.values.size())
            throw exception();

        for(size_t i=0; i<a.values.size(); i++)
            result.values.push_back(a.values[i] * b.values[i]);
		return result;
	}
	//--------------------------------------------------------------------------
    template<class T>
    friend Variable_ operator*(const Variable_& a, const T& b)
    {
        Variable_ result;
        for(size_t i=0; i<a.values.size(); i++)
            result.values.push_back(a.values[i] * b);
		return result;
    }
	//--------------------------------------------------------------------------
    friend Variable_ operator/(const Variable_& a, const Variable_& b)
    {
        if(a.values.size()==1) return operator/(b,a.values[0]);
        if(b.values.size()==1) return operator/(a,b.values[0]);
        Variable_ result;
        if(a.values.size() != b.values.size())
            throw exception();
		for(size_t i=0; i<a.values.size(); i++)
		{
            if(b.values[i]==0)
				result.values.push_back(0);
			else
				result.values.push_back(a.values[i] / b.values[i]);
		}
		return result;
	}
	//--------------------------------------------------------------------------
    template<class T>
    friend Variable_ operator/(const Variable_& a, const T& b)
    {
        Variable_ result;
		for(size_t i=0; i<a.values.size(); i++)
		{
			if(b==0)
				result.values.push_back(0);
			else
				result.values.push_back(a.values[i] / b);
		}
		return result;
    }
	//--------------------------------------------------------------------------
	template<class T>
	friend Variable_ operator/(const T& a, const Variable_& b)
	{
		Variable_ result;
		for (size_t i = 0; i<b.values.size(); i++)
		{
			if (a == 0)
				result.values.push_back(0);
			else
				result.values.push_back(a / b.values[i]);
		}
		return result;
	}
	//--------------------------------------------------------------------------

    friend Variable_ operator+(const Variable_& a, const Variable_& b)
    {

        if(a.values.size()==1) return operator+(b,a.values[0]);
        if(b.values.size()==1) return operator+(a,b.values[0]);
        Variable_ result;

        if(a.values.size() != b.values.size())
            throw exception();
        for(size_t i=0; i<a.values.size(); i++)
            result.values.push_back(a.values[i] + b.values[i]);
		return result;
	}
	friend Variable_ operator-(const Variable_& a, const Variable_& b)
	{

		if (a.values.size() == 1) return operator-(b, a.values[0]);
		if (b.values.size() == 1) return operator-(a, b.values[0]);
		Variable_ result;

		if (a.values.size() != b.values.size())
			throw exception();
		for (size_t i = 0; i<a.values.size(); i++)
			result.values.push_back(a.values[i] - b.values[i]);
		return result;
	}


	//--------------------------------------------------------------------------
    template<class T>
    friend Variable_ operator+(const Variable_& a, const T& b)
    {
        Variable_ result;
        for(size_t i=0; i<a.values.size(); i++)
            result.values.push_back(a.values[i] + b);
		return result;
    }
	//--------------------------------------------------------------------------
    
	//--------------------------------------------------------------------------
    template<class T>
    friend Variable_ operator-(const Variable_& a, const T& b)
    {
        Variable_ result;
        for(size_t i=0; i<a.values.size(); i++)
            result.values.push_back(a.values[i] - b);
        return result;
    }
	template<class T>
	friend Variable_ operator-(const T& a, const Variable_& b)
	{
		Variable_ result;
		for (int i = 0; i<b.values.size(); i++)
			result.values.push_back(a - b.values[i]);
		//result.SetType(b.GetType());
		return result;
	}
	//--------------------------------------------------------------------------
    friend std::ostream& operator<< (std::ostream& out, const Variable_& v)
    {
        if(v.values.size()==0) return out;
        if(v.values.size()==1)
        {
            out << v.values[0];
            return out;
        }
		int size = v.values.size();
        out << "(";
		for(int i=0; i<size; i++)
		{

			if(i!=0) out << ", ";
			out << v.values[i];

			if(size>4 && i==1)
			{
				out << ", ... ";
				i = size-3;
			}
        }
		out << ")";

		if(size>4)
		{
			out << " (всего: "<< size <<")";
		}

        return out;
	}
	
	//--------------------------------------------------------------------------
	V* getPtr(int size=-1)
	{
        if(size>=0)
			resize(size);
		return &(values[0]);
	}
	
	//--------------------------------------------------------------------------
	void resize(int size, V val=0)
	{
        values.resize(size, val);
	}
	//--------------------------------------------------------------------------
	int size() const
	{
        return static_cast<int>(values.size());
	}
	//--------------------------------------------------------------------------
	string toString(int index) const
	{
		stringstream ss(std::string(""));
		ss << values[index];
		return ss.str();
	}
	//--------------------------------------------------------------------------
	string toStringWithIndex(int index) const
	{
		stringstream ss("");
		ss <<  setw(6) << left << fixed << setprecision(3);
		ss << index << " "  << values[index];
		return ss.str();
	}
	//--------------------------------------------------------------------------
	void erase(int index)
	{
        values.erase(values.begin()+index);
	}
	//--------------------------------------------------------------------------
	const V& at(size_t i) const
	{
		return values.at(i);
	}
	//--------------------------------------------------------------------------
	V mean(const Variable_<uint8_t>& var) const
	{
	   V mean=0;
	   int count=0;
	   for(int i=0; i<size(); i++)
	   {
			if(var.at(i) )
			{
				mean += values[i];
				count++;
			}
	   }
	   mean /= count;
	   return mean;
	}
    //--------------------------------------------------------------------------
	V mean() const
	{
	   V mean=0;
	   int count=0;
	   for(int i=0; i<size(); i++)
	   {
			mean += values.at(i);
			count++;
	   }
	   mean /= count;
	   return mean;
	}
	//--------------------------------------------------------------------------
	Variable_<V> getWithoutUnchecked(const Variable_<uint8_t>& checked) const
	{
		Variable_<V> var;
		for (size_t i = 0; i < values.size(); i++)
		{
			if ((int)i >= checked.size()) break;
			bool b = checked[i];
			if (b)
				var.append(values[i]);
		}
		return var;
	}
	//--------------------------------------------------------------------------
	Variable_<V> meanByIntervals(const vector<int>& intervals)
	{
		Variable_<V> var;
		for (int i = 0; i < static_cast<int>(intervals.size()) - 1; i++)
		{
			if (i == intervals.size() - 2)
				int i = 0;
			int start = intervals[i];
			int end = intervals[i + 1];
			var.append(mean(start, end));
		}
		return var;
	}


	Variable_<V> SDbyIntervals(const vector<int>& intervals)
	{
		Variable_<V> var;
		for (int i = 0; i < intervals.size() - 1; i++)
		{
			if (i == intervals.size() - 2)
				int i = 0;
			int start = intervals[i];
			int end = intervals[i + 1];
			var.append(SD(start, end));
		}
		return var;
	}
#include <cmath>
	V SD(size_t start, size_t end) const
	{
		
		V temp = 0;
		if (start > end) return 0;

		V Mean = mean(start, end);
		
		if (end >= static_cast<size_t>(size()))
			end = size() - 1;
		for (size_t i = start; i <= end; i++)
		{
			
			V t = values[i] - Mean;

			temp += t*t;

		}

		temp /= (end-start+1);
		temp = sqrt(temp);
		if (temp < 0.1) temp = 0;
		return temp;
	}
	//--------------------------------------------------------------------------
	//--------------------------------------------------------------------------
	V mean(size_t start, size_t end) const
	{
		size_t count=0;
		V temp=0;
		
		for (size_t i = start; i <= end; i++)
		{
			if (i >= static_cast<size_t>(size())) break;
			count++;

			temp += values[i];
		}
		if(count!=0)
			temp /= count;
		return temp;
	}
	//--------------------------------------------------------------------------
	void setConst(bool status)
	{
		Const = status;
	}
	bool isConst() const { return Const; }

	
	int GetType() const { return Type; }
	void SetType(int _type=Vector)
	{
		Type = _type;

		Const = false;
	}
private:
	vector<V> values;
	int Type;
	
	bool Const;
	//--------------------------------------------------------------------------
	

};

typedef Variable_<double> Variable;
typedef Variable_<uint8_t> VariableBOOL;

#endif /* Variable_h */
