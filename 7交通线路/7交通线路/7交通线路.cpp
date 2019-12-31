#pragma warning( disable : 4996)
#include <stdio.h>
#include  <stdlib.h>
#include <Windows.h>
#include <string.h>
#include <vector>
#include <queue>
#include <stack>
using namespace std;
#define maxn 99999   //最大顶点个数
#define INF 99999999	//权值上限
int parent[maxn];
bool visited[maxn];
typedef struct info {
	char* stationName;
	char* rodeName;
}info;

typedef struct arcnode {
	int index;
	int val;
	struct arcnode* next;
}AR;

typedef struct MyGraph {
	int arcnum, vexnum;
	info *vexname;
	AR* N;
}GH;

struct node
{
	int id;
	int w;
	friend bool operator < (node a, node b)
	{
		return a.w > b.w;
	}
};
node d[maxn];
int findvex(char* s,char *t, GH* G)
{
	int i;
	for (i = 0; i < G->vexnum; i++) {
		if (!strcmp(s, G->vexname[i].stationName) && !strcmp(t,G->vexname[i].rodeName))
			return i;
	}
	return -1;
}

int findStation(char* s, GH* G)
{
	int i;
	for (int i = 0; i < G->vexnum; i++) {
		if (!strcmp(s, G->vexname[i].stationName))
			return i;
	}
	return -1;
}

int side[maxn];

void Insert(GH* G, int a, int b)
{
	AR* q = (AR*)malloc(sizeof(AR));
	if (q == NULL) {
		printf("error\n");
		exit(0);
	}
	q->index = b;
	q->val = 1;
	q->next = G->N[a].next;
	G->N[a].next = q;
}

void ShowGraph(GH* G)
{
	
	for (int i = 0; i < G->vexnum; i++) {
		printf("%s :", G->vexname[i].stationName);
		AR* p = G->N[i].next;
		while (p) {
			printf("%s %s->",G->vexname[p->index].rodeName, G->vexname[p->index].stationName);
			p = p->next;
		}
		printf("\n");
	}
}

void CreateGH1(GH* G)
{
	int i, j = 0, pos,num = 0;
	FILE* fp;
	char roadName[40], str[50], buf[3000];
	memset(str, 0, sizeof(str));
	memset(buf, 0, sizeof(buf));
	memset(side, 0, sizeof(side));
	fp = fopen("data.txt", "r");
	if (fp == NULL)
		printf("open file erro\n");
	//初始化图
	G->arcnum = 0;
	G->vexnum = 0;
	G->vexname = (info*)malloc(maxn * sizeof(info));
	G->N = (AR*)malloc(maxn * sizeof(AR));
	//从文件中读数据
	while (!feof(fp)) {
		fscanf(fp, "%s", roadName);		//读取是几路车
		fgets(buf, sizeof(buf), fp);		//获取这一路车所有的站点
		for (i = 0; buf[i]; i++) {
			if (buf[i] != ',' && buf[i] != ' ' && buf[i] != '\n')
			{
				str[j] = buf[i];
				j++;
			}
			else if (buf[i] == ',' || buf[i] == '\n') {
				pos = findStation(str, G);
				if (pos == -1) {  //如果该车站以前并没有，那么在图中加入这个车站
					G->vexname[G->vexnum].stationName = (char*)malloc((strlen(str) + 1) * sizeof(char));
					G->vexname[G->vexnum].rodeName = (char*)malloc((strlen(roadName) + 1) * sizeof(char));
					strcpy(G->vexname[G->vexnum].stationName, str);
					strcpy(G->vexname[G->vexnum].rodeName, roadName);
					G->N[G->vexnum].next = NULL;
					side[num] = G->vexnum;
					G->vexnum++;
				}
				else {				//如果这个车站有，就保存它的位置在side中
					side[num] = pos;
				}
				memset(str, 0, sizeof(str));
				j = 0;
				num++;		//站点数量+1
			}
		}
		for (i = 0; i < num-1; i++) {
			for (int k = i+1; k < num; k++) {
				Insert(G, side[i],side[k]);
				Insert(G, side[k], side[i]);
			}
		}
		num = 0;
		memset(side, 0, sizeof(side));		//清空side
	}
}

