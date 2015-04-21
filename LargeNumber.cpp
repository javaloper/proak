#include "LargeNumber.h"

// --------------------------------------------------
LargeNumber::LargeNumber()
{
	this->numberOfDigits = 1;
	this->decimalMarkPosition = 1;
	this->isPositive = true;
	this->digitsArray = new uint8_t[numberOfDigits] {0};

}
LargeNumber::LargeNumber(unsigned int length)
{
	this->numberOfDigits = length;
	this->digitsArray = new uint8_t[numberOfDigits];
	this->isPositive = true;
}
//Konstruktor tworzacy liczbe ze stringa, string dopuszcza minus na poczatku i kropke w dowolnym miejscu
LargeNumber::LargeNumber(string number)
{
	int stringCounter = 0;			// zmienna sluzaca do prawidlowego okreslenia dlugosci liczby pomijajac znaki minus i kropki
	this->decimalMarkPosition = -1; // punkt przecinka nie jest jeszcze ustalony
	/*
	Sprawdzamy czy liczba jest dodatnia, czy ujemna.
	*/
	if (number[0] == '-')
	{
		this->isPositive = false;
		++stringCounter;
	}
	else
		this->isPositive = true;
	// Sprawdzamy czy liczba zawiera przecinek oraz zapisujemy jego pozycje
	for (int i = 0; i < number.length(); i++)
	{
		if (number[i] == '.')
		{
			this->decimalMarkPosition = i - stringCounter; // - stringCounter uwzglednia mozliwy minus na poczatku
			++stringCounter;
			break;
		}
	}
	// Tworzymy liczbê mniejsz¹ o ewentualny znak minusa lub kropki	.
	this->numberOfDigits = number.length() - stringCounter;
	this->digitsArray = new uint8_t[numberOfDigits];
	// Jesli nie znalezlismy kropki, kropka jest na koncu liczby
	if (decimalMarkPosition == -1)
		decimalMarkPosition = numberOfDigits;
	// jesli liczba jest ujemna liczby bedziemy zapisywac od drugiego znaku stringa (!true == 0, !false == 1)
	stringCounter = !isPositive; 
	// Pierwsza petla przypisuje liczby do przecinka, druga od przecinka
	for (int i = 0; i < decimalMarkPosition; i++)
	{
		digitsArray[i] = number[i + stringCounter] - 48;
	}
	stringCounter++;
	for (int i = decimalMarkPosition; i < numberOfDigits; i++)
	{
		digitsArray[i] = number[i + stringCounter] - 48;
	}
	// Usuwamy nadmiarowe zera, jesli istnieja
	this->digitsArray = CutZerosLeft(digitsArray, numberOfDigits, decimalMarkPosition);
	this->digitsArray = CutZerosRight(digitsArray, numberOfDigits, decimalMarkPosition);
}
LargeNumber::LargeNumber(uint8_t* array, unsigned int length, unsigned int markPoint)
{
	this->numberOfDigits = length;
	this->decimalMarkPosition = markPoint;
	this->isPositive = true;
	this->digitsArray = new uint8_t[numberOfDigits];

	for (int i = 0; i < numberOfDigits; i++)
	{
		digitsArray[i] = array[i];
	}
}
LargeNumber::LargeNumber(uint8_t* array, unsigned int length, unsigned int markPoint, bool positive)
{
	this->numberOfDigits = length;
	this->decimalMarkPosition = markPoint;
	this->isPositive = positive;
	this->digitsArray = new uint8_t[numberOfDigits];

	for (int i = 0; i < numberOfDigits; i++)
	{
		digitsArray[i] = array[i];
	}
}
// --------------------------------------------------
LargeNumber::~LargeNumber()
{
	delete[] digitsArray;
}
// --------------------------------------------------
// Zapisywyanie do stringa
string LargeNumber::ToString()
{
	string number;
	// jesli liczba jest ujemna zacznij od zapisania znaku minus
	if (isPositive == false)
		number = "-";
	// Przypisuje liczby do przecinka, nastepnie jesli jest przecinek, dodaje go do stringa i wpisuje reszte liczb
	for (int i = 0; i < decimalMarkPosition; i++)
	{
		number += digitsArray[i] + 48;
	}
	if (decimalMarkPosition != numberOfDigits)
	{
		number += ".";
		for (int i = decimalMarkPosition; i < numberOfDigits; i++)
		{
			number += digitsArray[i] + 48;
		}
	}
	return number;
}
//Zwraca pozycje przecinka
unsigned int LargeNumber::GetDecimalMarkPosition()
{
	return decimalMarkPosition;
}
unsigned int LargeNumber::GetNumberOfDigits()
{
	return numberOfDigits;
}
bool LargeNumber::GetSign()
{
	return isPositive;
}
uint8_t LargeNumber::Digit(unsigned int position)
{
	uint8_t res = digitsArray[position];
	return res;
}

