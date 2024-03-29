import networkx as nx
import random as ra

graph = nx.barabasi_albert_graph(10000000, 3, 69)
print("graph created")
for e in graph.edges():
    graph[e[0]][e[1]]['weight'] = ra.randint(1,100)
print(graph.number_of_edges())
nx.write_weighted_edgelist(graph, "Resources\BarabasiSparse10M3E.txt")