void CreateGH2(GH* G)
{
	int i, j = 0, pos, num = 0,pre = -1;
	FILE* fp;
	char roadName[40], str[50], buf[3000];
	memset(str, 0, sizeof(str));
	memset(buf, 0, sizeof(buf));
	memset(side, 0, sizeof(side));
	fp = fopen("data.txt", "r");
	if (fp == NULL)
		printf("open file erro\n");
	//初始化图
	G->arcnum = 0;
	G->vexnum = 0;
	G->vexname = (info*)malloc(maxn * sizeof(info));
	G->N = (AR*)malloc(maxn * sizeof(AR));
	//从文件中读数据
	while (!feof(fp)) {
		fscanf(fp, "%s", roadName);		//读取是几路车
		fgets(buf, sizeof(buf), fp);		//获取这一路车所有的站点
		for (i = 0; buf[i]; i++) {
			if (buf[i] != ',' && buf[i] != ' ' && buf[i] != '\n')
			{
				str[j] = buf[i];
				j++;
			}
			else if (buf[i] == ',' || buf[i] == '\n') {
				pos = findStation(str, G);
				if (pos == -1) {  //如果该车站以前并没有，那么在图中加入这个车站
					G->vexname[G->vexnum].stationName = (char*)malloc((strlen(str) + 1) * sizeof(char));
					G->vexname[G->vexnum].rodeName = (char*)malloc((strlen(roadName) + 1) * sizeof(char));
					strcpy(G->vexname[G->vexnum].stationName, str);
					strcpy(G->vexname[G->vexnum].rodeName, roadName);
					G->N[G->vexnum].next = NULL;
					if (pre == -1)
						pre = G->vexnum;
					else {
						Insert(G, pre, G->vexnum);
						Insert(G, G->vexnum, pre);
						pre = G->vexnum;
					}
					G->vexnum++;
				}
				else {
					if (pre == -1)
						pre = pos;
					else {
						Insert(G, pre, pos);
						Insert(G, pos, pre);
						pre = pos;
					}
				}
				
				memset(str, 0, sizeof(str));
				j = 0;
				num++;		//站点数量+1
			}
		}
		num = 0;
		pre = -1;
		memset(side, 0, sizeof(side));		//清空side
	}
}

priority_queue<node> q;
void Dijkstra(GH * G,int s)
{
	for (int i = 0; i < G->vexnum; i++) {
		//初始化
		d[i].id = i;
		d[i].w = INF;		
		parent[i] = -1;		
		visited[i] = false;		
	}
	d[s].w = 0;			//源点到源点最短路径是0
	q.push(d[s]);		//压入队列中
	while (!q.empty()) {
		node cd = q.top();
		q.pop();
		int u = cd.id;
		if (visited[u])
			continue;
		visited[u] = true;
		AR* p = G->N[u].next;
		//松弛操作
		while (p != NULL) {
			//找所有与他相邻的顶点，进行松弛操作，更新估算距离，压入队列
			int v = p->index;
			if (!visited[v] && d[v].w > d[u].w + p->val) {
				d[v].w = d[u].w + p->val;
				parent[v] = u;
				q.push(d[v]);
			}
			p = p->next;
		}
	}
}

int main()
{
	char start[30],end[30],choice_str[100];
	int ch, st, ed;
	GH G1, G2;
	CreateGH1(&G1);
	CreateGH2(&G2);
	while (1) {
		memset(d, 0, sizeof(d));
		printf("\n================================公交线路系统================================\n");
		printf("请输入您要查询的选项： 1.转线最少路线。2.经过站点最少路线。\n");
		scanf("%s", choice_str);
		ch = atoi(choice_str);
		printf("请输入起点和终点：(两个站点名称用空格隔开)\n");
		scanf("%s %s", start, end);
		if (ch == 1) {
			st = findStation(start, &G1);
			ed = findStation(end, &G1);
			if (st == -1 || ed == -1) {
				printf("不存在该站");
				continue;
			}
			Dijkstra(&G1, st);
			stack<int> s;
			if (d[ed].w != INF) {
				printf("最少转线次数为 %d次\n", d[ed].w - 1);
				for (int i = parent[ed]; i != -1; i = parent[i]) {
					s.push(i);
				}
				printf("路径为：");
				while (!s.empty()) {
					int temp = s.top();
					s.pop();
					printf("%s--->", G1.vexname[temp]);
				}
				printf("%s\n", G1.vexname[ed]);
			}
			else
				printf("不存在最短线路");
		}
		else if (ch == 2) {
			st = findStation(start, &G2);
			ed = findStation(end, &G2);
			if (st == -1 || ed == -1) {
				printf("不存在该站");
				continue;
			}
			Dijkstra(&G2, st);
			stack<int> s;
			if (d[ed].w != INF) {
				printf("最少经过站点数为 %d个\n", d[ed].w + 1);
				for (int i = parent[ed]; i != -1; i = parent[i]) {
					s.push(i);
				}
				printf("路径为：");
				while (!s.empty()) {
					int temp = s.top();
					s.pop();
					printf("%s--->", G1.vexname[temp]);
				}
				printf("%s\n", G1.vexname[ed]);
			}
			else
				printf("不存在最短线路");
		}

	}
	return 0;
}