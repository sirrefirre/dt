// made by Simon Westin and Yukki nakamura
#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "include.h"

#define ENEMYSHIP 'B'
#define SHIP '>'
#define EMPTY ' '
#define SHOT '-'
#define ENEMYSHOT '('

int enemies = 24;		//number of enemies left
int fired = 0;			//fire rate
int alive = 1;			//play condition
int wall = 0;			//if wall
int timeoutcount = 0;	//update interval
int enemyupdate = 0;	//enemy movement interval
int enemydirection = 1; //1=right      -1=left
char string[4][16];		//play field

//init functions
int rand(void);
void enemymovement(void);
void game(void);
void move(void);
void hit(void);
void shoot(void);
void enemyshoot(void);
void enemytravel(void);
void travel(void);
void displayprint(void);
void reset(void);



int main(void) {

	init(); //init board
	while(1){
		startscreen(); //start screen
		delay(1000);
		//wait for button 2 press (start)
		while(!(getbtns() & 0x1)){}
		startmessages(); //starting messages
		startfield(); //starting field
		while(alive && enemies) game(); /* run game */
		//when game is no longer running win or lose
		if(alive){
			victory();
			delay(1000);
		}else{
			gameover();
			delay(1000);
		}
		//wait for restart
		while(!(getbtns() & 0x1)){}
		//reset values
		reset();
	}
	return 0;
}
/* Interrupt Service Routine */
void user_isr( void )
{
  return;
}

//main game loop
void game( void ){
	//if timer done restart timer
	if(IFS(0) & 0x100){
		IFSCLR(0) = 0x100;
		TMR2 = 0;
		while(getsw()){}	//pause
		if(timeoutcount < 9){
			timeoutcount++;
				travel();			//move player shot(s)
				if(timeoutcount%2) enemytravel();				//enemy shot(s) move 5 times a second
				if((getbtns() & 4) || (getbtns() & 2)) move();	//if move button pressed, move
				displayprint();

			//if button pressed and not already moved		
			if((getbtns() & 1) && !fired) fired = 1;
		}else{
			//if button pressed and not already moved
			if((getbtns() & 1) && !fired) fired = 1;
			timeoutcount = 0;
			travel();			//move player shot(s)
			enemytravel();		//move enemy shot(s)
			//update enemy movement every other second
			if(enemyupdate == 1){
				enemyupdate = 0;
				enemymovement();	//move enemies
			}else{
				enemyupdate++;
			}

			enemyshoot();			//move enemy shot
			if(fired) shoot();		//if we want to fire this second, fire
			if((getbtns() & 4) || (getbtns() & 2)) move();	//if move button pressed, move
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

//enemy movement
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
					//ignore player_shot
					if((string[j][i] == SHOT) || (string[j][i+1] == SHOT)){
					//ignore enemy_shot
					}else if((string[j][i] == ENEMYSHOT) || (string[j][i+1] == ENEMYSHOT)){	
					//if enemy_ship moves into player_shot remove both
					}else if((string[j][i] == ENEMYSHIP) && (string[j][i-1] == SHOT)){
						string[j][i] = EMPTY;
						string[j][i-1] = EMPTY;
						enemies--;
					//move enemy_ship
					} else{
						string[j][i] = string[j][i+1];
						//if enemy_ship on last row, dead
						if((string[j][i] == ENEMYSHIP) && (i == 1)){
							alive = 0;
							return;
						}
					}
				}
			}
			wall = 0;
			enemydirection = -1;
		}else{
			//move all enemies right
			for(i = 1; i < 15; i++){
				for(j = 3; j > 0; j--){
					//if move enemy_ship into player_shot, remove combined tile
					if((string[j][i] == SHOT) && (string[j-1][i] == ENEMYSHIP)){
						string[j][i] = EMPTY;
						enemies--;
					//ignore player_shot
					}else if((string[j][i] == SHOT) ||(string[j-1][i] == SHOT)){
					//ignore enemy_shot
					}else if((string[j][i] == ENEMYSHOT) ||(string[j-1][i] == ENEMYSHOT)){
					//move enemy_ship
					}else{
						string[j][i] = string[j-1][i];
					}
				}
				//remove old ship placement
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
					//ignore player_shot
					if((string[j][i] == SHOT) || (string[j][i+1] == SHOT)){
					//ignore enemy_shot
					}else if((string[j][i] == ENEMYSHOT) || (string[j][i+1] == ENEMYSHOT)){	
					//if enemy_ship moves into player_shot remove both
					}else if((string[j][i] == ENEMYSHIP) && (string[j][i-1] == SHOT)){
						string[j][i] = EMPTY;
						string[j][i-1] = EMPTY;
						enemies--;
					//move enemy_ship
					} else{
						string[j][i] = string[j][i+1];
						//if enemy_ship on last row, dead
						if((string[j][i] == ENEMYSHIP) && (i == 1)){
							alive = 0;
							return;
						}
					}
				}
			}
			wall = 0;
			enemydirection = 1;
			
		}else{
			//move all enemies left
			for(i = 1; i < 15; i++){
				for(j = 0; j < 3; j++){
					//if move enemy_ship into player_shot, remove combined tile
					if((string[j][i] == SHOT) && (string[j+1][i] == ENEMYSHIP)){
						string[j][i] = EMPTY;
						enemies--;
					//ignore player_shot
					}else if((string[j][i] == SHOT) || (string[j+1][i] == SHOT)){
					//ignore enemy_shot
					}else if((string[j][i] == ENEMYSHOT) || (string[j+1][i] == ENEMYSHOT)){
					//move enemy_ship
					}else{
						string[j][i] = string[j+1][i];
					}
				}
				//remove old ship placement
				if(string[3][i] == ENEMYSHIP) string[3][i] = EMPTY;
			}
		}
	}
}
//enemy shoot
void enemyshoot(void){
	int i, j;
	for(j = 0; j < 4; j++){
		for(i = 1; i < 14; i++){
			if((string[j][i] == ENEMYSHIP) && (string[j][i-1] != SHOT)){
				//random chance 
				if(rand()%10>8){
					string[j][i-1] = ENEMYSHOT;
					return;
				}
				break;
			}
		}
	}
}
//player shoot
void shoot(void){
	int ppos;
	for(ppos = 0; ppos< 4; ppos++) if(string[ppos][0] == SHIP) break;
	string[ppos][1] = SHOT;
}

