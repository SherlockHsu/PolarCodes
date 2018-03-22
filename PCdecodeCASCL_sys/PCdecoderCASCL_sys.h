/********************************************************
* File:		PCdecoderSCL.h								*
* Brief:	PolarCodesSCL������ͷ�ļ�					*
* Author:	SherlockHsu									*
* Version:	3.2.0										*
* Date:		2017/11/25									*
********************************************************/

#ifndef PDECODE_H
#define PDECODE_H

#include <stdint.h>
#include "PCparams.h"

/* Polar��CA-SCl���뺯����
	Input: 
		initial_LLRs; ������Ȼ�ȳ�ʼ��������ΪN
		pc: ������ز����ṹ��
	Output: 
		info: ����������ΪK
		*/
int8_t* PCdecoderCASCL_sys(double* initial_LLRs, PCparams* pc);

#endif 
