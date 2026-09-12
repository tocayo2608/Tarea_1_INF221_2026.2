/*
Fuente consultada para la multiplicacion ingenua:
- Cormen, T. H. et al. (2009). Introduction to Algorithms, 3rd ed.
    https://mitpress.mit.edu/9780262033848/introduction-to-algorithms/
*/

#include <iostream>

void naive(int** A, int** B, int** C, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int sum = 0;
            for (int k = 0; k < n; k++) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
} 