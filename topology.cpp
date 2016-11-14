#include "topology.h"

using namespace std;

ToMatrix::ToMatrix(const int nbRow, const int nbCol)
    : m_nbRow(nbRow), m_nbCol(nbCol) {}
int ToMatrix::operator()(int row, int col) { return m_nbCol * row + col; }
const int ToMatrix::getRow() const { return m_nbRow; }
const int ToMatrix::getCol() const { return m_nbCol; }
bool ToMatrix::isValid(int row, int col) {
  return row > -1 && row < getRow() && col > -1 && col < getCol();
}

void print_matrix(int *matrix, ToMatrix t) {
  std::cout << "Matrix : " << t.getRow() << " - " << t.getCol() << std::endl;
  for (size_t i = 0; i < t.getRow(); i++) {
    for (size_t j = 0; j < t.getCol(); j++) {
      std::cout << matrix[t(i, j)] << " " << flush;
    }
    std::cout << std::endl;
  }
}

void changeConnected(int row, int col, int *matrix, int *matAux, ToMatrix t) {
  if (t.isValid(row + 1, col) && matAux[t(row + 1, col)] == 0 &&
      matrix[t(row + 1, col)] == 1) {
    matAux[t(row + 1, col)] = matAux[t(row, col)];
    changeConnected(row + 1, col, matrix, matAux, t);
  }
  if (t.isValid(row - 1, col) && matAux[t(row - 1, col)] == 0 &&
      matrix[t(row - 1, col)] == 1) {
    matAux[t(row - 1, col)] = matAux[t(row, col)];
    changeConnected(row - 1, col, matrix, matAux, t);
  }
  if (t.isValid(row, col + 1) && matAux[t(row, col + 1)] == 0 &&
      matrix[t(row, col + 1)] == 1) {
    matAux[t(row, col + 1)] = matAux[t(row, col)];
    changeConnected(row, col + 1, matrix, matAux, t);
  }
  if (t.isValid(row, col - 1) && matAux[t(row, col - 1)] == 0 &&
      matrix[t(row, col - 1)] == 1) {
    matAux[t(row, col - 1)] = matAux[t(row, col)];
    changeConnected(row, col - 1, matrix, matAux, t);
  }
}

int nbConnectedComponent(double *image, const int taille) {
  int compteur = 0;
  int matrix[taille * taille];
  int matAux[taille * taille] = {};
  ToMatrix t;

  for (size_t i = 0; i < taille * taille; i++) {
    if (image[i] * 2 >= 1) {
      matrix[i] = 1;
    } else {
      matrix[i] = 0;
    }
  }
  // print_matrix(matrix, t);
  // print_matrix(matAux, t);
  for (size_t i = 1; i < taille; i++) {
    for (size_t j = 1; j < taille; j++) {
      if (matrix[t(i, j)] == 1 and matAux[t(i, j)] == 0) {
        compteur++;
        matAux[t(i, j)] = compteur;
        changeConnected(i, j, matrix, matAux, t);
      }
    }
  }

  // print_matrix(matAux, t);

  return compteur;
}
