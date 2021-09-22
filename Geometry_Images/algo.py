# -*- coding: utf-8 -*-
import random

from graph_struct import *

part1_state = 0 # 0 init; 1 drop

# part 1
def drop_trias():
    global part1_state, g_trias
    if part1_state == 0:
        # drop a random trias
        seed_trias = random.randint(0, len(g_trias) - 1)
        print(f'随机三角形索引：{seed_trias}')
        g_trias[seed_trias].drop(seed_trias)
        part1_state = 1
    elif part1_state == 1:
        # find edge adj only 1 trias
        counter = 0
        for idx in range(len(g_edges_trias)):
            if len(g_edges_trias[idx]) == 1:
                g_trias[g_edges_trias[idx][0]].drop(g_edges_trias[idx][0])
                g_edges[idx].deleted = True
                counter += 1
        print(f'当前迭代删除边{counter}处')
        remain = len(list(filter(lambda x: not x.deleted, g_edges)))
        print(f'剩余{remain}')

# part 2

## build vertex-edge index
def build_ve_index():
    for idx in range(len(g_vertexs)):
        g_vet_edges.append([])
    for idx in range(len(g_edges)):
        if g_edges[idx].deleted: continue
        g_vet_edges[g_edges[idx].v1].append(idx)
        g_vet_edges[g_edges[idx].v2].append(idx)

def drop_edge():
    counter = 0
    for idx in range(len(g_vet_edges)):
        if len(g_vet_edges[idx]) == 1:
            single = g_vet_edges[idx][0]
            # update
            other = g_edges[single].v1
            if other == idx:
                other = g_edges[single].v2
            g_vet_edges[other].remove(single)
            g_vet_edges[idx] = []
            g_edges[single].deleted = True
            counter += 1
    print(f'当前迭代删除边{counter}处')
    remain = len(list(filter(lambda x: not x.deleted, g_edges)))
    print(f'剩余{remain}')
    pass