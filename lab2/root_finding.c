#include <stdio.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_roots.h>

#include "quadratic_equation.h"
#include "quadratic_equation.c"
int MAX_ITERATIONS = 100;
double EPSILON = 0.0001;

void calculate_root(const gsl_root_fsolver_type * fsolver_type, gsl_function function, double x_lo, double x_hi);
void calculate(const gsl_root_fsolver_type *T);
void calculate_fdf(const gsl_root_fdfsolver_type *T);
void calculate_root_fdf(const gsl_root_fdfsolver_type * fsolver_type, double x, struct quadratic_params params);

int main (void){
  calculate(gsl_root_fsolver_bisection);
  calculate_fdf(gsl_root_fdfsolver_secant);
  /*
  2. Wyjaśnienie:
    Dla tego typu alogrytmów konieczne jest aby przedział poszukiwania pierwiastka zawierał punkt w których funkcja przechodzi przez miejsce zerowe(----0++++)/(+++0----).
    Pierwiastki wielokrotne(parzyste 2,4,6...) nie przechodzą przez 0, a tylko je dotykają. Dlatego algorytmy które bazują na zmianie znaku nie działają
    poprawnie dla parzytych wielokrotności perwiastów, natomiast będą działać dla nieparzystych wielokrotności pierwistkóœ(1,3,5..)
  */
  calculate_fdf(gsl_root_fdfsolver_steffenson);
  /*
  3.
  Do wyznaczenia miesca zerowego korzystają z pochodznych. Dla pierwsistków parzystych funkcja przyjmuje np (+++0+++), co powoduje że pochodna tej funkcji ma kształt (---0+++) i przechodzi przez zero. Analizując pochodną algorytm jest w stanie wykryć nawet parzyste miejsca zerowe.
  Porównanie czasu wykonania 400 000 powtórzeń:
  Metoda bisekcji: 0.190s
  Medoda siecznych: 0.126s
  Metoda Seffensona 0.080s
  Porownanie iteracji:

  Porównanie kryterium zakończenia obliczen:
  dla metody bisekcji kończy się poszukiwanie gdy przedział poszukiwania jest mniejszy od żądanego epsilon |a - b| < eps
  dla pozostałych metod kończt się gdy różnica dwóch poprzednie wyniki jest mniejsza od pewnego epsilon |x - x0| < eps
  */


  return 0;
}

void calculate(const gsl_root_fsolver_type *T){
  struct quadratic_params params1 = {1.0, 0.0, -5.0};
  gsl_function function;
  function.function = &quadratic;
  function.params = &params1;
  calculate_root(T, function, 0, 5);
  calculate_root(T, function, -5, 0);
  /*struct quadratic_params params2 = {1.0, -2.0, 1.0};
  function.function = &quadratic;
  function.params = &params2;
  calculate_root(T, function, -0, 2);*/
}

void calculate_root(const gsl_root_fsolver_type * fsolver_type, gsl_function function, double x_lo, double x_hi)
{
  int iter = 0;
  double r = 0;
  gsl_root_fsolver *s = gsl_root_fsolver_alloc(fsolver_type);
  gsl_root_fsolver_set(s, &function, x_lo, x_hi);
  struct quadratic_params *p
    = (struct quadratic_params *) function.params;
  printf ("Calculate root of x^2 * %f + x * %f + %f using %s method\n", p->a, p->b, p->c, gsl_root_fsolver_name (s));

  int status;
  do
    {
      iter++;
      status = gsl_root_fsolver_iterate(s);
      r = gsl_root_fsolver_root(s);
      x_lo = gsl_root_fsolver_x_lower(s);
      x_hi = gsl_root_fsolver_x_upper(s);
      status = gsl_root_test_interval(x_lo, x_hi, 0, EPSILON);

    }while (status == GSL_CONTINUE && iter < MAX_ITERATIONS);
  if (status == GSL_SUCCESS)
  {
    printf ("Root = %.7f\n", r);
  }
  else
  {
    printf("Root not found");
  }
  gsl_root_fsolver_free (s);
}

void calculate_fdf(const gsl_root_fdfsolver_type *T){
  struct quadratic_params params1 = {1.0, 0.0, -5.0};
  struct quadratic_params params2  = {1.0, -2.0, 1.0};
  calculate_root_fdf(T, 5.0, params1);
  calculate_root_fdf(T, -5.0, params1);
  calculate_root_fdf(T, 5.0, params2);
  calculate_root_fdf(T, -5.0, params2);
}

void calculate_root_fdf(const gsl_root_fdfsolver_type * fsolver_type, double x, struct quadratic_params params)
{
  int status;
    int iter = 0;
    gsl_root_fdfsolver *s = gsl_root_fdfsolver_alloc (fsolver_type);
    double x0;
    gsl_function_fdf FDF;


    FDF.f = &quadratic;
    FDF.df = &quadratic_deriv;
    FDF.fdf = &quadratic_fdf;
    FDF.params = &params;

    printf ("Calculate root of x^2 * %f + x * %f + %f using %s method\n", params.a, params.b, params.c, gsl_root_fdfsolver_name(s));
    gsl_root_fdfsolver_set (s, &FDF, x);
    do
    {
        iter++;
        status = gsl_root_fdfsolver_iterate (s);
        x0 = x;
        x = gsl_root_fdfsolver_root (s);
        status = gsl_root_test_delta (x, x0, 0, 1e-3);

    }
    while (status == GSL_CONTINUE && iter < MAX_ITERATIONS);
    if (status == GSL_SUCCESS)
    {
      printf ("Root = %10.7f\n", x);
    }
    gsl_root_fdfsolver_free (s);
}
