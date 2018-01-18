#ifndef	__VFCODING_H__
#define __VFCODING_H__

#include "StdAfx.h"

//
//	EncodeFeature: 对指纹特征进行压缩编码
//
int	EncodeFeature(FEATUREPTR lpFeature, char *lpData)
{
/////////////////////////////////////////////////////////////////////
//	lpFeature: [in] 要编码的指纹特征指针
//  lpData: [out] 压缩编码后的指纹特征缓冲区指针
/////////////////////////////////////////////////////////////////////
	int	i;
	int	totalLen = 0;
	int	len = 0;
	int	x, y, o;
	char	v, t, mn;
	int  boSize = 0;

	if(lpData == NULL || lpFeature == NULL)
		return 0;
	
	// 特征点数目
	mn = (char)lpFeature->MinutiaNum;
	// 特征总长度
	totalLen = 4 + 7*mn;
	// 第一个字节为特征总长度
	lpData[0] = (char)(totalLen / 256);
	lpData[1] = (char)(totalLen-lpData[0]*256);

	// 第二个字节为特征点数目
	lpData[2] = mn;

	len = 3;
	
	// 对每个特征点进行压缩
	for(i = 0; i < mn; i++)
	{
		x = lpFeature->MinutiaArr[i].x;		// 10位表示
		y = lpFeature->MinutiaArr[i].y;		// 10位表示
		o = lpFeature->MinutiaArr[i].Direction; // 9位表示
		t = (char)lpFeature->MinutiaArr[i].Type;	// 3位表示
		lpData[len] = (char)(x >> 2);
		lpData[len+1] = (char)((x & 0x3) << 6) + (char)(y >> 4);
		lpData[len+2] = (char)((y & 0xF) << 4) + (char)(o >> 5);
		lpData[len+3] = (char)((o & 0x1F) << 3) + (char)(t & 0x7);
		lpData[len+4] = (char)lpFeature->MinutiaArr[i].Triangle[0];
		lpData[len+5] = (char)lpFeature->MinutiaArr[i].Triangle[1];
		lpData[len+6] = (char)lpFeature->MinutiaArr[i].Triangle[2];

		len += 7;
	}
	
	// 计算校验位
	v = 0;
	for(i = 0; i < totalLen-1; i++)
	{
		v = v ^ lpData[i];
	}
	// 最后一个字节放校验字节
	lpData[len] = v;


	return totalLen;
}

//
//	DecodeFeature: 对指纹特征进行解码
//
int	DecodeFeature(char *lpData, FEATUREPTR lpFeature)
{
/////////////////////////////////////////////////////////////////////
//  lpData: [in] 压缩编码的指纹特征缓冲区指针
//	lpFeature: [out] 解码后的指纹特征指针
/////////////////////////////////////////////////////////////////////
	int	i;
	int	totalLen = 0;
	int	len = 0;
	char	v;
	int	boSize = 0;
	if(lpData == NULL || lpFeature == NULL)
		return 1;

	memset((void *)lpFeature, 0, sizeof(FEATURE));
	// 第一个字节为特征总长
	totalLen = (int)(lpData[0]*256 + lpData[1]);
	// 计算校验位
	v = 0;
	for(i = 0; i < totalLen; i++)
	{
		v = v ^ lpData[i];
	}
	if(v != 0)	// 校验失败则返回错误
		return 2;

	// 第二个字节为指纹特征点数目
	lpFeature->MinutiaNum = (char)lpData[2]; 
	
	len = 3;

	// 解码
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
