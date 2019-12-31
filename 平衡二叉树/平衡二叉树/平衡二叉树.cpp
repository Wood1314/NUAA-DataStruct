// 平衡二叉树.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <stdlib.h>
#define LH 1
#define EH 0
#define RH -1
typedef struct BiTNode
{
	int data;
	int bf;
	struct BiTNode * lchild, * rchild;
}BiTNode,*BiTree;
//对以P为根的二叉排序树做右旋处理
void R_Rotate(BiTree* P)
{
	BiTree Rc;
	Rc = (*P)->lchild;
	(*P)->lchild = Rc->rchild;
	Rc->rchild = (*P);
	*P = Rc;
}
//对以P为根的二叉排序树做左旋处理
void L_Rotate(BiTree* P)
{
	BiTree Rc;
	Rc = (*P)->rchild;
	(*P)->rchild = Rc->rchild;
	Rc->lchild = (*P);
	*P = Rc;
}

//对以T为根节点的二叉树做左平衡旋转处理
void LeftBanlance(BiTree* T)
{
	BiTree L, Lr;
	L = (*T)->lchild;
	switch (L->bf)
	{
		case LH://检查T的左子树平衡度，并作相应平衡处理
			(*T)->bf = L->bf = EH;//新结点插入在T的左孩子的左子树上，要作单右旋处理
			R_Rotate(T);
			break;
		case RH://新结点插入在T的左孩子的右子树上，要作双旋处理
			Lr = L->rchild;
		switch(Lr->bf)
		{
			case LH:
				(*T)->bf = RH;
				L->bf = EH;
				break;
			case EH:
				(*T)->bf = L->bf = EH;
				break;
			case RH:
				(*T)->bf = EH;
				L->bf = LH;
				break;
		}
		Lr->bf = EH;
		L_Rotate(&(*T)->lchild);
		R_Rotate(T);
	}
}
//对以T为根节点的二叉树作右平衡旋转处理
void RightBalance(BiTree* T)
{
	BiTree R, R1;
	R = (*T)->rchild;
	switch (R->bf)
	{
			//检查T的右子树平衡度，并作相应平衡处理
		case RH://新结点插入在T的左孩子的左子树上，要作单右旋处理
			(*T)->bf = R->bf = EH;
			L_Rotate(T);
			break;
		case LH://新结点插入在T的左孩子的右子树上，要作双旋处理
			R1 = R->lchild;
			switch (R1->bf)
			{
				case RH:
				case EH:
					(*T)->bf = R->bf = EH;
					break;
				case LH:
					(*T)->bf  = EH;
					R->bf = RH;
					break;
			}
			R1->bf = EH;
			R_Rotate(&(*T)->rchild);
			L_Rotate(T);
	}
}
int InsertAVL(BiTree* T, int e, int* taller)
{
	if (!*T) {
		//插入新节点，树长高，置taller为1
		*T = (BiTree)malloc(sizeof(BiTNode));
		(*T)->data = e;
		(*T)->lchild = (*T)->rchild = NULL;
		(*T)->bf = EH;
		*taller = 1;
	}
	else
	{
		if (e == (*T)->data)
		{
			*taller = 0;
			return 0;
		}
		if (e < (*T)->data) {
			//应继续再T的左子树中进行搜索
			if (!InsertAVL(&(*T)->lchild, e, taller))
				return 0; //未插入
			if (*taller) {//已经插入到Tde左子树中且左子树长高
				switch ((*T)->bf) 
				{
				case LH://原本左子树比右子树高，需要作左平衡处理
					LeftBanlance(T);
					*taller = 0;
					break;
				case EH://原本左右子树等高，现因左子树增高而树增高
					(*T)->bf = LH;
					*taller = 1;
					break;
				case RH://原本右子树比左子树高，现左右子树等高
					(*T)->bf = EH;
					*taller = 0;
					break;
				}
			}
		}
		else {//应继续在T的右子树中进行搜
			if (!InsertAVL(&(*T)->rchild, e, taller))
				return 0;
			if (*taller)//已插入到T的右子树中且右子树长高
			{
				switch ((*T)->bf)
				{
				case LH://原本左子树比右子树高，现左右子树等高
					(*T)->bf = EH;
					*taller = 0;
					break;
				case EH://原本左右子树等高，现因右子树增高而树增高
					(*T)->bf = LH;
					*taller = 1;
					break;
				case RH:////原本右子树比左子树高，需要作右平衡处理
					RightBalance(T);
					*taller = 0;
					break;
				}
			}
		}
	}
	return 1;
}

int Search(BiTree& T,int data)
{
	if (T) {
		if (T->data == data)
			return 1;
		if (Search(T->lchild, data) || Search(T->rchild, data))
			return 1;
		return 0;
	}
}

void Traverse(BiTree T)
{
	if (T) {
		Traverse(T->lchild);
		printf("%d", T->data);
		Traverse(T->lchild);
	}
}

int AVLDelete(BiTree& T, int data)
{
	BiTree pre, post;
	//没有找到该点
	if (!T)
		return 0;
	else if (data == T->data) {
		//待删除叶节点为叶子节点
		if (!T->lchild && !T->rchild)
			T = NULL;
		else if (!T->lchild)
			T = T->rchild;
		else if (!T->rchild)
			T = T->lchild;
		//待删除节点既有左孩子，又有右孩子
		else
		{
			//当待删除结点*T左子树的高度大于右子树的高度时，用*T的前驱结点pre代替*T，
			//再将结点pre从树中删除。这样可以保证删除结点后的树仍为二叉平衡树。
			if (T->bf == LH) {
				pre = T->lchild;
				while (pre->rchild)
					pre = pre->rchild;
				//用pre替换T
				T->data = pre->data;
				AVLDelete(T->lchild, pre->data);
			}
			else
			{
				//寻找后继节点post
				post = T->rchild;
				while (post->rchild)
					post = post->rchild;
				T->data = post->data;
				AVLDelete(T->rchild, post->data);
			}
		}
		return true;
	}
	//在右子树中递归删除
	else if(data > T->data){
		if (!AVLDelete(T->rchild, data))
			return 0;
		return 1;
	}
	else {
		if (!AVLDelete(T->lchild, data))
			return 0;
		return 1;
	}
}

int main()
{
	int i, j;
	int a[10] = { 3,2,1,4,5,6,7,10,9,8 };
	BiTree T = NULL;
	int taller;
	for (i = 0; i < 10; i++)
	{
		j = InsertAVL(&T, a[i], &taller);
	}
	Search(T,1);
}

