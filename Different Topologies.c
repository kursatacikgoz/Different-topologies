#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
This project is a programming assignment in C which aims at developing an algorithm that
builds a binary search tree (BST) from a given sequence and attemps to find other sequences
that yield the same BST topology.
*/

//This is the definition of binary search tree .
typedef struct node{ 
    int key; 
    struct node *left, *right; 
}node;

//This holds the nodes of probabilities
typedef struct seqlist{
	struct node *which;
	struct seqlist *next;
}seqlist;

//This is the definition of sequences
typedef struct series{
	int data;
	struct series *next;
}series;

struct node *newNode(int item);
struct node* search(struct node* root, int key);
struct node* insert(struct node* node, int key); 
void prefix(struct node *root);
struct seqlist* delfromlist(seqlist* seq, node* delseq);
struct seqlist* insertlist(seqlist* seq, node* addseq);
void findsequences(node* p,seqlist *seq,series *listed);
struct series* insertlisted(series *listed, int data);
void printlisted(series *listed);
struct series *copyseries(struct series *head);
struct seqlist* copyseqlist(seqlist* seq);
void freeseqList(seqlist* head);
void freeseriesList(series* head);

//needed a global int variable to wrie all posibilities number of sequences
int nth=1;

int main(){
	char *seq=(char*)malloc(sizeof(char)*60);
	printf("Enter the sequence: ");
	
	//Takes string input from user
    scanf("%[^\n]s", seq);
    
    //Splits input to tokens and adds them to token
    char *token = (char *)strtok(seq, " ");
    
	struct node *root = NULL;
	
	//This loop adds all numbers from 
	//input sequence adds to tree which is pointed by root
	while (token != NULL){
		root = insert(root,atoi(token));
		token = strtok(NULL, " ");
	} 
	
	//Calls the prefix function and shows the tree by prefix implementation
	printf("Tree in prefix: ");
	prefix(root);
	printf("\n");
	
	struct seqlist *sequence=NULL;
	struct series *listed = NULL;
	
	//Called void function findsequences to find all sequences and print all to console
	findsequences(root,sequence,listed);
	
	return 0;
}

/*Finds and prints the order in which the nodes in this tree 
can be exported to create the same BST topology sent to this function.*/
void findsequences(node* p,seqlist *seq,series *listed){
	//seq is copied to copy pointer which type is seqlist
	seqlist* copy=copyseqlist(seq);
	
	//node is added to copy list which is sent by function
	copy=insertlist(copy,p);
	
	//iteration pointed the node which is sent by function
	node *iteration=p;
	
	//listed is copied to listedptr pointer which type is series
	series *listedptr=copyseries(listed); 
	
	//key of node is added to listedptr list which is key of iteration
	listedptr=insertlisted(listedptr,iteration->key);
	
	//checks list of copy is not null
	if(copy!=NULL){
		//deletes iteration node from sopy list and adds childs of this node
		copy=delfromlist(copy,iteration);
		copy=insertlist(copy,iteration->left);
		copy=insertlist(copy,iteration->right);
		
		//checks copy is not null still
		if(copy!=NULL){
			//temp pointer points next of copy list
			seqlist* temp=copy->next;
			
			//calls function recursively to complate sequences
			findsequences(copy->which,copy,listedptr);
			
			//to find all probabilities calls the recursion with another nodes of tree which could be next
			while(temp!=NULL){
				findsequences(temp->which,copy,listedptr);
				temp=temp->next;	
			}
			
			//to do not use the memory unnecessarily make free lists
			freeseqList(copy);
			freeseriesList(listedptr);
			
			return;
		}else{
			//if list is null that means there is no posibilites up to here. 
			//should print list now and turn back.
			printlisted(listedptr);
			
			//to do not use the memory unnecessarily make free lists
			freeseqList(copy);
			freeseriesList(listedptr);
			return;
		}	
	}
}

//Takes a pointer of series and data as arguments and adds the data 
//to end of the list which is pointed by listed.finally returns list back.
struct series* insertlisted(series *listed, int data){
	//checks is list null or not if null add data to lists first and last node. returns list back.
	if(listed==NULL){
		listed=(series*)malloc(sizeof(series)); 
		listed->data=data;
		listed->next=NULL;
		return listed;
	}else{
		//if list is not null and first nodes data equals data does not change something
		if(listed->data==data)
			return listed;
		
		//if the data of first node is not equal data creates an iter which will point nodes
		series *iter=listed;
		
		//this loop changes the iter to find end of the list if there is no data which is equal data
		while(iter->next!=NULL && iter->next->data!=data){
			iter=iter->next;
		}
		
		//if there is a node has same data does not change something and return back
		if(iter->next!=NULL)
			if(iter->next->data==data)
				return listed;
		
		//if there is no other node has same data adds it to end of list and return back with listed
		iter->next=(series*)malloc(sizeof(series));
		iter->next->data=data;
		iter->next->next=NULL;
		return listed;
	}
}

