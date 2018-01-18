#ifndef	__VFAPI_H__
#define __VFAPI_H__

#include "VF_Type.h"
#include "VF_Err.h"
#include "VF_Global.h"
#include "VF_FileIO.h"
#include "VF_Smooth.h"
#include "VF_Zoom.h"
#include "VF_GetOrientMap.h"
#include "VF_Divide.h"
#include "VF_Enhance.h"
#include "VF_Binary.h"
#include "VF_Thin.h"
#include "VF_GetMinutia.h"
#include "VF_Feature.h"
#include "VF_Verify.h"
#include "VF_Template.h"
#include "VF_GetSingulary.h"

//
//	VF_LoadFinger: ���ļ��ж�ȡָ������
//
VF_RETURN	VF_LoadFinger(LPCSTR lpszFileName)
{
///////////////////////////////////////////////////////////////////////
//	lpszFileName: [in] ָ��ͼ���ļ���
///////////////////////////////////////////////////////////////////////
	sint32  bufferSize = 0;
	sint32  Width = 0;
	sint32  Height = 0;
	sint32  re;

	//	��ȡ��Ҫ�ڴ�ռ��С�������ڴ�
	re = loadBitmap((char *)lpszFileName, NULL, &Width, &Height);
	if(re != 0)
	{
		return VF_LOADFINGER_ERR;
	}
	
	if(Width > MAXIMGW || Height > MAXIMGH)
	{
		return VF_LOADFINGER_ERR;
	}
	//	��ȡ���ݵ�ͼ��������
	re = loadBitmap((char *)lpszFileName, g_lpOrgFinger, &Width, &Height);
	if(re != 0)
	{
		return VF_LOADFINGER_ERR;
	}
	
	// ���ô����ͼ���С
	IMGW = Width;
	IMGH = Height;
	IMGSIZE = (sint32)(IMGW * IMGH);

	return	VF_OK;
}

//
//	VF_SaveFinger: ����ָ������ΪBMP�ļ�
//
VF_RETURN	VF_SaveFinger(LPCSTR lpszFileName)
{
///////////////////////////////////////////////////////////////////////
//	lpszFileName: [in] ָ��ͼ���ļ���
///////////////////////////////////////////////////////////////////////

	sint32	re = 0;

	if(g_lpOrgFinger == NULL || IMGW <= 0 || IMGH <= 0)
	{
		return VF_SAVEFINGER_ERR;
	}
	// ��ͼ�������������ݱ����BMP�ļ�
	re = saveBitmap(g_lpOrgFinger, IMGW, IMGH, (char *)lpszFileName);

	if(re != 0)
	{
		return VF_SAVEFINGER_ERR;
	}

	return	VF_OK;
}
//
// VF_SaveOrientMap: �����򳡱����BMP�ļ�
//
VF_RETURN	VF_SaveOrientMap(LPCSTR lpszFileName)
{
///////////////////////////////////////////////////////////////////////
//	lpszFileName: [in] ����ͼ���ļ���
///////////////////////////////////////////////////////////////////////

	sint32	re = 0;

	if(g_lpOrient == NULL || IMGW <= 0 || IMGH <= 0)
	{
		return VF_SAVEFINGER_ERR;
	}
	// ���������ݱ���ΪBMPͼ���ļ�
	re = saveBitmap(g_lpOrient, IMGW, IMGH, (char *)lpszFileName);

	if(re != 0)
	{
		return VF_SAVEFINGER_ERR;
	}

	return	VF_OK;
}


//
//	VF_ImportFinger: �ӻ������е���ָ������
//
VF_RETURN	VF_ImportFinger(uint8  *lpImage, sint32 Width, sint32 Height)
{
///////////////////////////////////////////////////////////////////////
//	lpImage: [in] ָ��ͼ�����ݻ�����
//  Width:	[in] ָ��ͼ����
//  Height:	[in] ָ��ͼ��߶�
///////////////////////////////////////////////////////////////////////

	if(Width > MAXIMGW || Height > MAXIMGH)
	{
		return VF_IMPORTFINGER_ERR;
	}
	// ���ô����ͼ���С
	IMGW = Width;
	IMGH = Height;
	IMGSIZE = (sint32)(IMGW * IMGH);
	// ��ͼ�����ݿ�����ͼ��������
	memcpy((void *)g_lpOrgFinger, (void *)lpImage, IMGSIZE);

	return	VF_OK;
}

//
//	VF_FeatureEncode: ��ָ����������ѹ������
//
VF_RETURN VF_FeatureEncode(FEATUREPTR lpFeature, uint8 *lpData, sint32 *lpSize)
{
///////////////////////////////////////////////////////////////////////
//	lpFeature: [in] Ҫ�����ָ������ָ��
//  lpData:	[out] ������ָ���������ݻ�����ָ��
//  lpSize:	[out] ������ָ��������С
///////////////////////////////////////////////////////////////////////

	sint32 re;

	// ѹ������
	re = EncodeFeature(lpFeature, lpData);
	if(re == 0)
		return VF_FEATUREENCODE_ERR;

	*lpSize = re;

	return VF_OK;
}

