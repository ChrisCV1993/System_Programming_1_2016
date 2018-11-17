#include <iostream>
#include <fstream>
#include "Lista.hpp"

using namespace std;

Node::Node(){
	data=NULL;
	next=NULL;
	prev=NULL;
	//cout<<"A Node has just been created"<<endl;
}

Node::~Node(){
	if(data!=NULL)
		delete data;
	next=NULL;
	prev=NULL;
	//cout<<"A Node has just been destroyed"<<endl;
}

void Node::set_data(GNode* g){
	data=g;
}

GNode * Node::get_data(){
	return data;
}

void Node::set_next(Node* n){
	next=n;
}

Node* Node::get_next(){
	return next;
}

void Node::set_prev(Node* n){
	prev=n;
}

Node* Node::get_prev(){
	return prev;
}

void Node::print_node(){
	data->print_data();
}

void Node::export_node(ofstream* myfile){
	data->export_nodes(myfile);
}

void Node::export_edges(ofstream* myfile){
	data->export_edges(myfile);
}

void Node::nullify(){
	data=NULL;
}

Lista::Lista(){
	head=NULL;
	tail=NULL;
	size=0;
	//cout<<"A lista has just been created"<<endl;
}

Lista::~Lista(){
	Node * temp;
	while(size>0){
		temp=pop();
		if(temp!=NULL)
			delete temp;
		//size--;
	}
	//cout<<"A lista has just been destroyed"<<endl;
}

void Lista::set_head(Node* h){
	head=h;
}

Node * Lista::get_head(){
	return head;
}

void Lista::set_tail(Node* t){
	tail=t;
}

Node * Lista::get_tail(){
	return tail;
}

void Lista::set_size(int s){
	size=s;
}

int Lista::get_size(){
	return size;
}

void Lista::push_back(GNode* n){
	Node * temp;
	temp=new Node;
	temp->set_data(n);
	if(size==0){
		head=temp;
		tail=temp;	
	}
	else{
		temp->set_prev(tail);
		tail->set_next(temp);
		tail=temp;
	}
	size++;
}

void Lista::push_front(GNode* n){
	Node * temp;
	temp=new Node;
	temp->set_data(n);
	if(size==0){
		head=temp;
		tail=temp;	
	}
	else{
		temp->set_next(head);
		head->set_prev(temp);
		head=temp;
	}
	size++;
}

Node * Lista::pop(){
	Node * temp;
	if(size>0){
		temp=head;
		head=head->get_next();
		//head->set_prev(NULL);
		size--;
		return temp;
	}
	return NULL;
}

Node * Lista::delete_node(int pk){
	if(size==0){
		return NULL;
	}
	else{
		int i=0;
		Node * tempnode;
		GNode * gnode;
		tempnode=head;
		gnode=tempnode->get_data();
		for(i=0;i<size;i++){
			if(gnode->get_id()==pk){
				if(tempnode==head){
					return pop();
				}
				else if(tempnode==tail){
					tail=tempnode->get_prev();
					size--;
					return tempnode;
				}
				else{
					Node* thenext;
					Node* theprev;
					theprev=tempnode->get_prev();
					theprev->set_next(tempnode->get_next());
					thenext=tempnode->get_next();
					thenext->set_prev(tempnode->get_prev());
					size--;
					return tempnode;
				}
			}
			tempnode=tempnode->get_next();
			gnode=tempnode->get_data();
		}
		return NULL;
	}
}

void Lista::print_list(){
	int i=0;
	if(size==0)
		return;
	Node * temp;
	temp=head;
	while(i<size){
		temp->print_node();
		temp=temp->get_next();
		i++;
	}
}

void Lista::export_nodes(ofstream* myfile){
	int i=0;
	if(size==0)
		return;
	Node * temp;
	temp=head;
	while(i<size){
		temp->export_node(myfile);
		temp=temp->get_next();
		i++;
	}
}

void Lista::export_edges(ofstream* myfile){
	int i=0;
	if(size==0)
		return;
	Node * temp;
	temp=head;
	while(i<size){
		temp->export_edges(myfile);
		temp=temp->get_next();
		i++;
	}
}