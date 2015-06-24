#include "first.h"

unsigned char binary = 0;
unsigned long long MAX=18446744073709551615; // 2^64 -1

// пишем в hex рандомно(16-ичное). копируем-перегоняем в 2-ую сч. Переворачиваем их. Обратно в hex скаладываем(если сумма, например). И результат нужно перегнать из hex в 2-ую-переворачиваем, перевернуть и опять в hex

struct LongNumber loadLongNumber(const char* longNumFileName)
{
    struct LongNumber a;
    FILE *longNumFile;
    longNumFile = fopen(longNumFileName, "rb");
    
    unsigned long long numOfBytes = 0;
    while (getc(longNumFile) != EOF)
    {
        ++numOfBytes;
    }
    
    a.size = numOfBytes / 8;//кол-во элементв в массиве, т.к 64 бита в ячейке
    if (numOfBytes % 8 != 0)
    {
        ++a.size;
    }

    a.pointer = (unsigned long long*)malloc(a.size * sizeof(unsigned long long));
    if (a.pointer == NULL)
    {
        printf("Error: Unable to allocate memory \n");
        exit(0);
    }
    memset(a.pointer, 0, a.size * sizeof(unsigned long long)); //заполнили массив нулями

    fseek(longNumFile, SEEK_SET, 0);//прыгнули на начало файла

    unsigned long long tmp = 0;//временная x64 ячейка
    unsigned char byte = 0;//байт
    unsigned long long j = 0;
    unsigned long long i=0;
    for (i = 0; i < a.size; ++i)
    {
            do
        {
                 fread(&byte, sizeof(unsigned char), 1, longNumFile);//обязательно unsigned char
             byte = (byte * 0x0202020202ULL & 0x010884422010ULL) % 1023;//для x64 ячейки выполняется переворот битов
                 tmp ^= byte;
                 tmp <<= 8 * (j % 8);//Сдвигаем на 0,8,16,32 бит
                 a.pointer[i] ^= tmp;//записываем в массив
                 tmp = 0;
             ++j;
        }

    while (j % 8 != 0 && j != numOfBytes);

        tmp = a.pointer[i];
        tmp = 0;
        
    }

    fclose(longNumFile);
    return a;
}

void saveLongNumber(struct LongNumber a, const char* longNumFileName)
{
     FILE *longNumFile=fopen(longNumFileName,"wb");
     unsigned long long tmp = 0;
     unsigned char byte = 0;
     unsigned char lastElement[8];
     unsigned char lastNonZeroByte = 0;
     unsigned long long i = 0;
     unsigned char j = 0;

    for (i=0; i < a.size; ++i)
    {
            tmp = a.pointer[i];
            if (i != a.size - 1)
            {
                 for (j=0; j < 8; ++j)
                    {
                        byte = tmp;
                        byte = (byte * 0x0202020202ULL & 0x010884422010ULL) % 1023;
                        fwrite(&byte, sizeof(unsigned char), 1, longNumFile);
                        tmp >>= 8;
                    }
             }
        else  //Чтобы не было "Хвоста из нулей"
         {
            for (j = 0; j < 8; ++j)
        {
                    byte = tmp;
                    byte = (byte * 0x0202020202ULL & 0x010884422010ULL) % 1023;//Аналогично...
                    lastElement[j] = byte;
                    if (byte != 0x00)
                    {
                            lastNonZeroByte = j;
                    }
                    tmp >>= 8;
                }
                unsigned char j = 0;
        for (j = 0; j < lastNonZeroByte + 1; ++j)
                {
                    fwrite(&lastElement[j], sizeof(unsigned char), 1, longNumFile);//Побайтовая забись в файл
                }
        }
    }

    fclose(longNumFile);
}


struct LongNumber lvlUp(struct LongNumber a)//Используется в делении и в поиске остатка
{
    struct LongNumber res;
    res = allocate(res, a.size + 1);
    unsigned long long i=0;

    for (i = res.size - 1; i > 0; --i)
    {
        res.pointer[i] = a.pointer[i - 1];// +1 нулевая ячейка размером 64 бита в начало
    }

    a.size = res.size;
    return res;
}


