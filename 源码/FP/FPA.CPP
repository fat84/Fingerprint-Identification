#include "FPA.H"
#include "Dib.h"
#include "preprocess.h"
#include "MinutiaExtract.h"
#include "FPEngine.h"
#include "patternMatch.h"
#include "coding.h"


FPA_API int FPA_Test(int x)
{
	return x + 1;
}

FPA_API int FPA_CreateImage(LPVOID lpDib)
{
	CDib dib;
	memcpy(lpDib, &dib, sizeof(CDib));
	
	return 0;
}


FPA_API int FPA_CloseImage(LPVOID lpDib)
{
	if (lpDib == NULL) return 0;

	CDib *dib = (CDib*)(lpDib);
	dib->Close();
		
	return 0;
}


FPA_API int FPA_LoadImage(CString filename, LPVOID lpDib, BYTE **lpData, int *lpWidth, int *lpHeight)
{
	CDib *dib = (CDib*)(lpDib);

	char *path = new char[filename.GetLength()];
	path = filename.GetBuffer(0);
	dib->Open(path);

	*lpData = dib->m_pDibBits;
	*lpWidth = dib->GetWidth();
	*lpHeight = dib->GetHeight();
	
	return 0;
}

FPA_API int FPA_SaveImage(CString filename, LPVOID lpDib)
{		
	CDib *dib = (CDib*)(lpDib);
	dib->Save((LPCSTR)filename);
	return 0;
}

FPA_API int FPA_CopyObject(LPVOID lpDib, LPVOID lpnewDib)
{		
	memcpy(lpnewDib, lpDib, SizeCDib);

	CDib *dib = (CDib*)(lpDib);
	CDib *newdib = (CDib*)(lpnewDib);
	
	int Size = dib->bmpFileHeader.bfSize-14;


	newdib->m_pDib = new BYTE[Size];
	memcpy(newdib->m_pDib, dib->m_pDib, Size);

	newdib->m_pDibBits = newdib->GetBits();

	newdib->m_hDrawDib = DrawDibOpen();

	return 0;
}

FPA_API int FPA_GetData(LPVOID lpDib, BYTE **lpData)
{
	CDib *dib = (CDib*)(lpDib);
	*lpData = dib->m_pDibBits;
	return 0;
}

FPA_API int FPA_SetSize(LPVOID lpDib, int Width, int Height)
{
	CDib *dib = (CDib*)(lpDib);
	((BITMAPINFOHEADER *)(dib->m_pDib))->biWidth = Width;
	((BITMAPINFOHEADER *)(dib->m_pDib))->biHeight = Height;

	return 0;
}


//FPA_API int FPA_Preprocess(CString CurrentPath, BYTE *lpDataIn, BYTE *lpDataOut, LPVOID lpDibOut, BYTE *lpOrient, BYTE *lpSegment, int Width, int Height)
FPA_API int FPA_Preprocess(LPCSTR ImageFile, LPCSTR CurrentPath)
{

	CString filename;
	CString Path;			
	Path.Format("%s\\temp_", CurrentPath);
	
	const char *path = (LPCSTR)Path;
				
		
	CDib dib;
	dib.Open((LPCSTR)ImageFile);
	filename.Format("%s0.bmp", path);	
	dib.Save((LPCSTR)filename);

	int Width = dib.GetWidth();
	int Height = dib.GetHeight();
	int IMGSIZE = Width * Height;
	BYTE *lpDataIn = dib.m_pDibBits;
		

	BYTE *lpTemp = new BYTE[IMGSIZE];
	BYTE *lpGrad = new BYTE[IMGSIZE];
	BYTE *lpOrient = new BYTE[IMGSIZE];
	BYTE *lpSegment = new BYTE[IMGSIZE];
	BYTE *lpZoom = new BYTE[IMGSIZE];
	

	//Smooth
	smooth(lpDataIn, lpTemp, Width, Height, 1, 1);
	memcpy(lpDataIn, lpTemp, IMGSIZE);
	smooth(lpDataIn, lpTemp, Width, Height, 1, 1);
	memcpy(lpDataIn, lpTemp, IMGSIZE);
	filename.Format("%s1.bmp", path);	
	dib.Save((LPCSTR)filename);
	

	//zoom
	zoomout(lpDataIn, lpZoom, Width, Height);


	//Segment

	getGrads(lpZoom, lpGrad, Width, Height, 6);		
	segment(lpSegment, lpGrad, 12, 35, Width, Height); 		
	segment_clearEdge(lpDataIn, lpOrient, lpSegment, Width, Height);	
	//filename = CurrentPath + "2.bmp";
	//dib.Save((LPCSTR)filename);


	//getOrientation	
	getOrientMap(lpZoom, lpOrient, Width, Height, 6);
	memcpy(lpTemp, lpDataIn, IMGSIZE);
	memcpy(lpDataIn, lpOrient, IMGSIZE);
	filename.Format("%s5.bmp", path);	
	dib.Save((LPCSTR)filename);
	memcpy(lpDataIn, lpTemp, IMGSIZE);



	//Enhance
	orientEnhance(lpOrient, lpDataIn, Width, Height);	
	filename.Format("%s2.bmp", path);	
	dib.Save((LPCSTR)filename);


	//Binary
	binary(lpDataIn, lpTemp, lpOrient, Width, Height);	
	binaryClear(lpTemp, lpDataIn, lpSegment, Width, Height);	
	segment_clearEdge(lpDataIn, lpTemp, lpSegment, Width, Height);
	memcpy(lpDataIn, lpTemp, IMGSIZE);
	filename.Format("%s3.bmp", path);	
	dib.Save((LPCSTR)filename);



	//Thin
	imageThin(lpDataIn, lpTemp, Width, Height);	
	thinClear(lpTemp, 12, Width, Height);
	memcpy(lpDataIn, lpTemp, IMGSIZE);
	filename.Format("%s4.bmp", path);	
	dib.Save((LPCSTR)filename);
	
	dib.Close();

	return 0;
}


