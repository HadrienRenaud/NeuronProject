#ifndef TOPOLOGY_H_INCLUDED
#define TOPOLOGY_H_INCLUDED

#include "config.h"

// ToMatrix Function class
class ToMatrix {
public:
  ToMatrix(const int nbRow = TOPOLOGY_INPUT_SIZE,
           const int nbCol = TOPOLOGY_INPUT_SIZE);
  int operator()(int row, int col);
  const int getRow() const;
  const int getCol() const;
  bool isValid(int row, int col);

private:
  const int m_nbRow;
  const int m_nbCol;
};

void print_matrix(int *matrix, ToMatrix t);

void changeConnected(int row, int col, int *matrix, int *matAux, ToMatrix t);

//! Determine the number of holes
/*! In the image passed as component
    \param matrix : image
    \return nombre int number of holes
 */
int* numbers(double *image, const int taille = TOPOLOGY_INPUT_SIZE);


//! Transforme une matrice correspondant à une image filtrée en une ligne dont on calcule le nombre de composantes connexes.
int* topology(double image[TAILLE][TAILLE]);



#endif /* end of include guard: TOPOLOGY_H_INCLUDED */
