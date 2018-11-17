#include "Graph.hpp"


using namespace std;

#ifndef LISTA_HPP
	#define LISTA_HPP


//class Graph;

class Node{
	GNode * data;
	Node * next;
	Node * prev;

	public:
		Node();
		~Node();
		void set_data(GNode*);
		void nullify();
		GNode * get_data();
		void set_next(Node*);
		Node* get_next();
		void set_prev(Node*);
		Node* get_prev();
		void print_node();
		void export_node(ofstream*);
		void export_edges(ofstream*);
};

class Lista{
	Node * head;
	Node * tail;
	int size;

	public:
		Lista();
		~Lista();
		void set_head(Node*);
		Node * get_head();
		void set_tail(Node*);
		Node * get_tail();
		void set_size(int );
		int get_size();
		void push_back(GNode*);
		void push_front(GNode* );
		Node * delete_node(int);
		Node * pop();
		void print_list();
		void export_nodes(ofstream*);
		void export_edges(ofstream*);

};

#endif

