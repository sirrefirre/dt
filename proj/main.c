// made by Simon Westin and Yukki nakamura
#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "include.h"

#define ENEMYSHIP 'H'
#define SHIP '>'
#define EMPTY ' '
#define SHOT '-'
#define ENEMYSHOT '~'


int wall;
int moved = 0;
int timeoutcount = 0;
int enemyupdate = 0;
int enemydirection = 1; //1=right      -1=left
char string[4][16];		//play field


void enemymovement(void);
void dostuff(void);
void move(void);
void hit(void);
void shoot(void);



int main(void) {

	init(); //init board
	startscreen(); //start screen
	//wait for button 2 press (start)
	while(!(getbtns() & 0x1)){}
	startmessages(); //starting messages

	
	int i, j;
	//place enemies
	for(i = 0; i < 3; i++){
		for(j = 14; j> 8; j--){
			string[i][j] = ENEMYSHIP;
		}
	}

	//make empty spaces ' ' (space)
	for(i = 0; i < 4; i++){
		for(j = 0; j < 16; j++){
			if(string[i][j] != ENEMYSHIP) string[i][j] = EMPTY;
		}
	}
	//place player
	string[3][0] = SHIP;
	//display starting field
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

void dostuff( void )
{
	//if timer done restart timer
	if(IFS(0) & 0x100){
		IFSCLR(0) = 0x100;
		TMR2 = 0;
		if(timeoutcount < 9){
			timeoutcount++;
			//if button pressed and not already moved
			if(getbtns() && !moved){
				move();
				moved = 1;
			}
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
			//if button pressed and not already moved
			if(getbtns() && !moved){
				move();
				moved = 1;
			}
			timeoutcount = 0;
			//update enemy movement every other second
			if(enemyupdate == 1){
				enemyupdate = 0;
				enemymovement();
				display_string(0, string[0]);
				display_string(1, string[1]);
				display_string(2, string[2]);
				display_string(3, string[3]);

			}else{
				enemyupdate++;
			}
			//print on screen once a second
			display_update();
			moved = 0;
			//*portE += 1;
		}
	}
}


//player movement
void move(void){
	int i;
	//button 4 (left)
	if(getbtns() == 4){
		//if already at edge
		if(string[0][0] == SHIP){
		}else{
			//move player
			for(i = 0; i < 3; i++){
			string[i][0] = string[i+1][0];
			}
			string[3][0] = EMPTY;
		}
	//button 3 (right)	
	}else if(getbtns() == 2){
		//if already at edge
		if(string[3][0] == SHIP){
		}else{
			//move player
			for(i = 3; i > 0; i--){
			string[i][0] = string[i-1][0];
			}
			string[0][0] = EMPTY;
		}
	} 
}

void enemymovement(void){
	int wc, i, j;
	//if moving right
	if(enemydirection == 1){
		//check if next to wall
		for(wc = 14; wc > 0; wc--){
			if(string[3][wc] == ENEMYSHIP){
				wall = 1;
			}
		}
		//if at wall move down and change direction
		if(wall){
			for(i = 1; i < 15; i++){
				for(j = 0; j < 4; j++){
					string[j][i] = string[j][i+1];
				}
			}
			wall = 0;
			enemydirection = -1;
		}else{
			//move all enemies right
			for(i = 1; i < 15; i++){
				for(j = 3; j > 0; j--){
					string[j][i] = string[j-1][i];
				}
				string[0][i] = EMPTY;
			}
		}
	//if moving left
	}else{
		//check if next to wall
		for(wc = 14; wc > 0; wc--){
			if(string[0][wc] == ENEMYSHIP){
				wall = 1;
			}
		}
		//if at wall move down and change direction
		if(wall){
			for(i = 1; i < 15; i++){
				for(j = 0; j < 4; j++){
					string[j][i] = string[j][i+1];
					
				}
			}
			wall = 0;
			enemydirection = 1;
			
		}else{
			//move all enemies left
			for(i = 1; i < 15; i++){
				for(j = 0; j < 4; j++){
					string[j][i] = string[j+1][i];
				}
				string[3][i] = EMPTY;
			}
		}
	}
}

void hit(void){
	
}

void shoot(void){
	
}