unsigned char isLessOrEqual(struct LongNumber a, struct LongNumber b)//Используеся в вычитании, делении по модулю... Когда подаем массив, а в нем старшие ячейки нулевые(не перевернутого числа)
{
    a = Normalize(a);
    b = Normalize(b);
    if (a.size != b.size)//Сравниваем... Если true, то 2-ой больше первого
    {
        return a.size < b.size;
    }
    unsigned long long i=0;

    for ( i= a.size - 1; i < MAX; --i)
    {
            if (a.pointer[i] != b.pointer[i])
        {
                return a.pointer[i] < b.pointer[i];
            }
    }
        return 0;
}


unsigned char isEqual (struct LongNumber a,struct LongNumber b)
{

    a = Normalize(a);
    b = Normalize(b);
    if (a.size != a.size)
    {
        return 0;
    }
    unsigned long long i=0;
    for (i = a.size - 1; i < MAX; --i)
    {
            if (a.pointer[i] != b.pointer[i])
        {
                return 0;
            }
    }
        return 1;
}

struct LongNumber multiplyOnN(struct LongNumber a, unsigned long long n)//Умножение на короткое :) LittleEndian- переворачиваем число наоборот( было : 100 001 050 = 001 100 050)
 {
    struct LongNumber res;
  
    res = allocate(res, a.size + 1);
    unsigned long long i=0;
    __uint128_t binaryBuffer  = 1;
	
     for (i = 0; i < res.size - 1; ++i)
    {
            binaryBuffer *= (__uint128_t)a.pointer[i];
            binaryBuffer *= n;
            binaryBuffer += (__uint128_t)res.pointer[i];
            res.pointer[i] = binaryBuffer;
            binaryBuffer >>= 64;
            res.pointer[i + 1] = binaryBuffer;
            binaryBuffer = 1;
    }
    if (res.pointer[res.size - 1] == 0)
    {
        --res.size;
    }
    return res;
}

struct LongNumber sum(struct LongNumber a,struct LongNumber b)
{
    struct LongNumber res;
    if (a.size > b.size)
    {
        res.size = a.size + 1;
    }
    else
    {
        res.size = b.size + 1;
    }

        res = allocate(res, res.size);
        unsigned long long i=0;
        __uint128_t binaryBuffer  = 0;
        for (i = 0; i < res.size - 1; ++i)
        {
            if (a.size > i && b.size > i)
              {
                binaryBuffer = (__uint128_t)res.pointer[i] + (__uint128_t)a.pointer[i] + (__uint128_t)b.pointer[i];
                res.pointer[i] = binaryBuffer;
                binaryBuffer = binaryBuffer >>64;
                res.pointer[i + 1] = binaryBuffer;
                binaryBuffer = 0;
              }
            else
                 {
                 if (a.size > i)
                     {
                    res.pointer[i] += a.pointer[i];
                      }

                     if (b.size > i)
                       {
                        res.pointer[i] += b.pointer[i];
                       }
                 }
     }

    if (res.pointer[res.size - 1] == 0)
    {
        --res.size;
    }
  
    return res;
}


struct LongNumber substract(struct LongNumber a, struct LongNumber b)
{
    struct LongNumber res;
    a = Normalize(a);
    b = Normalize(b);    
        res = allocate(res, a.size);
        unsigned long long i=0;

       for (i = 0; i < res.size; ++i)
        {
            res.pointer[i] = a.pointer[i];
        }

           for (i = 0; i < b.size; ++i)
        {
                if (res.pointer[i] >= b.pointer[i])
            {
                    res.pointer[i] -= b.pointer[i];
                }
                else
            {
                    res.pointer[i] -= b.pointer[i];
                    --res.pointer[i + 1];
                }
            }

    res = Normalize(res);

    return res;
}


struct LongNumber multiply(struct LongNumber a, struct LongNumber b)
{
    struct LongNumber res;
    res = allocate(res, a.size + b.size + 1);
    unsigned long long i=0;
    unsigned long long j=0;
    __uint128_t binaryBuffer  = 1;
    unsigned long long carry = 0;
    for (i = 0; i < a.size; ++i)
    {
            for (j = 0; j < b.size; ++j)
        {
                binaryBuffer *= (__uint128_t)a.pointer[i];
                binaryBuffer *= (__uint128_t)b.pointer[j];
                binaryBuffer += carry;
                binaryBuffer += (__uint128_t)res.pointer[i + j];
                res.pointer[i + j] = binaryBuffer;
                binaryBuffer >>= 64;
                carry = binaryBuffer;
                binaryBuffer = 1;
                if (j == b.size - 1)
            {
                    res.pointer[i + j + 1] += carry;
            }
        }
            carry = 0;
    }

