#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int N = 256;
double Pi = 3.14;

double* generate256Arr_1();
double* generate256Arr_2();

int main (void){
//1
	generate256Arr_1();
//2
//todo
//3
	generate256Arr_2();

  return 0;
}

double* generate256Arr_1(){
	FILE *f = fopen("fft_1_file.txt", "w");
	double* arr = calloc(N, sizeof(double));
	for(int i = 0; i < N; i++){
		arr[i]=cos(4*Pi*i/N)+cos(16*Pi*i/N)/5+cos(32*Pi*i/N)/8+cos(128*Pi*i/N)/16;	
		fprintf(f, "%lf\n", arr[i]);
	}
	fclose(f);
}


double* generate256Arr_2(){
	FILE *f = fopen("fft_2_file.txt", "w");
	double* arr = calloc(N, sizeof(double));
	for(int i = 0; i < N; i++){
		arr[i]=cos(4*Pi*i/N)+((float)rand())/RAND_MAX/8.0;
		fprintf(f, "%lf\n", arr[i]);
	}
	fclose(f);
}
