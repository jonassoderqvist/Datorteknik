#include <pic32mx.h>
#include <stdint.h>

extern void _enable_interrupt();
int flags = 0;
int hex_num[20];

// variables for the wave-generation
volatile double  waveValueA = 0;
volatile double  waveValueB = 0;
volatile double  waveValueC = 0;
volatile double  waveValueD = 0;

char operandA = 'A';
char operandB = 'A';
char operandC = 'A';
char operandD = 'A';

volatile int wavelengthCounterA = 0;
volatile int wavelengthCounterB = 0;
volatile int wavelengthCounterC = 0;
volatile int wavelengthCounterD = 0;

int pulse = 0xFF;
static const off = 0xff;
/* Does it work? */
/* Defines pulse-widths for two octaves for a set of tones */
static const E6 = 3800;
static const D6 = 4250;
static const C6 = 4800;
static const E5 = 7584;
static const D5 = 8513;
static const C5 = 9555;

extern void _enable_interrupt();
int flags = 0;
int hex_num[20];

void initSynth() {
	for(;;) {
	    int[4] notes;
		int btns = getBtns();
		int switches = getSwitches();
		
		if((switches & 0b001) == 0b001) {
			if((btns & 0b001) == 0b001) {
			int n = sizeof(notes) / sizeof(int);
				notes[n+1]=E5;   // E
			}
			if((btns & 0b010) == 0b010) {
				int n = sizeof(notes) / sizeof(int);
                				notes[n+1]=D5;    //D
			}
			if((btns & 0b100) == 0b100) {
				int n = sizeof(notes) / sizeof(int);
                				notes[n+1]=C5;           // C
			}
		}		
		
		if((btns & 0b0001) == 0b0001) {
			playToneRev(E6, 2);   // E
			
		}
		if((btns & 0b010) == 0b010) {
			int n = sizeof(notes) / sizeof(int);
            				notes[n+1]=D6;    //D
		}
		if((btns & 0b100) == 0b100) {
			int n = sizeof(notes) / sizeof(int);
            				notes[n+1]=C6;           // C
		}
		playTone(notes);
		//klocka 3,B
		if((IFS(0)&0x00001000)==0x00001000){
			wavelengthCounterB++;
			switch(operandB){
				case 'A' :
					waveValueB += 0.001;
					break;
				case 'S':
					waveValueB -= 0.001;
					break;
			}
			if(wavelengthCounterB > 400000){
				switch(operandB){
					case 'A' : 
					operand = 'S';
					break;
					case 'S' : 
					operand = 'A';
					break;
				}
				wavelengthCounterB = 0;
			}			
			IFSCLR(0) = 0x00001000;// Clear the timer interrupt status flag
		}
		
		//klocka 4,C
		if((IFS(0)&0x00010000)==0x00010000){
			switch(operandC){
				case 'A' :
					waveValueC += 0.001;
					break;
				case 'S':
					waveValueC -= 0.001;
					break;
			}
			if(wavelengthCounterC > 400000){
				switch(operandC){
					case 'A' : 
					operand = 'S';
					break;
					case 'S' : 
					operand = 'A';
					break;
				}		
				wavelengthCounterC = 0;
			}			
			IFSCLR(0) = 0x00010000;// Clear the timer interrupt status flag
		}
		
		//klocka 5,D
		if((IFS(0)&0x00100000)==0x00100000){
			switch(operandD){
				case 'A' :
					waveValueD += 0.001;
					break;
				case 'S':
					waveValueD -= 0.001;
					break;
			}			
			if(wavelengthCounterD > 500000){
				switch(operandD){
					case 'A' : 
					operand = 'S';
					break;
					case 'S' : 
					operand = 'A';
					break;
				}
				wavelengthCounter = 0;
			}	
			IFSCLR(0) = 0x00100000;// Clear the timer interrupt status flag		
		}
		
		
		//klocka 2, A
		if((IFS(0)&0x00000100)==0x00000100){
			wavelengthCounterA++;
			wavelengthCounterC++;
			wavelengthCounterD++;
				
			switch(operandA){
				case 'A' :
					waveValueA += 0.001;
					break;
				case 'S':
					waveValueA -= 0.001;
					break;
			}
			
			if(wavelengthCounterA > 300000){
			switch(operandA){
				case 'A' : 
				operand = 'S';
				break;
				case 'S' : 
				operand = 'A';
				break;
			}		
			wavelengthCounter = 0;
		}
			PORTE = 0x0f;
			IFSCLR(0) = 0x00000100;// Clear the timer interrupt status flag
			PORTE = 0x00;
		}	
	}
}

int startTimer(int timerNum){
	switch(timerNum){
		case 2:
			T2CONSET = 0x8000; // Start the timer2
			break;
		case 3:
			T3CONSET = 0x8000; // Start the timer2
			break;
		case 4:
			T3CONSET = 0x8000; // Start the timer2
			break;
		case 5:
			T3CONSET = 0x8000; // Start the timer2
			break;
	}
	return 0;
}

