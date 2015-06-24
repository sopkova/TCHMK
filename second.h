extern "C"
{
	#include "first.h"
} 

class LN_Class
{
	LongNumber num;

public:

	LN_Class();
	~LN_Class();
	LN_Class(unsigned long long);
	LN_Class(char*);
	LN_Class(const LN_Class& t);

	const LN_Class operator=(const LN_Class& right); 
	const LN_Class operator+(const LN_Class&);
	const LN_Class operator-(const LN_Class&);
	const LN_Class operator*(const LN_Class&);
	const LN_Class operator/(const LN_Class&);
	const LN_Class operator%(const LN_Class&);
	const LN_Class operator+(const unsigned long long&);
	const LN_Class operator-(const unsigned long long&);
	const LN_Class operator*(const unsigned long long&);
	const LN_Class operator/(const unsigned long long&);
	const LN_Class operator%(const unsigned long long&);

	const LN_Class PowMod(const LN_Class& b, const LN_Class& c);
	const LN_Class PowMod(const unsigned long long& t, const LN_Class& c);
	const LN_Class PowMod(const unsigned long long& t1, const unsigned long long& t2);

	void ReadText(const char* filename);
	void WriteText(const char* filename);
	void ReadBin(const char* longNumFileName);
	void WriteBin(const char* longNumFileName);
	char* Print();


private:
	void ClearMemory();
	
	
};