FPA_API int FPA_ExtractFeature(BYTE *lpData, BYTE *lpOrient, LPVOID lpFeature, int Width, int Height)
{
//	return getMinutia(lpData, lpOrient, lpFeature, Width, Height);
	return 0;
}

FPA_API int FPA_Match(LPVOID lpFeature1, LPVOID lpFeature2, int *lpScore)
{	

	MATCHRESULT mr;
	FEATURE feature1;
	FEATURE feature2;
	DecodeFeature((char*)lpFeature1, &feature1);
	DecodeFeature((char*)lpFeature2, &feature2);
	patternMatch(&feature1, &feature2, &mr, 2);
	if(mr.MMCount < 8)
		*lpScore = 0;
	else
		*lpScore = mr.Similarity;
	
	return 0;
}



FPA_API int FPA_GaussSmooth(BYTE *lpDataIn, BYTE *lpDataOut, int Width, int Height, double sigma)
{	
	GaussSmooth(lpDataIn, lpDataOut, Width, Height, sigma);
	return 0;
}

FPA_API int FPA_Converge(BYTE *lpData, BYTE *lpDataOut, int Width, int Height, double sigma)
{
	GaussSmooth(lpData, lpDataOut, Width, Height, sigma);
	return 0;
}


FPA_API int FPA_OrientationFields(BYTE *lpDataIn, BYTE *lpDataOut, int Width, int Height, int r)
{
	getOrientMap(lpDataIn, lpDataOut, Width, Height, r);

	return 0;
}



FPA_API int FPA_Smooth(BYTE *lpDataIn, BYTE *lpDataOut, int Width, int Height, int Type)
{
	if (Type == 0)
		smooth(lpDataIn, lpDataOut, Width, Height, 1, 1);
	return 0;
}


FPA_API int FPA_ZoomOut(BYTE *lpDataIn, BYTE *lpDataOut, int Width, int Height)
{
	zoomout(lpDataIn, lpDataOut, Width, Height);

	return 0;
}


FPA_API int FPA_FrequencyFields(BYTE *lpData, BYTE *lpDataOut, int Width, int Height, int r)
{
	return 0;
}


FPA_API int FPA_Equalize(BYTE *lpData, BYTE *lpDataOut, int Width, int Height)
{
	equalize(lpData, lpDataOut, Width, Height);

	return 0;
}

FPA_API int FPA_Grads(BYTE *lpData, BYTE *lpDataOut, int Width, int Height, int r)
{
	getGrads(lpData, lpDataOut, Width, Height, r);

	return 0;
}

FPA_API int FPA_Segment(BYTE *lpData, BYTE *lpDataOut, int r, int threshold, int Width, int Height)
{	
	return segment(lpDataOut, lpData, r, threshold, Width, Height);
}

FPA_API int FPA_Segment_Clear(BYTE *lpData, BYTE *lpDataOut, BYTE *lpOrientation, BYTE *lpSegment, int Width, int Height)
{
	memcpy(lpDataOut, lpData, Width * Height);
	segment_clearEdge(lpDataOut, lpOrientation, lpSegment, Width, Height);
	return 0;
}

FPA_API int FPA_Enhance(BYTE *lpData, BYTE *lpDataOut, BYTE *lpOrientation, int Width, int Height, int Type)
{
	memcpy(lpDataOut, lpData, Width * Height);

	if (Type == 1)
		orientEnhance(lpOrientation, lpDataOut, Width, Height);

	return 0;	
}


FPA_API int FPA_Enhance_Gabor(BYTE *lpData, BYTE *lpDataOut, BYTE *lpOrientation, int Width, int Height)
{
	return FPA_Enhance(lpData, lpDataOut, lpOrientation, Width, Height, 1);
}

FPA_API int FPA_Binary(BYTE *lpData, BYTE *lpOrientation, BYTE *lpDataOut, int Width, int Height)
{
	return binary(lpData, lpDataOut, lpOrientation, Width, Height);
}

FPA_API int FPA_Binary_Clean(BYTE *lpData, BYTE *lpDataOut, BYTE *lpSegment, int Width, int Height)
{
	binaryClear(lpData, lpDataOut, lpSegment, Width, Height);

	return 0;
}

