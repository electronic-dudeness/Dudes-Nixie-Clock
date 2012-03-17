//#include <Wire.h>
#include <TimerOne.h>


#define multiplex_speed 375 //change anode every usecs
#define numAnodes 4

#define anodesOff() (PORTD &= B11000011) //all anodes off
#define anodesOn() (PORDT |= B00111100) // all anodes on
#define hitAnode(anode) (PORTD = (2 << anode) | (PORTD & B11000011)) //turn on specific anode
#define displayNumber(number) (PORTB = (number << 4) | (PORTB & B00001111)) //sink certain number on K155

byte pwm_duty = 2; // fraction of pwm_cycles tubes are OFF
const byte pwm_cycles = 24; //should be a multiple of the number of anodes.

volatile byte number[numAnodes]; //one value per anode
volatile byte anode=1; //variable holds currently lit tube
volatile byte pwm_cycle=0; // pwm counter 

void setup() {
  DDRB |= B11110000; //set port B4 to B7 as outputs for k155
  DDRD |= B00111100; // set port D2 to D5 as anode outputs for MPSA42/92
  Timer1.initialize(multiplex_speed);
  Timer1.attachInterrupt(display);      
  number[0]=0;
  number[1]=1;
  number[2]=2;
  number[3]=3;
}

void display() {
  //tell K155 to display the number that should be on the currently lit tube
  anodesOff();
  displayNumber(number[anode-1]);
  
  
  if (!pwm_cycle % pwm_duty == 0) //turn all anodes off after a complete if pwm duty is reached.
    hitAnode(anode);

  //go to the next anode
  anode++;
  if (anode > numAnodes)
    anode=1;
  
  //and advance the pwm cycle  
  if (anode == 1)
    pwm_cycle++; 
  if (pwm_cycle > pwm_cycles)
    pwm_cycle=0;
}

void loop() {
  delay(1500);
  for (char i=0; i<numAnodes; i++)
  {
    number[i]++;
    if (number[i] > 9)
      number[i]=0;
  }
}
