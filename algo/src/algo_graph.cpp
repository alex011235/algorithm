///
/// \brief Source file for graph algorithms.
/// \author alex011235
/// \date 2020-05-03
/// \link <a href=https://github.com/alex011235/algorithm>Algorithm, Github</a>
///

#include "algo_graph.hpp"

#include <numeric>
#include <queue>

namespace algo::graph {

namespace {
constexpr double kDblMax{1.79769e+308};
}// namespace

// //////////////////////////////////////////
//  Graph functions
// //////////////////////////////////////////

Graph NewGraph(size_t size)
{
  return Graph(size);
}

bool MakeEdge(Graph &graph, const int &s, const int &t, const double &w)
{
  if (s >= graph.size() || t >= graph.size() || s == t) {
    return false;
  }

  graph[s].emplace_back(Connection{t, w});
  graph[t].emplace_back(Connection{s, w});
  return true;
}

bool MakeEdge(Graph &graph, const int &s, const int &t)
{
  if (s >= graph.size() || t >= graph.size()) {
    return false;
  }
  graph[s].emplace_back(Connection{t, 0.0});
  graph[t].emplace_back(Connection{s, 0.0});
  return true;
}

bool MakeDirEdge(Graph &graph, const int &s, const int &t, const double &w)
{
  if (s >= graph.size() || t >= graph.size()) {
    return false;
  }
  graph[s].push_back(Connection{t, w});
  return true;
}

bool MakeDirEdge(Graph &graph, const int &s, const int &t)
{
  if (s >= graph.size() || t >= graph.size()) {
    return false;
  }
  graph[s].push_back(Connection{t, 0.0});
  return true;
}

void SetWeight(Graph &graph, const int &s, const int &t, double weight)
{
  for (auto &conn : graph[s]) {
    if (conn.node == t) {
      conn.weight = weight;
      return;
    }
  }
}

double GetWeight(Graph &graph, const int &s, const int &t)
{
  for (const auto &conn : graph[s]) {
    if (conn.node == t) {
      return conn.weight;
    }
  }
  return 0.0;
}

void RemoveEdge(Graph &graph, const int &s, const int &t)
{
  size_t pos{0};

  for (const auto &conn : graph[s]) {
    if (conn.node == t) {
      break;
    }
    pos++;
  }

  graph[s].erase(graph[s].begin() + pos);
}

Edges GetEdges(const Graph &graph)
{
  Edges edges;

  // Construct a list of edges
  for (int i = 0; i < graph.size(); ++i) {
    for (const auto &c : graph[i]) {
      edges.emplace_back(Edge{i, c.node, c.weight});
    }
  }
  return edges;
}

struct comp {
  bool operator()(const Connection &lhs, Connection &rhs) const
  {
    return lhs.weight > rhs.weight;
  }
};

// //////////////////////////////////////////
//  Breadth-First-Search (BFS)
// //////////////////////////////////////////

Nodes BFS(const Graph &graph, const int &source)
{
  // Forbidden input.
  if (graph.empty() || source < 0 || source >= graph.size()) {
    return Nodes{};
  }

  std::vector<int> distance(graph.size(), INT_MAX);
  Nodes parent(graph.size(), -1);
  std::queue<int> q;
  q.push(source);
  distance[source] = 0;

  while (!q.empty()) {

    int curr{q.front()};
    q.pop();

    for (const auto &n : graph[curr]) {
      if (distance[n.node] == INT_MAX) {
        distance[n.node] = distance[curr] + 1;
        parent[n.node] = curr;
        q.push(n.node);
      }
    }
  }

  return parent;
}

Path ShortestPathBFS(const Graph &graph, const int &source, const int &dest)
{
  // Forbidden input:
  size_t N{graph.size()};
  if (N < 2 || dest > N || source > N || source < 0 || dest < 0 || source == dest) {
    return Path{Nodes{}, false};
  }

  const Nodes kNodes{BFS(graph, source)};
  Nodes path;
  int prev{dest};

  while (prev != source) {
    if (kNodes[prev] == -1) {
      return Path{Nodes{}, false};
    }
    path.emplace_back(prev);
    prev = kNodes[prev];
  }

  path.emplace_back(prev);
  std::reverse(path.begin(), path.end());
  return Path{path, true};
}

bool IsBipartite(const Graph &graph)
{
  // Forbidden input.
  if (graph.size() < 2) {
    return false;
  }

  std::vector<int> colors(graph.size(), -1);
  std::queue<int> q;
  q.push(0);
  colors[0] = 1;

  // BFS, modified for checking bipartiteness.
  while (!q.empty()) {
    int curr{q.front()};
    q.pop();

    for (auto edge : graph[curr]) {

      if (colors[edge.node] == -1) {
        colors[edge.node] = 1 - colors[curr];
        q.push(edge.node);

        // Two adjacent nodes have the same color.
      } else if (colors[edge.node] == colors[curr]) {
        return false;
      }
    }
  }
  return true;
}

// //////////////////////////////////////////
//  Nearest neighbor algorithm, TSP.
// //////////////////////////////////////////

Nodes AllNodesPath(const Graph &graph, const int &source)
{
  // Forbidden input.
  if (graph.size() < 3 || source < 0 || source >= graph.size()) {
    return Nodes{};
  }
  int current_node{source};
  Visited visited(graph.size(), false);
  visited[current_node] = true;

  // Salesman's path
  Nodes path;
  path.emplace_back(current_node);

  while (std::any_of(visited.begin(), visited.end(), [](bool v) { return !v; })) {
    int next_node;
    double min_weight{kDblMax};

    for (const auto &node : graph[current_node]) {
      // Find closest neighbor
      if (node.weight < min_weight && !visited[node.node]) {
        next_node = node.node;
        min_weight = node.weight;
      }
    }

    if (current_node == next_node) {
      // Failed to find full path.
      break;
    }

    current_node = next_node;
    visited[current_node] = true;
    path.emplace_back(current_node);
  }

  // Try next source
  if (path.size() != graph.size() && source < graph.size()) {
    path = AllNodesPath(graph, source + 1);
  }

  return path;
}

// //////////////////////////////////////////
//  Prim's, minimum spanning tree.
// //////////////////////////////////////////

Graph MinimumSpanningTree(const Graph &graph, const int &source, double &total_weight)
{
  // Forbidden input.
  if (source >= graph.size() || source < 0 || graph.empty()) {
    return Graph{NewGraph(0)};
  }

  Weights weights(graph.size(), kDblMax);
  Nodes parent(graph.size());
  Visited visited(graph.size(), false);
  std::priority_queue<Connection, std::vector<Connection>, comp> pq;

  pq.push(Connection{source, 0});
  weights[source] = 0;

  while (!pq.empty()) {
    Connection U{pq.top()};
    pq.pop();
    int node{U.node};

    for (auto v : graph.at(node)) {
      if (!visited[v.node] && v.weight < weights[v.node]) {
        parent[v.node] = node;
        weights[v.node] = v.weight;
        pq.push(v);
      }
    }
    visited[node] = true;
  }

  // Construct the MST
  size_t index{parent.size() - 1};
  Graph mst{NewGraph(parent.size())};

  std::for_each(parent.rbegin(), parent.rend(), [&mst, &index, weights](int n) {
    MakeEdge(mst, n, index, weights[index]);
    index--;
  });

  total_weight = std::accumulate(weights.begin(), weights.end(), 0.0);

  return mst;
}

// //////////////////////////////////////////
//  Dijkstra's, shortest path
// //////////////////////////////////////////

/// \brief Computes the shortest path from all nodes back to source in the input graph.
/// \param graph The input graph.
/// \param source The source node.
/// \return The nodes constructing all the shortest path from any node to source.
Nodes ShortestPathPriv(const Graph &graph, const int &source)
{
  std::priority_queue<Connection, std::vector<Connection>, comp> pq;

  size_t N{graph.size()};
  Weights dist(N, kDblMax);
  Nodes prev(N, 0);

  dist[source] = 0.0;
  pq.push(Connection{source, 0.0});

  while (!pq.empty()) {
    Connection U{pq.top()};
    pq.pop();
    int u{U.node};

    for (const auto &v : graph[u]) {
      double alt = dist[u] + v.weight;
      if (alt < dist[v.node]) {
        dist[v.node] = alt;
        prev[v.node] = u;
        pq.push(Connection{v.node, alt});
      }
    }
  }

  return prev;
}

Nodes ShortestPathDijkstra(const Graph &graph, const int &source, const int &dest)
{
  // Forbidden input.
  if (dest > graph.size() || source > graph.size() || graph.size() < 2 || source < 0 || dest < 0 || source == dest) {
    return Nodes{};
  }

  const Nodes kNodes{ShortestPathPriv(graph, source)};
  Nodes path;
  int prev{dest};

  while (prev != source) {
    path.emplace_back(prev);
    prev = kNodes[prev];
  }

  path.emplace_back(source);
  std::reverse(path.begin(), path.end());
  return path;
}

// //////////////////////////////////////////
//  Bellman-Ford, shortest path
// //////////////////////////////////////////

std::pair<Weights, Nodes> ShortestPathBF(const Graph &graph, const int &source)
{
  // Forbidden input.
  if (source < 0 || graph.size() < 3 || source >= graph.size()) {
    return std::make_pair(Weights{}, Nodes{});
  }

  Weights dist(graph.size(), kDblMax);
  Nodes prev(graph.size(), -1);
  Edges edges{GetEdges(graph)};

  dist[source] = 0;// Distance to itself is zero

  for (size_t i = 1; i < dist.size(); i++) {
    Weights pre{dist};
    for (const auto &edge : edges) {
      if (dist[edge.u] + edge.w < dist[edge.v]) {
        dist[edge.v] = dist[edge.u] + edge.w;
        prev[edge.v] = edge.u;
      }
    }
    // Return earlier if no update
    if (std::equal(dist.begin(), dist.end(), pre.begin())) {
      break;
    }
  }

  // Check for negative-weight cycles (not good).
  for (auto edge : edges) {
    if (dist[edge.u] + edge.w < dist[edge.v]) {
      return std::make_pair(Weights{}, Nodes{});
    }
  }

  return std::make_pair(dist, prev);
}

std::pair<Nodes, double> ShortestPathBF(const Graph &graph, const int &source, const int &dest)
{
  // Forbidden input.
  if (source < 0 || source >= graph.size() || dest < 0 || dest >= graph.size() || graph.size() < 3) {
    return std::make_pair(Nodes{}, 0.0);
  }

  const std::pair<Weights, Nodes> kWeightsPaths{ShortestPathBF(graph, source)};
  const Nodes kNodes{kWeightsPaths.second};
  Nodes path;
  int prev{dest};

  while (prev != source) {
    path.emplace_back(prev);
    prev = kNodes[prev];
  }

  path.emplace_back(prev);
  std::reverse(path.begin(), path.end());
  return std::make_pair(path, kWeightsPaths.first[dest]);
}

// //////////////////////////////////////////
//  Edmonds-Karp, max-flow
// //////////////////////////////////////////

double FindMinOfResiduals(const Graph &graph, const Nodes &path)
{
  double min{kDblMax};
  Edges edges{GetEdges(graph)};

  for (size_t i = 0; i < path.size() - 1; ++i) {
    for (const auto &edge : edges) {
      if (edge.u == path[i] && edge.v == path[i + 1] && edge.w < min) {
        min = edge.w;
      }
    }
  }
  return min;
}

double MaxFlow(Graph graph, const int &source, const int &dest)
{
  // Forbidden input.
  if (source < 0 || dest < 0 || source >= graph.size() || dest >= graph.size() || source == dest) {
    return 0.0;
  }

  // Forbidden case.
  if (!ShortestPathBFS(graph, source, dest).is_path) {
    return 0.0;
  }

  double max_flow{0.0};
  bool is_path{true};
  Nodes prev;

  while (is_path) {
    Edges edges{GetEdges(graph)};

    for (const auto &edge : edges) {
      if (edge.w <= 0) {
        RemoveEdge(graph, edge.u, edge.v);
      }

      if (!graph[source].empty()) {
        Path pv{ShortestPathBFS(graph, source, dest)};
        Nodes path{pv.nodes};
        is_path = pv.is_path;

        if (is_path) {

          double min_capacity{FindMinOfResiduals(graph, path)};

          for (size_t i = 0; i < path.size() - 1; ++i) {
            // Get node in path
            int u{path[i]};
            int v{path[i + 1]};

            // Update weights
            SetWeight(graph, u, v, GetWeight(graph, u, v) - min_capacity);
            SetWeight(graph, v, u, min_capacity);
          }
          max_flow += min_capacity;
        }
      }
    }
  }
  return max_flow;
}

}// namespace algo::graph