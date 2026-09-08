#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <sstream>
#include <sys/resource.h>
#include <chrono>


#include "algorithms/naive.hpp"
#include "algorithms/strassen.hpp"


struct Metrics{
    int64_t time_ms;
    long    mem_kb;
};

template<typename Funcion>
Metrics medirPerformance(Funcion f) {
    struct rusage ru_before, ru_after;
    getrusage(RUSAGE_SELF, &ru_before);

    auto start = std::chrono::high_resolution_clock::now();
    f();
    auto end   = std::chrono::high_resolution_clock::now();

    getrusage(RUSAGE_SELF, &ru_after);

    Metrics m;
    m.time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    m.mem_kb   = ru_after.ru_maxrss - ru_before.ru_maxrss;
    return m;
}


int** readFromFile (const std::filesystem::path& filepath, int n) {
    int** A = createMatrix(n);

    std::ifstream inputFile(filepath);

    int num;
    std::string lineRow;
    for (int i = 0; getline(inputFile, lineRow); i++) {
        std::stringstream ss(lineRow);
        for (int j = 0; ss >> num; j++) {
            A[i][j] = num;
        }
    }

    inputFile.close();
    return A;
}

void saveMatrixToFIle (const std::filesystem::path& filepath, const int** A, int n) {
    std::ofstream outputFile(filepath);

    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++) {
            outputFile << A[i][j] <<" ";
        }
        outputFile << std::endl;
    }

    outputFile.close();
    return;
}


void saveMeasurement(const std::string& entrada,
    const std::filesystem::path& filepath,
    const Metrics medicion)
{
    std::ofstream measureFile(filepath, std::ios::app);
    measureFile << entrada << ";"
    << medicion.time_ms << ";"
    << medicion.mem_kb << "\n";
    measureFile.close();
}


int main() {
    std::filesystem::path inputPath = "./data/matrix_input"; 
    std::filesystem::path outputPath = "./data/matrix_output/";
    std::filesystem::path measurementPath = "./data/measurements/";
    
    for (auto& fileEntry :  std::filesystem::directory_iterator(inputPath)){
        std::string filename = fileEntry.path().filename().string();
        auto pos  = filename.find('_');              // busca el primer “_”
        int n = std::stoi(filename.substr(0, pos));

        if (filename.substr(filename.size()-6) == "_1.txt") {
            std::string base = filename.substr(0, filename.size()-6);
            std::string ofileName = base + "_out.txt";

            std::string fileext2 = base + "_2.txt";
            std::filesystem::path filepath2 = inputPath / fileext2;

            int** A = readFromFile(fileEntry.path(), n);
            int** B = readFromFile(filepath2, n);


            int** C_naive = createMatrix(n);
            int** C_strassen = createMatrix(n);

            // Medir y guardar naive
            Metrics m_naive = medirPerformance([&]() {
                naive(A, B, C_naive, n);
            });
            saveMeasurement(base, measurementPath / "naive.csv", m_naive);

            // Medir y guardar Strassen
            Metrics m_strassen = medirPerformance([&]() {
                strassen(A, B, C_strassen, n);
            });
            saveMeasurement(base, measurementPath / "strassen.csv", m_strassen);
            
            saveMatrixToFIle(outputPath / (base + "_out.txt"), const_cast<const int**>(C_strassen), n);
            
            deleteMatrix(A, n);
            deleteMatrix(B, n);
            deleteMatrix(C_naive, n);
            deleteMatrix(C_strassen, n);

        }
        
    }
  return 0;
}