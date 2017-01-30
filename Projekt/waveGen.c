#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Change length of the Steps.
#define step = 0.000000001;

typedef struct
{
char name;
int number;
int freq;
double A;
int x;
bool play;
} Note;

waveGen array[];

void gen(Note p){
float pi = 3.141592;
x = p.x;
freq = p.freq;
A = sin(freq*x*step*2*pi);
x++;
p.A = A;
p.x = x;
return 0;
}