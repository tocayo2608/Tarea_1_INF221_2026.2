#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#ifdef __linux__
#include <sys/resource.h>
#endif

// Implementaciones disponibles en este repositorio.
void mergesort(std::vector<int>& arreglo, int inicio, int final);
void quicksort(std::vector<int>& arreglo, int inicio, int final);
void patiencesort(std::vector<int>& arreglo);
std::vector<int> sortArray(std::vector<int>& arreglo);

struct Metrics {
	long long time_us;
	long memory_kb;
};

std::vector<int> readArrayFromFile(const std::filesystem::path& filepath) {
	std::ifstream inputFile(filepath);
	std::vector<int> arreglo;
	int numero;

	while (inputFile >> numero) {
		arreglo.push_back(numero);
	}

	return arreglo;
}

template <typename Funcion>
Metrics medirPerformance(Funcion funcion) {
	long memory_before = 0;
	long memory_after = 0;
#ifdef __linux__
	struct rusage usage_before {};
	struct rusage usage_after {};
	getrusage(RUSAGE_SELF, &usage_before);
#endif

	const auto inicio = std::chrono::steady_clock::now();
	funcion();
	const auto final = std::chrono::steady_clock::now();

#ifdef __linux__
	getrusage(RUSAGE_SELF, &usage_after);
	memory_before = usage_before.ru_maxrss;
	memory_after = usage_after.ru_maxrss;
#endif

	return {
		std::chrono::duration_cast<std::chrono::microseconds>(
			final - inicio
		).count(),
		memory_after - memory_before
	};
}

bool runAlgo(
	const std::string& nombreAlgoritmo,
	const std::string& entrada,
	const std::filesystem::path& measurementFile,
	const std::filesystem::path& outputFile,
	const std::function<void(std::vector<int>&)>& algoritmo,
	const std::vector<int>& baseArray
) {
	std::vector<int> copia = baseArray;
	const Metrics medicion = medirPerformance([&]() {
		algoritmo(copia);
	});

	if (!std::is_sorted(copia.begin(), copia.end())) {
		std::cerr << "Error: " << entrada << " no fue ordenado correctamente\n";
		return false;
	}

	if (!outputFile.empty()) {
		std::ofstream output(outputFile);
		for (int numero : copia) {
			output << numero << ' ';
		}
		output << '\n';
	}

	std::ofstream measurements(measurementFile, std::ios::app);
	measurements << entrada << ';'
				  << medicion.time_us << ';'
				  << medicion.memory_kb << '\n';

	std::cout << "  " << nombreAlgoritmo << ": "
			  << medicion.time_us << " us, "
			  << medicion.memory_kb << " KB\n";
	return true;
}

int main() {
	const std::filesystem::path inputPath = "data/array_input";
	const std::filesystem::path outputPath = "data/array_output";
	const std::filesystem::path measurementPath = "data/measurements";

	std::filesystem::create_directories(outputPath);
	std::filesystem::create_directories(measurementPath);

	for (const auto& fileEntry : std::filesystem::directory_iterator(inputPath)) {
		if (!fileEntry.is_regular_file()) {
			continue;
		}

		const std::string fileName = fileEntry.path().stem().string();
		const std::vector<int> baseArray = readArrayFromFile(fileEntry.path());
		const int n = static_cast<int>(baseArray.size());

		if (n == 0) {
			std::cerr << "Entrada vacia: " << fileName << '\n';
			continue;
		}

		std::cout << fileName << " (n=" << n << ")\n";

		runAlgo(
			"MergeSort",
			fileName,
			measurementPath / "mergesort.csv",
			{},
			[&](std::vector<int>& arreglo) {
				mergesort(arreglo, 0, n - 1);
			},
			baseArray
		);

		runAlgo(
			"QuickSort",
			fileName,
			measurementPath / "quicksort.csv",
			{},
			[&](std::vector<int>& arreglo) {
				quicksort(arreglo, 0, n - 1);
			},
			baseArray
		);

		runAlgo(
			"PatienceSort",
			fileName,
			measurementPath / "patiencesort.csv",
			{},
			[](std::vector<int>& arreglo) {
				patiencesort(arreglo);
			},
			baseArray
		);

		runAlgo(
			"std::sort",
			fileName,
			measurementPath / "sort.csv",
			outputPath / (fileName + "_out.txt"),
			[](std::vector<int>& arreglo) {
				sortArray(arreglo);
			},
			baseArray
		);
	}

	return 0;
}
