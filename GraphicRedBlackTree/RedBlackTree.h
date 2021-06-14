#pragma once
#include <windows.h>

enum NODE_COLOR
{
	BLACK = 0,
	RED
};

enum NODE_DIRECTION
{
	NONE,
	LEFT,
	RIGHT
};

struct st_NODE
{
	struct st_NODE* stp_Parent;
	struct st_NODE* stp_Left;
	struct st_NODE* stp_Right;

	NODE_COLOR Color;

	int iData;
};

class RedBlackTree
{
public:
	RedBlackTree();
	~RedBlackTree();

	int InsertNode(int iData);
	void DeleteNode(int iData);
	void Clear();

	void PreorderPrint(HDC hdc, RECT size);	//전위 순회
	void InorderPirnt();	//중위 순회
	void PostorderPrint();	//후위 순회


private:
	void preorderPrint(HDC hdc, RECT size, st_NODE* node, int iDepth = 0, NODE_DIRECTION dir = NODE_DIRECTION::NONE);
	void inorderPirnt(st_NODE* node);
	void postorderPrint(st_NODE* node);

	void clear(st_NODE* pNode);
	bool linkNode(st_NODE* pParent, st_NODE* pChild);
	st_NODE* findNode(st_NODE* pNode, int iData);
	void deleteNode(st_NODE* pNode, int iData);
	st_NODE* findDeleteNode(st_NODE* pNode, st_NODE* pParent);

	void LeftRotation(st_NODE* pNode);
	void RightRotation(st_NODE* pNode);

	void InsertBalanceTree(st_NODE* pNode);
	void DeleteBalanceTree(st_NODE* pNode);

	int GetBlackCount(st_NODE* pNode);

	st_NODE Nil;
	st_NODE* m_pRoot;
	int m_iNodeCount;

};