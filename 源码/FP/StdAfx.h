// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__2A02CA72_4EC1_45F4_A7DE_E6AF5CC08F80__INCLUDED_)
#define AFX_STDAFX_H__2A02CA72_4EC1_45F4_A7DE_E6AF5CC08F80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT


#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


//点类型，坐标值为浮点
typedef  struct DblPoint
{
	double x;
	double y;
} DBLPOINT;


//点类型，坐标值为长整数
/*typedef struct tagPOINT
{
    long  x;
    long  y;
} POINT, *PPOINT, NEAR *NPPOINT, FAR *LPPOINT;
*/

typedef POINT *PPOINT;

#define	MAX_SINGULARYNUM	30
#define     MAXMINUTIANUM			60			// 最大特征点数
#define     MAXRAWMINUTIANUM        100			// 包含虚假特征点的初始特征点最大数目
#define VF_MINUTIA_END        1	// 端点
#define VF_MINUTIA_FORK       2	// 叉点
#define VF_MINUTIA_CORE		  3
#define VF_MINUTIA_DELTA	  4
#define PI					3.1415926
#define EPI					57.29578

#define SAFE_FREE(x) { if (x != NULL) free((void *)x); x = NULL; }

#define WIDTHBYTES(i)    ((i+3)/4*4)
// 特征点结构
typedef struct tagMinutiae { 
  int    x;				// 横坐标
  int    y;				// 纵坐标
  int    Direction;		// 方向
  int	Triangle[3];	// 特征点为中心外接圆半径为定值的正三角形三个顶点的方向
  int    Type;			// 类型
} MINUTIA, *MINUTIAPTR; 

// 指纹特征(模板)结构
typedef struct tagFeature{
	int		MinutiaNum;					// 特征点数
	MINUTIA		MinutiaArr[MAXMINUTIANUM];	// 特征点数组
} FEATURE, *FEATUREPTR;


typedef struct tagMatchResult { 
  int    Similarity; 
  int    Rotation; 
  int    TransX;
  int    TransY;
  int	MMCount;
} MATCHRESULT, *PMATCHRESULT; 

typedef char  VF_FLAG;

// 精确比对模式，主要用在少量比对次数场合
#define	VF_MATCHMODE_VERIFY			1
// 快速比对模式，主要用在大量比对次数场合
#define	VF_MATCHMODE_IDENTIFY		2

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__2A02CA72_4EC1_45F4_A7DE_E6AF5CC08F80__INCLUDED_)
