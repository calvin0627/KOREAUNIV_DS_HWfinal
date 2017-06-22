#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <string.h>

#define RED 'r';
#define BLACK 'b';


struct Node {
	struct Node * left;
	struct Node * right;
	struct Node * parent;
	int val;
	char color;
};
//typedef struct Node *struct Node *;

struct RBTree {
	struct Node * root;
	struct Node * TreeNIL;
};
//typedef struct RBTree *struct RBTree *;

struct Node * initNode(int val) {
	struct Node * self = (struct Node *)malloc(sizeof(struct Node));
	self->left = NULL;//66666666666666666666666666666666666666666666666666666666666666
	self->right = NULL;//
	self->parent = NULL;//
	self->color = RED;
	if (val != NULL)
		self->val = val;
	return self;
}

struct RBTree * initRBT() {
	struct RBTree * self = (struct RBTree *)malloc(sizeof(struct RBTree));
	self->root = NULL;//6666666666666666666666666666666666666666666666666666666666666666666666666666666666
	self->TreeNIL = initNode(NULL);
	self->TreeNIL->color = BLACK;
	return self;
}

//void leftRotate(struct RBTree *,struct Node * n);
//void rightRotate(struct RBTree *,struct Node * n);

//struct Node * uncle(struct Node * n);
//struct Node * grand(struct Node * n);
//void insert_RBT(struct RBTree * ,struct Node * n);

void transplant(struct RBTree * T, struct Node* u, struct Node *v) {
	if (u->parent == T->TreeNIL)
		T->root = v;
	else if (u == u->parent->left)
		u->parent->left = v;
	else u->parent->right = v;
	//if(v!=NULL)
	//if(v!=T->TreeNIL)
	v->parent = u->parent;
}

void leftRotate(struct RBTree * T, struct Node * x) {
	struct Node * y = x->right;

	x->right = y->left;
	if (y->left != T->TreeNIL)
		y->left->parent = x;
	y->parent = x->parent;

	if (x->parent == T->TreeNIL)//x는 root
	{
		T->root = y;
	}
	else if (x == x->parent->left) {//x가 leftchild
		x->parent->left = y;
	}
	else if (x == x->parent->right) {//x가 rightchild
		x->parent->right = y;
	}
	y->left = x;
	x->parent = y;
}

void rightRotate(struct RBTree * T, struct Node * x) {
	struct Node * y = x->left;

	x->left = y->right;
	if (y->right != T->TreeNIL)
		y->right->parent = x;
	y->parent = x->parent;
	if (x->parent == T->TreeNIL)//x는 root
	{
		T->root = y;
	}
	else if (x == x->parent->left) {//x가 leftchild
		x->parent->left = y;
	}
	else if (x == x->parent->right) {//x가 rightchild
		x->parent->right = y;
	}

	y->right = x;
	x->parent = y;
}

void changeColor(struct Node* x, struct Node * y) {
	char tmp;

	tmp = x->color;
	x->color = y->color;
	y->color = tmp;
}

struct Node * grand(struct RBTree * T, struct Node * n) {
	if ((n != T->TreeNIL) && (n->parent != T->TreeNIL))
		return n->parent->parent;
	else
		return NULL;
}

struct Node * uncle(struct RBTree * T, struct Node * n) {
	if (grand(T, n) == T->TreeNIL)
		return NULL;
	else {
		if (n->parent == grand(T, n)->left)
			return grand(T, n)->right;
		else
			return grand(T, n)->left;
	}
}

void bst_insert(struct RBTree * tree, struct Node * treeNode, struct Node * n) {

	if (tree->root == NULL)//6666666666666666666666666666666666666666666666666666666
	{
		tree->root = n;
	}
	if (treeNode == NULL) {
		tree->root = n;
	}
	else if (n->val < treeNode->val) {
		if (treeNode->left == tree->TreeNIL) {
			treeNode->left = n;
			treeNode->left->parent = treeNode;
		}
		else
			bst_insert(tree, treeNode->left, n);
	}
	else {
		if (treeNode->right == tree->TreeNIL) {
			treeNode->right = n;
			treeNode->right->parent = treeNode;
		}
		else
			bst_insert(tree, treeNode->right, n);
	}
}

void FIX(struct RBTree *, struct Node *);

void insert_RBT(struct RBTree * tree, struct Node * newNode) {
	newNode->parent = tree->TreeNIL;
	newNode->left = tree->TreeNIL;
	newNode->right = tree->TreeNIL;

	bst_insert(tree, tree->root, newNode);

	FIX(tree, newNode);
}

