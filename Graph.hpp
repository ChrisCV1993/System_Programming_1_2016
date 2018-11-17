
using namespace std;

#ifndef GRAPH_HPP
	#define GRAPH_HPP

class GNode;
class ELista;


class GEdge{
	GNode * to;
	double value ;

	public:
		GEdge(GNode*,double);
		~GEdge();
		GNode * get_node();
		double get_value();
		void set_value(double);
		void print_data();
};

class GNode{
	
	int id;
	bool explored;
	GNode * tempdad;
	ELista * outboundlist;
	ELista * inboundlist;

	public:
			GNode(int);
			~GNode();
			void insert_out_edge(GNode*,double);			//insert an outbound edge
			GEdge * get_spec_out_edge(GNode *);			//get a specified outbound efge
			void insert_in_edge(GNode*,double);			//insert an inbound edge
			GEdge * get_spec_in_edge(GNode *);			//get a specified outbound efge
			int get_id();
			void print_data();
			bool get_explored();
			void set_explored(bool);
			GNode * get_dad(); 
			void set_dad(GNode*);
			ELista * get_outlist();
			ELista * get_inlist();
			void export_nodes(ofstream *);
			void export_edges(ofstream *);
			void search_conn(GNode *);					//uses BFS
			void search_cycle(GNode *);					//uses DFS
			void traceflow(int,int);					//uses DLS
			void triangle(int ,int ,double ,GNode * );		//uses DLS

};

class ENode{
	GEdge * data;
	ENode * next;
	ENode * prev;

	public:
		ENode();
		~ENode();
		void set_data(GEdge*);
		GEdge * get_data();
		void set_prev(ENode*);
		ENode* get_prev();
		void set_next(ENode*);
		ENode* get_next(); 
		void print_node();
};

class ELista{
	ENode * head;
	ENode * tail;
	int size;

	public:
		ELista();
		~ELista();
		void set_head(ENode*);
		ENode * get_head();
		void set_tail(ENode*);
		ENode * get_tail();
		void set_size(int );
		int get_size();
		void push_back(GEdge*);
		ENode * delete_edge(GNode*);
		double get_totalsum();
		ENode * pop();
		void print_list();

};

#endif