#if !defined(AFX_FINGERMATCH_H__513DC3F1_DB2C_4E6E_AB1A_1141F9E83CB4__INCLUDED_)
#define AFX_FINGERMATCH_H__513DC3F1_DB2C_4E6E_AB1A_1141F9E83CB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FingerMatch.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// FingerMatch dialog

class FingerMatch : public CDialog
{
// Construction
public:
	FingerMatch(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(FingerMatch)
	enum { IDD = IDD_FINGERMATCH_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(FingerMatch)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(FingerMatch)
	afx_msg void OnButtonOpenLeft();
	afx_msg void OnButtonOpenRight();
	afx_msg void OnButtonMatch();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINGERMATCH_H__513DC3F1_DB2C_4E6E_AB1A_1141F9E83CB4__INCLUDED_)
