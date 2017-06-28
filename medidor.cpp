/**************************************************************************************************************************

   Autor: Marcelo Henrique Moraes
   E-mail: marceloh220@hotmail.com
   Copyright (c) 2017 Marcelo Henrique Moraes

   MCU: Atmega88 (can be used with others avr-8bits without/few modifications)
   F_CPU: 16MHz
   avr-gcc 7.1
   avr-libc 2.0
   avr-binutils 2.28

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



#include "defines/defines.h"
#include "class/display.h"

Display display;

int main() {
	
	display.print("Hello World");
	display.set(0,1);
	display.print(-22.43);

	while(1) {
  
	}
	
	return 0;

}