//Takes a pointer of seqlist and node as arguments and adds the node 
//to end of the list which is pointed by seq.finally returns list back.
struct seqlist* insertlist(seqlist* seq, node* addseq){ 
	//checks node is not null 
	if(addseq!=NULL){
		//checks is list null or not if null add node to lists first and last node. returns list back.
		if(seq==NULL){
			seq=(seqlist*)malloc(sizeof(seqlist)); 
			seq->which=addseq;
			seq->next=NULL;
			return seq;
		}else{
			//if list is not null and first nodes equals addseq does not change something
			if(seq->which==addseq)
				return seq;
			
			//if the node of first is not equal addseq creates an iter which will point nodes of seqlist
			seqlist *iter=seq;
			
			//this loop changes the iter to find end of the list if there is no node which is equal addseq
			while(iter->next!=NULL && iter->next->which!=addseq){
				iter=iter->next;
			}
			
			//if there is a node  same addseq does not change something and return back
			if(iter->next!=NULL)
				if(iter->next->which==addseq)
					return seq;
			
			//if there is no other node same addseq adds it to end of list and return back with listed
			iter->next=(seqlist*)malloc(sizeof(seqlist));
			iter->next->which=addseq;
			iter->next->next=NULL;
			return seq;
		}
	}else{
		return seq;
	}
}

//this function checks the list. if this list has delseq deletes it
struct seqlist* delfromlist(seqlist* seq, node* delseq){ 
	//checks seq or delseq are not null
	if(delseq!=NULL && seq!=NULL){
		//created an iter to check all list up to see delseq
		seqlist* iter=seq;
		if(delseq==iter->which){
			seq=seq->next;
		}else{
			while(iter->next!=NULL&&iter->next->which!=delseq){
				iter=iter->next;
			}
			//if next of iter is not equal that means it equals delseq and deletes it
			if(iter->next!=NULL){
				if(iter->next->which==delseq){
					iter->next=iter->next->next;
				}
			}
		}
		return seq;
	}else{
		return seq;
	}
} 

//creates a new node with argument int
struct node *newNode(int item){ 
    node *temp =  (node *)malloc(sizeof(node)); 
    temp->key = item; 
    temp->left = NULL;
	temp->right = NULL; 
    return temp; 
}

//insert function of BST
struct node* insert(struct node* node, int key){ 
    if (node == NULL) 
		return newNode(key);
    if (key < node->key) 
        node->left  = insert(node->left, key); 
    else if (key > node->key) 
        node->right = insert(node->right, key);   
    return node; 
}

//search function of BST
struct node* search(struct node* root, int key){ 
    if (root == NULL || root->key == key) 
       return root; 
    if (root->key < key) 
       return search(root->right, key); 
    return search(root->left, key); 
}

//this function coppies the list and returns coppied
struct seqlist* copyseqlist(seqlist* seq){ 
	seqlist* iter=seq;
	seqlist* copy=NULL;
	while(iter!=NULL){
		copy=insertlist(copy,iter->which);
		iter=iter->next;
	}
	return copy;
}

//this function coppies the list and returns coppied
struct series *copyseries(struct series *head){
	series* copy=NULL;
	series* iter=head;
	while(iter!=NULL){
		copy=insertlisted(copy,iter->data);
		iter=iter->next;
	}
	return copy;
}

//this function makes free all nodes of linked list
void freeseqList(seqlist* head){
   seqlist* tmp;
   while (head != NULL){
       tmp = head;
       head = head->next;
       free(tmp);
    }
}

//this function makes free all nodes of linked list
void freeseriesList(series* head){
   series* tmp;
   while (head != NULL){
       tmp = head;
       head = head->next;
       free(tmp);
    }
}

//this function prints tree in prefix form
void prefix(node *root){ 
    if (root != NULL){ 
    	printf("%d ", root->key); 
        prefix(root->left); 
        prefix(root->right); 
    } 
}

//this function prints sequences
void printlisted(series *listed){
	series *iter=listed;
	printf("%-15d",nth++);
	while(iter!=NULL){
		printf("%d ", iter->data);
		iter=iter->next;
	}
	printf("\n");
}

