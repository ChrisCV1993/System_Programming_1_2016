#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include "Lista.hpp"
#include "Graph.hpp"
#include <fstream>

#include "Hashtable.hpp"

#define headsize 1000

using namespace std;

int main(int argc, char **argv){
	int no_of_entries=101;													//default value if no #of entries given
	FILE * fptr=NULL;
	if((argc > 5 )||(argc<3)){                                              //check number of arguments
		cout<<"Wrong number of arguments"<<endl;
		return 1;
	}
	if((argc%2)==0){
		cout<<"Wrong number of arguments"<<endl;
		return 1;
	}
	if(argc==3){															//must have the flag -b
		if(strcmp(argv[1],"-b")!=0){
			cout<<"Wrong flag1"<<endl;
			return 1;	
		}
		else{
			no_of_entries=atoi(argv[2]);
		}
	}
	if(argc==5){
		if(strcmp(argv[1],"-b")==0){
			no_of_entries=atoi(argv[2]);
		}
		else if(strcmp(argv[1],"-o")==0){
			fptr=fopen(argv[2],"r+");
			if(fptr==NULL){
				cout<<"Wrong file"<<endl;
				return 1;
			}
		}
		else{
			cout<<"Wrong flag"<<endl;
			return 1;	
		}
		if(strcmp(argv[3],"-b")==0){
			no_of_entries=atoi(argv[4]);
			cout<<no_of_entries<<endl;
		}
		else if(strcmp(argv[3],"-o")==0){
			fptr=fopen(argv[4],"r+");
			if(fptr==NULL){
				cout<<"Wrong file"<<endl;
				return 1;
			}	
		}
		else{
			cout<<"Wrong flag"<<endl;
			return 1;	
		}
	}
	Hashtable * hash1;
	char  head[headsize];
	int i;
	GNode * g1;
	hash1=new Hashtable(no_of_entries);												//create table
	if(fptr==NULL)																	//if no file is given get from stdin
		fptr=stdin;
	while(1){
		i=0;
		if (fgets(head, headsize,fptr) <= 0)										//read line by line until EOF is given
			break; 
		if(strncmp(head,"createnodes",11)==0){										//check the command
			int numb;
			int j;
			//cout<<"create"<<endl;
			i=12;
			while((j=sscanf(head+i,"%d",&numb))>0){									//get the data
				i=i+8;
				if (hash1->exists(numb)!=NULL){										//check them
					cout<<"failure: the node "<<numb<<" already exists"<<endl;
				}
				else{
					hash1->insert_hash(numb);
					cout<<"success: created "<<numb<<endl;
				}
			}
		}
		else if(strncmp(head,"delnodes",8)==0){
			int numb;
			int j;
			Node * temp;
			i=9;
			while((j=sscanf(head+i,"%d",&numb))>0){
				i=i+8;
				if ((temp=hash1->exists(numb))==NULL){
					cout<<"failure: the node "<<numb<<" does not exist"<<endl;
				}
				else{
					GNode * node1;
					int size1,size2;
					ELista * list1;
					ELista * list2;
					node1=temp->get_data();
					list1=node1->get_outlist();
					list2=node1->get_inlist();
					if(list1==NULL)
						size1=0;
					else
						size1=list1->get_size();										//a node can only be deleted if there are no edges to or from him
					if(list2==NULL)
						size2=0;
					else
						size2=list2->get_size();							
					if(size1!=0){
							cout<<"failure: the node "<<numb<<" has outbound edges"<<endl;
					}
					else if(size2!=0){
							cout<<"failure: the node "<<numb<<" has inbound edges"<<endl;
					}
					else{
						temp=hash1->delete_node(numb);
						delete temp;
						cout<<"success: deleted "<<numb<<endl;
					}
				}
			}
		}
		else if(strncmp(head,"addtran",7)==0){
			//cout<<"addtran"<<endl;
			int numb1,numb2;
			double value;
			int j;
			double tempval;
			Node * temp1;
			Node * temp2;
			GNode * node1;
			GNode * node2;
			GEdge * edge1;
			GEdge * edge2;

			i=8;
			j=sscanf(head+i,"%d %d %lf",&numb1,&numb2,&value);
			//cout<<numb1<<"    "<<numb2<<"   "<<value<<"   "<<endl;
			temp1=hash1->exists(numb1);
			if(temp1==NULL){
				cout<<"failure: the node "<<numb1<<" does not exist"<<endl;
				continue ;
			}
			temp2=hash1->exists(numb2);
			if(temp2==NULL){
				cout<<"failure: the node "<<numb2<<" does not exist"<<endl;
				continue ;
			}
			node1=temp1->get_data();
			node2=temp2->get_data();
			edge1=node1->get_spec_out_edge(node2);											//if the edge not exists create it
			edge2=node2->get_spec_in_edge(node1);
			if(edge1==NULL){
				node1->insert_out_edge(node2,value);
				cout<<"success: added transaction "<<numb1<<" "<<numb2<<" with amount "<<value<<endl;
			}
			else{
				tempval=edge1->get_value()+value;											// else update it 
				edge1->set_value(tempval);
				cout<<"success: added transaction "<<numb1<<" "<<numb2<<" with amount "<<value<<" total amount : "<<tempval<<endl;
			}
			if(edge2==NULL){																//fix the inbound edge of the other node
				node2->insert_in_edge(node1,value);
			}
			else{
				tempval=edge2->get_value()+value;
				edge2->set_value(tempval);
			}
		}
		else if(strncmp(head,"deltran",7)==0){
			int numb1,numb2,j;
			Node * temp1;
			Node * temp2;
			GNode * node1;
			GNode * node2;
			ENode * enode;
			ELista * outlist;
			ELista * inlist;
			i=8;
			j=sscanf(head+i,"%d %d",&numb1,&numb2);
			temp1=hash1->exists(numb1);
			if(temp1==NULL){																	//check if both nodes exist
				cout<<"failure: the node "<<numb1<<" does not exist"<<endl;
				continue ;	
			}
			temp2=hash1->exists(numb2);
			if(temp2==NULL){
				cout<<"failure: the node "<<numb2<<" does not exist"<<endl;
				continue ;	
			}
			node1=temp1->get_data();
			node2=temp2->get_data();
			outlist=node1->get_outlist();
			inlist=node2->get_inlist();
			if(outlist==NULL){																	//chack if the edge exists
				cout<<"failure: the edge ("<<numb1<<","<<numb2<<") does not exist"<<endl;
				continue ;
			}
			else{
				enode=outlist->delete_edge(node2);												//delete the edge
				if(enode==NULL)
					cout<<"failure: the edge ("<<numb1<<","<<numb2<<") does not exist"<<endl;
				else{
					delete enode;
					enode=NULL;
					cout<<"success: deleted transaction "<<numb1<<" "<<numb2<<endl;
				}
			}
			if(inlist==NULL){
				continue ;
			}
			else{
				enode=inlist->delete_edge(node1);												//delete the inbound edge of the other node
				if(enode!=NULL)
					delete enode;
			}	
		}
		else if(strncmp(head,"lookup sum",10)==0){
			int numb1,j;
			double totalval=0;
			Node * temp1;
			GNode * node1;
			ELista * outlist;
			ELista * inlist;
			i=11;
			j=sscanf(head+i,"%d",&numb1);
			temp1=hash1->exists(numb1);
			if(temp1==NULL){
				cout<<"failure: the node "<<numb1<<" does not exist"<<endl;						
				continue ;	
			}
			node1=temp1->get_data();
			outlist=node1->get_outlist();
			inlist=node1->get_inlist();
			if(inlist!=NULL){
				totalval=inlist->get_totalsum();											//sum is invalue - outvalue
			}
			if(outlist!=NULL){
				totalval=totalval-(outlist->get_totalsum());
			}
			cout<<"success: sum("<<numb1<<") = "<<totalval<<endl;
		}	
		else if(strncmp(head,"lookup out",10)==0){
			int numb1,j;
			double totalval=0;
			Node * temp1;
			GNode * node1;
			ELista * outlist;
			i=11;
			j=sscanf(head+i,"%d",&numb1);
			temp1=hash1->exists(numb1);
			if(temp1==NULL){
				cout<<"failure: the node "<<numb1<<" does not exist"<<endl;
				continue ;	
			}
			node1=temp1->get_data();
			outlist=node1->get_outlist();
			if(outlist!=NULL){														
				totalval=outlist->get_totalsum();
			}
			cout<<"success: out("<<numb1<<") = "<<totalval<<endl;
		}		
		else if(strncmp(head,"lookup in",9)==0){
			int numb1,j;
			double totalval=0;
			Node * temp1;
			GNode * node1;
			ELista * inlist;
			i=10;
			j=sscanf(head+i,"%d",&numb1);
			temp1=hash1->exists(numb1);
			if(temp1==NULL){
				cout<<"failure: the node "<<numb1<<" does not exist"<<endl;
				continue ;	
			}
			node1=temp1->get_data();
			inlist=node1->get_inlist();
			if(inlist!=NULL){
				totalval=inlist->get_totalsum();
			}
			cout<<"success: in("<<numb1<<") = "<<totalval<<endl;
		}		
		else if(strncmp(head,"triangle",8)==0){
			int numb1,j;
			double amount;
			Node * temp1;
			Node * temp2;
			GNode * node1;
			GNode * node2;
			i=9;
			j=sscanf(head+i,"%d %lf",&numb1,&amount);
			temp1=hash1->exists(numb1);
			if(temp1==NULL){
				cout<<"failure: the node "<<numb1<<" does not exist"<<endl;
				continue ;	
			}
			node1=temp1->get_data();
			cout<<"success: triangle("<<numb1<<","<<amount<<")="<<endl;
			node1->triangle(2,2,amount,node1);											//call dls with depth 2 minimum amount and targetnode itself
		}		
		else if(strncmp(head,"conn",4)==0){
			int numb1,numb2,j;
			Node * temp1;
			Node * temp2;
			GNode * node1;
			GNode * node2;
			i=5;
			j=sscanf(head+i,"%d %d",&numb1,&numb2);
			temp1=hash1->exists(numb1);
			if(temp1==NULL){
				cout<<"failure: the node "<<numb1<<" does not exist"<<endl;
				continue ;	
			}
			temp2=hash1->exists(numb2);
			if(temp2==NULL){
				cout<<"failure: the node "<<numb2<<" does not exist"<<endl;
				continue ;	
			}
			node1=temp1->get_data();
			node2=temp2->get_data();
			node1->search_conn(node2);											//call bfs with node2 as target node
		}
		else if(strncmp(head,"allcycles",9)==0){
			int numb1,j;
			Node * temp1;
			GNode * node1;
			i=10;
			j=sscanf(head+i,"%d",&numb1);
			temp1=hash1->exists(numb1);
			if(temp1==NULL){
				cout<<"failure: the node "<<numb1<<" does not exist"<<endl;
				continue ;	
			}
			node1=temp1->get_data();
			node1->search_cycle(node1);											//call dfs with itself as target node
		}
		else if(strncmp(head,"traceflow",9)==0){
			int numb1,depth,j;
			Node * temp1;
			GNode * node1;
			i=10;
			j=sscanf(head+i,"%d %d",&numb1,&depth);
			temp1=hash1->exists(numb1);
			if(temp1==NULL){
				cout<<"failure: the node "<<numb1<<" does not exist"<<endl;
				continue ;	
			}
			node1=temp1->get_data();
			node1->set_dad(NULL);
			node1->set_explored(1);
			cout<<"success: traceflow("<<numb1<<","<<depth<<")="<<endl;			//call dls with depth depth
			node1->traceflow(depth,depth);
			node1->set_dad(NULL);
			node1->set_explored(0);
		}
		else if(strncmp(head,"print",5)==0){
			//cout<<"print"<<endl;
			hash1->print_hash();												//print hash in a readable form
		}
		else if(strncmp(head,"bye",3)==0){
			delete hash1;														//delete hash
			cout<<"success: cleaned memory"<<endl;
			hash1=NULL;
			hash1=new Hashtable(no_of_entries);									//bye may be followed by another hash creation
		}
		else if(strncmp(head,"dump",4)==0){ 					
			int j;
			char buf[100];
			memset(buf,'\0',100);
			i=5;
			j=sscanf(head+i,"%s",buf);
			//cout<<buf<<endl;
			if(buf[i]=='\0'){
				cout<<"failure: No file name given"<<endl;
				continue;
			}
			ofstream * myfile = new ofstream;
  			myfile->open(buf);													//open a file given or create an open it
 			hash1->export_hash(myfile);											//dump the hash
  			myfile->close();													//close
  			delete myfile;
  			cout<<"success: dumped to "<<buf<<endl;
		}
		else{
			cout<<"Wrong input"<<endl;
		}
		//cout<<head;
		//memset(head,'\0',headsize);
	}
	if(hash1!=NULL)																//delete the hash if not deleted 
		delete hash1;
	if(fptr!=stdin)																//close fptr
		fclose(fptr);
	return 0;
}
