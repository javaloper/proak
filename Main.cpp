#include <iostream>
#include <stdlib.h>
#include <string>
#include "LargeNumber.h"

int main()
{
	LargeNumber number("-12637523762163325.23467237416");
	LargeNumber number1("2189436572364463236.2452354723");
	LargeNumber number2("-12345");
	LargeNumber number3("-123.45");

	LargeNumber number4;

	cout << number.ToString() << endl;
	cout << number.GetDecimalMarkPosition() << endl;
	cout << number1.ToString() << endl;
	cout << number1.GetDecimalMarkPosition() << endl;
	/*
	cout << number2.ToString() << endl;
	cout << number2.GetDecimalMarkPosition() << endl;
	cout << number3.ToString() << endl;
	cout << number3.GetDecimalMarkPosition() << endl;
	cout << "liczba" << endl;
	cout << number4.ToString() << endl;
	cout << number4.GetDecimalMarkPosition() << endl << endl;
	cout << "liczba" << endl;
	uint8_t liczba = 0x31;
	*/
	LargeNumber result = number * number1;
	//LargeNumber resultsub = number - number1;
	cout << "Wynik:  " << result.ToString() << endl;
	//cout << resultsub.ToString() << endl;
	//cout << endl;


	system("pause");
	return 0;
}