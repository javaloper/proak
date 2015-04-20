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
	uint8_t* digitsArray;             // przechowuje wszystkie cyfry reprezentacji
	unsigned int numberOfDigits;      // liczba cyfr reprezentacji
	unsigned int decimalMarkPosition; // pozycja przecinka w tablicy digitsArray
	bool isPositive;                  // wskazuje, czy liczba jest dodatnia (true) czy ujemna (false)

	/*
	Prywatne funkcje
	*/
	LargeNumber Addition(LargeNumber &x, LargeNumber &y);
	LargeNumber Subtraction(LargeNumber &x, LargeNumber &y);
public:
	/*
	Konstruktory
	*/
	// Tworzy 0.0 
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
	
	friend LargeNumber operator+ ( LargeNumber &x,  LargeNumber &y);
	friend LargeNumber operator- ( LargeNumber &x,  LargeNumber &y);
	
};

