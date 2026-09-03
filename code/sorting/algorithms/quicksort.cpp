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

/*
Fuentes:

https://www.youtube.com/watch?v=UIBaYOUs8K8
https://www.youtube.com/watch?v=UrPJLhKF1jY&t=239s
https://www.youtube.com/watch?v=PupaczLYxmA IMPLEMENTACIÓN
https://es.wikipedia.org/wiki/Quicksort


*/