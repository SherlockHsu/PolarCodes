/********************************************************
* File:		PCdecoderSCL.h								*
* Brief:	PolarCodesSCL������ͷ�ļ�					*
* Author:	Let it go									*
* Version:	2.2.2										*
* Date:		2017/11/25									*
********************************************************/

#ifndef PDECODE_H
#define PDECODE_H

#include <stdint.h>
#include "PCparams.h"

/*Polarcode SCl���뺯�������룺initial_LLRs������Ȼ�ȳ�ʼ��������N��pc������Ϣ��Žṹ�壬�����info����ṹ����K*/
int8_t* PCdecoderCASCL(double* initial_LLRs, PCparams* pc);

#endif 
