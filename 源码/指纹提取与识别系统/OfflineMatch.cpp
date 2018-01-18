// OfflineMatch.cpp : implementation file
//

#include "stdafx.h"
#include "指纹提取与识别系统.h"
#include "OfflineMatch.h"
#include "FPA.H"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// OfflineMatch dialog

LPVOID lpImage1 = new BYTE[SizeCDib];
LPVOID lpImage2 = new BYTE[SizeCDib];

BYTE *lpData1;
BYTE *lpData2;

int Width1, Height1;
int Width2, Height2;

CString ImagePathName1, ImagePathName2;


OfflineMatch::OfflineMatch(CWnd* pParent /*=NULL*/)
	: CDialog(OfflineMatch::IDD, pParent)
{
	//{{AFX_DATA_INIT(OfflineMatch)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void OfflineMatch::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(OfflineMatch)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(OfflineMatch, CDialog)
	//{{AFX_MSG_MAP(OfflineMatch)
	ON_BN_CLICKED(IDC_BUTTON_OPEN1, OnButtonOpen1)
	ON_BN_CLICKED(IDC_BUTTON_OPEN2, OnButtonOpen2)
	ON_BN_CLICKED(IDC_BUTTON_MATCH, OnButtonMatch)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// OfflineMatch message handlers

void OfflineMatch::OnButtonOpen1() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE,"bmp",".bmp",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"Bitmap (*.BMP)|*.BMP||");
	HBITMAP m_hImage;

	if (dlg.DoModal() != IDOK) return;

		ImagePathName1 = dlg.GetPathName();				
		
		m_hImage = (HBITMAP)LoadImage(AfxGetInstanceHandle(),
								dlg.GetPathName(),
								IMAGE_BITMAP,
								0,
								0,
								LR_LOADFROMFILE|LR_CREATEDIBSECTION);
		GetDlgItem(IDC_STATIC_IMG1)->SendMessage(STM_SETIMAGE,IMAGE_BITMAP, (LPARAM)m_hImage);
	

	

	FPA_CreateImage(lpImage1);
	FPA_LoadImage(ImagePathName1, lpImage1, &lpData1, &Width1, &Height1);
}

void OfflineMatch::OnButtonOpen2() 
{
	CFileDialog dlg(TRUE,"bmp",".bmp",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"Bitmap (*.BMP)|*.BMP||");
	HBITMAP m_hImage;

	if (dlg.DoModal() != IDOK) return;

		ImagePathName2 = dlg.GetPathName();				
		
		m_hImage = (HBITMAP)LoadImage(AfxGetInstanceHandle(),
								dlg.GetPathName(),
								IMAGE_BITMAP,
								0,
								0,
								LR_LOADFROMFILE|LR_CREATEDIBSECTION);
		GetDlgItem(IDC_STATIC_IMG2)->SendMessage(STM_SETIMAGE,IMAGE_BITMAP, (LPARAM)m_hImage);
	

	

	FPA_CreateImage(lpImage2);
	FPA_LoadImage(ImagePathName2, lpImage2, &lpData2, &Width2, &Height2);		
}

void OfflineMatch::OnButtonMatch() 
{
	// TODO: Add your control notification handler code here
	BYTE lpFeature1[430];
	BYTE lpFeature2[430];

	int lpSize1, lpSize2, score;

	FPA_AnalyzeFeature_File(ImagePathName1, lpFeature1, &lpSize1);
	FPA_AnalyzeFeature_File(ImagePathName2, lpFeature2, &lpSize2);

	FPA_PatternMatch(lpFeature1, lpFeature2, &score);

	if (score > 60)
	{
		AfxMessageBox("两个是相同的指纹！");
	}
	else
	{
		AfxMessageBox("两个是不同的指纹！");
	}

}

