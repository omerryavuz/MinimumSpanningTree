//============================================================================
// Name        : hw3.cpp
// Author      : Ömer Yavuz
// Version     : ceng315-hw3
//============================================================================

#include <iostream>
#include <queue>
#include <fstream>
#include <vector>
#include <string.h>
#include <limits.h>

using namespace std;

typedef pair<string, int> vertex_hash;

class Edge{
	public:
		int from;
		int to;
		double distance;
		int position;
};

class Vertex{
	public:
		int id;
		double cost;
		string name;
		bool known;
		int path;
		Edge pathEdge;
		vector<Edge> adjacentList;

};

class CompareVertex{
	public:
		bool operator()(Vertex &v1,Vertex &v2){
			if(v1.cost > v2.cost){
				return true;
			}
			return false;
		}
};

int getId(vector<vertex_hash> VertexList,string v){
	for(unsigned int i=0;i<VertexList.size();i++){
		if(VertexList[i].first == v){
			return VertexList[i].second;
		}
	}
	return -1;
}



void shellsort(vector<Edge>  &all_edges){
	int length = all_edges.size();
	int h = 1;
	while(h<length/2){
		h = 2*h+1;
	}
	while(h>0){
		for(int i=h;i<length;i++){
			for(int j=i;j>=h && all_edges[j].distance < all_edges[j-h].distance ; j=j-h){
				swap(all_edges[j],all_edges[j-h]);
			}
		}
		h = h/2;
	}
}
class MyPriorityQueue{
	private:
		vector<Vertex> list;
	public:
		int getParent(int p){
			return (p+1)/2 - 1;
		}
		int leftChild(int p){
			return (p+1)*2 - 1;
		}
		int rightChild(int p){
			return (p+1)*2 ;
		}
		int length(){
			return list.size();
		}
		void swap(Vertex &a,Vertex &b){
			Vertex tmp = a;
			a = b;
			b = tmp;
		}
		void insert(Vertex n){
			list.push_back(n);
			int child = list.size()-1;
			int parent ;
			while(child > 0 ){
				parent = getParent(child);
				if(list[child].cost < list[parent].cost){
					swap(list[child],list[parent]);
					child = parent;
				}
				else{
					break;
				}
			}
		}
		void pop(){
			if(list.size() == 0){
				return ;
			}
			list[0] = list[list.size()-1];
			int l = list.size();
			list.resize(l-1);
			int left,right,parent ;
			parent = 0;
			int len = list.size();
			int min ;
			while(parent < len/2){
				left = leftChild(parent);
				right= rightChild(parent);
				if(left < len && list[left].cost < list[parent].cost){
					min = left;
				}
				else{
					min = parent;		
				}
				if(right < len && list[right].cost < list[min].cost){
					min = right;
				}
				if(min != parent){
					swap(list[min],list[parent]);
					parent = min;
				}
				else{
					break;
				}
			}
			
		}
		Vertex top(){
			return list[0];
		}
		bool isEmpty(){
			if(list.size() == 0){
				return true;
			}
			return false;
		}
		void printQueue(){
			for(unsigned int i=0;i<list.size();i++){
				cout << list[i].cost << " " ;
			}
			cout << endl;
		}
};


vector<Edge> dijkstra(vector<Vertex> VertexList,int source , int destination){
	
	priority_queue<Vertex,vector<Vertex>,CompareVertex> pq;
//	MyPriorityQueue myq;
	
	for(unsigned int i=0;i<VertexList.size();i++){
		VertexList[i].cost = INT_MAX;
		VertexList[i].path = -1;
		VertexList[i].known = false;
	}
	VertexList[source].cost = 0;
	pq.push(VertexList[source]);
	
//	myq.insert(VertexList[source]);
	
	while(!pq.empty()/*!myq.isEmpty()*/){
		Vertex top = pq.top();
		pq.pop();
		
	//	Vertex top = myq.top();
	//	myq.pop();
		 
		top.known = true;
		for(unsigned int i=0;i<top.adjacentList.size();i++){
			if(VertexList[top.adjacentList[i].to].cost > top.adjacentList[i].distance + top.cost  ){
				VertexList[top.adjacentList[i].to].cost = top.adjacentList[i].distance + top.cost;
				VertexList[top.adjacentList[i].to].path = top.id;
				VertexList[top.adjacentList[i].to].pathEdge = top.adjacentList[i];
				if(VertexList[top.adjacentList[i].to].known == false){
					pq.push(VertexList[top.adjacentList[i].to]);
				//	myq.insert(VertexList[top.adjacentList[i].to]);
				}
			}
		}
	}
	
	int fstid = VertexList[destination].path;
	Edge e = VertexList[destination].pathEdge;
	vector<Edge> pathEdge;
	while(fstid != -1){
		pathEdge.push_back(e);
		e = VertexList[fstid].pathEdge;
		fstid = VertexList[fstid].path;
		
	}
	return pathEdge;
}

