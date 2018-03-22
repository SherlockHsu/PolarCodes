/********************************************************
* File:		PCdecoderSCL.h								*
* Brief:	PolarCodesSCL解码器头文件					*
* Author:	SherlockHsu									*
* Version:	3.2.0										*
* Date:		2017/11/25									*
********************************************************/

#ifndef PDECODE_H
#define PDECODE_H

#include <stdint.h>
#include "PCparams.h"

/* Polar码CA-SCl译码函数，
	Input: 
		initial_LLRs; 对数似然比初始向量，长为N
		pc: 译码相关参数结构体
	Output: 
		info: 译码结果，长为K
		*/
int8_t* PCdecoderCASCL_sys(double* initial_LLRs, PCparams* pc);

#endif 
