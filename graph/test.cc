#include "graph.h"
#include "graph.cc"
#include "bfs.cc"
#include "dfs.cc"
#include "dijkstra.cc"
#include "prim.cc"
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
using namespace std;

bool test_BFS(Graph g){
	vector<int> values = {0,1,2,3,4,5,6};
	vector<int> res = g.BFS(0);
	bool comp = equal(res.begin(),res.end(),values.begin());
	return comp;
}

bool test_DFS(Graph g){
	vector<int> values = {0,1,3,4,2,5,6};
	vector<int> res = g.DFS(0);
	bool comp = equal(res.begin(),res.end(),values.begin());
	return comp;
}


bool test_dijkstra() {
	Graph g(7);
	g.addEdge(0,5,1);
	g.addEdge(0,10,2);
	g.addEdge(2,2,4);
	g.addEdge(1,3,4);
	g.addEdge(1,6,3);
	g.addEdge(4,2,3);
	g.addEdge(3,6,5);
	g.addEdge(4,2,6);
	g.addEdge(6,2,5);
	vector<uint> dist = g.dijkstra(0);
	int res1 = dist[5];
	bool a = res1 == 12;

	Graph g2(6);
	g2.addEdge(0,2,2);
	g2.addEdge(0,4,1);
	g2.addEdge(1,1,2);
	g2.addEdge(1,5,3);
	g2.addEdge(2,8,3);
	g2.addEdge(2,10,4);
	g2.addEdge(4,3,5);
	g2.addEdge(3,2,4);
	g2.addEdge(3,6,5);
	
	vector<uint> dist2 = g2.dijkstra(0);
	int res2 = dist2[5];
	bool b = res2 == 13;

	return a && b;
}

bool test_prim() {
	Graph g(7);
	g.addEdge(0,5,3);
	g.addEdge(0,7,1);
	g.addEdge(1,8,2);
	g.addEdge(1,7,4);
	g.addEdge(2,5,4);
	g.addEdge(3,15,4);
	g.addEdge(3,6,5);
	g.addEdge(4,9,6);
	g.addEdge(5,8,4);
	g.addEdge(5,11,6);
	g.addEdge(3,9,1);
	vector<uint> net1 =g.prim(0);
	uint sum1 = accumulate(net1.begin(),net1.end(),0);
	bool a = sum1 == 39;

	Graph g2(9);
	g2.addEdge(0,10,1);
	g2.addEdge(0,12,2);
	g2.addEdge(1,9,2);
	g2.addEdge(2,3,4);
	g2.addEdge(2,1,5);
	g2.addEdge(1,8,3);
	g2.addEdge(4,7,3);
	g2.addEdge(4,3,5);
	g2.addEdge(3,5,7);
	g2.addEdge(5,6,7);
	g2.addEdge(3,8,6);
	g2.addEdge(6,9,7);
	g2.addEdge(6,2,8);
	g2.addEdge(7,11,8);
	vector<uint> net2 =g2.prim(0);
	uint sum2 = accumulate(net2.begin(),net2.end(),0);
	bool b = sum2 == 43;
	

	Graph g3(7);
	g3.addEdge(0,16,1);
	g3.addEdge(0,21,3);
	g3.addEdge(0,12,2);
	g3.addEdge(1,20,4);
	g3.addEdge(1,17,3);
	g3.addEdge(2,28,3);
	g3.addEdge(3,18,4);
	g3.addEdge(3,23,6);
	g3.addEdge(3,19,5);
	g3.addEdge(2,31,5);
	g3.addEdge(4,11,6);
	g3.addEdge(5,27,6);
	vector<uint> net3 = g3.prim(0);
	int sum3 = accumulate(net3.begin(),net3.end(),0);
	bool c = sum3 == 93;
	
	return a && b && c;

}

//Test-case file is downloaded from "Project Euler"
//Problem 107
void test_prim2(){
	Graph g(40);
	vector<int> total;
	ifstream infile("testfiles/p107_network.txt");
	string line;
	stringstream ss;
	int row = 0;
	int col = 0;
	int w = 0;
	while (getline(infile, line)){	
		string s = line;
		string delimiter = ",";

		size_t pos = 0;
		string token;
		while ((pos = s.find(delimiter)) != string::npos) {
		    	token = s.substr(0, pos);
		    
			try {
				int weight = stoi(token);
				total.push_back(weight);
				g.addEdge(row,weight,col); 

			} catch (std::invalid_argument&) { }

		    	s.erase(0, pos + delimiter.length());
			col++;
		}

		try {

			int weight = stoi(s);
			g.addEdge(row,weight,col);
			
		} catch (std::invalid_argument&) { }

		row++;
		col = 0;
	}

	int totsum = accumulate(total.begin(),total.end(),0);
	cout << "totsum " << totsum <<endl;
	vector<uint> weights = g.prim(0);
	int wsum = accumulate(weights.begin(),weights.end(),0);
	cout << "wsum " << wsum << endl;
	int saved = totsum - wsum;
	cout << "saved " << saved << endl;

}


int main() {
	Graph g(7);
	g.addEdge(0, 1);
	g.addEdge(0, 2);
	g.addEdge(1, 3);
	g.addEdge(1, 4);
	g.addEdge(2, 5);
	g.addEdge(2, 6);
	g.BFS(0);
	cout << endl;

	if (test_BFS(g)) {
		cout << "BFS pass" << endl;
	} else {
		cout << "BFS fail" << endl;
	}

	if (test_DFS(g)) {
		cout << "DFS pass" << endl;
	} else {
		cout << "DFS fail" << endl;
	}
	
	if (test_dijkstra()) {
		cout << "Dijkstra pass" << endl;
	} else {
		cout << "Dijkstra fail" << endl;
	}

	if (test_prim()) {
		cout << "Prim pass" << endl;
	} else {
		cout << "Prim fail" << endl;
	}
	test_prim2();
		
 	
}	
