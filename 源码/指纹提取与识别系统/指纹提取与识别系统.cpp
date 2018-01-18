// MyFPSys.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "指纹提取与识别系统.h"
#include "FingerMatch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyFPSysApp

BEGIN_MESSAGE_MAP(CMyFPSysApp, CWinApp)
	//{{AFX_MSG_MAP(CMyFPSysApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyFPSysApp construction

CMyFPSysApp::CMyFPSysApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMyFPSysApp object

CMyFPSysApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMyFPSysApp initialization

BOOL CMyFPSysApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	FingerMatch FM;
	m_pMainWnd = &FM;
	int nResponse =FM.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	    //"退出系统"
	}

else return FALSE;
}
