#include "RedBlackTree.h"

#include <stdio.h>
#include<math.h>

RedBlackTree::RedBlackTree()
{
	m_iNodeCount = 0;
	m_pRoot = NULL;

	Nil.Color = NODE_COLOR::BLACK;
	Nil.stp_Parent = NULL;
	Nil.stp_Left = NULL;
	Nil.stp_Right = NULL;

}

RedBlackTree::~RedBlackTree()
{
	Clear();
}

int RedBlackTree::InsertNode(int iData)
{
	st_NODE* pNewNode = new st_NODE;

	pNewNode->iData = iData;
	pNewNode->stp_Parent = NULL;
	pNewNode->stp_Left = &Nil;
	pNewNode->stp_Right = &Nil;
	pNewNode->Color = NODE_COLOR::RED;

	if (m_pRoot == NULL)
	{
		pNewNode->Color = NODE_COLOR::BLACK;
		m_pRoot = pNewNode;
	}
	else
	{

		linkNode(m_pRoot, pNewNode);
		InsertBalanceTree(pNewNode);
	}

	m_iNodeCount++;

	return pNewNode->iData;
}

void RedBlackTree::DeleteNode(int iData)
{
	if (m_pRoot != NULL)
	{
		deleteNode(m_pRoot, iData);
		m_iNodeCount--;
	}
}

void RedBlackTree::Clear()
{
	if (m_pRoot != NULL)
	{
		printf("\n");
		clear(m_pRoot);
	}
}


void RedBlackTree::PreorderPrint(HDC hdc, RECT size)
{
	if (m_pRoot != NULL)
		preorderPrint(hdc, size, m_pRoot);
}

void RedBlackTree::InorderPirnt()
{
	if (m_pRoot != NULL)
		inorderPirnt(m_pRoot);
}

void RedBlackTree::PostorderPrint()
{
	if (m_pRoot != NULL)
		postorderPrint(m_pRoot);
}


void RedBlackTree::inorderPirnt(st_NODE* node)
{
	if (node != &Nil)
	{
		inorderPirnt(node->stp_Left);
		printf("%d ", node->iData);
		inorderPirnt(node->stp_Right);
	}
}

void RedBlackTree::postorderPrint(st_NODE* node)
{
	if (node != &Nil)
	{
		postorderPrint(node->stp_Left);
		postorderPrint(node->stp_Right);
		printf("%d ", node->iData);
	}
}

void RedBlackTree::preorderPrint(HDC hdc, RECT size, st_NODE* node, int iDepth, NODE_DIRECTION dir)
{
	if (node != &Nil)
	{
		TCHAR data[15];
		int iTextPosition;

		if (node->Color == NODE_COLOR::BLACK)
			SetTextColor(hdc, RGB(0, 0, 0));
		else
			SetTextColor(hdc, RGB(255, 0, 0));

		if (dir == NODE_DIRECTION::NONE)
		{
			iTextPosition = size.right / 2;
		}
		else
		{
			MoveToEx(hdc, size.left, (iDepth - 1) * 60 + 30, NULL);

			if (dir == NODE_DIRECTION::LEFT)
			{
				iTextPosition = size.left - size.right / pow(2, iDepth + 1);
			}
			else if (dir == NODE_DIRECTION::RIGHT)
			{
				iTextPosition = size.left + size.right / pow(2, iDepth + 1);
			}

			LineTo(hdc, iTextPosition, iDepth * 60 - 5);
		}


		wsprintf(data, TEXT("%d : %d"), node->iData, GetBlackCount(node));
		TextOut(hdc, iTextPosition, iDepth * 60, data, wcslen(data));

		size.left = iTextPosition;

		preorderPrint(hdc, size, node->stp_Left, iDepth + 1, LEFT);
		preorderPrint(hdc, size, node->stp_Right, iDepth + 1, RIGHT);

		/*	printf("%d ", node->iData);
			preorderPrint(node->stp_Left);
			preorderPrint(node->stp_Right);*/
	}
}

int RedBlackTree::GetBlackCount(st_NODE* pNode)
{
	st_NODE* temp = pNode;
	int count = 0;
	while (temp != NULL)
	{
		if (temp->Color == NODE_COLOR::BLACK)
		{
			count++;
		}
		temp = temp->stp_Parent;
	}

	return count;
}

