#include "dijkstras.h"


vector<int> dijkstra_shortest_path(const Graph& G, int source, vector<int>& previous) {
    int n = G.numVertices;
    vector<int> dist(n, INF);
    vector<bool> visited(n, false);

    using PQItem = pair<int,int>;
    priority_queue<PQItem, vector<PQItem>, greater<PQItem>> pq;

    dist[source] = 0;
    pq.push({0, source});

    while (!pq.empty()) {
        auto [currDist, u] = pq.top();
        pq.pop();

        if (visited[u]) continue;
        visited[u] = true;

        for (const auto& e : G[u]) {
            int v = e.dst;
            int w = e.weight;
            if (!visited[v] && dist[u] != INF && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                previous[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}


vector<int> extract_shortest_path(const vector<int>& distances, const vector<int>& previous, int destination) 
{
    vector<int> path;
    if (destination < 0 || destination >= (int)distances.size() ||
        distances[destination] == INF) 
    {
        return path;
    }
    for (int v = destination; v != -1; v = previous[v]) {
        path.push_back(v);
    }
    reverse(path.begin(), path.end());
    return path;
}


void print_path(const vector<int>& path, int total) {
    if (path.empty()) {
        cout << "No path found.\n";
        return;
    }
    // Print the vertices separated by a space.
    for (size_t i = 0; i < path.size(); i++) {
        cout << path[i] << " ";
    }
    cout << "\nTotal cost is " << total << "\n";
}
