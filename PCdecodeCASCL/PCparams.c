/********************************************************
 * File:	PCparams.c									*
 * Brief:	PolarCodes参数及相关函数源文件				*
 * Author:	SherlockHsu									*
 * Email:	SherlockHsu@foxmail.com						*
 * Version:	3.2.0										*
 * Date:	2017/11/10									*
 ********************************************************/

#include <math.h>
#include <string.h>
#include "PCparams.h"
#include <intrin.h>

#define PM_MAX 10000

 /* 求下行LLR函数（g函数） */
double getLowerLLR(int8_t upperdecision, double upperllr, double lowerllr);

/* 求上行LLR函数（f函数） */
double getUpperLLR(double llr1, double llr2);
//__m256i getUpperLLR(__m256i vA, __m256i vB);

/* 按位反转函数 */
int bitReverse(int num, int BIT);

/* 符号函数 */
int sign(double x);

/* PCparams结构体初始化函数(已知FZlookup) */
void iniPCparams(PCparams *pc, int N, int K, int L, int Lcrc, int8_t* FZlookup, int8_t* gcrc)
{
	int i, j;
	int n = ceil(log2(N));
	pc->n = n;
	pc->N = pow(2, pc->n);
	pc->K = K;
	pc->L = L;
	pc->FZlookup = FZlookup;
	pc->Lcrc = Lcrc;
	pc->gcrc = gcrc;

	// 分配参数序列内存
	pc->BitReversedList = (int*)malloc(sizeof(int)*N);
	pc->getBITlevelList = (int8_t*)malloc(sizeof(int8_t)*N);
	pc->getLLRlevelList = (int8_t*)malloc(sizeof(int8_t)*N);

	pc->LLR = (double**)malloc(sizeof(double*)*L);
	pc->LLR[0] = (double*)malloc(sizeof(double)*L*(N * 2 - 1));
	for (i = 1; i < L; i++)
		pc->LLR[i] = pc->LLR[i - 1] + (N * 2 - 1);

	pc->BIT1 = (int8_t**)malloc(sizeof(int8_t*)*L);
	pc->BIT1[0] = (int8_t*)malloc(sizeof(int8_t)*L*(N - 1));
	for (i = 1; i < L; i++)
		pc->BIT1[i] = pc->BIT1[i - 1] + (N - 1);

	pc->BIT2 = (int8_t**)malloc(sizeof(int8_t*)*L);
	pc->BIT2[0] = (int8_t*)malloc(sizeof(int8_t)*L*(N - 1));
	for (i = 1; i < L; i++)
		pc->BIT2[i] = pc->BIT2[i - 1] + (N - 1);

	pc->d_hat = (int8_t**)malloc(sizeof(int8_t*)*L);
	pc->d_hat[0] = (int8_t*)malloc(sizeof(int8_t)*L*N);
	for (i = 1; i < L; i++)
		pc->d_hat[i] = pc->d_hat[i - 1] + N;

	pc->LFlag = (int8_t*)malloc(sizeof(int8_t)*L);

	pc->PM = (double*)malloc(sizeof(double) * L);

	pc->order = (int8_t*)malloc(sizeof(int8_t)*L);

	// 各参数序列中值的初始化
	for (i = 0; i < N; i++)
	{
		pc->BitReversedList[i] = bitReverse(i, pc->n);
		for (j = 0; j < n; j++)
			if (i < 1 << (j + 1))
			{
				pc->getLLRlevelList[i] = n - j;
				pc->getBITlevelList[N - 1 - i] = n - j;
				break;
			}
	}

	for (i = 0; i < L; i++)
	{
		pc->PM[i] = PM_MAX;
		pc->order[i] = i;
	}


	pc->LFlag[0] = 0;
	for (i = 1; i < L; i++)
		pc->LFlag[i] = 1;

	memset(pc->LLR[0], 0, sizeof(double)*L*(N * 2 - 1));

}

/* PCparams结构体内存释放 */
void freePCparams(PCparams * pc)
{
	free(pc->FZlookup);
	free(pc->BitReversedList);
	free(pc->LLR[0]);
	free(pc->LLR);
	free(pc->BIT1[0]);
	free(pc->BIT1);
	free(pc->BIT2[0]);
	free(pc->BIT2);
	free(pc->d_hat[0]);
	free(pc->d_hat);
	free(pc->PM);
	free(pc->getBITlevelList);
	free(pc->getLLRlevelList);
	free(pc->LFlag);
	free(pc->order);
	free(pc->gcrc);
	free(pc);
}

