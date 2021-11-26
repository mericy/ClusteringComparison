#include <vector>
#include <limits>
#include <cmath>

#ifndef POINT_HPP
#define POINT_HPP

struct Point
{
    std::vector<float> values;

    int cluster;
    int dimension;
    double minimumDistance;
    Point() : cluster(-1),
              dimension(0){};

    Point(std::vector<float> dims) : values(dims),
                                     cluster(-1),
                                     dimension(dims.size()),
                                     minimumDistance(std::numeric_limits<float>::max()){};

    double distance(Point p)
    {
        double distance = 0;
        for (int i = 0; i < dimension; i++)
        {
            distance += (p.values[i] - values[i]) * (p.values[i] - values[i]);
        }
        return distance;
    }
    //friend ostream& operator<<(ostream& os,const Point&p);
};

//ostream& operator<<(ostream& os,  const Point& p)
//{
//	for (size_t i = 0; i < p::dimension; i++)
//	{
//		os << p.values[i]<< " ";
//	}
//	return os;
//}

#endif