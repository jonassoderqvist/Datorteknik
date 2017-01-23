#include <pic32mx.h>

int pulse = 0xFF;
static const off = 0xff;
/*Does it work? */
/* Defines pulse-widths for two octaves for a set of tones */
static const E6 = 3800;
static const D6 = 4250;
static const C6 = 4800;
static const E5 = 7584;
static const D5 = 8513;
static const C5 = 9555;

int combineNotes(int posA, int posB, int posC, int posD){
int notes = 0;
if (posA != NULL){
    return posA;
}
if (posB != NULL){
    return ((posA+posB)/2);
}
if (posC != NULL){
    return ((posA+posB+posC)/3);
}
if (posD != NULL){
        return ((posA+posB+posC+posD)/4);
}
}


void initSynth() {
	for(;;) {
		int btns = getBtns();
		int switches = getSwitches();
		
		if((switches & 0b001) == 0b001) {
			if((btns & 0b001) == 0b001) {
				playTone(E5);   // E
			}
			if((btns & 0b010) == 0b010) {
				playTone(D5);    //D
			}
			if((btns & 0b100) == 0b100) {
				playTone(C5);           // C
			}
		}		
		
		if((btns & 0b0001) == 0b0001) {
			playToneRev(E6, 2);   // E
			
		}
		if((btns & 0b010) == 0b010) {
			playTone(D6);    //D
		}
		if((btns & 0b100) == 0b100) {
			playTone(C6);           // C
		}
	}
}

void delay(int numOfCycles) {/* Time for delay(1) is approximately 0.0000001s */
	volatile int i;
	for(i = numOfCycles; i > 0; i--);
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

int playTone(pulseWidth) {
			PORTE = pulse;
			delay(pulseWidth);
			PORTECLR = off;
			delay(100-pulseWidth);
			// PWM HERE
			// OC1 !		
}

int main(void) {
	TRISE &= ~0xff; 	/* Port E bits 0 through 7 is used for the LED and is set to 0 (output) */
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