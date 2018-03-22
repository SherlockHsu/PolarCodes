/********************************************************
 * File:    PCparams.h									*
 * Brief:	PolarCodes参数及相关函数头文件				*
 * Author:	SherlockHsu									*
 * Email:	SherlockHsu@foxmail.com						*
 * Version:	3.2.0										*
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
	int L;								// 最大路径个数
	int n;								// log2(N)
	int Lcrc;							// CRC生成码码长
	int8_t* FZlookup;					// 冻结位查询列表
	double** LLR;						// 对数似然比预留空间
	int8_t** BIT1;						// 码字判决预留空间			
	int8_t** BIT2;						// 码字判决临时预留空间
	int8_t** d_hat;						// 码字判决结果
	double* PM;							// 路径测量值存储空间
	int8_t* LFlag;						// 路径状态标志数组
	int8_t* order;						// 路径测量值顺序数组
	int* BitReversedList;				// 判决顺序查询列表
	int8_t* getLLRlevelList;			// 对数似然比判决相关列表
	int8_t* getBITlevelList;			// 码字判决相关列表
	int8_t* gcrc;						// CRC生成码
}PCparams;

/* PCparams结构体初始化函数(已知FZlookup) */
void iniPCparams(PCparams *pc, int N, int K, int L, int Lcrc, int8_t* FZlookup, int8_t* gcrc);

/* PCparams结构体内存释放 */
void freePCparams(PCparams* pc);

/* 对数似然比列表更新 */
void updateL(int l, int indx, PCparams* pc);

/* 码字判决列表更新 */
void updateB(int l, int indx, PCparams * pc);

/* 路径测量值计算函数,输入：PM_last以前路径测量值、LLn_now当前对数似然比值、u_judge判断比特、u_now当前比特 */
double getPM(double PM_last, double LLn_now, int8_t u_judge, int8_t u_now);

/* bit判断函数，输入：LLRi对数似然比值,输出：bit判断结果 */
int8_t judge_bit(double LLRi);

/* 路径复制函数 */
int duplicatePath(int indx, int r, PCparams* pc);

#endif