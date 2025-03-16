#include "dijkstras.h"

using namespace std;

int main() {
    // -------------------------------
    // Test 1: Simple Graph
    // -------------------------------
    // Construct a simple graph with 4 vertices:
    // Vertices: 0, 1, 2, 3
    // Edges:
    //   0 -> 1 (weight 1)
    //   0 -> 2 (weight 4)
    //   1 -> 2 (weight 2)
    //   1 -> 3 (weight 5)
    //   2 -> 3 (weight 1)
    Graph G;
    G.numVertices = 4;
    G.resize(4);
    G[0].push_back(Edge(0, 1, 1));
    G[0].push_back(Edge(0, 2, 4));
    G[1].push_back(Edge(1, 2, 2));
    G[1].push_back(Edge(1, 3, 5));
    G[2].push_back(Edge(2, 3, 1));
    
    vector<int> previous(G.numVertices, -1);
    vector<int> distances = dijkstra_shortest_path(G, 0, previous);

    // Expected distances from vertex 0:
    //   0: 0, 1: 1, 2: 3, 3: 4
    assert(distances[0] == 0);
    assert(distances[1] == 1);
    assert(distances[2] == 3);
    assert(distances[3] == 4);

    // Extract the shortest path from 0 to 3.
    // Expected path: 0 -> 1 -> 2 -> 3
    vector<int> path = extract_shortest_path(distances, previous, 3);
    vector<int> expected_path = {0, 1, 2, 3};
    assert(path == expected_path);

    // Optionally, print the path:
    print_path(path, distances[3]);

    // -------------------------------
    // Test 2: Graph with Unreachable Vertex
    // -------------------------------
    // Increase the number of vertices by one, so vertex 4 is isolated.
    G.numVertices = 5;
    G.resize(5);
    // Vertices 0-3 remain as before, but vertex 4 is now isolated.
    previous.resize(5, -1);
    distances = dijkstra_shortest_path(G, 0, previous);
    
    // Since vertex 4 is unreachable, its distance should be INF.
    assert(distances[4] == INF);
    
    // Attempt to extract a path to vertex 4; expect an empty path.
    vector<int> path_to_4 = extract_shortest_path(distances, previous, 4);
    assert(path_to_4.empty());

    cout << "All Dijkstra tests passed.\n";
    return 0;
}