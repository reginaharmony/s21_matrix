#ifndef MATRIX_21
#define MATRIX_21

#define OK 0
#define ERR_FAIL 1
#define ERR_CALC 2

#define SUCCESS 1
#define FAILURE 0
#define EPS 1e7

// #include <stdio.h>
#include <limits.h>
#include <math.h>
#include <stdlib.h>

extern double round(double x);

#define is_fin(x) __builtin_isfinite(x)
#define is_nan(x) __builtin_isnan(x)
#define is_inf(x) __builtin_isinf(x)

#define FOR(x) for (int i = 0; i < x; i++)
#define FORS(x, y) FOR(x) for (int j = 0; j < y; j++)
#define FORSZ(x, y, z) FORS(x, y) for (int k = 0; k < z; k++)

typedef struct matrix_struct {
  double **matrix;
  int rows;
  int columns;
} matrix_t;

#define M_A matrix_t *A
#define M_AB matrix_t *A, matrix_t *B
#define M_ARES matrix_t *A, matrix_t *result
#define M_ADRES matrix_t *A, double *result
#define M_ABRES matrix_t *A, matrix_t *B, matrix_t *result
#define M_ANRES matrix_t *A, double number, matrix_t *result

// SUPPLEMENTARY ||
int s21_m_valid(M_A);
int s21_m_eqdim(M_AB);

// BASIC ||
int s21_create_matrix(int rows, int columns, matrix_t *result);
void s21_remove_matrix(M_A);
int s21_eq_matrix(M_AB);

// CALCULATIONS ||
int s21_sum_matrix(M_ABRES);
int s21_sub_matrix(M_ABRES);
int s21_mult_number(M_ANRES);
int s21_mult_matrix(M_ABRES);

// MISCELLANEOUS ||
int s21_transpose(M_ARES);
int s21_inverse_matrix(M_ARES);
int s21_calc_complements(M_ARES);
int s21_determinant(M_ADRES);

matrix_t *s21_create_minor(int ex_rows, int ex_columns, matrix_t *A);

#endif  // MATRIX_21