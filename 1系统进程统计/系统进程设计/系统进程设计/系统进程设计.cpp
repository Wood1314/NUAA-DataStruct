//题目说明：请耐心稍等7到20秒即可获得结束的系统进程。
#include<Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <psapi.h>
#include <TlHelp32.h>
#include <iostream>
#include <string.h>
#include <wchar.h>
#include <atlcomtime.h>
#include <string>
using namespace std;

typedef struct processInof {
	WCHAR pName[500];			//进程名称
	unsigned long pMemory;	//内存
	unsigned time;		//持续时间
	unsigned id;
	int on;
}processInfo;

typedef struct Node {
	struct Node* next;
	struct Node* prev;
	processInfo value;
}Node, * pNode;

typedef struct SNode {
	struct SNode* next;
	processInfo value;
}SNode, *sNode;

int CreateDbLinkList(pNode& pHead,sNode& phead)
{
	HANDLE hSnapshot;
	HANDLE hProcess;
	PROCESSENTRY32 pe32;
	PROCESS_MEMORY_COUNTERS pmc;
	FILETIME ftCreateing, ftExit, ftKernel, ftUser;
	// 获取进程快照
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE)
	{
		printf("CreateToolhelp32Snapshot (of processes) 失败");
		return 0;
	}
	// 设置输入参数，结构的大小
	pe32.dwSize = sizeof(PROCESSENTRY32);

	// 开始列举进程信息
	if (!Process32First(hSnapshot, &pe32))
	{
		printf("Process32First() 失败");
		CloseHandle(hSnapshot); // 关闭句柄
		return 0;
	}
	int length = 0;
	pNode p_new, p_curr, p_pre;
	processInfo value;

	pHead = (pNode)malloc(sizeof(Node));
	if (pHead == NULL) {
		printf("内存分配出错\n");
		exit(0);
	}
	pHead->next = NULL;
	pHead->next = NULL;

	
	do {
		//打开对应id的进程
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
		GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc));
		GetProcessTimes(hProcess, &ftCreateing, &ftExit, &ftKernel, &ftUser);
		wcscpy_s(value.pName, pe32.szExeFile);
		value.pMemory = pmc.WorkingSetSize / 1024 / 1024;
		value.time = 0;
		value.id = pe32.th32ProcessID;
		value.on = 1;
		
		//创建新的节点
		p_new = (pNode)malloc(sizeof(Node));
		if (p_new == NULL) {
			printf("分配内存出错");
			exit(0);
		}
		p_curr = pHead->next;
		p_pre = pHead;
		p_new->value = value;

		while (p_curr != NULL && p_curr->value.pMemory >= value.pMemory) {
			p_pre = p_curr;
			p_curr = p_curr->next;
		}
		p_new->next = p_curr;
		p_pre->next = p_new;
		p_new->prev = p_pre;
		if (p_curr != NULL)
			p_curr->prev = p_new;
	} while (Process32Next(hSnapshot, &pe32));

	CloseHandle(hSnapshot);   //关闭句柄

	//创建一个单向链表
	phead = (sNode)malloc(sizeof(SNode));
	if (phead == NULL) {
		printf("内存分配错误\n");
		exit(0);
	}
	phead->next = NULL;
	return 1;
}


