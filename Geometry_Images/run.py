# -*- coding: utf-8 -*-
import sys

import matplotlib.pyplot as plt
import mpl_toolkits.mplot3d as a3d

from reader import *

figure = plt.figure()
ax = plt.axes(projection='3d')

def parse_argv() -> tuple:
    if len(sys.argv) <= 1:
        print('用法：python3 run.py <网格文件>')
        return False, None
    return True, sys.argv[1]


g_phase = 0 # 0 init;  1 algo-1; 2 algo-2
g_state = 0 # 0 manually; 1 auto

def press(event):
    global g_state
    global g_phase
    if event.key == 'enter':
        g_state = 0
        if g_phase == 0: g_phase = 1
        plt.title(f'【step-{g_phase}】手动执行')
        
    if event.key == ' ':
        g_state = 1
        if g_phase == 0: g_phase = 1
        plt.title(f'【step-{g_phase}】自动执行')

def draw_once():
    ax.clear()
    # fix aabb
    ax.set_xlim(*g_aabb.x_lim)
    ax.set_ylim(*g_aabb.y_lim)
    ax.set_zlim(*g_aabb.z_lim)
    all_trias = get_all_trias()
    tri = a3d.art3d.Poly3DCollection(all_trias, facecolors='orange', alpha=0.5)
    edges = a3d.art3d.Line3DCollection(all_trias, colors='green', linewidth=0.7, alpha=1)
    ax.add_collection3d(tri)
    ax.add_collection3d(edges)

def draw():
    draw_once()
    while g_phase == 0: # init
        plt.pause(1.0)
    
    while g_phase == 1:
        global g_trias, g_state
        if g_state == 0:
            g_state = -1
            draw_once()
            print(g_edges_trias)
        plt.pause(0.1)


def main():
    plt.rcParams['font.sans-serif'] = ['Arial Unicode MS']
    # read argv
    success, filepath = parse_argv()
    if not success: return
    # read mesh file
    readfile(filepath)
    figure.canvas.mpl_connect('key_press_event', press)
    # draw graph
    draw()

if __name__ == '__main__':
    main()