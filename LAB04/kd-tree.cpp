#include "kd-tree.hpp"

KdTree::KdTree() : root(nullptr) {}

KdTree::~KdTree()
{
    deleteRecursive(root);
}

// No implementado (falta corregir funcionamiento)
void KdTree::buildBalancedTree(std::vector<Point> &points)
{
    buildBalancedKDTree(points, 0);
}

void KdTree::insert(const Point &point)
{
    root = insertRecursive(root, point, 0);
    // std::cout << "(" << point.x << ", " << point.y << ", " << point.z << ") Insertado." << std::endl;
}

std::vector<std::pair<double, Point>> KdTree::knnSearch(const Point &target, int k)
{
    // Cola de prioridad con <distancia, Punto>, es max-heap 
    // (aquellos puntos que tengan distancia mas lejana estaran en la cima) 
    std::priority_queue<std::pair<double, Point>, std::vector<std::pair<double, Point>>, CompareDistance> nearestPoints;
    // Iniciar la busqueda desde la raiz
    knnSearchRecursive(root, target, 0, k, nearestPoints);

    std::vector<std::pair<double, Point>> result;
    // Extraer puntos de la cola
    while (!nearestPoints.empty()) {
        result.push_back(nearestPoints.top());
        nearestPoints.pop();
    }
    // Invertir el orden, el mas cercano está en el fondo
    std::reverse(result.begin(), result.end());
    return result;
}

Node *KdTree::insertRecursive(Node *node, const Point &point, int depth)
{
    if (node == nullptr) 
        return new Node(point);

    // Escoger dimension basado en profundidad (x=0, y=1, z=2)
    int currentDim = depth % 3;

    // Comparar el punto con la raiz respecto a la dimension actual
    if (pointAtDimension(point, currentDim) < pointAtDimension(node->point, currentDim))
        node->left = insertRecursive(node->left, point, depth + 1);
    else 
        node->right = insertRecursive(node->right, point, depth + 1);

    return node;
}

void KdTree::deleteRecursive(Node *node) {
    if (node != nullptr) {
        deleteRecursive(node->left);
        deleteRecursive(node->right);
        delete node;
    }
}

double KdTree::pointAtDimension(const Point &point, int dimension)
{
    switch (dimension) {
        case 0: return point.x;
        case 1: return point.y;
        case 2: return point.z;
        // Manejar dimension invalida (improbable)
        default: return 0.0; 
    }
}

void KdTree::knnSearchRecursive(Node *node, const Point &target, int depth, int k, 
    std::priority_queue<std::pair<double, Point>, 
    std::vector<std::pair<double, Point>>, CompareDistance>& nearestPoints)
{
    // Si el nodo actual es nulo
    if (node == nullptr)
        return;
    // Calculo de la dimension actual
    int currentDim = depth % 3;
    // Distancia entre punto objetivo y  punto del nodo
    double distance = euclidianDistance(node->point, target);
    // Agregar a la cola de prioridad
    nearestPoints.push(std::make_pair(distance, node->point));

    // Asegurarse que la cola no exceda el numero de elementos (k)
    // eliminando los candidatos mas lejanos (dist. mas grande)
    if (nearestPoints.size() > k)
        nearestPoints.pop();
    
    // Determinar en que lado/sub-arbol buscar recursivamente
    if (pointAtDimension(target, currentDim) < pointAtDimension(node->point, currentDim))
        knnSearchRecursive(node->left, target, depth + 1, k, nearestPoints);
    else
        knnSearchRecursive(node->right, target, depth + 1, k, nearestPoints);

    // Verificar si es necesario buscar en el sub-arbol contrario
    // en caso existan puntos mas cercanos
    double splitDistance = std::abs(pointAtDimension(target, currentDim) - pointAtDimension(node->point, currentDim));
    // Si la distancia (linea perpendicular a la seccion que no se visito) es menor al candidato actual
    if (splitDistance < nearestPoints.top().first) {
        // Muy probable que hayan mejores candidatos, buscar recursivamente en esta seccion 
        if (pointAtDimension(target, currentDim) < pointAtDimension(node->point, currentDim))
            knnSearchRecursive(node->right, target, depth + 1, k, nearestPoints);
        else
            knnSearchRecursive(node->left, target, depth + 1, k, nearestPoints);
    }
}

void KdTree::buildBalancedKDTree(std::vector<Point> &points, int depth)
{
    if (points.empty())
        return;

    // Selecciona la dimensión actual basada en la profundidad del árbol
    int dimension = depth % 3;

    // Ordena los puntos en la dimensión actual
    switch (dimension) {
        case 0: std::sort(points.begin(), points.end(), comparePointsX); break;
        case 1: std::sort(points.begin(), points.end(), comparePointsY); break;
        case 2: std::sort(points.begin(), points.end(), comparePointsZ); break;
    }

    /// Encuentra la mediana
    size_t medianIndex = points.size() / 2;
    Point medianPoint;

    if (points.size() % 2 == 0) {
        // Si el tamaño es par, toma el valor del punto central izquierdo (sin duplicar)
        medianPoint = points[medianIndex - 1];
        points.erase(points.begin() + medianIndex - 1);
    } else {
        // Si el tamaño es impar, toma el valor central
        medianPoint = points[medianIndex];
        points.erase(points.begin() + medianIndex);
    }

    // Inserta la mediana al KdTree utilizando tu método de inserción
    insert(medianPoint);

    std::vector<Point> firstHalf(points.begin(), points.begin() + medianIndex);
    std::vector<Point> secondHalf(points.begin() + medianIndex, points.end());
    buildBalancedKDTree(firstHalf, depth + 1);
    buildBalancedKDTree(secondHalf, depth + 1);
}

double euclidianDistance(const Point &a, const Point &b)
{
    return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2) + std::pow(a.z - b.z, 2));
}
