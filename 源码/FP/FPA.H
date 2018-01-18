
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DLLTEST_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DLLTEST_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
/*
#ifdef FPA_EXPORTS
#define FPA_API __declspec(dllexport)
#else
#define FPA_API __declspec(dllimport)
#endif
*/
//#pragma comment(lib, "Debug/FP.lib")

#include "StdAfx.h"

#define FPA_API __declspec(dllexport)

#ifdef __cplusplus
extern "C"
{
#endif

// This class is exported from the dlltest.dll
	/*
class FPA_API FPA {
public:
	FPA(void);
	// TODO: add your methods here.
};

extern FPA_API int nDlltest;
*/
#define SizeCDib 32

//extern FPA_API int g_RectifyParm[8][8] = {31,31,606,31,121,463,521,463,
//							25,6,609,6,104,475,538,475};



FPA_API int FPA_Test(int x);
FPA_API int FPA_CreateImage(LPVOID lpDib);
FPA_API int FPA_CloseImage(LPVOID lpDib);
FPA_API int FPA_LoadImage(CString filename, LPVOID lpDib, BYTE **lpData, int *lpWidth, int *lpHeight);
FPA_API int FPA_SaveImage(CString filename, LPVOID lpDib);
FPA_API int FPA_CopyObject(LPVOID lpDib, LPVOID lpnewDib);
FPA_API int FPA_GetData(LPVOID lpDib, BYTE **lpData);
FPA_API int FPA_SetSize(LPVOID lpDib, int Width, int Height);

/*
/////////////////////////////////////////////////////////////////////////
FPA_API int FPA_Rectify(BYTE *lpData, BYTE *lpDataOut, int Width, int Height);
*/

//FPA_API int FPA_Preprocess(CString CurrentPath, BYTE *lpDataIn, BYTE *lpDataOut, LPVOID lpDibOut, BYTE *lpOrient, BYTE *lpSegment, int Width, int Height);
FPA_API int FPA_Preprocess(LPCSTR ImageFile, LPCSTR CurrentPath);

FPA_API int FPA_ExtractFeature(BYTE *lpData, BYTE *lpOrient, LPVOID lpFeature, int Width, int Height);

FPA_API int FPA_Match(LPVOID lpFeature1, LPVOID lpFeature2, int *lpScore);

/////////////////////////////////////////////////////////////////////

FPA_API int FPA_GaussSmooth(BYTE *lpDataIn, BYTE *lpDataOut, int Width, int Height, double sigma);

FPA_API int FPA_Smooth(BYTE *lpDataIn, BYTE *lpDataOut, int Width, int Height, int Type);

FPA_API int FPA_ZoomOut(BYTE *lpDataIn, BYTE *lpDataOut, int Width, int Height);

FPA_API int FPA_FrequencyFields(BYTE *lpData, BYTE *lpDataOut, int Width, int Height, int r);

FPA_API int FPA_Equalize(BYTE *lpData, BYTE *lpDataOut, int Width, int Height);

FPA_API int FPA_Converge(BYTE *lpData, BYTE *lpDataOut, int Width, int Height, double sigma);

FPA_API int FPA_OrientationFields(BYTE *lpData, BYTE *lpDataOut, int Width, int Height, int r);

FPA_API int FPA_Grads(BYTE *lpData, BYTE *lpDataOut, int Width, int Height, int r);

FPA_API int FPA_Segment(BYTE *lpData, BYTE *lpDataOut, int r, int threshold, int Width, int Height);

FPA_API int FPA_Segment_Clear(BYTE *lpData, BYTE *lpDataOut, BYTE *lpOrientation, BYTE *lpSegment, int Width, int Height);

FPA_API int FPA_Enhance(BYTE *lpData, BYTE *lpDataOut, BYTE *lpOrientation, int Width, int Height, int Type);

FPA_API int FPA_Enhance_Gabor(BYTE *lpData, BYTE *lpDataOut, BYTE *lpOrientation, int Width, int Height);

FPA_API int FPA_Binary(BYTE *lpData, BYTE *lpOrientation, BYTE *lpDataOut, int Width, int Height);

FPA_API int FPA_Binary_Clean(BYTE *lpData, BYTE *lpDataOut, BYTE *lpSegment, int Width, int Height);

FPA_API int FPA_Thin(BYTE *lpData, BYTE *lpDataOut, int Width, int Height);

FPA_API int FPA_Thin_Clean(BYTE *lpData, BYTE *lpDataOut, int len, int Width, int Height);

FPA_API int FPA_GetMinutia(BYTE *lpData, BYTE *lpOrient, LPVOID feature, int IMGW, int IMGH);

FPA_API int FPA_GetSingulary(BYTE *lpOrient, int Width, int Height, int *lpNum, PPOINT lpArr, char flag);

//////////////////////////////////////////////////////
/////////////////////////////////////////////////////

FPA_API int FPA_Load_Fingerprint(CString FileName, BYTE *lpData, int *lpWidth, int *lpHeight);
FPA_API int FPA_Save_Fingerprint(CString FileName, BYTE *lpData, int Width, int Height);

FPA_API int FPA_Load_Fingerprint_Feature(CString FileName, BYTE *lpFeature, int *lpSize);
FPA_API int FPA_Save_Fingerprint_Feature(CString FileName, BYTE *lpFeature);

FPA_API int FPA_AnalyzeFeature(BYTE *lpData, int Width, int Height, BYTE *lpFeature, int *lpSize);
FPA_API int FPA_AnalyzeFeature_File(CString FileName, BYTE *lpFeature, int *lpSize);
FPA_API int FPA_PatternMatch(BYTE *lpFeature1, BYTE *lpFeature2, int *lpScore);
FPA_API int FPA_ImageQuality(BYTE *lpData, int Width, int Height, int *pScore);

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////


#ifdef __cplusplus
}
#endif
