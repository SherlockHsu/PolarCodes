/********************************************************
* File:    PCparams.h									*
* Brief:	PolarCodes��������غ���ͷ�ļ�				*
* Author:	SherlockHsu									*
* Email:	SherlockHsu@foxmail.com						*
* Version:	1.2.0										*
* Date:	2017/11/10									*
********************************************************/

#ifndef PCPARAMS_H
#define PCPARAMS_H

#include <stdint.h>
#include <stdlib.h>

/* ����PolarCodes��ز����Ľṹ�� */
typedef struct {
	int N;								// �볤
	int K;								// ��Ϣλ����
	int L;								// ·������
	int n;								// log2(N)
	int8_t* FZlookup;					// ����λ��ѯ�б�
	double** LLR;						// ������Ȼ��Ԥ���ռ�
	int8_t** BIT1;						// �����о�Ԥ���ռ�			
	int8_t** BIT2;
	int8_t** d_hat;
	double* PM;							// ·������ֵ�洢�ռ�
	int8_t* LFlag;
	int8_t* order;
	int* BitReversedList;				// �о�˳���ѯ�б�
	int8_t* getLLRlevelList;
	int8_t* getBITlevelList;
}PCparams;

/* PCparams�ṹ���ʼ������(��֪FZlookup) */
void iniPCparams(PCparams *pc, int N, int K, int L, int8_t* FZlookup);

/* PCparams�ṹ���ڴ��ͷ� */
void freePCparams(PCparams* pc);

/* ������Ȼ���б���� */
void updateL(int l, int indx, PCparams* pc);

/* �����о��б���� */
void updateB(int l, int indx, PCparams * pc);

/*·������ֵ���㺯��,���룺PM_last��ǰ·������ֵ��LLn_now��ǰ������Ȼ��ֵ��u_judge�жϱ��ء�u_now��ǰ����*/
double getPM(double PM_last, double LLn_now, int8_t u_judge, int8_t u_now);

/*bit�жϺ��������룺LLRi������Ȼ��ֵ,�����bit�жϽ��*/
int8_t judge_bit(double LLRi);

int duplicatePath(int indx, int r, PCparams* pc);

#endif