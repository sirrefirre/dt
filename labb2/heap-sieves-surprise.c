#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define COLUMNS 6
int increment;
void print_number(int n);

void print_sieves(int n){
	int i, j;
	int *A = malloc(n*sizeof(int));
	//set all elements in array to 0
	for(i=0; i<n; i++){
		A[i] = 0;
	}
	//set non-primes to 1
	for(i = 2; i<sqrt(n); i++){
		if(A[i]==0){
			for(j = 2*i; j<=n; j = j + i){
				A[j] = 1;
			}
		}
	}
	//print
	int prim1 = 0, prim2, count = 0;
	double distance = 0;
	for(i = 2; i<n; i++){
		if(A[i]==0){
			if(prim1==0){
				prim1 = i;
			}else{
				prim2 = i;
				distance += (prim2-prim1);
				count++;
				prim1 = i;
			}
		}
	}
	distance = (distance/count);
	printf("average prime distance is: %f", distance);
	printf("\n");
		free(A);
}


// 'argc' contains the number of program arguments, and
// 'argv' is an array of char pointers, where each
// char pointer points to a null-terminated string.
int main(int argc, char *argv[]){
  if(argc == 2)
    print_sieves(atoi(argv[1]));
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