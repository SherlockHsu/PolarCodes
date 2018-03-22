/********************************************************
 * File:	    SCL_decoder.c							*
 * Brief:		����.mexw64�ļ�							*
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
	double *llr = mxGetPr(prhs[0]);								// ��Ȼ�Ƚ��վ��� llr=-2*y*snr
	int N = mxGetScalar(prhs[1]);								// ����������� N=2^n
	int K = mxGetScalar(prhs[2]);                               // �����źŸ��� K=N*C
	int L = mxGetScalar(prhs[3]);                               // ��ѡ·������    
	double* FZlookupD = mxGetPr(prhs[4]);                       // ����λ��ѯ�б�
	double* gcrcD = mxGetPr(prhs[5]);							// CRC���ɶ���ʽ����
	int Lcrc = mxGetM(prhs[5])*mxGetN(prhs[5]);					// CRC����

	int i;

	// ��������ת��
	int8_t* FZlookup = (int8_t*)malloc(sizeof(int8_t)*N);
	for (i = 0; i < N; i++)
		FZlookup[i] = FZlookupD[i];
	int8_t* gcrc = (int8_t*)malloc(sizeof(int8_t)*Lcrc);
	for (i = 0; i < Lcrc; i++)
		gcrc[i] = gcrcD[i];
	PCparams *pc = (PCparams*)malloc(sizeof(PCparams));

	// ��ʼ���ṹ��PCparams
	iniPCparams(pc, N, K, L, Lcrc, FZlookup, gcrc);

	// CA-SCL����
	int8_t *info = PCdecoderCASCL_sys(llr, pc);
	plhs[0] = mxCreateDoubleMatrix(K, 1, mxREAL);
	double *u = mxGetPr(plhs[0]);
	for (i = 0; i < K; i++)
		u[i] = info[i];

	// �ͷ��ڴ�
	freePCparams(pc);
}