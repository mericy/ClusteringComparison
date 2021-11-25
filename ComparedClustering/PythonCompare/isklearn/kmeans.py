from method import Method
from sklearn import cluster

import numpy as np

class KMeans(Method):
    def _method(self):
        kmeans = cluster.KMeans(n_clusters=self.n_clusters,
                                n_init=1,
                                n_jobs=1)
        labels = kmeans.fit_predict(self.samples)
        centers = kmeans.cluster_centers_

        return centers, labels
