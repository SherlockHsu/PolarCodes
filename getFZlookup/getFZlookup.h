/********************************************************
 * File:	getFZlookup.h								*
 * Brief:	冻结位查询序列生成函数头文件				*
 * Author:	SherlockHsu									*
 * Email:	SherlockHsu@foxmail.com						*
 * Version:	1.2.0										*
 * Date:	2017/11/10									*
 ********************************************************/

#ifndef GETFZLOOKUP_H
#define GETFZLOOKUP_H

#include <stdint.h>

int8_t *getFZlookup(int N, int K, double SNRdB, int8_t frozenbit);

#endif // !GETFZLOOKUP_H
