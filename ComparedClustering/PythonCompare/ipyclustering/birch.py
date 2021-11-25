from method import Method
import numpy as np

from pyclustering.cluster.birch import birch


class BIRCH(Method):
    def _method(self):
        birch_instance = birch(self.samples.tolist(), self.n_clusters).process()
        clusters = birch_instance.get_clusters()
        centers = np.asarray([self.samples[c].mean(axis=0) for c in clusters])
        labels = np.squeeze([np.where([i in c for c in clusters])
                             for i in range(len(self.samples))])
        return centers, labels
