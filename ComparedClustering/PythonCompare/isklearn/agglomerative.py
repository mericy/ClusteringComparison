from method import Method
from sklearn.cluster import AgglomerativeClustering

import numpy as np


class Agglomerative(Method):
    def _method(self):
        agglomerative = AgglomerativeClustering(n_clusters=self.n_clusters)
        labels = agglomerative.fit_predict(self.samples)

        dimension = self.samples.shape[-1]
        centers = np.zeros((self.n_clusters, dimension))

        for i in range(self.n_clusters):
            centers[i] = self.samples[labels == i].mean(axis=0)

        return centers, labels
