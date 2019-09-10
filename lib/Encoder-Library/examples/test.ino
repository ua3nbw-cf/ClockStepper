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

#include "Encoder.h"


void setup() {
	Serial.begin(9600); // открываем серийный порт для дебаггинга
	while (!Serial) {
		delay(1);
	}
	Serial.println(F("Starting ..."));

	EncoderInit();//Инициализация энкодера

}

//-----------------------------------------------------------------------------------------------
void loop() {

	if (EncoderValue != newpos) {

		Serial.print("EncoderValue --   "); Serial.println(EncoderValue);
		newpos = EncoderValue;

	}


	if (flag_int_click ) { // установка времени энкодером и запись в PCF2129 при клике кнопки энкодера

		EncoderValue = 6;
		TimeSet(1, 12);
		int8_t sethour = EncoderValue;
		EncoderValue = 30;
		TimeSet(0, 59);
		int8_t setmin = EncoderValue;

		Serial.print("   record hour -"); Serial.println(sethour);
		Serial.print("    record  min -"); Serial.println(setmin);
		Serial.println(".");

	}

	flag_int_click = false;
}


//-----------------------------------------------------------------------------------------------
void TimeSet(uint8_t posmin, uint8_t posmax) { //пределы установки минут и часов

	flag_int_click = !flag_int_click;  // меняем значение на противоположное
	Serial.println("--------------------set time---------------------");
	for ( ; ; ) {

		EncoderValue = (EncoderValue < posmin) ? posmin : EncoderValue;//if (EncoderValue < posmin)  { EncoderValue = posmin; }
		EncoderValue = (EncoderValue > posmax) ? posmax : EncoderValue;//if (EncoderValue > posmax)  { EncoderValue = posmax; }


		if ((EncoderValue != newpos) && !((EncoderValue < posmin) || (EncoderValue > posmax)  )) {

			Serial.print("EncoderValue --   "); Serial.println(EncoderValue);
			newpos = EncoderValue;

		}


		//delay(10);
		if (flag_int_click) { break; } //выход из цикла при следующем клике
	}

}


