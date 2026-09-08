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

    // 1) Inicializo en cero los 4 sub-bloques de C
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = 0;

    // 2) Creo sólo tres matrices temporales k×k
    int** X = createMatrix(k);
    int** Y = createMatrix(k);
    int** Z = createMatrix(k);

    // 3) Para cada uno de los 7 productos M_i:
    //    – construyo los operandos en X,Y
    //    – llamo recursivamente strassen(X,Y,Z,k)
    //    – añado o resto Z en el bloque correspondiente de C
    //
    // M1 = (A11+A22)*(B11+B22)
    for (int i = 0; i < k; i++) {
      for (int j = 0; j < k; j++) {
        X[i][j] = A[i][j] + A[i+k][j+k];
        Y[i][j] = B[i][j] + B[i+k][j+k];
      }
    }
    strassen(X, Y, Z, k);
    for (int i = 0; i < k; i++) {
      for (int j = 0; j < k; j++) {
        C[i][j]       += Z[i][j];  // C11 += M1
        C[i+k][j+k]   += Z[i][j];  // C22 += M1
      }
    }

    // M2 = (A21+A22)*B11
    for (int i = 0; i < k; i++) {
      for (int j = 0; j < k; j++) {
        X[i][j] = A[i+k][j] + A[i+k][j+k];
        Y[i][j] = B[i][j];
      }
    }
    strassen(X, Y, Z, k);
    for (int i = 0; i < k; i++) {
      for (int j = 0; j < k; j++) {
        C[i+k][j]     += Z[i][j];  // C21 += M2
        C[i+k][j+k]   -= Z[i][j];  // C22 -= M2
      }
    }

    // M3 = A11*(B12−B22)
    for (int i = 0; i < k; i++) {
      for (int j = 0; j < k; j++) {
        X[i][j] = A[i][j];
        Y[i][j] = B[i][j+k] - B[i+k][j+k];
      }
    }
    strassen(X, Y, Z, k);
    for (int i = 0; i < k; i++) {
      for (int j = 0; j < k; j++) {
        C[i][j+k]     += Z[i][j];  // C12 += M3
        C[i+k][j+k]   += Z[i][j];  // C22 += M3
      }
    }

    // M4 = A22*(B21−B11)
    for (int i = 0; i < k; i++) {
      for (int j = 0; j < k; j++) {
        X[i][j] = A[i+k][j+k];
        Y[i][j] = B[i+k][j] - B[i][j];
      }
    }
    strassen(X, Y, Z, k);
    for (int i = 0; i < k; i++) {
      for (int j = 0; j < k; j++) {
        C[i][j]       += Z[i][j];  // C11 += M4
        C[i+k][j]     += Z[i][j];  // C21 += M4
      }
    }

    // M5 = (A11+A12)*B22
    for (int i = 0; i < k; i++) {
      for (int j = 0; j < k; j++) {
        X[i][j] = A[i][j] + A[i][j+k];
        Y[i][j] = B[i+k][j+k];
      }
    }
    strassen(X, Y, Z, k);
    for (int i = 0; i < k; i++) {
      for (int j = 0; j < k; j++) {
        C[i][j]       -= Z[i][j];  // C11 −= M5
        C[i][j+k]     += Z[i][j];  // C12 += M5
      }
    }

    // M6 = (A21−A11)*(B11+B12)
    for (int i = 0; i < k; i++) {
      for (int j = 0; j < k; j++) {
        X[i][j] = A[i+k][j] - A[i][j];
        Y[i][j] = B[i][j] + B[i][j+k];
      }
    }
    strassen(X, Y, Z, k);
    for (int i = 0; i < k; i++) {
      for (int j = 0; j < k; j++) {
        C[i+k][j+k]   -= Z[i][j];  // C22 −= M6
      }
    }

    // M7 = (A12−A22)*(B21+B22)
    for (int i = 0; i < k; i++) {
      for (int j = 0; j < k; j++) {
        X[i][j] = A[i][j+k] - A[i+k][j+k];
        Y[i][j] = B[i+k][j] + B[i+k][j+k];
      }
    }
    strassen(X, Y, Z, k);
    for (int i = 0; i < k; i++) {
      for (int j = 0; j < k; j++) {
        C[i][j]       += Z[i][j];  // C11 += M7
        C[i+k][j]     += Z[i][j];  // C21 += M7
      }
    }

    // 4) libero los únicos 3 buffers
    deleteMatrix(X, k);
    deleteMatrix(Y, k);
    deleteMatrix(Z, k);
}




/*
    Código modificado para Strassen puro, desde la fuente
    Rangelak. (s.f.). Strassen-Matrix-Multiplication/strassen.cpp at master | rangelak/Strassen-Matrix-Multiplication. GitHub. 
    https://github.com/rangelak/Strassen-Matrix-Multiplication/blob/master/strassen.cpp
*/