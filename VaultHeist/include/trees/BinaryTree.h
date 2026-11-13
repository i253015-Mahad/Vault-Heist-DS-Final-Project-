#include "TreeNode.h"

class BinaryTree {
private:
	TreeNode* b;

public:
	BinaryTree();
	~BinaryTree();
	TreeNode* add(TreeNode* b, int v);
	void preOrder(TreeNode* b);
	void inOrder(TreeNode* b);
	void postOrder(TreeNode* b);
	void levelOrder(TreeNode* b);
	void reconPre(TreeNode* b);
	void reconPost(TreeNode* b);
	int getHeight();
	double getDiameter();

};