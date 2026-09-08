# Ordenamiento de arreglos

## Algoritmos

El programa compara las implementaciones de:

- MergeSort: `algorithms/mergesort.cpp`
- QuickSort: `algorithms/quicksort.cpp`
- PatienceSort: `algorithms/patiencesort.cpp`
- `std::sort`: `algorithms/sort.cpp`

Todas reciben una copia del mismo arreglo. El programa verifica el resultado
con `std::is_sorted` y mide exclusivamente el tiempo del algoritmo en
microsegundos (`us`). La lectura del archivo, la copia del arreglo y la
escritura de resultados quedan fuera de esa medición.

## Requisitos

En Linux o WSL se necesita:

```bash
sudo apt install g++ make python3 python3-numpy
```

Desde Windows también se puede usar un entorno equivalente con `g++`, `make`
y NumPy disponibles.

## Compilar y ejecutar

Ejecuta los comandos desde esta carpeta (`code/sorting`):

```bash
make compile
make run
```

El programa lee todos los archivos regulares de `data/array_input/`. Por cada
entrada genera una medición en:

- `data/measurements/mergesort.csv`
- `data/measurements/quicksort.csv`
- `data/measurements/patiencesort.csv`
- `data/measurements/sort.csv`

La salida ordenada de `std::sort` se guarda en
`data/array_output/<nombre>_out.txt`.

Los CSV usan el formato:

```text
nombre_de_entrada;tiempo_en_microsegundos
```

Las mediciones compartidas durante el desarrollo se conservaron en
`data/measurements/resultados_para_latex.txt` como respaldo para preparar el
informe. Este archivo es histórico y no reemplaza los CSV generados por el
programa.

Para borrar entradas, salidas y mediciones anteriores:

```bash
make clean-data
```

## Generar entradas

El generador distingue:

- `D1`: valores entre `0` y `9`, con muchos duplicados.
- `D7`: valores entre `0` y `10^7`, con un dominio mucho más amplio.
- Tipos: `ascendente`, `descendente` y `aleatorio`.

Para generar todo el conjunto configurado:

```bash
make generate
```

Para probar una única entrada de cada tamaño, limpiando los datos anteriores:

```bash
make generate-1000
make run

make generate-100000
make run

make generate-10000000
make run
```

Los targets anteriores generan un arreglo aleatorio `D1` con muestra `a`.
También se puede invocar directamente el script:

```bash
cd scripts
python3 array_generator.py --n 1000 --tipo aleatorio --dominio D7 --muestras a
cd ..
```

## Interpretación

Con arreglos muy pequeños es normal obtener `0 us`, porque el algoritmo tarda
menos de un microsegundo o la resolución del reloj no permite distinguirlo.
Para comparar rendimiento conviene usar al menos `n=1000`.

En `D1` hay muchos valores repetidos. Eso puede perjudicar a QuickSort, que
usa el primer elemento como pivote, y también puede aumentar el costo de la
implementación actual de PatienceSort. `std::sort` suele ser el más rápido por
estar altamente optimizado.

El caso `n=10^7` puede requerir varios minutos y bastante memoria. La versión
actual no tiene timeout por proceso: se puede detener con `Ctrl+C`.

## Limpieza y recompilación

```bash
make clean
make compile
```