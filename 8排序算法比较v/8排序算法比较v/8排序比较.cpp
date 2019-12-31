// 8排序算法比较v.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#pragma warning( disable : 4996)
#include<stdio.h>
#include<stdlib.h>
#include<algorithm>
#include <time.h>
using namespace std;
#define LEN 20000

void SelectSort(int* arr, int len)
{
	for (int i = 0; i < len - 1; i++) {
		int min = i;
		//每一趟循环比较时，min用于存放较小元素的数组下标，
		//这样当前批次比较完毕最终存放的就是此趟内最小的元素的下标，避免每次遇到较小元素都要进行交换。
		for (int j = i + 1; j < len; j++) {
			if (arr[j] < arr[min]) {
				min = j;
			}
		}
		if (min != i) {
			int temp;
			temp = arr[i];
			arr[i] = arr[min];
			arr[min] = temp;
		}
	}
}

void InsertSort(int* a, int n)
{
	int i;
	int j;
	for (i = 1; i <= n-1; i++)
	{
		int temp;
		temp = a[i];
		j = i - 1;
		while (temp < a[j])
		{
			a[j + 1] = a[j];
			j--;
		}
		a[j + 1] = temp;
	}
}

void Show(int* arr, int len)
{
	for (int i = 0; i < len; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}

void merge(int* arr, int L, int M, int R)
{
	//左边数组的大小 
	int *left = (int*)malloc(sizeof(int)*(M-L));
	//右边数组的大小 
	int *right = (int*)malloc(sizeof(int)*(R-M+1));
	//往这两个数组填充数据
	for (int i = L; i < M; i++) {
		left[i - L] = arr[i];
	}
	for (int i = M; i <= R; i++) {
		right[i - M] = arr[i];
	}
	int i = 0, j = 0;
	int k = L;

	while (i < M - L && j < R - M + 1) {
		if (left[i] < right[j])
			arr[k++] = left[i++];
		else
			arr[k++] = right[j++];
	}
	while (i < M - L)
		arr[k++] = left[i++];
	while (j < R - M + 1)
		arr[k++] = right[j++];
	free(left);
	left = NULL;
	free(right);
	right = NULL;
	//printf("%d %d %d",L,M,R);
	//for(int i=L; i<M; i++)
	//	printf("%d ",arr[i]);
	//printf("\n");
}
void BubbleSort(int* arr, int len)
{
	for (int i = 0; i < len - 1; i++) {
		int flag = 1;
		for (int j = 0; j < len - 1 - i; j++) {
			if (arr[j] > arr[j + 1]) {
				int temp;
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
				flag = 0;
			}
		}
		if (flag)
			break;
	}
}
void mergeSort(int* arr, int L, int R)
{
	if (L == R)
		return;
	int M = (L + R) / 2;
	mergeSort(arr, L, M);
	//printf("%d %d %d\n",L,M,R);
	mergeSort(arr, M + 1, R);
	merge(arr, L, M + 1, R);
}

void QuickSort(int* arr, int low, int high)
{
	if (low < high) {
		int i = low;
		int j = high;
		int k = arr[low];
		while (i < j) {
			while (i < j && arr[j] >= k)
				j--;
			if (i < j)
				arr[i++] = arr[j];
			while (i < j && arr[i] < k)
				i++;
			if (i < j)
				arr[j--] = arr[i];
		}
		arr[i] = k;
		QuickSort(arr, low, i - 1);
		QuickSort(arr, i + 1, high);
	}
}

void AdjustDown(int* A, int i, int len)
{
	int temp = A[i];

	for (int largest = 2 * i + 1; largest < len; largest = 2 * largest + 1)
	{
		if (largest != len - 1 && A[largest + 1] > A[largest])
			++largest;
		if (temp < A[largest]) {
			A[i] = A[largest];
			i = largest;
		}
		else
			break;
	}
	A[i] = temp;
}

void BuildMaxHeap(int* A, int len)
{
	for (int i = len / 2 - 1; i >= 0; i--) {  //i从len/2-1到0反复调整堆 
		AdjustDown(A, i, len);
	}
}

void HeapSort(int* A, int n)
{
	BuildMaxHeap(A, n);
	for (int i = n - 1; i > 0; --i) {
		int temp;
		temp = A[0];
		A[0] = A[i];
		A[i] = temp;

		AdjustDown(A, 0, i);
	}
}

void ShellSort(int arr[], int n)
{
	int i, j, k;
	int temp, gap;

	for (gap = n / 2; gap > 0; gap /= 2) //步长的选取
	{
		for (i = 0; i < gap; i++)        //直接插入排序原理
		{
			for (j = i + gap; j < n; j += gap)    //每次加上步长，即按列排序。
				if (arr[j] < arr[j - gap])
				{
					temp = arr[j];
					k = j - gap;
					while (k >= 0 && arr[k] > temp) //记录后移，查找插入位置
					{
						arr[k + gap] = arr[k];
						k -= gap;
					}
					arr[k + gap] = temp;  //找到位置插入
				}
		}
	}
}

void GetData(char *fileName, int * arr)
{
	FILE* fp;
	fp = fopen(fileName, "r");
	if (fp == NULL)
		puts("open file erro");
	for (int j = 0; j < LEN; j++) {
		fscanf(fp, "%d", &arr[j]);
	}
	fclose(fp);
}

//找到num的从低到高的第pos位的数据
int GetNumInPos(int num, int pos)
{
	int temp = 1;
	for (int i = 0; i < pos - 1; i++) {
		temp *= 10;
	}
	return (num / temp) % 10;
}
//基数排序 
void RadixSort(int* arr, int len)
{
	int* radixArrays[10];
	for (int i = 0; i < 10; i++) {
		radixArrays[i] = (int*)malloc(sizeof(int) * (len + 1));
		radixArrays[i][0] = 0; //indx位为0记录这组数据的个数
	}
	for (int pos = 1; pos <= 5; pos++) {//从个位到万位
		for (int i = 0; i < len; i++) {//分配过程
			int num = GetNumInPos(arr[i], pos);
			int index = ++radixArrays[num][0];
			radixArrays[num][index] = arr[i];
		}
		for (int i = 0, j = 0; i < 10; i++) {
			//收集
			for (int k = 1; k <= radixArrays[i][0]; k++)
				arr[j++] = radixArrays[i][k];
			radixArrays[i][0] = 0; //复位
		}
	}
}

int main()
{
	int arr[30000];
	char fileName[20] = "datan.txt";
	for (int i = 0; i < 10; i++) {
		fileName[4] = '0' + i;
		
		double start, finish; /* 定义开始的时间和结束的时间 */
		printf("\n====================样本%d:==========================\n", i);

		GetData(fileName, arr);
		start = (double)clock();
		InsertSort(arr, LEN);
		finish = (double)clock();
		printf("直接插入排序：%.4fms\n", (finish - start));

		GetData(fileName, arr);
		start = (double)clock();
		BubbleSort(arr, LEN);
		finish = (double)clock();
		printf("冒泡排序：%.4fms\n", (finish - start));


		GetData(fileName, arr);
		start = (double)clock();
		SelectSort(arr, LEN);
		finish = (double)clock();
		printf("选择排序：%.4fms\n", (finish - start));


		GetData(fileName, arr);
		start = (double)clock();
		mergeSort(arr,0,LEN-1);
		finish = (double)clock();
		printf("归并排序：%.4fms\n", (finish - start));

		GetData(fileName, arr);
		start = (double)clock();
		QuickSort(arr,0, 10000);
		finish = (double)clock();
		printf("快速排序：%.4fms\n", (finish - start));
		
		
		GetData(fileName, arr);
		
		start = (double)clock();
		HeapSort(arr, LEN);
		finish = (double)clock();
		printf("堆排序：%.4fms\n", (finish - start));

		GetData(fileName, arr);
		start = (double)clock();
		ShellSort(arr, LEN);
		finish = (double)clock();
		printf("希尔排序：%.4fms\n", (finish - start));

		GetData(fileName, arr);
		start = (double)clock();
		RadixSort(arr, LEN);
		finish = (double)clock();
		printf("基数排序：%.4fms\n", (finish - start));

	}
	return 0;
	
}


