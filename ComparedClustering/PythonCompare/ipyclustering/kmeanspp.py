from method import Method
import numpy as np

from pyclustering.cluster.kmeans import kmeans
from pyclustering.cluster.center_initializer import kmeans_plusplus_initializer


class KMeansPlusPlus(Method):
    def _method(self):
        initial_centers = kmeans_plusplus_initializer(
            self.samples, self.n_clusters).initialize()
        kmeans_instance = kmeans(self.samples, initial_centers).process()

        centers = np.asarray(kmeans_instance.get_centers())
        clusters = kmeans_instance.get_clusters()
        labels = np.squeeze([np.where([i in c for c in clusters])
                             for i in range(len(self.samples))])

        return centers, labels
