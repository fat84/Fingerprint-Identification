// Match.h: interface for the Match class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATCH_H__E121BDFD_DCAE_4972_B54B_1143A3018393__INCLUDED_)
#define AFX_MATCH_H__E121BDFD_DCAE_4972_B54B_1143A3018393__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*
#include "UtilityFunction.h"


// 为节省时间，短距离的两点间距离采用查表法
// DisTbl[m][n] = (int)(sqrt(m*m+n*n)+0.5)
int  DisTbl[10][10] = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 
	1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 
	2, 2, 2, 3, 4, 5, 6, 7, 8, 9, 
	3, 3, 3, 4, 5, 5, 6, 7, 8, 9, 
	4, 4, 4, 5, 5, 6, 7, 8, 8, 9, 
	5, 5, 5, 5, 6, 7, 7, 8, 9, 10, 
	6, 6, 6, 6, 7, 7, 8, 9, 10, 10, 
	7, 7, 7, 7, 8, 8, 9, 9, 10, 11, 
	8, 8, 8, 8, 8, 9, 10, 10, 11, 12, 
	9, 9, 9, 9, 9, 10, 10, 11, 12, 12
};

void verify(FEATUREPTR lpFeature, FEATUREPTR lpTemplate, PMATCHRESULT lpMatchResult, VF_FLAG matchMode);
void globalMatch(FEATUREPTR lpFeature, FEATUREPTR lpTemplate, PMATCHRESULT lpMatchResult, VF_FLAG matchMode);
void alignMatch(FEATUREPTR lpAlignFeature, FEATUREPTR lpTemplate, PMATCHRESULT lpMatchResult, VF_FLAG matchMode);
void align(FEATUREPTR lpFeature, FEATUREPTR lpAlignedFeature, MINUTIAPTR lpFeatureCore, int rotation, int transx, int transy);

*/

#endif // !defined(AFX_MATCH_H__E121BDFD_DCAE_4972_B54B_1143A3018393__INCLUDED_)
