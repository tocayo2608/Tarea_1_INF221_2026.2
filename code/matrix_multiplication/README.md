# Multiplicacion de matrices

## Algoritmos

El programa compara:

- Naive: `algorithms/naive.cpp`
- Strassen: `algorithms/strassen.cpp`

El programa principal no incluye archivos `.hpp`. Declara las funciones que
necesita y enlaza las implementaciones `.cpp` mediante el `makefile`.

## Compilar y ejecutar

Ejecuta desde esta carpeta (`code/matrix_multiplication`):

```bash
make compile
make run
```

También se puede compilar directamente:

```bash
g++ -std=c++17 -O2 -Wall -Wextra \
  matrix_multiplication.cpp \
  algorithms/naive.cpp \
  algorithms/strassen.cpp \
  -o matrix_multiplication
```

El programa procesa solo archivos que terminan en `_1.txt`. Para cada uno
busca su pareja con el mismo nombre terminado en `_2.txt`.

## Generar entradas

```bash
make clean-data
make generate
```

El generador crea los casos exigidos por el enunciado:

- `n`: `16`, `64`, `256` y `1024`.
- Tipo: `dispersa`, `diagonal` y `densa`.
- Dominio: `D0` y `D10`.
- Muestras: `a`, `b` y `c`.

Cada caso produce dos archivos, por ejemplo:

```text
16_densa_D0_a_1.txt
16_densa_D0_a_2.txt
```

El archivo `data/matrix_input/a.txt` es un archivo antiguo del template y no
se procesa porque no representa una pareja de matrices válida.

## Resultados

Las mediciones se guardan en:

- `data/measurements/naive.csv`
- `data/measurements/strassen.csv`

Para generar gráficos desde las mediciones:

```bash
make plot
```

Los PNG se guardan en `data/plots/`, agrupados por tipo de matriz y dominio.
Se genera un gráfico de tiempo y otro de memoria cuando hay datos disponibles.

El formato es:

```text
nombre_de_entrada;tiempo_us;memoria_kb
```

La salida se guarda en:

```text
data/matrix_output/<nombre_de_entrada>_out.txt
```

Para cada entrada se verifica que Naive y Strassen produzcan exactamente la
misma matriz. Si difieren, se muestra un error en la terminal.

Para limpiar resultados anteriores:

```bash
make clean-data
```

El valor de memoria usa `getrusage`, por lo que la medición debe ejecutarse en
Linux o WSL. La medición de tiempo está expresada en microsegundos (`us`).