// 2表达式求值.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stack>
#include <iostream>
#include <graphics.h>
using namespace std;
char pre_ptr[1000] = "3.22";
int pos;

//获取数字
double Translation(int& pos)
{
	double integer = 0.0;
	double remainder = 0.0;
	int t = 0;

	while (pre_ptr[pos] >= '0' && pre_ptr[pos] <= '9') {
		integer *= 10;
		t += pre_ptr[pos] - '0';
		integer += t;
		pos++;
	}
	
	if (pre_ptr[pos] == '.') {
		pos++;
		t = 0;
		double cff = 0.1;
		while (pre_ptr[pos] >= '0' && pre_ptr[pos] <= '9') {
			t = pre_ptr[pos] - '0';
			remainder += cff * t;
			cff = cff * 0.1;
			pos++;
		}
	}
	return integer + remainder;
}

//获取优先级
int GetProvity(char ch)
{
	switch (ch) {
	case '+':
	case '-':
		return 1;
	case '*':
	case '/':
		return 2;
	case '(':
		return 0;
	case '#':
		return -1;
	default:
		cout << "错误运算符" << endl;
		exit(0);
	};
}

//返回运算结果
double Calclate(char op, double a1, double a2)
{
	switch (op) {
	case '+':
		return a1 + a2;
	case '-':
		return a1 - a2;
	case '*':
		return a1 * a2;
	case '/':
		return a1 / a2;
	default:
		cout << "错误运算符" << endl;
		exit(0);
	};
}

//利用两个栈进行计算
double Compute()
{
	stack<char> optr;	//操作符栈
	stack<double> num;	//操作数栈
	double Num;

	optr.push('#');
	int len;
	int is_minus = 1;
	int pos = 1;
	if (pre_ptr[0] != '#') {
		cout << "请按格式输入" << endl;
		exit(0);
	}
		
	for (len = 1; pre_ptr[len] != '#'; len++);
	while (pos < len) {
		//如果是负号
		if (pre_ptr[pos] == '-' && is_minus) {
			num.push(0);
			optr.push('-');
			printf("运算符 - 入栈");
			pos++;
		}
		else if (pre_ptr[pos] == ' ') {
			pos++;
		}
		//如果是右括号
		else if (pre_ptr[pos] == ')') {
			is_minus = 0;
			pos++;

			while (optr.top() != '(') {
				double a2 = num.top();
				num.pop();
				printf("数字 %f 出栈\n",a2);
				double a1 = num.top();
				num.pop(); 
				printf("数字 %f 出栈\n", a1);
				char op = optr.top();
				optr.pop();
				printf("运算符 %c 出栈\n", op);
				if (op == '/' && a2 == 0) {
					cout << "0不能做除数" << endl;
					exit(0);
				}
				double result = Calclate(op, a1, a2);
				num.push(result);
				printf("数字 %f 入栈\n", result);
			}
			optr.pop();
			printf("运算符 ( 出栈\n");
		}
		//如果是左括号
		else if (pre_ptr[pos] == '(') {
			is_minus = 1;
			optr.push(pre_ptr[pos]);
			printf("运算符 ( 入运算符栈");
			pos++;
		}
		//如果是数字
		else if (pre_ptr[pos] >= '0' && pre_ptr[pos] <= '9') {
			is_minus = 0;
			Num = Translation(pos);
			num.push(Num);
			printf("数字 %f 入栈\n", Num);
		}
		//其它运算符
		else {
			is_minus = 0;
			while (GetProvity(pre_ptr[pos]) <= GetProvity(optr.top())) {
				double a2 = num.top();
				num.pop();
				printf("数字 %f 出栈\n", a2);
				double a1 = num.top();
				num.pop();
				printf("数字 %f 出栈\n", a1);
				char op = optr.top();
				optr.pop();
				printf("字符 %c 出栈\n", op);
				if (op == '/' && a2 == 0) {
					cout << "0不能做除数" << endl;
					exit(0);
				}
				double result = Calclate(op, a1, a2);
				num.push(result);
				printf("数字 %f 入栈\n", result);
			}

			optr.push(pre_ptr[pos]);
			printf("运算符 %c 入栈\n", pre_ptr[pos]);
			pos++;
		}
	}
	while (optr.top() != '#') {
		double a2 = num.top();
		num.pop();
		printf("数字 %f 出栈\n", a2);
		double a1 = num.top();
		num.pop();
		printf("数字 %f 出栈\n", a1);
		char op = optr.top();
		optr.pop();
		printf("字符 %c 出栈\n", op);
		if (op == '/' && a2 == 0) {
			cout << "0不能做除数" << endl;
			exit(0);
		}
		double result = Calclate(op, a1, a2);
		num.push(result);
		printf("数字 %f 入栈\n", result);
	}
	return num.top();
}

int main()
{
	while(1){
		cout << "请输入算数表达式: ";
		cin >> pre_ptr;
		cout << "结果是: " << Compute() << endl;
	}
	
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
