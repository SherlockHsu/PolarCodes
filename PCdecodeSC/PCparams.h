/********************************************************
 * File:    PCparams.h									*
 * Brief:	PolarCodes参数及相关函数头文件				*
 * Author:	SherlockHsu									*
 * Email:	SherlockHsu@foxmail.com						*
 * Version:	1.2.0										*
 * Date:	2017/11/10									*
 ********************************************************/

#ifndef PCPARAMS_H
#define PCPARAMS_H

#include <stdint.h>
#include <stdlib.h>

 /* 包含PolarCodes相关参数的结构体 */
typedef struct {
	int N;								// 码长
	int K;								// 信息位长度
	int n;								// log2(N)
	int8_t* FZlookup;					// 冻结位查询列表
	double** LLR;						// 对数似然比预留空间
	int8_t** BIT;						// 码字判决预留空间					
	int* BitReversedList;				// 判决顺序查询列表
}PCparams;

///* PCparams结构体初始化（未知FZlookup） */
//void iniPCparams(PCparams* pc, int N, int K, double SNRdB);

/* PCparams结构体初始化函数（已知FZlookup） */
void iniPCparams(PCparams *pc, int N, int K, int8_t* FZlookup);

/* PCparams结构体内存释放 */
void freePCparams(PCparams* pc);

/* 对数似然比列表更新 */
void updateL(int i, int j, PCparams* pc);

/* 码字判决列表更新 */
void updateB(int i, int j, PCparams* pc);

#endif