#define F_CPU 8000000ul

#define DELAY_BETWEEN_MESSAGE 40
#define OUT_OF_RANGE_BIT 8
#define POINT_DURATION 2
#define DASH_DURATION 6

#include <avr/io.h>
#include <avr/interrupt.h>
#include "MorseCode.h"

//������������ ���������
const char message[] = "VVV VVV VVV";        	
unsigned short int index = 0;                   			//���������� ����� ������������ ������� ���������
unsigned char readedSymbol;           					//����������� ������
unsigned short int indexOfBit = 0;           				//���������� ����� ������������ ���� ������� (index)
unsigned short int timerCounter = 0;                  	//������� �������


/*---------------------------------------------------------
������ �������.
---------------------------------------------------------*/
void StartTimer() {
	
	//����� ������������
	GTCCR |= ( 1 << PSR0 );
	//����� �������� ��������
	TCNT0 = 0;
	//������������ 1/1024
	//TCCR0B |= ( 1 << CS02 ) | ( 1 << CS00 );
	//������������ 1/256
	TCCR0B |= ( 1 << CS02 );
	//��������� ����������
	sei();
}


/*---------------------------------------------------------
��������� ��������.
---------------------------------------------------------*/
void NextTransfer() {
	
	if ( indexOfBit < OUT_OF_RANGE_BIT ) {
		 
		//������� ������� ���������
		PORTB &= ~( 1 << 3 );
		
		if ( readedSymbol & ( 1 << indexOfBit ) ) {

			//��������� ����
			timerCounter = DASH_DURATION;
		}
		else {

			//��������� �����
			timerCounter = POINT_DURATION;
		}
		
		++indexOfBit;
	}
	else {
		
		//������ ��� ������������ ������, ������ �����
		timerCounter = DASH_DURATION;
	}
	
	StartTimer();
}


/*---------------------------------------------------------
���������� ����� (readedSymbol). ���������� ������� �����
������ ����������� �������.
---------------------------------------------------------*/
unsigned int FindStartingBitPosition() {
	
	for ( int i = 0; i < OUT_OF_RANGE_BIT; i++ ) {
		
		//������ �� ����� �� ������ �������
		if( readedSymbol & ( 1 << i ) ) {
			
			return ++i;
		}
	}
	
	//��� ������
	return 8;
}


/*---------------------------------------------------------
������������ ���������� ���������.
---------------------------------------------------------*/
void ReadNextSymbol() {

	if ( message[ index ] != '\0' ) {
		 
		readedSymbol = TranslateToTelegraph( message[ index ] );
		indexOfBit = FindStartingBitPosition();
		++index;
		NextTransfer();
	}
	else {
		
		//����� � ������ ���������
		timerCounter = DELAY_BETWEEN_MESSAGE;
		index = 0;
		indexOfBit = OUT_OF_RANGE_BIT;
		StartTimer();
	}
}


/*---------------------------------------------------------
��������� ���������� �� ������������.
---------------------------------------------------------*/
ISR( TIMER0_OVF_vect ) {
	
	//��������� ����������
	cli();
	//������������ 0
	TCCR0B &= ~( 1 << CS02 );
	
	--timerCounter;
	
	if ( timerCounter < 1 ) {
		
		//���� �� ��3 +5�
		if( !(PINB & 1 << 3) ) {
			
			//������������� ������� ���������
			PORTB |= ( 1 << 3 );
			
			//����� ����� ��������
			if( indexOfBit < OUT_OF_RANGE_BIT ) {
				
				//����� ����� ������ ���������
				timerCounter = POINT_DURATION;
			}
			else {
				
				//����� ����� ��������� ��������� � ������ �������
				timerCounter = DASH_DURATION;
			}
			
			StartTimer();
		}
		//���� ������������ ����� ���� � ��3 = 0
		else {
			
			//���� ���� ����������� ���� � ������������ �������
			if( indexOfBit < OUT_OF_RANGE_BIT ) {
				
				//��������� ��������
				NextTransfer();
			}
			else {
				
				//����� ���������� ���������� �������
				ReadNextSymbol();
			}
		}
	}
	else {
		
		StartTimer();
	}
}


int main() {
	
	//���� � �� �����
	DDRB |= ( 1 << 3 );
	//������������� ������� ���������
	PORTB |= ( 1 << 3 );
	//���������� �������
	//������ ����������
	cli();
	//��������� ���������� �� ������������
	TIMSK |= ( 1 << TOIE0 ); 
	//��������� ����������
	sei();
	
	ReadNextSymbol();
	
	while ( true ) {
	         
	}
	
	return 0;
}