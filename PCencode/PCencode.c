/********************************************************
 * File:	PCencode.c									*
 * Brief:	PolarCodes±àÂëÆ÷Ô´ÎÄ¼þ						*
 * Author:	SherlockHsu									*
 * Email:	SherlockHsu@foxmail.com						*
 * Version:	1.2.0										*
 * Date:	2017/11/10									*
 ********************************************************/

#include "PCencode.h"

#include <math.h>
#include <stdlib.h>

int8_t * PCencode(int8_t * u, int N, int K, int8_t* FZlookup)
{
	int n = ceil(log2(N));
	int8_t* x = (int8_t*)malloc(sizeof(int8_t)*N);
	int8_t* uN = (int8_t*)malloc(sizeof(int8_t)*N);

	int i, j, group_size, op_per_group;
	int group_n, index_in_group, i1, i2;

	j = 0;
	for (i = 0; i < N; i++)
		if (FZlookup[i] == -1)
		{
			uN[i] = u[j];
			j++;
		}
		else
			uN[i] = FZlookup[i];

	int8_t* in = uN;


	for (i = 0; i < n; i++)
	{
		group_size = 1 << i + 1;
		op_per_group = group_size / 2;
		for (j = 0; j < N / 2; j++)
		{
			group_n = j / op_per_group;
			index_in_group = j - op_per_group*group_n;

			i1 = group_n*group_size + index_in_group;
			i2 = group_n*group_size + op_per_group + index_in_group;

			x[i1] = (in[i1] + in[i2]) & 1;
			x[i2] = in[i2];

		}
		in = x;
	}

	free(uN);

	return x;

}
