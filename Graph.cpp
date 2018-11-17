#include <iostream>
#include <stdio.h>
#include <fstream>
#include "Graph.hpp"
#include "Lista.hpp"

using namespace std;


GEdge::GEdge(GNode* t,double val){
	to=t;
	value=val;
	//cout<<"A GEdge has just been created"<<endl;
}

GEdge::~GEdge(){
	to=NULL;
	//cout<<"A GEdge has just been destroyed"<<endl;
}

double GEdge::get_value(){
	return value;
}

GNode * GEdge::get_node(){
	return to;
}

void GEdge::set_value(double val){
	value=val;
}

void GEdge::print_data(){
	cout<<to->get_id()<<endl;
}

GNode::GNode(int num){
	id = num;
	explored=0;
	tempdad=NULL;
	outboundlist=NULL;
	inboundlist=NULL;
	//cout<<"A GNode has just been created"<<endl;
}

GNode::~GNode(){
	if(outboundlist!=NULL)
		delete outboundlist;
	if(inboundlist!=NULL)
		delete inboundlist;
	//cout<<"A GNode has just been destroyed"<<endl;
}
		
GEdge * GNode::get_spec_out_edge(GNode *to){						//return a specific edge from this node to node given
	int size,i;
	GEdge * tempedge;
	ENode * tempnode;
	if(outboundlist==NULL)
		return NULL;
	size = outboundlist->get_size();
	tempnode = outboundlist->get_head();
	for(i=0;i<size;i++){
		tempedge=tempnode->get_data();
		if(tempedge->get_node()==to)
			return tempedge;
		tempnode=tempnode->get_next();
	}
	return NULL;
}	

GEdge * GNode::get_spec_in_edge(GNode *to){						//return a sepecific edge to this node from the node given
	int size,i;		
	GEdge * tempedge;
	ENode * tempnode;
	if(inboundlist==NULL)
		return NULL;
	size = inboundlist->get_size();
	tempnode = inboundlist->get_head();
	for(i=0;i<size;i++){
		tempedge=tempnode->get_data();
		if(tempedge->get_node()==to)
			return tempedge;
		tempnode=tempnode->get_next();
	}
	return NULL;
}	

void GNode::insert_out_edge(GNode * to,double val){			//adds one edge from this node to the node given
	GEdge * newedge;
	newedge = new GEdge(to,val);
	if(outboundlist==NULL)
		outboundlist=new ELista;
	outboundlist->push_back(newedge);
}

void GNode::insert_in_edge(GNode * to,double val){				//adds one inbound edge
	GEdge * newedge;
	newedge = new GEdge(to,val);
	if(inboundlist==NULL)
		inboundlist=new ELista;
	inboundlist->push_back(newedge);
}

int GNode::get_id(){
	return id;
}

GNode * GNode::get_dad(){
	return tempdad;
}
			
void GNode::set_dad(GNode* td){
	tempdad=td;
}

bool GNode::get_explored(){
	return explored;
}
			
void GNode::set_explored(bool b){
	explored=b;
}

ELista * GNode::get_inlist(){
	return inboundlist;
}

ELista * GNode::get_outlist(){
	return outboundlist;
}

void GNode::export_nodes(ofstream* myfile){							//used in dump for write nodes in file
	*myfile<<"createnodes "<<id<<endl;
}

void GNode::export_edges(ofstream* myfile){							//used in dump for writing edges in file		
	int i=0;
	ENode *temp;
	GEdge *tempedge;
	GNode *tempnode;
	if(outboundlist==NULL){
		cout<<endl;
		return;
	}
	if(outboundlist->get_size()==0){
		cout<<endl;
		return;
	}
	temp=outboundlist->get_head();
	tempedge=temp->get_data();
	tempnode=tempedge->get_node();
	for(i=0;i<outboundlist->get_size();i++){
		*myfile<<"addtran "<<id<<" "<<tempnode->get_id()<<" "<<tempedge->get_value()<<endl;
		temp=temp->get_next();
		if(temp==NULL)
			break;
		else{
			tempedge=temp->get_data();
			tempnode=tempedge->get_node();
		}
	}
}

