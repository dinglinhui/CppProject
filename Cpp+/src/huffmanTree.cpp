#include <algorithm>
#include <iostream>
#include <string.h>
#include <stdlib.h>

#define MAXSIZE 1000

using namespace std;

struct TreeNode {
	int value;
	TreeNode* left;
	TreeNode* right;
};

int compare(const void* a, const void* b) {
	return ((*(TreeNode**) a)->value - (*(TreeNode**) b)->value);
}

TreeNode* ConstructHuffman(TreeNode** nodeArray, int n) {
	int count = n;
	while (count > 1) {
		qsort(nodeArray, count, sizeof(TreeNode*), compare);
		int temp1 = nodeArray[0]->value;
		int temp2 = nodeArray[1]->value;

		TreeNode* parent = new TreeNode;
		parent->left = nodeArray[0];
		parent->right = nodeArray[1];
		parent->value = temp1 + temp2;

		nodeArray[0] = parent;
		nodeArray[1] = nodeArray[count - 1];
		count--;
	}
	return nodeArray[0];
}
void PrintPreTree(TreeNode* node) {
	cout << node->value << endl;
	if (node->left != NULL)
		PrintPreTree(node->left);
	if (node->right != NULL)
		PrintPreTree(node->right);
}

void DeleteTree(TreeNode* node) {
	if (node->left != NULL)
		DeleteTree(node->left);
	if (node->right != NULL)
		DeleteTree(node->right);
	delete node;
	node = NULL;
}

void CountWeight(TreeNode* node, int high, int& total) {
	if (node != NULL && node->left == NULL && node->right == NULL) {
		//cout<<high<<" "<<node->value<<endl;
		total += (high * (node->value));
	}
	int currHigh = high;
	if (node->left != NULL)
		CountWeight(node->left, ++currHigh, total);
	currHigh = high;
	if (node->right != NULL)
		CountWeight(node->right, ++currHigh, total);
}

/*int main() {
	int n;
	TreeNode** nodeArray = new TreeNode*[MAXSIZE];
	while (cin >> n) {
		memset(nodeArray, 0, sizeof(TreeNode*) * MAXSIZE);
		//初始化叶节点
		TreeNode* tempTreeNode = NULL;
		for (int i = 0; i < n; ++i) {
			tempTreeNode = new TreeNode;
			tempTreeNode->left = NULL;
			tempTreeNode->right = NULL;
			cin >> tempTreeNode->value;
			nodeArray[i] = tempTreeNode;
		}
		TreeNode* root = ConstructHuffman(nodeArray, n);
		//PrintPreTree(root);
		int total = 0;
		CountWeight(root, 0, total);
		cout << total << endl;
		DeleteTree(root);
	}
}*/
