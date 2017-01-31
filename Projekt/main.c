#include <pic32mx.h>
#include <stdint.h>
#include "waveGen.c"

int pulse = 0xFF;
static const off = 0xff;
Note array[11];
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
void initNotes(){

array[0] = Note C;
array[1] = Note C#;
array[2] = Note D;
array[3] = Note D#;
array[4] = Note E;
array[5] = Note F;
array[6] = Note F#;
array[7] = Note G;
array[8] = Note G#;
array[9] = Note A;
array[10] = Note A#;
array[11] = Note B;

C.number = 0;
C#.number = 1;
D.number = 2;
D#.number = 3;
E.number = 4;
F.number = 5;
F#.number = 6;
G.number = 7;
G#.number = 8;
A.number = 9;
A#.number = 10;
B.number = 11;

C.freq = 4800;
C#.freq = 4500;
D.freq = 4300;
D#.freq = 4100;
E.freq = 3900;
F.freq = 3700;
F#.freq = 3500;
G.freq = 3300;
G#.freq = 3100;
B.freq = 2900;
B#.freq = 2700;

C.A = 0;
C#.A = 0;
D.A = 0;
D#.A = 0;
E.A = 0;
F.A = 0;
F#.A = 0;
G.A = 0;
G#.A = 0;
B.A = 0;
B#.A = 0;

C.play = 0;
C#.play = 0;
D.play = 0;
D#.play = 0;
E.play = 0;
F.play = 0;
F#.play = 0;
G.play = 0;
G#.play = 0;
B.play = 0;
B#.play = 0;

}
void initSynth() {
	for(;;) {
		int[2] notes;
		int btns = getBtns();
		int switches = getSwitches();
		
		int numNotes = 0;
		
		if((btns & 0b0001) == 0b0001) {
			//playToneRev(E6, 2);   // E
			    //notes = notes + waveValueA;
			    //numNotes++;
			    array[0].play = TRUE;

		}else{
		      array[0].play = FALSE;
		}
		if((btns & 0b010) == 0b010) {
            //playToneRev(E6, 2);   // E
            //notes = notes + 300;
            //numNotes++;
            array[1].play = TRUE;
		}
		else{
		    array[1].play = FALSE;
		}
		if((btns & 0b100) == 0b100) {
            //playToneRev(E6, 2);   // E
            //notes = notes + 350;
            //numNotes++;
            array[2].play = TRUE;
		}else{
		    array[2].play = FALSE;
		}
		/*
		int noteToPlay = notes/numNotes;
		playTone(noteToPlay);
		*/
		int c;
		int nr;
	for(i = 0; i < sizeof(array) / sizeof(Note); i++)
        {
        if(array[i].play == TRUE){
          c = c + array[i].A;
          gen(array[i]);
          nr++;
          }
        }
        setPwm(c/nr, 50);
	}
}

int getBtns(void) {
 	return((PORTD >>5) &0x7);	/* Port D bits 5 through 8 is used for the Buttons and is set to 1 (input) */
 }

 int getSwitches(void) {
 	return((PORTD>>8) &0xF); 	/* Port D bits 8 through 12 is used for the Switches and is set to 1 (input) */
 }

 int getbtn(void) {
 	return(PORTD &0x1);
 }

/*  Pulsewidth to Frequency:
	(pulseWidth * 0.0000001) * 2 = periodTime
	(1 / periodTime) = frequency
	------------ OR -------------
	Frequency (aka tone) to pulsewidth:
	(1 / Frequency) = periodTime
	( PeriodTime / 2 ) / 0.0000001 = pulseWidth */

void initPwm(){
	T2CON = 0x070; // Clear timer2, prescale at 1:1
	TMR2 = 0x0; // Timer2 value starts at 0
	OC1CON = 0x0000; // Turn off and clear pwm
	OC1R = 0x0064;
	OC1RS = 0; // Dutycycle
	OC1CON = 0x0006; // Configure for PWM mode without Fault pin
	PR2 = 0; // Set dutycycle, HÄR ÄNDRAR MAN TONER!
	IECSET(0) = 0x0100; // Enable T2 interrupt
	IPCSET(2) = 0x01C; // Set T2 interrupt priority to 7
	enable_interrupt();
	OC1CONSET = 0x08000; // Enable OC1
	T2CONSET |= 0x08000; // Enable Timer2
}

void setPwm(int pwm, int duty){
    int dutycycle = 0xFFFFFFFF * (duty / 100);
	OC1RS = dutycycle;
	PR2 = pwm;
}

int main(void) {
	TRISE = 0x00; 	/* Port E bits 0 through 7 is used for the LED and is set to 0 (output) */
	PORTE = 0x00;
	initNotes();
	initTimers();
	initPwm();
	initSynth();
	return 0;
}

void user_isr( void ) {
	if((IFS(0)&0x0100)==0x0100){
		IFSCLR(0) = 0x0100;
	}
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
