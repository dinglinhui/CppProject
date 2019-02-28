#include <stdio.h>  
#include <string.h>  
#include <iostream>  

using namespace std;

typedef struct Node {
	char value;
	Node* left;
	Node* right;
} Node;

typedef struct BinaryTree {
	Node* root;
} BinaryTree;

void InitBinaryTree(BinaryTree* bt, char rootValue) {
	Node* root = new Node();
	root->left = NULL;
	root->right = NULL;
	root->value = rootValue;
	bt->root = root;
}

void InsertLeftTree(Node* n, char value[], int& pos);
void InsertRightTree(Node* n, char value[], int& pos);

void InsertTreeNode(BinaryTree* t, char value[], int size) {
	Node* temp = t->root;
	int pos = 0;
	if ((++pos) < strlen(value))
		InsertLeftTree(t->root, value, pos);
	if (++pos < strlen(value))
		InsertRightTree(t->root, value, pos);
}

void InsertRightTree(Node* n, char value[], int& pos) {
	if (value[pos] == '#')
		return;
	Node* node = new Node();
	node->value = value[pos];
	node->left = NULL;
	node->right = NULL;
	n->right = node;
	if ((++pos) < strlen(value))
		InsertLeftTree(node, value, pos);
	if (++pos < strlen(value))
		InsertRightTree(node, value, pos);
}

void InsertLeftTree(Node* n, char value[], int& pos) {
	if (value[pos] == '#')
		return;
	Node* node = new Node();
	node->value = value[pos];
	node->left = NULL;
	node->right = NULL;
	n->left = node;
	if ((++pos) < strlen(value))
		InsertLeftTree(node, value, pos);
	if (++pos < strlen(value))
		InsertRightTree(node, value, pos);
}

void MidBrowse(Node* node) {
	if (node->left != NULL)
		MidBrowse(node->left);
	cout << node->value << " ";
	if (node->right != NULL)
		MidBrowse(node->right);
}
/*
 int main() {
 BinaryTree bt;
 char value[101];
 memset(value, 0, sizeof(char) * 101);
 while (cin.getline(value, 101)) {
 InitBinaryTree(&bt, value[0]);
 InsertTreeNode(&bt, value, strlen(value));
 MidBrowse(bt.root);
 cout << endl;
 }
 }*/