st_NODE* RedBlackTree::findNode(st_NODE* pNode, int iData)
{
	if (pNode == &Nil)
		return NULL;

	if (pNode->iData > iData)
		return findNode(pNode->stp_Left, iData);
	else if (pNode->iData > iData)
		return findNode(pNode->stp_Right, iData);
	else
		return pNode;

}

void RedBlackTree::clear(st_NODE* pNode)
{
	if (pNode->stp_Left != &Nil)
		clear(pNode->stp_Left);

	if (pNode->stp_Right != &Nil)
		clear(pNode->stp_Right);

	printf("%d delete\n", pNode->iData);

	delete pNode;
}

bool RedBlackTree::linkNode(st_NODE* pParent, st_NODE* pChild)
{
	if (pParent->iData > pChild->iData)
	{
		if (pParent->stp_Left == &Nil)
		{
			pParent->stp_Left = pChild;
			pChild->stp_Parent = pParent;
			return true;
		}
		else
		{
			linkNode(pParent->stp_Left, pChild);
		}
	}
	else if (pParent->iData < pChild->iData)
	{
		if (pParent->stp_Right == &Nil)
		{
			pParent->stp_Right = pChild;
			pChild->stp_Parent = pParent;
			return true;
		}
		else
		{
			linkNode(pParent->stp_Right, pChild);
		}
	}

	return false;
}

//
//- Node N 을 기준으로 좌회전
//
//
//		N
//
//	A       D
//
//B   C   E   F
//
//
//< N 에서 좌회전 >
//
//		  D
//
//		N    F
//
//	  A   E
//
//	B   C

void RedBlackTree::LeftRotation(st_NODE* pNode)
{
	st_NODE* stp_RightChild = pNode->stp_Right;
	pNode->stp_Right = stp_RightChild->stp_Left;

	stp_RightChild->stp_Left->stp_Parent = pNode;
	stp_RightChild->stp_Parent = pNode->stp_Parent;

	if (pNode->stp_Parent != NULL)
	{
		if (pNode == pNode->stp_Parent->stp_Right)
			pNode->stp_Parent->stp_Right = stp_RightChild;
		else
			pNode->stp_Parent->stp_Left = stp_RightChild;
	}

	stp_RightChild->stp_Left = pNode;
	pNode->stp_Parent = stp_RightChild;

	if (stp_RightChild->stp_Parent == NULL)
		m_pRoot = stp_RightChild;
}

//- Node N 을 기준으로 우회전
//
//		N
//
//	A       D
//
//B   C   E   F
//
//
//< N 에서 우회전 >
//
//		A
//
//	B       N
//
//		C     D
//
//			E    F

void RedBlackTree::RightRotation(st_NODE* pNode)
{
	st_NODE* stp_LeftChild = pNode->stp_Left;
	pNode->stp_Left = stp_LeftChild->stp_Right;

	stp_LeftChild->stp_Right->stp_Parent = pNode;
	stp_LeftChild->stp_Parent = pNode->stp_Parent;

	if (pNode->stp_Parent != NULL)
	{
		if (pNode == pNode->stp_Parent->stp_Left)
			pNode->stp_Parent->stp_Left = stp_LeftChild;
		else
			pNode->stp_Parent->stp_Right = stp_LeftChild;
	}

	stp_LeftChild->stp_Right = pNode;
	pNode->stp_Parent = stp_LeftChild;

	if (stp_LeftChild->stp_Parent == NULL)
		m_pRoot = stp_LeftChild;

	//pNode->stp_Left->stp_Parent = pNode->stp_Parent;

	//pNode->stp_Parent = pNode->stp_Left;
	//pNode->stp_Left = pNode->stp_Left->stp_Right;
	//pNode->stp_Left->stp_Parent = pNode;
	//pNode->stp_Parent->stp_Right = pNode;
}

