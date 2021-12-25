import sys
from sage.all import *

def save_graph(graph, name):
    f = open(name, "w")
    
    name = {}
    for vertex in graph.vertices(key=str):
        name[vertex] = str(len(name))

    for edge in graph.edges(labels=False, key=str):
        f.write(name[edge[0]] + ' ' + name[edge[1]] + '\n')
    f.close()

print("Введите количество: ")
n = int(input())

for i in range(2,2+n):
    grid = graphs.Grid2dGraph(i,i//2*2+3)
    CFI, p = graphs.CaiFurerImmermanGraph(grid)
    tCFI, p = graphs.CaiFurerImmermanGraph(grid, True)
    save_graph(CFI, "CFI_grid_" + str(i) + ".cgf")
    save_graph(tCFI, "CFI_grid_" + str(i) + "t.cgf")
