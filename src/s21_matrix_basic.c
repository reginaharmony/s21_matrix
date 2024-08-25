#include "s21_matrix.h"

//==============   SUPPLEMENTARY   ========================

int s21_m_valid(M_A) {
  return !!A && !!A->matrix && (A->columns > 0 && A->rows > 0);
}
int s21_m_eqdim(M_AB) { return A->rows == B->rows && A->columns == B->columns; }

//==================   BASIC   ============================

void s21_remove_matrix(M_A) {
  if (!A) return;
  if (s21_m_valid(A)) FOR(A->rows) free(A->matrix[i]);
  free(A->matrix), A->matrix = NULL;
}

#define NULLS(x, y, z) !(x = calloc(y, sizeof(z)))
int s21_create_matrix(int rows, int columns, matrix_t *result) {
  if (rows == 0 || columns == 0) return ERR_FAIL;
  if (NULLS(result->matrix, rows, double *)) return ERR_FAIL;
  FOR(rows)
  if (NULLS(result->matrix[i], columns, double))
    return s21_remove_matrix(result), ERR_FAIL;

  result->rows = rows, result->columns = columns;
  return OK;
}

#define ROUND(x) round(x->matrix[i][j] * EPS)
int s21_eq_matrix(M_AB) {
  if (!s21_m_valid(A) || !s21_m_valid(B)) return FAILURE;
  if (!s21_m_eqdim(A, B)) return FAILURE;
  FORS(A->rows, A->columns) if (ROUND(A) != ROUND(B)) return FAILURE;
  return SUCCESS;
}

//=================   CALCULATIONS   ======================

#define SUMSUB(s)                                                             \
  if (!s21_m_valid(A) || !s21_m_valid(B)) return ERR_FAIL;                    \
  if (!s21_m_eqdim(A, B) || !!s21_create_matrix(A->rows, A->columns, result)) \
    return ERR_CALC;                                                          \
  FORS(A->rows, A->columns)                                                   \
  result->matrix[i][j] = A->matrix[i][j] s B->matrix[i][j];                   \
  return OK;

int s21_sum_matrix(M_ABRES) { SUMSUB(+); }
int s21_sub_matrix(M_ABRES) { SUMSUB(-); }

#define MULT(a, b) ((a) * (b))
#define DIV(a, b) (!b ? a / b : ERR_CALC)

#define MULTDIVN(s)                                                        \
  if (!s21_m_valid(A) || !result) return ERR_FAIL;                         \
  if (!!s21_create_matrix(A->rows, A->columns, result) || !is_fin(number)) \
    return ERR_CALC;                                                       \
  FORS(A->rows, A->columns) if (!is_fin(A->matrix[i][j])) return ERR_CALC; \
  FORS(A->rows, A->columns)                                                \
  result->matrix[i][j] += s(A->matrix[i][j], number);                      \
  return OK;

int s21_mult_number(M_ANRES) { MULTDIVN(MULT); }
// int s21_div_number(M_ANRES) { MULTDIVN(DIV); }  // extra (not required)

#define MULTDIV(s)                                                       \
  if (!s21_m_valid(A) || !s21_m_valid(B)) return ERR_FAIL;               \
  if (A->columns != B->rows) return ERR_CALC;                            \
  if (!!s21_create_matrix(A->rows, B->columns, result)) return ERR_CALC; \
  FORSZ(A->rows, B->columns, B->rows)                                    \
  result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];             \
  return OK;

int s21_mult_matrix(M_ABRES) { MULTDIV(MULT); }
// // int s21_div_matrix(M_ABRES) { MULTDIV(DIV); }  // extra (not required)
