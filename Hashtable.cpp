#include <iostream>
#include <fstream>
#include "Hashtable.hpp"

using namespace std;

Bucket::Bucket(){
	lista=new Lista;
	//cout<<"A Bucket has just been created"<<endl;
}

Bucket::~Bucket(){
	delete lista;
	//cout<<"A Bucket has just been destroyed"<<endl;
}

void Bucket::print_bucket(){
	if(lista!=NULL)
		lista->print_list();
}

void Bucket::insert_bucket(GNode * g ){
	lista->push_back(g);
}

Lista * Bucket::get_lista(){
	return lista;
}

void Bucket::export_bucket_nodes(ofstream * myfile){
	if(lista!=NULL)
		lista->export_nodes(myfile);
}
void Bucket::export_bucket_edges(ofstream * myfile){
	if(lista!=NULL)
		lista->export_edges(myfile);
}

Hashtable::Hashtable(int sz){
	int i=0;
	size=sz;
	table=new Bucket*[sz];
	for(i=0;i<sz;i++){
		table[i]=NULL;
	}
	//cout<<"A hashtable has just been created"<<endl;
}

Hashtable::~Hashtable(){
	int i;

	for(i=0;i<size;i++){
		if (table[i]!=NULL)
			delete table[i];
		table[i]=NULL;
	}
	delete[] table;
	//cout<<"A hashtable has just been destroyed"<<endl;
}

int Hashtable::hash_function(int pk){
	return pk%size;
}

Node * Hashtable::exists(int pk){										//checks if a node with the given pk exists
	int res;
	res=hash_function(pk);												//find its position

	if(table[res]==NULL)
		return NULL;
	else{
		Lista * templist;
		Node * tempnode;
		GNode * tempg;
		int tempcounter;
		int i=0;

		templist=table[res]->get_lista();
		tempcounter=templist->get_size();
		if(tempcounter==0)
			return NULL;
		else{
			tempnode=templist->get_head();
			while(i<tempcounter){
				tempg=tempnode->get_data();
				if(tempg->get_id()==pk)
					return tempnode;									//return the node else null
				i++;
				tempnode=tempnode->get_next();
				if(tempnode==NULL)
					break;
			}
		}
	}
	return NULL;
}

void Hashtable::insert_hash(int pk){
	int res;
	res=hash_function(pk);												//find in which bucket the new node would be inserted
	GNode * gnode;
	gnode=new GNode(pk);												//create a graph node an insert to a bucket 
	if(table[res]==NULL){
		table[res]=new Bucket;
	}
	table[res]->insert_bucket(gnode);
}

Node * Hashtable::delete_node(int pk){
	int res;
	res=hash_function(pk);
	GNode * gnode;
	Lista * templist;
	//gnode=new GNode(pk);
	if(table[res]==NULL){
		return NULL;
	}
	templist=table[res]->get_lista();
	if(templist==NULL)
		return NULL;
	else
		return templist->delete_node(pk);

}

int Hashtable::get_size(){
	return size;
}

void Hashtable::print_hash(){
	int i ;
	for(i=0;i<size;i++){
		if(table[i]!=NULL){
			table[i]->print_bucket();
		}
	}
}

void Hashtable::export_hash(ofstream * myfile){
	int i;
	for(i=0;i<size;i++){
		if(table[i]!=NULL){
			table[i]->export_bucket_nodes(myfile);
		}
	}
	for(i=0;i<size;i++){
		if(table[i]!=NULL){
			table[i]->export_bucket_edges(myfile);
		}
	}
}