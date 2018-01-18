// 指纹提取与识别系统.h : main header file for the MYFPSYS application
//

#if !defined(AFX_MYFPSYS_H__3C5F069A_E868_406A_938F_1592D97DFEA6__INCLUDED_)
#define AFX_MYFPSYS_H__3C5F069A_E868_406A_938F_1592D97DFEA6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMyFPSysApp:
// See MyFPSys.cpp for the implementation of this class
//

class CMyFPSysApp : public CWinApp
{
public:
	CMyFPSysApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyFPSysApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMyFPSysApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYFPSYS_H__3C5F069A_E868_406A_938F_1592D97DFEA6__INCLUDED_)
