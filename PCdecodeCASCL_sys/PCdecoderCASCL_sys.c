/********************************************************
 * File:		PCdecoderSCL.c							*
 * Brief:		PolarCodesSCL������Դ�ļ�				*
 * Author:		SherlockHsu								*
 * Version:		3.2.0									*
 * Date:		2017/11/25								*
 ********************************************************/

#include <math.h>
#include "PCdecoderCASCL_sys.h"
#include "PCencode_uN.h"
#define L_max 64

 /* ���N������ĵ�LС���� */
double getArrayL(double* array, int N, int L);

/* ���N����������� */
void getOrder(int8_t* order, double* array, int N);

void getOrderRev(int8_t* orderRev, int8_t* order, int N);

/* CRCУ�麯�� */
int CRCtest(int8_t* info, int K, int8_t* g, int Lcrc);

/* Polar��CA-SCl���뺯�� */
int8_t * PCdecoderCASCL_sys(double * initial_LLRs, PCparams * pc)
{
	int N = pc->N;									// �볤
	int K = pc->K;									// ��Ϣλ��
	int L = pc->L;									// ���·������
	double** LLR = pc->LLR;							// ������Ȼ��Ԥ���ռ�
	int8_t* FZlookup = pc->FZlookup;				// ����λ��ѯ�б�
	double* PM = pc->PM;							// ·������ֵ
	int8_t** d_hat = pc->d_hat;						// ��·��������
	int8_t* d_hat_sys;
	int8_t* LFlag = pc->LFlag;						// ·��״̬��־����
	int8_t* order = pc->order;						// ·������ֵ������

	int8_t* d_hat_tem = (int8_t*)malloc(sizeof(int8_t)*N);

	int8_t* info =
		(int8_t*)malloc(sizeof(int8_t)*K);			// ������
	int8_t d_temp;									// ��ǰ������
	int i, l, r, k, ldup;
	int N_path;										// ·������
	double tau;										// ·������ֵ����
	double* PM_temp =
		(double*)malloc(sizeof(double)*(2 * L));	// ��ʱ·������ֵ��������
	int8_t* ordertem = (int8_t*)malloc(sizeof(int8_t)*(2 * L));
	int8_t* orderRevtem = (int8_t*)malloc(sizeof(int8_t)*(2 * L));

	// ������Ȼ�ȳ�ʼ��
	for (l = 0; l < L; l++)
		for (i = 0; i < N; i++)
			LLR[l][N - 1 + i] = initial_LLRs[i];

	// ��ѭ��
	N_path = 1;
	PM[0] = 0;
	for (i = 0; i < N; i++)
	{
		r = pc->BitReversedList[i];					// ��ǰ�������
		if (FZlookup[r] != -1)
		{
			// ����λ����ѭ��
			for (l = 0; l < N_path; l++)
			{
				updateL(l, r, pc);
				d_temp = judge_bit(LLR[l][0]);
				PM[l] = getPM(PM[l], LLR[l][0], d_temp, FZlookup[r]);
				d_hat[l][r] = FZlookup[r];
			}
		}
		else
		{
			// ��Ϣλ����ѭ��
			for (l = 0; l < N_path; l++)
			{
				updateL(l, r, pc);
				d_temp = judge_bit(LLR[l][0]);
				PM_temp[l * 2] = getPM(PM[l], LLR[l][0], d_temp, 0);
				PM_temp[l * 2 + 1] = getPM(PM[l], LLR[l][0], d_temp, 1);
			}
			if (N_path < L)
			{
				// ����·��
				for (l = 0; l < N_path; l++)
				{
					ldup = duplicatePath(l, r, pc);
					PM[l] = PM_temp[l * 2];
					PM[ldup] = PM_temp[l * 2 + 1];
				}
				N_path = N_path * 2;
			}
			else
			{
				getOrder(ordertem, PM_temp, 2 * L);
				getOrderRev(orderRevtem, ordertem, 2 * L);
				// �жϸ�·��״̬
				for (l = 0; l < L; l++)
				{
					if (orderRevtem[l * 2] >= L&&orderRevtem[l * 2 + 1] >= L)
						LFlag[l] = 1;
					else if (orderRevtem[l * 2] < L&&orderRevtem[l * 2 + 1] >= L)
					{
						d_hat[l][r] = 0;
						PM[l] = PM_temp[l * 2];
					}
					else if (orderRevtem[l * 2] >= L&&orderRevtem[l * 2 + 1] < L)
					{
						d_hat[l][r] = 1;
						PM[l] = PM_temp[l * 2 + 1];
					}
				}
				// ·������
				for (l = 0; l < L; l++)
					if (orderRevtem[l * 2] < L&&orderRevtem[l * 2 + 1] < L)
					{
						ldup = duplicatePath(l, r, pc);
						PM[l] = PM_temp[l * 2];
						PM[ldup] = PM_temp[l * 2 + 1];
					}
			}
		}
		// ���������о��б�
		for (l = 0; l < N_path; l++)
			updateB(l, r, pc);
	}
	getOrder(order, PM, L);
	// ��·������ֵ˳�����CRCУ��
	for (l = 0; l < L; l++)
	{
		d_hat_sys = PCencode_uN(d_hat[order[l]], N, K);
		k = 0;
		for (i = 0; i < N; i++)
			if (FZlookup[i] == -1)
			{
				info[k] = d_hat_sys[i];
				k++;
			}
		free(d_hat_sys);
		if (CRCtest(info, K, pc->gcrc, pc->Lcrc))
			return info;
	}
	// ��û������ͨ��CRCУ�飬ֱ�����PMֵ��С����
	d_hat_sys = PCencode_uN(d_hat[order[0]], N, K);
	k = 0;
	for (i = 0; i < N; i++)
		if (FZlookup[i] == -1)
		{
			info[k] = d_hat_sys[i];
			k++;
		}
	free(d_hat_sys);
	return info;
}

void getOrder(int8_t* order, double* array, int N)
{
	int i, j;
	double arrayT[L_max];
	int8_t ti;
	double td;
	for (i = 0; i < N; i++)
	{
		arrayT[i] = array[i];
		order[i] = i;
	}

	for (i = 0; i < N - 1; i++)
		for (j = 0; j < N - 1 - i; j++)
			if (arrayT[j] > arrayT[j + 1])
			{
				td = arrayT[j]; arrayT[j] = arrayT[j + 1]; arrayT[j + 1] = td;
				ti = order[j]; order[j] = order[j + 1]; order[j + 1] = ti;
			}
}

void getOrderRev(int8_t* orderRev, int8_t* order, int N)
{
	int i;
	for (i = 0; i < N; i++)
		orderRev[order[i]] = i;
}

double getArrayL(double* array, int N, int L)
{
	int8_t order[L_max * 2];
	getOrder(order, array, N);
	return array[order[L - 1]];
}

int CRCtest(int8_t* info, int K, int8_t* g, int Lcrc)
{
	int i, j, tem;
	int8_t* reg = (int8_t*)malloc(sizeof(int8_t)*Lcrc);
	for (i = 0; i < Lcrc; i++)
		reg[i] = info[i];
	for (i = Lcrc; i < K; i++)
	{
		tem = reg[0];
		for (j = 0; j < Lcrc - 1; j++)
			reg[j] = reg[j + 1];
		reg[Lcrc - 1] = info[i];
		if (tem)
			for (j = 0; j < Lcrc; j++)
				reg[j] = (reg[j] + g[j]) & 1;
	}
	for (i = 0; i < Lcrc; i++)
		if (reg[i])
			return 0;
	return 1;
}