void GNode::print_data(){												//print data in a readable form
	int i =0;
	ENode *temp;
	GEdge *tempedge;
	GNode *tempnode;
	cout<<"vertex("<<id<<") = ";
	if(outboundlist==NULL){
		cout<<endl;
		return;
	}
	if(outboundlist->get_size()==0){
		cout<<endl;
		return;
	}
	temp=outboundlist->get_head();
	tempedge=temp->get_data();
	tempnode=tempedge->get_node();
	for(i=0;i<outboundlist->get_size();i++){
		if(i>0)
			cout<<",";
		cout<<"("<<tempnode->get_id()<<","<<tempedge->get_value()<<")";
		temp=temp->get_next();
		if(temp==NULL)
			break;
		else{
			tempedge=temp->get_data();
			tempnode=tempedge->get_node();
		}

	}
	cout<<endl;
}
		
void GNode::search_conn(GNode * targetnode){							//used for finding the path between this node and the node given
	int i;		
	Lista * queue;
	Lista * exploredlist;
	Node * temp;
	ENode * tempedgenode;
	GEdge * tempedge;
	GNode * tempnode;
	GNode * tempdad;
	ELista * elist;
	if(outboundlist==NULL){												//if outbound list == NULL return 
		cout<<"success: conn("<<id<<","<<targetnode->get_id()<<") not found "<<endl;
		return ;
	}
	if(outboundlist->get_size()==0){									//if size ==0 return 
		cout<<"success: conn("<<id<<","<<targetnode->get_id()<<") not found "<<endl;
		return ;
	}
	explored=1;															//mark the starting as explored
	queue = new Lista;
	exploredlist = new Lista;
	tempedgenode=outboundlist->get_head();
	tempedge=tempedgenode->get_data();
	tempnode=tempedge->get_node();
	if(tempnode->get_dad()==NULL)
		tempnode->set_dad(this);
	for(i=0;i<outboundlist->get_size();i++){							//get his child-nodes and push then in queue-frontier
		queue -> push_back(tempnode);
		tempedgenode=tempedgenode->get_next();
		if(tempedgenode==NULL)
			break;
		tempedge=tempedgenode->get_data();
		tempnode=tempedge->get_node();
		if(tempnode->get_dad()==NULL)
			tempnode->set_dad(this);
	}
	while(queue->get_size()>0){                					//--------BFS---------------------//
		//cout<<"im in BFS"<<endl;
		temp = queue->pop();
		tempnode = temp->get_data();
		/*cout<<tempnode->get_id()<<endl;
		cout<<tempnode->get_explored()<<endl;
		GNode * supertemp;
		supertemp=tempnode->get_dad();
		if(supertemp!=NULL){
			cout<<"with father : "<<supertemp->get_id()<<endl;
		}*/
		temp->nullify();
		delete temp;
		if(tempnode==targetnode){									//if the node is found break
			//cout<<"found"<<endl;
			break;}
		if (tempnode->get_explored()==1)							//if the node is already visited
			continue;
		if(tempnode->get_explored()==0){							//else
			tempnode->set_explored(1);
			tempdad=tempnode;
			exploredlist->push_back(tempnode);						//push the node in the explored list for unmarking later 
			elist=tempnode->get_outlist();
			if(elist==NULL)
				continue;
			if(elist->get_size()==0)
				continue;
			tempedgenode=elist->get_head();							//expand child-nodes
			tempedge=tempedgenode->get_data();
			tempnode=tempedge->get_node();
			if(tempnode->get_dad()==NULL)
				tempnode->set_dad(tempdad);
			for(i=0;i<elist->get_size();i++){
				queue -> push_back(tempnode);
				tempedgenode=tempedgenode->get_next();
				if(tempedgenode==NULL)
					break;
				tempedge=tempedgenode->get_data();
				tempnode=tempedge->get_node();
				if(tempnode->get_dad()==NULL)
					tempnode->set_dad(tempdad);
			}
		}
	}
	if(tempnode==targetnode){										//if the node is found
		cout<<"success: conn("<<id<<","<<targetnode->get_id()<<") = ("<<id; 
		tempdad=tempnode;
		Lista * printright;
		printright=new Lista;										//use a list for print the path in a readable way 
		while(tempdad!=NULL){
			//cout<<"1st while"<<endl;
			//cout<<tempdad->get_id()<<endl;
			if(tempdad==this)
				break;
			printright->push_front(tempdad);
			tempdad->get_id();
			tempdad=tempdad->get_dad();
		}
		while(printright->get_size()>0){
			temp=printright->pop();
			tempnode=temp->get_data();
			cout<<","<<tempnode->get_id();
			tempnode->set_dad(NULL);
			temp->nullify();
			delete temp;
		}
		delete printright;
		cout<<")"<<endl;
	}
   	else															
		cout<<"success: conn("<<id<<","<<targetnode->get_id()<<") not found "<<endl;
	while(queue->get_size()>0){										//free and delete the queue
		temp=queue->pop();
		tempnode=temp->get_data();
		tempnode->set_dad(NULL);
		temp->nullify();
		delete temp;
	}
	delete queue;
	while(exploredlist->get_size()>0){								//free and delete the explored list
		temp=exploredlist->pop();
		tempnode=temp->get_data();
		tempnode->set_explored(0);
		tempnode->set_dad(NULL);
		temp->nullify();
		delete temp;
	}
	delete exploredlist;
	explored=0;
	tempdad=NULL;
}

