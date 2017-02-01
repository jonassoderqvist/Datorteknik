#include <pic32mx.h>
#include <stdint.h>

extern void _enable_interrupt();

//Change length of the Steps.
#define step 0.00001

struct Note{
int number;
int freq;
double A;
double x;
int play;
}Note;

struct Note array[12];
	
void gen(int i){
	float pi = 3.141592;
	double freq = array[i].freq;
	double x = array[i].x;
	double A = ((sin(x*step*freq) + 1) * 1000);
	array[i].x = x++;
	array[i].A = A;
}

void initNotes(){

	
	
	struct Note C = {0, 500, 0.0, 0.0, 0};
	struct Note Ciss = {1, 500, 0.0, 0.0, 0};
	struct Note D = {2, 500, 0.0, 0.0, 0};
	struct Note Diss = {3, 4100, 0.0, 0.0, 0};
	struct Note E = {4, 3900, 0.0, 0.0, 0};
	struct Note F = {5, 3700, 0.0, 0.0, 0};
	struct Note Fiss = {6, 3500, 0.0, 0.0, 0};
	struct Note G = {7, 3300, 0.0, 0.0, 0};
	struct Note Giss = {8, 3100, 0.0, 0.0, 0};
	struct Note B = {9, 2900, 0.0, 0.0, 0};
	struct Note Biss = {10, 2700, 0.0, 0.0, 0};
	
	array[0] = C;
	array[1] = Ciss;
	array[2] = D;
	array[3] = Diss;
	array[4] = E;
	array[5] = F;
	array[6] = Fiss;
	array[7] = G;
	array[8] = Giss;
	array[9] = B;
	array[10] = Biss;
	
	
}
void initSynth() {
	for(;;) {
		int btns = getBtns();
		// Check buttons. If button is pressed, corresponding note's play-value will be set to 1 (true)
		if((PORTD & 0b000000100000) == 0b000000100000){
			    array[0].play = 1; 

		}else{
		      array[0].play = 0;
		}
		if((PORTD & 0b000001000000) == 0b000001000000){
            array[1].play = 1;
		}
		else{
		    array[1].play = 0;
		}
		if((PORTD & 0b000010000000) == 0b000010000000){
            array[2].play = 1;
		}else {
		    array[2].play = 0;
		}
		
		// Check note-array for notes with play-value 1 (true). These are added together.
		int c;
		int nr;
		int i;
		for(i = 0; i < 11; i++){
			if(array[i].play == 1){
				c =  c + (array[i].A + 1);
				gen(i);
				nr++;
			}
        }
		
		// Set PWM to previously calculated value.
		if((c/nr) > 0){
        setPwm((c/nr), 50);
		} else { setPwm(0,50); }
		
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


void initPwm(){
	T2CON = 0x070; // Clear timer2, prescale at 1:1
	TMR2 = 0x0; // Timer2 value starts at 0
	OC1CON = 0x0000; // Turn off and clear pwm
	OC1R = 0x0001;
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
	OC1RS = duty;
	int pwm2 = pwm;
	PR2 = pwm2;
}

int main(void) {
	TRISE = 0x00; 	/* Port E bits 0 through 7 is used for the LED and is set to 0 (output) */
	PORTE = 0x00;
	initNotes();
	initPwm();
	initSynth();
	return 0;
}

void user_isr( void ) {
	if((IFS(0)&0x0100)==0x0100){
		PORTE = 0xffff;
		IFSCLR(0) = 0x0100;
		
	}
}

