#include "TreeNode.h"
#include "../containers/DynArray.h"

class BinaryTree {
private:
	TreeNode* b;

public:
	BinaryTree();
	~BinaryTree();
	TreeNode* add(TreeNode* b, int v);
	DynArray<int>* preOrder(TreeNode* b, DynArray<int>* arr);
	DynArray<int>* preOrder();
	DynArray<int>* inOrder(TreeNode* b, DynArray<int>* arr);
	DynArray<int>* inOrder();
	DynArray<int>* postOrder(TreeNode* b, DynArray<int>* arr);
	DynArray<int>* postOrder();
	DynArray<int>* levelOrder(TreeNode* b, DynArray<int>* arr);
	DynArray<int>* levelOrder();
	void reconPre(TreeNode* b);
	void reconPost(TreeNode* b);
	int getHeight();
	double getDiameter();
	TreeNode* LCA(TreeNode* b);
	void path();
	void mirror();
};

//implementation

//constructor and destructor

BinaryTree::BinaryTree()
{
	b = nullptr;
}

BinaryTree::~BinaryTree()
{
	if (b == nullptr)
		return;
	delete b;
	b = nullptr;
}

//insertion

TreeNode* BinaryTree::add(TreeNode* b, int v)
{
	if (b == nullptr)
	{
		b = new TreeNode();
		b->val = v;
		return b;
	}

	if (b->left == nullptr || b->right != nullptr)
		b = add(b->left, v);
	else
		b = add(b->right, v);

	return b;
}

// Recursive travesrsals 

DynArray<int>* BinaryTree::preOrder(TreeNode* b, DynArray<int>* arr)
{
	arr->pushBack(b->val);

	if (b->left != nullptr)
		arr = preOrder(b->left, arr);

	if (b->right != nullptr)
		arr = preOrder(b->right, arr);

	return arr;
}

DynArray<int>* BinaryTree::inOrder(TreeNode* b, DynArray<int>* arr)
{
	if (b->left != nullptr)
		arr = inOrder(b->left, arr);

	arr->pushBack(b->val);

	if (b->right != nullptr)
		arr = inOrder(b->right, arr);

	return arr;
}

DynArray<int>* BinaryTree::postOrder(TreeNode* b, DynArray<int>* arr)
{
	if (b->left != nullptr)
		arr = postOrder(b->left, arr);

	if (b->right != nullptr)
		arr = postOrder(b->right, arr);

	arr->pushBack(b->val);

	return arr;
}

DynArray<int>* BinaryTree::levelOrder(TreeNode* b, DynArray<int>* arr)
{
	arr->pushBack(b->val);

	if (b->left != nullptr)
		arr = postOrder(b->left, arr);

	if (b->right != nullptr)
		arr = postOrder(b->right, arr);

	return arr;
}

//Iterative Traversals

DynArray<int>* BinaryTree::preOrder()
{
	DynArray<int>* arr = new DynArray<int>(2);

	

}