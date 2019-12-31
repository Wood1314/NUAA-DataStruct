#include <stdio.h>
#include <stdlib.h>
#pragma warning( disable : 4996)
#define M 10
#define N 10
#define MAX_SIZE 1000

typedef struct step {
	int x;			//步的坐标
	int y;
	int dir;		//步的方向
}step, *pStep;

typedef struct stack {
	step e;
	pStep base;
	pStep top;
	int size;
}stack, *pStack;//定义存放步数的栈

//初始化栈
void init(pStack p)
{
	if (!p) {
		printf("stack is empty\n");
		return;
	}
	p->base = (pStep)malloc(MAX_SIZE * sizeof(step));
	if (!p->base)
		exit(-1);
	p->top = p->base;
	p->size = MAX_SIZE;
}

//压栈
void push(pStack p, step st)
{
	if (p->top - p->base + 1 == p->size) {
		printf("stack full\n");
		exit(-1);
	}
	*p->top++ = st;
}

//出栈
step pop(pStack p)
{
	if (p->base == p->top) {
		printf("Stack NULL");
		exit(-1);
	}
	p->top--;
	return *(p->top);
}
//或者栈顶
step getTop(pStack p)
{
	if (p->base == p->top) {
		printf("Stack is empty\n");
		exit(-1);
	}
	return *(p->top - 1);
}
int move[8][8] = { {0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1} };//存方向的数组
int maze[12][12];
int main()
{
	FILE* fp = NULL;
	fp = fopen("data.txt", "r");
	if (fp == NULL) {
		printf("read file erro\n");
		exit(-1);
	}
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			fscanf(fp, "%d", &maze[i][j]);
		}
	}
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			printf("%d ", maze[i][j]);
		}
		printf("\n");
	}
	int i, j,endx,endy;
	int over = 0; //是否到终点

	step cur;//当前
	cur.dir = 0;

	printf("请输入起点坐标: ");
	scanf("%d %d", &cur.x, &cur.y);
	printf("请输入终点坐标: ");
	scanf("%d %d", &endx, &endy);
	if (maze[cur.x][cur.y] == 0 || maze[endx][endy] == 0) {
		printf("无该条线路\n");
		exit(0);
	}
	stack mazeStep;
	pStack pmazeStep = &mazeStep;
	init(pmazeStep);

	
	push(pmazeStep, cur);
	push(pmazeStep, cur);//将第一步压栈两次，以便输出第一步

	while (pmazeStep->base != pmazeStep->top && over == 0) {
		pop(pmazeStep); //将不能走的步出栈

		if (pmazeStep->base != pmazeStep->top)
			cur = getTop(pmazeStep);
		while (cur.dir < 8) {
			i = cur.x + move[cur.dir][0];
			j = cur.y + move[cur.dir][1];

			if (maze[i][j] == 1) {
				cur = *(pStep)malloc(sizeof(step));
				cur.x = i;
				cur.y = j;
				cur.dir = 0;
				push(pmazeStep, cur);
				if (i == endx && j == endy) {
					over = 1;
					break;
				}
			}
			else {
				cur.dir++;
			}
		}
	}
	if (over == 0)
		printf("迷宫无正确路线！\n");
	else {
		pStep pst;
		pst = pmazeStep->base;
		while (pst != pmazeStep->top) {
			printf("%d,%d\n", pst->x, pst->y);
			pst++;
		}
	}

	return 0;
}