void RedBlackTree::InsertBalanceTree(st_NODE* pNode)
{
	while (1)
	{
		if (pNode->stp_Parent->Color != NODE_COLOR::RED)
			break;

		st_NODE* stp_Parent = pNode->stp_Parent;
		st_NODE* stp_GrandFa = stp_Parent->stp_Parent;

		if (stp_Parent == stp_GrandFa->stp_Left)
		{
			st_NODE* uncle = stp_GrandFa->stp_Right;

			if (uncle->Color == NODE_COLOR::RED)
			{
				stp_Parent->Color = NODE_COLOR::BLACK;
				uncle->Color = NODE_COLOR::BLACK;
				stp_GrandFa->Color = NODE_COLOR::RED;

				if (stp_GrandFa == m_pRoot)
					break;

				pNode = stp_GrandFa;
			}
			else
			{
				if (pNode == stp_Parent->stp_Right)
				{
					LeftRotation(stp_Parent);

					pNode = stp_Parent;
					stp_Parent = stp_GrandFa->stp_Left;

				}


				stp_Parent->Color = NODE_COLOR::BLACK;
				stp_GrandFa->Color = NODE_COLOR::RED;
				RightRotation(stp_GrandFa);
				break;
			}
		}
		else
		{
			st_NODE* uncle = stp_GrandFa->stp_Left;

			if (uncle->Color == NODE_COLOR::RED)
			{
				stp_Parent->Color = NODE_COLOR::BLACK;
				uncle->Color = NODE_COLOR::BLACK;
				stp_GrandFa->Color = NODE_COLOR::RED;

				if (stp_GrandFa == m_pRoot)
					break;

				pNode = stp_GrandFa;
			}
			else
			{
				if (pNode == stp_Parent->stp_Left)
				{
					RightRotation(stp_Parent);

					pNode = stp_Parent;
					stp_Parent = stp_GrandFa->stp_Right;

				}


				stp_Parent->Color = NODE_COLOR::BLACK;
				stp_GrandFa->Color = NODE_COLOR::RED;
				LeftRotation(stp_GrandFa);
				break;
			}
		}

	}

	m_pRoot->Color = NODE_COLOR::BLACK;
}

void RedBlackTree::DeleteBalanceTree(st_NODE* pNode)
{
	st_NODE* sibling;
	st_NODE* pParent;
	st_NODE* pDeleteNode = pNode;

	if (pDeleteNode == NULL)
		return;


	while (pDeleteNode != m_pRoot)
	{
		if (m_pRoot == NULL)
		{
			break;
		}

		pParent = pDeleteNode->stp_Parent;

		if (pDeleteNode->Color == NODE_COLOR::RED)
		{
			pDeleteNode->Color = NODE_COLOR::BLACK;
			break;
		}

		//Error
		//왼쪽 자식이였는지 오른쪽 자식이였는지 판별해야하는데
		//삭제됐기때문에 알 수 가 없음.
		/*if (pParent->stp_Left == pDeleteNode || pParent->stp_Left == &Nil)*/
		if (pParent->stp_Left == pDeleteNode->stp_Left || pParent->stp_Left == pDeleteNode)
		{
			if (pParent->stp_Left->Color == NODE_COLOR::RED)
			{
				pParent->stp_Left->Color = NODE_COLOR::BLACK;
				break;
			}

			sibling = pParent->stp_Right;

			if (sibling->Color == NODE_COLOR::RED)
			{
				sibling->Color = NODE_COLOR::BLACK;
				pParent->Color = NODE_COLOR::RED;

				LeftRotation(pParent);
			}
			else
			{
				if (sibling->stp_Left->Color == NODE_COLOR::BLACK && sibling->stp_Right->Color == NODE_COLOR::BLACK)
				{
					sibling->Color = NODE_COLOR::RED;
					pDeleteNode = pParent;
				}
				else if (sibling->stp_Left->Color == NODE_COLOR::RED && sibling->stp_Right->Color == NODE_COLOR::BLACK)
				{
					sibling->Color = NODE_COLOR::RED;
					sibling->stp_Left->Color = NODE_COLOR::BLACK;
					RightRotation(sibling);

					sibling = pParent->stp_Right;
				}

				if (sibling->stp_Right->Color == NODE_COLOR::RED)
				{
					sibling->Color = pParent->Color;
					sibling->stp_Right->Color = NODE_COLOR::BLACK;
					pParent->Color = NODE_COLOR::BLACK;
					LeftRotation(pParent);

					break;
				}
			}
		}
		else if(pParent->stp_Right == pDeleteNode->stp_Right|| pParent->stp_Right == pDeleteNode)
		{
			if (pParent->stp_Right->Color == NODE_COLOR::RED)
			{
				pParent->stp_Right->Color = NODE_COLOR::BLACK;
				break;
			}

			sibling = pParent->stp_Left;

			if (sibling->Color == NODE_COLOR::RED)
			{
				sibling->Color = NODE_COLOR::BLACK;
				pParent->Color = NODE_COLOR::RED;

				RightRotation(pParent);
			}
			else
			{
				if (sibling->stp_Left->Color == NODE_COLOR::BLACK && sibling->stp_Right->Color == NODE_COLOR::BLACK)
				{
					sibling->Color = NODE_COLOR::RED;
					pDeleteNode = pParent;
				}
				else if (sibling->stp_Right->Color == NODE_COLOR::RED && sibling->stp_Left->Color == NODE_COLOR::BLACK)
				{
					sibling->Color = NODE_COLOR::RED;
					sibling->stp_Right->Color = NODE_COLOR::BLACK;
					LeftRotation(sibling);

					sibling = pParent->stp_Left;
				}

				if (sibling->stp_Left->Color == NODE_COLOR::RED)
				{
					sibling->Color = pParent->Color;
					sibling->stp_Left->Color = NODE_COLOR::BLACK;
					pParent->Color = NODE_COLOR::BLACK;
					RightRotation(pParent);
					break;
				}
			}
		}
	}

	if(m_pRoot != NULL)
		m_pRoot->Color = NODE_COLOR::BLACK;;

	Nil.stp_Parent = NULL;
}

