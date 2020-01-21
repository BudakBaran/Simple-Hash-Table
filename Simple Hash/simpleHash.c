#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>
#define SIZE 7

typedef struct HASH_TABLE_N *HASH_TABLE_P;

typedef struct HASH_TABLE_N {
	int data;
	void *Tree;
} HASH_TABLE_t[1];

typedef struct LINKED_STACK_NODE_s *LINKED_STACK_NODE;

typedef struct LINKED_STACK_NODE_s {

	LINKED_STACK_NODE next;
	void *data;
} LINKED_STACK_NODE_t[1];

typedef struct LINKED_STACK_s {

	LINKED_STACK_NODE head;
} LINKED_STACK_t[1], *LINKED_STACK;

typedef struct LINKED_TREE_AVL_NODE_s *LINKED_TREE_AVL_NODE;
typedef struct LINKED_TREE_AVL_NODE_s {
	LINKED_TREE_AVL_NODE right; /*Do not alter -- DSW--*/
	LINKED_TREE_AVL_NODE left;
	void *data;
	int height;
} LINKED_TREE_AVL_NODE_t[1];

typedef struct LINKED_TREE_AVL_s *LINKED_TREE_AVL;
typedef struct LINKED_TREE_AVL_s {
	LINKED_TREE_AVL_NODE root;
} LINKED_TREE_AVL_t[1];


HASH_TABLE_P hash_table_init(){
	int i;
	HASH_TABLE_P hash = (HASH_TABLE_P) malloc(sizeof(HASH_TABLE_t)*SIZE);
	for(i = 0; i < SIZE; i++){
		hash[i].data = -1;
		hash[i].Tree = NULL;
	}
	return hash;
}


LINKED_TREE_AVL linked_tree_init() {
	LINKED_TREE_AVL new = (LINKED_TREE_AVL) malloc(sizeof(LINKED_TREE_AVL_t));
	new->root = NULL;
	return new;
}

LINKED_STACK linked_stack_init() {

	LINKED_STACK stack = (LINKED_STACK) malloc(sizeof(LINKED_STACK_t));
	stack->head = NULL;
	return stack;
}

void linked_stack_push(LINKED_STACK stack, void *data) {

	LINKED_STACK_NODE node = (LINKED_STACK_NODE) malloc(
			sizeof(LINKED_STACK_NODE_t));
	if (node != NULL) {
		node->data = data;
		node->next = stack->head;
		stack->head = node;

	} else {
		printf("Error @ linked_stack_push: Cannot allocatememory.\n");
	}

}

void *linked_stack_pop(LINKED_STACK stack) {
	LINKED_STACK_NODE node;
	void *res = NULL;
	if (stack->head != NULL) {
		node = stack->head;
		res = node->data;
		stack->head = node->next;
		free(node);
	} else {
		printf("Error @ linked_stack_pop: Empty stack is popped.\n");
	}
	return res;

}

int linked_stack_is_empty(LINKED_STACK stack) {
	if (stack->head != NULL) {
		return 1;
	} else {
		return 0;
	}

}

LINKED_TREE_AVL_NODE node_init(void *data) {
	LINKED_TREE_AVL_NODE node = (LINKED_TREE_AVL_NODE) malloc(
			sizeof(LINKED_TREE_AVL_NODE_t));
	node->data = data;
	node->left = NULL;
	node->right = NULL;
	return node;
}

void Free_node(LINKED_TREE_AVL_NODE node) {
	node->left = NULL;
	node->right = NULL;
	node->data = NULL;
	free(node);
}

int static compare_data(void *d1, void *d2) {
	int c = 3;
	int a = ((int *)d1);
	int b = ((int *)d2);
	if (a < b ) {
		c = 1;
	} else if (a > b) {
		c = 2;
	} else {
		c = 0;
	}
	return c;
}