   res = Normalize(res);

    return res;
}
struct LongNumber module(struct LongNumber a, struct LongNumber b)
{
    struct LongNumber res;
    struct LongNumber CNum;
    struct LongNumber cur;
    res = allocate(res, a.size - b.size + 1);
    CNum = allocate(CNum, 0);//отрезанный кусок от делимомого числа от конца до [i]-ой ячейки включительно
    cur.size = 0;
    unsigned long long i=0;
    if(b.size==1)
	{
		res=ShortDivide(a,b.pointer[0]);
		cur = multiplyOnN(res,b.pointer[0]);
                CNum = substract(a, cur);	
	}
    else
    {
    	for (i = a.size - 1; i < MAX; --i)
    	{
        	    CNum = lvlUp(CNum);// умножаем число на основание системы счисления(нулевая ячейка в начале массива(прям в начале))

                    CNum.pointer[0] = a.pointer[i];//Берем от превого большого числа(первый раз-старшая ячейка FirstLongNum(последняя))


            	unsigned long long x = 0, l = 0;//x-максимальное число,  которое дает максимально приближенное к делимому(FirstLongNum) число
            	__uint128_t r = 0;//Чтобы влезло основание системы счисления
            	r = --x;//r=(2^64)-1
            	x=0;//x=0
            	++r;//основание СЧ (2^64)

        	while (l <= r)
    		{
               		unsigned long long m = (l + r) >> 1;
               		cur = multiplyOnN(b, m);

                	if (isLessOrEqual(cur,CNum))
            		{
                        	x = m;
                        	l = m + 1;
                	}
            		else
        		{
                		r = m - 1;
            		}
        	}
        	res.pointer[i] = x;//Формируем частное(со старших разрядов)
        	cur = multiplyOnN(b, x);//это произведение делителя и x (x*SecondlongNum(которое явл делителем))
        	CNum = substract(CNum, cur);//На данном шаге получаем остаток, но не возвращаем...
    	}

    }
		
        int flag = 0;

       		if(CNum.size==b.size)
       		{
         		for(i=0;i<b.size;i++)
        		{
            			if(CNum.pointer[i]!=b.pointer[i])
            			{
                			flag=1;

            			}

        		}

         		if(flag==0)
         		{
             			CNum=zero(CNum,CNum.size);
             			CNum=Normalize(CNum);
         		}

       		}
        return CNum;	
}

struct LongNumber divide(struct LongNumber a, struct LongNumber b)
{
   struct LongNumber answer;
   struct LongNumber res;
   struct LongNumber CNum;
   struct LongNumber cur;
    res = allocate(res, a.size - b.size + 1); 
    CNum = allocate(CNum,0);//отрезанный кусок от делимомого числа от конца до [i]-ой ячейки включительно
   
    unsigned long long i=0;
	if(b.size==1)
	{
		return ShortDivide(a,b.pointer[0]);
	}

    for (i = a.size - 1; i < MAX; --i)
    {
            CNum = lvlUp(CNum);// умножаем число на основание системы счисления(нулевая ячейка в начале массива(прям в начале))

            CNum.pointer[0] = a.pointer[i];//Берем от первого большого числа(первый раз-старшая ячейка FirstLongNum(последняя))


            unsigned long long x = 0, l = 0;//x-максимальное число,  которое дает максимально приближенное к делимому(FirstLongNum) число
            __uint128_t r = 0;//Чтобы влезло основание системы счисления
            r = --x;//r=(2^64)-1
            ++x;//x=0
            ++r;//основание СЧ (2^64)

        while (l <= r)
    	{
                unsigned long long m = (l + r) >> 1;
                cur = multiplyOnN(b, m);
                if (isLessOrEqual(cur,CNum))
           	 {
                        x = m;
                        l = m + 1;
                }
            else
        	{
                  r = m - 1;
            	}
   	 }
        res.pointer[i] = x;//Формируем частное(со старших разрядов)
        cur = multiplyOnN(b, x);//это произведение делителя и x (x*SecondlongNum(которое явл делителем))
        CNum = substract(CNum, cur);//На данном шаге получаем остаток, но не возвращаем...
    }

    
   