void GNode::search_cycle(GNode * targetnode){						//used for finding cycles 
	int i;
	int flag1=0;
	Lista * stack;
	Lista * exploredlist;
	Node * temp;
	ENode * tempedgenode;
	GEdge * tempedge;
	GNode * tempnode;
	GNode * tempdad;
	ELista * elist;
	explored=1;
	if(outboundlist==NULL){										//check if list ==NULL or its size is 0
		cout<<"success: no cycle found for "<<id<<endl;
		return ;
	}
	if(outboundlist->get_size()==0){
		cout<<"success: no cycle found for "<<id<<endl;
		return ;
	}
	stack = new Lista;
	exploredlist = new Lista;
	tempedgenode=outboundlist->get_head();
	tempedge=tempedgenode->get_data();
	tempnode=tempedge->get_node();
	if(tempnode->get_dad()==NULL)								//expand the 1st node and push its chlidren to a stack
		tempnode->set_dad(this);
	for(i=0;i<outboundlist->get_size();i++){
		stack -> push_front(tempnode);
		tempedgenode=tempedgenode->get_next();
		if(tempedgenode==NULL)
			break;
		tempedge=tempedgenode->get_data();
		tempnode=tempedge->get_node();
		if(tempnode->get_dad()==NULL)
			tempnode->set_dad(this);
	}
	while(stack->get_size()>0){                				//--------DFS---------------------//
		temp = stack->pop();
		tempnode = temp->get_data();
		temp->nullify();
		int tsize=0;
		delete temp;
		if (tempnode->get_explored()==1)
			continue;
		if((tempedge=tempnode->get_spec_out_edge(targetnode))!=NULL){			//if the goal is achieved
			flag1++;
			if(flag1==1){
				cout<<"success: cycles "<<id<<" ="<<endl;						//print 
			}
			tempdad=tempnode;
			Lista * printright;
			printright=new Lista;
			while(tempdad!=NULL){
				if(tempdad==this)
					break;
				printright->push_front(tempdad);
				tempdad->get_id();
				tempdad=tempdad->get_dad();
			}
		tsize=printright->get_size();
		if(tsize>=3)															//in a readable form only those who have size >=3
			cout<<"("<<id;
		while(printright->get_size()>0){
			temp=printright->pop();
			tempnode=temp->get_data();
			if(tsize>=3)
				cout<<","<<tempnode->get_id();
			temp->nullify();
			delete temp;
		}
		delete printright;
		if(tsize>=3)
			cout<<")"<<endl;
		}
		
		if(tempnode->get_explored()==0){
			tempnode->set_explored(1);											//mark the nodes
			tempdad=tempnode;
			exploredlist->push_back(tempnode);
			elist=tempnode->get_outlist();
			if(elist==NULL)
				continue;
			if(elist->get_size()==0)
				continue;
			tempedgenode=elist->get_head();
			tempedge=tempedgenode->get_data();
			tempnode=tempedge->get_node();
			if(tempnode->get_explored()==0)
				tempnode->set_dad(tempdad);
			for(i=0;i<elist->get_size();i++){
				stack -> push_front(tempnode);
				tempedgenode=tempedgenode->get_next();
				if(tempedgenode==NULL)
					break;
				tempedge=tempedgenode->get_data();
				tempnode=tempedge->get_node();
				if(tempnode->get_explored()==0)
					tempnode->set_dad(tempdad);
			}
		}
	}
	if(flag1==0)															//finally free the memory and unmark the nodes
		cout<<"success: no cycle found for "<<id<<endl;
	while(stack->get_size()>0){
		temp=stack->pop();
		tempnode=temp->get_data();
		tempnode->set_dad(NULL);
		temp->nullify();
		delete temp;
	}
	delete stack;
	while(exploredlist->get_size()>0){
		temp=exploredlist->pop();
		tempnode=temp->get_data();
		tempnode->set_explored(0);
		tempnode->set_dad(NULL);
		temp->nullify();
		delete temp;
	}
	delete exploredlist;
	explored=0;
	tempdad=NULL;
}

