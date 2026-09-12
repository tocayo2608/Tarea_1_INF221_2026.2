#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#ifdef __linux__
#include <sys/resource.h>
#endif

// Estas funciones estan implementadas en otros archivos del proyecto.
void mergesort(std::vector<int>& arreglo, int inicio, int final);
void quicksort(std::vector<int>& arreglo, int inicio, int final);
void patiencesort(std::vector<int>& arreglo);
std::vector<int> sortArray(std::vector<int>& arreglo);

// Guardamos el tiempo que demoro el algoritmo y la memoria que uso.
// No hace falta una clase para esto, con una struct simple basta.
struct Metrics {
	long long time_us;
	long memory_kb;
};

// Lee un archivo de texto con numeros separados por espacios/saltos de linea
// y los mete en un vector. Nada raro, un ifstream y un while con >>.
std::vector<int> readArrayFromFile(const std::filesystem::path& filepath) {
	std::vector<int> arreglo;
	std::ifstream inputFile(filepath);
	int numero;

	while (inputFile >> numero) {
		arreglo.push_back(numero);
	}

	return arreglo;
}

// Mide tiempo de ejecucion con steady_clock (no usamos high_resolution_clock
// porque steady_clock siempre avanza para adelante, es mas seguro para medir
// duraciones). La memoria solo se mide en Linux porque getrusage no existe
// en Windows; en otros sistemas simplemente queda en 0.

Metrics medirTiempoYMemoria(void (*algoritmo)(std::vector<int>&), std::vector<int>& arreglo) {
#ifdef __linux__
	struct rusage usoAntes {};
	struct rusage usoDespues {};
	getrusage(RUSAGE_SELF, &usoAntes);
#endif

	auto inicio = std::chrono::steady_clock::now();
	algoritmo(arreglo);
	auto final = std::chrono::steady_clock::now();

	long memoriaKB = 0;
#ifdef __linux__
	getrusage(RUSAGE_SELF, &usoDespues);
	memoriaKB = usoDespues.ru_maxrss - usoAntes.ru_maxrss;
#endif

	long long tiempoUs = std::chrono::duration_cast<std::chrono::microseconds>(final - inicio).count();

	Metrics resultado;
	resultado.time_us = tiempoUs;
	resultado.memory_kb = memoriaKB;
	return resultado;
}

// Corre un algoritmo de ordenamiento sobre una copia del arreglo base,
// verifica que haya quedado bien ordenado, y deja registrado el tiempo
// y memoria en un csv. Si outputFile no viene vacio, tambien guarda el
// arreglo resultante (esto lo usamos solo para std::sort, como referencia).
bool correrAlgoritmo(
	const std::string& nombreAlgoritmo,
	const std::string& nombreEntrada,
	const std::filesystem::path& archivoMediciones,
	const std::filesystem::path& archivoSalida,
	void (*algoritmo)(std::vector<int>&),
	const std::vector<int>& arregloBase
) {
	std::vector<int> copia = arregloBase;

	Metrics medicion = medirTiempoYMemoria(algoritmo, copia);

	if (!std::is_sorted(copia.begin(), copia.end())) {
		std::cerr << "Error: " << nombreEntrada << " no quedo bien ordenado\n";
		return false;
	}

	if (!archivoSalida.empty()) {
		std::ofstream salida(archivoSalida);
		for (int numero : copia) {
			salida << numero << ' ';
		}
		salida << '\n';
	}

	// Guardamos en modo "append" porque este archivo va acumulando
	// una fila por cada entrada que se procesa.
	std::ofstream mediciones(archivoMediciones, std::ios::app);
	mediciones << nombreEntrada << ';'
			   << medicion.time_us << ';'
			   << medicion.memory_kb << '\n';

	std::cout << "  " << nombreAlgoritmo << ": "
			  << medicion.time_us << " us, "
			  << medicion.memory_kb << " KB\n";

	return true;
}

// Wrappers chicos para que mergesort y quicksort (que necesitan indices
// de inicio y final ademas del vector) puedan pasarse como un puntero a
// funcion void(vector<int>&), igual que patiencesort y sortArray.
void correrMergesort(std::vector<int>& arreglo) {
	mergesort(arreglo, 0, static_cast<int>(arreglo.size()) - 1);
}

void correrQuicksort(std::vector<int>& arreglo) {
	quicksort(arreglo, 0, static_cast<int>(arreglo.size()) - 1);
}

void correrPatiencesort(std::vector<int>& arreglo) {
	patiencesort(arreglo);
}

void correrStdSort(std::vector<int>& arreglo) {
	sortArray(arreglo);
}

int main() {
	const std::filesystem::path inputPath = "data/array_input";
	const std::filesystem::path outputPath = "data/array_output";
	const std::filesystem::path measurementPath = "data/measurements";

	std::filesystem::create_directories(outputPath);
	std::filesystem::create_directories(measurementPath);

	// Recorremos cada archivo de entrada en data/array_input y le
	// corremos los 4 algoritmos de ordenamiento uno por uno.
	for (const auto& fileEntry : std::filesystem::directory_iterator(inputPath)) {
		if (!fileEntry.is_regular_file()) {
			continue;
		}

		std::string nombreArchivo = fileEntry.path().stem().string();
		std::vector<int> arregloBase = readArrayFromFile(fileEntry.path());
		int n = static_cast<int>(arregloBase.size());

		if (n == 0) {
			std::cerr << "Entrada vacia: " << nombreArchivo << '\n';
			continue;
		}

		std::cout << nombreArchivo << " (n=" << n << ")\n";

		correrAlgoritmo(
			"MergeSort",
			nombreArchivo,
			measurementPath / "mergesort.csv",
			"",
			correrMergesort,
			arregloBase
		);

		correrAlgoritmo(
			"QuickSort",
			nombreArchivo,
			measurementPath / "quicksort.csv",
			"",
			correrQuicksort,
			arregloBase
		);

		correrAlgoritmo(
			"PatienceSort",
			nombreArchivo,
			measurementPath / "patiencesort.csv",
			"",
			correrPatiencesort,
			arregloBase
		);

		correrAlgoritmo(
			"std::sort",
			nombreArchivo,
			measurementPath / "sort.csv",
			outputPath / (nombreArchivo + "_out.txt"),
			correrStdSort,
			arregloBase
		);
	}

	return 0;
}
