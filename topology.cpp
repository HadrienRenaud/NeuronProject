#include "topology.h"

using namespace std;

ToMatrix::ToMatrix(const int nbRow, const int nbCol)
	: m_nbRow(nbRow), m_nbCol(nbCol) {
}
int ToMatrix::operator()(int row, int col) {
	return m_nbCol * row + col;
}
const int ToMatrix::getRow() const {
	return m_nbRow;
}
const int ToMatrix::getCol() const {
	return m_nbCol;
}

bool ToMatrix::isValid(int row, int col) {
	return row > -1 && row < getRow() && col > -1 && col < getCol();
}

void print_matrix(int *matrix, ToMatrix t) {
	std::cout << "Matrix : " << t.getRow() << " - " << t.getCol() << std::endl;
	for (int i = 0; i < t.getRow(); i++) {
		for (int j = 0; j < t.getCol(); j++) {
			std::cout << matrix[t(i, j)] << " " << flush;
		}
		std::cout << std::endl;
	}
}

void changeConnected(int row, int col, int *matrix, int *matAux, ToMatrix t) {
	int comp = matrix[t(row, col)];
	if (t.isValid(row + 1, col) && matAux[t(row + 1, col)] == 0 &&
	    matrix[t(row + 1, col)] == comp) {
		matAux[t(row + 1, col)] = matAux[t(row, col)];
		changeConnected(row + 1, col, matrix, matAux, t);
	}
	if (t.isValid(row - 1, col) && matAux[t(row - 1, col)] == 0 &&
	    matrix[t(row - 1, col)] == comp) {
		matAux[t(row - 1, col)] = matAux[t(row, col)];
		changeConnected(row - 1, col, matrix, matAux, t);
	}
	if (t.isValid(row, col + 1) && matAux[t(row, col + 1)] == 0 &&
	    matrix[t(row, col + 1)] == comp) {
		matAux[t(row, col + 1)] = matAux[t(row, col)];
		changeConnected(row, col + 1, matrix, matAux, t);
	}
	if (t.isValid(row, col - 1) && matAux[t(row, col - 1)] == 0 &&
	    matrix[t(row, col - 1)] == comp) {
		matAux[t(row, col - 1)] = matAux[t(row, col)];
		changeConnected(row, col - 1, matrix, matAux, t);
	}
}


vector<int> topology(double image[TAILLE][TAILLE])
{
	double line[TAILLE*TAILLE];

	for (int j = 0; j < TAILLE; j++)
	{
		for (int i = 0; i < TAILLE; i++)
			line[j*TAILLE + i] = image[j][i];

	}

	return numbers(line, TAILLE);
}

vector<int> numbers(double *image, const int taille) {
	int nbConnectedComponents = 0;
	int nbSpaces = 0;
	int nbHoles = 0;
	int matrix[taille * taille];
	int matAux[taille * taille];
	ToMatrix t(taille,taille);
	vector<int> result = {};
	
	
	// matAux initialisation, matrix format
	for (int i = 0; i < taille * taille; i++)
	{
		matAux[i] = 0;

		if (image[i] >= LIMIT_EMPTY_TOPOLOGY)
			matrix[i] = 1;
		else
			matrix[i] = 0;
	}

	// Border initialisation
	for (int i=0; i < taille; i++)
	{
		if (matrix[t(0, i)] == 0) {
			matAux[t(0, i)] = -1;
			changeConnected(0, i, matrix, matAux, t);
		}
		if (matrix[t(taille - 1, i)] == 0) {
			matAux[t(taille - 1, i)] = -1;
			changeConnected(taille - 1, i, matrix, matAux, t);
		}
		if (matrix[t(i, 0)] == 0 && matAux[t(i, 0)] == 0) {
			matAux[t(i, 0)] = -1;
			changeConnected(i, 0, matrix, matAux, t);
		}
		if (matrix[t(i, taille - 1)] == 0 && matAux[t(i, taille - 1)]) {
			matAux[t(i, taille -1)] = -1;
			changeConnected(i, taille - 1, matrix, matAux, t);
		}
	}

	// print_matrix(matAux,t);

	// Count nbHoles
	for (int i = 1; i < taille; i++) {
		for (int j = 1; j < taille; j++) {
			if (matAux[t(i, j)] == 0) {
				// compteurs
				nbSpaces++;
				if (matrix[t(i, j)] == 0)
					nbHoles++;
				else
					nbConnectedComponents++;
				
				matAux[t(i, j)] = nbSpaces;
				changeConnected(i, j, matrix, matAux, t);
			}
		}
	}

	// print_matrix(matAux, t);
	// std::cout << "Résultat :" << nbHoles << '\n';

	// Result
	result.push_back(nbConnectedComponents);
	result.push_bask(nbHoles);
	return result;
}