int stopTimer(int timerNum){
	switch(timerNum){
		case 2:
			T2CONSET = 0x0000; // Start the timer2
			break;	
		case 3:
			T3CONSET = 0x0000; // Start the timer2
			break;
		case 4:
			T3CONSET = 0x0000; // Start the timer2
			break;
		case 5:
			T3CONSET = 0x0000; // Start the timer2
			break;
	}
	return 0;
}

int initTimers(){
	// Initiates the timers that ultimately will generate a oscilating sawtooth-wave/sound-wave/sinus-wave
	
	T2CON = 0x0; // Stop the timer and clear the control register, prescaler at 1:1,internal clock source
	T3CON = 0x0;
	T4CON = 0x0;
	T5CON = 0x0;
	
	TMR2 = 0x0; // Clear the timer register
	TMR3 = 0x0;
	TMR4 = 0x0;
	TMR5 = 0x0;
	
	PR2 = 0xff; // Load the period register
	PR3 = 0xff;
	PR4 = 0xff;
	PR5 = 0xff;

	IPCSET(2) = 0x0000000D; // Set priority level = 3,Set subpriority level = 1
	IPCSET(3) = 0x0000000D;
	IPCSET(4) = 0x0000000D;
	IPCSET(5) = 0x0000000D;

	IFSCLR(0) = 0x00111100; // Clear the timer interrupt status flags
	IECSET(0) = 0x00111100; // Enable timer interrupts
	
	return 0;
}

int getBtns(void) {
	return((PORTD >>5) &0x7);	/* Port D bits 5 through 8 is used for the Buttons and is set to 1 (input) */
}

int getSwitches(void) {
	return((PORTD>>8) &0xF); 	/* Port D bits 8 through 12 is used for the Switches and is set to 1 (input) */
}

int getbtn(void) {
	return(PORTF &0x1);
}

/*  Pulsewidth to Frequency:
	(pulseWidth * 0.0000001) * 2 = periodTime
	(1 / periodTime) = frequency
	------------ OR -------------
	Frequency (aka tone) to pulsewidth:
	(1 / Frequency) = periodTime
	( PeriodTime / 2 ) / 0.0000001 = pulseWidth */

int playToneRev(pulseWidth, x) {
			PORTE = pulse;
			delay(pulseWidth);
			PORTECLR = off;
			delay(pulseWidth);
			
			if(x < 10){
					playTone(pulseWidth, (++x));
			}
			// PWM HERE
			// OC1 !		
}

void playTone(int[] pulseWidth) {
    if (pulseWidth[3] != 0){
        return pwm(((pulseWidth[0]+pulseWidth[1]+pulseWidth[2]+pulseWidth[3])/4), 50);
    }
    if (pulseWidth[2] != 0){
        return pwm(((pulseWidth[0]+pulseWidth[1]+pulseWidth[2])/3), 50);
    }
    if (pulseWidth[1] != 0){
        return pwm(((pulseWidth[0]+pulseWidth[1])/2), 50);
    }
    if (pulseWidth[0] != 0){
        return pwm(pulseWidth[0], 50);
    }
    return 0;
}

void user_isr( void ) {
	IFSCLR(0) = 0x0100;
}
void pwm(int pwm, int dutycycle){
	T2CON = 0x070; // Clear timer2, prescale at 1:1
	TMR2 = 0x0; // Timer2 value starts at 0
	OC1CON = 0x0000; // Turn off and clear pwm
	OC1R = 0x0064;
	OC1RS = dutycycle; // Dutycycle
	OC1CON = 0x0006; // Configure for PWM mode without Fault pin


	PR2 = pwm; // Set dutycycle, HÄR ÄNDRAR MAN TONER!
	IECSET(0) = 0x0100; // Enable T2 interrupt
	IPCSET(2) = 0x01C; // Set T2 interrupt priority to 7

	enable_interrupt();

	OC1CONSET = 0x08000; // Enable OC1
	T2CONSET |= 0x08000; // Enable Timer2
	return 0;
}

int main(void) {
	TRISE &= ~0xff; 	/* Port E bits 0 through 7 is used for the LED and is set to 0 (output) */
	initTimers();
	initSynth();
	return 0;
}

// Vad mer göra?
// Använda oss av timers, inbyggd pwm-funktion och volym.
// Kunna spela fyra toner



// Timers?
// Inbyggda PWM-funktionen?
// Volym
//
// Lägga till funktioner med utgångspunkt i musiken:
// Delay
// Tremolo
// Reverb
// Volym
// Lägga till fler tangenter


// förändra dutycycle för att simulera sinuskurva med olika 