    if (isEqual(CNum, b))
    {
            struct LongNumber tmp;
            tmp = allocate(tmp,1);
            tmp.pointer[0] = 1;
            res = sum(res,tmp);
    }
	res = Normalize(res);
	answer=copy(res);
	
    	return answer;
  
}





/*struct LongNumber remainder; 

    remainder = copy(a);
    unsigned long long i=0;
    unsigned long long x=0;
    
    __uint128_t r = 0;
     r = --x;
     ++x;
     ++r;
     struct LongNumber qG;
     qG=allocate(qG,2);
	
    for (i = a.size - b.size + 1; i != 0; i--)
    {
         __uint128_t qGuessMax = r, qGuessMin = 0, qGuess; 
	printf("1\n");		
        while (qGuessMax - qGuessMin > 1)
        {
		printf("2\n");

            qGuess = (qGuessMax + qGuessMin) / 2;

	  
	  
	    qG.pointer[0] = qGuess;
            qGuess >>= 64;
            qG.pointer[1] = qGuess;
	    
	    printf("%llu\n",qG.pointer[0]);
	    printf("%llu\n",qG.pointer[1]);

            struct LongNumber tmp = multiply(b, qG);
            tmp = shiftLeft(tmp, i - 1);   
            if (compare(tmp, remainder) > 0)
                qGuessMax = qGuess;
            else
                qGuessMin = qGuess;
            
            clear(&tmp);
        }
	 
	 
	 qG.pointer[0] = qGuessMin;
         qGuess >>= 64;
         qG.pointer[1] = qGuessMin;

         struct LongNumber tmp = multiply(b, qG);
         tmp = shiftLeft(tmp, i - 1);

        remainder = substract(remainder, tmp);

	clear(&tmp);
        
        res.pointer[i - 1] = qG.pointer[0];
    }

	remainder = Normalize(remainder);

	res = Normalize(res);
	return res;
	
}
*/


struct LongNumber power(struct LongNumber a, struct LongNumber b,struct LongNumber c)
{
	struct LongNumber res;
	if(a.size < c.size)
	{
		res.size = c.size + c.size;
	}
	else
	{
		res.size = a.size + a.size;
	}
	res = allocate(res, res.size);
   
	res.pointer[0] = 1;

	struct LongNumber com;
	com.size = 1;
	com = allocate(com, com.size);
	

	struct LongNumber value;
	value.size = res.size;
	value = allocate(value, value.size);


	
	value=copy(a);

	struct LongNumber pow = copy(b);

	while((compare(pow,com)) > 0)
	{
		if((pow.pointer[0] & 1)==1)
		{
			res = multiply(res, value);
			res = module(res, c);
		}

		value = multiply(value, value);
		value = module(value, c);
		pow = ShortDivide(pow, 2);
	}

	clear(&com);
	clear(&pow);
	clear(&value);

	return res;
}

struct LongNumber ReadTextFile(const char* file)
{	
	
        struct LongNumber bin, number;
        char ch;

        number.size = 0;

        FILE *in = fopen(file, "r");

        fseek(in, 0, SEEK_END);
        number.size = ftell(in);
        number.size = number.size -1;
        fseek(in, SEEK_SET, 0);

        number = allocate(number, number.size);
        bin.size = number.size / 16 + 1;
	
        bin = allocate(bin, bin.size);
        unsigned long long a, carry = 0, tmp, current, j, x;

        long long i = number.size-1;

        while((ch = getc(in))!=EOF)
        {
            number.pointer[i--] = ch - '0';
            if(i == -1) break;
        }
        fclose(in);

        current = 1;
        j = 0;
        x = 0;
   

        while (number.size != 1 || number.pointer[0]!=0)
        {
            carry = 0;


            for (i = number.size - 1; i >-1; i--)
            {
                tmp = carry * 10 + number.pointer[i];
                number.pointer[i] = tmp / 2;
                carry = tmp - number.pointer[i] * 2;

            }
            number = Normalize(number);
            bin.pointer[j] = ((current << x) * carry) | bin.pointer[j];
            x++;

            if(x == 64)
            {
                x = 0;
                j++;
            }
        }
        
        clear(&number);
        bin = Normalize(bin);
        return bin;
    }


