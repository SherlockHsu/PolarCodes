/********************************************************
* File:		mexFunction.c								*
* Brief:	生成.mexw64文件								*
* Author:	SherlockHsu									*
* Email:	SherlockHsu@foxmail.com						*
* Date:		2018/03/01									*
********************************************************/

#include <mex.h>
#include <stdint.h>
#include <stdlib.h>

#include "PCencode_sys.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	double* uD = mxGetPr(prhs[0]);
	int N = mxGetScalar(prhs[1]);
	int K = mxGetScalar(prhs[2]);
	double* FZlookupD = mxGetPr(prhs[3]);

	int i;

	int8_t* u = (int8_t*)malloc(sizeof(int8_t)*K);
	for (i = 0; i < K; i++)
		u[i] = uD[i];

	int8_t* FZlookup = (int8_t*)malloc(sizeof(int8_t)*N);
	for (i = 0; i < N; i++)
		FZlookup[i] = FZlookupD[i];

	int8_t* x = PCencode_sys(u, N, K, FZlookup);

	plhs[0] = mxCreateDoubleMatrix(N, 1, mxREAL);
	double* output = mxGetPr(plhs[0]);

	for (i = 0; i < N; i++)
		output[i] = x[i];

	free(u);
	free(FZlookup);
	free(x);

}