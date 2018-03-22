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
	int n;								// log2(N)
	int8_t* FZlookup;					// ����λ��ѯ�б�
	double** LLR;						// ������Ȼ��Ԥ���ռ�
	int8_t** BIT;						// �����о�Ԥ���ռ�					
	int* BitReversedList;				// �о�˳���ѯ�б�
}PCparams;

///* PCparams�ṹ���ʼ����δ֪FZlookup�� */
//void iniPCparams(PCparams* pc, int N, int K, double SNRdB);

/* PCparams�ṹ���ʼ����������֪FZlookup�� */
void iniPCparams(PCparams *pc, int N, int K, int8_t* FZlookup);

/* PCparams�ṹ���ڴ��ͷ� */
void freePCparams(PCparams* pc);

/* ������Ȼ���б���� */
void updateL(int i, int j, PCparams* pc);

/* �����о��б���� */
void updateB(int i, int j, PCparams* pc);

#endif