// DIP.h: interface for the DIP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIP_H__0107E418_518C_4E2D_BBD3_418B9F89356D__INCLUDED_)
#define AFX_DIP_H__0107E418_518C_4E2D_BBD3_418B9F89356D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



void GaussSmooth(BYTE *pUnchImg, BYTE *pUnchSmthdImg, int nWidth, int nHeight, double sigma);

void MakeGauss(double sigma, double **pdKernel, int *pnWindowSize);

void getOrientMap(unsigned char* g_lpTemp, unsigned char* g_lpOrient, int IMGW, int IMGH, long  r);

void getGrads(unsigned char* g_lpTemp, unsigned char* g_lpOrient, int IMGW, int IMGH, long  r);

void smooth(BYTE *lpInBuffer, BYTE *lpOutBuffer, int IMGW, int IMGH, int r, int d);

void zoomout(BYTE *lpDataIn, BYTE *lpDataOut, int Width, int Height);

void equalize(BYTE *lpDIBBits, BYTE *lpDataOut, int lWidth, int lHeight);

int segment(BYTE *g_lpDivide, BYTE *g_lpTemp, int r, int threshold, int IMGW, int IMGH);

void segment_clearEdge(BYTE *g_lpOrgFinger, BYTE *g_lpOrient, BYTE* g_lpDivide, int IMGW, int IMGH);

void orientEnhance(BYTE *g_lpOrient, BYTE *g_lpOrgFinger, int IMGW, int IMGH);

int binary(BYTE *g_lpOrgFinger, BYTE *g_lpTemp, BYTE *g_lpOrientation, int IMGW, int IMGH);

void binaryClear(BYTE *g_lpOrgFinger, BYTE *g_lpTemp, BYTE *g_lpDivide, int IMGW, int IMGH);

int imageThin(BYTE *lpBits, BYTE *g_lpTemp, int Width, int Height);

int thinClear(BYTE *g_lpOrgFinger, int  len, int IMGW, int IMGH);


#endif // !defined(AFX_DIP_H__0107E418_518C_4E2D_BBD3_418B9F89356D__INCLUDED_)
