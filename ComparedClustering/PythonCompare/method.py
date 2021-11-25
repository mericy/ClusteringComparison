import datetime

from abc import abstractmethod
from sklearn.metrics import mean_squared_error
import numpy as np

import pywt
import math


def decode(dataIn):
    decoded = []
    for i in range(len(dataIn)):
        wp = pywt.WaveletPacket(data=dataIn[i], wavelet='haar', mode='symmetric')
        for t, node in enumerate(wp.get_level(int(np.log2(dataIn.shape[-1])), "natural")):
            wp[node.path].data = [dataIn[i][t]]
        wp.reconstruct()
        decoded.append(wp.data)
    return np.ravel(decoded)


class Method():
    def __init__(self, samples, n_clusters):
        self.samples = samples
        self.decoded = decode(samples)
        self.n_clusters = n_clusters

    @abstractmethod
    def _method(self):
        pass

    def run(self, repeat, moduledir = None, verbose=False):
        times = []
        rmses = []
        for i in range(repeat):
            start = datetime.datetime.now()
            centers, _labels = self._method()
            end = datetime.datetime.now()
            labels = np.asarray([np.argmin(np.linalg.norm(centers - signal, axis=1)) for signal in self.samples])
            predict = centers[labels]
            time = (end-start).total_seconds()
            rmse = math.sqrt(mean_squared_error(self.decoded, decode(predict)))
            if moduledir != None:
                np.savetxt("{}/{}_{}_{}.txt".format(moduledir, self.__class__.__name__.lower(), i, self.n_clusters), centers)
            if verbose:
                print("{:3} | Time: {:.2f} | RMSE: {}".format(i+1, time, rmse))
            rmses.append(rmse)
            times.append(time)
        if moduledir != None:
            np.savetxt("{}/{}_times.txt".format(moduledir, self.__class__.__name__.lower(), repeat, self.n_clusters), times)
            np.savetxt("{}/{}_rmses.txt".format(moduledir, self.__class__.__name__.lower(), repeat, self.n_clusters), rmses)
        return times, rmses
