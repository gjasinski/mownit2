#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static int MONTE_CARLO_ITERATIONS = 100000;

double generateRandomDoubleFromZeroTo(double maxValue);
double findMaxValue(int start, int end, double (*function)(double));
double hitAndMiss(double start, double end, double (*function)(double));
double xToPowerOfTWo(double x){
  return x * x;
}

double oneDivSqrtOfX(double x){
  return 1.0 / sqrt(x);
}

int main (void){
  srand(time(NULL));
  FILE *f1 = fopen("xToPowerOfTWo", "w");
  FILE *f2 = fopen("oneDivSqrtOfX", "w");
  double sumXToPowerOfTwo = 0.0;
  double sumOneDivSqrtOfX = 0.0;
  double step;
	for(int subRanges = 1; subRanges < 10; subRanges++){
      step = 1.0 / (double)subRanges;
      double currX = 0.0;
      printf("step %lf\n", step);
      for(int j = 0; j < subRanges; j++){
        sumOneDivSqrtOfX += hitAndMiss(currX, currX + step, oneDivSqrtOfX);
        sumXToPowerOfTwo += hitAndMiss(currX, currX + step, xToPowerOfTWo);
        currX += step;
      }
    sumOneDivSqrtOfX += hitAndMiss(currX, 1.0, oneDivSqrtOfX);
    sumXToPowerOfTwo += hitAndMiss(currX, 1.0, xToPowerOfTWo);
    printf("%d %lf\n", subRanges, sumOneDivSqrtOfX);
    printf("%d %lf\n", subRanges, sumXToPowerOfTwo);
    sumXToPowerOfTwo = 0.0;
    sumXToPowerOfTwo = 0.0;
  }

  return 0;
}


double hitAndMiss(double start, double end, double (*function)(double)){
  double maxValue = findMaxValue(start, end, function);
  //printf("maxValue %lf \n", maxValue);
  double minValue = 0.0;
  int hit = 0;
  int miss = 0;
  for(int i = 0; i < MONTE_CARLO_ITERATIONS; i++){
    double x = generateRandomDoubleFromZeroTo(end - start) + start;
    double y = generateRandomDoubleFromZeroTo(maxValue);
    double functionValueInX = function(x);
    if(y > functionValueInX){
      miss++;
    }
    else{
      hit++;
    }
  }
  return (end - start) * maxValue * hit / (hit + miss);
}

double findMaxValue(int start, int end, double (*function)(double)){
  double maxIntValue = 2147483647.0;
  double maxValue = 0.0;
  double currX = start;
  double step = 0.01;
  while(currX < end){
    double currValue = function(currX);
    if(currValue > maxValue && currValue < maxIntValue){
      maxValue = currValue;
    }
    currX += step;
  }
  return maxValue;
}

double generateRandomDoubleFromZeroTo(double maxValue){
    return (double)rand() / ((double)RAND_MAX / maxValue) ;
}
