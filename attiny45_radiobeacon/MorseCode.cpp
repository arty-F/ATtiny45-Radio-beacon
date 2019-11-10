#include "MorseCode.h"

/*---------------------------------------------------------
Перевод символа в телеграфный вид.
0 точка
1 тире
Символ начинается после первой 1 в байте
---------------------------------------------------------*/
unsigned char TranslateToTelegraph( char ch ) {

	switch ( ch ) {

	case 'a': 
	case 'A':
		return 0b10100000;

	case 'b':
	case 'B':
		return 0b00011000;

	case 'c':
	case 'C':
		return 0b01011000;

	case 'd':
	case 'D':
		return 0b00110000;

	case 'e':
	case 'E':
		return 0b01000000;

	case 'f':
	case 'F':
		return 0b01001000;

	case 'g':
	case 'G':
		return 0b01110000;

	case 'h':
	case 'H':
		return 0b00001000;

	case 'i':
	case 'I':
		return 0b00100000;

	case 'j':
	case 'J':
		return 0b11101000;

	case 'k':
	case 'K':
		return 0b10110000;

	case 'l':
	case 'L':
		return 0b00101000;

	case 'm':
	case 'M':
		return 0b11100000;

	case 'n':
	case 'N':
		return 0b01100000;

	case 'o':
	case 'O':
		return 0b11110000;

	case 'p':
	case 'P':
		return 0b01101000;

	case 'q':
	case 'Q':
		return 0b10111000;

	case 'r':
	case 'R':
		return 0b01010000;

	case 's':
	case 'S':
		return 0b00010000;

	case 't':
	case 'T':
		return 0b11000000;

	case 'u':
	case 'U':
		return 0b10010000;

	case 'v':
	case 'V':
		return 0b10001000;

	case 'w':
	case 'W':
		return 0b11010000;

	case 'x':
	case 'X':
		return 0b10011000;

	case 'y':
	case 'Y':
		return 0b11011000;

	case 'z':
	case 'Z':
		return 0b00111000;

	case '1':
		return 0b11110100;

	case '2':
		return 0b11100100;

	case '3':
		return 0b11000100;

	case '4':
		return 0b10000100;

	case '5':
		return 0b00000100;

	case '6':
		return 0b00001100;

	case '7':
		return 0b00011100;

	case '8':
		return 0b00111100;

	case '9':
		return 0b01111100;

	case '.':
		return 0b00000010;

	case ',':
		return 0b10101010;

	case ';':
		return 0b01010110;

	case ':':
		return 0b00011110;

	case '?':
		return 0b00110010;

	case '!':
		return 0b11001110;

	default: return 0b00000000;
	}
}