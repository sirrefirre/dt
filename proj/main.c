// made by Simon Westin and Yukki nakamura
#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "include.h"

#define ENEMYSHIP 'H'
#define SHIP '>'
#define EMPTY ' '
#define SHOT '-'
#define ENEMYSHOT '~'
int fired = 0;
int alive = 1;
int wall = 0;
int timeoutcount = 0;
int enemyupdate = 0;
int enemydirection = 1; //1=right      -1=left
char string[4][16];		//play field

int rand(void);
void enemymovement(void);
void dostuff(void);
void move(void);
void hit(void);
void shoot(void);
void enemyshoot(void);
void enemytravel(void);
void travel(void);
void displayprint(void);



int main(void) {

	init(); //init board
	while(1){
		startscreen(); //start screen
		delay(1000);
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
		displayprint();
		
		while(alive)
		{
		  dostuff(); /* run game */
		}
		gameover();
		//wait for restart
		while(!(getbtns() & 0x1)){}
		//reset values
		fired = 0;
		alive = 1;
		wall = 0;
		timeoutcount = 0;
		enemyupdate = 0;
		enemydirection = 1;
	}
	return 0;
}
/* Interrupt Service Routine */
void user_isr( void )
{
  return;
}

void dostuff( void ){
	//if timer done restart timer
	if(IFS(0) & 0x100){
		IFSCLR(0) = 0x100;
		TMR2 = 0;
		if(timeoutcount < 9){
			timeoutcount++;
				if(timeoutcount % 2) travel();
				if((getbtns() & 4) || (getbtns() & 2)) move();
				displayprint();

			//if button pressed and not already moved
						
			if((getbtns() & 1) && !fired) fired = 1;
		}else{
			//if button pressed and not already moved
			
			if((getbtns() & 1) && !fired) fired = 1;
			timeoutcount = 0;
			//update enemy movement every other second
			travel();
			enemytravel();
			if(enemyupdate == 1){
				enemyupdate = 0;
				enemymovement();
			}else{
				enemyupdate++;
			}

			enemyshoot();
			if(fired) shoot();
			if((getbtns() & 4) || (getbtns() & 2)) move();
			//print on screen once a second
			displayprint();
			fired = 0;
		}
	}
}


//player movement
void move(void){
	int i;
	//button 4 (left)
	
	if(getbtns() & 4){
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
	}else if(getbtns() & 2){
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
					if((string[j][i] == SHOT) || (string[j][i+1] == SHOT)) continue;
					if((string[j][i] == ENEMYSHOT) || (string[j][i+1] == ENEMYSHOT)) continue;	
					string[j][i] = string[j][i+1];
					if((string[j][i] == ENEMYSHIP) && (i == 1)){
						alive = 0;
						return;
					}
				}
			}
			wall = 0;
			enemydirection = -1;
		}else{
			//move all enemies right
			for(i = 1; i < 15; i++){
				for(j = 3; j > 0; j--){
					
					if(string[j-1][i] == SHOT){
						string[j][i] = EMPTY;
					} else if((string[j][i] == SHOT) && (string[j-1][i] == ENEMYSHIP)){
						string[j][i] = EMPTY;
					}else if((string[j][i] == SHOT) ||(string[j-1][i] == SHOT)){
						continue;
					}else if((string[j][i] == ENEMYSHOT) ||(string[j][i] == ENEMYSHOT)){
						continue;
					}else{
						string[j][i] = string[j-1][i];
					}
				}
				if(string[0][i] == ENEMYSHIP) string[0][i] = EMPTY;
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
					if((string[j][i] == SHOT) || (string[j][i+1] == SHOT)) continue;
					if((string[j][i] == ENEMYSHOT) || (string[j][i+1] == ENEMYSHOT)) continue;	
					string[j][i] = string[j][i+1];
					if((string[j][i] == ENEMYSHIP) && (i == 1)) alive = 0;

				}
			}
			wall = 0;
			enemydirection = 1;
			
		}else{
			//move all enemies left
			for(i = 1; i < 15; i++){
				for(j = 0; j < 3; j++){
					
					if(string[j+1][i] == SHOT){
						string[j][i] = EMPTY;
					} else if((string[j][i] == SHOT) && (string[j+1][i] == ENEMYSHIP)){
						string[j][i] = EMPTY;
					}else if((string[j][i] == SHOT) || (string[j+1][i] == SHOT)){
						continue;
					}else if((string[j][i] == ENEMYSHOT) || (string[j+1][i] == ENEMYSHOT)){
						continue;
					}else{
						string[j][i] = string[j+1][i];
					}
				}
				if(string[3][i] == ENEMYSHIP) string[3][i] = EMPTY;
			}
		}
	}
}
void enemyshoot(void){
	int i, j;
	for(j = 0; j < 4; j++){
		for(i = 1; i < 14; i++){
			if(string[j][i] == ENEMYSHIP){
				if(rand()%10>8){
					string[j][i-1] = ENEMYSHOT;
					return;
				}
				break;
			}
		}
	}
}

void shoot(void){
	int ppos;
	for(ppos = 0; ppos< 4; ppos++) if(string[ppos][0] == SHIP) break;
	string[ppos][1] = SHOT;
}

//player hit, death detection
void hit(void){
	int ppos;
	for(ppos = 0; ppos < 4; ppos++) if(string[ppos][0] == SHIP) break;
	if(string[ppos][1] == ENEMYSHOT) alive = 0;
	//hit game over
}

//enemy shot travel
void enemytravel(void){
	int i, pos;
	for(pos = 0; pos < 4; pos++){
		if(string[pos][1] == ENEMYSHOT) hit();
		for(i = 1; i < 13; i++){
		if(string[pos][i+1] == ENEMYSHIP){
			string[pos][i] = EMPTY;
		break;
		}
			string[pos][i] = string[pos][i+1];
		}
	}
}
//player shot travel
void travel(void){
	int i, pos;
	for(pos = 0; pos < 4; pos++){
		for(i = 15; i > 1; i--){
			if((string[pos][i] == ENEMYSHIP) || (string[pos][i-1] == ENEMYSHIP)) continue;
			if((string[pos][i] == ENEMYSHOT) || (string[pos][i-1] == ENEMYSHOT)) continue;
			if((string[pos][i] == SHOT) && (string[pos][i+1] == ENEMYSHIP)){
				string[pos][i] = EMPTY;
				string[pos][i+1] = EMPTY;
			}
			if((string[pos][i-1] == ENEMYSHOT) && (string[pos][i-2] == SHOT)){
				string[pos][i-1] = EMPTY;
				string[pos][i-2] = EMPTY;
				break;
			}	
			string[pos][i] = string[pos][i-1];	
		}
		if(string[pos][1] == SHOT) string[pos][1] = EMPTY;
	}
}



void displayprint(void){
	display_string(0, string[0]);
	display_string(1, string[1]);
	display_string(2, string[2]);
	display_string(3, string[3]);
	display_update();
}

unsigned long int next = 42;
/* rand: return pseudo-random integer on 0..32767 */
int rand(void){
next = next * 1103515245 + 12345;
return (unsigned int)(next/65536) % 32768;
}
