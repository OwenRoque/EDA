#include "Octree.hpp"

Octree::Octree(int np) : nPoints(np), BottomLeftFront(Point()), h(0.0), isInternal(false) {
    points.reserve(nPoints);
}

Octree::Octree(const Point& p, double height, int np) : 
    nPoints(np), BottomLeftFront(p), h(height), isInternal(true) {
    points.reserve(nPoints);
    // Crear octantes
    children.assign(8, nullptr);
    for (int i = LFB; i <= RBT; i++)
        children[i] = new Octree(np);
}

Octree::~Octree() {
    for (auto& child : children) {
        delete child;
        child = nullptr; // Asignar nullptr despues de la eliminacion
    }
}

void Octree::setOctant(const Point& p, int& octPos) {
    // Si excede limites del espacio
    if (p.x < BottomLeftFront.x || p.x > BottomLeftFront.x + h || 
        p.y < BottomLeftFront.y || p.y > BottomLeftFront.y + h || 
        p.z < BottomLeftFront.z || p.z > BottomLeftFront.z + h) {
        // std::cout << p.x << " < " << BottomLeftFront.x << " | " << p.x << " > " << BottomLeftFront.x + h << "\n";
        // std::cout << p.y << " < " << BottomLeftFront.y << " | " << p.y << " > " << BottomLeftFront.y + h << "\n";
        // std::cout << p.z << " < " << BottomLeftFront.z << " | " << p.z << " > " << BottomLeftFront.z + h << "\n";
        octPos = -1;
        return;
    }
    // Obtener coordenadas del centro del octante actual
    double midx = (BottomLeftFront.x + BottomLeftFront.x + h) / 2.0,
        midy = (BottomLeftFront.y + BottomLeftFront.y + h) / 2.0,
        midz = (BottomLeftFront.z + BottomLeftFront.z + h) / 2.0;
    // Algoritmo para determinar el octante
    if (p.x <= midx) {              // x: Left 
        if (p.y <= midy) {          // y: Front
            if (p.z <= midz)        // z: Bottom
                octPos = LFB;
            else                    // z: Top
                octPos = LFT;
        }
        else {                      // y: Back 
            if (p.z <= midz)        // z: Bottom 
                octPos = LBB;
            else                    // z: Top
                octPos = LBT;
        }
    }
    else {                          // x: Right
        if (p.y <= midy) {          // y: Front
            if (p.z <= midz)        // z: Bottom    
                octPos = RFB;
            else                    // z: Top
                octPos = RFT;
        }
        else {                      // y: Back
            if (p.z <= midz)        // z: Bottom
                octPos = RBB;
            else                    // z: Top
                octPos = RBT;
        }
    }
    return;
}

bool Octree::exists(const Point& p) {
    int octPos;
    setOctant(p, octPos);
    if (octPos == -1)
        return false;
    // Busqueda segun los casos
    // Si es un nodo interno, seguir buscando en los hijos
    if (children[octPos]->isInternal)
        return children[octPos]->exists(p);
    // Si es un nodo hoja y esta vacio
    else if(children[octPos]->points.empty()){
        return false;
    }
    // Si no esta vacio, busqueda en el vector de Puntos
    else {
        for (auto point : children[octPos]->points) {
            if(p.x == point.x && p.y == point.y && p.z == point.z)
                return true;
        }
    }
    // Si no se encuentra en el vector de Puntos
    return false;
}

