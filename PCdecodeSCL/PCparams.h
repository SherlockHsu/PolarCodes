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
	int L;								// 路径个数
	int n;								// log2(N)
	int8_t* FZlookup;					// 冻结位查询列表
	double** LLR;						// 对数似然比预留空间
	int8_t** BIT1;						// 码字判决预留空间			
	int8_t** BIT2;
	int8_t** d_hat;
	double* PM;							// 路径测量值存储空间
	int8_t* LFlag;
	int8_t* order;
	int* BitReversedList;				// 判决顺序查询列表
	int8_t* getLLRlevelList;
	int8_t* getBITlevelList;
}PCparams;

/* PCparams结构体初始化函数(已知FZlookup) */
void iniPCparams(PCparams *pc, int N, int K, int L, int8_t* FZlookup);

/* PCparams结构体内存释放 */
void freePCparams(PCparams* pc);

/* 对数似然比列表更新 */
void updateL(int l, int indx, PCparams* pc);

/* 码字判决列表更新 */
void updateB(int l, int indx, PCparams * pc);

/*路径测量值计算函数,输入：PM_last以前路径测量值、LLn_now当前对数似然比值、u_judge判断比特、u_now当前比特*/
double getPM(double PM_last, double LLn_now, int8_t u_judge, int8_t u_now);

/*bit判断函数，输入：LLRi对数似然比值,输出：bit判断结果*/
int8_t judge_bit(double LLRi);

int duplicatePath(int indx, int r, PCparams* pc);

#endif