# Ordenamiento de arreglos

## Algoritmos

El programa compara las implementaciones de:

- MergeSort: `algorithms/mergesort.cpp`
- QuickSort: `algorithms/quicksort.cpp`
- PatienceSort: `algorithms/patiencesort.cpp`
- `std::sort`: `algorithms/sort.cpp`

Todas reciben una copia del mismo arreglo. El programa verifica el resultado
con `std::is_sorted` y mide exclusivamente el algoritmo: tiempo en
microsegundos (`us`) y aumento del máximo de memoria residente en KB. La
lectura del archivo, la copia del arreglo y la escritura de resultados quedan
fuera de esa medición.

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

Para generar gráficos desde los CSV:

```bash
make plot
```

Los PNG se guardan en `data/plots/` y se generan agrupados por tipo de entrada
y dominio. El script también genera gráficos de memoria si los CSV contienen
una tercera columna de memoria.

El formato real de los CSV incluye también memoria:

```text
nombre_de_entrada;tiempo_us;memoria_kb
```

Las mediciones compartidas durante el desarrollo se conservaron en
`data/measurements/resultados_para_latex.txt` como respaldo para preparar el
informe. Este archivo es histórico y no reemplaza los CSV generados por el
programa. Las mediciones nuevas deben generarse nuevamente, porque las
anteriores no contienen memoria.

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

`make generate` crea las 72 entradas del enunciado, incluyendo las 18
entradas de `n=10^7`. Debido al costo de ejecutar QuickSort y PatienceSort en
ese tamaño, para las mediciones prácticas se usa:

```bash
make generate-reduced
```

Este target genera las 54 combinaciones completas con `n=10`, `1000` y
`100000`, más una única entrada grande:

```text
10000000_aleatorio_D7_a.txt
```

Las demás muestras `aleatorio` de `n=10^7` (`D7_b`, `D7_c`, `D1_a`, `D1_b`,
`D1_c`) se generaron aparte y se midieron con un programa auxiliar,
`sorting_missing.cpp` (ver más abajo), sin modificar `sorting.cpp` ni volver
a correr lo ya medido. En total quedaron 57 entradas de sorting con los
cuatro algoritmos completos, más una entrada parcial
(`10000000_aleatorio_D1_a`, solo MergeSort y QuickSort). Esta decisión queda
documentada para que los gráficos y resultados del informe sean
reproducibles.

### Programa auxiliar `sorting_missing.cpp`

Corre los 4 algoritmos solo sobre una lista fija de archivos (los que faltan
de `n=10^7`, tipo `aleatorio`) y agrega los resultados a los mismos CSV de
`data/measurements/`, sin tocar `sorting.cpp`. Se usa así:

```bash
g++ -std=c++17 -O2 -Wall -Wextra \
  sorting_missing.cpp algorithms/mergesort.cpp algorithms/quicksort.cpp \
  algorithms/patiencesort.cpp algorithms/sort.cpp -o sorting_missing
./sorting_missing
```

**No se generaron ni corrieron** las combinaciones `ascendente`/`descendente`
de `n=10^7` (12 casos) ni las muestras `D1_b`/`D1_c` de `aleatorio`: con el
pivote fijo de QuickSort y el escaneo lineal de pilas de PatienceSort, esos
casos alcanzan complejidad práctica $O(n^2)$ y no terminan en un tiempo
razonable (el caso `D1,a` que sí se corrió, con QuickSort, tardó ~50.8
minutos solo para esa combinación).

Para generar y medir el conjunto completo exigido por el enunciado, incluyendo
las 18 entradas de `n=10^7`, ejecuta:

```bash
make clean-data
make generate
make run
make plot
```

La ejecución completa puede requerir varios minutos u horas y varios GB de
espacio temporal, especialmente para QuickSort y PatienceSort.

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

Para acumular varios tamaños en los mismos CSV y obtener curvas con más de un
punto, limpia las mediciones una sola vez y usa los targets `prepare-*`:

```bash
make clean-data
make prepare-1000
make run
make prepare-100000
make run
make prepare-10000000
make run
make plot
```

`prepare-*` elimina las entradas y salidas anteriores, pero conserva los CSV.
Los targets `generate-*`, en cambio, también eliminan las mediciones y sirven
para ejecutar un tamaño aislado.

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

La medición de memoria usa `getrusage` y por eso debe ejecutarse en Linux o
WSL. En Windows nativo el programa compila, pero registra `0 KB` para memoria.

## Limpieza y recompilación

```bash
make clean
make compile
```