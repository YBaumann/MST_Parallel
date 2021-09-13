import networkx as nx
import random as ra
nodes = 10_000_000
edge = 3



graph = nx.barabasi_albert_graph(nodes, edge, 69)
print("graph created")
for e in graph.edges():
    graph[e[0]][e[1]]['weight'] = ra.randint(1,100)


print(graph.number_of_edges())

nx.write_weighted_edgelist(graph, "BarabasiSparse10M" + str(3) + "E.txt")