void GNode::traceflow(int depth,int total){									// trace flow uses DLS recursively
	if(depth==0){															//if depth = 0
		//cout<<"Im in depth 0"<<endl;
		Node * temp;
		int flag=0;
		ENode * tempedgenode;
		GEdge * tempedge;
		GNode * tempnode;
		GNode * prev;
		Lista * elist;
		elist= new Lista;
		double sum=0;
		elist->push_front(this);
		prev=this;
		while(tempdad!=NULL){												//find the path from node->dad()
			//cout<<"in while"<<endl;
			tempedge=tempdad->get_spec_out_edge(prev);
			sum=sum+tempedge->get_value();
			elist->push_front(tempdad);
			prev=tempdad;
			tempdad=tempdad->get_dad();
		}
		if(elist->get_size()!=total+1)										//if the path has the size we want and no less
			flag=1;
		if(flag==0)
			cout<<"(";														//print in a readable form
		while(elist->get_size()>0){
			temp=elist->pop();
			if (temp==NULL)
				break;
			tempnode=temp->get_data();
			if(flag==0)
				cout<<tempnode->get_id()<<",";
			tempnode->set_dad(NULL);
			temp->nullify();
			delete temp;
		}
		delete elist;
		if(flag==0)
			cout<<sum<<")"<<endl;
		tempdad=NULL;
		explored=0;															//unmark this node
	}
	else if(depth>0){														//if depth >0
		int i=0;
		explored=1;
		ENode * tempedgenode;
		GEdge * tempedge;
		GNode * tempnode;
		ELista * elist;
		if(outboundlist==NULL){
			explored=0;
			tempdad=NULL;
			return;
		}
		if(outboundlist->get_size()==0){									//if outbound list ==NULL or size =0 return and unmark
			explored=0;
			tempdad=NULL;
			return;
		}
		tempedgenode=outboundlist->get_head();
		if(tempedgenode==NULL){
			explored=0;
			tempdad=NULL;
			return;
		}
		tempedge=tempedgenode->get_data();
		tempnode=tempedge->get_node();
		//outboundlist->print_list();

		while(tempedgenode!=NULL){
			if(tempnode->get_explored()==0){
					tempnode->set_dad(this);
					tempnode->traceflow(depth-1,total);					//call for each child the tarceflow recursively with depth-1
			}
			tempedgenode=tempedgenode->get_next();
			if(tempedgenode==NULL)
				break;
			tempedge=tempedgenode->get_data();
			tempnode=tempedge->get_node();
		}
		tempdad=NULL;
		explored=0;
	}
}

void GNode::triangle(int depth,int total,double minimum,GNode * startnode){    //uses DLS with fixed depth 2 and a goalnode
	if(depth==0){
		//cout<<"Im in depth 0"<<endl;
		Node * temp;
		int flag=0;
		ENode * tempedgenode;
		GEdge * tempedge;
		GNode * tempnode;
		GNode * prev;
		Lista * elist;
		if((tempedge=get_spec_out_edge(startnode))==NULL){					//if the node does not have an edge to the goal node return
			tempdad=NULL;
			explored=0;
			return;
		}

		elist= new Lista;
		double sum=0;
		elist->push_front(this);
		prev=this;
		while(tempdad!=NULL){												//else find the path
			//cout<<"in while"<<endl;
			tempedge=tempdad->get_spec_out_edge(prev);
			sum=tempedge->get_value();										//check if all the edges overcome the minimum value
			if(sum<minimum)
				flag=1;
			elist->push_front(tempdad);
			prev=tempdad;
			tempdad=tempdad->get_dad();
		}
		if(elist->get_size()!=total+1)
			flag=1;
		if(flag==0)															//print the path in a readable form
			cout<<"(";
		while(elist->get_size()>0){
			temp=elist->pop();
			if (temp==NULL)
				break;
			tempnode=temp->get_data();
			if(flag==0)
				if(elist->get_size()==0)
					cout<<tempnode->get_id();
				else
					cout<<tempnode->get_id()<<",";
			tempnode->set_dad(NULL);
			temp->nullify();
			delete temp;
		}
		delete elist;
		if(flag==0)
			cout<<")"<<endl;
		tempdad=NULL;
		explored=0;
	}
	else if(depth>0){
		int i=0;
		explored=1;
		ENode * tempedgenode;
		GEdge * tempedge;
		GNode * tempnode;
		ELista * elist;
		if(outboundlist==NULL){
			explored=0;
			tempdad=NULL;
			return;
		}
		if(outboundlist->get_size()==0){
			explored=0;
			tempdad=NULL;
			return;
		}
		tempedgenode=outboundlist->get_head();
		if(tempedgenode==NULL){
			explored=0;
			tempdad=NULL;
			return;
		}
		tempedge=tempedgenode->get_data();
		tempnode=tempedge->get_node();
		//outboundlist->print_list();

		while(tempedgenode!=NULL){
			if(tempnode->get_explored()==0){
					tempnode->set_dad(this);
					tempnode->triangle(depth-1,total,minimum,startnode);      //call triangle recursively for each child
			}
			tempedgenode=tempedgenode->get_next();
			if(tempedgenode==NULL)
				break;
			tempedge=tempedgenode->get_data();
			tempnode=tempedge->get_node();
		}
		tempdad=NULL;
		explored=0;
	}
}
ENode::ENode(){
	data=NULL;
	next=NULL;
	prev=NULL;
	//cout<<"A ENode has just been created"<<endl;
}

