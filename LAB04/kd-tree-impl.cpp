#include "kd-tree.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <filesystem>

namespace fs = std::filesystem;

std::vector<Point> kNNBruteForce(const std::vector<Point>& points, const Point& target, int k) {
    // Vector de pares que almacene la distancia y el punto correspondiente
    std::vector<std::pair<double, Point>> distances;

    // Calcular distancias
    for (const auto& point : points) {
        double distance = euclidianDistance(point, target);
        // std::cout << distance << std::endl;
        distances.push_back(std::make_pair(distance, point));
    }

    // Ordenar el vector de distancias en orden ascendente (menor a mayor)
    std::sort(distances.begin(), distances.end(), [](const auto& a, const auto& b) {
        return a.first < b.first;
    });

    // Extraer los k puntos más cercanos del vector ordenado
    std::vector<Point> result;
    for (int i = 0; i < k && i < distances.size(); ++i)
        result.push_back(distances[i].second);

    return result;
}

int main() {
    KdTree tree;
    // Insertar puntos de csv
    std::string csvFilename, ext = ".csv";
    std::cout << "Ingrese nombre del archivo con puntos: ";
    std::getline(std::cin, csvFilename);
    csvFilename += ext;

    fs::path fullPath = fs::current_path() / "TestX" / csvFilename;
    std::ifstream file(fullPath);
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo." << std::endl;
        return 0;
    }
    std::string line;
    std::vector<Point> csvPoints;
    std::cout << "Leyendo Puntos . . . " << std::endl;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        int x, y, z; char delim;
        if (iss >> x >> delim >> y >> delim >> z) 
            csvPoints.push_back(Point(x ,y, z));
        else {
            std::cerr << "Error durante la lectura de puntos. Revise el archivo CSV y vuelva a intentarlo.";
            system("pause");
            return 0;
        }
    }
    file.close();
    std::cout << "Hecho . . . " << std::endl;

    // Insertar los puntos al kd-tree
    auto startInsert = std::chrono::high_resolution_clock::now();
    // for (const auto& point : csvPoints)
    //     tree.insert(point);
    tree.buildBalancedTree(csvPoints);
    auto endInsert = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> durationInsert = endInsert - startInsert;
    std::cout << "Insercion completada en " << durationInsert.count() << " segundos."<< std::endl;
    
    // Buscar los k puntos mas cercanos a un punto especifico
    std::cout << "===Busqueda kNN con K-d Tree===" << std::endl;
    double a, b, c; int k;
    std::cout << "Ingrese coordenadas del punto objetivo:\n";
    std::cin >> a >> b >> c;
    Point targetPoint = { a, b, c };
    std::cout << "Ingrese valor de k: ";
    std::cin >> k;
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::pair<double, Point>> nearestPoints = tree.knnSearch(targetPoint, k);
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Puntos más cercanos a (" << targetPoint.x << ", " << targetPoint.y << ", " << targetPoint.z << "):" << std::endl;
    for (const auto& point : nearestPoints)
        std::cout << "(" << point.second.x << ", " << point.second.y << ", " << point.second.z << ") -> " 
        << point.first << "u." << std::endl;
    std::chrono::duration<double> duration = end - start;
    std::cout << "Tiempo de ejecución (k-d Tree): " << duration.count() << " segundos" << std::endl;

    std::cout << "===Busqueda kNN por fuerza bruta===" << std::endl;
    auto startBruteForce = std::chrono::high_resolution_clock::now();
    std::vector<Point> nearestPointsBF = kNNBruteForce(csvPoints, targetPoint, k);
    auto endBruteForce = std::chrono::high_resolution_clock::now();

    std::cout << "Puntos más cercanos a (" << targetPoint.x << ", " << targetPoint.y << ", " << targetPoint.z << "):" << std::endl;
    for (const Point& point : nearestPointsBF)
        std::cout << "(" << point.x << ", " << point.y << ", " << point.z << ")" << std::endl;
    std::chrono::duration<double> durationBF = endBruteForce - startBruteForce;
    std::cout << "Tiempo de ejecución (fuerza bruta): " << durationBF.count() << " segundos" << std::endl;
    
    const std::vector<int> kvalues = {1, 3, 5, 10, 15, 20, 50, 100, 250, 500};

    // Archivo de salida
    std::ofstream outputFile("tiempos_ejecucion.csv");
    outputFile << "K-valores,KD-Tree,Fuerza Bruta\n";  // Encabezado

    for (int k : kvalues) {
        // Tiempo de ejecución para KD-Tree
        auto startKDTree = std::chrono::high_resolution_clock::now();
        std::vector<std::pair<double, Point>> nearestPoints = tree.knnSearch(targetPoint, k);
        auto endKDTree = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> durationKDTree = endKDTree - startKDTree;

        // Tiempo de ejecución para Fuerza Bruta
        auto startBruteForce = std::chrono::high_resolution_clock::now();
        std::vector<Point> nearestPointsBF = kNNBruteForce(csvPoints, targetPoint, k);
        auto endBruteForce = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> durationBF = endBruteForce - startBruteForce;

        // Escribir en el archivo CSV
        outputFile << k << "," << durationKDTree.count() << "," << durationBF.count() << "\n";
    }

    // Cerrar el archivo
    outputFile.close();
    
    return 0;
}