#include "Octree.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <functional>
#include <filesystem>
#include <limits>

namespace fs = std::filesystem;

void consoleOctree() {
    double a, b, c, h;
    int np;
    std::cout << "Dimensiones del Octree:\n";
    std::cout << "Ingrese valores del Punto:\n";
    std::cin >> a >> b >> c;
    std::cout << "Ingrese longitud: ";
    std::cin >> h;
    std::cout << "Numero de Puntos por Nodo: ";
    std::cin >> np;
    Octree* tree = new Octree(Point(a, b, c), h, np);

    int opt;
    do {
        std::cout << "====Operaciones====" << std::endl;
        std::cout << " (0) Salir" << std::endl;
        std::cout << " (1) Insertar Punto" << std::endl;
        std::cout << " (2) Buscar Punto" << std::endl;
        std::cin >> opt;
        switch (opt) {
            case 0:
                break;
            case 1:
            {
                double x, y, z;
                std::cout << "Ingrese valores del Punto a insertar:" << std::endl;
                std::cin >> x >> y >> z;
                tree->insert(Point(x, y, z));
                break;
            }
            case 2:
            {
                double x, y, z;
                std::cout << "Ingrese valores del Punto a buscar:" << std::endl;
                std::cin >> x >> y >> z;
                std::cout << (tree->exists(Point(x, y, z)) 
                    ? "El punto existe.\n" : "El punto no existe.\n");
                break;
            }
            default:
                continue;
        }
        system("pause");
        system("cls");
    }
    while (opt != 0);
    // 3, 3, 3
    // 3, 3, 4
    // 3, 4, 4
    // 4, 3, 4
    delete tree;
}

void printOctree() {
    // Leer puntos desde un archivo CSV
    std::string csvFilename, ext = ".csv"; int np;
    std::cout << "Ingrese el nombre del archivo CSV con puntos: ";
    std::cin >> csvFilename;
    std::cout << "Numero de Puntos por Nodo: ";
    std::cin >> np;
    csvFilename += ext;
    fs::path currentPath = fs::current_path();
    fs::path parentPath = currentPath.parent_path().parent_path();
    fs::path fullPath = parentPath / "data" / csvFilename;

    std::ifstream inputFile;
    inputFile.open(fullPath);
    if (!inputFile.is_open()) {
        std::cerr << "Error al abrir el archivo." << std::endl;
        return;
    }
    // Algoritmo Bulk-Insert en Octree
    // Guardar puntos
    std::string line;
    std::vector<Point> csvPoints;
    std::cout << "Leyendo Puntos . . . " << std::endl;
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        int x, y, z; char delim;
        if (iss >> x >> delim >> y >> delim >> z) 
            csvPoints.push_back(Point(x ,y, z));
        else {
            std::cerr << "Error durante la lectura de puntos. Revise el archivo CSV y vuelva a intentarlo.";
            system("pause");
            return;
        }
    }
    inputFile.close();
    std::cout << "Hecho . . . " << std::endl;

    // Calcular punto BottomLeftFront y longitud del cubo
    double minX, minY, minZ, maxX, maxY, maxZ;
    minX = minY = minZ = std::numeric_limits<int>::max();
    maxX = maxY = maxZ = std::numeric_limits<int>::min();

    for (const auto& punto : csvPoints) {
        minX = std::min(minX, punto.x);
        minY = std::min(minY, punto.y);
        minZ = std::min(minZ, punto.z);
        maxX = std::max(maxX, punto.x);
        maxY = std::max(maxY, punto.y);
        maxZ = std::max(maxZ, punto.z);
    }

    double length = static_cast<double>(std::max({ std::abs(maxX - minX), std::abs(maxY - minY), std::abs(maxZ - minZ) }));
    // Caso solo se ingrese 1 punto, length no puede ser 0
    if (length == 0.0) length = 1.0;
    Point BLF { minX, minY, minZ };

    std::cout << "h: " << length << std::endl;
    std::cout << "BLF: " << BLF.x << " " << BLF.y << " " << BLF.z << std::endl;

    // Construir Octree
    Octree* tree = new Octree(BLF, length, np);

    // std::cout << "Insertando Puntos . . . " << std::endl;
    inputFile.open(fullPath);
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        int x, y, z; char delim;
        if (!(iss >> x >> delim >> y >> delim >> z)) {
            std::cerr << "Error al leer línea del archivo CSV\n";
            break;
        }
        // Insertar el punto en el Octree
        tree->insert(Point(x, y, z));
    }
    std::cout << "Hecho . . . " << std::endl;
    // VTK configuration
    vtkNew<vtkNamedColors> colors;
    // Create a rendering window and renderer.
    vtkNew<vtkRenderer> ren;
    vtkNew<vtkRenderWindow> renWin;
    renWin->SetWindowName("OctreeVisualization");
    renWin->AddRenderer(ren);
    // Create a renderwindow interactor.
    vtkNew<vtkRenderWindowInteractor> iren;
    iren->SetRenderWindow(renWin);

    // Visualizar Octree
    tree->visualizeOctree(ren);

    // Configuraciones adicionales del renderer
    ren->ResetCamera();
    ren->GetActiveCamera()->Azimuth(30);
    ren->GetActiveCamera()->Elevation(30);
    ren->ResetCameraClippingRange();
    ren->SetBackground(colors->GetColor3d("Silver").GetData());
    renWin->SetSize(600, 600);

    // Enable user interface interaction.
    iren->Initialize();
    renWin->Render();
    iren->Start();

    delete tree;
}

int main(int argc, char const *argv[]) {
    std::vector<std::function<void()>> options;
    options.emplace_back(consoleOctree);
    options.emplace_back(printOctree);

    int opt;
    std::cout << "[0] Crear/Insertar/Buscar Octree" << std::endl;
    std::cout << "[1] Visualizar Octree con Mapa de Puntos" << std::endl;
    std::cin >> opt;
    options[opt]();

    return EXIT_SUCCESS;
}
