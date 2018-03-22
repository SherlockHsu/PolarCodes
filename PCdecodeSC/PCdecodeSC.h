/********************************************************
 * File:	PCdecodeSC.h								*
 * Brief:	PolarCodesSC解码器头文件					*
 * Author:	SherlockHsu									*
 * Email:	SherlockHsu@foxmail.com						*
 * Version:	1.2.0										*
 * Date:	2017/11/10									*
 ********************************************************/

#ifndef PDECODE_H
#define PDECODE_H

#include <stdint.h>
#include "PCparams.h"

/* PolarCodes译码函数 */
int8_t* PCdecodeSC(double* initial_LLRs, PCparams* pc);

#endif // !PDECODE_H