/********************************************************
 * File:	getFZlookup.h								*
 * Brief:	����λ��ѯ�������ɺ���ͷ�ļ�				*
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
