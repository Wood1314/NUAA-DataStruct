// 树的应用.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#pragma warning( disable : 4996)
using namespace std;

typedef struct Json {
	bool flag;
	//标记是不是对象
	string key;
	//键
	string value;
	//值
}json;

fstream file;
int n, m;
json test[10000];
//hash映射数组

string nowKey;
string tempKey;
string nowValue;

int flag = 0;

//hash函数
int Hash(char* p)
{
	int x = 1, y;
	string key = p;
	//用于碰撞处理
	for (; *p; p++) {
		x *= *p;
		x -= *p / 2;
		x %= 10000;
	}
	y = x;

	while (test[x].key.size() && !(test[x].key == key.substr(5, 1000))) {
		//如果发生碰撞，则后移
		x++;
		x %= 10000;
		if (x == y) {
			cout << "Error";
			exit(-1);
		}
	}
	return x;
}

void toJson(char* line)
{
	while (*line)
	{
		if (*line == '{')
			//对象的开始 
		{
			if (nowKey.size())
				//改变父节点的字符串 
			{
				nowKey = tempKey;
				nowKey += '.';
				tempKey = nowKey;
				flag ^= 1;
			}
			else
				//第一个'{'字符特殊处理 
			{
				nowKey = "json.";
				tempKey = nowKey;
			}
		}
		else if (*line == '"')
			//处理字符串 
		{
			if (!flag)
				//处理key 
			{
				line++;
				while (*line != '"')
				{
					if (*line == '\\')
					{
						line++;
					}
					tempKey += *line;
					line++;
				}
				//cout << tempKey << ":";
			}
			else
				//处理value 
			{
				line++;
				while (*line != '"')
				{
					if (*line == '\\')
					{
						line++;
					}
					nowValue += *line;
					line++;
				}
				//cout << nowValue << "\n";
			}
		}
		else if (*line == ':' || *line == ',' || *line == '}')
			//转变和结束 
		{
			flag ^= 1;
			if (*line == ',' || *line == '}')
				//保存键值映射到json数组 
			{
				test[Hash((char*)tempKey.c_str())].key = tempKey.substr(5, 1000);
				if (!nowValue.size())
				{
					test[Hash((char*)tempKey.c_str())].value = "OBJECT";
					test[Hash((char*)tempKey.c_str())].flag = 1;
				}
				else
				{
					test[Hash((char*)tempKey.c_str())].value = nowValue;
					test[Hash((char*)tempKey.c_str())].flag = 0;
				}
				//cout << test[hash((char *)tempKey.c_str())].key << ":" << test[hash((char *)tempKey.c_str())].value << endl;
				if (*line == ',')
				{
					tempKey = nowKey;
				}
				nowValue = "";
			}
			if (*line == '}')
				//结束一个对象，修改父节点字符串 
			{
				flag ^= 1;
				if (nowKey == "json.")
				{
					nowKey = "";
					tempKey = nowKey;
				}
				else
				{
					nowKey = nowKey.substr(0, nowKey.rfind("."));
					tempKey = nowKey;
					//cout << tempKey << endl;
					nowKey = nowKey.substr(0, nowKey.rfind(".") + 1);
					nowValue = "";
					//cout << nowKey << endl;
				}
			}
		}

		line++;
	}

	return;
}
//将文本信息处理成json结构体 

void getValue(char* line)
{
	string s = "json.";
	s += line;
	if (test[Hash((char*)s.c_str())].value.size()) {
		if (!test[Hash((char*)s.c_str())].flag) {
			cout << "STRING ";
		}
		cout << test[Hash((char*)s.c_str())].value << endl;
	}
	else {
		cout << "NOTEXIST" << endl;
	}
	return;
}

int main()
{
	char line[1000];
	file.open("data.txt", ios::in);
	file >> n >> m;
	file.getline(line, 1000);
	while (n--) {
		file.getline(line, 1000);
		toJson(line);
	}
	while (m--) {
		file.getline(line, 1000);
		getValue(line);
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
