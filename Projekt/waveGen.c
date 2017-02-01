#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Change length of the Steps.
#define step = 0.000000001;

typedef struct
{
int number;
double freq;
double A;
double x;
bool play;
} Note;

void gen(Note p){
float pi = 3.141592;
double x = p.x;
double freq = p.freq;
double A = sin(freq*x*step*2*pi);
x++;
p.A = A;
p.x = x;
return 0;
}