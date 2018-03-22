/********************************************************
* File:		PCencode_sys.c								*
* Brief:	系统Polar码编码源文件						*
* Author:	SherlockHsu									*
* Email:	SherlockHsu@foxmail.com						*
* Date:		2018/03/01									*
********************************************************/

#include "PCencode_sys.h"
#include <math.h>
#include <stdlib.h>

int8_t * PCencode_sys(int8_t * u, int N, int K, int8_t * FZlookup)
{
	int n = ceil(log2(N));
	int8_t* x;
	int8_t** X;
	int i, j, s, delta, t, l, kappa;

	// 初始化
	x = (int8_t*)malloc(sizeof(int8_t)*N);
	X = (int8_t**)malloc(sizeof(int8_t*)*N);
	X[0] = (int8_t*)malloc(sizeof(int8_t)*N*(n + 1));
	for (i = 1; i < N; i++)
		X[i] = X[i - 1] + (n + 1);
	j = 0;
	for (i = 0; i < N; i++)
	{
		X[i][0] = FZlookup[i];
		if (FZlookup[i] == -1)
		{
			X[i][n] = u[j];
			j++;
		}
		else
			X[i][n] = -1;
	}

	// EncoderA
	for (i = N - 1; i >= 0; i--)
	{
		if (FZlookup[i] == -1)
		{
			s = n;
			delta = -1;
		}
		else
		{
			s = 0;
			delta = 1;
		}
		for (j = 0; j < n; j++)
		{
			t = s + (j + 1)*delta;
			l = min(t, t - delta);
			kappa = 1 << (n - l - 1);
			if ((i >> (n - l - 1) & 1) == 0)
				X[i][t] = (X[i][t - delta] + X[i + kappa][t - delta]) & 1;
			else
				X[i][t] = X[i][t - delta];
		}
	}

	for (i = 0; i < N; i++)
		x[i] = X[i][n];

	// free
	free(X[0]);
	free(X);

	return x;
}