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
// Extract: 从指纹图像中提取指纹特征
//

FPENGINE_API int Analyze(BYTE *lpImage, int Width, int Height, BYTE *lpFeature, int *lpSize)
{
///////////////////////////////////////////////////////////////////////
//  Width:	[in] 指纹图像宽度
//  Height:	[in] 指纹图像高度
//  lpImage:    [in] 指纹图像数据指针
//  Resolution:	[in] 指纹图像分辨率，默认500
//  lpFeature:	[out] 提取的指纹特征数据指针
//  lpSize:	[out] 指纹特征数据大小
///////////////////////////////////////////////////////////////////////

	// TODO: Add your implementation code here
	VF_RETURN	re;

	// 导入指纹图像数据
	VF_ImportFinger(lpImage, Width, Height);
	
	// 处理指纹图像，提取指纹特征
	re = VF_Process();
	if(re != VF_OK)
		return re;
	
	// 对指纹特征进行编码
	re = VF_FeatureEncode(&g_Feature, lpFeature, lpSize);
	if(re != VF_OK)
		return re;

	return 0;
}

//
// ExtractEx: 从指纹图像BMP文件中提取指纹特征
//
FPENGINE_API int AnalyzeFromFile(LPCSTR lpszFileName, BYTE *lpFeature, int *lpSize)
{
//////////////////////////////////////////////////////////////////////////
//  lpszFileName: [in] 指纹图像文件路径
//  Resolution: [in] 指纹图像分辨率
//  lpFeature: [out] 提取的指纹特征数据缓冲区指针
//  lpSize: [out] 提取的指纹特征数据大小
//////////////////////////////////////////////////////////////////////////
	// TODO: Add your implementation code here
	VF_RETURN	re;
	
	// 读取指纹图像数据
	re = VF_LoadFinger(lpszFileName);
	if(re != VF_OK)
		return re;

	// 处理指纹图像，提取指纹特征
	re = VF_Process();
	if(re != VF_OK)
		return re;

	// 对指纹特征进行编码
	re = VF_FeatureEncode(&g_Feature, lpFeature, lpSize);
	if(re != VF_OK)
		return re;

	return 0;
}

//
// VerifyMatch: 两个指纹特征间的比对
//
FPENGINE_API int PatternMatch(BYTE *lpFeature1, BYTE *lpFeature2, int *lpScore)
{
//////////////////////////////////////////////////////////////////////////
//	lpFeature1:		[in] 第一个指纹特征
//	lpFeature2:		[in] 第二个指纹特征
//	lpScore:		[out] 比对的相似度
//	FastMode:		[in] 是否进行快速模式比对
//////////////////////////////////////////////////////////////////////////
	// TODO: Add your implementation code here
	VF_RETURN	re;
	MATCHRESULT mr;
	FEATURE		feat1, feat2;

	// 第一个指纹特征的解码
	re = VF_FeatureDecode(lpFeature1, &feat1);
	if(re != VF_OK)
		return re;
	
	// 第二个指纹特征的解码
	re = VF_FeatureDecode(lpFeature2, &feat2);
	if(re != VF_OK)
		return re;

	*lpScore = 0;

	bool FastMode = true;

	if(FastMode)
	{
		// 快速模式的比对
		VF_VerifyMatch(&feat1, &feat2, &mr, VF_MATCHMODE_IDENTIFY);
	}
	else
	{
		// 精确模式的比对
		VF_VerifyMatch(&feat1, &feat2, &mr, VF_MATCHMODE_VERIFY);
	}

	// 匹配的相似度
	if(mr.MMCount < 8)
		*lpScore = 0;
	else
		*lpScore = mr.Similarity;

	return 0;
}

