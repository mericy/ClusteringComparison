#include "KMeans.hpp"

#include <chrono>
#include <iostream>
#include <tuple>
#include <vector>

bool KMeans::seeded = false;

KMeansResults KMeans::run(bool debug)
{
    //Randomly select k centroids from point list
    std::vector<Point> centroids;

    int pointCount = (int)points.size();
    int dimensionSize = (int)points[0].values.size();

    std::vector<int> used_point_idx;

    srand((unsigned int)time(NULL));

    for (int i = 0; i < K; i++)
    {
        while (true)
        {
            int index = rand() % pointCount;
            if (std::find(used_point_idx.begin(), used_point_idx.end(), index) == used_point_idx.end())
            {
                points[index].cluster = i;
                points[index].minimumDistance = 0;
                centroids.push_back(points[index]);
                used_point_idx.push_back(index);
                break;
            }
        }
    }
    std::vector<int> previous_clusters;
    for (int i = 0; i < points.size(); i++)
    {
        previous_clusters.push_back(points[i].cluster);
    }
    std::vector<double> emptyVector;
    for (int i = 0; i < dimensionSize; i++)
    {
        emptyVector.push_back(0.0);
    }

    int curEpoch = 1;
    int indis = 0;
    bool done;
    std::chrono::steady_clock::time_point begin, end;
    int64_t time = 0;

    while (curEpoch <= epochs)
    {
        done = true;
        if (debug)
        {
            std::cout << "\rEpoch\t" << curEpoch << "/" << epochs;
        }
        begin = std::chrono::steady_clock::now();

        for (int j = 0; j < points.size(); j++)
        {
            Point p = points[j];
            int prev_cluster = p.cluster;
            for (int i = 0; i < K; i++)
            {
                double distance = centroids[i].distance(p);
                if (distance < p.minimumDistance)
                {
                    p.minimumDistance = distance;
                    p.cluster = i;
                }
            }
            if (p.cluster != prev_cluster)
            {
                done = false;
            }
            points[j] = p;
        }

        //Update centroids.
        std::vector<int> nPoints;
        std::vector<std::vector<double>> dimensionSums;
        for (int i = 0; i < K; i++)
        {
            nPoints.push_back(0);
            dimensionSums.push_back(emptyVector);
        }

        for (int i = 0; i < points.size(); i++)
        {
            int clusterId = points[i].cluster;
            nPoints[clusterId] += 1;

            for (int j = 0; j < dimensionSize; j++)
            {
                dimensionSums[clusterId][j] += points[i].values[j];
            }
            points[i].minimumDistance = std::numeric_limits<double>::max();
        }

        for (int i = 0; i < K; i++)
        {
            for (int j = 0; j < dimensionSize; j++)
            {
                if (nPoints[i] > 0)
                {
                    centroids[i].values[j] = dimensionSums[i][j] / nPoints[i];
                }
            }
        }

        if (done == true)
        {
            break;
        }

        end = std::chrono::steady_clock::now();
        time += std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
        std::cout << "\t" << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
        std::cout << std::endl;
        curEpoch++;
    }
    if (debug)
    {
        std::cout << "Finished in " << curEpoch - 1 << " epochs." << std::endl;
    }
    return KMeansResults(curEpoch - 1, time, centroids);
}

bool KMeans::isConverged(std::vector<int> *prev_clusters)
{
    for (int i = 0; i < points.size(); i++)
    {
        if (prev_clusters->at(i) != points[i].cluster)
        {
            return false;
        }
    }
    return true;
}

std::vector<std::vector<float>> KMeans::unpackPointVector(std::vector<Point> pts)
{
    std::vector<std::vector<float>> unpacked;
    for (int i = 0; i < pts.size(); i++)
    {
        unpacked.push_back(pts[i].values);
    }
    return unpacked;
}