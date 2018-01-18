// Match.cpp: implementation of the Match class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Match.h"
/*

void verify(FEATUREPTR lpFeature, FEATUREPTR lpTemplate, PMATCHRESULT lpMatchResult,
			   VF_FLAG matchMode)
{
/////////////////////////////////////////////////////////////////////////////
//	lpFeature: [in] Ҫ�ȶԵĵ�һ��ָ������ָ��
//	lpTemplate: [in] Ҫ�ȶԵĵڶ���ָ������ָ��
//	lpMatchResult: [out] �ȶԽ��ָ��
//	matchMode: [in] �ȶ�ģʽ
/////////////////////////////////////////////////////////////////////////////
	
	MATCHRESULT globalMatchResult;
	int		score = 0;
	
	//ȫ�ֱȶ�
	globalMatch(lpFeature, lpTemplate, &globalMatchResult, matchMode);
	*lpMatchResult = globalMatchResult;
	//���ƥ�������������٣���ԱȶԽ�����ƶȽ�������
	if(matchMode != VF_MATCHMODE_IDENTIFY && lpMatchResult->MMCount < 15)
	{
		score = (int)(lpMatchResult->Similarity*sin(lpMatchResult->MMCount*PI/30.0) + 0.5);
		lpMatchResult->Similarity = score;
	}

}

void  globalMatch(FEATUREPTR lpFeature, FEATUREPTR lpTemplate, 
				  PMATCHRESULT lpMatchResult, VF_FLAG matchMode)
{
/////////////////////////////////////////////////////////////////////////////
//	lpFeature: [in] Ҫ�ȶԵĵ�һ��ָ������ָ��
//	lpTemplate: [in] Ҫ�ȶԵĵڶ���ָ������ָ��
//	lpMatchResult: [out] �ȶԽ��ָ��
//	matchMode: [in] �ȶ�ģʽ
/////////////////////////////////////////////////////////////////////////////
	int	m, n, a1, a2;
	int	rotation;		// ��ת�Ƕ�
	int	transx, transy;	// λ��ƫ��
	FEATURE	alignFeature;	// ������ָ������
	MATCHRESULT alignMax;	// ���ƶ����ıȶԽ��
	MATCHRESULT globalMatchResult;	// �ȶԽ��
	int  agate = 8;		// �������˽ṹ�Ƕ����
	int  num = 0;
	// ��ʼ����õıȶԽ��
	alignMax.Similarity = 0;
	alignMax.MMCount = 0;
	alignMax.Rotation = 0;
	alignMax.TransX = 0;
	alignMax.TransY = 0;

	// ����ͬ���͵�ָ������������Ϊͬһ��ָ���������ж���ȶ�
	for(n = 0; n < lpTemplate->MinutiaNum; n++)
	{
		for(m = 0; m < lpFeature->MinutiaNum; m++)
		{
			// ��ͬ�����򲻱ȶ�
			if(lpFeature->MinutiaArr[m].Type != lpTemplate->MinutiaArr[n].Type)
				continue;
			if(matchMode == VF_MATCHMODE_IDENTIFY)
			{
				// �������������˽ṹ�ȶԣ������������׼
				if(lpFeature->MinutiaArr[m].Triangle[0] != 255 && lpTemplate->MinutiaArr[n].Triangle[0] != 255)
				{
					a1 = GetJiajiao(lpFeature->MinutiaArr[m].Triangle[0], lpFeature->MinutiaArr[m].Direction % 180);
					a2 = GetJiajiao(lpTemplate->MinutiaArr[n].Triangle[0], lpTemplate->MinutiaArr[n].Direction % 180);
					if(abs(a1-a2)>agate)
						continue;
				}
				if(lpFeature->MinutiaArr[m].Triangle[0] != 255 && lpTemplate->MinutiaArr[n].Triangle[0] != 255 &&
					lpFeature->MinutiaArr[m].Triangle[1] != 255 && lpTemplate->MinutiaArr[n].Triangle[1] != 255)
				{
					a1 = GetJiajiao(lpFeature->MinutiaArr[m].Triangle[0], lpFeature->MinutiaArr[m].Triangle[1]);
					a2 = GetJiajiao(lpTemplate->MinutiaArr[n].Triangle[0], lpTemplate->MinutiaArr[n].Triangle[1]);
					if(abs(a1-a2)>agate)
						continue;
				}
				if(lpFeature->MinutiaArr[m].Triangle[2] != 255 && lpTemplate->MinutiaArr[n].Triangle[2] != 255 &&
					lpFeature->MinutiaArr[m].Triangle[1] != 255 && lpTemplate->MinutiaArr[n].Triangle[1] != 255)
				{
					a1 = GetJiajiao(lpFeature->MinutiaArr[m].Triangle[1], lpFeature->MinutiaArr[m].Triangle[2]);
					a2 = GetJiajiao(lpTemplate->MinutiaArr[n].Triangle[1], lpTemplate->MinutiaArr[n].Triangle[2]);
					if(abs(a1-a2)>agate)
						continue;
				}
				if(lpFeature->MinutiaArr[m].Triangle[0] != 255 && lpTemplate->MinutiaArr[n].Triangle[0] != 255 &&
					lpFeature->MinutiaArr[m].Triangle[2] != 255 && lpTemplate->MinutiaArr[n].Triangle[2] != 255)
				{
					a1 = GetJiajiao(lpFeature->MinutiaArr[m].Triangle[0], lpFeature->MinutiaArr[m].Triangle[2]);
					a2 = GetJiajiao(lpTemplate->MinutiaArr[n].Triangle[0], lpTemplate->MinutiaArr[n].Triangle[2]);
					if(abs(a1-a2)>agate)
						continue;
				}
			}
			alignFeature.MinutiaNum = 0;
			// ��ת�Ƕ�
			rotation = GetAngleDis(lpFeature->MinutiaArr[m].Direction, 
									lpTemplate->MinutiaArr[n].Direction);
			// λ��ƫ��
			transx = (lpTemplate->MinutiaArr[n].x - lpFeature->MinutiaArr[m].x);
			transy = (lpTemplate->MinutiaArr[n].y - lpFeature->MinutiaArr[m].y);
			
			// ��lpFeature��lpTemplate����
			align(lpFeature, &alignFeature, &lpFeature->MinutiaArr[m],
					rotation, transx, transy);
			// �����������ָ���������бȶ�
			alignMatch(&alignFeature, lpTemplate, &globalMatchResult, matchMode);
			// ����ȶԽ������õıȶԽ�����ã������alignMax
			if(globalMatchResult.Similarity > alignMax.Similarity)
			{
				alignMax.MMCount = globalMatchResult.MMCount;
				alignMax.Similarity = globalMatchResult.Similarity;
				alignMax.Rotation = rotation;
				alignMax.TransX = transx;
				alignMax.TransY = transy;
				
				// ����ǿ��ٱȶ�ģʽ�������ƶȴﵽһ���̶����˳�
				if(matchMode == VF_MATCHMODE_IDENTIFY && alignMax.MMCount >= 8 )
				{
					if(alignMax.Similarity > 100)
					{
						*lpMatchResult = alignMax;
						return;
					}
				}
			}
		}
	}
	
	//���ձȶԽ��
	*lpMatchResult = alignMax;


}




//
//	align: ��ָ��������һ���ĽǶȺ�λ��ƫ�ƽ�������任
//
void align(FEATUREPTR lpFeature, FEATUREPTR lpAlignedFeature, MINUTIAPTR lpFeatureCore,
				  int rotation, int transx, int transy)
{
///////////////////////////////////////////////////////////////////////
//  lpFeature:	[in] Ҫ�任��ָ������
//  lpAlignedFeature:	[out] ��������任���ָ������
//  lpFeatureCore:	[in] ��ת�任������������
//  rotation:	[in] ��ת�Ƕ�
//  transx:	[in] ˮƽƫ��
//  transy: [in] ��ֱƫ��
///////////////////////////////////////////////////////////////////////

	int  i;
	int  x, y;
	int  cx, cy;
	double  rota, sinv, cosv;

	// ���������ṹ��Ϣ
	*lpAlignedFeature = *lpFeature;
	
	// ����ת�������ĵ�����
	cx = lpFeatureCore->x;
	cy = lpFeatureCore->y;
	// ��ת�Ļ���
	rota = rotation/EPI;
	// ��ת���ȵ�sinֵ
	sinv = sin(rota);
	// ��ת���ȵ�cosֵ
	cosv = cos(rota);
	for(i = 0; i < lpFeature->MinutiaNum; i++)
	{
		x = lpFeature->MinutiaArr[i].x;
		y = lpFeature->MinutiaArr[i].y;
		//  ����ת�����������
		lpAlignedFeature->MinutiaArr[i].x = (int)(cx + cosv*(x-cx) - sinv*(y-cy) + transx + 0.5);
		lpAlignedFeature->MinutiaArr[i].y = (int)(cy + sinv*(x-cx) + cosv*(y-cy) + transy + 0.5);
		// ��ת����������·���
		lpAlignedFeature->MinutiaArr[i].Direction = (lpFeature->MinutiaArr[i].Direction + rotation) % 360;
	}

}

//
// alignMatch: ��������ϵ�����ָ���������бȶ�
//
void alignMatch(FEATUREPTR lpAlignFeature, FEATUREPTR lpTemplate, 
				PMATCHRESULT lpMatchResult, VF_FLAG matchMode)
{
	int	i, j;
	BYTE	flagA[MAXMINUTIANUM];	// ���lpAlignFeature���������Ƿ��Ѿ���ƥ�����
	BYTE	flagT[MAXMINUTIANUM];	// ���lpTemplate���������Ƿ��Ѿ���ƥ�����
	int	x1, y1, x2, y2;
	int  dis, angle;
	int  score, matchNum, s;
	int	num1, num2;
	int  step = 1;
	num1 = lpAlignFeature->MinutiaNum;
	num2 = lpTemplate->MinutiaNum;
	// �������
	memset(flagA, 0, MAXMINUTIANUM);
	memset(flagT, 0, MAXMINUTIANUM);

	score = 0;		// �ܷ�����
	matchNum = 0;	// ƥ��������������



	// ��ͬ�����������ƥ�����
	for(i = 0; i < lpTemplate->MinutiaNum; i++)
	{
		if(flagT[i])	// �Ƿ�����ƥ�����
			continue;
		for(j = 0; j < lpAlignFeature->MinutiaNum; j++)
		{
			if(flagA[j])	// �Ƿ�����ƥ�����
				continue;
			// �����������Ƿ���ͬ
			if(lpTemplate->MinutiaArr[i].Type != lpAlignFeature->MinutiaArr[j].Type)
				continue;
			// �����㷽��н�
			angle = AngleAbs360(lpTemplate->MinutiaArr[i].Direction, 
								lpAlignFeature->MinutiaArr[j].Direction);
			// �н�>=10��ƥ��
			if(angle >= 10)
				continue;
			x1 = lpTemplate->MinutiaArr[i].x;
			y1 = lpTemplate->MinutiaArr[i].y;
			x2 = lpAlignFeature->MinutiaArr[j].x;
			y2 = lpAlignFeature->MinutiaArr[j].y;
			// ˮƽ����>=10��ƥ��
			if(abs(x1-x2) >= 10)
				continue;
			// ��ֱ����>=10��ƥ��
			if(abs(y1-y2) >= 10)
				continue;
			// ���������ľ���
			dis = DisTbl[abs(y1-y2)][abs(x1-x2)];
			// ����>=10��ƥ��
			if(dis >= 10)
				continue;
			// ������������������ǣ���ʾ�Ѿ���ƥ�����
			flagA[j] = 1;
			flagT[i] = 1;
			// �ּܷ��ϴ������������ƥ�����
			score += (10-angle);
			score += (10-dis);
			// ƥ������������һ
			matchNum++;
			
			// ����ǿ��ٱȶ�ģʽ
			if(matchMode == VF_MATCHMODE_IDENTIFY && matchNum >= 8)
			{
				// �������ƶ�
				s = 4* score * matchNum * MAXMINUTIANUM / ((num1+num2)*(num1+num2));
				if(s > 100)  // ���ƶ��㹻���򷵻رȶԽ��
				{
					lpMatchResult->MMCount = matchNum;
					lpMatchResult->Rotation = 0;
					lpMatchResult->Similarity = s;
					lpMatchResult->TransX = 0;
					lpMatchResult->TransX = 0;
					return;
				}
			}

		}
	}

	if(matchMode != VF_MATCHMODE_IDENTIFY)
	{
		// ����ͼ��������¿��ܶ˵㴦��ɲ����㴦��ɶ˵㣬�������Ϊ50%������
		// ��������ķ���
		for(i = 0; i < lpTemplate->MinutiaNum; i++)
		{
			if(flagT[i])	// �Ƿ�����ƥ�����
				continue;
			for(j = 0; j < lpAlignFeature->MinutiaNum; j++)
			{
				if(flagA[j])	// �Ƿ�����ƥ�����
					continue;
				// �Ƿ����Ͳ�ͬ
				if(lpTemplate->MinutiaArr[i].Type == lpAlignFeature->MinutiaArr[j].Type)
					continue;
				// �����㷽��н�
				angle = AngleAbs360(lpTemplate->MinutiaArr[i].Direction, 
									lpAlignFeature->MinutiaArr[j].Direction);
				// �н�>=10��ƥ��
				if(angle >= 10)
					continue;
				x1 = lpTemplate->MinutiaArr[i].x;
				y1 = lpTemplate->MinutiaArr[i].y;
				x2 = lpAlignFeature->MinutiaArr[j].x;
				y2 = lpAlignFeature->MinutiaArr[j].y;

				// ˮƽ����>=10��ƥ��
				if(abs(x1-x2) >= 10)
					continue;
				// ��ֱ����>=10��ƥ��
				if(abs(y1-y2) >= 10)
					continue;
				// ���������ľ���
				dis = DisTbl[abs(y1-y2)][abs(x1-x2)];
				// ����>=10��ƥ��
				if(dis >= 10)
					continue;
				// ������������������ǣ���ʾ�Ѿ���ƥ�����
				flagA[j] = 1;
				flagT[i] = 1;
				// �ּܷ��ϴ������������ƥ�����
				score += ((10-angle)/2);
				score += ((10-dis)/2);
				// ƥ������������һ
				matchNum++;

			}
		}								
	}
	// �������ƶȣ����رȶԽ��
	s = 4* score * matchNum * MAXMINUTIANUM / ((num1+num2)*(num1+num2));

	lpMatchResult->MMCount = matchNum;
	lpMatchResult->Rotation = 0;
	lpMatchResult->Similarity = s;
	lpMatchResult->TransX = 0;
	lpMatchResult->TransX = 0;

}
*/