//
// SaveFeature: 保存指纹特征到文件中
//
FPENGINE_API int SaveFeature(LPCSTR lpszFileName, BYTE *lpFeature)
{
/////////////////////////////////////////////////////////////////////////
//	lpFeature: [in] 指纹特征
//  lpszFileName: [in] 文件路径
/////////////////////////////////////////////////////////////////////////
	// TODO: Add your implementation code here
   	FILE               *fp = NULL;
	BYTE v;
	int         i;
	
	// 对指纹特征进行校验
	v = 0;
	for(i = 0; i < (lpFeature[0]*256+lpFeature[1]); i++)
	{
		v = v ^ lpFeature[i];
	}
	// 校验失败返回错误
	if(v != 0) 
		return VF_SAVEFEATURE_ERR;
	
	// 创建并写入指纹特征数据
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
// LoadFeature: 从指纹特征到文件中读取指纹特征
//
FPENGINE_API int LoadFeature(LPCSTR lpszFileName, BYTE *lpFeature, int *lpSize)
{
/////////////////////////////////////////////////////////////////////////
//  lpszFileName: [in] 指纹特征文件路径
//	lpFeature: [out] 指纹特征
//  lpSize: [out] 指纹特征大小
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
	// 读取指纹特征大小
	fread((void *)&lpFeature[0], 2, 1, fp);
	// 读取指纹特征其他字段数据
	fread((void *)&lpFeature[2], lpFeature[0]*256+lpFeature[1]-1, 1, fp);

	fclose(fp);	

	// 校验
	v = 0;
	for(i = 0; i < (lpFeature[0]*256+lpFeature[1]); i++)
	{
		v = v ^ lpFeature[i];
	}
	// 校验失败返回错误
	if(v != 0)
		return VF_LOADFEATURE_ERR;


	return 0;
}

//
// LoadFinger: 从指纹图像文件中读取指纹图像数据
//
FPENGINE_API int LoadFingerImage(LPCSTR lpszFileName, BYTE *lpFinger, int *lpWidth, int *lpHeight)
{
/////////////////////////////////////////////////////////////////////////
//  lpszFileName: [in] 指纹图像文件路径
//	lpFinger: [out] 指纹图像矩阵数据
//  lpWidth: [out] 图像宽度指针
//  lpHeight: [out] 图像高度指针
/////////////////////////////////////////////////////////////////////////

	// TODO: Add your implementation code here
	int	re;

	// 读取图像数据并得到图像大小
	re = loadBitmap((char *)lpszFileName, lpFinger, lpWidth, lpHeight);
	if(re != 0)
		return VF_LOADFINGER_ERR;

	return 0;
}

//
// SaveFinger: 将指纹图像数据保存为BMP文件
//
FPENGINE_API int SaveFingerImage(LPCSTR lpszFileName, BYTE *lpFinger, int Width, int Height)
{
/////////////////////////////////////////////////////////////////////////
//	lpFinger: [in] 指纹图像矩阵数据
//  Width: [in] 图像宽度
//  Height: [in] 图像高度
//  lpszFileName: [in] 指纹图像文件路径
/////////////////////////////////////////////////////////////////////////

	// TODO: Add your implementation code here
	int	re;

	// 保存指纹图像数据为BMP文件
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
// GetQuality: 获取指纹图像质量分数(100分满分)
//
FPENGINE_API int GetImageQuality(BYTE *lpFinger, int Width, int Height, int *pScore)
{
/////////////////////////////////////////////////////////////////////////
//	lpFinger: [in] 指纹图像矩阵数据
//  Width: [in] 图像宽度
//  Height: [in] 图像高度
//  pScore: [out] 质量分数指针
/////////////////////////////////////////////////////////////////////////

	// TODO: Add your implementation code here
	VF_RETURN	re;
	sint32	x, y, num;
	sint32  temp,score;

	// 导入指纹图像数据
	re = VF_ImportFinger(lpFinger, Width, Height);
	
	if(re != VF_OK)
		return re;
	
	// 对指纹图像进行二次平滑
	smooth(g_lpOrgFinger, g_lpTemp, 1, 1);
	smooth(g_lpOrgFinger, g_lpTemp, 1, 1);
	memcpy(g_lpOrgFinger, g_lpTemp, IMGSIZE);
	// 利用图像低频部分计算方向场，提高速度
	zoomout();
	// 计算指纹图像的方向场，得到幅值
	getOrientMap(6);
	// 分割指纹图像
	divide(12, 32);

	//统计前景区域大小
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
	
	// 计算质量分数
	score = (int)(100*num/(0.8*IMGSIZE)+0.5);
	if(score > 100)
		score = 100;
	*pScore = score;

	return 0;
}