/*
Fuentes consultadas para QuickSort:
- Cormen, T. H. et al. (2009). Introduction to Algorithms, 3rd ed.
    https://mitpress.mit.edu/9780262033848/introduction-to-algorithms/
- Wikipedia contributors. "Quicksort".
    https://en.wikipedia.org/wiki/Quicksort
*/

#include <iostream>
#include <vector>

using namespace std;

/*  

Tipo divide y vencerás

*/


int particion(vector<int> &array, int inicio, int final){
    int pivote = array[inicio];
    int i = inicio + 1;
    for (int j = i ; j <= final ; j++){
        if (array[j] < pivote){
            swap(array[i], array[j]);
            i++;
        }
    }
    swap(array[inicio], array[i-1]);
    return i-1;
}

void quicksort(vector<int> &array , int inicio, int final){

    if (inicio < final){
        int pivote = particion(array, inicio, final);
        quicksort(array, inicio, pivote-1);
        quicksort(array, pivote + 1 , final);

    }

}
