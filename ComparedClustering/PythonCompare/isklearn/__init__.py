from .birch import BIRCH
from .kmeans import KMeans
from .gmm import GMM
from .agglomerative import Agglomerative

methods = [
    KMeans,
    BIRCH,
    Agglomerative,
    GMM,
]
