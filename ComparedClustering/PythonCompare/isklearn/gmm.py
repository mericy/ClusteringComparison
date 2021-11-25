from method import Method
from sklearn.mixture import GaussianMixture

import numpy as np


class GMM(Method):
    def _method(self):
        gmm = GaussianMixture(n_components=self.n_clusters, n_init=1)
        gmm = gmm.fit(self.samples)

        centers = gmm.means_
        
        return centers, None
