// 8�����㷨�Ƚ�v.cpp : ���ļ����� "main" ����������ִ�н��ڴ˴���ʼ��������
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
		//ÿһ��ѭ���Ƚ�ʱ��min���ڴ�Ž�СԪ�ص������±꣬
		//������ǰ���αȽ�������մ�ŵľ��Ǵ�������С��Ԫ�ص��±꣬����ÿ��������СԪ�ض�Ҫ���н�����
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
	//�������Ĵ�С 
	int *left = (int*)malloc(sizeof(int)*(M-L));
	//�ұ�����Ĵ�С 
	int *right = (int*)malloc(sizeof(int)*(R-M+1));
	//�������������������
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
	for (int i = len / 2 - 1; i >= 0; i--) {  //i��len/2-1��0���������� 
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

	for (gap = n / 2; gap > 0; gap /= 2) //������ѡȡ
	{
		for (i = 0; i < gap; i++)        //ֱ�Ӳ�������ԭ��
		{
			for (j = i + gap; j < n; j += gap)    //ÿ�μ��ϲ���������������
				if (arr[j] < arr[j - gap])
				{
					temp = arr[j];
					k = j - gap;
					while (k >= 0 && arr[k] > temp) //��¼���ƣ����Ҳ���λ��
					{
						arr[k + gap] = arr[k];
						k -= gap;
					}
					arr[k + gap] = temp;  //�ҵ�λ�ò���
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

//�ҵ�num�Ĵӵ͵��ߵĵ�posλ������
int GetNumInPos(int num, int pos)
{
	int temp = 1;
	for (int i = 0; i < pos - 1; i++) {
		temp *= 10;
	}
	return (num / temp) % 10;
}
//�������� 
void RadixSort(int* arr, int len)
{
	int* radixArrays[10];
	for (int i = 0; i < 10; i++) {
		radixArrays[i] = (int*)malloc(sizeof(int) * (len + 1));
		radixArrays[i][0] = 0; //indxλΪ0��¼�������ݵĸ���
	}
	for (int pos = 1; pos <= 5; pos++) {//�Ӹ�λ����λ
		for (int i = 0; i < len; i++) {//�������
			int num = GetNumInPos(arr[i], pos);
			int index = ++radixArrays[num][0];
			radixArrays[num][index] = arr[i];
		}
		for (int i = 0, j = 0; i < 10; i++) {
			//�ռ�
			for (int k = 1; k <= radixArrays[i][0]; k++)
				arr[j++] = radixArrays[i][k];
			radixArrays[i][0] = 0; //��λ
		}
	}
}

int main()
{
	int arr[30000];
	char fileName[20] = "datan.txt";
	for (int i = 0; i < 10; i++) {
		fileName[4] = '0' + i;
		
		double start, finish; /* ���忪ʼ��ʱ��ͽ�����ʱ�� */
		printf("\n====================����%d:==========================\n", i);

		GetData(fileName, arr);
		start = (double)clock();
		InsertSort(arr, LEN);
		finish = (double)clock();
		printf("ֱ�Ӳ�������%.4fms\n", (finish - start));

		GetData(fileName, arr);
		start = (double)clock();
		BubbleSort(arr, LEN);
		finish = (double)clock();
		printf("ð������%.4fms\n", (finish - start));


		GetData(fileName, arr);
		start = (double)clock();
		SelectSort(arr, LEN);
		finish = (double)clock();
		printf("ѡ������%.4fms\n", (finish - start));


		GetData(fileName, arr);
		start = (double)clock();
		mergeSort(arr,0,LEN-1);
		finish = (double)clock();
		printf("�鲢����%.4fms\n", (finish - start));

		GetData(fileName, arr);
		start = (double)clock();
		QuickSort(arr,0, 10000);
		finish = (double)clock();
		printf("��������%.4fms\n", (finish - start));
		
		
		GetData(fileName, arr);
		
		start = (double)clock();
		HeapSort(arr, LEN);
		finish = (double)clock();
		printf("������%.4fms\n", (finish - start));

		GetData(fileName, arr);
		start = (double)clock();
		ShellSort(arr, LEN);
		finish = (double)clock();
		printf("ϣ������%.4fms\n", (finish - start));

		GetData(fileName, arr);
		start = (double)clock();
		RadixSort(arr, LEN);
		finish = (double)clock();
		printf("��������%.4fms\n", (finish - start));

	}
	return 0;
	
}


