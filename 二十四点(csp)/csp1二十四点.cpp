#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <ctime>

using namespace std;

double compute(double a, double b, char c)
{
	if (c == '+')	return a + b;
	if (c == '-') return a - b;
	if (c == '*') return a * b;
	if (c == '/') return a / b;
}

double solve1(double a, double b, double c, double d, char e, char f, char g)
{
	double r1, r2, r3;
	r1 = compute(a, b, e);
	r2 = compute(r1, c, f);
	r3 = compute(r2, d, g);
	return r3;
}

double solve2(double a, double b, double c, double d, char e, char f, char g)
{
	double r1, r2, r3;
	r1 = compute(b, c, f);
	r2 = compute(a, r1, e);
	r3 = compute(r2, d, g);
	return r3;
}

double solve3(double a, double b, double c, double d, char e, char f, char g)
{
	double r1, r2, r3;
	r1 = compute(c, d, g);
	r2 = compute(b, r1, f);
	r3 = compute(a, r2, e);
	return r3;
}

double solve4(double a, double b, double c, double d, char e, char f, char g)
{
	double r1, r2, r3;
	r1 = compute(b, c, f);
	r2 = compute(r1, d, g);
	r3 = compute(a, r2, e);
	return r3;
}

double solve5(double a, double b, double c, double d, char e, char f, char g)
{
	double r1, r2, r3;
	r1 = compute(a, b, e);
	r2 = compute(c, d, g);
	r3 = compute(r1, r2, f);
	return r3;
}

bool Calculate24(double a, double b, double c, double d)
{
	int i, j, k;
	double sum;
	char op[4] = { '+','-','*','/' };
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			for (k = 0; k < 4; k++)
			{
				sum = solve1(a, b, c, d, op[i], op[j], op[k]);
				if (sum == 24)
				{
					printf("((%.0lf%c%.0lf)%c%.0lf)%c%.0lf\n", a, op[i], b, op[j], c, op[k], d);
					return 1;
				}
				sum = solve2(a, b, c, d, op[i], op[j], op[k]);
				if (sum == 24)
				{
					printf("(%.0lf%c(%.0lf%c%.0lf))%c%.0lf\n", a, op[i], b, op[j], c, op[k], d);
					return 1;
				}
				sum = solve3(a, b, c, d, op[i], op[j], op[k]);
				if (sum == 24)
				{
					printf("%.0lf%c(%.0lf%c(%.0lf%c%.0lf))\n", a, op[i], b, op[j], c, op[k], d);
					return 1;
				}
				sum = solve4(a, b, c, d, op[i], op[j], op[k]);
				if (sum == 24)
				{
					printf("%.0lf%c((%.0lf%c%.0lf)%c%.0lf)\n", a, op[i], b, op[j], c, op[k], d);
					return 1;
				}
				sum = solve5(a, b, c, d, op[i], op[j], op[k]);
				if (sum == 24)
				{
					printf("(%.0lf%c%.0lf)%c(%.0lf%c%.0lf)\n", a, op[i], b, op[j], c, op[k], d);
					return 1;
				}
			}
		}
	}
	return 0;
}

int main()
{
	int i, j, k, l;
	srand((unsigned)time(0));
	double num[4];
	bool flag = false;
	num[0] = (double)((int)rand() % 13 + 1);
	num[1] = (double)((int)rand() % 13 + 1);
	num[2] = (double)((int)rand() % 13 + 1);
	num[3] = (double)((int)rand() % 13 + 1);
	cout << "生成的四个数为：" << num[0] << "," << num[1] << "," << num[2] << "," << num[3] << endl;
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			if (i == j) continue;
			for (k = 0; k < 4; k++)
			{
				if (k == i || k == j) continue;
				for (l = 0; l < 4; l++)
				{
					if (l == k || l == i || l == j) continue;
					if (Calculate24(num[i], num[j], num[k], num[l]))
						flag = 1;
				}
			}
		}
	}
	if (!flag)
		cout << "无结果" << endl;
	return 0;
}
