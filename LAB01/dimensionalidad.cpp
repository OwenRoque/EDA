#include <iostream>
#include <random>
#include <cmath>
#include <vector>
#include <fstream>

// Method for calculating the Euclidean distance between two points
double euclidean(const std::vector<double>& a, const std::vector<double>& b)
{
    double distance = 0.0;
    for (size_t i = 0; i < a.size(); i++)
    {
        double diff = a.at(i) - b.at(i);
        distance += pow(diff, 2);
    }
    return sqrt(distance);
}

int main()
{
    // Will be used to obtain a seed for the random number engine
    std::random_device rd;
    // Standard mersenne_twister_engine seeded with rd()
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 1.0);
    // Dimensions
    const std::vector<int> dimensions = {10, 50, 100, 500, 1000, 2000, 5000};
    const int npoints = 100;
    
    for (int d : dimensions)
    {
        std::vector<std::vector<double>> points(npoints, std::vector<double>(d));

        // Generate numbers between 0 and 1
        for (int i = 0; i < npoints; ++i)
            for (int j = 0; j < d; ++j)
                points[i][j] = dis(gen);

        std::ofstream file("distances_dim_" + std::to_string(d) + ".txt");
        for (int i = 0; i < npoints; ++i)
            for (int j = i + 1; j < npoints; ++j) {
                double distance = euclidean(points[i], points[j]);
                file << distance << "\n";
            }
        
        file.close();
    }
    return 0;
}
