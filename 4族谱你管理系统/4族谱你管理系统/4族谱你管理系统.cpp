// 4族谱你管理系统.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#pragma warning( disable : 4996)
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <queue>
#include <algorithm>
#include <time.h>
#include <graphics.h>
using namespace std;
//定义数据的类型  
typedef struct ElemType {
	char name[30];
	char birth[30];
	char marrige[2];
	char addr[40];
	char isLive[2];
	char deadDay[30];
}ElemType;
//定义孩子兄弟二叉树
typedef struct CSNode {
	ElemType data;
	struct CSNode* firstchild;
	struct CSNode* nextsibling;
}CSNode, *CSTree;

typedef struct node
{
	CSTree p;
	int sx;
	int sy;
	int ex;
	int ey;
}widget;

fstream dataFile;

void findFather(CSTree T,char *name, CSTree &fa)
{
	if (T) {
		CSTree son = T->firstchild;
		while (son) {
			if (!strcmp(son->data.name, name)) {
				fa = T;
				return;
			}
			son = son->nextsibling;
		}
		findFather(T->firstchild, name, fa);
		findFather(T->nextsibling, name, fa);
	}
}

//打开数据文件，文件中#表示结束
bool OpenFile(string& fileName)
{
	dataFile.open(fileName, ios::in|ios::out);
	if (!dataFile.is_open())
		return false;
	return true;
}
//创建孩子兄弟二叉树
bool CreateCSTree(CSTree& T)
{
	char name[30], birth[30], marrige[2],addr[40],isLive[2],deadDay[30];
	dataFile >> name;
	if (!strcmp(name, "#")) {
		T = NULL;
	}
	else {
		T = (CSTree)malloc(sizeof(CSNode));
		if (!T) {
			cout << "分配内存失败" << endl;
			exit(1);
		}
		dataFile >> birth;
		dataFile >> marrige;
		dataFile >> addr;
		dataFile >> isLive;
		if (!strcmp(isLive, "n") || !strcmp(isLive, "N")) {
			dataFile >> deadDay;
			strcpy_s(T->data.deadDay, deadDay);
		}
		strcpy_s(T->data.name, name);
		strcpy_s(T->data.birth, birth);
		strcpy_s(T->data.marrige, marrige);
		strcpy_s(T->data.addr, addr);
		strcpy_s(T->data.isLive, isLive);
		CreateCSTree(T->firstchild);
		CreateCSTree(T->nextsibling);
	}
	return true;
}
//显示这个人的个人信息
void ShowInformation(CSTree T)
{
	cout << T->data.name << " " << T->data.birth << " " << T->data.marrige << " " << T->data.addr << " " << T->data.isLive ;
	if (!strcmp(T->data.isLive, "n") || !strcmp(T->data.isLive, "N"))
		cout << " " << T->data.deadDay;
	cout << endl;
}
//获取第n代的信息
bool GetNGenration(CSTree& T, int n)
{
	if (!T) {
		cout << "族谱为空" << endl;
		return false;
	}
	int depth = 1;
	queue<CSTree> q1, q2;
	q1.push(T);
	while (!q1.empty() || !q2.empty()) {
		CSTree temp;
		if (!q1.empty()) {
			while (!q1.empty()) {
				temp = q1.front();
				if (depth == n)				//如果该层是要输出的层，输出 
					ShowInformation(temp);
				q1.pop();
				if (temp->firstchild) q2.push(temp->firstchild);   //如果孩子不为空，就放到另一个队列 
				if (temp->nextsibling) {							 //如果兄弟不为空，还是把兄弟放进这个队列，继续遍历该层 
					q1.push(temp->nextsibling);
				}
			}
		}
		else {
			while (!q2.empty()) {
				temp = q2.front();
				if (depth == n)
					ShowInformation(temp);
				q2.pop();
				if (temp->firstchild) q1.push(temp->firstchild);
				if (temp->nextsibling) {
					q2.push(temp->nextsibling);
				}
			}
		}
		depth++;
	}
	return true;

}
//先序遍历
bool InOrderTraverse(CSTree T)
{
	if (T) {
		ShowInformation(T);
		InOrderTraverse(T->firstchild);
		InOrderTraverse(T->nextsibling);
	}
	return true;
}
//获取这个人是第几代
int GetDepth(CSTree T, char *name)
{
	if (!T) {
		cout << "族谱为空" << endl;
		return 0;
	}

	int depth = 1;
	queue<CSTree> q1, q2;
	q1.push(T);
	while (!q1.empty() || !q2.empty()) {
		CSTree temp;
		if (!q1.empty()) {
			while (!q1.empty()) {
				temp = q1.front();
				if (!strcmp(temp->data.name, name))				//如果该元素是要寻找的元素 
					return depth;
				q1.pop();
				if (temp->firstchild) q2.push(temp->firstchild);   //如果孩子不为空，就放到另一个队列 
				if (temp->nextsibling) {							 //如果兄弟不为空，还是把兄弟放进这个队列，继续遍历该层 
					q1.push(temp->nextsibling);
				}
			}
		}
		else {
			while (!q2.empty()) {
				temp = q2.front();
				if (!strcmp(temp->data.name, name))				//如果该元素是要寻找的元素 
					return depth;
				q2.pop();
				if (temp->firstchild) q1.push(temp->firstchild);
				if (temp->nextsibling) {
					q2.push(temp->nextsibling);
				}
			}
		}
		depth++;
	}
	cout << "没有找到" <<name << endl;
}

