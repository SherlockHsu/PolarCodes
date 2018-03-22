/********************************************************
 * File:	PCparams.c									*
 * Brief:	PolarCodes参数及相关函数源文件				*
 * Author:	SherlockHsu									*
 * Email:	SherlockHsu@foxmail.com						*
 * Version:	1.2.0										*
 * Date:	2017/11/10									*
 ********************************************************/

#include <math.h>
#include <string.h>
#include "PCparams.h"
#include "logdomain_tool.h"

 /* 求下行LLR函数 */
double getLowerLLR(int8_t upperdecision, double upperllr, double lowerllr);

/* 求上行LLR函数 */
double getUpperLLR(double llr1, double llr2);

/* 按位反转函数 */
int bitReverse(int num, int BIT);

/* 冻结位查询列表生成函数 */
void iniFZlookup(PCparams *pc, double SNRdB, int8_t frozenbit);

/* 符号函数 */
int sign(double x);

///* PCparams结构体初始化函数（未知FZlookup） */
//void iniPCparams(PCparams *pc, int N, int K,double SNRdB)
//{
//	int i, j;
//	pc->n = ceil(log2(N));
//	pc->N = pow(2, pc->n);
//	pc->K = K;
//
//	// 分配参数序列内存
//	pc->FZlookup = (int8_t*)malloc(sizeof(int8_t)*N);
//	pc->BitReversedList = (int*)malloc(sizeof(int)*N);
//
//	pc->LLR = (double**)malloc(sizeof(double*)*pc->N);
//	pc->LLR[0] = (double*)malloc(sizeof(double)*(pc->n + 1)*pc->N);
//	for (i = 1; i < pc->N; i++)
//		pc->LLR[i] = pc->LLR[i - 1] + (pc->n + 1);
//
//	pc->BIT = (int8_t**)malloc(sizeof(int8_t*)*pc->N);
//	pc->BIT[0] = (int8_t*)malloc(sizeof(int8_t)*(pc->n + 1)*pc->N);
//	for (i = 1; i < pc->N; i++)
//		pc->BIT[i] = pc->BIT[i - 1] + (pc->n + 1);
//
//	// 各参数序列中值的初始化
//	for (i = 0; i < N; i++)
//		pc->BitReversedList[i] = bitReverse(i, pc->n);
//	iniFZlookup(pc, SNRdB, 0);
//	memset(pc->LLR[0], 0, sizeof(double)*(pc->n + 1)*pc->N);
//
//}

/* PCparams结构体初始化函数（已知FZlookup） */
void iniPCparams(PCparams *pc, int N, int K, int8_t* FZlookup)
{
	int i, j;
	pc->n = ceil(log2(N));
	pc->N = pow(2, pc->n);
	pc->K = K;

	// 分配参数序列内存
	pc->BitReversedList = (int*)malloc(sizeof(int)*N);

	pc->LLR = (double**)malloc(sizeof(double*)*pc->N);
	pc->LLR[0] = (double*)malloc(sizeof(double)*(pc->n + 1)*pc->N);
	for (i = 1; i < pc->N; i++)
		pc->LLR[i] = pc->LLR[i - 1] + (pc->n + 1);

	pc->BIT = (int8_t**)malloc(sizeof(int8_t*)*pc->N);
	pc->BIT[0] = (int8_t*)malloc(sizeof(int8_t)*(pc->n + 1)*pc->N);
	for (i = 1; i < pc->N; i++)
		pc->BIT[i] = pc->BIT[i - 1] + (pc->n + 1);

	// 各参数序列中值的初始化
	for (i = 0; i < N; i++)
		pc->BitReversedList[i] = bitReverse(i, pc->n);
	pc->FZlookup = FZlookup;
	memset(pc->LLR[0], 0, sizeof(double)*(pc->n + 1)*pc->N);

}