//Usuwanie zer z prawej strony przecinka
uint8_t* LargeNumber::CutZerosRight(uint8_t *x, unsigned int &length, const unsigned int &markPoint)
{
	uint8_t *newArray;
	unsigned int newLength = length;
	int i;
	// Usuwanie niepotrzebnych zer z prawej strony przecinka
	for (i = length - 1; i >= markPoint; i--)
	{
		if (x[i] != 0)
		{
			break;
		}
		newLength--;
	}
	// Jesli wielkosc sie nie zmienila zwroc stary wskaznik
	if (length == newLength)
		return x;
	
	newArray = new uint8_t[newLength];
	for (i = 0; i < newLength; i++)
	{
		newArray[i] = x[i];
	}
	length = newLength;
	delete x;
	x = newArray;
	return x;
}
//Usuwanie zer z lewej strony przecinka
uint8_t* LargeNumber::CutZerosLeft(uint8_t *x, unsigned int &length, unsigned int &markPoint)
{
	uint8_t *newArray;
	unsigned int cut = 0;
	int i;
	// Usuwanie niepotrzebnych zer z prawej strony przecinka
	for (i = 0; i < markPoint - 1 ; i++)
	{
		if (x[i] != 0)
		{
			break;
		}
		cut++;
	}
	// Jesli wielkosc sie nie zmienila zwroc stary wskaznik
	if (cut == 0)
		return x;

	markPoint = markPoint - cut;
	newArray = new uint8_t[length - cut];
	for (i = 0; i < length - cut; i++)
	{
		newArray[i] = x[i + cut];
	}
	length = length - cut;
	delete x;
	x = newArray;
	return x;
}
/*
Operacja dodawania, gdy obie liczby sa tego samego znaku:
dodawanie dwoch dodatnich, dodawanie dwoch ujemnych, odejmowanie, gdy jedna jest dodatnia
+x + +y; -x + -y; -x - +y, +x - -y;
*/
LargeNumber LargeNumber::Addition(LargeNumber &x, LargeNumber &y)
{
	uint8_t *arrayOfDigits;
	// Dlugosci liczb po przecinku x i y
	unsigned int XRightPart = x.GetNumberOfDigits() - x.GetDecimalMarkPosition();
	unsigned int YRightPart = y.GetNumberOfDigits() - y.GetDecimalMarkPosition();
	// Dlugosci liczb przed przecinkiem x i y
	unsigned int XLeftPart = x.GetDecimalMarkPosition();
	unsigned int YLeftPart = y.GetDecimalMarkPosition();
	// Dlugosc nowej liczby
	unsigned int numberLength = 0;
	// Pozycja przecinka nowej liczby
	unsigned int decimalMark = 0;
	// zmienna iteracyjna
	int i;
	//Dodajemy dluzsza pozycje przecinka do dlugosci liczby
	if (XRightPart >= YRightPart)
		numberLength += XRightPart;
	else
		numberLength += YRightPart;
	// prawa granica - tyle cyfr znajduje sie po przecinku
	unsigned int rightBorder = numberLength;
	// Dodajemy dluzsza pozycje przed przecinkiem do dlugosci liczby i ustalamy pozycje przecinka
	if (x.GetDecimalMarkPosition() >= y.GetDecimalMarkPosition())
	{
		decimalMark = x.GetDecimalMarkPosition();
		numberLength += x.GetDecimalMarkPosition();
	}
	else
	{
		decimalMark = y.GetDecimalMarkPosition();
		numberLength += y.GetDecimalMarkPosition();
	}
	// tworzenie nowej tablicy
	arrayOfDigits = new uint8_t[numberLength];
	// Zerowanie nowej tablicy
	for (i = 0; i < numberLength; i++)
		arrayOfDigits[i] = 0;
	// Przeniesienie
	uint8_t carry = 0;
	// Dodawanie liczb po przecinku
	for (i = numberLength - 1; i >= decimalMark; i--)
	{
		--rightBorder;
		// Sprawdzamy czy pozycja ktora chcemy dodac istnieje
		if (x.GetDecimalMarkPosition() + rightBorder < x.GetNumberOfDigits())
			arrayOfDigits[i] += x.Digit(x.GetDecimalMarkPosition() + rightBorder);
		if (y.GetDecimalMarkPosition() + rightBorder < y.GetNumberOfDigits())
			arrayOfDigits[i] += y.Digit(y.GetDecimalMarkPosition() + rightBorder);
		// Dodajemy ewentualne przeniesienie
		arrayOfDigits[i] += carry;
		// Zerujemy przeniesienie
		carry = 0;
		/*
		Sprawdzamy czy wartosc jest wieksza od podstawy (domyslnie 10, przy innych trzeba stworzyc template i zastapic 10
		Jesli tak, to ustawiamy przeniesienie
		*/

		if (arrayOfDigits[i] > 9)
		{
			arrayOfDigits[i] -= 10;
			carry = 1;
		}
	}
	// Dodawanie liczb po przecinku
	for (i = decimalMark - 1; i >= 0; i--)
	{
		if (XLeftPart > 0)
		{
			--XLeftPart;
			arrayOfDigits[i] += x.Digit(XLeftPart);
		}
		if (YLeftPart > 0)
		{
			--YLeftPart;
			arrayOfDigits[i] += y.Digit(YLeftPart);
		}
		// Dodajemy ewentualne przeniesienie
		arrayOfDigits[i] += carry;
		// Zerujemy przeniesienie
		carry = 0;
		/*
		Sprawdzamy czy wartosc jest wieksza od podstawy (domyslnie 10, przy innych trzeba stworzyc template i zastapic 10
		Jesli tak, to ustawiamy przeniesienie
		*/
		if (arrayOfDigits[i] > 9)
		{
			arrayOfDigits[i] -= 10;
			carry = 1;
		}
	}
	// Dodajemy nowa cyfre na poczatku jesli mamy przeniesienie
	uint8_t *newArrayOfDigits;	
	if (carry == 1)
	{
		++numberLength;
		++decimalMark;
		newArrayOfDigits = new uint8_t[numberLength];

		newArrayOfDigits[0] = 1;
		for (i = 1; i < numberLength; i++)
		{
			newArrayOfDigits[i] = arrayOfDigits[i - 1];
		}

		delete arrayOfDigits;
		arrayOfDigits = newArrayOfDigits;

	}
	// Usuwanie zer z konca
	arrayOfDigits = CutZerosRight(arrayOfDigits, numberLength, decimalMark);

	return LargeNumber(arrayOfDigits, numberLength, decimalMark, x.GetSign());
}
/*
Operacja odejmowania, wykonywana, gdy:
+x + -y; -x + y; +x - +y; -x - -y;
*/
LargeNumber LargeNumber::Subtraction(LargeNumber &x, LargeNumber &y)
{
	return LargeNumber();
}
// Operacja mno¿enie
LargeNumber LargeNumber::Multiplication(LargeNumber &x, LargeNumber &y)
{
	// inne zmienne
	unsigned int xLen = x.GetNumberOfDigits();
	unsigned int yLen = y.GetNumberOfDigits();
	// Dlugosc jest maksymalna dlugoscia jaka moze powstac;
	unsigned int numberLength = xLen + yLen;
	
	// Ustalenie dlugosci znaku
	unsigned int decimalMark = numberLength - (numberLength - x.GetDecimalMarkPosition() - y.GetDecimalMarkPosition());
	// inicjalizacja tablicy;
	uint8_t *arrayOfDigits = new uint8_t[numberLength];
	// Ustawienie znaku liczby
	bool sign = !(x.GetSign() ^ y.GetSign());
	// zmienne iteracyjna
	int i, j, k = 1;
	// Iteracyjna zmienna dlugosci
	unsigned int iterLength = numberLength - k;
	unsigned int carry; 
	// zerowanie liczb
	for (i = 0; i < numberLength; i++)
		arrayOfDigits[i] = 0;
	// Mno¿enie 
	for (i = yLen - 1; i >= 0; i--)
	{
		for (j = xLen - 1; j >= 0; j--)
		{
			arrayOfDigits[iterLength] += (y.Digit(i) * x.Digit(j));
			// Przeniesienie to wynik z dzielenia przez podstawe
			carry = arrayOfDigits[iterLength] / 10;
			// To co zostaje w komorce to reszta z dzielenia przez podstawe
			arrayOfDigits[iterLength] = arrayOfDigits[iterLength] % 10;
			// Przeniesienie dodajemy do wczesniejszej komorki
			iterLength--;
			arrayOfDigits[iterLength] += carry;
		}
		k++;
		iterLength = numberLength - k;
	}
	// Usuwanie zer z konca i poczatku
	arrayOfDigits = CutZerosRight(arrayOfDigits, numberLength, decimalMark);
	arrayOfDigits = CutZerosLeft(arrayOfDigits, numberLength, decimalMark);
	return LargeNumber(arrayOfDigits, numberLength, decimalMark, sign);
}
// Operacja dzielenia
LargeNumber LargeNumber::Division(LargeNumber &x, LargeNumber &y)
{
	return LargeNumber();
}
// --------------------------------------------------
LargeNumber operator+ (LargeNumber &x, LargeNumber &y)
{
	if (!(x.GetSign() ^ y.GetSign()))
		return LargeNumber().Addition(x, y);
	else
		return LargeNumber().Subtraction(x, y);
}
LargeNumber operator- (LargeNumber &x, LargeNumber &y)
{
	if (x.GetSign() ^ y.GetSign())
		return LargeNumber().Addition(x, y);
	else
		return LargeNumber().Subtraction(x, y);
}
LargeNumber operator* (LargeNumber &x, LargeNumber &y)
{
		return LargeNumber().Multiplication(x, y);
}
LargeNumber operator/ (LargeNumber &x, LargeNumber &y)
{
		return LargeNumber().Division(x, y);
}

