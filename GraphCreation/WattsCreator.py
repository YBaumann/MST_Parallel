import networkx as nx
import random as ra

graph = nx.connected_watts_strogatz_graph(100, 12, 0.3, 100, seed=69)
print("graph created")
for e in graph.edges():
    graph[e[0]][e[1]]['weight'] = ra.randint(1,100)
nx.write_weighted_edgelist(graph, "test.txt")

