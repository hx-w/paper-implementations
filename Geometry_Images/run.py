# -*- coding: utf-8 -*-
import sys

import matplotlib.pyplot as plt
import mpl_toolkits.mplot3d as a3d

from reader import *

def parse_argv() -> tuple:
    if len(sys.argv) <= 1:
        print('用法：python3 run.py <网格文件>')
        return False, None
    return True, sys.argv[1]


def draw_init():
    ax = plt.axes(projection='3d')
    tri = a3d.art3d.Poly3DCollection((get_all_trias()))
    ax.add_collection3d(tri)

    # fix aabb
    ax.set_xlim(*g_aabb.x_lim)
    ax.set_ylim(*g_aabb.y_lim)
    ax.set_zlim(*g_aabb.z_lim)
    plt.show()


def main():
    # read argv
    success, filepath = parse_argv()
    if not success: return
    # read mesh file
    readfile(filepath)
    # draw init graph
    draw_init()

if __name__ == '__main__':
    main()