#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <psapi.h>
#include <TlHelp32.h>
#include <iostream>
#include <string.h>
#include <wchar.h>
using namespace std;

/*
TlHelp32.h for
	PROCESSENTRY32
	CreateToolhelp32Snapshot()
	Process32First()
	Process32Next()
*/
typedef struct processInof {
	unsigned int pid;		//进程id
	unsigned int provity;	//优先级
	wchar_t *pName;			//进程名称
	unsigned long pMemory;	//进程占用内存
}processInfo;

typedef struct Node {
	struct Node* next;
	struct Node* prev;
	processInfo value;
}Node,*pNode;



int IsEmptyDbLinkList(pNode pHead)
{
	pNode pt = pHead->next;

	if (pt == NULL)
		return 1;
	else
		return 0;
}

int GetLengthDbLinkList(pNode pHead)
{
	int length = 0;
	pNode pt = pHead->next;

	while (pt != NULL) {
		length++;
		pt = pt->next;
	}
	return length;
}

int InsertEleDbLinkList(pNode pHead, int pos, processInfo data)
{
	pNode pt = NULL, p_new = NULL;

	if (pos > 0 && pos < GetLengthDbLinkList(pHead) + 2) {
		p_new = (pNode)malloc(sizeof(Node));

		if (p_new == NULL) {
			printf("分配内存失败");
			exit(0);
		}

		while (1) {
			pos--;
			if (pos == 0)
				break;
			pHead = pHead->next;
		}

		pt = pHead->next;
		p_new->value = data;
		p_new->next = pt;
		if (pt != NULL) {
			pt->prev = p_new;
		}
		p_new->prev = pHead;
		pHead->next = p_new;

		return 1;
	}
	else
		return 0;
}

int DeletexEleDbLinkList(pNode pHead, int pos)
{
	pNode pt = NULL;

	if (pos > 0 && pos < GetLengthDbLinkList(pHead) + 1) {
		while (1) {
			pos--;
			if (pos == 0)
				break;
			pHead = pHead->next;
		}

		pt = pHead->next->next;
		free(pHead->next);
		pHead->next = pt;
		if (pt != NULL)
			pt->prev = pHead;
		return 1;
	}
	else
		return 0;
}

pNode CreateDbLinkList()
{
	int  length = 0;
	pNode p_new,p_curr, p_pre;
	
	pNode pHead = (pNode)malloc(sizeof(Node));

	if (pHead == NULL) {
		printf("分配内存失败\n");
		exit(234);
	}
	pHead->next = NULL;
	pHead->prev = NULL;

	p_curr = pHead->next;
	p_pre = pHead;
	HANDLE hSnapshot;
	HANDLE hProcess;
	PROCESSENTRY32 pe32;
	PROCESS_MEMORY_COUNTERS pmc;
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

	do {
		//打开对应id的进程
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
		GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc));
		p_curr = pHead->next;
		p_pre = pHead;
		while (p_curr != NULL && p_curr->value.pMemory >= pmc.WorkingSetSize/1024/1024) {
			p_pre = p_curr;
			p_curr = p_curr->next;
		}
		p_new = (pNode)malloc(sizeof(Node));
		if (p_new == NULL) {
			printf("内存分配错误\n");
			exit(123);
		}
		p_new->value.pid = pe32.th32ProcessID;					//进程名称
		p_new->value.provity = pe32.pcPriClassBase;			//基本优先级
		p_new->value.pName = pe32.szExeFile;				//进程名称
		p_new->value.pMemory = pmc.WorkingSetSize / 1024 / 1024;//进程大小;
		p_new->next = p_curr;
		if (p_curr != NULL) {
			p_curr->prev = p_new;
		}
		p_pre->next = p_new;
		p_new->prev = p_pre;
		printf("%ls\n", pHead->next->value.pName);
	} while (Process32Next(hSnapshot, &pe32));
	//while (pHead->next != NULL) {
	//	printf("%ls\n", pHead->next->value.pName);
	//	pHead = pHead->next;
	//}
	//CloseHandle(hSnapshot);   //关闭句柄
	return pHead;
}





int PrintInfo()
{
	HANDLE hSnapshot;
	HANDLE hProcess;
	PROCESSENTRY32 pe32;
	PROCESS_MEMORY_COUNTERS pmc;
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

	printf("进程ID\t父进程\t线程数\t优先级\t进程名\t占用内存"); // 基本优先级
	do {
		//打开对应id的进程
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
		GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc));
		// 打印进程相关信息
		printf("\n%u", pe32.th32ProcessID);    // 进程id
		printf("\t%u", pe32.th32ParentProcessID);  // 父进程id
		printf("\t%d", pe32.cntThreads);       // 线程数
		printf("\t%d", pe32.pcPriClassBase);   // 基本优先级
		printf("\t%ls", pe32.szExeFile);        // 进程名;
		printf("\t%ldMB", pmc.WorkingSetSize / 1024 / 1024); //占用内存 
	} while (Process32Next(hSnapshot, &pe32));


	//CloseHandle(hSnapshot);   //关闭句柄
	return 0;
}


int main(int argc, char const* argv[])
{
	pNode pHead = NULL;
	pNode p;
	//PrintInfo();
	/*pHead = CreateDbLinkList();
	p = pHead;
	while (pHead->next != NULL) {
		//printf("进程名称：%ls\t", pHead->next->value.pName);
		printf("进程id：%u\t", pHead->next->value.pid);
		printf("内存%ldMB\n", pHead->next->value.pMemory);
		p = p->next;
	}*/
	PrintInfo();
	return 0;
}