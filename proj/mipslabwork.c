/* mipslabwork.c

   This file written 2015 by F Lundevall

   This file should be changed by YOU! So add something here:

   This file modified 2015-12-24 by Ture Teknolog 

   Latest update 2015-08-28 by F Lundevall

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */


int mytime = 0x5957;
int timeoutcount = 0;


char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
void user_isr( void )
{
  return;
}

/* Lab-specific initialization goes here */
volatile int* portE = (int *)0xbf886110;
volatile int* trisE = (int *)0xbf886100;

void labinit( void )
{
	*trisE &= ~0xff;
	*portE = 0;
	TRISD |= 0xfe0;
	T2CONSET = 0x8070;	//enable timer2 with 1:256
	PR2 = 31250;		//set counter value

  return;
}

/* This function is called repetitively from the main program */
void labwork( void )
{
	//if timer done restart timer
	if(IFS(0) & 0x100){
		IFSCLR(0) = 0x100;
		TMR2 = 0;
	if(timeoutcount < 9){
		timeoutcount++;
		  int btns = getbtns();
		  if(btns){
			int sw = getsw();
			//BTN4
			if(btns & 0x4){
				mytime &= 0x0fff;; // remove old digit
				mytime |= (sw<<12); // set new digit
			}
			//BTN3
			if(btns & 0x2){
				mytime &= 0xf0ff;
				mytime |= (sw<<8);
			}
			//BTN2
			if(btns & 0x1){
				mytime &= 0xff0f;
				mytime |= (sw<<4);
			}
		  }
	}else{
		//print on screen once a second
		timeoutcount = 0;
		time2string( textstring, mytime );
		display_string( 3, textstring );
		display_update();
		tick( &mytime );
		display_image(96, icon);
		*portE += 1;
	}
}
}
