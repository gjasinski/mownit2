#include "stdio.h"

double funDouble1(double n){
	double x1 = 3 * n;
	double x2 = 1 - n;
	return n + x1 * x2;
}

float funFloat1(float n){
	float x1 = 3 * n;
	float x2 = 1 - n;
	return n + x1 * x2;
}

double funDouble2(double n){
	double x1 = 4 * n;
	double x2 = 3 * n * n;
	return x1 - x2;
}

float funFloat2(float n){
	float x1 = 4 * n;
	float x2 = 3 * n * n;
	return x1 - x2;

}

void calcEps(){
	double d = 1;
	double p = 1;
	int i = 0;
	while(d + p > 1){
		p = p / 2;	
	i++;
	}
	printf("p:= %lf\n, i:= %d", p, i);
}


int main() {
	double n1, n2;
	float n3, n4;
	n1 = n2 = 0.1;
	n3 = n4 = 0.1;
	for(int i = 0; i < 100; i++){
		n1 = funDouble1(n1);
		n2 = funDouble2(n2);
		n3 = funFloat1(n3);
		n4 = funFloat2(n4);
		printf("VALS %lf %lf %f %f\n", n1, n2, n3, n4);	
	}
	calcEps();
	return 0;
}

