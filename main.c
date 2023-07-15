/***********************************************************************
* Application for AVR Rough API                                        *
* @author Ahmed Elzoughby                                              *
* @date July 12, 2023                                                  *
* Purpose: Show how to use the GPIO driver in the applications         *
*          Debugged with UART thru the onboard CH340 IC for the nano   *
*		   which is home to an ATMega328P                              *
***********************************************************************/

#include <GPIO/gpio.c>
#include <util/delay.h>

// Need for BR calculation with usart.h
#define F_CPU 16000000UL
#define UART_BAUD 115200UL

#include <UART/uart.c>

uint8_t numBits = 6;
uint8_t flagCount = 0;
uint8_t i;					// bad name for a modular program
uint8_t maxBitPos = 7;

uint8_t buff[2];

int main(void) {

	UART_Init(TX, false, NONE);
	gpio_port_init(BASE_B, DIR_OUTPUT);

	UART_Transmit_String((unsigned char*)"Welcome to ATMega328P Driver Dev. by Kevin Harper");
	UART_Transmit_NL(2, false);

	_delay_ms(1000);

	while(1) {

		UART_Transmit_String((unsigned char*)"Flashing all bits on port B...");
		UART_Transmit_NL(2, false);

		// Dont flash pinB6 or 7; external oscillator pins
		for (i = 0; i <= maxBitPos - 2; i++) {
			gpio_pin_write(BASE_B, i, HIGH);
			_delay_ms(100);
			gpio_pin_write(BASE_B, i, LOW);
			_delay_ms(100);
			UART_Transmit_Byte(i + 0x30);
			UART_Transmit_String((unsigned char*)" done.");
			UART_Transmit_NL(1, false);
		}

		UART_Transmit_NL(1, false);
		_delay_ms(100);

		//UART_Transmit_String((unsigned char*)"Mackenzie");
		//UART_Transmit_NL(2, false);

		UART_Transmit_String((unsigned char*)"All pins done.");
		UART_Transmit_NL(2, false);
		_delay_ms(100);

		UART_Transmit_String((unsigned char*)"Starting UART Flag Check...");
		UART_Transmit_NL(2, false);

		for (i = maxBitPos; i >= maxBitPos - numBits; i--) {
			// Dont increment for transmit complete or UDRE
			if ((UART_Check_Flag(i) == true) && (!(i == TXC) || (i == UDRE))) flagCount++;
		}

		if (flagCount == 0) {
			UART_Transmit_String((unsigned char*)"No flags found.");
			UART_Transmit_NL(2, false);
		}
		else {
			decToASCII(buff, flagCount);
			UART_Transmit_String((unsigned char*)buff);
			UART_Transmit_String((unsigned char*)" flags found.");
			UART_Transmit_NL(2, false);
		}

		_delay_ms(2000);
	}
}

/* =================================================================================
 *                                    // END FILE //
 * ================================================================================= */