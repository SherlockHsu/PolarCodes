/********************************************************
 * File:	PCencode.h									*
 * Brief:	PolarCodes������ͷ�ļ�						*
 * Author:	SherlockHsu									*
 * Email:	SherlockHsu@foxmail.com						*
 * Version:	1.2.0										*
 * Date:	2017/11/10									*
 ********************************************************/

#ifndef PCENCODE_H
#define PCENCODE_H

#include <stdint.h>

int8_t * PCencode(int8_t * u, int N, int K, int8_t* FZlookup);

#endif // !PCENCODE_H

