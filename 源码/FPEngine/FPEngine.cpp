// FPEngine.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "FPEngine.h"
#include "VF_Api.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}


// This is an example of an exported variable
FPENGINE_API int nFPEngine=0;

// This is an example of an exported function.
FPENGINE_API int fnFPEngine(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see FPEngine.h for the class definition
CFPEngine::CFPEngine()
{ 
	return; 
}


//
// Extract: ��ָ��ͼ������ȡָ������
//

FPENGINE_API int Analyze(BYTE *lpImage, int Width, int Height, BYTE *lpFeature, int *lpSize)
{
///////////////////////////////////////////////////////////////////////
//  Width:	[in] ָ��ͼ����
//  Height:	[in] ָ��ͼ��߶�
//  lpImage:    [in] ָ��ͼ������ָ��
//  Resolution:	[in] ָ��ͼ��ֱ��ʣ�Ĭ��500
//  lpFeature:	[out] ��ȡ��ָ����������ָ��
//  lpSize:	[out] ָ���������ݴ�С
///////////////////////////////////////////////////////////////////////

	// TODO: Add your implementation code here
	VF_RETURN	re;

	// ����ָ��ͼ������
	VF_ImportFinger(lpImage, Width, Height);
	
	// ����ָ��ͼ����ȡָ������
	re = VF_Process();
	if(re != VF_OK)
		return re;
	
	// ��ָ���������б���
	re = VF_FeatureEncode(&g_Feature, lpFeature, lpSize);
	if(re != VF_OK)
		return re;

	return 0;
}

//
// ExtractEx: ��ָ��ͼ��BMP�ļ�����ȡָ������
//
FPENGINE_API int AnalyzeFromFile(LPCSTR lpszFileName, BYTE *lpFeature, int *lpSize)
{
//////////////////////////////////////////////////////////////////////////
//  lpszFileName: [in] ָ��ͼ���ļ�·��
//  Resolution: [in] ָ��ͼ��ֱ���
//  lpFeature: [out] ��ȡ��ָ���������ݻ�����ָ��
//  lpSize: [out] ��ȡ��ָ���������ݴ�С
//////////////////////////////////////////////////////////////////////////
	// TODO: Add your implementation code here
	VF_RETURN	re;
	
	// ��ȡָ��ͼ������
	re = VF_LoadFinger(lpszFileName);
	if(re != VF_OK)
		return re;

	// ����ָ��ͼ����ȡָ������
	re = VF_Process();
	if(re != VF_OK)
		return re;

	// ��ָ���������б���
	re = VF_FeatureEncode(&g_Feature, lpFeature, lpSize);
	if(re != VF_OK)
		return re;

	return 0;
}

//
// VerifyMatch: ����ָ��������ıȶ�
//
FPENGINE_API int PatternMatch(BYTE *lpFeature1, BYTE *lpFeature2, int *lpScore)
{
//////////////////////////////////////////////////////////////////////////
//	lpFeature1:		[in] ��һ��ָ������
//	lpFeature2:		[in] �ڶ���ָ������
//	lpScore:		[out] �ȶԵ����ƶ�
//	FastMode:		[in] �Ƿ���п���ģʽ�ȶ�
//////////////////////////////////////////////////////////////////////////
	// TODO: Add your implementation code here
	VF_RETURN	re;
	MATCHRESULT mr;
	FEATURE		feat1, feat2;

	// ��һ��ָ�������Ľ���
	re = VF_FeatureDecode(lpFeature1, &feat1);
	if(re != VF_OK)
		return re;
	
	// �ڶ���ָ�������Ľ���
	re = VF_FeatureDecode(lpFeature2, &feat2);
	if(re != VF_OK)
		return re;

	*lpScore = 0;

	bool FastMode = true;

	if(FastMode)
	{
		// ����ģʽ�ıȶ�
		VF_VerifyMatch(&feat1, &feat2, &mr, VF_MATCHMODE_IDENTIFY);
	}
	else
	{
		// ��ȷģʽ�ıȶ�
		VF_VerifyMatch(&feat1, &feat2, &mr, VF_MATCHMODE_VERIFY);
	}

	// ƥ������ƶ�
	if(mr.MMCount < 8)
		*lpScore = 0;
	else
		*lpScore = mr.Similarity;

	return 0;
}

//
// SaveFeature: ����ָ���������ļ���
//
FPENGINE_API int SaveFeature(LPCSTR lpszFileName, BYTE *lpFeature)
{
/////////////////////////////////////////////////////////////////////////
//	lpFeature: [in] ָ������
//  lpszFileName: [in] �ļ�·��
/////////////////////////////////////////////////////////////////////////
	// TODO: Add your implementation code here
   	FILE               *fp = NULL;
	BYTE v;
	int         i;
	
	// ��ָ����������У��
	v = 0;
	for(i = 0; i < (lpFeature[0]*256+lpFeature[1]); i++)
	{
		v = v ^ lpFeature[i];
	}
	// У��ʧ�ܷ��ش���
	if(v != 0) 
		return VF_SAVEFEATURE_ERR;
	
	// ������д��ָ����������
	fp = fopen(lpszFileName, "wb");
	if(fp == NULL)
	{
		return VF_SAVEFEATURE_ERR;
	}
	fwrite((char *)lpFeature, lpFeature[0]*256+lpFeature[1], 1, fp);
	fclose(fp);	

	return 0;
}

//
// LoadFeature: ��ָ���������ļ��ж�ȡָ������
//
FPENGINE_API int LoadFeature(LPCSTR lpszFileName, BYTE *lpFeature, int *lpSize)
{
/////////////////////////////////////////////////////////////////////////
//  lpszFileName: [in] ָ�������ļ�·��
//	lpFeature: [out] ָ������
//  lpSize: [out] ָ��������С
/////////////////////////////////////////////////////////////////////////

	// TODO: Add your implementation code here
   	FILE               *fp = NULL;
	BYTE v;
	int         i;

	fp = fopen(lpszFileName, "rb");
	if(fp == NULL)
	{
		return VF_LOADFEATURE_ERR;
	}
	// ��ȡָ��������С
	fread((void *)&lpFeature[0], 2, 1, fp);
	// ��ȡָ�����������ֶ�����
	fread((void *)&lpFeature[2], lpFeature[0]*256+lpFeature[1]-1, 1, fp);

	fclose(fp);	

	// У��
	v = 0;
	for(i = 0; i < (lpFeature[0]*256+lpFeature[1]); i++)
	{
		v = v ^ lpFeature[i];
	}
	// У��ʧ�ܷ��ش���
	if(v != 0)
		return VF_LOADFEATURE_ERR;


	return 0;
}

//
// LoadFinger: ��ָ��ͼ���ļ��ж�ȡָ��ͼ������
//
FPENGINE_API int LoadFingerImage(LPCSTR lpszFileName, BYTE *lpFinger, int *lpWidth, int *lpHeight)
{
/////////////////////////////////////////////////////////////////////////
//  lpszFileName: [in] ָ��ͼ���ļ�·��
//	lpFinger: [out] ָ��ͼ���������
//  lpWidth: [out] ͼ����ָ��
//  lpHeight: [out] ͼ��߶�ָ��
/////////////////////////////////////////////////////////////////////////

	// TODO: Add your implementation code here
	int	re;

	// ��ȡͼ�����ݲ��õ�ͼ���С
	re = loadBitmap((char *)lpszFileName, lpFinger, lpWidth, lpHeight);
	if(re != 0)
		return VF_LOADFINGER_ERR;

	return 0;
}

//
// SaveFinger: ��ָ��ͼ�����ݱ���ΪBMP�ļ�
//
FPENGINE_API int SaveFingerImage(LPCSTR lpszFileName, BYTE *lpFinger, int Width, int Height)
{
/////////////////////////////////////////////////////////////////////////
//	lpFinger: [in] ָ��ͼ���������
//  Width: [in] ͼ����
//  Height: [in] ͼ��߶�
//  lpszFileName: [in] ָ��ͼ���ļ�·��
/////////////////////////////////////////////////////////////////////////

	// TODO: Add your implementation code here
	int	re;

	// ����ָ��ͼ������ΪBMP�ļ�
	try {
		re = saveBitmap(lpFinger, Width, Height, (char *)lpszFileName);
	} catch (char *str) {		
		FILE	*fp = NULL;
		fp = fopen("d:\\err.txt", "wb");
		fwrite(str, strlen(str), 1, fp);
		fclose(fp);	
	}
	if(re != 0)
		return VF_SAVEFINGER_ERR;


	return 0;
}

//
// GetQuality: ��ȡָ��ͼ����������(100������)
//
FPENGINE_API int GetImageQuality(BYTE *lpFinger, int Width, int Height, int *pScore)
{
/////////////////////////////////////////////////////////////////////////
//	lpFinger: [in] ָ��ͼ���������
//  Width: [in] ͼ����
//  Height: [in] ͼ��߶�
//  pScore: [out] ��������ָ��
/////////////////////////////////////////////////////////////////////////

	// TODO: Add your implementation code here
	VF_RETURN	re;
	sint32	x, y, num;
	sint32  temp,score;

	// ����ָ��ͼ������
	re = VF_ImportFinger(lpFinger, Width, Height);
	
	if(re != VF_OK)
		return re;
	
	// ��ָ��ͼ����ж���ƽ��
	smooth(g_lpOrgFinger, g_lpTemp, 1, 1);
	smooth(g_lpOrgFinger, g_lpTemp, 1, 1);
	memcpy(g_lpOrgFinger, g_lpTemp, IMGSIZE);
	// ����ͼ���Ƶ���ּ��㷽�򳡣�����ٶ�
	zoomout();
	// ����ָ��ͼ��ķ��򳡣��õ���ֵ
	getOrientMap(6);
	// �ָ�ָ��ͼ��
	divide(12, 32);

	//ͳ��ǰ�������С
	temp = 0;
	num = 0;
	for(y = 0; y < IMGH; y++)
	{
		for(x = 0; x < IMGW; x++)
		{
			if(*(g_lpDivide + temp + x) != 255)
			{
				num++;
			}
		}
		temp += IMGW;
	}
	
	// ������������
	score = (int)(100*num/(0.8*IMGSIZE)+0.5);
	if(score > 100)
		score = 100;
	*pScore = score;

	return 0;
}