int Find_Height(LINKED_TREE_AVL_NODE node){
	int left_height,right_height;

	if(node == NULL){

		return 0;
	}

	left_height=Find_Height(node->left);
	right_height=Find_Height(node->right);

	if (left_height > right_height) {
		return left_height + 1;
	}else{
		return right_height + 1;
	}
}


LINKED_TREE_AVL_NODE RightRotate(LINKED_TREE_AVL_NODE node){

	LINKED_TREE_AVL_NODE parent = node->left;

		node->left = parent->right;
		parent->right = node;
		return parent;
}

LINKED_TREE_AVL_NODE LeftRotate(LINKED_TREE_AVL_NODE node){

	LINKED_TREE_AVL_NODE parent = node->right;


	LINKED_TREE_AVL_NODE temp = parent->left;
	parent->left = node;
	node->right = temp;

	return parent;
}

					/////// NODE TYPE FUNCTION BECAUSE OTHER FUNCTION NEEDS.
LINKED_TREE_AVL_NODE linked_tree_insert(LINKED_TREE_AVL_NODE node, void *data) {

	int check = 1,Left_H,Right_H,TrueFalse = 4,i=0;
	LINKED_STACK stack=linked_stack_init();
	LINKED_TREE_AVL_NODE parent;

	/*////
	FOR TREE ROOT PUSH THE NODE
	*//////
	linked_stack_push(stack,node);


	while (check == 1) {
		if (compare_data( data, node->data ) == 1) {
			if (node->left == NULL) {
				node->left = node_init(data);
				check = 0;
			} else {
				node = node->left;
				linked_stack_push(stack,node);
			}
		} else if (compare_data(data, node->data) == 2) {
			if (node->right == NULL) {
				node->right = node_init(data);
				check = 0;
			} else {
				node = node->right;
				linked_stack_push(stack,node);
			}
		} else {
			printf("DUBLICATE\n");
			check = 0;
		}
	}
	////////////////////// START
	while(linked_stack_is_empty(stack)==1 || i == 0){

		Left_H = Find_Height(node->left);
		Right_H = Find_Height(node->right);
		node->height = Left_H - Right_H;

		if(node->height > 1 || node->height < -1){
			if(linked_stack_is_empty(stack)==1){

				LINKED_TREE_AVL_NODE temp = linked_stack_pop(stack);

				if(temp->left == node){
					TrueFalse = 1;
				}else if(temp->right == node){//	CONTROL OF WHİCH LİNK IS LAST ONE
					TrueFalse = 2;
				}

				linked_stack_push(stack,temp);

			}else{
				i = 1;
				TrueFalse = 3;
			}
			if(node->height > 0){				//LEFT ERROR

				parent = node->left;

				if(node->left->height == 1 || node->left->height == 0 ){				//LEFT-LEFT ERROR

																		// Boolean for how we understand link with pop which node child or parent(This case Parent to Left)
					node = RightRotate(node);

				}else if(node->left->height == -1){										//LEFT-RİGHT ERROR

																		// Boolean for how we understand link with pop which node child or parent(This case Child to left)
					node->left = LeftRotate(parent);
					node = RightRotate(node);

				}

			}else if(node->height < 0){				//RIGHT EROR

				parent = node->right;

				if(node->right->height == 1){											//RIGHT-LEFT ERROR

																		// Boolean for how we understand link with pop which node child or parent(This case Child to right)
					node->right = RightRotate(parent);
					node = LeftRotate(node);

				}else if(node->right->height == 0 || node->right->height == -1){	//RIGHT-RIGHT ERROR

																		// Boolean for how we understand link with pop which node child or parent(This case Parent to right)
					node = LeftRotate(node);
				}

			}
		}


		LINKED_TREE_AVL_NODE Tempnode;
		if(TrueFalse == 1){
			Tempnode = linked_stack_pop(stack);
			Tempnode->left = node;
			TrueFalse = 4;

		}else if(TrueFalse == 2){
			Tempnode = linked_stack_pop(stack);
			Tempnode->right = node;
			TrueFalse = 4;

		}else if(TrueFalse == 4){

			TrueFalse = 4;
			if(linked_stack_is_empty(stack) != 1){
				TrueFalse = 3;
			}else{
				Tempnode = linked_stack_pop(stack);
			}

		}else if(TrueFalse == 3){
			TrueFalse = 4;
			Tempnode = node;
			i = 1;
		}


		node = Tempnode;
	}
	return node;
}
void insert(LINKED_TREE_AVL tree, void *data) {

	if (data == NULL) {
		printf("ERROR\n");
	} else if (tree->root == NULL) {
		tree->root = node_init(data);
	} else {
		tree->root = linked_tree_insert(tree->root, data);
	}
}



