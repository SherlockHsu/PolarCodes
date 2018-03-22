/********************************************************
 * File:	getFZlookup.c					 			*
 * Brief:	冻结位查询序列生成函数源文件				*
 * Author:	SherlockHsu									*
 * Email:	SherlockHsu@foxmail.com						*
 * Version:	1.2.0										*
 * Date:	2017/11/10									*
 ********************************************************/

#include "getFZlookup.h"

#include <math.h>
#include <stdlib.h>
#include "logdomain_tool.h"

int8_t *getFZlookup(int N, int K, double SNRdB, int8_t frozenbit)
{
	int n = ceil(log2(N));
	int lev, i, j, B;
	double T;

	int8_t *FZlookup = (int8_t*)malloc(sizeof(int8_t)*N);

	double* z = (double*)malloc(sizeof(double)*N);
	double SNR = pow(10, SNRdB / 10);
	z[0] = -((double)K / N)*SNR;


	int *seq = (int*)malloc(sizeof(int)*N);
	for (i = 0; i < N; i++)
		seq[i] = i;

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
		FZlookup[seq[i]] = -1;

	for (i = K; i < N; i++)
		FZlookup[seq[i]] = frozenbit;

	free(z);
	free(seq);

	return FZlookup;
}