int main() {
	ifstream inputFile;
	inputFile.open(("hw3.inp"));	
	int N,M,K;
	
	vector<Edge> all_Edges;
	vector<Edge> heavy_traffic;
	vector<int> accepted_edges;
	vector<int> algo_array;
	vector<Vertex> VertexList;
	
	int hash = 48;
	
	vector< vector<vertex_hash> > vertexNames(48);
	
	if(inputFile.is_open()){
		
		inputFile >> N;
		inputFile >> M;
		inputFile >> K;
		string tmp;
		string tmp2;
		double dis;
		Edge e;
		vertex_hash v_h;
		for(int i=0;i<N ; i++){
			inputFile >> tmp;
			algo_array.push_back(-1);
		
			v_h.first = tmp;
			v_h.second = i;
			vertexNames[tmp[0]%hash].push_back(v_h);
			
			Vertex v;
			v.id = i;
			v.name = tmp;
			VertexList.push_back(v);
			
		}
		cout << N << endl;
		cout << M << endl;
		
		for(int i=0;i<M ; i++){
			cout << "----------" << endl;
			inputFile >> tmp;
			inputFile >> tmp2;
			inputFile >> dis;
			cout << tmp << " " << tmp2 << " " << dis << endl;
			e.from 	= getId(vertexNames[tmp[0]%hash],tmp);
			e.to 	= getId(vertexNames[tmp2[0]%hash],tmp2);
			e.distance = dis;
			e.position = i;
			all_Edges.push_back(e);
		
			VertexList[e.from].adjacentList.push_back(e);
			int t = e.from;
			e.from = e.to;
			e.to = t;
			VertexList[e.from].adjacentList.push_back(e);
			
		}
		cout << "------" << endl;
		cout << M << endl ;
		for(int i=0;i<K ; i++){
			inputFile >> tmp;
			inputFile >> tmp2;
			e.from 	= getId(vertexNames[tmp[0]%hash],tmp);
			e.to 	= getId(vertexNames[tmp2[0]%hash],tmp2);
			e.distance = -1;
			heavy_traffic.push_back(e);
		}
	}
	else{
		cout << "File cannot open!" << endl;
	}
	for(unsigned int i=0;i<heavy_traffic.size();i++){
		
		vector<Edge> path = dijkstra(VertexList,heavy_traffic[i].from,heavy_traffic[i].to);
		for(int i=path.size()-1;i>=0;i--){
			all_Edges[path[i].position].distance = INT_MIN;
		}
		
	}
	
	shellsort(all_Edges);

	int set1,set2;
	int value1,value2;
	int numberOfShortestEdges = 0;
	for(unsigned int i=0;i<all_Edges.size();i++){
		if(all_Edges[i].distance == INT_MIN){
			numberOfShortestEdges++;
		}
		else{
			break;
		}
	}
	bool connected = false;
	
	for(int i=0;i<numberOfShortestEdges ;i++){

		set1 = all_Edges[i].from;
		value1 = algo_array[all_Edges[i].from];
	
		while(value1 > -1){
			set1 = value1;
			value1 = algo_array[set1];
		}
		set2 = all_Edges[i].to;
		value2 = algo_array[all_Edges[i].to];
	
		while(value2 > -1){
			set2 = value2;
			value2 = algo_array[set2];
		}
		if(set1 != set2){
			accepted_edges.push_back(i);
			if(algo_array[set1] >= algo_array[set2] ){
				algo_array[set2] += algo_array[set1];
				algo_array[set1] = set2;
				if(algo_array[set2] == -N){
					connected = true;
					break;
				}
			}
			else{
				algo_array[set1] += algo_array[set2];
				algo_array[set2] = set1;
				if(algo_array[set1] == -N){
					connected = true;
					break;
				}
			}
		}
		else if(set1 == set2){
				accepted_edges.push_back(i);
		}
	}	
	
	for(unsigned int i=numberOfShortestEdges;i<all_Edges.size();i++){

		set1 = all_Edges[i].from;
		value1 = algo_array[all_Edges[i].from];
	
		while(value1 > -1){
			set1 = value1;
			value1 = algo_array[set1];
		}
		set2 = all_Edges[i].to;
		value2 = algo_array[all_Edges[i].to];
	
		while(value2 > -1){
			set2 = value2;
			value2 = algo_array[set2];
		}

		if(set1 != set2){
			accepted_edges.push_back(i);
			if(algo_array[set1] >= algo_array[set2] ){
				algo_array[set2] += algo_array[set1];
				algo_array[set1] = set2;
				if(algo_array[set2] == -N){
					connected = true;
					break;
				}
			}
			else{
				algo_array[set1] += algo_array[set2];
				algo_array[set2] = set1;
				if(algo_array[set1] == -N){
					connected = true;
					break;
				}
			}
		}
	
	}
	
	ofstream output ;
	output.open("hw3.out");
	if(connected == true){
		
		if(accepted_edges.size() > 0){
			int e = accepted_edges[0];
			output << VertexList[all_Edges[e].from].name <<" " <<  VertexList[all_Edges[e].to].name ;
		}
		
		for(unsigned int i=1;i<accepted_edges.size();i++){
			output << endl;
			int e = accepted_edges[i];
			output << VertexList[all_Edges[e].from].name <<" " <<  VertexList[all_Edges[e].to].name ;
		}
	}
	output.close();
	
	return 0;
}
