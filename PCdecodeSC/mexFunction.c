/********************************************************
 * File:	mexFunction.c								*
 * Brief:	生成.mexw64文件								*
 * Author:	SherlockHsu									*
 * Email:	SherlockHsu@foxmail.com						*
 * Version:	1.2.0										*
 * Date:	2017/11/10									*
 ********************************************************/

#include <mex.h>
#include <stdlib.h>

#include "PCdecodeSC.h"
#include "PCparams.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	double *llr = mxGetPr(prhs[0]);
	int N = mxGetScalar(prhs[1]);
	int K = mxGetScalar(prhs[2]);
	double* FZlookupD = mxGetPr(prhs[3]);
	int i;

	int8_t* FZlookup = (int8_t*)malloc(sizeof(int8_t)*N);
	for (i = 0; i < N; i++)
		FZlookup[i] = FZlookupD[i];

	// 初始化结构体PCparams
	PCparams *pc = (PCparams*)malloc(sizeof(PCparams));
	iniPCparams(pc, N, K, FZlookup);

	// SC译码
	int8_t *uint = PCdecodeSC(llr, pc);

	freePCparams(pc);

	plhs[0] = mxCreateDoubleMatrix(K, 1, mxREAL);
	double *u = mxGetPr(plhs[0]);
	for (i = 0; i < K; i++)
		u[i] = uint[i];
}
