# -*- coding: utf-8 -*-

g_vertexs = []
g_edges = []
g_trias = []

g_edges_trias = [] # [[trias: int, ...], ...]
g_vet_edges = []

# Axis Aligned Bounding Box
class AABB(object):
    def __init__(self, x_init: float = 0.0, y_init: float = 0.0, z_init: float = 0.0):
        self.x_lim = [x_init, x_init]
        self.y_lim = [y_init, y_init]
        self.z_lim = [z_init, z_init]
    
    def update(self, x: float, y: float, z: float):
        if len(g_vertexs) == 0:
            self.x_lim = [x, x]
            self.y_lim = [y, y]
            self.z_lim = [z, z]
            return
        if x < self.x_lim[0]: self.x_lim[0] = x
        elif x > self.x_lim[1]: self.x_lim[1] = x
        if y < self.y_lim[0]: self.y_lim[0] = y
        elif y > self.y_lim[1]: self.y_lim[1] = y
        if z < self.z_lim[0]: self.z_lim[0] = z
        elif z > self.z_lim[1]: self.z_lim[1] = z

g_aabb = AABB()

class Vertex(object):
    def __init__(self, x: float = 0.0, y: float = 0.0, z: float = 0.0):
        self.x = x
        self.y = y
        self.z = z
        # update aabb
        global g_aabb
        g_aabb.update(x, y, z)
        
    
    def __eq__(self, obj: object) -> bool:
        return obj.x == self.x and obj.y == self.y
    
    def xyz(self) -> list:
        return [self.x, self.y, self.z]


class Edge(object):
    def __init__(self, v1: int = -1, v2: int = -1):
        self.v1 = v1
        self.v2 = v2
        self.deleted = False
    
    def __eq__(self, obj: object) -> bool:
        return min(self.v1, self.v2) == min(obj.v1, obj.v2) \
            and max(self.v1, self.v2) == max(obj.v1, obj.v2)

class Triangle(object):
    def __init__(self, e1: int = -1, e2: int = -1, e3: int = -1):
        self.edges = [e1, e2, e3]
        self.deleted = False
    
    def get_trias(self) -> list:
        if self.deleted: return None
        trias_idx = set()
        for edge in self.edges:
            trias_idx.add(g_edges[edge].v1)
            trias_idx.add(g_edges[edge].v2)
        trias = []
        for idx in trias_idx:
            trias.append(g_vertexs[idx].xyz())
        return trias
    
    def get_edges(self) -> list:
        trias_idx = set()
        for edge in self.edges:
            if g_edges[edge].deleted: continue
            trias_idx.add(g_edges[edge].v1)
            trias_idx.add(g_edges[edge].v2)
        trias = []
        for idx in trias_idx:
            trias.append(g_vertexs[idx].xyz())
        return trias
    
    def get_vertex(self) -> list:
        trias_idx = set()
        for edge in self.edges:
            if g_edges[edge].deleted: continue
            trias_idx.add(g_edges[edge].v1)
            trias_idx.add(g_edges[edge].v2)
        return trias_idx

    def drop(self, selfid: int):
        self.deleted = True
        # update index
        for idx in range(len(g_edges_trias)):
            if selfid in g_edges_trias[idx]:
                g_edges_trias[idx].remove(selfid)