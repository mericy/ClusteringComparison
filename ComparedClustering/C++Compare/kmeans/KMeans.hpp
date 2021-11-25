#include "Point.hpp"

struct KMeansResults
{
  std::vector<Point> iter_centroids;
  int epoch;
  int64_t time;
  KMeansResults(int _epoch, int64_t _time, std::vector<Point> _iter_centroids) : epoch(_epoch), time(_time), iter_centroids(_iter_centroids) {};
};

class KMeans
{
private:
  int K;
  int epochs;
  static bool seeded;

public:
  std::vector<Point> points;
  KMeans(std::vector<Point> points, int K, int epochs) : points(points),
                                                         K(K),
                                                         epochs(epochs){};

  KMeansResults run(bool debug = false);
  bool isConverged(std::vector<int> *prev_clusters);
  std::vector<std::vector<float>> unpackPointVector(std::vector<Point> pts);
};