/* 对数似然比列表更新 */
void updateL(int l, int indx, PCparams* pc)
{
	int n = pc->n;
	double** LLR = pc->LLR;
	int8_t** BIT1 = pc->BIT1;

	int nextlevel, lastlevel, st, ed;
	int i, lev;

	if (indx == 0)
		nextlevel = n;
	else
	{
		lastlevel = pc->getLLRlevelList[indx];
		st = (1 << (lastlevel - 1)) - 1;
		ed = (1 << lastlevel) - 2;
		for (i = st; i <= ed; i++)
			LLR[l][i] = getLowerLLR(
				BIT1[l][i],
				LLR[l][ed + 2 * (i - st) + 1],
				LLR[l][ed + 2 * (i - st) + 2]
			);
		nextlevel = lastlevel - 1;
	}
	for (lev = nextlevel; lev > 0; lev--)
	{
		st = (1 << (lev - 1)) - 1;
		ed = (1 << lev) - 2;
		for (i = st; i <= ed; i++)
			LLR[l][i] = getUpperLLR(
				LLR[l][ed + 2 * (i - st) + 1],
				LLR[l][ed + 2 * (i - st) + 2]
			);
	}
}

/* 码字判决列表更新 */
void updateB(int l, int indx, PCparams * pc)
{
	int N = pc->N;
	int8_t** BIT1 = pc->BIT1;
	int8_t** BIT2 = pc->BIT2;
	int8_t** d_hat = pc->d_hat;

	int lastlevel;
	int i, lev, st, ed;

	if (indx == N - 1)
		return;
	else if (indx < N / 2)
		BIT1[l][0] = d_hat[l][indx];
	else
	{
		lastlevel = pc->getBITlevelList[indx];
		BIT2[l][0] = d_hat[l][indx];
		for (lev = 1; lev <= lastlevel - 2; lev++)
		{
			st = (1 << (lev - 1)) - 1;
			ed = (1 << lev) - 2;
			for (i = st; i <= ed; i++)
			{
				BIT2[l][ed + 2 * (i - st) + 1] = (BIT1[l][i] + BIT2[l][i]) & 1;
				BIT2[l][ed + 2 * (i - st) + 2] = BIT2[l][i];
			}
		}
		lev = lastlevel - 1;
		st = (1 << (lev - 1)) - 1;
		ed = (1 << lev) - 2;
		for (i = st; i <= ed; i++)
		{
			BIT1[l][ed + 2 * (i - st) + 1] = (BIT1[l][i] + BIT2[l][i]) & 1;
			BIT1[l][ed + 2 * (i - st) + 2] = BIT2[l][i];
		}
	}
}

double getLowerLLR(int8_t upperdecision, double upperllr, double lowerllr)
{
	if (upperdecision == 0)
		return lowerllr + upperllr;
	else
		return lowerllr - upperllr;
}

//double getLowerLLR(int8_t upperdecision, double upperllr, double lowerllr)
//{
//	return upperdecision ? lowerllr - upperllr : upperllr + lowerllr;
//}

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

//__m256i getUpperLLR(__m256i vA, __m256i vB)
//{
//	const __m256i mask = _mm256_set1_epi8(0x40);
//	__m256i mA = _mm256_abs_epi8(vA);
//	__m256i mB = _mm256_abs_epi8(vB);
//	__m256i mR = _mm256_min_epi8(mA, mB);
//	__m256i sS = _mm256_xor_si256(vA, vB);
//	__m256i sR = _mm256_or_si256(mR, mask);
//	return _mm256_sign_epi8(mR, sR);
//}

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
		res += (root % 2)*(1 << i);
		root /= 2;
		i--;
	}
	return res;
}

/* 路径测量值计算函数,输入：PM_last以前路径测量值、LLn_now当前对数似然比值、u_judge判断比特、u_now当前比特 */
double getPM(double PM_last, double LLn_now, int8_t u_judge, int8_t u_now)
{
	if (u_judge == u_now)                                    ///////////////////////判断正确//////////////////////////
	{
		return PM_last;
	}
	else                                                     ///////////////////////判断错误//////////////////////////
	{
		return (PM_last + fabs(LLn_now));
	}

}

int8_t judge_bit(double LLRi)
{
	if (LLRi >= 0)
		return 0;
	else
		return 1;
}

int duplicatePath(int indx, int r, PCparams* pc)
{
	int l, i;
	int L = pc->L;
	int N = pc->N;
	for (l = 0; l < L; l++)
		if (pc->LFlag[l])
		{
			for (i = 0; i < N - 1; i++)
			{
				pc->BIT1[l][i] = pc->BIT1[indx][i];
				pc->BIT2[l][i] = pc->BIT2[indx][i];
				pc->LLR[l][i] = pc->LLR[indx][i];
				pc->d_hat[l][i] = pc->d_hat[indx][i];
			}
			pc->d_hat[indx][r] = 0;
			pc->d_hat[l][r] = 1;
			pc->LFlag[l] = 0;
			return l;
		}
	return indx;
}