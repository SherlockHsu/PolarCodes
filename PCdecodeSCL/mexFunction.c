/********************************************************
* File:	    SCL_decoder.c								*
* Brief:	生成.mexw64文件								*
* Author:	Let it go									*
* Version:	2.2.2										*
* Date:	    2017/11/25								    *
********************************************************/

#include <mex.h>
#include <stdlib.h>

#include "PCdecoderSCL.h"
#include "PCparams.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	double *llr = mxGetPr(prhs[0]);								////似然比接收矩阵 llr=-2*y*snr
	int N = mxGetScalar(prhs[1]);								////接受译码个数 N=2^n
	int K = mxGetScalar(prhs[2]);                               ////接收信号个数 K=N*C
    int L = mxGetScalar(prhs[3]);                               ////候选路径个数    
	double* FZlookupD = mxGetPr(prhs[4]);                       ////冻结位查询列表
	int i;

	int8_t* FZlookup = (int8_t*)malloc(sizeof(int8_t)*N);
	for (i = 0; i < N; i++)
    {
		FZlookup[i] = FZlookupD[i];
    }
	PCparams *pc = (PCparams*)malloc(sizeof(PCparams));      	 
    iniPCparams( pc, N , K , L , FZlookup);						////初始化结构体PCparams   polar参数与译码参数   
	
	int8_t *info = PCdecoderCASCL(llr, pc);                       ////scl 译码  译码结果存入info
    
    plhs[0] = mxCreateDoubleMatrix( K ,  1 , mxREAL);           ////创建输出矩阵，输出译码结果
	double *u = mxGetPr(plhs[0]);
	for (i = 0; i < K; i++)
    {
		u[i] = info[i];											////输出矩阵赋值						
    }
	freePCparams(pc);                                           ////释放内存
}