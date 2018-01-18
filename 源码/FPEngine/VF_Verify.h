#ifndef	__VFVERIFY_H__
#define __VFVERIFY_H__

#include "VF_Type.h"
#include "VF_Global.h"
#include "VF_Function.h"
#include "VF_GlobalMatch.h"
#include "VF_PatternMatch.h"


//
//	verify : ָ�Ʊȶ�
//
void	verify(FEATUREPTR lpFeature, FEATUREPTR lpTemplate, PMATCHRESULT lpMatchResult,
			   VF_FLAG matchMode)
{
/////////////////////////////////////////////////////////////////////////////
//	lpFeature: [in] Ҫ�ȶԵĵ�һ��ָ������ָ��
//	lpTemplate: [in] Ҫ�ȶԵĵڶ���ָ������ָ��
//	lpMatchResult: [out] �ȶԽ��ָ��
//	matchMode: [in] �ȶ�ģʽ
/////////////////////////////////////////////////////////////////////////////
	
	MATCHRESULT globalMatchResult;
	sint32		score = 0;
	
	//ȫ�ֱȶ�
	globalMatch(lpFeature, lpTemplate, &globalMatchResult, matchMode);
	//patternMatch(lpFeature, lpTemplate, &globalMatchResult, matchMode);
	*lpMatchResult = globalMatchResult;
	//���ƥ�������������٣���ԱȶԽ�����ƶȽ�������
	if(matchMode != VF_MATCHMODE_IDENTIFY && lpMatchResult->MMCount < 15)
	{
		score = (sint32)(lpMatchResult->Similarity*sin(lpMatchResult->MMCount*PI/30.0) + 0.5);
		lpMatchResult->Similarity = score;
	}

}


#endif

