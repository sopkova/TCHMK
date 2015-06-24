#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct LongNumber
{
    unsigned long long size;
    unsigned long long* pointer;
};

unsigned long long CountDigits(struct LongNumber number);

struct LongNumber ShortDivide(struct LongNumber a, unsigned long long b);

struct LongNumber lvlUp(struct LongNumber a);

unsigned char isLessOrEqual( struct LongNumber a, struct LongNumber b);

struct LongNumber multiplyOnN(struct LongNumber a, unsigned long long n);

unsigned char isEqual(struct LongNumber a, struct LongNumber b);

int ShortCompare(struct LongNumber a, unsigned long long b);

struct LongNumber sum(struct LongNumber a, struct LongNumber b);

struct LongNumber substract(struct LongNumber a, struct LongNumber b);

struct LongNumber multiply(struct LongNumber a, struct LongNumber b);

struct LongNumber divide(struct LongNumber a, struct LongNumber b);

struct LongNumber module(struct LongNumber a, struct LongNumber b);

struct LongNumber power(struct LongNumber a, struct LongNumber b,struct LongNumber c);

struct LongNumber ReadTextFile(const char* file);

void WriteTextFile(const char* file, struct LongNumber number);

void clear(struct LongNumber *number);

struct LongNumber allocate(struct LongNumber number, unsigned long long size);

struct LongNumber zero( struct LongNumber number, unsigned long long size);

struct LongNumber Normalize(struct LongNumber a);

struct LongNumber copy(struct LongNumber from);

struct LongNumber loadLongNumber(const char* longNumFileName);

void saveLongNumber(struct LongNumber a, const char* longNumFileName);

char* toString(struct LongNumber number);

struct LongNumber ReadFromString(char* buffer);

struct LongNumber shiftLeft(struct LongNumber a, unsigned long long s);

int compare(struct LongNumber a, struct LongNumber b);
