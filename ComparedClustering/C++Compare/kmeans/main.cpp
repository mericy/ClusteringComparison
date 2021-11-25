#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "KMeans.hpp"

int main(int argc, char **argv)
{
    if (argc != 5)
    {
        cout << "Two parameters needed i.e; input_file output_file cluster_size max_iter" << endl;
        return 0;
    }

    vector<Point> points;
    string filename = argv[1];
    string outfilename = argv[2];

    ifstream waveFile(filename);
    string line;
    int dim = 8;

    while (getline(waveFile, line))
    {
        vector<float> wave;
        wave.push_back(stof(line));
        for (int j = 0; j < dim - 1; j++)
        {
            getline(waveFile, line);
            wave.push_back(stof(line));
        }
        Point point(wave);
        points.push_back(point);
    }

    int cluster = stoi(argv[3]);
    int epoch = stoi(argv[4]);

    KMeans km = KMeans(points, cluster, epoch);

    KMeansResults results = km.run(true);

    std::cout << "Finished " << results.epoch << " epoch in " << (float)results.time / 1000 << " secs." << std::endl;

    ofstream out(outfilename);
    for (int i = 0; i < cluster; i++)
    {
        for (int j = 0; j < dim - 1; j++)
        {
            out << results.iter_centroids[i].values[j] << " ";
        }
        out << results.iter_centroids[i].values[dim - 1] << std::endl;
    }
    out.close();
    return 0;
}
