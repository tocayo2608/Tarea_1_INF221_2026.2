#include <iostream>

// Crea una matriz n×n inicializada a 0
int** createMatrix(int n) {
    int** M = new int*[n];
    for (int i = 0; i < n; i++) {
        M[i] = new int[n];
        for (int j = 0; j < n; j++)
            M[i][j] = 0;
    }
    return M;
}

// Libera una matriz creada con createMatrix
void deleteMatrix(int** M, int n) {
    for (int i = 0; i < n; i++)
        delete[] M[i];
    delete[] M;
}

// Suma A + B en una nueva matriz de tamaño n×n
int** addMatrix(int** A, int** B, int n) {
    int** C = createMatrix(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] + B[i][j];
    return C;
}

// Resta A – B en una nueva matriz de tamaño n×n
int** subMatrix(int** A, int** B, int n) {
    int** C = createMatrix(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] - B[i][j];
    return C;
}

// Copia el bloque n×n de P (empezando en (iB,jB)) a C[0..n-1][0..n-1]
void splitMatrix(int** P, int** C, int iB, int jB, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = P[i + iB][j + jB];
}

// Copia C[0..n-1][0..n-1] dentro de P en el bloque que empieza en (iB,jB)
void joinMatrix(int** C, int** P, int iB, int jB, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            P[i + iB][j + jB] = C[i][j];
}


// Strassen recursivo: C = A × B, tamaño n×n (void, escribe en C)
// --------------------------------------------------
// Strassen “in-place” con sólo 3 temporales
// --------------------------------------------------
void strassen(int** A, int** B, int** C, int n) {
    if (n == 1) {
        C[0][0] = A[0][0] * B[0][0];
        return;
    }

    int k = n/2;

    int** A11 = createMatrix(k);
    int** A12 = createMatrix(k);
    int** A21 = createMatrix(k);
    int** A22 = createMatrix(k);
    int** B11 = createMatrix(k);
    int** B12 = createMatrix(k);
    int** B21 = createMatrix(k);
    int** B22 = createMatrix(k);
    int** M1 = createMatrix(k);
    int** M2 = createMatrix(k);
    int** M3 = createMatrix(k);
    int** M4 = createMatrix(k);
    int** M5 = createMatrix(k);
    int** M6 = createMatrix(k);
    int** M7 = createMatrix(k);

    splitMatrix(A, A11, 0, 0, k);
    splitMatrix(A, A12, 0, k, k);
    splitMatrix(A, A21, k, 0, k);
    splitMatrix(A, A22, k, k, k);
    splitMatrix(B, B11, 0, 0, k);
    splitMatrix(B, B12, 0, k, k);
    splitMatrix(B, B21, k, 0, k);
    splitMatrix(B, B22, k, k, k);

    int** left = nullptr;
    int** right = nullptr;

    left = addMatrix(A11, A22, k);
    right = addMatrix(B11, B22, k);
    strassen(left, right, M1, k);
    deleteMatrix(left, k);
    deleteMatrix(right, k);

    left = addMatrix(A21, A22, k);
    strassen(left, B11, M2, k);
    deleteMatrix(left, k);

    right = subMatrix(B12, B22, k);
    strassen(A11, right, M3, k);
    deleteMatrix(right, k);

    right = subMatrix(B21, B11, k);
    strassen(A22, right, M4, k);
    deleteMatrix(right, k);

    left = addMatrix(A11, A12, k);
    strassen(left, B22, M5, k);
    deleteMatrix(left, k);

    left = subMatrix(A21, A11, k);
    right = addMatrix(B11, B12, k);
    strassen(left, right, M6, k);
    deleteMatrix(left, k);
    deleteMatrix(right, k);

    left = subMatrix(A12, A22, k);
    right = addMatrix(B21, B22, k);
    strassen(left, right, M7, k);
    deleteMatrix(left, k);
    deleteMatrix(right, k);

    int** C11 = createMatrix(k);
    int** C12 = createMatrix(k);
    int** C21 = createMatrix(k);
    int** C22 = createMatrix(k);
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            C11[i][j] = M1[i][j] + M4[i][j] - M5[i][j] + M7[i][j];
            C12[i][j] = M3[i][j] + M5[i][j];
            C21[i][j] = M2[i][j] + M4[i][j];
            C22[i][j] = M1[i][j] - M2[i][j] + M3[i][j] + M6[i][j];
        }
    }

    joinMatrix(C11, C, 0, 0, k);
    joinMatrix(C12, C, 0, k, k);
    joinMatrix(C21, C, k, 0, k);
    joinMatrix(C22, C, k, k, k);

    deleteMatrix(A11, k);
    deleteMatrix(A12, k);
    deleteMatrix(A21, k);
    deleteMatrix(A22, k);
    deleteMatrix(B11, k);
    deleteMatrix(B12, k);
    deleteMatrix(B21, k);
    deleteMatrix(B22, k);
    deleteMatrix(M1, k);
    deleteMatrix(M2, k);
    deleteMatrix(M3, k);
    deleteMatrix(M4, k);
    deleteMatrix(M5, k);
    deleteMatrix(M6, k);
    deleteMatrix(M7, k);
    deleteMatrix(C11, k);
    deleteMatrix(C12, k);
    deleteMatrix(C21, k);
    deleteMatrix(C22, k);
}




/*
    Código modificado para Strassen puro, desde la fuente
    Rangelak. (s.f.). Strassen-Matrix-Multiplication/strassen.cpp at master | rangelak/Strassen-Matrix-Multiplication. GitHub. 
    https://github.com/rangelak/Strassen-Matrix-Multiplication/blob/master/strassen.cpp
*/