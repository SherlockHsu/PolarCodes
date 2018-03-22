/********************************************************
 * File:	PCdecodeSC.h								*
 * Brief:	PolarCodesSC������ͷ�ļ�					*
 * Author:	SherlockHsu									*
 * Email:	SherlockHsu@foxmail.com						*
 * Version:	1.2.0										*
 * Date:	2017/11/10									*
 ********************************************************/

#ifndef PDECODE_H
#define PDECODE_H

#include <stdint.h>
#include "PCparams.h"

/* PolarCodes���뺯�� */
int8_t* PCdecodeSC(double* initial_LLRs, PCparams* pc);

#endif // !PDECODE_H