void WriteTextFile(const char* file, struct LongNumber number)
{
       
        FILE* out = fopen(file, "w");

	long long int i =0;
        struct LongNumber number3;

	number3=copy(number);

        struct LongNumber decimal;

        decimal.size = number3.size*20;

        decimal = allocate(decimal, decimal.size);

        unsigned long long a, j = 0;

        __uint128_t tmp;

        __uint128_t number2=0;

        i = number3.size-1;

        char carry =0;

        unsigned long long x=0;
 
        number2 = --x;

        number2++;
	

        while (number3.size != 1 || number3.pointer[0]!=0)
            {

            carry = 0;
                                            
            for (i = number3.size - 1; i >-1; i--)
            {

                tmp = carry * number2 + (__uint128_t)number3.pointer[i];
                number3.pointer[i] = tmp / 10;
                carry = tmp - number3.pointer[i] * 10;
            }
                                         
            carry += '0';                               
            decimal.pointer[j] = carry;
         
            j++;                          
            number3 = Normalize(number3);
        }
	
        decimal = Normalize(decimal);

        i=0;
        for (i = decimal.size -1 ; i > -1; i--)
        {
                        fprintf(out, "%c" ,decimal.pointer[i]);
        }
	

        fclose(out);
    }


    void clear(struct LongNumber *number)
    {
        free(number->pointer);
	  
    }

unsigned long long CountDigits(struct LongNumber number)
{
	long long int i =0;
        struct LongNumber number3;

	number3=copy(number);

        struct LongNumber decimal;

        decimal.size = number3.size*20;

        decimal = allocate(decimal, decimal.size);

        unsigned long long a, j = 0;

        __uint128_t tmp;

        __uint128_t number2=0;

        i = number3.size-1;

        char carry =0;

        unsigned long long x=0;
 
        number2 = --x;

        number2++;
	

        while (number3.size != 1 || number3.pointer[0]!=0)
            {

            carry = 0;
                                            
            for (i = number3.size - 1; i >-1; i--)
            {

                tmp = carry * number2 + (__uint128_t)number3.pointer[i];
                number3.pointer[i] = tmp / 10;
                carry = tmp - number3.pointer[i] * 10;
            }
                                         
            carry += '0';                               
            decimal.pointer[j] = carry;
         
             j++;                   
            number3 = Normalize(number3);
       	    }
	
        decimal = Normalize(decimal);
	j=0;
	/* for (i = decimal.size -1 ; i > -1; i--)
        {
                        printf("%c" ,decimal.pointer[i]);
			j++;	
        }*/
	
	unsigned long long res=0;
	res=decimal.size;
        return res;
    }


struct LongNumber allocate(struct LongNumber number, unsigned long long size)
{	
    number.size = size;
    number.pointer = (unsigned long long*)malloc(sizeof(unsigned long long)*(size));
    zero(number, size);
    return number;
}

struct LongNumber zero(struct LongNumber number, unsigned long long size)
{	
    memset(number.pointer, 0, size * sizeof(unsigned long long));
    return number;
}
struct LongNumber Normalize( struct LongNumber a)
{
    unsigned long long i = a.size-1;

    while ( (i>0) && (a.pointer[i]==0) )
        i--;

    a.size = i+1;

    return a;
}

struct LongNumber copy(struct LongNumber from)
{
    struct LongNumber cpy;

    cpy.size = from.size;
    cpy = allocate(cpy, cpy.size);
    memcpy(cpy.pointer, from.pointer, cpy.size * sizeof(unsigned long long));

    return cpy;
}

int ShortCompare(struct LongNumber a, unsigned long long b)
{
	if (a.size > 1) return 1;
	if (a.size == 0) return -1;
	if (a.pointer[0] > b) return 1;
	if (a.pointer[0] < b) return -1;
	return 0;
}
char* toString(struct LongNumber number)
{

	if((number.size==0)||(ShortCompare(number,0)==0))
		return "0";	

	long long int i =0;
        struct LongNumber number3;

	number3=copy(number);

        struct LongNumber decimal;

        decimal.size = number3.size*20;

        decimal = allocate(decimal, decimal.size);

        unsigned long long a, j = 0;

        __uint128_t tmp;

        __uint128_t number2=0;

        i = number3.size-1;

        char carry =0;

        unsigned long long x=0;
 
        number2 = --x;

        number2++;
	

        while (number3.size != 1 || number3.pointer[0]!=0)
            {

            carry = 0;
                                            
            for (i = number3.size - 1; i >-1; i--)
            {

                tmp = carry * number2 + (__uint128_t)number3.pointer[i];
                number3.pointer[i] = tmp / 10;
                carry = tmp - number3.pointer[i] * 10;
            }
                                         
            carry += '0';                               
            decimal.pointer[j] = carry;
         
            j++;                          
            number3 = Normalize(number3);
        }
	
        decimal = Normalize(decimal);

        i=0;
        for (i = decimal.size -1 ; i > -1; i--)
        {
            printf("%c",decimal.pointer[i]);
            
        }

	char *str = NULL;
	j = 0;

	str = (char*)malloc(sizeof(char)*(1));
	str[0] = '\0';

	

	return str;
}	

