/********************************************************
* File:		PCencode_sys.h								*
* Brief:	系统Polar码编码头文件						*
* Author:	SherlockHsu									*
* Email:	SherlockHsu@foxmail.com						*
* Date:		2018/03/01									*
********************************************************/

#ifndef PCENCODE_SYS_H
#define PCENCODE_SYS_H

#include <stdint.h>

int8_t * PCencode_sys(int8_t * u, int N, int K, int8_t* FZlookup);

#endif