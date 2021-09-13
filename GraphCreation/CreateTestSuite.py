import networkx as nx
import random as ra

#[(100,"hun"),(1000,"1K"),(10_000,"10K"),(100_000,"100K")]


for (nodes,name) in [(100,"hun"),(1000,"1K"),(10_000,"10K"),(100_000,"100K")]:
    for edge in [3,60]:
        for copy in range(0,10):
            graph = nx.barabasi_albert_graph(nodes, edge, 69)
            print("graph created " + name)
            for e in graph.edges():
                graph[e[0]][e[1]]['weight'] = ra.randint(1,100)

            nx.write_weighted_edgelist(graph, "TestingGraphs\BarabasiSparse"+name + str(edge) + "E_" +  str(copy) + ".txt")