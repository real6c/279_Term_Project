/*****************************************

 * By submitting this file, I affirm that
 * I am the author of all modifications to
 * the provided code.
 *
 * The following is a list of those students
 * I had worked together in preparing this project:
 *    - Jake Sakamoto
 *    - Lukas Real
 *****************************************/

 #ifndef WEIGHTED_GRAPH_H
 #define WEIGHTED_GRAPH_H
 
 #ifndef nullptr
 #define nullptr 0
 #endif
 
 #include <iostream>
 #include <limits>
 #include "Exception.h"
 
 class Weighted_graph {
	 private:
		 int n;
		 int num_edges;
		 int *degrees;
		 double **adj;
		 static const double INF;
 
	 public:
		 Weighted_graph( int = 50 );
		 ~Weighted_graph();
 
		 int degree( int ) const;
		 int edge_count() const;
		 double adjacent( int, int ) const;
		 double distance( int, int );
 
		 void insert( int, int, double );
 
	 // Friends
 
	 friend std::ostream &operator<<( std::ostream &, Weighted_graph const & );
 };
 
 const double Weighted_graph::INF = std::numeric_limits<double>::infinity();
 
 //Graph with v vertices. If v happens to be less than or equal to 0, use 1 vertex instead
 Weighted_graph::Weighted_graph(int v){
	 if(v <= 0){
		 v = 1;
	 }
 
	 n = v;
	 num_edges = 0;
 
	 degrees = new int[n];
	 for(int i = 0; i < n; i++){
		 degrees[i] = 0;
	 }
 
	 adj = new double *[n];
	 for(int i = 0; i < n; i++){
		 adj[i] = new double[n];
		 for(int j = 0; j < n; j++){
			 adj[i][j] = INF;
		 }
	 }
 }
 
 //frees the adjacency matrix rows and columns and the degrees array
 Weighted_graph::~Weighted_graph(){
	 for(int i = 0; i < n; i++){
		 delete[] adj[i];
	 }
	 delete[] adj;
	 delete[] degrees;
 }
 
 //returns the degree of vertex 
 int Weighted_graph::degree(int v) const{
	 if(v < 0 || v >= n){ //handling for edgecase where vertex is not within range
		 throw illegal_argument();
	 }
	 return degrees[v];
 }
 
 //returns the number of edges in the graph
 int Weighted_graph::edge_count() const{
	 return num_edges;
 }
 
 //returns the weight of the edge between m and v: 0 if m == v, infinity if the edge is missing
 double Weighted_graph::adjacent(int m, int v) const{
	 if(m < 0 || m >= n || v < 0 || v >= n ){ //handling for edgecase where m or v is not within range
		 throw illegal_argument();
	 }
 
	 if(m == v){
		 return 0.0;
	 }
 
	 return adj[m][v];
 }
 
 //insert/replace undirected edge of a weight w between m and v
 void Weighted_graph::insert(int m, int v, double w){
	 if(m < 0 || m >= n || v < 0 || v >= n || m == v){ //handling for edgecase where m, v, or is not within range
		 throw illegal_argument();
	 }
	 if(w <= 0 || w == INF){
		 throw illegal_argument();
	 }
 
	 if(adj[m][v] == INF){ //if there isn't already an edge between m and v, then we need to update num_edges and degrees
		 num_edges++;
		 degrees[m]++;
		 degrees[v]++;
	 }
 
	 adj[m][v] = w;
	 adj[v][m] = w;
 }
 
 //returns the shortest path distance from s to t using Dijkstra: 0 if s == t, infinity if s does not reach t
 double Weighted_graph::distance(int s, int t){
	 if(s < 0 || s >= n || t < 0 || t >= n ){
		 throw illegal_argument(); 
	 }
 
	 if(s == t){
		 return 0.0;
	 }
 
	 double *dist = new double[n];
	 bool *visited = new bool[n];
 
	 for( int i = 0; i < n; i++){ //initializing dist and visited arrays
		 dist[i] = INF;
		 visited[i] = false;
	 }
	 dist[s] = 0.0;
 
	 for(int k = 0; k < n; k++){
		 //picks the unvisited vertex with the smallest current distance
		 int u = -1;
		 double min_d = INF;
		 for(int i = 0; i < n; i++){
			 if (!visited[i] && dist[i] < min_d){
				 min_d = dist[i];
				 u = i;
			 }
		 }
 
		 if(u == -1){  
			 break;
		 }
		 if(u == t){
			 break;
		 }
 
		 visited[u] = true;
 
		 //update the distances to the neighbors of u
		 for(int i = 0; i < n; i++){
			 if(!visited[i] && adj[u][i] != INF){
				 double nd = dist[u] + adj[u][i];
				 if(nd < dist[i] ){
					 dist[i] = nd;
				 }
			 }
		 }
	 }
 
	 double result = dist[t]; //storing the result before cleaning memory
	 delete[] dist; //cleaning up memory
	 delete[] visited;
	 return result;
 }
 
 // You can modify this function however you want:  it will not be tested
 
 std::ostream &operator<<( std::ostream &out, Weighted_graph const &graph ) {
	 return out;
 }
 
 #endif