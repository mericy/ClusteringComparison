#include <algorithm>
#include <iostream>
#include <tuple>
#include <chrono>

#include "Point.hpp"
#include "Cluster.hpp"
#define returnStruct std::tuple<std::vector<Point>, int64_t>

/*
https://www.researchgate.net/publication/27661047_Pairwise_Nearest_Neighbor_Method_Revisited
Page : 10, Section 3.1
*/
class Pnn
{
private:
    int M;
    std::vector<std::vector<float>> distanceMatrix;

    float calculateDistance(Cluster c1, Cluster c2);
    std::pair<int, int> getNearestClusters();
    void mergeClusters(int c1_id, int c2_id);
    void InitDistanceMatrix();
    void updateDistanceMatrix(int c1_id, int c2_id);
    std::vector<Point> getCentroids();

public:
    std::vector<Point> points;
    std::vector<Cluster> clusters;
    Pnn(std::vector<Point> points, int M) : points(points),
                                            M(M){};
    returnStruct run();
};

returnStruct Pnn::run()
{
    int m = points.size();
    std::chrono::steady_clock::time_point begin, end; //, begin_loop, end_loop,endDist;

    begin = std::chrono::steady_clock::now();
    //Initialize all points as clusters.
    for (int i = 0; i < points.size(); i++)
    {
        Cluster c = Cluster();
        c.clusterID = i;
        c.size = 1;
        points[i].cluster = i;
        c.points.push_back(points[i]);
        c.centroid = points[i];

        clusters.push_back(c);
    }
    InitDistanceMatrix();
    // endDist = std::chrono::steady_clock::now();
    // std::cout << "Distance matrix initialization finished in " << std::chrono::duration_cast<std::chrono::seconds>(endDist - begin).count() <<" seconds"<<std::endl;
    // begin_loop = std::chrono::steady_clock::now();
    while (m != M)
    {
        std::pair<int, int> cids = getNearestClusters();
        int c1_id = cids.first;
        int c2_id = cids.second;
        mergeClusters(c1_id, c2_id);
        updateDistanceMatrix(c1_id, c2_id);

        // if (m % 500 == 0)
        // {
        // 	end_loop = std::chrono::steady_clock::now();
        // 	std::cout << "\r " << m << "-> end " << M <<" | "<< std::chrono::duration_cast<std::chrono::seconds>(end_loop - begin_loop).count()<< "      ";
        // 	begin_loop = end_loop;
        // }
        m -= 1;
    }
    end = std::chrono::steady_clock::now();
    int64_t time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();

    return {getCentroids(), time};
}
float Pnn::calculateDistance(Cluster c1, Cluster c2)
{
    int n_a = c1.size;
    int n_b = c2.size;

    float distance = c1.centroid.distance(c2.centroid);

    return (n_a * n_b * distance) / (n_a + n_b);
}
void Pnn::InitDistanceMatrix()
{
    std::chrono::steady_clock::time_point begin1, end1;
    // begin1 = std::chrono::steady_clock::now();
    std::vector<std::vector<float>> newDistance;
    for (int i = 0; i < clusters.size(); i++)
    {
        //Update cluster ID's too.
        clusters[i].clusterID = i;
        std::vector<float> rowDistance;
        for (int j = i + 1; j < clusters.size(); j++)
        {
            float distance = calculateDistance(clusters[i], clusters[j]);
            rowDistance.push_back(distance);
        }
        // if (i % 1000 == 0)
        // {
        //     end1 = std::chrono::steady_clock::now();
        //     std::cout << i << "/" << clusters.size() << " " << std::chrono::duration_cast<std::chrono::seconds>(end1 - begin1).count() << " seconds" << std::endl;
        //     begin1 = end1;
        // }
        newDistance.push_back(rowDistance);
    }
    distanceMatrix = newDistance;
}

void Pnn::updateDistanceMatrix(int c1_id, int c2_id)
{
    //Update C1 Distances
    for (int i = 0; i < c1_id; i++)
    {
        int inneridx = c1_id - i - 1;
        distanceMatrix[i][inneridx] = calculateDistance(clusters[c1_id], clusters[i]);
    }
    //Update c1_id
    std::vector<float> rowDistance;
    for (int j = c1_id + 1; j < clusters.size(); j++)
    {
        rowDistance.push_back(calculateDistance(clusters[c1_id], clusters[j]));
    }
    distanceMatrix[c1_id] = rowDistance;

    //Delete C2 Distances
    for (int i = 0; i < c2_id; i++)
    {
        if (i == c1_id)
        {
            continue;
        }
        else
        {
            int inneridx = c2_id - i - 1;
            distanceMatrix[i].erase(distanceMatrix[i].begin() + inneridx);
        }
    }
    //Delete C2 Row
    distanceMatrix.erase(distanceMatrix.begin() + c2_id);

    //Update ID's
    for (int i = c2_id; i < clusters.size(); i++)
    {
        clusters[i].clusterID = i - 1;
    }
}

std::pair<int, int> Pnn::getNearestClusters()
{
    float distance = std::numeric_limits<float>::max();
    int c1_id, c2_id;
    for (int i = 0; i < distanceMatrix.size(); i++)
    {
        for (int j = 0; j < distanceMatrix[i].size(); j++)
        {
            if (distanceMatrix[i][j] < distance)
            {
                c1_id = i;
                c2_id = i + j + 1;
                distance = distanceMatrix[i][j];
            }
        }
    }

    return {c1_id, c2_id};
}

void Pnn::mergeClusters(int c1_id, int c2_id)
{

    int c1_size = clusters[c1_id].size;
    int c2_size = clusters[c2_id].size;

    Point c1_centroid = clusters[c1_id].centroid;
    Point c2_centroid = clusters[c2_id].centroid;

    std::vector<float> newVals;
    for (int i = 0; i < clusters[c1_id].centroid.dimension; i++)
    {
        float value = (c1_size * c1_centroid.values[i] + c2_size * c2_centroid.values[i]) / (c1_size + c2_size);
        newVals.push_back(value);
    }
    Point newCentroid = Point(newVals);
    newCentroid.cluster = clusters[c1_id].clusterID;

    clusters[c1_id].size += clusters[c2_id].size;
    clusters[c1_id].points.insert(clusters[c1_id].points.end(), clusters[c2_id].points.begin(), clusters[c2_id].points.end());
    clusters[c1_id].centroid = newCentroid;

    clusters.erase(clusters.begin() + c2_id);
}

std::vector<Point> Pnn::getCentroids()
{
    std::vector<Point> centroids;

    for (int i = 0; i < clusters.size(); i++)
    {
        centroids.push_back(clusters[i].centroid);
    }
    return centroids;
}