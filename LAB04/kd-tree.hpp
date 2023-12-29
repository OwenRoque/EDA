#ifndef __KD_TREE__H_
#define __KD_TREE__H_

#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <limits>
#include <algorithm>

struct Point {
    double x, y, z;
    Point() = default;
    Point(double a, double b, double c) : x(a), y(b), z(c) {}
};

// Nodo del Kd-Tree
struct Node {
    Point point;
    Node* left;
    Node* right;

    Node(const Point& p) : point(p), left(nullptr), right(nullptr) {}
};

class CompareDistance {
public:
    bool operator()(const std::pair<double, Point>& a, const std::pair<double, Point>& b) const {
        // Para que el punto más lejano esté en la parte superior de la cola
        return a.first < b.first;
    }
};

double euclidianDistance(const Point& a, const Point& b);

// Kd-Tree
class KdTree 
{
public:
    // Init Constructor
    KdTree();
    // Destructor
    ~KdTree();
    // Construir KDTree Balanceado
    void buildBalancedTree(std::vector<Point>& points);
    // Metodo insert
    void insert(const Point& point);
    // Busqueda K-Nearest Neighbor (kNN)
    std::vector<std::pair<double, Point>> knnSearch(const Point& target, int k);

private:
    // Nodo raiz 
    Node* root;
    Node* insertRecursive(Node* node, const Point& point, int depth);
    // Para el destructor
    void deleteRecursive(Node* node);
    // Metodo de utilidad para insert
    double pointAtDimension(const Point& point, int dimension);
    // Metodo recursivo para kNN
    void knnSearchRecursive(Node* node, const Point& target, int depth, int k, 
        std::priority_queue<std::pair<double, Point>, 
        std::vector<std::pair<double, Point>>, CompareDistance>& nearestPoints);
    // Calculo de la distancia euclidiana entre puntos
    friend double euclidianDistance(const Point& a, const Point& b);
    // Construir KDTree Balanceado recursivamente
    void buildBalancedKDTree(std::vector<Point>& points, int depth);
    // Funciones de comparación para ordenar puntos en cada dimensión
    static bool comparePointsX(const Point& p1, const Point& p2) {
        return p1.x < p2.x;
    }
    static bool comparePointsY(const Point& p1, const Point& p2) {
        return p1.y < p2.y;
    }
    static bool comparePointsZ(const Point& p1, const Point& p2) {
        return p1.z < p2.z;
    }
};


#endif
