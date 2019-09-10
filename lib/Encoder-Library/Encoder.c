// encoder
// -*- mode: C++ -*-
//Для энкодеров, которые за один щелчок совершают полный импульс
//http://arduino.ru/forum/apparatnye-voprosy/ispolzuem-enkoder?page=5#comment-449417
//МОДУЛЬ ПОДДЕРЖКИ ЭНКОДЕРА (От Леонида Ивановича)
//Для подавления дребезга используется анализ двух последовательных
//состояний. Это позволяет обойтись без временных задержек.
//Функция EncoderInit() должна вызываться один раз в начале программы.
//При повороте энкодера на шаг вправо или влево изменяется значение глобальной переменной EncoderValue.
//
// Copyright (C) 2019 ua3nbw

//Энкодер на пинах А0, А1. Используется внутренняя подтяжка.
//кнопка энкодера на D3


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "Encoder.h"

//-----------------------------------------------------------------------------------------------

void EncoderInit() {
	pinMode(3, INPUT_PULLUP);//кнопка энкодера D3
	attachInterrupt(1, int_click, LOW); // привязываем 1-е прерывание к функции int_click().
	//Энкодер на пинах А0, А1. Используется внутренняя подтяжка.
	pinMode(A0, INPUT_PULLUP);
	pinMode(A1, INPUT_PULLUP);
	PCIFR = PCIFR; PCICR = 1 << PCIE1; //разрешить прерывание
	PCMSK1 = 1 << PCINT8 | 1 << PCINT9; //выбрать вход на котором сработает прерывание

}

//-----------------------------------------------------------------------------------------------
ISR(PCINT1_vect) {
	static char EncPrev = 0;    //предыдущее состояние энкодера
	static char EncPrevPrev = 0; //пред-предыдущее состояние энкодера
	char EncCur = 0;
	if (!(PINC & (1 << PC0))) {EncCur  = 1;} //опрос фазы 1 энкодера
	if (!(PINC & (1 << PC1))) { EncCur |= 2;} //опрос фазы 2 энкодера
	if (EncCur != EncPrev) {          //если состояние изменилось,
		if (EncPrev == 3 &&       //если предыдущее состояние 3
		        EncCur != EncPrevPrev ) {    //и текущее и пред-предыдущее не равны,
			if (EncCur == 2)         //если текущее состояние 2,
			{ EncoderValue++; }            //шаг вверх
			else                          //иначе
			{ EncoderValue--; }            //шаг вниз
		}
		EncPrevPrev = EncPrev;          //сохранение пред-предыдущего состояния
		EncPrev = EncCur;               //сохранение предыдущего состояния
	}
}

//-----------------------------------------------------------------------------------------------
void int_click() {
	static unsigned long millis_prev;
	if (millis() - 100 > millis_prev) { button = true; } // меняем значение на противоположное
	millis_prev = millis();
	//flag_int_click = true;
}