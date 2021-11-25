from method import Method
from sklearn.cluster import Birch

import numpy as np


class BIRCH(Method):
    def _method(self):
        brc = Birch(n_clusters=self.n_clusters).fit(self.samples)

        dimension = self.samples.shape[-1]
        centers = np.zeros((self.n_clusters, dimension))
        labels = brc.labels_

        for i in range(self.n_clusters):
            centers[i] = self.samples[labels == i].mean(axis=0)

        return centers, labels
