from method import Method
import numpy as np

from pyclustering.cluster.agglomerative import agglomerative

class Agglomerative(Method):
    def _method(self):
        agglomerative_instance = agglomerative(self.samples, self.n_clusters).process()
        clusters = agglomerative_instance.get_clusters()
        centers = np.asarray([self.samples[c].mean(axis=0) for c in clusters])
        labels = np.squeeze([np.where([i in c for c in clusters]) for i in range(len(self.samples))])
        return centers, labels
