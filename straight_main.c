#include <stdio.h>
#include <stdlib.h>
#include "Gostraight.c"
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"


int time = 10000;
int max_speed;

int main (void){

  Gostraight(time, 10);
  return (0);
}
