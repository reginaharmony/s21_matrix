#include "s21_matrix.h"

//=================   MISCELLANEOUS   ======================

int s21_transpose(M_ARES) {
  if (!s21_m_valid(A) || !result) return ERR_FAIL;
  s21_create_matrix(A->columns, A->rows, result);
  FORS(A->rows, A->columns) result->matrix[j][i] = A->matrix[i][j];
  return OK;
}

int s21_check_square(M_A) { return A->rows == A->columns; }

matrix_t *s21_create_minor(int ex_rows, int ex_columns, matrix_t *A) {
  if (!s21_m_valid(A)) return NULL;
  matrix_t *minor = calloc(1, sizeof(matrix_t));
  if (!minor || !!s21_create_matrix(A->rows - 1, A->columns - 1, minor))
    return free(minor), NULL;
  FORS(A->rows, A->columns)
  if (i != ex_rows && j != ex_columns)
    minor->matrix[i - (i > ex_rows)][j - (j > ex_columns)] = A->matrix[i][j];

  return minor;
}

#define FREEMINOR           \
  s21_remove_matrix(minor); \
  free(minor);              \
  minor = NULL;

int s21_determinant(M_ADRES) {
  if (!s21_m_valid(A) || !result) return ERR_FAIL;
  if (!s21_check_square(A)) return ERR_CALC;

  double det = 0, det_temp = 0;
  if (A->rows == 1)
    *result = A->matrix[0][0];
  else if (A->rows == 2)
    *result =
        A->matrix[0][0] * A->matrix[1][1] - A->matrix[0][1] * A->matrix[1][0];
  else
    FOR(A->rows) {
      int sign = (i % 2 == 0) ? 1 : -1;
      matrix_t *minor = s21_create_minor(0, i, A);
      if (!minor || s21_determinant(minor, &det_temp)) return ERR_FAIL;
      det += sign * A->matrix[0][i] * det_temp;
      FREEMINOR
      *result = det;
    }
  return OK;
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  if (!s21_m_valid(A) || !result) return ERR_FAIL;
  if (!s21_check_square(A)) return ERR_CALC;

  s21_create_matrix(A->rows, A->columns, result);
  FORS(A->rows, A->columns) {
    matrix_t *minor = s21_create_minor(i, j, A);
    double det_temp = 0;
    if (!minor || s21_determinant(minor, &det_temp)) return ERR_FAIL;
    result->matrix[i][j] = pow(-1, i + j) * det_temp;
    FREEMINOR
  }
  return OK;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  if (!s21_m_valid(A) || !result) return ERR_FAIL;
  if (!s21_check_square(A)) return ERR_CALC;
  double det = 0;
  if (s21_determinant(A, &det) || det == 0) return ERR_CALC;

  matrix_t transspouse, adjoint;
  int ok_adjoint = s21_calc_complements(A, &adjoint);
  if (ok_adjoint || !s21_m_valid(&adjoint)) return ERR_CALC;
  int ok_transspouse = s21_transpose(&adjoint, &transspouse);
  if (ok_transspouse || !s21_m_valid(&transspouse)) return ERR_CALC;

  s21_create_matrix(A->rows, A->columns, result);
  FORS(A->rows, A->columns) {
    result->matrix[i][j] = transspouse.matrix[i][j] / det;
  }
  s21_remove_matrix(&transspouse);
  s21_remove_matrix(&adjoint);

  return OK;
}
