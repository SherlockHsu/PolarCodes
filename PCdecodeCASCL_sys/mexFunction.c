/********************************************************
 * File:	    SCL_decoder.c							*
 * Brief:		生成.mexw64文件							*
 * Author:		SherlockHsu								*
 * Version:		3.2.0									*
 * Date:	    2017/11/25							    *
 ********************************************************/

#include <mex.h>
#include <stdlib.h>

#include "PCdecoderCASCL_sys.h"
#include "PCparams.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	double *llr = mxGetPr(prhs[0]);								// 似然比接收矩阵 llr=-2*y*snr
	int N = mxGetScalar(prhs[1]);								// 接受译码个数 N=2^n
	int K = mxGetScalar(prhs[2]);                               // 接收信号个数 K=N*C
	int L = mxGetScalar(prhs[3]);                               // 候选路径个数    
	double* FZlookupD = mxGetPr(prhs[4]);                       // 冻结位查询列表
	double* gcrcD = mxGetPr(prhs[5]);							// CRC生成多项式数组
	int Lcrc = mxGetM(prhs[5])*mxGetN(prhs[5]);					// CRC长度

	int i;

	// 数据类型转换
	int8_t* FZlookup = (int8_t*)malloc(sizeof(int8_t)*N);
	for (i = 0; i < N; i++)
		FZlookup[i] = FZlookupD[i];
	int8_t* gcrc = (int8_t*)malloc(sizeof(int8_t)*Lcrc);
	for (i = 0; i < Lcrc; i++)
		gcrc[i] = gcrcD[i];
	PCparams *pc = (PCparams*)malloc(sizeof(PCparams));

	// 初始化结构体PCparams
	iniPCparams(pc, N, K, L, Lcrc, FZlookup, gcrc);

	// CA-SCL译码
	int8_t *info = PCdecoderCASCL_sys(llr, pc);
	plhs[0] = mxCreateDoubleMatrix(K, 1, mxREAL);
	double *u = mxGetPr(plhs[0]);
	for (i = 0; i < K; i++)
		u[i] = info[i];

	// 释放内存
	freePCparams(pc);
}