//查找这个人和它的父亲儿子的信息
bool SearchInformation(CSTree T, char *name)
{
	if (T) {
		CSTree father;
		CSTree son;
		father = T->firstchild;
		son = T->firstchild;
		if (!strcmp(T->data.name, name)) {
			cout << "他本人的信息："<< endl;
			ShowInformation(T);
			if(son)
				cout << "他孩子的信息: " << endl;
			while (son) {
				ShowInformation(son);
				son = son->nextsibling;
			}
			return true;
		}
		while (father) {
			if (!strcmp(father->data.name, name)) {
				cout << "他父亲的信息： "<<endl;
				ShowInformation(T);
			}
			father = father->nextsibling;
		}
		if (SearchInformation(T->firstchild, name) || SearchInformation(T->nextsibling, name))
			return true;
	}
	return false;
}
//按生日查询
bool SearchForBirth(CSTree T, char* birth)
{
	if (T) {
		if (!strcmp(T->data.birth, birth)) {
			ShowInformation(T);
			return true;
		}
		if (SearchForBirth(T->firstchild, birth) || SearchForBirth(T->nextsibling, birth))
			return true;
	}
	return false;
}
//确定两人关系
void DeterRela(CSTree T, char* name1, char* name2)
{
	int depth1 = GetDepth(T, name1);
	int depth2 = GetDepth(T, name2);
	if (depth1 == depth2) {
		cout << name1 << "是" << name2 << "的兄弟" << endl;
	}
	if (depth1 < depth2) {
		if (depth2 - depth1 >= 2)
			cout << name1 << "是" << name2 << "的祖父" << endl;
		else if (depth2 - depth1 == 1) {
			cout << name1 << "是" << name2 << "的父辈" << endl;
		}
	}
	if (depth1 > depth2) {
		if (depth1 - depth2 >= 2)
			cout << name2 << "是" << name1 << "的祖父" << endl;
		else if (depth1 - depth2 == 1) {
			cout << name2 << "是" << name1 << "的父辈" << endl;
		}
	}
}
//添加孩子
int AddChild(CSTree &T, char* name) {
	CSTree p_new;
	if (T) {
		if (!strcmp(T->data.name, name)) {
			p_new = (CSTree)malloc(sizeof(CSNode));
			if (p_new == NULL) {
				cout << "内存分配出错" << endl;
				exit(1);
			}
			p_new->firstchild = NULL;
			p_new->nextsibling = NULL;
			cout << "请输入要添加孩子的姓名: ";
			cin >> p_new->data.name;
			cout << "请输入要添加孩子的生日: ";
			cin >> p_new->data.birth;
			cout << "请输入要添加孩子的婚恋状况：";
			cin >> p_new->data.marrige;
			cout << "请输入要添加孩子的地址：";
			cin >> p_new->data.addr;
			cout << "请输入要添加的孩子的建在状况：";
			cin >> p_new->data.isLive;
			if (!strcmp(p_new->data.isLive, "n")) {
				cout << "请输入要添加的孩子的死亡日期：" << endl;
				cin >> p_new->data.deadDay;
			}
			if (T->firstchild == NULL) {
				T->firstchild = p_new;
				return 1;
			}
			else {
				CSTree anotherSon = T->firstchild;
				while (anotherSon->nextsibling) {
					anotherSon = anotherSon->nextsibling;
				}
				anotherSon->nextsibling = p_new;
				return 1;
			}
		}
		if (AddChild(T->firstchild, name) || AddChild(T->nextsibling, name))
			return 1;
	}
	return 0;
}
void subPrint(CSTree A, widget* data, int* cnt)
{
	//清空画布
	cleardevice();
	*cnt = 0;
	wchar_t name[20];

	//画出退出按钮
	RECT r_exit = { 535,405,600,440 };
	rectangle(535, 405, 600, 440);
	drawtext(_T("退出"), &r_exit, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	//画出返回按钮
	RECT r_return = { 430, 405, 495, 440 };
	rectangle(430, 405, 495, 440);
	drawtext(_T("返回"), &r_return, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	//画出父亲结点
	RECT r_pa = { 260,100,380,150 };
	rectangle(260, 100, 380, 150);
	//信息转换，utf-8转为Unicode，char转为wchar
	int nRet = MultiByteToWideChar(CP_ACP, 0, A->data.name, 20, name, 20);
	drawtext(name, &r_pa, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	//画出所有子结点
	CSTree  p = A->firstchild;
	while (p)
	{
		data[*cnt].p = p;
		(*cnt)++;
		p = p->nextsibling;
	}
	int x = (640 - 60 * (*cnt)) / ((*cnt) + 1);
	int i, sx = x;
	for (i = 0; i < *cnt; i++)
	{
		data[i].sx = sx;
		data[i].sy = 270;
		data[i].ex = sx + 60;
		data[i].ey = 300;
		RECT r_son = { sx,270,sx + 60,300 };
		rectangle(sx, 270, sx + 60, 300);
		line(sx + 30, 270, 320, 150);
		int nRet = MultiByteToWideChar(CP_ACP, 0, data[i].p->data.name, 20, name, 20);
		drawtext(name, &r_son, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		sx += x + 60;
	}
}
void Print(CSTree T)
{
	// 设置随机函数种子
	srand((unsigned)time(NULL));

	// 初始化图形模式
	initgraph(640, 480);

	// 设置字体
	settextstyle(16, 8, _T("Courier"));

	// 设置颜色
	COLORREF color = RGB(252, 253, 248);
	setbkcolor(color);
	settextcolor(BLACK);
	setlinecolor(BLACK);

	// 定义鼠标参数
	MOUSEMSG m;

	CSTree tree = T;
	CSTree fa = NULL;
	widget data[20];
	int cnt = 0, i;
	subPrint(T, data, &cnt);
	while (1)
	{
		m = GetMouseMsg();
		fa = NULL;
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			//退出
			if (m.x <= 600 && m.x >= 535 && m.y >= 405 && m.y <= 440)
			{
				break;
			}
			//返回
			if (m.x <= 495 && m.x >= 430 && m.y >= 405 && m.y <= 440)
			{
				findFather(T, tree->data.name, fa);
				if (fa != NULL)
				{
					subPrint(fa, data, &cnt);
					tree = fa;
				}
			}
	
			//选择子结点
			for (i = 0; i < cnt; i++)
			{
				if (m.x >= data[i].sx && m.x <= data[i].ex && m.y >= data[i].sy && m.y <= data[i].ey)
				{
					tree = data[i].p;
					subPrint(tree, data, &cnt);
				}
			}
		}
	}

	// 关闭图形模式
	closegraph();

}
//菜单
void Menue()
{
	cout << "\n==========================================族谱管理系统===================================================\n";
	cout << "                                     1.显示n代人的所有信息" << endl;
	cout << "                                     2.按姓名查找" << endl;
	cout << "                                     3.按生日查找" << endl;
	cout << "                                     4.查找两人关系" << endl;
	cout << "                                     5.添加孩子" << endl;
	cout << "                                     6.保存修改" << endl;
	cout << "                                     7.移出族谱" << endl;
	cout << "                                     8.修改成员信息" << endl;
	cout << "                                     9.绘制成员信息" << endl;
	cout << "\n========================================================================================================\n";
}
//保存修改
void SaveChange(CSTree& T) 
{
	if (T) {
		dataFile << T->data.name << " " << T->data.birth << " " << T->data.marrige << " " << T->data.addr << " " << T->data.isLive;
		if (!strcmp("n", T->data.isLive) || !strcmp("N", T->data.isLive))
			dataFile << " " << T->data.deadDay;
		dataFile << endl;
		SaveChange(T->firstchild);
		SaveChange(T->nextsibling);
	}
	else {
		dataFile << "#" << endl;
	}
}
//删除成员
void Delete(CSTree& T)
{
	CSTree child, sibling;
	if (T) {
		child = T->firstchild;
		sibling = T->nextsibling;
		free(T);
		T = NULL;
		if (child)
			Delete(child);
		if (sibling)
			Delete(sibling);
	}
}
int SearchForDelete(CSTree& T, char *name)
{
	if (T) {
		if (!strcmp(T->data.name, name)) {
			Delete(T->firstchild);
			free(T);
			T = NULL;
			return 1;
		}
		if(SearchForDelete(T->firstchild, name) || SearchForDelete(T->nextsibling, name))
			return 1;
	}
}

//修改成员信息
void Change(CSTree& T)
{
	int choice;
	while (1) {
		cout << "请输入要修改信息的序号" << endl;
		cout << "1.姓名" << endl;
		cout << "2.生日" << endl;
		cout << "3.婚否" << endl;
		cout << "4.居住地" << endl;
		cout << "5.健在" << endl;
		cout << "6.死亡日期" << endl;
		cout << "7.结束修改" << endl;
		cin >> choice;
		switch (choice)
		{
		case 1:
			cout << "请输入要修改成的姓名: " << endl;
			cin >> T->data.name;
			break;
		case 2:
			cout << "请输入要修改成的生日：" << endl;
			cin >> T->data.birth;
			break;
		case 3:
			cout << "请输入新的婚姻状态：" << endl;
			cin >> T->data.marrige;
		case 4:
			cout << "请输入新的住址：" << endl;
			cin >> T->data.addr;
		case 5:
			cout << "请输入新的健在状态：" << endl;
			cin >> T->data.isLive;
		case 6:
			cout << "请输入新的死亡日期：" << endl;
			cin >> T->data.deadDay;
		case 7:
			goto END;
			break;
		default:
			break;
		}
	}
END:return;
}
bool SearchForChange(CSTree& T, char* name)
{
	if (T) {
		if (!strcmp(T->data.name, name)) {
			Change(T);
			return true;
		}
		if (SearchForChange(T->firstchild, name) || SearchForChange(T->nextsibling, name))
			return true;
	}
	return false;
}
int main()
{
	CSTree T = NULL;
	string fileName = "data.txt";
	char str[100];
	int choice;
	if (!OpenFile(fileName)) {
		cout << "文件不存在" << endl;
		exit(0);
	}
	CreateCSTree(T);
	dataFile.close();
	while (1) {
		Menue();
		cout << "请输入你的选择：";
		scanf("%s", str);
		choice = atoi(str);
		switch (choice)
		{
		case 1:
			int n;
			cout << "请输入要查找第几代（一个整数）：";
			cin >> n;
			GetNGenration(T, n);
			break;
		case 2:
			char name[30];
			cout << "请输入要查找的姓名：";
			cin >> name;
			if (!SearchInformation(T, name))
				cout << "没有找到相应信息" << endl;
			break;
		case 3:
			char birth[30];
			cout << "请输入要查找的生日：";
			cin >> birth;
			if(!SearchForBirth(T, birth))
				cout << "没有找到相应信息" << endl;
			break;
		case 4:
			char name1[30], name2[30];
			cout << "请输入要查询的两人的姓名：" << endl;
			cin >> name1 >> name2;
			DeterRela(T, name1, name2);
			break;
		case 5:
			cout << "请输入要添加孩子的父亲的姓名：" << endl;
			cin >> name;
			if (!AddChild(T, name))
				cout << "没有找到父亲的信息：" << endl;
			break;
		case 6:
			if (!OpenFile(fileName)) {
				cout << "文件不存在" << endl;
				exit(0);
			}
			SaveChange(T);
			dataFile.close();
			break;
		case 7:
			cout << "请输入要移出族谱者的姓名：" << endl;
			cin >> name;
			if (!SearchForDelete(T, name))
				cout << "未找到要移出族谱者" << endl;
			break;
		case 8:
			cout << "请输入要修改信息者的姓名：" << endl;
			cin >> name;
			if (!SearchForChange(T, name))
				cout << "未找到要修改信息者" << endl;
			break;
		case 9:
			Print(T);
		default:
			break;
		}
		system("pause");
		system("cls");
	}
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
