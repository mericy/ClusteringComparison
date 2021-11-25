// #include <iostream>
#include <limits>
#include <vector>

#ifndef POINT_HPP
#define POINT_HPP

struct Point {
    std::vector<float> values;
    int cluster;
    double minimumDistance;

    Point(std::vector<float> dims) : values(dims),
                                     cluster(-1),
                                     minimumDistance(std::numeric_limits<float>::max()){};

    Point(size_t dims) : cluster(-1),
                         minimumDistance(std::numeric_limits<float>::max()) {
        for (size_t i = 0; i < dims; i++) {
            values.push_back(0);
        }
    };

    double distance(Point p) {
        double distance = 0;
        for (size_t i = 0; i < values.size(); i++) {
            distance += (p.values[i] - values[i]) * (p.values[i] - values[i]);
        }
        return sqrt(distance);
    }

    void set_zero() {
        for (size_t i = 0; i < values.size(); i++) {
            values[i] = 0;
        }
    }

    Point &operator+=(const Point &p) {
        for (size_t i = 0; i < values.size(); i++) {
            values[i] += p.values[i];
        }
        return *this;
    }

    Point &operator/=(const float &d) {
        for (size_t i = 0; i < values.size(); i++) {
            values[i] /= d;
        }
        return *this;
    }

    Point &operator*=(const float &d) {
        for (size_t i = 0; i < values.size(); i++) {
            values[i] *= d;
        }
        return *this;
    }

    friend Point operator*(Point lhs, const float &rhs) {
        lhs *= rhs;
        return lhs;
    }

};

#endif