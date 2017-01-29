#include <pic32mx.h>
#include <stdint.h> 

extern void _enable_interrupt();
int flags = 0;
int hex_num[20];

int main(void) {
	TRISE = 0x00; // Enable led
	PORTE = 0x00;
	
	T2CON = 0x070; // Clear timer2, prescale at 1:1
	TMR2 = 0x0; // Timer2 value starts at 0
	OC1CON = 0x0000; // Turn off and clear pwm
	OC1R = 0x0064;
	OC1RS = 0x0064; // Dutycycle
	OC1CON = 0x0006; // Configure for PWM mode without Fault pin 

	
	PR2 = 255; // Set dutycycle, HÄR ÄNDRAR MAN TONER!
	IECSET(0) = 0x0100; // Enable T2 interrupt
	IPCSET(2) = 0x01C; // Set T2 interrupt priority to 7

	enable_interrupt();
	
	OC1CONSET = 0x08000; // Enable OC1
	T2CONSET |= 0x08000; // Enable Timer2
	
	int i = 0;	
	for(i = 0; i < 15000000; i++){
	}
	
	PR2 = 255.3333333;
	
	for(i = 0; i < 15000000; i++){
	}
	
	PR2 = 254.7777777;
	
	return 0;
}

void dectohex(int dec_num){
	int i=0;
 while (dec_num) {
	hex_num[i] = dec_num % 16;
	dec_num = dec_num / 16;
	i++;
 }
}

void user_isr( void ) {
	IFSCLR(0) = 0x0100;
}


