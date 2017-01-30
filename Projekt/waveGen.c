#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Change length of the Steps.
#define step = 0.000000001;

waveGen[] array;

Object* waveGen(char name, int number, int freq, double A) {
  Object* p = malloc(sizeof(Object));
  p->name = name;
  p->freq = freq;
  p->A = A;
  p->x = 0;
  p->play = 0;
  array[number] = p;
  return p;
}

void play(waveGen p, bool play){
    p->play = play;
}

bool getPlay(waveGen p){
    return p->play;
}

double getValue(waveGen p){
    return p->A;
}
void gen(waveGen p){
float pi = 3.141592;
x = p->x;
freq = p->freq;
A = sin(freq*x*step*2*pi);
x++;
p->A = A;
p->x = x;
return 0;
}

waveGen[] getArray(){
    return array[];
}