#include "graph.h"
using namespace std;

Graph::Graph(int V){ 
	this->V = V;
	adj = new list<int>[V];
}

void Graph::addEdge(int u, int v){
	adj[u].push_back(v);
}

void Graph::addEdgeWeight(int u, int v, int w){
	auto p = pair<int,int>(u,v);
	ews.insert(pair<pair<int,int>,int>(p,w));	
}

map<pair<int,int>,int> Graph::getEdgeWeights() {
	return ews;
}


