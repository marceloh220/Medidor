#ifndef _DISPLAY_H_
#define _DISPLAY_H_

/**************************************************************************************************************************

   Autor: Marcelo Henrique Moraes
   E-mail: marceloh220@hotmail.com
   Copyright (c) 2017 Marcelo Henrique Moraes

   Can be used with any avr-8bits without/few modifications
   avr-gcc 7.1
   avr-libc 2.0
   avr-binutils 2.28

	A tinny and minimalist library to display HD44780
	Matriz 5x8 pixels
	16/2 characteres
	 
	 RS:  PB3
	 E:   PB4
	 PD4: PD4
	 D5:  PD5
	 D6:  PD6
	 D7:  PD7

    This application is a free software, you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 3.0 of the License, or (at your option) any later version.
    This application is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY, without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    https://www.gnu.org/licenses/gpl-3.0.en.html

***************************************************************************************************************************/

#include <avr/io.h>//auxiliar avr libraries(i do not fan in decorating the address of the registers)

class Display {

private:

	void delay2ms() {//delay of 2ms
		// time = cicles / F_CPU
		// cicles2ms * 16e6 = 32000 ... need 2 registers
		// dec = 1 cicle, brne = 2 cicles ... 3 cicles per loop
		// 32000 / 3 = 10666.666 cicles ... stil 3 registers '-'
		// with 1 register of 8bits
		// 10666 / 255 = ~42
		// nothing needs to be very accurate in this application,
		// i did in assembly just for demonstration...
		asm volatile (
			"   ldi		r18,42" 	"\n"//load imediate the constant 4 to register r18
			"	clr		r19"		"\n"//clear the register r19
			"delay2ms:"   			"\n"//label to jump and branch
			"	dec		r19"		"\n"//decrement 1 from register r19, r19 will be drecreassed 255 times
			"   brne	delay2ms"	"\n"//branch to label 'delay2ms' if register is not zero
			"   dec		r18"		"\n"//decrement 1 from register r18, r18 will be drecreassed 42 times
			"   brne	delay2ms"	"\n"//branch to label 'delay2ms' if register is not zero
		);
	}//end delay2ms

	inline void enable() {//send a enable pulse
		PORTB |= (1<<PB4);//enable pulse to display controller
		
		// time = cicles / F_CPU
		// cicles = 500us * 16e6 = 8
		// dec = 1 cicle, brne = 2 cicles ... 3 cicles per loop
		// 8 / 3 = ~3 cicles
		asm volatile (
			"   ldi		r18,3" 	"\n"//and more of the same, load constant...
			"delay500ns:"   		"\n"//label...
			"   dec		r18"		"\n"//decrement 1 from register bla bla bla...
			"   brne	delay500ns"	"\n"//branch if, etc etc etc...
		);//wait for ~500ns
		
		PORTB &= ~(1<<PB4);//prepare to another enable pulse
		
		// time = cicles / F_CPU
		// cicles = 40us * 16e6 = 640
		// dec = 1 cicle, brne = 2 cicles ... 3 cicles per loop
		// 640 / 3 = ~213 cicles
		asm volatile (
			"	ldi		r18,213"	"\n"//read the first subroutine of delay...
			"delay40us:"   			"\n"//...
			"	dec		r18"		"\n"//...
			"	brne	delay40us"	"\n"//...
		);//wait for ~40us to controller processe the operation
	
	}//end enable pulse

	void cmd(uint8_t command) {//send a commander to display controller
		
		PORTB &= ~(1<<PB3);//send a commander to display controller
		
		PORTD &= ~0xF0;//clear data pins
		PORTD |= (0xF0&(command<<0));//send the MSB to display controller
		this->enable();//enable pulse
		
		PORTD &= ~0xF0;//clear data again
		PORTD |= (0xF0&(command<<4));//send the LSB to display controller
		this->enable();//enable pulse again
	
	}//end cmd