/* PCparams结构体内存释放 */
void freePCparams(PCparams * pc)
{
	free(pc->FZlookup);
	free(pc->BitReversedList);
	free(pc->BIT[0]);
	free(pc->BIT);
	free(pc->LLR[0]);
	free(pc->LLR);
	free(pc);
}

/* 对数似然比列表更新 */
void updateL(int i, int j, PCparams* pc)
{
	int s = pow(2, pc->n - j);
	int l = i%s;
	if (l < s / 2)
	{
		if (pc->LLR[i][j + 1] == 0)
			updateL(i, j + 1, pc);
		if (pc->LLR[i + s / 2][j + 1] == 0)
			updateL(i + s / 2, j + 1, pc);
		pc->LLR[i][j] = getUpperLLR(pc->LLR[i][j + 1], pc->LLR[i + s / 2][j + 1]);
	}
	else
		pc->LLR[i][j] = getLowerLLR(pc->BIT[i - s / 2][j], pc->LLR[i][j + 1], pc->LLR[i - s / 2][j + 1]);
}

/* 码字判决列表更新 */
void updateB(int i, int j, PCparams * pc)
{
	int s = pow(2, pc->n - j);
	int l = i%s;
	int8_t **B = pc->BIT;
	if (l < s / 2 || j >= pc->n)
		return;
	else
	{
		B[i - s / 2][j + 1] = (B[i][j] + B[i - s / 2][j]) % 2;
		B[i][j + 1] = B[i][j];
		updateB(i, j + 1, pc);
		updateB(i - s / 2, j + 1, pc);
	}
}

void iniFZlookup(PCparams * pc, double SNRdB, int8_t frozenbit)
{
	int N = pc->N;
	int K = pc->K;
	int n = pc->n;

	double* z;
	z = (double*)malloc(sizeof(double)*N);
	double SNR = pow(10, SNRdB / 10);
	z[0] = -((double)K / N)*SNR;

	int lev, i, j, B;
	double T;
	for (lev = 0; lev < n; lev++)
	{
		B = pow(2, lev + 1);
		for (j = 0; j < B / 2; j++)
		{
			T = z[j];
			z[j] = logdomain_diff(log(2) + T, 2 * T);
			z[B / 2 + j] = 2 * T;
		}
	}

	int *seq = (int*)malloc(sizeof(int)*N);
	for (i = 0; i < N; i++)
		seq[i] = i;

	int ts;
	double tz;
	for (i = 0; i < N; i++)
		for (j = i; j < N; j++)
			if (z[i] > z[j])
			{
				tz = z[i];
				z[i] = z[j];
				z[j] = tz;
				ts = seq[i];
				seq[i] = seq[j];
				seq[j] = ts;
			}

	for (i = 0; i < K; i++)
		pc->FZlookup[seq[i]] = -1;

	for (i = K; i < N; i++)
		pc->FZlookup[seq[i]] = frozenbit;

	free(z);
	free(seq);
}

double getLowerLLR(int8_t upperdecision, double upperllr, double lowerllr)
{
	if (upperdecision == 0)
		return upperllr + lowerllr;
	else
		return upperllr - lowerllr;
}

//double getUpperLLR(double upperllr, double lowerllr)
//{
//	return logdomain_sum(upperllr + lowerllr, 0) - logdomain_sum(upperllr, lowerllr);
//}

double getUpperLLR(double upperllr, double lowerllr)
{
	double tem;
	if (fabs(upperllr) < fabs(lowerllr))
		tem = fabs(upperllr);
	else
		tem = fabs(lowerllr);
	return sign(upperllr*lowerllr)*tem;
}

int sign(double x)
{
	int z;
	if (x > 0)
		z = 1;
	else if (x < 0)
		z = -1;
	else
		z = 0;
	return(z);
}

int bitReverse(int num, int BIT)
{
	int i = BIT - 1;
	int root = num;
	int res = 0;
	while (root > 0 && i >= 0)
	{
		res += (root % 2)*pow(2, i);
		root /= 2;
		i--;
	}
	return res;
}
