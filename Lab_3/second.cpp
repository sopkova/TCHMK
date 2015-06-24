#include "second.h"

LN_Class::~LN_Class()
{	
    //printf("%llu\n", this->num.pointer[0]);	
    if(this->num.pointer != NULL)
	{
    		ClearMemory();
	}
	
};

LN_Class::LN_Class()
{
	this->num.pointer=NULL;
	this->num.size=0;
}

LN_Class::LN_Class(unsigned long long t)
{
	this->num=allocate((this->num),1);
	this->num.pointer[0]=t;
}

LN_Class::LN_Class(const LN_Class& t)
{
this->num = copy(t.num);
}

LN_Class::LN_Class(char* str)
{
	
	this->num = ReadFromString(str);
	  	
}


const LN_Class LN_Class::operator=(const LN_Class& right)
{
	this->num = copy(right.num);
	return *this;
}

const LN_Class LN_Class::operator+(const LN_Class& right) 
{
	LN_Class res;
	res.num = sum(this->num, right.num);
	return res;
}

const LN_Class LN_Class::operator-(const LN_Class& right) 
{
	LN_Class res;
	res.num = substract(this->num, right.num);
	return res;
}

const LN_Class LN_Class::operator*(const LN_Class& right) 
{
	LN_Class res;
	res.num = multiply(this->num, right.num);
	return res;
}

const LN_Class LN_Class::operator/(const LN_Class& right) 
{
	LN_Class res;
	res.num = divide(this->num, right.num);
	return res;
}

const LN_Class LN_Class::operator%(const LN_Class& right) 
{
	LN_Class res;
	res.num = module(this->num, right.num);
	return res;
}


const LN_Class LN_Class::PowMod(const LN_Class& b,const LN_Class& c)
{
	LN_Class res;
	res.num = power(this->num, b.num, c.num);
	return res;
}


const LN_Class LN_Class::operator+(const unsigned long long& t) 
{
	LN_Class res;
	LN_Class right;
	right.num=allocate(right.num,1);
	right.num.pointer[0]=t;
	res.num = sum(this->num, right.num);
	return res;
}

const LN_Class LN_Class::operator-(const unsigned long long& t) 
{
	LN_Class res;
	LN_Class right;
	right.num=allocate(right.num,1);
	right.num.pointer[0]=t;
	res.num = substract(this->num, right.num);
	return res;;
}

const LN_Class LN_Class::operator*(const unsigned long long& t) 
{
	LN_Class res;
	res.num = multiplyOnN(this->num, t);
	return res;
}

const LN_Class LN_Class::operator/(const unsigned long long& t) 
{
	LN_Class res;
	LN_Class right;
	right.num=allocate(right.num,1);
	right.num.pointer[0]=t;
	res.num = divide(this->num, right.num);
	return res;
}

const LN_Class LN_Class::operator%(const unsigned long long& t) 
{
	LN_Class res;
	LN_Class right;
	right.num=allocate(right.num,1);
	right.num.pointer[0]=t;
	res.num = module(this->num, right.num);
	return res;
}


const LN_Class LN_Class::PowMod(const unsigned long long& t,const LN_Class& c)
{
	LN_Class res;
	LN_Class left;
	left.num=allocate(left.num,1);
	left.num.pointer[0]= t;
	res.num = power(this->num, left.num, c.num);
	return res;
}


const LN_Class LN_Class::PowMod(const unsigned long long& t1, const unsigned long long& t2)
{
	LN_Class res;
	LN_Class left;
	left.num=allocate(left.num,1);
	left.num.pointer[0]=t1;
	LN_Class right;
	right.num=allocate(right.num,1);
	right.num.pointer[0]=t2;
	res.num = power(this->num, left.num, right.num);
	return res;
}

void LN_Class::ReadText(const char* filename)
{
	LN_Class res;
	res.num = ReadTextFile(filename);
	*this = res; 
}

void LN_Class::ReadBin(const char* longNumFileName)
{
	LN_Class res;
	res.num = loadLongNumber(longNumFileName);
	*this = res;
}

void LN_Class::WriteText(const char* filename)
{
	WriteTextFile(filename, this->num);
}

void LN_Class::WriteBin(const char* longNumFileName)
{	
	saveLongNumber(this->num,longNumFileName);
}

char* LN_Class::Print()
{
  return toString(this->num);
}

void LN_Class::ClearMemory()
{
	clear(&(this->num));
	this->num.pointer=NULL;
}


