#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h" 

int getsw(void){
	int ret = PORTD >> 8;
	return ret &= 0xf;
}

int getbtns(void){
	int ret = PORTD >> 5;
	return ret &= 0x7;
}