void RedBlackTree::deleteNode(st_NODE* pNode, int iData)
{
	while (pNode->iData != iData && pNode != &Nil)
	{
		if (pNode->iData < iData)
			pNode = pNode->stp_Right;
		else
			pNode = pNode->stp_Left;
	}


	st_NODE* pParent = pNode->stp_Parent;
	st_NODE* deleteNode = NULL;

	//값은 찾았고 3개자의 경우를 확인해봐야함
	//자식이 아무도 없을때
	if (pNode->stp_Left == &Nil && pNode->stp_Right == &Nil)
	{
		if (pParent == NULL)
		{
			m_pRoot = NULL;
		}
		else if (pParent->stp_Left == pNode)
		{
			pParent->stp_Left = &Nil;
		}
		else
		{
			pParent->stp_Right = &Nil;
		}


		deleteNode = pNode;
	}
	//자식이 둘다 있을때
	//자기 자리를 대체할 기준
	//자기보다 작은값중 큰 값 (O)
	//자기보다 큰값중 작은 값
	else if (pNode->stp_Left != &Nil && pNode->stp_Right != &Nil)
	{
		st_NODE* tempNode = findDeleteNode(pNode->stp_Left, pNode);


		pNode->iData = tempNode->iData;
		tempNode->stp_Left->stp_Parent = tempNode->stp_Parent;

		if(tempNode->stp_Parent->stp_Left == tempNode)
			tempNode->stp_Parent->stp_Left = tempNode->stp_Left;
		else
			tempNode->stp_Parent->stp_Right = tempNode->stp_Left;
				
		deleteNode = tempNode;
		pNode = tempNode;

	}
	//자식이 하나만 있을때
	else
	{
		if (pNode->stp_Left != &Nil)
		{
			if (pNode != m_pRoot)
			{
				if (pParent->stp_Left != NULL && pParent->stp_Left == pNode)
				{
					pParent->stp_Left = pNode->stp_Left;
					pNode->stp_Left->stp_Parent = pParent;
				}
				else
				{
					pParent->stp_Right = pNode->stp_Left;
					pNode->stp_Left->stp_Parent = pParent;
				}
			}
			else
			{
				m_pRoot = pNode->stp_Left;
				m_pRoot->stp_Parent = NULL;
			}

			deleteNode = pNode->stp_Left;
		}
		else
		{
			if (pNode != m_pRoot)
			{
				if (pParent->stp_Left != NULL && pParent->stp_Left == pNode)
				{
					pParent->stp_Left = pNode->stp_Right;
					pNode->stp_Right->stp_Parent = pParent;

				}
				else
				{
					pParent->stp_Right = pNode->stp_Right;
					pNode->stp_Right->stp_Parent = pParent;
				}
			}
			else
			{
				m_pRoot = pNode->stp_Right;
				m_pRoot->stp_Parent = NULL;
			}	

			deleteNode = pNode->stp_Right;
		}	
	}

	if (deleteNode->Color == NODE_COLOR::BLACK)
		DeleteBalanceTree(deleteNode);
	else
		deleteNode->Color = NODE_COLOR::BLACK;

	delete pNode;
}

st_NODE* RedBlackTree::findDeleteNode(st_NODE* pNode, st_NODE* pParent)
{
	if (pNode->stp_Right == &Nil)
	{
		return pNode;
	}

	return findDeleteNode(pNode->stp_Right, pNode);
}