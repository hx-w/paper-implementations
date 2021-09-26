# -*- coding: utf-8 -*-
import random

from graph_struct import *

part1_state = 0 # 0 init; 1 drop
last_2_edges = [-1, -1]

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

def drop_edge() -> bool:
    counter = 0
    global last_2_edges, g_phase, g_vet_edges, g_edges
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
            ## last 2 edges
            last_2_edges[1] = last_2_edges[0]
            last_2_edges[0] = single
    print(f'当前迭代删除边{counter}处')
    remain = len(list(filter(lambda x: not x.deleted, g_edges)))
    print(f'剩余{remain}')
    if remain == 0:
        g_edges[last_2_edges[0]].deleted = False
        g_edges[last_2_edges[1]].deleted = False
    if counter == 0:
        # for idx in range(len(g_trias)):
        #     g_trias[idx].deleted = False
        return True
    return False

def is_neighbor(idx, jdx) -> tuple:
    if g_edges[idx].v1 == g_edges[jdx].v1:
        return True, g_edges[idx].v2, g_edges[jdx].v2
    elif g_edges[idx].v1 == g_edges[jdx].v2:
        return True, g_edges[idx].v2, g_edges[jdx].v1
    elif g_edges[idx].v2 == g_edges[jdx].v2:
        return True, g_edges[idx].v1, g_edges[jdx].v1
    elif g_edges[idx].v2 == g_edges[jdx].v1:
        return True, g_edges[idx].v1, g_edges[jdx].v2
    return False, None, None

def is_connect(v1, v2) -> int:
    tedge = Edge(v1, v2)
    for idx in range(len(g_edges)):
        if tedge == g_edges[idx]:
            return idx
    return -1

def straighten_loop():
    global g_edges
    affact = 1
    while affact != 0:
        affact = 0
        for idx in range(len(g_edges) - 1):
            if g_edges[idx].deleted: continue
            for jdx in range(idx + 1, len(g_edges)):
                if g_edges[jdx].deleted: continue
                success, front, back = is_neighbor(idx, jdx)
                if not success: continue
                bridge = is_connect(front, back)
                if bridge == -1: continue
                g_edges[idx].deleted = True
                g_edges[jdx].deleted = True
                g_edges[bridge].deleted = False
                affact += 1
                break
            if affact != 0: break
        print(f"affact: {affact}")
    remain = len(list(filter(lambda x: not x.deleted, g_edges)))
    print(f'剩余{remain}')