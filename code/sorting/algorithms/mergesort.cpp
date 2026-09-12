/*
Fuentes consultadas para MergeSort:
- Cormen, T. H. et al. (2009). Introduction to Algorithms, 3rd ed.
    https://mitpress.mit.edu/9780262033848/introduction-to-algorithms/
- Wikipedia contributors. "Merge sort".
    https://en.wikipedia.org/wiki/Merge_sort
*/

#include <iostream>
#include <vector>


using namespace std;

/*función recursiva*/
/*tipo divide y vencerás*/
/* 
1. dividir en subproblemas
2. resolver cada problema de forma recursiva
3. Combinar soluciones y tener una gran solución
*/
void merge(vector<int> &arreglo, int inicio, int mitad, int final){
    int k ;
    int elementosIzq = mitad-inicio +1;
    int elementosDer = final-mitad;

    vector <int> izquierda(elementosIzq);
    vector<int> derecha(elementosDer);

    for (int i = 0 ; i < elementosIzq ; i++) izquierda[i] = arreglo[inicio+i];
    for (int j = 0 ; j < elementosDer ; j++) derecha[j] = arreglo[mitad +j+ 1];

    int i = 0;
    int j = 0;
    k = inicio;

    while( i < elementosIzq && j < elementosDer){
        if (izquierda[i] <= derecha[j]){
            arreglo[k] = izquierda[i];
            i++;
        }
        else{
            arreglo[k] = derecha[j];
            j++;
        }
        k++;
    }

    while (j< elementosDer){
        arreglo[k] = derecha[j];
        j++;
        k++;
    }

    while(i < elementosIzq){
        arreglo[k] = izquierda[i];
        i++;
        k++;
    }
}

void mergesort(vector<int> &arreglo, int inicio, int final){
    if (inicio < final){ /*tpdo alg recursivo debe empezar con un caso base*/
        int mitad = inicio + (final - inicio)/2;
        mergesort(arreglo, inicio, mitad);
        mergesort(arreglo,mitad+1, final);
        merge(arreglo, inicio, mitad, final);
    }
}

void imprimirArreglo(vector<int> arreglo){
    for(size_t i = 0 ; i < arreglo.size(); i++){
         cout << arreglo[i] << " " ;
    }
    cout << endl; 
}