void FIX(struct RBTree * tree, struct Node * n) {
	if (n == tree->root) n->color = BLACK;

	if (n->parent->color == 'b') return;

	if (uncle(tree, n) != tree->TreeNIL&&uncle(tree, n)->color == 'r') {
		n->parent->color = BLACK;
		uncle(tree, n)->color = BLACK;
		grand(tree, n)->color = RED;
		FIX(tree, grand(tree, n));
	}
	else {
		if ((n == n->parent->right) && (n->parent == grand(tree, n)->left)) {
			leftRotate(tree, n->parent);
			n = n->left;
		}
		else if ((n == n->parent->left) && (n->parent == grand(tree, n)->right)) {
			rightRotate(tree, n->parent);
			n = n->right;
		}
		changeColor(grand(tree, n), n->parent);
		if (n == n->parent->left)
			rightRotate(tree, grand(tree, n));
		else
			leftRotate(tree, grand(tree, n));
	}

}

void inorder(struct RBTree * T, struct Node * n) {
	if (n != T->TreeNIL) {
		inorder(T, n->left);
		printf("%d %C\n", n->val, n->color - 32);
		inorder(T, n->right);
	}
}

void printTree(struct RBTree *self, struct Node* tree, int level) {
	if (tree->right != self->TreeNIL)
		printTree(self, tree->right, level + 1);
	for (int i = 0; i < level; i++)
		printf("    ");
	printf("%d[%c]\n", tree->val, tree->color);
	if (tree->left != self->TreeNIL)
		printTree(self, tree->left, level + 1);
}

int total(struct RBTree * T, struct Node * n, int num) {
	if (n != T->TreeNIL) {
		num = total(T, n->left, num);
		num++;
		num = total(T, n->right, num);
	}
	return num;
}
int inorder3(struct RBTree * T, struct Node * n, int num) {
	if (n != T->TreeNIL) {
		num = inorder3(T, n->left, num);
		if (n->color == 'b')//why not BLACK???
			num++;
		num = inorder3(T, n->right, num);
	}
	return num;
}

int numOfBlack(struct RBTree* tree) {
	int num = 0;
	num = inorder3(tree, tree->root, num);
	return num;
}

int blackHeight(struct RBTree*tree) {
	struct Node* node = tree->root;
	int num = 0;

	do {
		if (node->color == 'b')
			num++;
		node = node->left;
	} while (node->left != tree->TreeNIL);

	return num;
}

struct Node * minimum(struct RBTree * T, struct Node * node) {
	if (node->left == T->TreeNIL)
		return node;
	else
		return minimum(T, node->left);
}

void fix_delete(struct RBTree*, struct Node*);

struct Node * search(struct RBTree * T, struct Node * self, int val) {
	if (self == T->TreeNIL)
		return T->TreeNIL;
	else if (val == self->val)
		return self;
	else if (val < self->val)
		return search(T, self->left, val);
	else
		return search(T, self->right, val);
}

void delete_RBT(struct RBTree *tree, struct Node *n) {
	struct Node *y = NULL;//node moved or removed
	struct Node *x = NULL;//temp's original position
	char y_color;

	y = n;
	y_color = n->color;

	if (n->left == tree->TreeNIL) {
		x = n->right;
		transplant(tree, y, y->right);
	}
	else if (n->right == tree->TreeNIL) {
		x = n->left;
		transplant(tree, y, y->left);
	}
	else {
		y = n->right;
		y = minimum(tree, y);

		y_color = y->color;
		x = y->right;//y n자리로이동, x y 자리로이동

		transplant(tree, y, y->right);
		y->right = n->right;
		y->right->parent = y;

		transplant(tree, n, y);
		y->left = n->left;
		y->left->parent = y;
		y->color = n->color;
	}

	if (y_color == 'b')
		fix_delete(tree, x);
}

