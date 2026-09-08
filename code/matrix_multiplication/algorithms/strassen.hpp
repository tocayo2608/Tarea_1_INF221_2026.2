#ifndef STRASSEN_HPP
#define STRASSEN_HPP

#include <iostream>

int** createMatrix(int n);

void deleteMatrix(int** M, int n);

int** addMatrix(int** A, int** B, int n);

int** subMatrix(int** A, int** B, int n);

void splitMatrix(int** P, int** C, int iB, int jB, int n);

void joinMatrix(int** C, int** P, int iB, int jB, int n);

void strassen(int** A, int** B, int** C, int n);

#endif