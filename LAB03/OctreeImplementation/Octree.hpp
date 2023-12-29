#ifndef __OCTREE_H__
#define __OCTREE_H__

// VTK libraries
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkCubeSource.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>

#include "Point.hpp"
#include <iostream>
#include <vector>

enum Position {
    LFB,
    LFT,
    LBB,
    LBT,
    RFB,
    RFT,
    RBB,
    RBT
};

class Octree {
private:
    // Octantes 
    std::vector<Octree*> children;
    // Puntos por nodo
    int nPoints;
    std::vector<Point> points;
    // Bandera si es Nodo Interno
    bool isInternal;
    // Punto BottomLeftFront y h
    // Para representar espacio
    Point BottomLeftFront;
    double h;

public:
    Octree(int np);
    Octree(const Point& p, double height, int np);
    ~Octree();
    void setOctant(const Point& p, int& oct);
    bool exists(const Point& p);
    void insert(const Point& p);
    // Para renderizar octree con VTK
    vtkSmartPointer<vtkActor> createCubeActor();
    void visualizeOctree(vtkSmartPointer<vtkRenderer> renderer);
    void visualizeOctreeNode(vtkSmartPointer<vtkRenderer> renderer, Octree* node);
};

#endif