void fix_delete(struct RBTree* tree, struct Node *n) {
	struct Node * sibling = NULL;

	while ((n != tree->root) && (n->color == 'b')) {
		if (n == (n->parent->left)) {
			sibling = n->parent->right;

			if (sibling->color == 'r') {//case 1
				changeColor(sibling, n->parent);//99999999999999999999
				leftRotate(tree, n->parent);
				sibling = n->parent->right;
			}
			//case2 
			if (sibling->left->color == 'b'&&sibling->right->color == 'b') {
				sibling->color = RED;
				n = n->parent;
			}
			//case 3
			else if (sibling->left->color == 'r'&&sibling->right->color == 'b') {
				changeColor(sibling, sibling->left);//99999999999
				rightRotate(tree, sibling);
				sibling = n->parent->right;
			}
			//case4
			if (sibling->right->color == 'r') {
				sibling->color = n->parent->color;
				sibling->right->color = BLACK;
				n->parent->color = BLACK;
				leftRotate(tree, n->parent);

				//n = tree->root;
				break;
			}
		}
		else {
			sibling = n->parent->left;

			if (sibling->color == 'r') {//case 1
				changeColor(sibling, n->parent);//999999999
				rightRotate(tree, n->parent);
				sibling = n->parent->left;
			}
			//case2 
			if (sibling->left->color == 'b'&&sibling->right->color == 'b') {
				sibling->color = RED;
				n = n->parent;
			}
			//case 3
			else if (sibling->right->color == 'r'&&sibling->left->color == 'b') {
				changeColor(sibling, sibling->right);
				leftRotate(tree, sibling);
				sibling = n->parent->left;
			}
			//case4
			if (sibling->left->color == 'r') {
				sibling->color = n->parent->color;
				sibling->left->color = BLACK;
				n->parent->color = BLACK;
				rightRotate(tree, n->parent);

				//n=tree->root;
				break;
			}
		}
	}
	n->color = BLACK;
}

struct Node* successor(struct RBTree* tree, struct Node* x)
{
	struct Node *tmp = NULL;
	if (x->right!=tree->TreeNIL)
		return minimum(tree,x->right);

	else
	{
		tmp = x->parent;
		while (tmp != NULL && x == tmp->right)
		{
			x = tmp;
			tmp = tmp->parent;
		}
		return tmp;
	}
}

struct Node* maximum(struct RBTree* tree,struct Node* n) {
	if (n->right == tree->TreeNIL)
		return n;
	else
		return maximum(tree, n->right);
}

struct Node* predessor(struct RBTree* tree, struct Node* n) {
	struct Node* tmp = NULL;
	if (n->left != tree->TreeNIL)
		return maximum(tree, n->left);
	else {
		tmp = n->parent;
		while (tmp != NULL && n == tmp->left)
		{
			n = tmp;
			tmp = tmp->parent;
		}
		return tmp;
	}
}

void main() {
	const wchar_t temp1[50][50];
	FILE *fp[50];
	int stk = 0;
	WIN32_FIND_DATA ffd;
	TCHAR szDir[MAX_PATH];
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;
	const wchar_t fileName[50][50];

		struct RBTree * tree = initRBT();
		struct Node * nodeTemp = NULL;

		FILE* fp1 = fopen("prjtest\\test01.txt", "r");
		FILE* fp2 = fopen("prjtest\\search01.txt","r");
		FILE* fp3 = fopen("prjtest\\ouput01.txt","w");


		int i = -1;
		int size = 0;
		while (fscanf(fp1, "%d", &i) != EOF) {
			if (i > 0) {
				nodeTemp = initNode(i);
				insert_RBT(tree, nodeTemp);
				size++;
			}
			else if (i < 0) {
				nodeTemp = search(tree, tree->root, (-1)*i);
				if (nodeTemp == NULL) {
					//printf("%d 는 없는값\n",(-1)*i);
					continue;
				}
				delete_RBT(tree, nodeTemp);
				size--;
			}
			else {
				break;
			}
		}
		//printTree(tree, tree->root, 0);
		fclose(fp1);
		i = -1;

		while (fscanf(fp2, "%d", &i) != EOF) { 
			if (i == 0)
				break;
			nodeTemp = search(tree, tree->root, i);
			if (nodeTemp == tree->TreeNIL) {
				nodeTemp = initNode(i);
				insert_RBT(tree, nodeTemp);
				if (predessor(tree, nodeTemp) == tree->TreeNIL)fprintf(fp3,"NIL ");
				else fprintf(fp3,"%d ", predessor(tree, nodeTemp)->val);
				fprintf(fp3,"NIL ");
				if (successor(tree, nodeTemp) == tree->TreeNIL)fprintf(fp3,"NIL \n");
				else fprintf(fp3,"%d\n", successor(tree, nodeTemp)->val);
				delete_RBT(tree, nodeTemp);
			}
			else {//값 존재
				if(predessor(tree,nodeTemp)==tree->TreeNIL)fprintf(fp3,"NIL ");
				else fprintf(fp3,"%d ", predessor(tree, nodeTemp)->val);
				fprintf(fp3,"%d ",nodeTemp->val);
				if (successor(tree, nodeTemp) == tree->TreeNIL)fprintf(fp3,"NIL \n");
				else fprintf(fp3,"%d\n", successor(tree, nodeTemp)->val);
			}
		}
		fclose(fp3);
		fclose(fp2);
}