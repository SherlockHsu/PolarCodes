/********************************************************
 * File:	mexFunction.c								*
 * Brief:	生成.mexw64文件								*
 * Author:	SherlockHsu									*
 * Email:	SherlockHsu@foxmail.com						*
 * Version:	1.2.0										*
 * Date:	2017/11/10									*
 ********************************************************/

#include <mex.h>
#include "getFZlookup.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	int N = mxGetScalar(prhs[0]);
	int K = mxGetScalar(prhs[1]);
	double SNRdB = mxGetScalar(prhs[2]);
	int8_t frozenbit = mxGetScalar(prhs[3]);

	int8_t* FZlookup = getFZlookup(N, K, SNRdB, frozenbit);

	plhs[0] = mxCreateDoubleMatrix(N, 1, mxREAL);
	double* output = mxGetPr(plhs[0]);
	int i;
	for (i = 0; i < N; i++)
		output[i] = FZlookup[i];

	free(FZlookup);
}