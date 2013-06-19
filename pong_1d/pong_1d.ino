//One-Dimensional Pong Game for the Elefu MMC84
//Coded by: Tim Anderson
//Date: 2013-06-17

/*
This is the code to run a one-dimensional pong game off the Elefu MMC84.

It operated by having two players with one button each, and a row 
of LEDs between them. One player will press a button, and the game 
begins. Then, the other player must press their button when the LED 
reaches the green LED on the end of the row. If the player does not 
press their button when the LED is on the green, their green LED 
will blink indicating that they lost. If they press it in time, the 
lights will travel back to the original player's direction, and that 
player must hit the button when the green LED at the end of the row 
is on.

This back and forth motion will continue until one player misses. 
Also, the speed that the lights travel from side to side increases 
every time someone correctly presses a button, making the game speed 
up more and more until someone loses.

This is a simple and fun circuit that you can make with your friends 
or children and will give you a way to learn about microcontrollers, 
while building a fun and challenging game of skill.
*/

//debounce library for buttons
#include <Bounce.h>

#define START_DELAY_TIME 100
#define DELAY_INCREMENT 2
#define DEBOUNCE_TIME 1

//these are for controlling LED flow direction
#define ASCENDING 1
#define DESCENDING 2

#define NUM_LEDS 8
int light_array[] = {1, 2, 3, 4, 5, 6, 7, 8};

int p1_button_pin = 9;
int p2_button_pin = 10;
Bounce p1b = Bounce(p1_button_pin, DEBOUNCE_TIME);
Bounce p2b = Bounce(p2_button_pin, DEBOUNCE_TIME);

//controls the direction the LEDs scan
int dir = ASCENDING;
//controls wether a game is currently being played
bool is_playing = false;
//this is the LED currently being lit as it progresses across the board
int current_LED = 0;
//this tells if it has blinked before the game starts
bool has_blinked = false;
//this is the actual current delay time
unsigned long delay_time = START_DELAY_TIME;
//this is the time the light last changed
unsigned long last_light_change = 0;
//this lets the game know if the player hit a button while the last LED was lit
bool p_hit = false;


void setup(){
  //set buttons as inputs
  pinMode(p1_button_pin, INPUT_PULLUP);
  pinMode(p2_button_pin, INPUT_PULLUP);
  //set LEDs as outputs and light them all up at the default speed
  for(int i=0; i<NUM_LEDS; i++){
    pinMode(light_array[i], OUTPUT);
    digitalWrite(light_array[i], HIGH);
    delay(START_DELAY_TIME);
    digitalWrite(light_array[i], LOW);
  }
}

