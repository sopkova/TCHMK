all:
	gcc -c -fPIC first.c -o first.o
	gcc -shared -o libfirst.so first.o ##first lab

	g++ -c -fPIC second.cpp -o second.o
	g++ -shared -L. -lfirst -o libsecond.so second.o ## second lab
	
	LD_LIBRARY_PATH=/home/v/Desktop/Lab_3

	swig -c++ -python -o LN_Class_wrap.cpp LN_Class.i
	g++ -fPIC -c LN_Class_wrap.cpp -I/usr/include/python2.7 ##third lab
	g++ -shared second.o first.o LN_Class_wrap.o -o _LN_Class.so
	