LINKED_TREE_AVL_NODE linked_tree_delete(LINKED_TREE_AVL_NODE node, void *data)
{
	LINKED_TREE_AVL_NODE temp,parent;
	LINKED_STACK stack = linked_stack_init();
	int check = 0, Left_H, Right_H, TrueFalse = 4, i = 0;

	while(check == 0){
		if(compare_data( data, node->data ) == 1){

			linked_stack_push(stack, node);
			node = node->left;

		}
		else if( compare_data( data, node->data ) == 2 ){

				linked_stack_push(stack, node);
				node = node->right;

		}else{
			check = 1;

			if((node->left==NULL)&&(node->right==NULL))
			{
				temp = linked_stack_pop(stack);
				if(node->data < temp->data){

					free(node);
					temp->left = NULL;

				}else{						// I DID THIS IF ELSE BECAUSE I CAN NOT DELETE ONE NODE WİTH FREE AND NULL THERE İS ALWAYS NODE HAVE A DATA = 0

					free(node);
					temp->right = NULL;
				}

				node = temp;

			}else if(node->left==NULL)
			{

				temp=node->right;
				free(node);
				node=temp;

			}
			else if(node->right==NULL)
			{

				temp=node->left;
				free(node);
				node=temp;

			}
			else
			{

				void* data;
				int CheckRightForLeft = 1;
				LINKED_STACK stack2 = linked_stack_init();
				LINKED_TREE_AVL_NODE temp;
				linked_stack_push(stack2, node);
				node = node->left;
				while(node->right!=NULL)
				{
					CheckRightForLeft = 0;
					linked_stack_push(stack2,node);
					node = node->right;

				}

				data = node->data;
				if(node->left != NULL){
					temp = node->left;
					node = linked_stack_pop(stack2);
					node->right = temp;

				}else{
					node = linked_stack_pop(stack2);
					if(CheckRightForLeft == 0){
						node->right = NULL;
					}else{						//Every time biggest in smaller node are not in left->right some times just left
						node->left = NULL;
					}

				}

				while(linked_stack_is_empty(stack2) == 1){

					node=linked_stack_pop(stack2);

				}

				node->data = data;

			}

		}
	}

	while(linked_stack_is_empty(stack)==1 || i == 0){

		Left_H = Find_Height(node->left);
		Right_H = Find_Height(node->right);
		node->height = Left_H - Right_H;

		if(node->height > 1 || node->height < -1){
			if(linked_stack_is_empty(stack)==1){

				LINKED_TREE_AVL_NODE temp = linked_stack_pop(stack);

				if(temp->left == node){
					TrueFalse = 1;
				}else if(temp->right == node){		//	CONTROL OF WHİCH LİNK IS LAST ONE
					TrueFalse = 2;
				}

				linked_stack_push(stack,temp);

			}else{
				i = 1;
				TrueFalse = 3;
			}

			if(node->height > 0){				//LEFT ERROR

				parent = node->left;

				if(node->left->height == 1 || node->left->height == 0 ){		//LEFT-LEFT ERROR

																	// Boolean for how we understand link with pop which node child or parent(This case Parent to Left)
					node = RightRotate(node);

				}else if(node->left->height == -1){								//LEFT-RİGHT ERROR

																	// Boolean for how we understand link with pop which node child or parent(This case Child to left)
					node->left = LeftRotate(parent);
					node = RightRotate(node);

				}

			}else if(node->height < 0){			//RIGHT EROR

				parent = node->right;

				if(node->right->height == 1){									//RIGHT-LEFT ERROR

																	// Boolean for how we understand link with pop which node child or parent(This case Child to right)
					node->right = RightRotate(parent);
					node = LeftRotate(node);

				}else if(node->right->height == 0 || node->right->height == -1){	//RIGHT-RIGHT ERROR

																	// Boolean for how we understand link with pop which node child or parent(This case Parent to right)
					node = LeftRotate(node);
				}

			}
		}


		LINKED_TREE_AVL_NODE Tempnode;
		if(TrueFalse == 1){
			Tempnode = linked_stack_pop(stack);
			Tempnode->left = node;
			TrueFalse = 4;

		}else if(TrueFalse == 2){
			Tempnode = linked_stack_pop(stack);
			Tempnode->right = node;
			TrueFalse = 4;

		}else if(TrueFalse == 4){

			TrueFalse = 4;
			if(linked_stack_is_empty(stack) != 1){
				TrueFalse = 3;
				Tempnode = node;
			}else{
				Tempnode = linked_stack_pop(stack);
			}

		}else if(TrueFalse == 3){
			TrueFalse = 4;
			Tempnode = node;
			i = 1;
		}


		node = Tempnode;
	}
	return node;
}

