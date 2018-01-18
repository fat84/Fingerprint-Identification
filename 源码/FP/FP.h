// FP.h : main header file for the FP DLL
//

#if !defined(AFX_FP_H__E6FB4BFE_4334_4C9D_A3D8_B5EB2BD19DEB__INCLUDED_)
#define AFX_FP_H__E6FB4BFE_4334_4C9D_A3D8_B5EB2BD19DEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CFPApp
// See FP.cpp for the implementation of this class
//

class CFPApp : public CWinApp
{
public:
	CFPApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFPApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CFPApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FP_H__E6FB4BFE_4334_4C9D_A3D8_B5EB2BD19DEB__INCLUDED_)
