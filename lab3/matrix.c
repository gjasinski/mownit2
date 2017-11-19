#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <gsl/gsl_linalg.h>
#include <sys/resource.h>
#include <string.h>

int czas(struct rusage *ru0, struct rusage *ru1, FILE* f);
void print_info(double* matrix, double* vector, int size, gsl_vector *x);
void matrix_solver(int size);
void verifyResult(double * a, double* x, int size);

FILE *f_d;
FILE *f_s;

int main (void){
  f_d = fopen("decomposition.txt", "w");
  f_s = fopen("solve.txt", "w");
  srand(time(NULL));
  for(int i = 100; i <= 1000; i+=100){
    matrix_solver(i);
  }
  fclose(f_d);
  fclose(f_s);
  return 0;
}

double* generate_arr(int size){
  double* result_arr = calloc(size, sizeof(double));
  for(int i = 0; i < size; i++){
    result_arr[i] = (double) (rand()%10);
  }
  return result_arr;
}

void print_arr(double* arr, int size){
  for(int i = 0; i < size; i++){
    printf("%lf ", arr[i]);
  }
  printf("\n");
}

void matrix_solver(int size){
  struct rusage t_decomposition_s, t_decomposition_e, t_solve_s, t_solve_e;
  double* matrix = generate_arr(size * size);
  double* vector = generate_arr(size);
  double* matrix_copy = calloc(size * size, sizeof(double));
  memcpy(matrix_copy, matrix, size * size * sizeof(double));
  gsl_matrix_view m = gsl_matrix_view_array(matrix, size, size);
  gsl_vector_view b = gsl_vector_view_array(vector, size);

  gsl_vector *x = gsl_vector_alloc(size);
  int s;
  gsl_permutation *p = gsl_permutation_alloc(size);
  getrusage(RUSAGE_SELF, &t_decomposition_s);
  gsl_linalg_LU_decomp(&m.matrix, p, &s);
  getrusage(RUSAGE_SELF, &t_decomposition_e);
  getrusage(RUSAGE_SELF, &t_solve_s);
  gsl_linalg_LU_solve(&m.matrix, p, &b.vector, x);
  getrusage(RUSAGE_SELF, &t_solve_e);

  czas(&t_decomposition_s, &t_decomposition_e, f_d);
  czas(&t_solve_s, &t_solve_e, f_s);

  print_info(matrix_copy, vector, size, x);
  verifyResult(matrix_copy, x->data, size);

  gsl_permutation_free (p);
  gsl_vector_free (x);

}

int czas(struct rusage *ru0, struct rusage *ru1, FILE* f){
	long utime = 0, stime = 0, ttime = 0;
  utime = (ru1->ru_utime.tv_sec - ru0->ru_utime.tv_sec) * 1.e6 + ru1->ru_utime.tv_usec - ru0->ru_utime.tv_usec;
  stime = (ru1->ru_stime.tv_sec - ru0->ru_stime.tv_sec) * 1.e6 + ru1->ru_stime.tv_usec - ru0->ru_stime.tv_usec;
	ttime = stime + utime;

	fprintf(f, "%ld\n", ttime);

}

void verifyResult(double * a, double* x, int size){
   gsl_matrix_view A = gsl_matrix_view_array(a, size, size);
   gsl_matrix_view B = gsl_matrix_view_array(x, size, 1);
   double* c = calloc(size, sizeof(double));
   gsl_matrix_view C = gsl_matrix_view_array(c, size, 1);
   gsl_blas_dgemm (CblasNoTrans, CblasNoTrans,
                   1.0, &A.matrix, &B.matrix,
                   0.0, &C.matrix);
  printf("verifyResult should be equal B:\n");
   for(int i = 0; i < size; i++){
     printf("%g\n", c[i]);
   }
}

void print_info(double* matrix, double* vector, int size, gsl_vector *x){
  printf("A=\n");
  for(int i = 0; i < size; i++){
    for(int j = 0; j < size; j++){
      printf("%lf, ",matrix[i * size + j]);
    }
    printf("\n");
  }
  printf ("x:\n");
  gsl_vector_fprintf (stdout, x, "%g");
  printf("B:\n");
  for(int i = 0; i < size; i++){
    printf("%lf \n", vector[i]);
  }
}