//player hit(death) detection
void hit(void){
	int ppos;
	for(ppos = 0; ppos < 4; ppos++) if(string[ppos][0] == SHIP) break;
	if(string[ppos][1] == ENEMYSHOT) alive = 0;
	//hit game over
}

//enemy_shot travel
void enemytravel(void){
	int i, pos;
	for(pos = 0; pos < 4; pos++){
		if(string[pos][1] == ENEMYSHOT) hit();	//check if enemy_shot would hit player
		for(i = 1; i < 13; i++){
			//ignore player_shots
			if((string[pos][i] == SHOT) || (string[pos][i+1] == SHOT)){
			//if reached enemy_ship stop
			}else if(string[pos][i] == ENEMYSHIP){
				break;
			//if next tile is enemy_ship, remove old enemy_shot
			}else if(string[pos][i+1] == ENEMYSHIP){
				if(string[pos][i] == ENEMYSHOT) string[pos][i] = EMPTY;
				break;
			//if enemy_shot and player_shot meet remove both
			} else if((string[pos][i] == ENEMYSHOT) && (string[pos][i-1] == SHOT)){
				string[pos][i] = EMPTY;
				string[pos][i-1] = EMPTY;
			//move enemy_shot
			} else {
				string[pos][i] = string[pos][i+1];
			}
		}
	}
}
//player shot travel
void travel(void){
	int i, pos;
	for(pos = 0; pos < 4; pos++){
		for(i = 15; i > 1; i--){
			//if player_shot is behind enemy_ship ignore enemy_ship
			if((string[pos][i] == SHOT) && (string[pos][i-1] == ENEMYSHIP)){
				string[pos][i] = EMPTY;
			//ignore enemy_ships
			}else if((string[pos][i] == ENEMYSHIP) || (string[pos][i-1] == ENEMYSHIP)){
			//if enemy_shot and player_shot meet remove both
			}else if((string[pos][i] == ENEMYSHOT) && (string[pos][i-1] == SHOT)){
				string[pos][i] = EMPTY;
				string[pos][i-1] = EMPTY;
			//ignore enemy_shot
			}else if((string[pos][i] == ENEMYSHOT) || (string[pos][i-1] == ENEMYSHOT)){
			//if player_shot will hit enemy_ship, remove enemy_ship and player_shot
			}else if((string[pos][i] == SHOT) && (string[pos][i+1] == ENEMYSHIP)){
				string[pos][i] = EMPTY;
				string[pos][i+1] = EMPTY;
				enemies--;
			//move player_shot
			} else{
				string[pos][i] = string[pos][i-1];	
			}
		}
		//remove old player_shot
		if(string[pos][1] == SHOT) string[pos][1] = EMPTY;
	}
}


//print game field
void displayprint(void){
	display_string(0, string[0]);
	display_string(1, string[1]);
	display_string(2, string[2]);
	display_string(3, string[3]);
	display_update();
}
//reset values
void reset(void){
		int i, j;
		for(i = 0; i < 4; i++){
			for(j = 0; j < 16; j++) string[i][j] = EMPTY;
		}
		enemies = 24;
		fired = 0;
		alive = 1;
		wall = 0;
		timeoutcount = 0;
		enemyupdate = 0;
		enemydirection = 1;
}
	
unsigned long int next = 42;
/* rand: return pseudo-random integer on 0..32767 */
int rand(void){
next = next * 1103515245 + 12345;
return (unsigned int)(next/65536) % 32768;
}
