#ifndef	__VFALIGNMATCH_H__
#define __VFALIGNMATCH_H__

#include "VF_Type.h"
#include "VF_Global.h"
#include "VF_Function.h"

// Ϊ��ʡʱ�䣬�̾��������������ò��
// DisTbl[m][n] = (int)(sqrt(m*m+n*n)+0.5)
sint32  DisTbl[10][10] = {
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

//
//	align: ��ָ��������һ���ĽǶȺ�λ��ƫ�ƽ�������任
//
void align(FEATUREPTR lpFeature, FEATUREPTR lpAlignedFeature, MINUTIAPTR lpFeatureCore,
				  sint32 rotation, sint32 transx, sint32 transy)
{
///////////////////////////////////////////////////////////////////////
//  lpFeature:	[in] Ҫ�任��ָ������
//  lpAlignedFeature:	[out] ��������任���ָ������
//  lpFeatureCore:	[in] ��ת�任������������
//  rotation:	[in] ��ת�Ƕ�
//  transx:	[in] ˮƽƫ��
//  transy: [in] ��ֱƫ��
///////////////////////////////////////////////////////////////////////

	sint32  i;
	sint32  x, y;
	sint32  cx, cy;
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
		lpAlignedFeature->MinutiaArr[i].x = (sint32)(cx + cosv*(x-cx) - sinv*(y-cy) + transx + 0.5);
		lpAlignedFeature->MinutiaArr[i].y = (sint32)(cy + sinv*(x-cx) + cosv*(y-cy) + transy + 0.5);
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
	sint32	i, j;
	uint8	flagA[MAXMINUTIANUM];	// ���lpAlignFeature���������Ƿ��Ѿ���ƥ�����
	uint8	flagT[MAXMINUTIANUM];	// ���lpTemplate���������Ƿ��Ѿ���ƥ�����
	sint32	x1, y1, x2, y2;
	sint32  dis, angle;
	sint32  score, matchNum, s;
	sint32	num1, num2;
	sint32  step = 1;
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
			// �˱���dis,angleԽ�󣬷���ԽС
			score += (10-angle);
			score += (10-dis);
			// ƥ������������һ
			matchNum++;
			
			// ����ǿ��ٱȶ�ģʽ
			if(matchMode == VF_MATCHMODE_IDENTIFY && matchNum >= 8)
			{
				// �������ƶ�
				s = 4 * score * matchNum * MAXMINUTIANUM / ((num1 + num2) * ( num1 + num2));
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
	s = 4 * score * matchNum * MAXMINUTIANUM / ((num1+num2)*(num1+num2));

	lpMatchResult->MMCount = matchNum;
	lpMatchResult->Rotation = 0;
	lpMatchResult->Similarity = s;
	lpMatchResult->TransX = 0;
	lpMatchResult->TransX = 0;

}



#endif