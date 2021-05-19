import networkx as nx
import random as ra

graph = nx.barabasi_albert_graph(100000, 12, 69)
print("graph created")
for e in graph.edges():
    graph[e[0]][e[1]]['weight'] = ra.randint(1,100)
nx.write_weighted_edgelist(graph, "Resources\Barabasi100000_12.txt")

