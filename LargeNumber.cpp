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
	// Tworzymy liczb� mniejsz� o ewentualny znak minusa lub kropki	.
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
	delete[]x;
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
	for (i = 0; i < markPoint - 1; i++)
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
	delete[] x;
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
		/*
		Sprawdzamy czy wartosc jest wieksza od podstawy (domyslnie 10, przy innych trzeba stworzyc template i zastapic 10
		Jesli tak, to ustawiamy przeniesienie
		*/
		carry = arrayOfDigits[i] / 10;
		arrayOfDigits[i] = arrayOfDigits[i] % 10;
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
		/*
		Sprawdzamy czy wartosc jest wieksza od podstawy (domyslnie 10, przy innych trzeba stworzyc template i zastapic 10
		Jesli tak, to ustawiamy przeniesienie
		*/
		carry = arrayOfDigits[i] / 10;
		arrayOfDigits[i] = arrayOfDigits[i] % 10;
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
LargeNumber LargeNumber::Subtraction(LargeNumber &x, LargeNumber &y, bool isPositive)
{
	uint8_t* arrayOfDigits;
	uint8_t borrow = 0; // po�yczka na danej pozycji 
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
	numberLength = XRightPart;
	if (YRightPart > XRightPart)
		numberLength = YRightPart;
	unsigned int rightBorder = numberLength; // prawa granice- tyle liczb po przecinku
	decimalMark = x.GetDecimalMarkPosition(); // pozycja przecinka dla d�u�szej reprezentacji przed przecinkiem (domy�lnie pozycja w liczbie x, ale sprawdzenie poni�ej)
	if (y.GetDecimalMarkPosition() > x.GetDecimalMarkPosition())
		decimalMark = y.GetDecimalMarkPosition();
	numberLength += decimalMark; // dodana d�ugo�� przed przecinkiem, dzi�ki temu jest ju� pe�na d�ugo�� nowej reprezentacji
	arrayOfDigits = new uint8_t[numberLength]; // inicjalizacja nowej tablicy na wynik odejmowania
	for (i = 0; i < numberLength; i++)
		arrayOfDigits[i] = 0; // inicjalizacja tablicy pierwsza liczb�, �eby m�c od niej odejmowa�
	for (i = numberLength - 1; i >= decimalMark; i--)
	{
		--rightBorder;
		uint8_t xDigit = x.Digit(x.GetDecimalMarkPosition() + rightBorder); // kolejna cyfra reprezentacji x
		uint8_t yDigit = y.Digit(y.GetDecimalMarkPosition() + rightBorder); // kolejna cyfra reprezentacji y
		// Sprawdzamy czy pozycja ktora chcemy odj�� istnieje
		if (x.GetDecimalMarkPosition() + rightBorder < x.GetNumberOfDigits())
			arrayOfDigits[i] += xDigit; // dodajemy cyfr� do pustej reprezentacji (odjemn�)
		if (y.GetDecimalMarkPosition() + rightBorder < y.GetNumberOfDigits())
			arrayOfDigits[i] -= yDigit; // odejmujemy cyfr� odjemnika (je�li istnieje)
		// Uwzgl�dniamy ewentualn� po�yczk�
		arrayOfDigits[i] -= borrow;
		// je�li otrzymano ujemn� cyfr� na danej pozycji (licznik si� cofa, bo ka�da cyfra jest liczb� unsigned 
		// (np. -1 to jest 255, -2, to 254, ... -9 to 247), to po�yczka jest 1 z nast�pnej pozycji 
		borrow = arrayOfDigits[i] > 246 ? 1 : 0;
		if (borrow == 1) // je�li uzyskano ujemn� liczb� na danej pozycji (borrow jest 1), to trzeba j� pomno�y� przez -1, aby mie� dodatni� warto��
			arrayOfDigits[i] += 10; // trzeba doda� przeniesienie z poprzedniej pozycji, aby uzyska� poprawn� cyfr� 
	}
	// odejmowanie cyfr przed przecinkiem:
	for (int i = decimalMark - 1; i >= 0; i--)
	{
		if (XLeftPart > 0)
		{
			--XLeftPart;
			arrayOfDigits[i] += x.Digit(XLeftPart);
		}
		if (YLeftPart > 0)
		{
			--YLeftPart;
			arrayOfDigits[i] -= y.Digit(YLeftPart);
		}

		arrayOfDigits[i] -= borrow;
		borrow = arrayOfDigits[i] > 246 ? 1 : 0;
		if (borrow == 1)
			arrayOfDigits[i] += 10;
	}
	arrayOfDigits = CutZerosLeft(arrayOfDigits, numberLength, decimalMark); // usuwa zb�dne zera z lewej
	return LargeNumber(arrayOfDigits, numberLength, decimalMark, isPositive); // znak nowej liczby ustalany w wywo�aniu funkcji Subtraction
}
// Operacja mno�enie
LargeNumber LargeNumber::Multiplication(LargeNumber &x, LargeNumber &y)
{
	// dlugosc x i y
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
	uint8_t carry;
	// zerowanie liczb
	for (i = 0; i < numberLength; i++)
		arrayOfDigits[i] = 0;
	// Mno�enie 
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

// Operacja dzielenia (precyzja domy�lna to 30 cyfr)
LargeNumber LargeNumber::Division(LargeNumber &x, LargeNumber &y, unsigned int prec)
{
	int XLen = x.numberOfDigits; // d�ugo�� dzielnej
	int YLen = y.numberOfDigits; // d�ugo�� dzielnika
	int markPosCorrection = 0; // korekcja pozycja przecinka (domy�lnie 2, je�li obie liczby s� ca�kowite)
	string divisorVal = y.ToString();
	if (y.GetSign() == false) // je�li ujemna, to usuwamy "-" z ci�gu znak�w, �eby mie� dodatni dzielnik (y)
		divisorVal = divisorVal.erase(0, 1);
	LargeNumber divisor(divisorVal); // zmienne lokalne, aby nie zmienia� zawarto�ci oryginalnych argument�w
	LargeNumber dividend(x.ToString());
	unsigned int precision = prec; // liczba cyfr reprezentacji
	int resultCurrentDigit = 0; // obecna cyfra wstawiana do reprezentacji wynikowej
	int XCurrentPosition = 0; // pozycja obecnie przetwarzanej cyfry reprezentacji x
	unsigned int markPos = 1;// XCurrentPosition;
	// tablica wstawianych cyfr do wyniku:
	uint8_t digits[] = { '0' - 48, '1' - 48, '2' - 48, '3' - 48, '4' - 48, '5' - 48, '6' - 48, '7' - 48, '8' - 48, '9' - 48 };
	uint8_t *result = new uint8_t[precision];
	LargeNumber R = LargeNumber("0"); // reszta z dzielenia 
	uint8_t *temp; // tablica tymczasowa na potrzeby inicjalizacji odpowiednio d�ugiej (dostosowanej do dzielnika) cz�ci dzielnej  
	unsigned int tempLength;
	char nextDigit; // kolejna cyfra rezprezentacji x
	LargeNumber Xpart; // cz�� dzielnej do aktualnego przetwarzania 
	LargeNumber Diff; // wynik wielokrotnego odejmowania od obecnej cz�ci dzielnej (x) wielokrotno�ci dzielnika (y)
	int n = 0; // licznik wielokrotno�ci dzielnika (y), przy kt�rej wyskalowany dzielnik mie�ci si� w dzielnej (x)

	if (XLen > YLen)
	{
		tempLength = YLen;
		// pozycja przecinka ustalana wzgl�dem d�ugo�ci dzielnika (y): je�li dzielnik jest tej samej d�ugo�ci, co dzielna (x), 
		// to markPos = 1, je�li dzielnik jest kr�tszy, to przecinek przesuwany w prawo tyle razy, ile dzielnik jest kr�tszy od dzielnej
		markPos += (XLen - YLen);
	}
	else
		tempLength = XLen;
	temp = new uint8_t[tempLength]; // tablica jest inicjalizowana d�ugo�ci� kr�tszej reprezentacji dzielnika (y)
	for (int i = 0; i < tempLength; i++)
		temp[i] = x.Digit(i);
	XCurrentPosition = tempLength; // obecna pozycja wynika z d�ugo�ci dzielnika (y)
	Xpart = LargeNumber(temp, tempLength, tempLength); // dzielna (x) jest skalowana do dzielnika (y), �eby mie� r�wn� d�ugo��
	if (dividend.decimalMarkPosition != dividend.numberOfDigits) // je�eli istnieje przecinek, to przesu� go na koniec (aby liczba by�a bez przecinka)
	{
		markPosCorrection += (dividend.numberOfDigits - dividend.decimalMarkPosition); // uwzgl�dnienie poprawki
		dividend.decimalMarkPosition = dividend.numberOfDigits; // przecinek przesuwany na koniec liczby 
		XCurrentPosition = dividend.decimalMarkPosition;
	}
	if (divisor.decimalMarkPosition != divisor.numberOfDigits) // je�eli istnieje przecinek, to przesu� go na koniec (aby liczba by�a bez przecinka)
	{
		markPosCorrection -= (divisor.numberOfDigits - divisor.decimalMarkPosition); //ewentualna korekta ->przesuni�cie przecinka w prawo
		divisor.decimalMarkPosition = divisor.numberOfDigits;
	}

	do{
		Diff = Xpart;
		for (n = 0; n < 10; n++)
		{
			if (IsGreater(Diff, divisor) < 0) // sprawdzenie, czy dzielnik zmie�ci si� jeszcze w obecnej reszcie (reprezentowanej tu przez Diff)
				break;
			Diff = Diff - divisor; // je�li tak, to odejmij ponownie dzielknik od obecnej reszty (Diff)
		}
		R = Diff; // r�nica cz�ci reprezentacji X i najwi�kszej mieszcz�cej si� w zakresie wielokrotno�ci dzielnika
		result[resultCurrentDigit] = digits[n]; // wstawia dan� cyfr� uint8_t do reprezentacji wynikowej
		resultCurrentDigit++; // ustawienie na kolejn� cyfr� w tablicy wynikowej
		precision--; // zmniejszenie liczby cyfr do dodania
		nextDigit = (char)(x.Digit(XCurrentPosition) + 48); // kolejna cyfra reprezentacji x do rozszerzenia obecnej reszty 
		if (XCurrentPosition >= x.numberOfDigits) // je�li reprezentacja x nie ma ju� wi�cej cyfr, to uzupe�niaj reszt� R zerami:
			nextDigit = '0';
		else
			XCurrentPosition++; // kolejna (istniej�ca) cyfra reprezentacji x
		if (R.ToString() == "0") // sprawdzenie, czy reszta R == 0 i je�li tak, to ustawia precyzj� na obecn� pozycj� (wi�ksza precyzja niepotrzebna) i ko�czy
		{
			prec = resultCurrentDigit;
			break;
		}
		string s = R.ToString() + nextDigit; // �a�cuch z dotychczasow� reprezentacj� rozszerzony o kolejn� cyfr�
		R = LargeNumber(s); // rozszerzona reprezentacja reszty R
		Xpart = R; // rozszerzona reszta idzie do dalszego dzielenia
	} while (precision > 0);

	markPos = markPos - markPosCorrection; // korekta pozycji przecinka z uwzgl�dnieniem rozszerze� dzielnika (y) i dzielnej (x)
	result = CutZerosLeft(result, prec, markPos); // usuwa zb�dne zera z przodu
	bool sign = !(x.GetSign() ^ y.GetSign()); // znak dodatni, gdy obie reprezentacje tych samych znak�w
	return LargeNumber(result, prec, markPos, sign);
}

// --------------------------------------------------
LargeNumber operator+ (LargeNumber &x, LargeNumber &y)
{
	if (!(x.GetSign() ^ y.GetSign()))
		return LargeNumber().Addition(x, y);
	else
	{
		//return LargeNumber().Subtraction(x, y);
		//return x - y;
		if (LargeNumber::IsGreater(x, y) >= 0)
			return LargeNumber().Subtraction(x, y, x.GetSign()); // domy�lnie isPositive = true
		else
			return LargeNumber().Subtraction(y, x, false);
	}
}
LargeNumber operator- (LargeNumber &x, LargeNumber &y)
{
	if (x.GetSign() ^ y.GetSign())
		return LargeNumber().Addition(x, y); // znak wyniku zale�ny od znaku x
	else
	{
		// sprawdzenie, czy x >= y (r�nica nieujemna, odejmowanie x - y) czy te� x < y 
		// (r�nica ujemna, odejmowanie -(y-x); w wyniku daje to wtedy dodatni� reprezentacj�, ale znak wyniku jest ujemny) 
		if (LargeNumber::IsGreater(x, y) >= 0)
			return LargeNumber().Subtraction(x, y, x.GetSign()); // domy�lnie isPositive = true
		else
			return LargeNumber().Subtraction(y, x, false); // liczba b�dzie ujemna, wi�c isPositive = false
	}
}
LargeNumber operator* (LargeNumber &x, LargeNumber &y)
{
	return LargeNumber().Multiplication(x, y);
}
LargeNumber operator/ (LargeNumber &x, LargeNumber &y)
{
	return LargeNumber().Division(x, y);
	//if (x.GetSign() ^ y.GetSign()) // uwzgl�dnienie znaku wyniku na podstawie znak�w dzielnej i dzielnika
	//	res.isPositive = false;
	//return res;
}

LargeNumber& LargeNumber::operator= (const LargeNumber &x)
{
	
	this->decimalMarkPosition = x.decimalMarkPosition;
	this->isPositive = x.isPositive;
	this->numberOfDigits = x.numberOfDigits;
//	delete[]this->digitsArray;
	this->digitsArray = new uint8_t[this->numberOfDigits];
	for (int i = 0; i < this->numberOfDigits; i++)
		this->digitsArray[i] = x.digitsArray[i];
	return *this;
}
// Przypisanie liczby ze stringa
LargeNumber& LargeNumber::operator= (const string &number)
{
	delete[]this->digitsArray;
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
	// Tworzymy liczb� mniejsz� o ewentualny znak minusa lub kropki	.
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
	return *this;

}
// por�wnuje dwie reprezentacje i szuka tej o wi�kszej warto�ci: zwraca dodatni� liczb�, je�li x > y, ujemn�, gdy y > x lub 0, gdy x = y
int LargeNumber::IsGreater(LargeNumber &x, LargeNumber &y)
{
	// je�li pozycja przecinka jest taka sama, to lewa cz�� liczby takiej samej d�ugo�ci 
	// i sprawdzane s� poszczeg�lne cyfry (licz�c od lewej)
	if (x.decimalMarkPosition == y.decimalMarkPosition)
	{
		int i = 0; // pozycja aktualnie sprawdzanej cyfry
		int returned = 0; // warto�� zwracana (domy�lnie: liczby r�wne)
		int XRight = x.numberOfDigits - x.decimalMarkPosition; // d�ugo�� liczby za przecinkiem
		int YRight = y.numberOfDigits - y.decimalMarkPosition;
		int shorterLastDigit; // ostatnia cyfra por�wnywana jest dopasowana do kr�tszej reprezentacji je�li liczby s� r�nej d�ugo�ci
		if (XRight > YRight)
		{
			shorterLastDigit = y.numberOfDigits - 1; // x ma d�u�sze rozwiniecie dziesi�tne, wi�c ostatnia sprawdzana cyfra jest ostatni� cyfr� Y (ka�da kolejna oznacza, �e x jest wi�ksze)
			returned = 1; // za�o�enie, �e x jest wi�kszy od y (ale mo�e nie by�, gdy cz�� dziesi�tna y b�dzie kr�tsza ale te� mia�a wi�ksz� warto�� ni� x
		}
		else
		{
			shorterLastDigit = x.numberOfDigits - 1; // je�li y ma d�u�sze lub r�wnej d�ugo�ci rozwini�cie dziesi�tne 
			returned = -1; // za�o�enie, �e y jest wi�kszy (analogicznie jak wy�ej)
		}
		while (i < shorterLastDigit)
		{
			if (x.Digit(i) == y.Digit(i)) // je�li cyfry r�wne, to przejd� do nast�pnej
				i++;
			else
			{
				//int foo = x.Digit(i) - y.Digit(i);
				//if (x.Digit(i) - y.Digit(i) > 246) // je�li dodatni, to x > y, je�li ujemny, to y > x
				//return -1;
				//else
				//return 1;
				return (x.Digit(i) - y.Digit(i));
			}
		}
		if (XRight == YRight) // je�li nie znaleziono r�nych cyfr i d�ugo�� obu reprezentacji zar�wno przed jak i po przecinku s� r�wne, to liczby s� takie same
			return 0; // zwraca zero, co oznacza, �e obie liczby s� r�wnej warto�ci i tym samym r�wnej d�ugo�ci
		return returned; // w przeciwnym razie zwraca za�o�onie, kt�re si� potwierdzi�o (d�u�sza reprezentacja by�a istotnie wi�ksza)
	}
	else
		return x.decimalMarkPosition - y.decimalMarkPosition; // je�li x ma d�u�sz� reprezentacj� z lewej, to jest wi�ksza (zwraca liczb� dodatni�)
}