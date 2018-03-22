/********************************************************
 * File:		PCdecoderSCL.c							*
 * Brief:		PolarCodesSCL解码器源文件				*
 * Author:		SherlockHsu								*
 * Version:		3.2.0									*
 * Date:		2017/11/25								*
 ********************************************************/

#include <math.h>
#include "PCdecoderCASCL.h"
#define L_max 64

 /* 获得N长数组的第L小数据 */
double getArrayL(double* array, int N, int L);

/* 获得N长数组的排序 */
void getOrder(int8_t* order, double* array, int N);

void getOrderRev(int8_t* orderRev, int8_t* order, int N);

/* CRC校验函数 */
int CRCtest(int8_t* info, int K, int8_t* g, int Lcrc);

/* Polar码CA-SCl译码函数 */
int8_t * PCdecoderCASCL(double * initial_LLRs, PCparams * pc)
{
	int N = pc->N;									// 码长
	int K = pc->K;									// 信息位长
	int L = pc->L;									// 最大路径个数
	double** LLR = pc->LLR;							// 对数似然比预留空间
	int8_t* FZlookup = pc->FZlookup;				// 冻结位查询列表
	double* PM = pc->PM;							// 路径测量值
	int8_t** d_hat = pc->d_hat;						// 各路径译码结果
	int8_t* LFlag = pc->LFlag;						// 路径状态标志数组
	int8_t* order = pc->order;						// 路径测量值排序结果

	int8_t* info =
		(int8_t*)malloc(sizeof(int8_t)*K);			// 译码结果
	int8_t d_temp;									// 当前译码结果
	int i, l, r, k, ldup;
	int N_path;										// 路径个数
	double tau;										// 路径测量值门限
	double* PM_temp =
		(double*)malloc(sizeof(double)*(2 * L));	// 临时路径测量值缓存数组
	int8_t* ordertem = (int8_t*)malloc(sizeof(int8_t)*(2 * L));
	int8_t* orderRevtem = (int8_t*)malloc(sizeof(int8_t)*(2 * L));

	// 对数似然比初始化
	for (l = 0; l < L; l++)
		for (i = 0; i < N; i++)
			LLR[l][N - 1 + i] = initial_LLRs[i];

	// 主循环
	N_path = 1;
	PM[0] = 0;
	for (i = 0; i < N; i++)
	{
		r = pc->BitReversedList[i];					// 当前译码序号
		if (FZlookup[r] != -1)
		{
			// 冻结位处理循环
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
			// 信息位处理循环
			for (l = 0; l < N_path; l++)
			{
				updateL(l, r, pc);
				d_temp = judge_bit(LLR[l][0]);
				PM_temp[l * 2] = getPM(PM[l], LLR[l][0], d_temp, 0);
				PM_temp[l * 2 + 1] = getPM(PM[l], LLR[l][0], d_temp, 1);
			}
			if (N_path < L)
			{
				// 扩大路径
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
				// 判断各路径状态
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
		// 更新码字判决列表
		for (l = 0; l < N_path; l++)
			updateB(l, r, pc);
	}
	getOrder(order, PM, L);
	// 按路径测量值顺序进行CRC校验
	for (l = 0; l < L; l++)
	{
		k = 0;
		for (i = 0; i < N; i++)
			if (FZlookup[i] == -1)
			{
				info[k] = d_hat[order[l]][i];
				k++;
			}
		if (CRCtest(info, K, pc->gcrc, pc->Lcrc))
			return info;
	}
	// 若没有序列通过CRC校验，直接输出PM值最小序列
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

int CRCtest(int8_t* info, int K, int8_t* g, int Lcrc)
{
	int i, j, tem;
	int8_t* reg = (int8_t*)malloc(sizeof(int8_t)*Lcrc);
	for (i = 0; i < Lcrc; i++)
		reg[i] = info[i];
	for (i = Lcrc; i < K; i++)
	{
		tem = reg[0];
		for (j = 0; j < Lcrc - 1; j++)
			reg[j] = reg[j + 1];
		reg[Lcrc - 1] = info[i];
		if (tem)
			for (j = 0; j < Lcrc; j++)
				reg[j] = (reg[j] + g[j]) & 1;
	}
	for (i = 0; i < Lcrc; i++)
		if (reg[i])
			return 0;
	return 1;
}