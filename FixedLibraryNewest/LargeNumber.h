#pragma once
#include <stdint.h>
#include <string>
#include <iostream>
using namespace std;
// ----------Klasa Large Number----------------------
class LargeNumber
{
private:
	/*
	Zmienne
	*/
	uint8_t* digitsArray; // przechowuje wszystkie cyfry reprezentacji
	unsigned int numberOfDigits; // liczba cyfr reprezentacji
	unsigned int decimalMarkPosition; // pozycja przecinka w tablicy digitsArray
	bool isPositive; // wskazuje, czy liczba jest dodatnia (true) czy ujemna (false)
	/*
	Prywatne funkcje
	*/
	LargeNumber Addition(LargeNumber &x, LargeNumber &y);
	LargeNumber Subtraction(LargeNumber &x, LargeNumber &y, bool isPositive = true); // trzeci opcjonalny parametr wskazuje, czy spodziewany jest dodatni wynik czy ujemny (gdy y > x)
	LargeNumber Multiplication(LargeNumber &x, LargeNumber &y);
	LargeNumber Division(LargeNumber &x, LargeNumber &y, unsigned int precision = 30);
	static int IsGreater(LargeNumber &x, LargeNumber &y); // funkcja pomocnicza sprawdzaj¹ca, czy liczba x jest wiêksza od y (zwraca dodatni¹) czy te¿ nie (0, gdy równe; ujemna gdy x mniejsze od y)
	// Usuwanie niepotrzebnych zero powstalych po dzialaniach arytmetycznych
	// Usuwanie zer z prawej strony przecinka
	uint8_t* CutZerosRight(uint8_t *x, unsigned int &length, const unsigned int &markPoint);
	// Usuwanie zer z lewej strony przecinka
	uint8_t* CutZerosLeft(uint8_t *x, unsigned int &length, unsigned int &markPoint);
public:
	/*
	Konstruktory
	*/
	// Tworzy 0
	LargeNumber();
	LargeNumber(unsigned int length);
	LargeNumber(LargeNumber number, unsigned int length);
	LargeNumber(uint8_t* array, unsigned int length, unsigned int markPoint);
	LargeNumber(uint8_t* array, unsigned int length, unsigned int markPoint, bool positive);
	//Tworzy liczbe ze stringa
	LargeNumber(string number);
	/*
	Destruktor
	*/
	~LargeNumber();
	/*
	Funkcje
	*/
	string ToString();
	unsigned int GetDecimalMarkPosition();
	unsigned int GetNumberOfDigits();
	bool GetSign();
	uint8_t Digit(unsigned int position);
	friend LargeNumber operator+ (LargeNumber &x, LargeNumber &y);
	friend LargeNumber operator- (LargeNumber &x, LargeNumber &y);
	friend LargeNumber operator* (LargeNumber &x, LargeNumber &y);
	friend LargeNumber operator/ (LargeNumber &x, LargeNumber &y);
	LargeNumber& LargeNumber::operator= (const LargeNumber &x);
	LargeNumber& LargeNumber::operator= (const string &x);
};

