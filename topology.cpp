#include "topology.h"

using namespace std;

int nbConnectedComponent(double *image, int n) {
  int compteur = 0;
  int matrix[n][n];
  int matAux[n][n];

  // table to matrix
  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < n; j++) {
      matrix[i][j] = (int)floor(table[i * n + j] * 2);
    }
  }

  // Initialisation of matAux
  matAux[0][0] = 0;
  // 1st line
  for (size_t j = 1; j < n; j++) {
    if (matrix[0][j] == 1) {
      if (matrix[0][j - 1] > 0) {
        matrix[0][j] = matrix[0][j - 1];
      } else {
        compteur++;
        matrix[0][j] = compteur;
      }
    }
  }
  // 1st column
  for (size_t i = 1; i < n; i++) {
    if (matrix[i][0] == 1) {
      if (matrix[i - 1][0] > 0) {
        matrix[i][0] = matrix[i - 1][0];
      } else {
        compteur++;
        matrix[i][0] = compteur;
      }
    }

    // Go
    for (size_t i = 1; i < n; i++) {
      for (size_t j = 1; j < n; j++) {
        if (matrix[i][j] == 1) {
          matAux[i][j] = max(max(matrix[i - 1][j - 1], matrix[i - 1][j]),
                             matrix[i][j - 1]);
          if (matAux[i][j] == 0) {
            compteur++;
            matAux[i][j] = compteur;
          }
        }
      }
    }

    // Return
    for (size_t i = n - 1; i > -1; i--) {
      for (size_t j = n - 1; j > -1; j--) {
        if (matrix[i][j] == 1) {
        }
      }
    }

    return compteur;
  }
