#ifndef	__VFCODING_H__
#define __VFCODING_H__

#include "StdAfx.h"

//
//	EncodeFeature: ��ָ����������ѹ������
//
int	EncodeFeature(FEATUREPTR lpFeature, char *lpData)
{
/////////////////////////////////////////////////////////////////////
//	lpFeature: [in] Ҫ�����ָ������ָ��
//  lpData: [out] ѹ��������ָ������������ָ��
/////////////////////////////////////////////////////////////////////
	int	i;
	int	totalLen = 0;
	int	len = 0;
	int	x, y, o;
	char	v, t, mn;
	int  boSize = 0;

	if(lpData == NULL || lpFeature == NULL)
		return 0;
	
	// ��������Ŀ
	mn = (char)lpFeature->MinutiaNum;
	// �����ܳ���
	totalLen = 4 + 7*mn;
	// ��һ���ֽ�Ϊ�����ܳ���
	lpData[0] = (char)(totalLen / 256);
	lpData[1] = (char)(totalLen-lpData[0]*256);

	// �ڶ����ֽ�Ϊ��������Ŀ
	lpData[2] = mn;

	len = 3;
	
	// ��ÿ�����������ѹ��
	for(i = 0; i < mn; i++)
	{
		x = lpFeature->MinutiaArr[i].x;		// 10λ��ʾ
		y = lpFeature->MinutiaArr[i].y;		// 10λ��ʾ
		o = lpFeature->MinutiaArr[i].Direction; // 9λ��ʾ
		t = (char)lpFeature->MinutiaArr[i].Type;	// 3λ��ʾ
		lpData[len] = (char)(x >> 2);
		lpData[len+1] = (char)((x & 0x3) << 6) + (char)(y >> 4);
		lpData[len+2] = (char)((y & 0xF) << 4) + (char)(o >> 5);
		lpData[len+3] = (char)((o & 0x1F) << 3) + (char)(t & 0x7);
		lpData[len+4] = (char)lpFeature->MinutiaArr[i].Triangle[0];
		lpData[len+5] = (char)lpFeature->MinutiaArr[i].Triangle[1];
		lpData[len+6] = (char)lpFeature->MinutiaArr[i].Triangle[2];

		len += 7;
	}
	
	// ����У��λ
	v = 0;
	for(i = 0; i < totalLen-1; i++)
	{
		v = v ^ lpData[i];
	}
	// ���һ���ֽڷ�У���ֽ�
	lpData[len] = v;


	return totalLen;
}

//
//	DecodeFeature: ��ָ���������н���
//
int	DecodeFeature(char *lpData, FEATUREPTR lpFeature)
{
/////////////////////////////////////////////////////////////////////
//  lpData: [in] ѹ�������ָ������������ָ��
//	lpFeature: [out] ������ָ������ָ��
/////////////////////////////////////////////////////////////////////
	int	i;
	int	totalLen = 0;
	int	len = 0;
	char	v;
	int	boSize = 0;
	if(lpData == NULL || lpFeature == NULL)
		return 1;

	memset((void *)lpFeature, 0, sizeof(FEATURE));
	// ��һ���ֽ�Ϊ�����ܳ�
	totalLen = (int)(lpData[0]*256 + lpData[1]);
	// ����У��λ
	v = 0;
	for(i = 0; i < totalLen; i++)
	{
		v = v ^ lpData[i];
	}
	if(v != 0)	// У��ʧ���򷵻ش���
		return 2;

	// �ڶ����ֽ�Ϊָ����������Ŀ
	lpFeature->MinutiaNum = (char)lpData[2]; 
	
	len = 3;

	// ����
	for(i = 0; i < lpFeature->MinutiaNum; i++)
	{
		lpFeature->MinutiaArr[i].x = (int)(((int)lpData[len])<<2) + 
								   (int)(((int)(lpData[len+1] & 0xC0))>>6);
		lpFeature->MinutiaArr[i].y = (int)(((int)(lpData[len+1] & 0x3F))<<4) + 
			                       (int)(((int)(lpData[len+2] & 0xF0))>>4);
		lpFeature->MinutiaArr[i].Direction = (int)(((int)(lpData[len+2] & 0xF))<<5) + 
			                       (int)(((int)(lpData[len+3] & 0xF8))>>3);
		lpFeature->MinutiaArr[i].Type = (char)(lpData[len+3] & 0x7);
		lpFeature->MinutiaArr[i].Triangle[0] = lpData[len+4];
		lpFeature->MinutiaArr[i].Triangle[1] = lpData[len+5];
		lpFeature->MinutiaArr[i].Triangle[2] = lpData[len+6];

		len += 7;
	}

	return 0;
}


#endif
