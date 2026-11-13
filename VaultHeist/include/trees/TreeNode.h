
class TreeNode {
public:
	int val;
	TreeNode* left;
	TreeNode* right;
	int height;
	int subTreeSize;

	TreeNode();
	~TreeNode();
};

//implementation

//constructor and destructor

TreeNode::TreeNode()
{
	left = right = nullptr;
	height = subTreeSize = val = 0;
}

TreeNode::~TreeNode()
{
	if (left != nullptr)
		delete left;
	left = nullptr;
	if (right != nullptr)
		delete right;
	right = nullptr;
}