
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the FPENGINE_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// FPENGINE_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#pragma comment(lib, "Debug/FPEngine.lib")

#ifdef FPENGINE_EXPORTS
#define FPENGINE_API __declspec(dllexport)
#else
#define FPENGINE_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

// This class is exported from the FPEngine.dll
class FPENGINE_API CFPEngine {
public:
	CFPEngine(void);
	// TODO: add your methods here.
};

extern FPENGINE_API int nFPEngine;

FPENGINE_API int fnFPEngine(void);


FPENGINE_API int Analyze(BYTE *lpImage, int Width, int Height, BYTE *lpFeature, int *lpSize);

FPENGINE_API int AnalyzeFromFile(LPCSTR lpszFileName, BYTE *lpFeature, int *lpSize);

FPENGINE_API int PatternMatch(BYTE *lpFeature1, BYTE *lpFeature2, int *lpScore);

FPENGINE_API int SaveFeature(LPCSTR lpszFileName, BYTE *lpFeature);

FPENGINE_API int LoadFeature(LPCSTR lpszFileName, BYTE *lpFeature, int *lpSize);

FPENGINE_API int LoadFingerImage(LPCSTR lpszFileName, BYTE *lpFinger, int *lpWidth, int *lpHeight);

FPENGINE_API int SaveFingerImage(LPCSTR lpszFileName, BYTE *lpFinger, int Width, int Height);

FPENGINE_API int GetImageQuality(BYTE *lpFinger, int Width, int Height, int *pScore);


#ifdef __cplusplus
}
#endif
