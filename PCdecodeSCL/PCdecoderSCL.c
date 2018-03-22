/********************************************************
* File:		PCdecoderSCL.c								*
* Brief:	PolarCodesSCL解码器源文件					*
* Author:	Let it go									*
* Version:	2.2.2										*
* Date:		2017/11/25									*
********************************************************/

#include <math.h>
#include "PCdecoderSCL.h"
#define L_max 64

double getArrayL(double* array, int N, int L);
void getOrder(int8_t* order, double* array, int N);
int CRCtest(int8_t* info);
void getOrderRev(int8_t* orderRev, int8_t* order, int N);

//Polarcode SCl译码函数，输入：initial_LLRs对数似然比初始向量长度N、pc码字信息存放结构体，输出：info译码结构长度K///
int8_t * PCdecoderCASCL(double * initial_LLRs, PCparams * pc)
{
	int N = pc->N;
	int K = pc->K;
	int L = pc->L;
	double** LLR = pc->LLR;
	int8_t* FZlookup = pc->FZlookup;
	double* PM = pc->PM;
	int8_t** d_hat = pc->d_hat;
	int8_t* LFlag = pc->LFlag;
	int8_t* order = pc->order;

	int8_t* info = (int8_t*)malloc(sizeof(int8_t)*K);					///////为信息位分配存储空间/////////////

	int8_t d_temp;														    ///////第i个sc译码结果//////////////////

	int i, l, r, k, ldup;
	int N_path;    									                ///////N_path_temp为暂存变量////////////
	double tau;

	double* PM_temp = (double*)malloc(sizeof(double)*(2 * (pc->L)));        ///////暂存2L后路径测量值///////////////
	int8_t* ordertem = (int8_t*)malloc(sizeof(int8_t)*(2 * L));
	int8_t* orderRevtem = (int8_t*)malloc(sizeof(int8_t)*(2 * L));

	for (l = 0; l < L; l++)
		for (i = 0; i < N; i++)
			LLR[l][N - 1 + i] = initial_LLRs[i];

	N_path = 1;
	for (i = 0; i < N; i++)
	{
		r = pc->BitReversedList[i];                                        //////////////比特反转///////////////////
		if (FZlookup[r] != -1)
		{
			for (l = 0; l < N_path; l++)
			{
				updateL(l, r, pc);
				d_temp = judge_bit(LLR[l][0]);
				PM[l] = getPM(PM[l], LLR[l][0], d_temp, FZlookup[r]);
				d_hat[l][r] = FZlookup[r];
			}
		}
		else
		{
			for (l = 0; l < N_path; l++)
			{
				updateL(l, r, pc);
				d_temp = judge_bit(LLR[l][0]);
				PM_temp[l * 2] = getPM(PM[l], LLR[l][0], d_temp, 0);
				PM_temp[l * 2 + 1] = getPM(PM[l], LLR[l][0], d_temp, 1);
			}
			if (N_path < L)
			{
				for (l = 0; l < N_path; l++)
				{
					ldup = duplicatePath(l, r, pc);
					PM[l] = PM_temp[l * 2];
					PM[ldup] = PM_temp[l * 2 + 1];
				}
				N_path = N_path * 2;
			}
			else
			{
				getOrder(ordertem, PM_temp, 2 * L);
				getOrderRev(orderRevtem, ordertem, 2 * L);
				for (l = 0; l < L; l++)
				{
					if (orderRevtem[l * 2] >= L&&orderRevtem[l * 2 + 1] >= L)
						LFlag[l] = 1;
					else if (orderRevtem[l * 2] < L&&orderRevtem[l * 2 + 1] >= L)
					{
						d_hat[l][r] = 0;
						PM[l] = PM_temp[l * 2];
					}
					else if (orderRevtem[l * 2] >= L&&orderRevtem[l * 2 + 1] < L)
					{
						d_hat[l][r] = 1;
						PM[l] = PM_temp[l * 2 + 1];
					}
				}
				// 路径复制
				for (l = 0; l < L; l++)
					if (orderRevtem[l * 2] < L&&orderRevtem[l * 2 + 1] < L)
					{
						ldup = duplicatePath(l, r, pc);
						PM[l] = PM_temp[l * 2];
						PM[ldup] = PM_temp[l * 2 + 1];
					}
			}
		}
		for (l = 0; l < N_path; l++)
			updateB(l, r, pc);
	}
	getOrder(order, PM, L);
	for (l = 0; l < L; L++)
	{
		k = 0;
		for (i = 0; i < N; i++)
			if (FZlookup[i] == -1)
			{
				info[k] = d_hat[order[l]][i];
				k++;
			}
		if (CRCtest(info))
			return info;
	}

	k = 0;
	for (i = 0; i < N; i++)
		if (FZlookup[i] == -1)
		{
			info[k] = d_hat[order[0]][i];
			k++;
		}
	return info;
}

void getOrder(int8_t* order, double* array, int N)
{
	int i, j;
	double arrayT[L_max];
	int8_t ti;
	double td;
	for (i = 0; i < N; i++)
	{
		arrayT[i] = array[i];
		order[i] = i;
	}

	for (i = 0; i < N - 1; i++)
		for (j = 0; j < N - 1 - i; j++)
			if (arrayT[j] > arrayT[j + 1])
			{
				td = arrayT[j]; arrayT[j] = arrayT[j + 1]; arrayT[j + 1] = td;
				ti = order[j]; order[j] = order[j + 1]; order[j + 1] = ti;
			}
}

void getOrderRev(int8_t* orderRev, int8_t* order, int N)
{
	int i;
	for (i = 0; i < N; i++)
		orderRev[order[i]] = i;
}

double getArrayL(double* array, int N, int L)
{
	int8_t order[L_max * 2];
	getOrder(order, array, N);
	return array[order[L - 1]];
}

int CRCtest(int8_t* info)
{
	return 1;
}