	void write(uint8_t data) {//send a data to display controller
		
		PORTB |= (1<<PB3);//send a data to display controller
		
		PORTD &= ~0xF0;//clear data pins
		PORTD |= (0xF0&(data<<0));//send the LSB to display controler
		this->enable();//enable pulse
		
		PORTD &= ~0xF0;//clear data again
		PORTD |= (0xF0&(data<<4));//send the MSB to display controller
		this->enable();//enable pulse again
	
	}//end write
	
	void init() {//initiate the display controller
		
		DDRD |= 0xF0;//PD4 to PD7 like output
		DDRB |= (1<<PB3)|(1<<PB4);//RS and E like output
		
		this->delay2ms();//wait power estableshed
		
		PORTD &= ~0xF0;//clear data pins
		
		PORTD |= 0x30;//load a commander to initiate the display in 8bits mode
		
		this->enable();//try1 with 8bits
		this->delay2ms();
		
		this->enable();//try2 with 8bits
		this->delay2ms();
		
		this->enable();//try3 with 8bits
		this->delay2ms();
		
		PORTD &= ~0xF0;//clear data pins
		
		PORTD |= 0x20;//load a commander to initiate the display in 4bits mode
		
		this->enable();//change to 4bits mode
		this->delay2ms();
		
		this->cmd(0x28);//initiate lcd
		this->delay2ms();
		
		this->cmd(0x0C);//no cursor
		this->delay2ms();
		
		this->cmd(0x01);//clear display
		this->delay2ms();
		
	}//end init

public:
	//
	Display() { this->init(); }

	void clear() {//clear display
		this->cmd(0x01);//send commander
		this->delay2ms();
	}//end clear

	void set(uint8_t coll, uint8_t row) {//ser the cursor
		if(row) row = 0xC0;//if row 1, address DDRAM 0xC
		else row = 0x80;//else, row 0, address DDRAM 0x8
		this->cmd(row|coll);//sendo row and coll to controller
		this->delay2ms();//wait position
	}//end set

	//send a caracter data
	void print(char c) { this->write(c); }
	
	//send a string data
	void print(const char *string) {
		while(*string != '\0') {//while not hit the end of string
			this->write(*string);//send the caracter
			string++;//increassed the addres of string
		}
	}//end print(string)

	//print a integer number
	void print(int num) {
		
		unsigned char aux[6];//only 6 positions to write max 6 characteres,
							 //more is unecessary for this application
		
		uint8_t i = 0;//interator to alocate the characteres
		uint8_t test = 0;//test if number is negative
		
		if(num < 0) {//if number is negative
			test = 1;//set test to pring '-' signal
			num = -num;//invert the number to positive
		}
		
		//convert the number in characteres
		do {
			aux[i++] = (num % 10) + '0';//convert and allocate the unities
			num /= 10;//to process the new unit
		} while(num);//while the number is not zero
		
		i--;//correct the poss
		
		if(test) this->write('-');//if number is negative print '-' signal
		
		while(i != 255) this->write(aux[i--]);//while is not the end of string, print the characteres
		
	}//end print(int)

	//print a float number
	void print(double num) {
		
		unsigned char aux[7];//only 7 positions to write max 7 characteres,
							 //more is unecessary for this application
							 
		uint8_t i = 0;//interator to alocate the characteres
		uint8_t test = 0;//test if number is negative
		
		if(num < 0.0) {//if number is negative
			test = 1;//set test to pring '-' signal
			num = -num;//invert the number to positive
		}
		
		uint16_t inum = num * 100;//convert the float in integer number
		
		for(uint8_t b = 0; b < 2; b++) {//first convert the float part
			aux[i++] = (inum % 10) + '0';
			inum /= 10;
		}
		
		aux[i++] = '.';//put the dot signal
		
		//convert the number in characteres
		do {
			aux[i++] = (inum % 10) + '0';//convert and allocate the unities
			inum /= 10;//to process the new unit
		} while(inum);//while the number is not zero
		
		i--;//correct the poss
		
		if(test) this->write('-');//if number is negative print '-' signal
		
		while(i != 255) this->write(aux[i--]);//while is not the end of string, print the characteres
		
	}//end print(double)

};


#endif
