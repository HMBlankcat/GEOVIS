import terrain_visualization
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

class QuadtreeNode:
    def __init__(self, x, y, size, data):
        self.x = x
        self.y = y
        self.size = size
        self.data = data
        self.children = []

    def subdivide(self):
        half_size = self.size // 2
        self.children = [
            QuadtreeNode(self.x, self.y, half_size, self.data[:half_size, :half_size]),
            QuadtreeNode(self.x + half_size, self.y, half_size, self.data[:half_size, half_size:]),
            QuadtreeNode(self.x, self.y + half_size, half_size, self.data[half_size:, :half_size]),
            QuadtreeNode(self.x + half_size, self.y + half_size, half_size, self.data[half_size:, half_size:])
        ]

def build_quadtree(data, threshold):
    root = QuadtreeNode(0, 0, data.shape[0], data)
    nodes = [root]
    while nodes:
        node = nodes.pop()
        if np.std(node.data) > threshold and node.size > 1:
            node.subdivide()
            nodes.extend(node.children)
    return root

def plot_quadtree(node, ax):
    if not node.children:
        x = np.arange(node.x, node.x + node.size)
        y = np.arange(node.y, node.y + node.size)
        x, y = np.meshgrid(x, y)
        z = node.data
        ax.plot_surface(x, y, z, cmap='terrain')
    else:
        for child in node.children:
            plot_quadtree(child, ax)

def plot_terrain_with_quadtree(elevation, threshold):
    quadtree = build_quadtree(elevation, threshold)
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    plot_quadtree(quadtree, ax)
    plt.show()

if __name__ == "__main__":
    filename = 'path/to/your/dem/file.tif'
    elevation = read_terrain_data(filename)
    plot_terrain_with_quadtree(elevation, threshold=10)
