# **graphs_algorithms**
  # description :
    A collection of graphs algorithms implementation in c++

  # How to use :
    After compiling provide the executable file with a file path as a command line argument.
    The file should have the following syntax :
    * the first line : string indicating the type of the graph which can be one of the following strings:
      "undirected_unweighted", "undirected_weighted", "directed_unweighted", "directed_weighted"
	  (obviously with no quotes in the console :))
	* the second line : 2 space separeted integers N and M, N is the number of vertices in the graph and M is the number of edges.
    * the next M lines : depends on the type of the graph, for :
      * undirected_unweighted : 2 space seperated integers indicating the indexes of the vertices of the undirected edge.
      * undirected_weighted : 3 space seperated integers where the first two integers are the indexes of the vertices of the undirected edge
        and the last one is its weight.
      * directed_unweighted : 2 space seperated integers indicating the indexes of the vertices of the directed edge.
      * directed_weighted : 3 space seperated integers where the first two integers are the indexes of the vertices of the directed edge
        and the last one is its weight.
      **the vertices indexes are 0 indexed and labeled from 0 to N - 1 inclusive**
    After that if the file is correctly formated you should get a list of the algorithms you can perform on the graph
  # Implemented algorithms :
    * Depth first search.
    * Breadth first search.
    * Djikstra shortest path.
    * Bellman-Ford shortest path.
    * Floyl-warshal all pairs shortest path.
    * Prim's minimum spanning tree.
    * Kruskal's minimum spanning tree.
    * Kusaraju strongly connected components.
    * Tarjan strongly connected components.
    * Welsh-Powell graph coloring.
    * DSATUR graph coloring.
  # Important notes :
    * be aware of windows file system ie file names and folder names should never contain spaces.
