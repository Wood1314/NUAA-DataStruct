#include<stdio.h>
#include<stdlib.h>
typedef struct Node{
	struct Node* next;
	struct Node* pre;
	int value;
}*pNode, Node;

int CreateLinkList(pNode &pHead)
{
	pHead = (pNode)malloc(sizeof(Node));
	if(pHead == NULL){
		printf("内存分配出错\n");
		exit(0);
	}
	pHead->next = NULL;
	pHead->pre = NULL;
	pNode p_new,p_pre,p_curr;
	int value;
	for(int i=1; i<=9; i++){
		p_curr = pHead->next;
		p_pre = pHead; 
		p_new = (pNode)malloc(sizeof(Node));
		if(p_new == NULL){
			printf("内存分配出错\n");
			exit(0);
		}
		scanf("%d",&value);
		p_new->value = value;
		//printf("%d",p_new->value);
		while(p_curr!=NULL && p_curr->value >= value  ){
			p_pre = p_curr;
			p_curr = p_curr->next;
		}
		p_new->next = p_curr;
		p_pre->next = p_new;
		p_new->pre = p_pre;
		if(p_curr != NULL)
			 p_curr->pre = p_new;
	}
	return 1;
}

int Traverse(pNode pHead)
{
	pNode p;
	p = pHead->next;
	while(p){
		printf("%d",p->value);
		p = p->next;
	}
	return 1;
}

int main()
{
	pNode pHead;
	CreateLinkList(pHead);
	Traverse(pHead);
	return 0;
} 
