// made by Simon Westin and Yukki nakamura
#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "include.h"

#define ENEMYSHIP 'H'
#define SHIP '>'
#define EMPTY ' '
#define SHOT '-'
#define ENEMYSHOT '~'
int firerate = 0;
int holdleft = 0, holdright = 0;
int fired = 0;
int alive = 1;
int wall = 0;
int timeoutcount = 0;
int enemyupdate = 0;
int enemydirection = 1; //1=right      -1=left
char string[4][16];		//play field


void enemymovement(void);
void dostuff(void);
void move(void);
void hit(int);
void shoot(void);
void enemyshoot(void);
int enemytravel(int);
void travel(void);
void displayprint(void);



int main(void) {

	init(); //init board
	while(1){
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
		displayprint();
		
		while(alive)
		{
		  dostuff(); /* Do lab-specific things again and again */
		}
		gameover();
		//wait for restart
		while(!(getbtns() & 0x1)){}
		//reset values
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

void dostuff( void )
{
	//if timer done restart timer
	if(IFS(0) & 0x100){
		IFSCLR(0) = 0x100;
		TMR2 = 0;
		if(timeoutcount < 9){
			timeoutcount++;
				travel();
				if((getbtns() & 4) || (getbtns() & 2)) move();
				displayprint();

			//if button pressed and not already moved
						
			if((getbtns() & 1) && !fired) fired = 1;
		}else{
			//if button pressed and not already moved
			
			if((getbtns() & 1) && !fired) fired = 1;
			timeoutcount = 0;
			//update enemy movement every other second
			if(enemyupdate == 1){
				enemyupdate = 0;
				enemymovement();
			}else{
				enemyupdate++;
			}
			travel();
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
		holdright = 1;
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
					string[j][i] = string[j][i+1];
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
					}else if(string[j][i] == SHOT){
						continue;
					}else{
						string[j][i] = string[j-1][i];
					}
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
					if((string[j][i] == SHOT) || (string[j][i+1] == SHOT)) continue;
					string[j][i] = string[j][i+1];
					
				}
			}
			wall = 0;
			enemydirection = 1;
			
		}else{
			//move all enemies left
			for(i = 1; i < 15; i++){
				for(j = 0; j < 4; j++){
					
					if(string[j+1][i] == SHOT){
						string[j][i] = EMPTY;
					} else if((string[j][i] == SHOT) && (string[j+1][i] == ENEMYSHIP)){
						string[j][i] = EMPTY;
					}else if(string[j][i] == SHOT){
						continue;
					}else{
						string[j][i] = string[j+1][i];
					}
				}
				string[3][i] = EMPTY;
			}
		}
	}
}
void enemyshoot(void){
	
}

//player hit, death detection
void hit(int spos){
	int ppos;
	for(ppos = 0; ppos< 4; ppos++) if(string[0][ppos] == SHIP){
		break;
	}
	if(ppos == spos) alive = 0;//hit game over
}

//enemy shot travel
int enemytravel(int pos){
	int i;
	for(i = 1; i < 13; i++){
	if(string[pos][i+1] == ENEMYSHIP ){
		string[pos][i] = EMPTY;
	break;
	}
		string[pos][i] = string[pos][i+1];
	}
	return pos;
}
//player shot travel
void travel(void){
	int i, pos;
	for(pos = 0; pos < 4; pos++){
		for(i = 15; i > 1; i--){
			if((string[pos][i] == ENEMYSHIP) || (string[pos][i-1] == ENEMYSHIP)) continue;
			
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
		string[pos][1] = EMPTY;
	}
}

void shoot(void){
	int ppos;
	for(ppos = 0; ppos< 4; ppos++) if(string[ppos][0] == SHIP){
		break;
	}
	string[ppos][1] = SHOT;
}

void displayprint(void){
	display_string(0, string[0]);
	display_string(1, string[1]);
	display_string(2, string[2]);
	display_string(3, string[3]);
	display_update();
}


