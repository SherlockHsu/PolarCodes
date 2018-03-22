/********************************************************
 * File:	logdomain_tool.c							*
 * Brief:	对数域快速计算工具源文件					*
 * Author:	SherlockHsu									*
 * Version:	1.2.0										*
 * Date:	2017/11/10									*
 ********************************************************/

#include "logdomain_tool.h"

#include <math.h>

 /* 对数域求和函数 */
double logdomain_sum(double x, double y)
{
	if (x < y)
		return y + log1p(exp(x - y));
	else
		return x + log1p(exp(y - x));
}

/* 对数域求差函数 */
double logdomain_diff(double x, double y)
{
	return x + log1p(-exp(y - x));
}