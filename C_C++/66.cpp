#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <algorithm>
#include <limits>
#include <utility>
#include <set>

class Graph {
public:
    struct Edge {
        int destination;
        int weight;
    };

    explicit Graph(int vertices) : adjList(vertices) {}

    void addEdge(int source, int destination, int weight) {
        adjList[source].push_back({destination, weight});
        adjList[destination].push_back({source, weight}); // Comment this line if directed
    }

    void dijkstra(int start) {
        std::vector<int> dist(adjList.size(), std::numeric_limits<int>::max());
        dist[start] = 0;
        std::set<std::pair<int, int>> pq; // (distance, vertex)
        pq.insert({0, start});

        while (!pq.empty()) {
            int u = pq.begin()->second;
            pq.erase(pq.begin());

            for (const auto& edge : adjList[u]) {
                int v = edge.destination;
                int weight = edge.weight;

                if (dist[u] + weight < dist[v]) {
                    pq.erase({dist[v], v});
                    dist[v] = dist[u] + weight;
                    pq.insert({dist[v], v});
                }
            }
        }

        std::cout << "Vertex\tDistance from Source" << std::endl;
        for (int i = 0; i < dist.size(); i++) {
            std::cout << i << "\t" << dist[i] << std::endl;
        }
    }

    void kruskal() {
        std::vector<std::tuple<int, int, int>> edges; // (weight, source, destination)
        for (int u = 0; u < adjList.size(); u++) {
            for (const auto& edge : adjList[u]) {
                if (u < edge.destination) { // Prevent duplicating edges in undirected graph
                    edges.emplace_back(edge.weight, u, edge.destination);
                }
            }
        }

        std::sort(edges.begin(), edges.end());
        std::vector<int> parent(adjList.size());
        for (int i = 0; i < adjList.size(); i++) parent[i] = i;

        std::function<int(int)> find = [&](int i) {
            if (parent[i] != i)
                parent[i] = find(parent[i]);
            return parent[i];
        };

        auto unionSets = [&](int u, int v) {
            int set_u = find(u);
            int set_v = find(v);
            if (set_u != set_v) {
                parent[set_u] = set_v;
            }
        };

        std::cout << "Edges in the Minimum Spanning Tree:\n";
        for (const auto& [weight, u, v] : edges) {
            if (find(u) != find(v)) {
                std::cout << u << " -- " << v << " == " << weight << "\n";
                unionSets(u, v);
            }
        }
    }

    void topologicalSort() {
        std::vector<int> in_degree(adjList.size(), 0);
        for (const auto& list : adjList) {
            for (const auto& edge : list) {
                in_degree[edge.destination]++;
            }
        }

        std::queue<int> zero_in_degree;
        for (int i = 0; i < in_degree.size(); i++) {
            if (in_degree[i] == 0) {
                zero_in_degree.push(i);
            }
        }

        int count = 0;
        std::vector<int> top_order;
        while (!zero_in_degree.empty()) {
            int u = zero_in_degree.front();
            zero_in_degree.pop();
            top_order.push_back(u);

            for (const auto& edge : adjList[u]) {
                if (--in_degree[edge.destination] == 0) {
                    zero_in_degree.push(edge.destination);
                }
            }
            count++;
        }

        if (count != adjList.size()) {
            std::cout << "Cycle detected! Topological sort not possible.\n";
        } else {
            std::cout << "Topological Sort:\n";
            for (int v : top_order) {
                std::cout << v << " ";
            }
            std::cout << "\n";
        }
    }

private:
    std::vector<std::list<Edge>> adjList;
};

int main() {
    Graph g(6);

    g.addEdge(0, 1, 4);
    g.addEdge(0, 2, 3);
    g.addEdge(1, 2, 1);
    g.addEdge(1, 3, 2);
    g.addEdge(2, 3, 4);
    g.addEdge(3, 4, 2);
    g.addEdge(4, 5, 6);

    std::cout << "Running Dijkstra's Algorithm from node 0:\n";
    g.dijkstra(0);

    std::cout << "\nRunning Kruskal's Algorithm:\n";
    g.kruskal();

    std::cout << "\nRunning Topological Sort (Suitable for DAG. Add a new directed acyclic graph for appropriate demonstration):\n";
    g.topologicalSort();

    return 0;
}