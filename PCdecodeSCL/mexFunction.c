/********************************************************
* File:	    SCL_decoder.c								*
* Brief:	����.mexw64�ļ�								*
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
	double *llr = mxGetPr(prhs[0]);								////��Ȼ�Ƚ��վ��� llr=-2*y*snr
	int N = mxGetScalar(prhs[1]);								////����������� N=2^n
	int K = mxGetScalar(prhs[2]);                               ////�����źŸ��� K=N*C
    int L = mxGetScalar(prhs[3]);                               ////��ѡ·������    
	double* FZlookupD = mxGetPr(prhs[4]);                       ////����λ��ѯ�б�
	int i;

	int8_t* FZlookup = (int8_t*)malloc(sizeof(int8_t)*N);
	for (i = 0; i < N; i++)
    {
		FZlookup[i] = FZlookupD[i];
    }
	PCparams *pc = (PCparams*)malloc(sizeof(PCparams));      	 
    iniPCparams( pc, N , K , L , FZlookup);						////��ʼ���ṹ��PCparams   polar�������������   
	
	int8_t *info = PCdecoderCASCL(llr, pc);                       ////scl ����  ����������info
    
    plhs[0] = mxCreateDoubleMatrix( K ,  1 , mxREAL);           ////��������������������
	double *u = mxGetPr(plhs[0]);
	for (i = 0; i < K; i++)
    {
		u[i] = info[i];											////�������ֵ						
    }
	freePCparams(pc);                                           ////�ͷ��ڴ�
}