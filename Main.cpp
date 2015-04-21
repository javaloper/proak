#include <iostream>
#include <stdlib.h>
#include <string>
#include "LargeNumber.h"

int main()
{
	LargeNumber number("23.23230");
	LargeNumber number1("0.7734");
	LargeNumber number2("452143543543663.236765437653365");
	LargeNumber number3("-375327342573.23534326345");

	LargeNumber number4;

	cout << number.ToString() << endl;
	cout << number.GetDecimalMarkPosition() << endl;
	cout << number1.ToString() << endl;
	cout << number1.GetDecimalMarkPosition() << endl;
	cout << number2.ToString() << endl;
	cout << number2.GetDecimalMarkPosition() << endl;
	cout << number3.ToString() << endl;
	cout << number3.GetDecimalMarkPosition() << endl;

	cout << "Domyslny: " <<number4.ToString() << endl;
	cout << number4.GetDecimalMarkPosition() << endl << endl;

	LargeNumber result = number + number1;
	LargeNumber resultsub = number2 * number3;
	cout << "Wynik dodawania:  " << result.ToString() << endl;
	cout << "Wynik mnozenia:  " << resultsub.ToString() << endl;

	cout << endl;

	system("pause");
	return 0;
}