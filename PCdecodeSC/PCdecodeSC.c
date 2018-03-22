/********************************************************
 * File:	PCdecodeSC.c								*
 * Brief:	PolarCodesSC������Դ�ļ�					*
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

	// Ϊ�о��б�������Ϣλ�����ڴ�
	int8_t* u = (int8_t*)malloc(sizeof(int8_t)*K);

	// ���ݽ��յ��Ķ�����Ȼ�ȳ�ʼ��LLR�б�
	int i, l, j;
	for (i = 0; i < N; i++)
		pc->LLR[i][n] = y[i];

	// ������ѭ��
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

	// ����Ϣλ����u
	j = 0;
	for (i = 0; i < N; i++)
		if (FZlookup[i] == -1)
		{
			u[j] = pc->BIT[i][0];
			j++;
		}

	return u;
}
