//One-Dimensional Pong Game for the Elefu MMC84
//Coded by: Tim Anderson
//Date: 2013-06-17

/*
This is the code to run a one-dimensional pong game off the Elefu MMC84.
The MMC84 is now available from the folks at http://www.elefu.com and
is on kickstarter at:
http://www.kickstarter.com/projects/104819283/elefu-base-a-modular-arduino-compatible-system

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
