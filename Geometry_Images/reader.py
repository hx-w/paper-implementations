# -*- coding: utf-8 -*-

from graph_struct import *

def readfile(filename: str):
    '''
    @param[in] filename, *.m defined as mesh object
    @param[out] None, structure saved in g_vertexs, g_edges, g_trias 
    '''
    with open(filename, 'r') as ifile:
        alllines = ifile.readlines() # for mem sake
        for line in alllines:
            line = line.strip()
            if line.startswith('#'): # it's a comment line
                continue
            elements = list(filter(lambda x: x != '', line.split(' ')))
            if elements[0] == 'Vertex':
                numbers = list(map(lambda x: float(x), elements[2:]))
                g_vertexs.append(Vertex(*numbers))
                assert len(g_vertexs) == int(elements[1])
            elif elements[0] == 'Face':
                numbers = list(map(lambda x: int(x) - 1, elements[2:]))
                # build edges
                edges = [
                    Edge(numbers[0], numbers[1]),
                    Edge(numbers[0], numbers[2]),
                    Edge(numbers[1], numbers[2])
                ]
                edges_idx = []
                for edge in edges:
                    if edge in g_edges:
                        edges_idx.append(g_edges.index(edge))
                    else:
                        edges_idx.append(len(g_edges))
                        g_edges.append(edge)
                # build trias
                g_trias.append(Triangle(*edges_idx))
                # check index valid
                assert len(g_trias) == int(elements[1])
            else:
                raise Exception(f'Tag<{elements[0]}> is not defined')
    print(f'点数：{len(g_vertexs)}; 边数：{len(g_edges)}; 面数：{len(g_trias)}')

def get_all_trias() -> list:
    return list(map(lambda x: x.get_trias(), g_trias))

def get_all_edges() -> list:
    return list(map(lambda x: x.get_edges(), g_edges))

# def get_all_vertexs() -> list: # [x: list, y:list, z: list]
#     x_list = list(map(lambda x: x.x, g_vertexs))
#     y_list = list(map(lambda x: x.y, g_vertexs))
#     z_list = list(map(lambda x: x.z, g_vertexs))
#     return [x_list, y_list, z_list]