void loop(){
  //update buttons every loop
  p1b.update();
  p2b.update();
  
  //do nothing until one of the two buttons is pressed
  if(p1b.fallingEdge() || p2b.fallingEdge()){//check to see if either button was pressed down
    //set the led closest to the button that was pressed as the starting LED
    if(p1b.fallingEdge()){//button 1 was pressed
      current_LED = 0;
      is_playing = true;
      has_blinked = false;
      dir = ASCENDING;
    }
    else{//has to have been button 2 that was pressed
      current_LED = NUM_LEDS-1;
      is_playing = true;
      has_blinked = false;
      dir = DESCENDING;
    }
  }
  if(is_playing){
    //blink the player that pressed the button's green light 3 times before starting
    if(!has_blinked){
      blink_indication();
      digitalWrite(light_array[current_LED], HIGH);
      has_blinked = true;
      last_light_change = millis();
    }
    //start the game. It will be trapped in here until someone misses
    while(is_playing){//we're INSIDE the game!
      p1b.update();
      p2b.update();

      //ascending light movements, check for falling edge during only when current_LED = NUM_LEDS-1
      if(dir == ASCENDING){
        if(current_LED < NUM_LEDS-1){//if it's not the last LED
          if(last_light_change + delay_time <= millis()){//check to see if enough time has passed to change the LED
            digitalWrite(light_array[current_LED], LOW);
            current_LED = current_LED + 1;
            digitalWrite(light_array[current_LED], HIGH);
            last_light_change = millis();
          }
        }
        else{
          if(p2b.read()== LOW){//on the last LED only, check every cycle to see if the player hit the button in time
            //you only need to worry about p2 when ti is ascending
            p_hit = true;
          }
          if(last_light_change + delay_time*2 <= millis()){//check to see if enough time has passed to send it back at your opponent.
            if(p_hit == true){//if the player hit it in time, send it back
              dir = DESCENDING;
              digitalWrite(light_array[current_LED], LOW);
              current_LED = current_LED - 1;
              digitalWrite(light_array[current_LED], HIGH);
              last_light_change = millis();
              delay_time = delay_time - DELAY_INCREMENT;
              p_hit = false;
            }
            else{//you lose
              if(current_LED == NUM_LEDS-1){//only lose if it's on the last LED
                is_playing = false;
              }
            }
          }//if(last_light_change...
        }//else
      }//if(dir == ASCENDING)


      //now basically the same as above, but the led moves down instead of up and blinks when current_LED == 0:
      //descending light movements, check for falling edge during only when current_LED = 0
      if(dir == DESCENDING){
        if(current_LED > 0){//if it's not the last LED
          if(last_light_change + delay_time <= millis()){//check to see if enough time has passed to change the LED
            digitalWrite(light_array[current_LED], LOW);
            current_LED = current_LED - 1;
            digitalWrite(light_array[current_LED], HIGH);
            last_light_change = millis();
          }
        }
        else{
          if(p1b.read()==LOW){//on the last LED only, check every cycle to see if the player hit the button in time
            //you only need to worry about p1 when ti is ascending
            p_hit = true;
          }
          if(last_light_change + delay_time*2 <= millis()){//check to see if enough time has passed to send it back at your opponent.
            if(p_hit == true){//if the player hit it in time, send it back
              dir = ASCENDING;
              digitalWrite(light_array[current_LED], LOW);
              current_LED = current_LED + 1;
              digitalWrite(light_array[current_LED], HIGH);
              last_light_change = millis();
              delay_time = delay_time - DELAY_INCREMENT;
              p_hit = false;
            }
            else{//you lose
              if(current_LED == 0){//only lose if it's on the last led
                is_playing = false;
              }
            }
          }//if(last_light_change...
        }//else
      }//if(dir == ASCENDING)
      
      //before finishing once is_playing is false, blink 3 times on the loser's led.
      if(!is_playing){
        for(int i=0; i<NUM_LEDS; i++){//turn all LEDs off
          digitalWrite(light_array[i], LOW);
        }
        if(dir == ASCENDING){//blink the loser
          current_LED = NUM_LEDS-1;
          blink_indication();
        }
        if(dir == DESCENDING){//blink the loser
          current_LED = 0;
          blink_indication();
        }
        //reset delay time to default
        delay_time = START_DELAY_TIME;
      }
    }//while(is_playing)
  }

}

void blink_indication(){ 
      digitalWrite(light_array[current_LED], HIGH);
      delay(START_DELAY_TIME);
      digitalWrite(light_array[current_LED], LOW);
      delay(START_DELAY_TIME);
      digitalWrite(light_array[current_LED], HIGH);
      delay(START_DELAY_TIME);
      digitalWrite(light_array[current_LED], LOW);
      delay(START_DELAY_TIME);
      digitalWrite(light_array[current_LED], HIGH);
      delay(START_DELAY_TIME);
      digitalWrite(light_array[current_LED], LOW);
      delay(START_DELAY_TIME);
      digitalWrite(light_array[current_LED], HIGH);
      delay(START_DELAY_TIME);
      digitalWrite(light_array[current_LED], LOW);
      delay(START_DELAY_TIME);
}