ENode::~ENode(){
	if(data!=NULL)
		delete data;
	next=NULL;
	prev=NULL;
	//cout<<"A ENode has just been destroyed"<<endl;
}

void ENode::set_data(GEdge* g){
	data=g;
}

GEdge * ENode::get_data(){
	return data;
}

void ENode::set_next(ENode* n){
	next=n;
}

ENode* ENode::get_next(){
	return next;
}

void ENode::set_prev(ENode* n){
	prev=n;
}

ENode* ENode::get_prev(){
	return prev;
}

void ENode::print_node(){
	data->print_data();
}
ELista::ELista(){
	head=NULL;
	tail=NULL;
	size=0;
	//cout<<"A elista has just been created"<<endl;
}

ELista::~ELista(){
	ENode * temp;
	while(size>0){
		temp=pop();
		if(temp!=NULL){
			delete temp;
		}
		//size--;
	}
	//cout<<"A elista has just been destroyed"<<endl;
}

void ELista::set_head(ENode* h){
	head=h;
}

ENode * ELista::get_head(){
	return head;
}

void ELista::set_tail(ENode* t){
	tail=t;
}

ENode * ELista::get_tail(){
	return tail;
}

void ELista::set_size(int s){
	size=s;
}

int ELista::get_size(){
	return size;
}

void ELista::push_back(GEdge* n){							//places an item at the end of the list
	ENode * temp;
	temp=new ENode;
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

ENode * ELista::pop(){									//removes an item from the top of the list
	ENode * temp;
	if(size>0){
		temp=head;
		head=head->get_next();
		size--;
		return temp;
	}
	return NULL;
}

ENode * ELista::delete_edge(GNode * gnode){			//delete a specific item
	//cout<<"i m in delete"<<endl;
	if(size==0){
		return NULL;
	}
	else{
		int i=0;
		ENode * tempnode;
		GEdge * gedge;
		tempnode=head;
		gedge=tempnode->get_data();
		for(i=0;i<size;i++){
			if(gedge->get_node()==gnode){
				if(tempnode==head){
					return pop();
				}
				else if(tempnode==tail){
					tail=tempnode->get_prev();
					size--;
					return tempnode;
				}
				else{
					ENode* thenext;
					ENode* theprev;
					theprev=tempnode->get_prev();
					theprev->set_next(tempnode->get_next());
					thenext=tempnode->get_next();
					thenext->set_prev(tempnode->get_prev());
					size--;
					return tempnode;
				}
			}
			tempnode=tempnode->get_next();
			gedge=tempnode->get_data();
		}
		return NULL;
	}
}

double ELista::get_totalsum(){										//get the sum of all the values of the edges
	if(size==0)
		return 0;
	else{
		int i=0;
		double sum=0;
		ENode * tempnode;
		GEdge * tempedge;
		tempnode=head;
		if(tempnode==NULL)
			return 0;
		tempedge=tempnode->get_data();
		for(i=0;i<size;i++){
			sum=sum + tempedge->get_value();
			tempnode=tempnode->get_next();
			if(tempnode==NULL)
				break;
			tempedge=tempnode->get_data();

		}
		return sum;
	}
}
void ELista::print_list(){											
	int i=0;
	if(size ==0)
		return;
	ENode * temp;
	temp=head;
	while(i<size){
		temp->print_node();
		temp=temp->get_next();
		i++;
	}
}
