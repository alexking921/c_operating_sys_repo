#include<stdio.h>
#include<stdlib.h>

/*
 * Recitation section number: 014
 * Your name: Alex King ---- king1493@umn.edu
 * 
 */

struct node{
  int x;
  struct node * next;
};

struct node* create_list(struct node* head){

    //create first node
    head = (struct node*)malloc(sizeof(struct node));
    head->x=2;
    head->next=NULL;

    //create second node
    struct node* temp =(struct node*)malloc(sizeof(struct node));
    temp->x=6;
    temp->next=NULL;
    head->next=temp;

    // OTHER OPTION:
    //struct node node1;  // create instance of node struct 
    //node1.x = 15;     // access the structs fields 
    //int answer = node1.x 
  
    //create third node
    struct node* temp2 = (struct node*)malloc(sizeof(struct node));
    temp2->x=10;
    temp2->next=NULL;
    head->next->next=temp2;
    
    return head;
}



void print_list(struct node* head){
  struct node* cur=head;
  while(cur!=NULL){
    printf("%d ", cur->x);
    cur=cur->next;
  }
  printf("\n");
}

struct node* insert_list (struct node* head, int num){
  //TO DO
  // create a node, store num there, and insert the node in the list so that the list remains in sorted order
  
  // ptr to the newNode we're trying to insert
  struct node* myNode = (struct node*)malloc(sizeof(struct node));
  
  // Storing num in the new node
  myNode->x = num;

  struct node* temp = head; // temporary variable to store the ptr to head of list so not lost
  
  // store the data val we'll be doing comparisons w for ease of access
  int key = myNode->x;   
  
  // check if you're inserting a node smaller than any of current nodes
  if (key < head->x){
     myNode->next = head; 
     return myNode;
  }


  

  // GOAL: find spot where currNode < key && nextNode > key
  // loop: as long as the matching condition are 
  // java equiv: while prev != NULL && curr.next.data < key
  while(temp->next != NULL && temp->next->x < key)
    temp = temp->next; // move pointer over 1 left and test again since we know the node we're inserting is GREATER than the nextNode
    // the nextNode must be LARGER than our targetKey to be 'sorted'
  
  // once it breaks out of while loop, we know we're at the right spot to insert node
  myNode->next = temp->next; // have newNode we're inserting point to the nextNode
  temp->next = myNode; // have the currNode (aka temp) point to newNode were inserting (remember myNode is a POINTER, which is why you can assign curr.next as such)
  
  free(myNode);
  return head;
}


// [50]-> [60] -> [70]
// Inserting:  [40].  CASE: key < head.x
// IF this IS the case, --- then we want to:
// Assign insertingNode to point to head like so:     myNode->next = head
// AND we DONT want to return head like w the other scenarios bc that's no longer the 'head' of the list



int main (int argc, char** argv){
  if (argc < 2) {

  		printf("Invalid number of args. Please enter 1 argument\n");
  		exit(1);
  	}
    int num = atoi(argv[1]);

    //build an initial list
    struct node * head;
    head = create_list(head);

    head = insert_list(head, num);

    print_list(head);

    return 0;


}
