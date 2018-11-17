#include "Graph.hpp"
#include "Lista.hpp"


using namespace std;

#ifndef HASHTABLE_HPP
	#define HASHTABLE_HPP


class Bucket{
	Lista * lista;

	public:
		Bucket();
		~Bucket();
		void print_bucket();
		void insert_bucket(GNode *);
		void export_bucket_nodes(ofstream *);
		void export_bucket_edges(ofstream *);
		Lista * get_lista();
};

class Hashtable{
	Bucket ** table;
	int size;

	public:
			Hashtable(int);
			~Hashtable();
			int hash_function(int);
			void insert_hash(int );
			Node * delete_node(int );
			Node * exists(int);
			int get_size();
			void print_hash();
			void export_hash(ofstream * );
};

#endif