void Traverse(pNode& pHead,sNode &phead)
{
	pNode p;
	sNode s;
	int count = 0;
	p = pHead->next;
	s = phead->next;
	printf("====================正在进行的进程信息================ ========================已经结束的进程==================\n");
	printf("进程名称\t       进程持续时间\t    占用内存       进程名称\t               进程结束时间\t     占用内存\n");
	while (p || s) {
		count++;
		if (p) {
			printf("%-30ls", p->value.pName);
			printf("%-8u\t", p->value.time);
			printf("%8ld MB\t", p->value.pMemory);
			p = p->next;
		}
		if (s) {
			printf("%-30ls\t", s->value.pName);
			printf("%-8u\t", s->value.time);
			printf("%8ld MB", s->value.pMemory);
			s = s->next;
		}
		printf("\n");
		if (count == 25)
			return;
	}
}
//双向链表删除
void DeleteDbLinkList(pNode pHead, processInfo value)
{
	pNode head = pHead;
	pNode pre, cur;
	pre = head, cur = head->next;
	
	while (cur) {
		if (cur->value.id == value.id) {
			pre->next = cur->next;
			if (cur->next) {
				cur->next->prev = pre;
			}
			free(cur);
			cur = NULL;//防止uaf漏洞
			return;
		}
		pre = cur;
		cur = cur->next;
	}
	
	printf("双向链表删除失败\n");
}
//双链表插入
void InsertDbLinkList(pNode &pHead ,processInfo value)
{
	pNode pre = pHead,cur,p_new;
	cur = pHead->next;
	while (cur && cur->value.pMemory >= value.pMemory) {
		pre = cur;
		cur = cur->next;
	}
	p_new = (pNode)malloc(sizeof(Node)); 
	if (p_new == NULL) {
		printf("内存分配失败\n");
		exit(0);
	}
	p_new->value.id = value.id;
	p_new->value.pMemory = value.pMemory;
	wcscpy_s(p_new->value.pName,value.pName);
	p_new->value.time = value.time;

	pre->next = p_new;
	p_new->prev = pre;
	p_new->next = cur;
	if (cur != NULL)
		cur->prev = p_new;
}
//初始化双向链表
void SetDbLinkList(pNode pHead)
{
	pNode p = pHead;
	while (p) {
		p->value.on = 0;
		p = p->next;
	}
	return;
}
//判断点该元素是否在单向链表内
int isInLinkList(sNode& phead, processInfo value)
{
	sNode p = phead->next;
	while (p) {
		if (p->value.id == value.id)
			return 1;
		p = p->next;
	}
	return  0;
}
//从单项连链表中删除元素
int DeleteLinkList(sNode& phead, processInfo value)
{
	sNode p = phead, temp;
	while (p->next) {
		if (p->next->value.id == value.id) {
			temp = p->next;
			p->next = p->next->next;
			free(temp);
			return 1;
		}
	}
	printf("单项链表删除失败\n");
	return 0;
}
//向单项链表中插入元素
int InsertLinkList(sNode& phead, processInfo value)
{
	sNode  p_new;
	p_new = (sNode)malloc(sizeof(SNode));
	if (p_new == NULL) {
		printf("内存分配出错\n");
		exit(0);
	}
	p_new->value.id = value.id;
	p_new->value.pMemory = value.pMemory;
	int length = wcslen(value.pName);
	if (length > 500)
		return 0;
	wcscpy_s(p_new->value.pName, value.pName);
	p_new->value.time = value.time;
	p_new->next = phead->next;
	phead->next = p_new;
	return 1;
}
int UpdateLinkList(pNode pHead, sNode phead, int time)
{
	pNode p;
	unsigned int id, flag;
	processInfo value;

	HANDLE hSnapshot;
	HANDLE hProcess;
	PROCESSENTRY32 pe32;
	PROCESS_MEMORY_COUNTERS pmc;
	// 获取进程快照
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE)
	{
		printf("Cr eateToolhelp32Snapshot (of processes) 失败");
		return 0;
	}
	// 设置输入参数，结构的大小
	pe32.dwSize = sizeof(PROCESSENTRY32);

	// 开始列举进程信息
	if (!Process32First(hSnapshot, &pe32))
	{
		printf("Process32First() 失败");
		CloseHandle(hSnapshot); // 关闭句柄
		return 0;
	}
	//初始化双向链表标志置位
	SetDbLinkList(pHead);
	do {
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
		GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc));
		flag = 0;
		id = pe32.th32ProcessID;
		//遍历双向链表看是否有相同id的进程
		p = pHead->next;
		while (p) {
			if (p->value.id == id) {
				p->value.on = 1;
				flag = 1;
				p->value.time = time;
				break;
			}
			p = p->next;
		}
		//如果没有就增添该进程
		if (flag == 0) {
			value.id = pe32.th32ProcessID;
			value.pMemory = pmc.WorkingSetSize/1024/1024;
			wcscpy_s(value.pName, pe32.szExeFile);
			InsertDbLinkList(pHead, value);
			if (isInLinkList(phead, value)) {
				DeleteLinkList(phead, value);
			}
		}
	} while (Process32Next(hSnapshot, &pe32));
	CloseHandle(hSnapshot);   //关闭句柄
	p = pHead;
	//寻找结束了的进程
	while (p) {
		//如果找到该进程
		if (p->value.on == 0) {
			//添加到单项链表中
			InsertLinkList(phead, p->value);
		
		}
		p = p->next;
	}
	return 1;
}

int main()
{
	pNode pHead;
	sNode phead;
	int time = 0;
	CreateDbLinkList(pHead, phead);
	while (1) {
		Traverse(pHead, phead);
		UpdateLinkList(pHead, phead, time);
		Sleep(1000);
		system("cls");
		time++;
	}
	return 0;
}