void delete(LINKED_TREE_AVL tree, void *key)
{
	if(tree->root!=NULL){
		tree->root = linked_tree_delete(tree->root, key);
	}else{
		printf("Key is NULL\n");
	}
}



LINKED_TREE_AVL_NODE find_Max_For_Node_Remove(LINKED_TREE_AVL_NODE node) {
	while (node->right != NULL) {
		node = node->right;
	}
	return node;
}

int myRandom() {
	return ((int) random()) * ((int) random());
}

void PrintH(LINKED_TREE_AVL_NODE node, int l){
	int i;
	if(node != NULL){

		PrintH(node->right,l+1);
		for(i=1; i < l; i++){

			printf("		");

		}

		printf("%d\n",*((int*)node->data));
		PrintH(node->left,l+1);
	}

}

int FindPower(int number,int power){
	int i,k = 1;
	for(i = 0 ;i < power; i++){
		k = number * k;
	}
	return k;
}

void *HashTable_Avl(HASH_TABLE_P newHash,void *data){
	int n1,n2;
	n1 = FindPower(*((int*)data),2);
	n2 = FindPower(*((int*)data),3);
	int Position = (n1+n2+1)%SIZE;
	if(newHash[Position].data == -1){
		newHash[Position].data = *((int*)data);
	}else{
		if(newHash[Position].Tree == NULL){
			LINKED_TREE_AVL tree = linked_tree_init();
			insert(tree,data);
			newHash[Position].Tree = tree;
		}else{
			LINKED_TREE_AVL tree2 = newHash[Position].Tree;
			insert(tree2,data);
			newHash[Position].Tree = tree2;
		}
	}
	return newHash;
}

int main() {

	HASH_TABLE_P newHash = hash_table_init();
	int a[5]={5,6,12,11,9};

	HashTable_Avl(newHash,&a[0]);
	HashTable_Avl(newHash,&a[1]);
	HashTable_Avl(newHash,&a[2]);
	HashTable_Avl(newHash,&a[3]);
	HashTable_Avl(newHash,&a[4]);
	int i;
	for(i = 0; i < SIZE ; i++){
		printf("\nARRAY DATA = %d	 POSITION = %d",newHash[i].data,i);
		if(newHash[i].Tree != NULL){
			LINKED_TREE_AVL tree = newHash[i].Tree;
			printf("\n\nTREE IS PRINTING FOR POSITION %d\n\n",i);
			PrintH(tree->root,1);
		}
	}
	return 1;
}
