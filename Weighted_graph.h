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

		 // Neighbor indices per vertex (sparse structure), entries are added only when
		 // a new edge appears, weight changes update the adjacency matrix only
		 int **nbr_list;

		 // Reused by distance() to avoid allocating dist/visited arrays on every call
		 double *dist_buf;
		 bool   *vis_buf;

		 static const double INF;

	 public:
		 Weighted_graph( int = 50 );
		 ~Weighted_graph();

		 int degree( int ) const;
		 int edge_count() const;
		 double adjacent( int, int ) const;
		 double distance( int, int );

		 void insert( int, int, double );

	 // friend

	 friend std::ostream &operator<<( std::ostream &, Weighted_graph const & );
 };

 const double Weighted_graph::INF = std::numeric_limits<double>::infinity();

 // Construct a graph with v vertices labeled 0 through v-1 and if v <= 0, use v = 1
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

	 // At most n-1 neighbors per vertex preallocate one int row per vertex
	 nbr_list = new int *[n];
	 for(int i = 0; i < n; i++){
		 nbr_list[i] = new int[n];
	 }

	 dist_buf = new double[n];
	 vis_buf  = new bool[n];
 }

 // Release adjacency rows, neighbor rows, then pointer arrays and buffers
 Weighted_graph::~Weighted_graph(){
	 for(int i = 0; i < n; i++){
		 delete[] adj[i];
		 delete[] nbr_list[i];
	 }
	 delete[] adj;
	 delete[] nbr_list;
	 delete[] degrees;
	 delete[] dist_buf;
	 delete[] vis_buf;
 }

 // Degree of vertex v (number of incident edges) from the maintained degree[] array
 int Weighted_graph::degree(int v) const{
	 if(v < 0 || v >= n){
		 throw illegal_argument();
	 }
	 return degrees[v];
 }

 // Total undirected edges; counter updated in insert()
 int Weighted_graph::edge_count() const{
	 return num_edges;
 }

 // Weight of edge (m, v): 0 if m == v, infinity if no edge, otherwise adj[m][v]
 double Weighted_graph::adjacent(int m, int v) const{
	 if(m < 0 || m >= n || v < 0 || v >= n ){
		 throw illegal_argument();
	 }

	 if(m == v){
		 return 0.0;
	 }

	 return adj[m][v];
 }

 // Add or update an undirected edge between m and v with weight w
 void Weighted_graph::insert(int m, int v, double w){
	 if(m < 0 || m >= n || v < 0 || v >= n || m == v){
		 throw illegal_argument();
	 }
	 if(w <= 0 || w == INF){
		 throw illegal_argument();
	 }

	 if(adj[m][v] == INF){ // For new edge extend neighbor lists and bump edge/degree counts
		 nbr_list[m][degrees[m]] = v;
		 nbr_list[v][degrees[v]] = m;
		 num_edges++;
		 degrees[m]++;
		 degrees[v]++;
	 }

	 adj[m][v] = w;
	 adj[v][m] = w;
 }

 // Shortest path distance from s to t (dijkstra) and unreachable targets yield infinity
 double Weighted_graph::distance(int s, int t){
	 if(s < 0 || s >= n || t < 0 || t >= n ){
		 throw illegal_argument();
	 }

	 if(s == t){
		 return 0.0;
	 }

	 for(int i = 0; i < n; i++){
		 dist_buf[i] = INF;
		 vis_buf[i]  = false;
	 }
	 dist_buf[s] = 0.0;

	 for(int k = 0; k < n; k++){
		 // Choose the unvisited vertex with minimum tentative distance
		 int u = -1;
		 double min_d = INF;
		 for(int i = 0; i < n; i++){
			 if(!vis_buf[i] && dist_buf[i] < min_d){
				 min_d = dist_buf[i];
				 u = i;
			 }
		 }

		 if(u == -1){
			 break;
		 }
		 if(u == t){
			 break;
		 }

		 vis_buf[u] = true;

		 // Relax edges to neighbors listed for u, weights read from adjacency matrix
		 int deg_u = degrees[u];
		 for(int i = 0; i < deg_u; i++){
			 int nb = nbr_list[u][i];
			 if(!vis_buf[nb]){
				 double nd = min_d + adj[u][nb];
				 if(nd < dist_buf[nb]){
					 dist_buf[nb] = nd;
				 }
			 }
		 }
	 }

	 return dist_buf[t];
 }

 // You can modify this function however you want:  it will not be tested

 std::ostream &operator<<( std::ostream &out, Weighted_graph const &graph ) {
	 return out;
 }

 #endif
