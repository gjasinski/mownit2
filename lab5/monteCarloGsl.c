#include <stdlib.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_monte.h>
#include <gsl/gsl_monte_miser.h>
#include <gsl/gsl_monte_plain.h>
#include <gsl/gsl_monte_vegas.h>

double exactXToPowerOfTWo = 1.0 / 3.0;
double exactOneDivSqrtOfX = 2.0;
double xl[3] = {0};
double xu[3] = {1};

double xToPowerOfTWo(double *k, size_t dim, void *params) {
  return k[0] * k[0];
}

double oneDivSqrtOfX(double *k, size_t dim, void *params) {
  return 1.0 / sqrt(k[0]);
}

void calculateMonteCarloMultipleTiemesAndWriteErrorToFile(
    gsl_monte_function G, FILE *plain, FILE *miser, FILE *vegas, double exac) {
  double res, err, error;
  const gsl_rng_type *T;
  gsl_rng *r;
  gsl_rng_env_setup();
  for (size_t calls = 100; calls < 1000000; calls += 100) {
    printf("%ld %lf\n", calls, exac);
    T = gsl_rng_default;
    r = gsl_rng_alloc(T);
    {
      gsl_monte_plain_state *s = gsl_monte_plain_alloc(1);
      gsl_monte_plain_integrate(&G, xl, xu, 1, calls, r, s, &res, &err);
      gsl_monte_plain_free(s);

      error = fabs(res - exac);
      fprintf(plain, "%ld %lf\n", calls, error);
    }
    {
      gsl_monte_miser_state *s = gsl_monte_miser_alloc(1);
      gsl_monte_miser_integrate(&G, xl, xu, 1, calls, r, s, &res, &err);
      gsl_monte_miser_free(s);

      error = fabs(res - exac);
      fprintf(miser, "%ld %lf\n", calls, error);
    }
    {
      gsl_monte_vegas_state *s = gsl_monte_vegas_alloc(1);
      gsl_monte_vegas_integrate(&G, xl, xu, 1, calls, r, s, &res, &err);
      gsl_monte_vegas_free(s);
      error = fabs(res - exac);
      fprintf(vegas, "%ld %lf\n", calls, error);
    }
  }
}

int main(void) {
  FILE *plainXToPowerTwo = fopen("measurements/plainXToPowerTwo", "w");
  FILE *miserXToPowerTwo = fopen("measurements/miserXToPowerTwo", "w");
  FILE *vegasXToPowerTwo = fopen("measurements/veagasXToPowerTwo", "w");
  FILE *plainOneDivSqrtOfX = fopen("measurements/plainOneDivSqrtOfX", "w");
  FILE *miserOneDivSqrtOfX = fopen("measurements/miserOneDivSqrtOfX", "w");
  FILE *vegasOneDivSqrtOfX = fopen("measurements/veagasOneDivSqrtOfX", "w");

  gsl_monte_function G1 = {&xToPowerOfTWo, 1, 0};
  calculateMonteCarloMultipleTiemesAndWriteErrorToFile(G1, plainXToPowerTwo, miserXToPowerTwo, vegasXToPowerTwo, exactXToPowerOfTWo);
  gsl_monte_function G2 = {&oneDivSqrtOfX, 1, 0};
  calculateMonteCarloMultipleTiemesAndWriteErrorToFile(G2, plainOneDivSqrtOfX, miserOneDivSqrtOfX, vegasOneDivSqrtOfX, exactOneDivSqrtOfX);

  return 0;
}
