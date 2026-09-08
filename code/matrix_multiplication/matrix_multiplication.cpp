#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/resource.h>

// Declaraciones de las implementaciones en algorithms/*.cpp.
void naive(int** A, int** B, int** C, int n);
int** createMatrix(int n);
void deleteMatrix(int** M, int n);
void strassen(int** A, int** B, int** C, int n);


struct Metrics{
    long long time_us;
    long mem_kb;
};

template<typename Funcion>
Metrics medirPerformance(Funcion f) {
    struct rusage ru_before, ru_after;
    getrusage(RUSAGE_SELF, &ru_before);

    const auto start = std::chrono::steady_clock::now();
    f();
    const auto end = std::chrono::steady_clock::now();

    getrusage(RUSAGE_SELF, &ru_after);

    Metrics m;
    m.time_us = std::chrono::duration_cast<std::chrono::microseconds>(
        end - start
    ).count();
    m.mem_kb   = ru_after.ru_maxrss - ru_before.ru_maxrss;
    return m;
}


int** readFromFile(const std::filesystem::path& filepath, int n) {
    int** A = createMatrix(n);

    std::ifstream inputFile(filepath);

    int num;
    std::string lineRow;
    for (int i = 0; i < n && getline(inputFile, lineRow); i++) {
        std::stringstream ss(lineRow);
        for (int j = 0; j < n && ss >> num; j++) {
            A[i][j] = num;
        }
    }

    inputFile.close();
    return A;
}

void saveMatrixToFile(
    const std::filesystem::path& filepath,
    int** A,
    int n
) {
    std::ofstream outputFile(filepath);

    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++) {
            outputFile << A[i][j] << ' ';
        }
        outputFile << '\n';
    }

    outputFile.close();
}

bool sameMatrix(int** A, int** B, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (A[i][j] != B[i][j]) {
                return false;
            }
        }
    }
    return true;
}


void saveMeasurement(const std::string& entrada,
    const std::filesystem::path& filepath,
    const Metrics medicion)
{
    std::ofstream measureFile(filepath, std::ios::app);
    measureFile << entrada << ';'
                << medicion.time_us << ';'
                << medicion.mem_kb << '\n';
}


int main() {
    const std::filesystem::path inputPath = "data/matrix_input";
    const std::filesystem::path outputPath = "data/matrix_output";
    const std::filesystem::path measurementPath = "data/measurements";

    std::filesystem::create_directories(outputPath);
    std::filesystem::create_directories(measurementPath);
    
    for (const auto& fileEntry : std::filesystem::directory_iterator(inputPath)) {
        if (!fileEntry.is_regular_file()) {
            continue;
        }

        const std::string filename = fileEntry.path().filename().string();
        const std::string suffix = "_1.txt";
        if (filename.size() <= suffix.size() ||
            filename.substr(filename.size() - suffix.size()) != suffix) {
            continue;
        }

        const auto pos = filename.find('_');
        const int n = std::stoi(filename.substr(0, pos));

        const std::string base = filename.substr(0, filename.size() - suffix.size());
        const std::filesystem::path filepath2 = inputPath / (base + "_2.txt");
        if (!std::filesystem::exists(filepath2)) {
            std::cerr << "Falta la segunda matriz para: " << base << '\n';
            continue;
        }

        int** A = readFromFile(fileEntry.path(), n);
        int** B = readFromFile(filepath2, n);
        int** C_naive = createMatrix(n);
        int** C_strassen = createMatrix(n);

        const Metrics m_naive = medirPerformance([&]() {
            naive(A, B, C_naive, n);
        });
        const Metrics m_strassen = medirPerformance([&]() {
            strassen(A, B, C_strassen, n);
        });

        const bool correcto = sameMatrix(C_naive, C_strassen, n);
        if (!correcto) {
            std::cerr << "Error: resultados distintos en " << base << '\n';
        }

        saveMeasurement(base, measurementPath / "naive.csv", m_naive);
        saveMeasurement(base, measurementPath / "strassen.csv", m_strassen);
        saveMatrixToFile(outputPath / (base + "_out.txt"), C_strassen, n);

        std::cout << base << " (n=" << n << ")\n"
                  << "  Naive: " << m_naive.time_us << " us\n"
                  << "  Strassen: " << m_strassen.time_us << " us\n"
                  << "  Resultado correcto: " << std::boolalpha << correcto << '\n';

        deleteMatrix(A, n);
        deleteMatrix(B, n);
        deleteMatrix(C_naive, n);
        deleteMatrix(C_strassen, n);
    }

    return 0;
}