//
//	VF_FeatureDecode: ��ָ���������н���
//
VF_RETURN VF_FeatureDecode(uint8 *lpData, FEATUREPTR lpFeature)
{
///////////////////////////////////////////////////////////////////////
//  lpData:	[in] Ҫ�����ָ���������ݻ�����ָ��
//  lpFeature:	[out] ������ָ������
///////////////////////////////////////////////////////////////////////

	sint32 re;
	// ����
	re = DecodeFeature(lpData, lpFeature);

	if(re != 0)
		return VF_FEATUREDECODE_ERR;

	return VF_OK;
}

//
// ��ָ��ͼ����д�������ȡָ������
//
VF_RETURN	VF_Process()
{
	sint32	re = 0;

	// ��ָ��ͼ���������ƽ������������
	smooth(g_lpOrgFinger, g_lpTemp, 1, 1);
	memcpy(g_lpOrgFinger, g_lpTemp, IMGSIZE);
	smooth(g_lpOrgFinger, g_lpTemp, 1, 1);
	memcpy(g_lpOrgFinger, g_lpTemp, IMGSIZE);
	
	// ����ͼ���Ƶ���ּ��㷽�򳡣�����ٶ�
	zoomout();
	// ���㷽��
	getOrientMap(6);
	saveBitmap(g_lpOrient, IMGW, IMGH, "c:\\0.bmp");

	// �ָ�ǰ������
	re = divide(12, 35);
	if(re != 0)
		return VF_PROCESS_ERR;
	saveBitmap(g_lpDivide, IMGW, IMGH, "c:\\00.bmp");

	// ���ζ����˲�
	orientFilter();
//	orientFilter();
	saveBitmap(g_lpOrgFinger, IMGW, IMGH, "c:\\1.bmp");

	// ��ֵ��
	binary();
	saveBitmap(g_lpOrgFinger, IMGW, IMGH, "c:\\2.bmp");

	// ��ֵ��ͼ��ȥ����
	binaryClear();
	saveBitmap(g_lpOrgFinger, IMGW, IMGH, "c:\\3.bmp");

	// �������
	clearEdge();
	saveBitmap(g_lpOrgFinger, IMGW, IMGH, "c:\\4.bmp");

	// ϸ��
	thin();
	// ϸ��ͼ��ȥ�̰���ë��
	thinClear(12);
	saveBitmap(g_lpOrgFinger, IMGW, IMGH, "c:\\5.bmp");

	// ��ȡָ������
	re = getMinutia();
	int re2 = getSingular(g_lpOrient, IMGW, IMGH, -1);
	//int re3 = getSingular(g_lpOrient, IMGW, IMGH, 1);

	if(re != 0 && re2 != 0)
		return VF_PROCESS_ERR;

	return VF_OK;
}

//
// VF_Extract: ��ָ����������ѹ�����벢���
//
VF_RETURN VF_Extract(uint8 *lpData, sint32 *lpSize)
{
///////////////////////////////////////////////////////////////////////
//  lpData:	[out] �����ָ���������ݻ�����ָ��
//  lpSize:	[out] �����ָ��������С
///////////////////////////////////////////////////////////////////////

	VF_RETURN  re;
	// ����ȡ��ָ���������б��벢���
	re = VF_FeatureEncode(&g_Feature, lpData, lpSize);
	
	if(re != VF_OK)
		return VF_EXTRACT_ERR;

	return VF_OK;
}


//
// VF_VerifyMatch: ����ָ�������ıȶ�
//
BOOL	VF_VerifyMatch(FEATUREPTR lpFeature1, FEATUREPTR lpFeature2, 
						   PMATCHRESULT lpMatchResult, VF_FLAG matchMode)
{
///////////////////////////////////////////////////////////////////////
//  lpFeature1:	[in] ��һ��ָ������
//  lpFeature2:	[in] �ڶ���ָ������
//  lpMatchResult:	[out] �ȶԽ��
//  matchMode:	[in] �ȶ�ģʽ
///////////////////////////////////////////////////////////////////////

	// �ȶ�
	verify(lpFeature1, lpFeature2, lpMatchResult, matchMode);
	
	// Ĭ����ֵΪ50�����ƶ�>=50����Ϊ�ȶԳɹ�������ȶ�ʧ��,
	// һ������£���35���ϼ�����Ϊ����, Ϊ��ȫ����������50-100
	if(lpMatchResult->Similarity >= 50)
		return TRUE;
	else
		return FALSE;
}



#endif
