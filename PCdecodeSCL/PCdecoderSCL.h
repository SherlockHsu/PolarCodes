/********************************************************
* File:		PCdecoderSCL.h								*
* Brief:	PolarCodesSCL解码器头文件					*
* Author:	Let it go									*
* Version:	2.2.2										*
* Date:		2017/11/25									*
********************************************************/

#ifndef PDECODE_H
#define PDECODE_H

#include <stdint.h>
#include "PCparams.h"

/*Polarcode SCl译码函数，输入：initial_LLRs对数似然比初始向量长度N、pc码字信息存放结构体，输出：info译码结构长度K*/
int8_t* PCdecoderCASCL(double* initial_LLRs, PCparams* pc);

#endif 
