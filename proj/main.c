// made by Simon Westin and Yukki nakamura
#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

volatile int* portE = (int *)0xbf886110;
volatile int* trisE = (int *)0xbf886100;
int timeoutcount = 0;
int enemyupdate = 0;
int enemydirection = 1;
char string[4][16];

int flag = 0;

void enemymovement(void);
void dostuff(void);
void move(void);

void init( void )
{
	/**************** from lab3 main *****************/
	/* Set up peripheral bus clock */
    /* OSCCONbits.PBDIV = 1; */
    OSCCONCLR = 0x100000; /* clear PBDIV bit 1 */
	OSCCONSET = 0x080000; /* set PBDIV bit 0 */
	
	/* Set up output pins */
	AD1PCFG = 0xFFFF;
	ODCE = 0x0;
	TRISECLR = 0xFF;
	PORTE = 0x0;
	
	/* Output pins for display signals */
	PORTF = 0xFFFF;
	PORTG = (1 << 9);
	ODCF = 0x0;
	ODCG = 0x0;
	TRISFCLR = 0x70;
	TRISGCLR = 0x200;
	
	/* Set up input pins */
	TRISDSET = (1 << 8);
	TRISFSET = (1 << 1);
	
	/* Set up SPI as master */
	SPI2CON = 0;
	SPI2BRG = 4;
	/* SPI2STAT bit SPIROV = 0; */
	SPI2STATCLR = 0x40;
	/* SPI2CON bit CKP = 1; */
        SPI2CONSET = 0x40;
	/* SPI2CON bit MSTEN = 1; */
	SPI2CONSET = 0x20;
	/* SPI2CON bit ON = 1; */
	SPI2CONSET = 0x8000;
	/********************************************************/
	
	
	*trisE &= ~0xff;
	*portE = 0;
	TRISD |= 0xfe0;
	T2CONSET = 0x8070;	//enable timer2 with 1:256
	PR2 = 31250;		//set counter value

	display_init();

  return;
}

int main(void) {

	init(); //init board
	display_string(0, "Welcome to");
	display_string(1, "mips invaders");
	display_string(2, "this screen is");
	display_string(3, "small :(");
	display_update();
	delay(1000);
	display_string(0, "press button");
	display_string(1, "2 to start");
	display_string(2, "");
	display_string(3, "");
	display_update();
	
	while(!(getbtns() & 0x1)){}
	
	int i, j;
	for(i = 0; i < 3; i++){
		for(j = 14; j> 8; j--){
			string[i][j] = 'H';
		}
	}
	
	//init enemy position
	int f, g;
	for(g = 0; g < 4; g++){
		for(f = 0; f < 16; f++){
			if(string[g][f] != 'H') string[g][f] = ' ';
		}
	}
	string[3][0] = '>';
	display_string(0, string[0]);
	display_string(1, string[1]);
	display_string(2, string[2]);
	display_string(3, string[3]);
	display_update();
	
	
	while( 1 )
	{
	  dostuff(); /* Do lab-specific things again and again */
	}
	return 0;
}
/* Interrupt Service Routine */
void user_isr( void )
{
  return;
}


int getsw(void){
	int ret = PORTD >> 8;
	return ret &= 0xf;
}

int getbtns(void){
	int ret = PORTD >> 5;
	return ret &= 0x7;
}

void dostuff( void )
{
	//if timer done restart timer
	if(IFS(0) & 0x100){
		IFSCLR(0) = 0x100;
		TMR2 = 0;
		if(timeoutcount < 9){
			timeoutcount++;
			  /*
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
			  }*/
		}else{
			//print on screen once a second
			timeoutcount = 0;
			if(enemyupdate == 1){
				enemyupdate = 0;
				enemymovement();
				display_string(0, string[0]);
				display_string(1, string[1]);
				display_string(2, string[2]);
				display_string(3, string[3]);
				display_update();
			}else{enemyupdate++;}


			*portE += 1;
		}
	}
}
int wall;

void move(void){
	
}

void enemymovement(void){

			int qw, we, i, j;
	if(enemydirection == 1){
		for(qw = 14; qw > 0; qw--){
			if(string[3][qw] == 'H'){
				wall = 1;
			}
		}
		if(wall){
			for(i = 1; i < 15; i++){
				for(j = 0; j < 4; j++){
					string[j][i] = string[j][i+1];
				}
			}
			wall = 0;
			enemydirection = -1;
		}else{
			for(i = 1; i < 15; i++){
				for(j = 3; j > 0; j--){
					string[j][i] = string[j-1][i];
				}
				string[0][i] = ' ';
			}
		}
		//left
	}else{
		for(qw = 14; qw > 0; qw--){
			if(string[0][qw] == 'H'){
				wall = 1;
			}
		}
		if(wall){
			for(i = 1; i < 15; i++){
				for(j = 0; j < 4; j++){
					string[j][i] = string[j][i+1];
					
				}
			}
			wall = 0;
			enemydirection = 1;
		}else{
			for(i = 1; i < 15; i++){
				for(j = 0; j < 4; j++){
					string[j][i] = string[j+1][i];
				}
				string[3][i] = ' ';
			}
		}
	}
}



