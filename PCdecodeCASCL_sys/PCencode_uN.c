/********************************************************
* File:		PCencode.c									*
* Brief:	PolarCodes±àÂëÆ÷Ô´ÎÄ¼þ						*
* Author:	SherlockHsu									*
* Email:	SherlockHsu@foxmail.com						*
* Date:		2018/03/05									*
********************************************************/

#include "PCencode_uN.h"

#include <math.h>
#include <stdlib.h>

int8_t * PCencode_uN(int8_t * uN, int N, int K)
{
	int n = ceil(log2(N));
	int8_t* x = (int8_t*)malloc(sizeof(int8_t)*N);

	int i, j, group_size, op_per_group;
	int group_n, index_in_group, i1, i2;

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

	return x;

}