FPA_API int FPA_Thin(BYTE *lpData, BYTE *lpDataOut, int Width, int Height)
{
	return imageThin(lpData, lpDataOut, Width, Height);
}

FPA_API int FPA_Thin_Clean(BYTE *lpData, BYTE *lpDataOut, int len, int Width, int Height)
{
	memcpy(lpDataOut, lpData, Width * Height);
	return thinClear(lpDataOut, len, Width, Height);
}

FPA_API int FPA_GetMinutia(BYTE *lpData, BYTE *lpOrient, LPVOID feature, int IMGW, int IMGH)
{
	return getMinutia(lpData, lpOrient, feature, IMGW, IMGH);
}


FPA_API int FPA_GetSingulary(BYTE *lpOrient, int Width, int Height, int *lpNum, PPOINT lpArr, char flag)
{
	return getSingular(lpOrient, Width, Height, lpNum, lpArr, flag);
}


FPA_API int FPA_Load_Fingerprint(CString FileName, BYTE *lpData, int *lpWidth, int *lpHeight)
{	
	return LoadFingerImage((LPCSTR)FileName, lpData, lpWidth, lpHeight);
}


FPA_API int FPA_Save_Fingerprint(CString FileName, BYTE *lpData, int Width, int Height)
{
	return SaveFingerImage((LPCSTR)FileName, lpData, Width, Height);
}


FPA_API int FPA_Load_Fingerprint_Feature(CString FileName, BYTE *lpFeature, int *lpSize)
{
	return LoadFeature((LPCSTR)FileName, lpFeature, lpSize);	
}

FPA_API int FPA_Save_Fingerprint_Feature(CString FileName, BYTE *lpFeature)
{
	return SaveFeature((LPCSTR)FileName, lpFeature);
}

FPA_API int FPA_AnalyzeFeature(BYTE *lpData, int Width, int Height, BYTE *lpFeature, int *lpSize)
{
	return Analyze(lpData, Width, Height, lpFeature, lpSize);	
}

FPA_API int FPA_AnalyzeFeature_File(CString FileName, BYTE *lpFeature, int *lpSize)
{
	return AnalyzeFromFile((LPCSTR)FileName, lpFeature, lpSize);
}


FPA_API int FPA_PatternMatch(BYTE *lpFeature1, BYTE *lpFeature2, int *lpScore)
{
	return PatternMatch(lpFeature1, lpFeature2, lpScore);	
}

FPA_API int FPA_ImageQuality(BYTE *lpData, int Width, int Height, int *pScore)
{
	 return GetImageQuality(lpData, Width, Height, pScore);	 
}



FPA_API int FPA_CreateDatabase(CString FileName, int *lpIndexList, int *Size)
{
	FILE *file = fopen((LPCSTR)FileName, "w");
	*Size = 0;
	fwrite(Size, 4, 1, file);		
	fclose(file);
	return 0;
}



FPA_API int FPA_LoadDatabase(CString FileName, int *lpIndexList, int *Size)
{
	FILE *file = fopen((LPCSTR)FileName, "r");
	fread(Size, 4, 1, file);	
	if (*Size > 0 && *Size != 0xFFFFFFFF)
		fread(lpIndexList, 4, *Size, file);
	fclose(file);	

	return 0;
}

FPA_API int FPA_SaveDatabase(CString FileName, int *lpIndex, int Size)
{
	FILE *file = fopen((LPCSTR)FileName, "w");
	fwrite(&Size, 4, 1, file);	
	if (Size > 0)
		fwrite(lpIndex, 4, Size, file);
	fclose(file);	

	return 0;
}


FPA_API int FPA_AddEntry(CString FolderName, int ID, int *lpList, int *Size, BYTE FingerIndex, BYTE *lpFeature)
{
	int Value = ID * 10 + FingerIndex;

	CString t;
	t.Format("\\%d.txt", Value);

	CString FilePath(FolderName + t);

	FILE *file = fopen((LPCSTR)FilePath, "r");
	if (file != NULL)
	{
		fclose(file);
		return -1;
	}	

	
	FPA_Save_Fingerprint_Feature(FilePath, lpFeature);

	*(lpList + *Size) = Value;
	(*Size)++;

	return 0;	
}

void readEntry(CString FolderName, int Index, BYTE *lpFeature2)
{
	int Size2;
	CString t;
	t.Format("\\%d.txt", Index);
	CString FileName(FolderName + t);
	FPA_Load_Fingerprint_Feature(FileName, lpFeature2, &Size2);
}

FPA_API int FPA_Identify(CString FileName, int *lpIndex, int Size, BYTE *lpFeature1)
{
	int score, max = 0, result = -1;
	
	BYTE lpFeature2[430];

	CString FolderName = FileName;
	int t = FolderName.ReverseFind('\\');
	FolderName = FolderName.Left(t);

	for(int i = 0; i < Size; i++)
	{
		readEntry(FolderName, *(lpIndex + i), lpFeature2);
		FPA_PatternMatch(lpFeature1, lpFeature2, &score);
		
		if (score > max)
		{
			max = score;
			result = i;
		}
	}
	
	return (max > 60) ? result : -1;
}