void Octree::insert(const Point& p) {
    if (exists(p)) {
        std::cout << "(" << p.x << ", " << p.y << ", " << p.z << ") Punto ya existe." << std::endl;
        return;
    }
    int octPos;
    setOctant(p, octPos);
    if (octPos == -1) {
        std::cout << "(" << p.x << ", " << p.y << ", " << p.z << ") Fuera del Limite." << std::endl;
        return;
    }
    // Insercion segun los casos
    // Si es un nodo interno
    if (children[octPos]->isInternal) {
        children[octPos]->insert(p);
        return;
    }
    // Es un nodo hoja, si aun hay espacio en el vector de Puntos
    else if (children[octPos]->points.size() < nPoints) {
        children[octPos]->points.push_back(p);
        // Guardar valores de BottomLeftFront y h
        switch (octPos) {
        case LFB:
            children[octPos]->BottomLeftFront = BottomLeftFront;
            break;
        case LFT:
            children[octPos]->BottomLeftFront = Point(BottomLeftFront.x, BottomLeftFront.y, BottomLeftFront.z + h/2);
            break;
        case LBB:
            children[octPos]->BottomLeftFront = Point(BottomLeftFront.x, BottomLeftFront.y + h/2, BottomLeftFront.z);
            break;
        case LBT:
            children[octPos]->BottomLeftFront = Point(BottomLeftFront.x, BottomLeftFront.y + h/2, BottomLeftFront.z + h/2);
            break;
        case RFB:
            children[octPos]->BottomLeftFront = Point(BottomLeftFront.x + h/2, BottomLeftFront.y, BottomLeftFront.z);
            break;
        case RFT:
            children[octPos]->BottomLeftFront = Point(BottomLeftFront.x + h/2, BottomLeftFront.y, BottomLeftFront.z + h/2);
            break;
        case RBB:
            children[octPos]->BottomLeftFront = Point(BottomLeftFront.x + h/2, BottomLeftFront.y + h/2, BottomLeftFront.z);
            break;
        case RBT:
            children[octPos]->BottomLeftFront = Point(BottomLeftFront.x + h/2, BottomLeftFront.y + h/2, BottomLeftFront.z + h/2);
            break;
        }
        children[octPos]->h = h/2;
        std::cout << "(" << p.x << ", " << p.y << ", " << p.z << ") Insertado." << std::endl;
        return;
    }
    // Si no hay espacio disponible, dividir en un nuevo Octree, convertir en nodo interno
    else {
        std::vector<Point> temp = children[octPos]->points;
        std::cout << "Split: ";
        for (auto tp : temp) std::cout << "(" << tp.x << ", " << tp.y << ", " << tp.z << ") ";
        std::cout << "A Reinsertar." << std::endl;
        delete children[octPos];
        children[octPos] = nullptr;
        switch (octPos) {
        case LFB:
            children[octPos] = new Octree(BottomLeftFront, h/2, nPoints);
            break;
        case LFT:
            children[octPos] = new Octree(Point(BottomLeftFront.x, BottomLeftFront.y, BottomLeftFront.z + h/2), h/2, nPoints);
            break;
        case LBB:
            children[octPos] = new Octree(Point(BottomLeftFront.x, BottomLeftFront.y + h/2, BottomLeftFront.z), h/2, nPoints);
            break;
        case LBT:
            children[octPos] = new Octree(Point(BottomLeftFront.x, BottomLeftFront.y + h/2, BottomLeftFront.z + h/2), h/2, nPoints);
            break;
        case RFB:
            children[octPos] = new Octree(Point(BottomLeftFront.x + h/2, BottomLeftFront.y, BottomLeftFront.z), h/2, nPoints);
            break;
        case RFT:
            children[octPos] = new Octree(Point(BottomLeftFront.x + h/2, BottomLeftFront.y, BottomLeftFront.z + h/2), h/2, nPoints);
            break;
        case RBB:
            children[octPos] = new Octree(Point(BottomLeftFront.x + h/2, BottomLeftFront.y + h/2, BottomLeftFront.z), h/2, nPoints);
            break;
        case RBT:
            children[octPos] = new Octree(Point(BottomLeftFront.x + h/2, BottomLeftFront.y + h/2, BottomLeftFront.z + h/2), h/2, nPoints);
            break;
        }
        // Reinsertar vector de puntos en sus octantes correspondientes
        for (auto tp : temp)
            children[octPos]->insert(tp);
        // Insertar punto de entrada
        children[octPos]->insert(p);
    }
}

vtkSmartPointer<vtkActor> Octree::createCubeActor() {
    vtkNew<vtkNamedColors> colors;
    // Crear el cubo
    vtkSmartPointer<vtkCubeSource> cubeSource = vtkSmartPointer<vtkCubeSource>::New();
    double bounds[6] = {
        BottomLeftFront.x, BottomLeftFront.x + h,
        BottomLeftFront.y, BottomLeftFront.y + h,
        BottomLeftFront.z, BottomLeftFront.z + h
    };
    cubeSource->SetBounds(bounds);
    // Mapper
    vtkSmartPointer<vtkPolyDataMapper> cubeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    cubeMapper->SetInputConnection(cubeSource->GetOutputPort());
    // Actor
    vtkSmartPointer<vtkActor> cubeActor = vtkSmartPointer<vtkActor>::New();
    cubeActor->SetMapper(cubeMapper);
    cubeActor->GetProperty()->SetColor(colors->GetColor3d("chrome_oxide_green").GetData());
    return cubeActor;
}

void Octree::visualizeOctree(vtkSmartPointer<vtkRenderer> renderer)
{
    std::cout << "Renderizando Octree . . . " << std::endl;
    visualizeOctreeNode(renderer, this);
    std::cout << "Finalizó la renderización del Octree." << std::endl;
}

void Octree::visualizeOctreeNode(vtkSmartPointer<vtkRenderer> renderer, Octree *node)
{
    // Buscar recursivamente los nodos hoja y crear los cubos
    if (node->isInternal) {
        // std::cout << "h: " << node->h << std::endl;
        // std::cout << "BLF: " << node->BottomLeftFront.x << " " << node->BottomLeftFront.y << " " << node->BottomLeftFront.z << std::endl;
        // std::cout << "Internal Not Rendering." << std::endl;
        for (const auto& child : node->children) {
            visualizeOctreeNode(renderer, child);
        }
            
    } else {
        // Si hay informacion en el nodo hoja
        if (!(node->points.empty())) {
            // std::cout << "h: " << node->h << std::endl;
            // std::cout << "BLF: " << node->BottomLeftFront.x << " " << node->BottomLeftFront.y << " " << node->BottomLeftFront.z << std::endl;
            vtkSmartPointer<vtkActor> cubeActor = node->createCubeActor();
            renderer->AddActor(cubeActor);
            // std::cout << "Added cube actor." << std::endl;
        }
    }
}
