#define F_CPU 8000000ul

#define DELAY_BETWEEN_MESSAGE 40
#define OUT_OF_RANGE_BIT 8
#define POINT_DURATION 2
#define DASH_DURATION 6

#include <avr/io.h>
#include <avr/interrupt.h>
#include "MorseCode.h"

//Передаваемое сообщение
const char message[] = "VVV VVV VVV";        	
unsigned short int index = 0;                   			//Порядковый номер считываемого символа сообщения
unsigned char readedSymbol;           					//Считываемый символ
unsigned short int indexOfBit = 0;           				//Порядковый номер считываемого бита символа (index)
unsigned short int timerCounter = 0;                  	//Счетчик таймера


/*---------------------------------------------------------
Запуск таймера.
---------------------------------------------------------*/
void StartTimer() {
	
	//сброс предделителя
	GTCCR |= ( 1 << PSR0 );
	//сброс счетного регистра
	TCNT0 = 0;
	//предделитель 1/1024
	//TCCR0B |= ( 1 << CS02 ) | ( 1 << CS00 );
	//предделитель 1/256
	TCCR0B |= ( 1 << CS02 );
	//разрешить прерывания
	sei();
}


/*---------------------------------------------------------
Следующая передача.
---------------------------------------------------------*/
void NextTransfer() {
	
	if ( indexOfBit < OUT_OF_RANGE_BIT ) {
		 
		//убираем высокий потенциал
		PORTB &= ~( 1 << 3 );
		
		if ( readedSymbol & ( 1 << indexOfBit ) ) {

			//обработка тире
			timerCounter = DASH_DURATION;
		}
		else {

			//обработка точки
			timerCounter = POINT_DURATION;
		}
		
		++indexOfBit;
	}
	else {
		
		//пробел или неопознанный символ, долгая пауза
		timerCounter = DASH_DURATION;
	}
	
	StartTimer();
}


/*---------------------------------------------------------
Считывание байта (readedSymbol). Возвращает позицию после
первой встреченной единици.
---------------------------------------------------------*/
unsigned int FindStartingBitPosition() {
	
	for ( int i = 0; i < OUT_OF_RANGE_BIT; i++ ) {
		
		//проход по байту до первой единицы
		if( readedSymbol & ( 1 << i ) ) {
			
			return ++i;
		}
	}
	
	//код ошибки
	return 8;
}


/*---------------------------------------------------------
Посимвольное считывание сообщения.
---------------------------------------------------------*/
void ReadNextSymbol() {

	if ( message[ index ] != '\0' ) {
		 
		readedSymbol = TranslateToTelegraph( message[ index ] );
		indexOfBit = FindStartingBitPosition();
		++index;
		NextTransfer();
	}
	else {
		
		//пауза и повтор сообщения
		timerCounter = DELAY_BETWEEN_MESSAGE;
		index = 0;
		indexOfBit = OUT_OF_RANGE_BIT;
		StartTimer();
	}
}


/*---------------------------------------------------------
Обработка прерывания по переполнению.
---------------------------------------------------------*/
ISR( TIMER0_OVF_vect ) {
	
	//запретить прерывания
	cli();
	//предделитель 0
	TCCR0B &= ~( 1 << CS02 );
	
	--timerCounter;
	
	if ( timerCounter < 1 ) {
		
		//если на РВ3 +5В
		if( !(PINB & 1 << 3) ) {
			
			//устанавливаем высокий потенциал
			PORTB |= ( 1 << 3 );
			
			//пауза после передачи
			if( indexOfBit < OUT_OF_RANGE_BIT ) {
				
				//пауза между каждой передачей
				timerCounter = POINT_DURATION;
			}
			else {
				
				//пауза между последней передачей и нового символа
				timerCounter = DASH_DURATION;
			}
			
			StartTimer();
		}
		//сюда возвращаемся после пауз с РВ3 = 0
		else {
			
			//если есть несчитанные биты в передаваемом символе
			if( indexOfBit < OUT_OF_RANGE_BIT ) {
				
				//следующая передача
				NextTransfer();
			}
			else {
				
				//иначе считывание следующего символа
				ReadNextSymbol();
			}
		}
	}
	else {
		
		StartTimer();
	}
}


int main() {
	
	//порт В на выход
	DDRB |= ( 1 << 3 );
	//устанавливаем высокий потенциал
	PORTB |= ( 1 << 3 );
	//найстройка таймера
	//запрет прерываний
	cli();
	//разрешить прерывания по переполнению
	TIMSK |= ( 1 << TOIE0 ); 
	//разрешить прерывания
	sei();
	
	ReadNextSymbol();
	
	while ( true ) {
	         
	}
	
	return 0;
}