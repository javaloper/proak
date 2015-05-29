#include <iostream>
#include <stdlib.h>
#include <string>
#include <Windows.h>
#include "LargeNumber.h"
#include <time.h>
#include "MierzenieCzasu.h"

double PCFreq = 0.0;
__int64 CounterStart = 0;

void StartCounter()
{
	LARGE_INTEGER li;

	if (!QueryPerformanceFrequency(&li))
		cout << "QueryPerformanceFrequency failed!\n";

	PCFreq = double(li.QuadPart) / 1000.0;

	QueryPerformanceCounter(&li);
	CounterStart = li.QuadPart;
}
double GetCounter()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return double(li.QuadPart - CounterStart) / PCFreq;
}

int main()
{
	LargeNumber number("99999.9999");
	LargeNumber number1("99999.9999");
	LargeNumber number2("452145658435768756435466475745692342953678586554556786544354676543545678556544289501253864798756443543543663.23656756768783568456846634668765437653365");
	LargeNumber number3("375325467543567895468456648368343456882368456283425683465342568349129126410736876788554345698763342573.23534317362734237946963163463433426702702126345");
	LargeNumber number4;

	LargeNumber l1("444.33");
	LargeNumber l2("4.3333");
	LargeNumber l3;
	 l3 = l1 * l2;
	cout << l3.ToString() << endl;
	string z = "223.34";
	
	l3 = "-233.34";
	cout << l3.ToString() << endl;
//	MierzenieCzasu czas;	
	/*
	string n1 = "", n2 = ""; 
	srand(time(NULL));
	// Utworzenie losowej liczby
	int k;
	for (int i = 0; i < 900; i++)
	{
		k = rand() % 10;
		n1 += k + 48;
		k = rand() % 10;
		n2 += k + 48;
	}
	n1 += '.';
	n2 += '.';
	for (int i = 0; i < 100; i++)
	{
		k = rand() % 10;
		n1 += k + 48;
		k = rand() % 10;
		n2 += k + 48;
	}
	
	LargeNumber w1,w2,w3,w4;
	LargeNumber l1(n1);
	LargeNumber l2(n2);
	
	double res;

	StartCounter();
	w1 = l1 + l2;
	res = GetCounter();
	cout << "Czas dodawania: " << res << endl;

	StartCounter();
	w2 = l1 - l2;
	res = GetCounter();
	cout << "Czas odejmowania:  " << res << endl;

	StartCounter();
	w3 = l1 * l2;
	res = GetCounter();
	cout << "Czas mnozenia:  " << res << endl;

	StartCounter();
	w4 = l1 / l2;
	res = GetCounter();
	cout << "Czas dzielenia:  " << res << endl;
	MierzenieCzasu();
	
	cout << "L1:  " << l1.ToString() << endl;
	cout << "L2:  " << l2.ToString() << endl;
	cout << "Wynik dodawania:  " << w1.ToString() << endl;
	cout << "Wynik odejmowania:  " << w2.ToString() << endl;
	cout << "Wynik mnozenia:  " << w3.ToString() << endl;
	cout << "Wynik dzielenia:  " << w4.ToString() << endl;
	cout << endl;
	*/
	
	system("pause");
	return 0;
}