struct LongNumber ReadFromString(char* buffer)
{
	int size = 0;
	
	struct LongNumber res;
	res=allocate(res,1);
	
	if (buffer == NULL)
	{
		printf("Error by reading string!");
		res.size = 0;
		return res;
	}
	
	if (strlen(buffer) == 0)
	{
		res.size = 0;
		return res;
	}
	
	struct LongNumber k;
	k=allocate(k,1);
	k.pointer[0] = 1;
		
	struct LongNumber tmp1;
 	struct LongNumber tmp2;

	unsigned long long tmp = 0;
	char ch;
	int i=0;
	
	for (i = strlen(buffer)-1; i>-1; i--)
	{
	        ch = buffer[i];
		

		if ((ch > '9') || (ch < '0'))
		{
			printf("Incorrect input!");
			res.size = 0;
			return res;
		}
		tmp = ch - '0';	
				
		tmp1 = multiplyOnN(k, tmp);
		
		tmp2 = Normalize(tmp1);
		
		res = sum(res, tmp2);
		
		clear(&tmp2);
		k = multiplyOnN(k,10);
		
	}
	
	clear(&k);
	return res;
}
struct LongNumber ShortDivide(struct LongNumber a, unsigned long long b)
{
	
	__uint128_t tmp;
	__uint128_t number2=0;
	unsigned long long kb=0;
	unsigned long long ka=0;
	unsigned long long kr=0;

	number2 = --kb;
        number2++;
	kb=0;
	unsigned long long g=0;
	unsigned long long carry = 0;
	long long int i;
	g=b;
	
	while(g!=0)
	{
	   g=g/10;
	   kb++;//razmer b
	}
	ka=CountDigits(a);
	
        struct LongNumber res;
	

        res.size = a.size;
        res = allocate(res, res.size);

	
	 
        for (i = a.size - 1; i >-1; i--)
            {	
            tmp = (__uint128_t) carry * number2 + (__uint128_t)a.pointer[i];
            res.pointer[i] = tmp / b;
            carry = tmp - (__uint128_t) res.pointer[i] * (__uint128_t) b;
	   
            }
  	/*res = Normalize(res);
		
	kr=CountDigits(res);

	
	kr=kr+(res.size-1)*19;         // razmer res(kolvo cifr)
	unsigned long long j=0;
	carry=0;
	for(j=0;j<kb-ka+kr;j++)
	{
	    for (i = res.size - 1; i >-1; i--)
            {	
            tmp = (__uint128_t) carry * number2 + (__uint128_t)res.pointer[i];
            res.pointer[i] = tmp / 10;
            carry = tmp - (__uint128_t) res.pointer[i] * 10;
	   
            }
 		carry=0;
	    
	}*/
        res = Normalize(res);
	return res;
}

int compare(struct LongNumber a, struct LongNumber b)
{	
	if(a.size > b.size)
		return 1;

	if(a.size < b.size)
		return -1;

	int i = a.size - 1;

	while (a.pointer[i] == b.pointer[i] && i > 0)
		i--;
	
	if(a.pointer[i] > b.pointer[i])
		return 1;

	if(a.pointer[i] < b.pointer[i])
		return -1;

	if(a.pointer[i] == b.pointer[i])
		return 0;
}

struct LongNumber shiftLeft(struct LongNumber a, unsigned long long s) 
{
    struct LongNumber current;
	
	current.size = a.size + s;
	current = allocate(current, current.size);
	unsigned long long i;

	for(i = s; i < a.size + s; i++)
		current.pointer[i] = a.pointer[i-s];

    return current;
}

