/********************************************************
 * File:	PCdecodeSC.c								*
 * Brief:	PolarCodesSC解码器源文件					*
 * Author:	SherlockHsu									*
 * Email:	SherlockHsu@foxmail.com						*
 * Version:	1.2.0										*
 * Date:	2017/11/10									*
 ********************************************************/

#include <math.h>
#include "PCdecodeSC.h"

int8_t * PCdecodeSC(double * y, PCparams * pc)
{
	int N = pc->N;
	int K = pc->K;
	int n = pc->n;
	int8_t* FZlookup = pc->FZlookup;

	// 为判决列表和输出信息位分配内存
	int8_t* u = (int8_t*)malloc(sizeof(int8_t)*K);

	// 根据接收到的对数似然比初始化LLR列表
	int i, l, j;
	for (i = 0; i < N; i++)
		pc->LLR[i][n] = y[i];

	// 译码主循环
	for (i = 0; i < N; i++)
	{
		l = pc->BitReversedList[i];
		updateL(l, 0, pc);
		if (FZlookup[l] == -1)
			if (pc->LLR[l][0] > 0)
				pc->BIT[l][0] = 0;
			else
				pc->BIT[l][0] = 1;
		else
			pc->BIT[l][0] = FZlookup[l];
		updateB(l, 0, pc);
	}

	// 将信息位存入u
	j = 0;
	for (i = 0; i < N; i++)
		if (FZlookup[i] == -1)
		{
			u[j] = pc->BIT[i][0];
			j++;
		}

	return u;
}
