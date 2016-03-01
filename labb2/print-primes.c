/*
 print-prime.c
 By David Broman.
 Last modified: 2015-09-15
 This file is in the public domain.
*/


#include <stdio.h>
#include <stdlib.h>

#define COLUMNS 6
int increment = 0;
void print_number(int n);

void print_primes(int n){
  int i;
	for(i = 2; i< n; i++){
		if(is_prime(i) == 1){
			print_number(i);
		}
	}
	printf("\n");
}

// 'argc' contains the number of program arguments, and
// 'argv' is an array of char pointers, where each
// char pointer points to a null-terminated string.
int main(int argc, char *argv[]){
  if(argc == 2)
    print_primes(atoi(argv[1]));
  else
    printf("Please state an interger number.\n");
  return 0;
}

void print_number(int n){
	if(increment < COLUMNS){
		printf("%10d ", n);
		increment++;
	}else{
		printf("\n");
		printf("%10d ", n);
		increment = 1;
	}
}

int is_prime(int n){
	int i;
	for(i = 2; i <= n/2; i++){
		if (n%i==0)
			return 0;
	}
	return 1;
}