#include <vector>
#include <limits>
#include <cmath>

#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include "Point.hpp"

struct Cluster
{
    int clusterID;
    int size;
    std::vector<Point> points;
    Point centroid;

    Cluster() : size(0){};
};

#endif
