#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_fft_real.h>
#include <gsl/gsl_fft_halfcomplex.h>

int N = 256;
double Pi = 3.1415926535;

double* generate256Arr_1();
double* generate256Arr_2();
void makeFFT(double* data);
double* clearFFT(double* data);
void reverseFFT(double* data);
void printArrToFile(double* arr, char* fileName);
void printFFTArrToFile(double* arr, char* fileName);

int main (void){
//1
	double* generatedFirstArr = generate256Arr_1();
	printArrToFile(generatedFirstArr, "measurements/signal_file.txt");
//2
	makeFFT(generatedFirstArr);
	printFFTArrToFile(generatedFirstArr, "measurements/fft_signal_file.txt");
//3
	double* generatedSecArr = generate256Arr_2();
	printArrToFile(generatedSecArr, "measurements/noise_file.txt");
//4
	makeFFT(generatedSecArr);
	printFFTArrToFile(generatedSecArr, "measurements/fft_noise_file.txt");
//5
	double* clearedFftArr = clearFFT(generatedSecArr);
	printFFTArrToFile(clearedFftArr, "measurements/fft_cleared_noise_file.txt");
//6
	reverseFFT(clearedFftArr);
	printArrToFile(clearedFftArr, "measurements/reversed_fft_cleared_noise_file.txt");
  return 0;
}

double* generate256Arr_1(){
	double* arr = calloc(N, sizeof(double));
	for(int i = 0; i < N; i++){
		arr[i]=cos(4*Pi*i/N)+cos(16*Pi*i/N)/5+cos(32*Pi*i/N)/8+cos(128*Pi*i/N)/64;
	}
	return arr;
}

double* generate256Arr_2(){
	double* arr = calloc(N, sizeof(double));
	for(int i = 0; i < N; i++){
		arr[i]=cos(4*Pi*i/N)+((float)rand())/RAND_MAX/8.0;
	}
	return arr;
}

void makeFFT(double* data){
	gsl_fft_real_radix2_transform(data, 1, N);
}

double* clearFFT(double* data){
	for(int i = 0; i < N; i++){
		if(abs(data[i] < 50)){
			data[i] = 0;
		}
	}
	return data;
}

void reverseFFT(double* data){
	gsl_fft_halfcomplex_radix2_inverse(data, 1, N);
}

void printArrToFile(double* arr, char* fileName){
	FILE *f = fopen(fileName, "w");
	for(int i = 0; i < N; i++){
		fprintf(f, "%lf\n",  arr[i]);
	}
	fclose(f);
}

void printFFTArrToFile(double* arr, char* fileName){
	FILE *f = fopen(fileName, "w");
	for(int i = 0; i < N; i++){
		fprintf(f, "%d %lf\n", i*4,  arr[i]);
	}
	fclose(f);
}
