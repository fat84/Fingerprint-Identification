// MinutiaExtract.cpp: implementation of the MinutiaExtract class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "math.h"
#include "MinutiaExtract.h"


/*

int  getMinutia(BYTE *g_lpOrgFinger, BYTE *g_lpOrient, LPVOID feature, int IMGW, int IMGH)
{
	Setup(IMGW, IMGH, g_lpOrgFinger);

	int	x, y, i, j, k, n;
	int  temp;
	int  sum;
	BYTE   *lpNow;
	BYTE   *lpDis;
	int  r = 16;		// ���������㿿��ǰ����Ե���������
	double  d, d1, d2, d0, a;
	int  dGate = 16;
	bool    flag;
	int  x11, y11;
	int  x21, y21;
	int  angle1, angle2, angle3, angle4;
	BYTE   *tempPtr[8];
	BYTE   *disPtr[3];
	BYTE   *lpOri;
	int  tempForkNum;
	int  tempEndNum;
	int  ForkNum;
	int  EndNum;
	// ��ʱ����������
	MINUTIA     tempFork[MAXRAWMINUTIANUM];
	MINUTIA     tempEnd[MAXRAWMINUTIANUM];
	MINUTIA     ForkArr[MAXRAWMINUTIANUM];
	MINUTIA     EndArr[MAXRAWMINUTIANUM];
	// ĳ��ΪԲ�ģ��뾶Ϊ5��Բ�����е�ĵ�ַƫ��
	int	SiteR5[28] = {
		-5, IMGW-5, 2*IMGW-5, 3*IMGW-4, 4*IMGW-3, 5*IMGW-2, 5*IMGW-1, 5*IMGW,
		5*IMGW+1, 5*IMGW+2, 4*IMGW+3, 3*IMGW+4, 2*IMGW+5, IMGW+5, 5, -IMGW+5,
		-2*IMGW+5, -3*IMGW+4, -4*IMGW+3, -5*IMGW+2, -5*IMGW+1, -5*IMGW,
		-5*IMGW-1, -5*IMGW-2, -4*IMGW-3, -3*IMGW-4, -2*IMGW-5, -IMGW-5
	}; 
	// ĳ����Χ8����ĵ�ַƫ��
	int	SiteU8[8] = {IMGW-1, IMGW, IMGW+1, 1, -IMGW+1, -IMGW, -IMGW-1, -1};

	// ��ʼ����ʱ����������
	memset((void *)&tempFork[0], 0, MAXRAWMINUTIANUM*sizeof(MINUTIA));
	memset((void *)&tempEnd[0], 0, MAXRAWMINUTIANUM*sizeof(MINUTIA));
	memset((void *)&ForkArr[0], 0, MAXRAWMINUTIANUM*sizeof(MINUTIA));
	memset((void *)&EndArr[0], 0, MAXRAWMINUTIANUM*sizeof(MINUTIA));


	bool    bGood = false;	// �����ֵ�Ƿ�����ã�ʹ��������Ŀ������Χ��
	int  densD = 7;	// �������������������ľ���
	int  loopnum;	// ѭ������
	int  trilen = 16;
	// ��һ��Ѱ�����ж˵�Ͳ��	
	ForkNum = 0; 
	EndNum = 0; 
	temp = 17*IMGW;
	for(y = 17; y < IMGH-17; y++)
	{
		for(x = 17; x < IMGW-17; x++)
		{
			lpNow = g_lpOrgFinger + temp + x;
			lpOri = g_lpOrient + y*IMGW + x;
			// ���Ǻڵ��򲻿���
			if(*lpNow != 0)
			{
				continue;
			}
			// �ǲ��
			if(IsFork(lpNow))
			{
				// ����Ƿ񿿽���Ե, ������Ե�򲻿���
				flag = true;
				for(i = -r; i <= r && flag; i++)
				{
					for(j = -r; j <= r && flag; j++)
					{
						if(y+i<0 || y+i>=IMGH || x+j<0 || x+j>=IMGW)
						{
							continue;
						}

						if(*(g_lpOrient + temp + i*IMGW + x + j) == 255)
						{
							flag = false;
							break;
						}
					}
				}
				// ���õ���Χ�����Ƿ�仯���ң��仯�����򲻿��ǣ���Ϊ�ܿ�������ٵ�
				sum = 0;
				for(i = 0; i < 28; i++)
				{
					sum += GetJiajiao(*(lpOri+SiteR5[(i+1)%28]), *(lpOri+SiteR5[i]));
				}
				if(sum > 96)
					flag = false;
				
				// �ﵽ����Ҫ�����¼�����õ�
				if(flag)
				{
					ForkArr[ForkNum].x = x;
					ForkArr[ForkNum].y = y;
					ForkNum++;
					// ��������Ѿ��������������Ŀ���򷵻ش���
					if(ForkNum >= MAXRAWMINUTIANUM)
					{
						ForkNum = 0;
						return 1;
					}

				}

			}
			else if(IsEnd(lpNow))	// ����Ƕ˵�
			{
				// ����Ƿ񿿽���Ե, ������Ե�򲻿���
				flag = true;
				for(i = -r; i <= r && flag; i++)
				{
					for(j = -r; j <= r && flag; j++)
					{
						if(y+i<0 || y+i>=IMGH || x+j<0 || x+j>=IMGW)
						{
							continue;
						}

						if(*(g_lpOrient + temp + i*IMGW + x + j) == 255)
						{
							flag = false;
							break;
						}
					}
				}

				// ���õ���Χ�����Ƿ�仯���ң��仯�����򲻿��ǣ���Ϊ�ܿ�������ٵ�
				sum = 0;
				for(i = 0; i < 28; i++)
				{
					sum += GetJiajiao(*(lpOri+SiteR5[(i+1)%28]), *(lpOri+SiteR5[i]));
				}
				if(sum > 96)
					flag = false;

				// �ﵽ����Ҫ�����¼�����õ�
				if(flag)
				{
					EndArr[EndNum].x = x;
					EndArr[EndNum].y = y;
					EndNum++;
					// ��������Ѿ��������������Ŀ���򷵻ش���
					if(EndNum >= MAXRAWMINUTIANUM)
					{
						EndNum = 0;
						return 1;
					}

				}

			}
		}
		temp += IMGW;
	}

	// ��ʼ����ʱ��������׼��ѭ������������������������С���룬ʹ֮����һ�����ʵ�ֵ
	// ��Ϊ���������ǳ��ܼ��ĵط����п����������ܲ�ĵط�
	for(i = 0; i < MAXRAWMINUTIANUM; i++)
	{
		tempEnd[i] = EndArr[i];
	}
	for(i = 0; i < MAXRAWMINUTIANUM; i++)
	{
		tempFork[i] = ForkArr[i];
	}
	tempForkNum = ForkNum;
	tempEndNum = EndNum;

	// ѭ������
	bGood = false;
	loopnum = 0;	// �����Ĵ���
	while(!bGood && loopnum < 32) // ������32��
	{
		loopnum++;	// ����������һ
		// �õ��µ�����������
		for(i = 0; i < MAXRAWMINUTIANUM; i++)
		{
			EndArr[i] = tempEnd[i];
		}
		for(i = 0; i < MAXRAWMINUTIANUM; i++)
		{
			ForkArr[i] = tempFork[i];
		}
		// �µ���������
		ForkNum = tempForkNum;	// ���
		EndNum = tempEndNum;	// �˵�

		// ȥ����ٵĶ˵�
		bGood = true;
		for(i = 0; i < EndNum-1; i++)
		{
			flag = false;
			for(j = i+1; j < EndNum; j++)
			{
				// �������˵�ľ���
				d = sqrt((double)((EndArr[i].x-EndArr[j].x)*(EndArr[i].x-EndArr[j].x) +
					(EndArr[i].y-EndArr[j].y)*(EndArr[i].y-EndArr[j].y)));
				// �����㹻��������һ���˵�
				if(d > dGate && d > densD)
				{
					continue;
				}
				// ����̫С������������Ϊ��0��0��
				if(d <= densD)
				{
					EndArr[j].x = 0;
					EndArr[j].y = 0;
					flag = true;
					continue;
				}
				// ���һ���˵��������ߵĽǶ�
				lpNow = g_lpOrgFinger + EndArr[i].y*IMGW + EndArr[i].x;
				if(GetByDis(lpNow, &lpDis, 8) != 0)
				{
					break;
				}
				angle1 = GetAngle(EndArr[i].x, EndArr[i].y, PX(lpDis), PY(lpDis));
				
				// ��ڶ����˵��������ߵĽǶ�
				lpNow = g_lpOrgFinger + EndArr[j].y*IMGW + EndArr[j].x;
				if(GetByDis(lpNow, &lpDis, 8) != 0)
				{
					continue;
				}
				angle2 = GetAngle(EndArr[j].x, EndArr[j].y, PX(lpDis), PY(lpDis));

				// �������Ƕȼ�ľ���
				angle3 = GetAngleDis(angle1, angle2);
				
				// ��������Ƕȼ����ǣ��������������
				if(angle3 > 270 || angle3 < 90)
				{
					continue;
				}

				// �������˵����ߵĽǶ�
				angle3 = GetAngle(EndArr[i].x, EndArr[i].y, EndArr[j].x, EndArr[j].y);
				// ���һ���˵����������ߵļн�
				angle3 = GetAngleDis(angle1, angle3);
				// ����нǽϴ�������ٵ�
				if(angle3 < 150 || angle3 > 210)
				{
					continue;
				}

				// ��ڶ����˵����������ߵļн�
				angle4 = GetAngle(EndArr[j].x, EndArr[j].y, EndArr[i].x, EndArr[i].y);
				angle4 = GetAngleDis(angle2, angle4);
				
				// ����нǽϴ�������ٵ�
				if(angle4 < 150 || angle4 > 210)
				{
					continue;
				}
				// �����ʾ����������ͬһ�������ϵĶ��Ѵ��������˵�,������ԭ��
				EndArr[j].x = 0;
				EndArr[j].y = 0;
				flag = true;
			}
			if(flag)// ��ʾ����������ͬһ�������ϵĶ��Ѵ��������˵�,������ԭ��
			{
				EndArr[i].x = 0;
				EndArr[i].y = 0;
			}
		}

		// ͳ���µĶ˵���Ŀ
		j = 0;
		for(i = 0; i < EndNum; i++)
		{
			if(EndArr[i].x == 0 || EndArr[i].y == 0)
			{
				continue;
			}
			lpNow = g_lpOrgFinger + EndArr[i].y*IMGW + EndArr[i].x;
			if(GetByDis(lpNow, &lpDis, 8) != 0)
			{
				continue;
			}

			j++;
			// ����˵���Ŀ���ǹ��࣬����Ҫ����densD
			if(j >= MAXMINUTIANUM)
			{
				bGood = false;
				break;
			}

		}
		// ����densD
		if(!bGood)
		{
			densD++; // ��һ
			continue;
		}

		// ���˵�װ�䵽g_Feature
		j = 0;
		for(i = 0; i < EndNum; i++)
		{
			if(EndArr[i].x == 0 || EndArr[i].y == 0)
			{
				continue;
			}
			// �������ҵ���8����
			lpNow = g_lpOrgFinger + EndArr[i].y*IMGW + EndArr[i].x;
			if(GetByDis(lpNow, &lpDis, 8) != 0)
			{
				continue;
			}
			// ��õ���˵����ߵĽǶ�
			angle1 = GetAngle(EndArr[i].x, EndArr[i].y, PX(lpDis), PY(lpDis));
			angle2 = *(g_lpOrient + EndArr[i].y*IMGW + EndArr[i].x);
			angle3 = GetAngleDis(angle1, angle2);
			// ����ɶ۽ǣ����ʾ�������㷽����õ㷽�򳡷����෴
			if(angle3 > 90 && angle3 < 270)
			{
				g_Feature.MinutiaArr[j].Direction = angle2 + 180;
			}
			else // �������ǣ����ʾ�������㷽����õ㷽�򳡷�����ͬ
			{
				g_Feature.MinutiaArr[j].Direction = angle2;
			}
			a = g_Feature.MinutiaArr[j].Direction / EPI + PI/4.0;
			x11 = EndArr[i].x + (int)(trilen*cos(a)+ 0.5);
			y11 = EndArr[i].y + (int)(trilen*sin(a)+ 0.5);
			g_Feature.MinutiaArr[j].Triangle[0] = *(g_lpOrient + y11*IMGW + x11);
			a += PI*2/3.0;
			x11 = EndArr[i].x + (int)(trilen*cos(a)+ 0.5);
			y11 = EndArr[i].y + (int)(trilen*sin(a)+ 0.5);
			g_Feature.MinutiaArr[j].Triangle[1] = *(g_lpOrient + y11*IMGW + x11);
			a += PI*2/3.0;
			x11 = EndArr[i].x + (int)(trilen*cos(a)+ 0.5);
			y11 = EndArr[i].y + (int)(trilen*sin(a)+ 0.5);
			g_Feature.MinutiaArr[j].Triangle[2] = *(g_lpOrient + y11*IMGW + x11);

			g_Feature.MinutiaArr[j].x = EndArr[i].x;	//������
			g_Feature.MinutiaArr[j].y = EndArr[i].y;	//������
			g_Feature.MinutiaArr[j].Type = VF_MINUTIA_END;	//����

			j++;

		}

		EndNum = j;	// �˵���Ŀ


		// ȥ������̫���Ĳ��
		for(i = 0; i < ForkNum-1; i++)
		{
			flag = false;
			for(j = i+1; j < ForkNum; j++)
			{
				d = sqrt((double)((ForkArr[i].x-ForkArr[j].x)*(ForkArr[i].x-ForkArr[j].x) +
					(ForkArr[i].y-ForkArr[j].y)*(ForkArr[i].y-ForkArr[j].y)));

				if(d <= densD-2)
				{
					ForkArr[j].x = 0;
					ForkArr[j].y = 0;
					flag = true;
				}
			}
			if(flag)
			{
				ForkArr[i].x = 0;
				ForkArr[i].y = 0;
			}
		}

		// ͳ���µ������Ĳ��
		j = 0;
		for(i = 0; i < ForkNum; i++)
		{
			if(ForkArr[i].x == 0 || ForkArr[i].y == 0)
			{
				continue;
			}
			lpNow = g_lpOrgFinger + ForkArr[i].y*IMGW + ForkArr[i].x;

			tempPtr[0] = lpNow;
			*tempPtr[0] = 255;
			// ����Ƿ��������
			n = 0;
			for(k = 0; k < 8; k++)
			{
				if(*(lpNow + SiteU8[k]) == 0)
				{
					tempPtr[n+1] = lpNow + SiteU8[k];
					*tempPtr[n+1] = 255;
					n++;
				}
			}
			for(k = 0; k < 4; k++)
			{
				*tempPtr[k] = 0;
			}

			if(n != 3)
			{
				continue;
			}
			j++;
			// �����������������������Ŀ��Ҫ����densD
			if(EndNum + j >= MAXMINUTIANUM)
			{
				densD++;
				bGood = false;
				break;
			}

		}

		if(!bGood)
		{
			continue;
		}

		// װ���㵽g_Feature
		j = 0;
		for(i = 0; i < ForkNum; i++)
		{
			if(ForkArr[i].x == 0 || ForkArr[i].y == 0)
			{
				continue;
			}
			lpNow = g_lpOrgFinger + ForkArr[i].y*IMGW + ForkArr[i].x;

			tempPtr[0] = lpNow;
			*tempPtr[0] = 255;
			n = 0;
			for(k = 0; k < 8; k++)
			{
				if(*(lpNow + SiteU8[k]) == 0)
				{
					tempPtr[n+1] = lpNow + SiteU8[k];
					*tempPtr[n+1] = 255;
					n++;
				}
			}
			if(n != 3)
			{
				continue;
			}
			// �ҵ�����������������Ͼ���Ϊ8�ĵ�
			for(k = 0; k < 3; k++)
			{
				GetByDis(tempPtr[k+1], &disPtr[k], 8);
			}
			for(k = 0; k < 4; k++)
			{
				*tempPtr[k] = 0;
			}
			// ���������������ľ���
			d0 = sqrt((double)((PX(disPtr[1]) - PX(disPtr[2]))*(PX(disPtr[1]) - PX(disPtr[2])) +
				(PY(disPtr[1]) - PY(disPtr[2]))*(PY(disPtr[1]) - PY(disPtr[2]))));
			d1 = sqrt((double)((PX(disPtr[0]) - PX(disPtr[2]))*(PX(disPtr[0]) - PX(disPtr[2])) +
				(PY(disPtr[0]) - PY(disPtr[2]))*(PY(disPtr[0]) - PY(disPtr[2]))));
			d2 = sqrt((double)((PX(disPtr[1]) - PX(disPtr[0]))*(PX(disPtr[1]) - PX(disPtr[0])) +
				(PY(disPtr[1]) - PY(disPtr[0]))*(PY(disPtr[1]) - PY(disPtr[0]))));

			// ������̵ı߶�Ӧ���ǲ��߷���
			if(d0 < d1 && d0 < d2)
			{
				x11 = PX(disPtr[1]);
				y11 = PY(disPtr[1]);
				x21 = PX(disPtr[2]);
				y21 = PY(disPtr[2]);
				lpDis = disPtr[0];
			}
			else if(d1 < d0 && d1 < d2)
			{
				x11 = PX(disPtr[0]);
				y11 = PY(disPtr[0]);
				x21 = PX(disPtr[2]);
				y21 = PY(disPtr[2]);
				lpDis = disPtr[1];
			}
			else
			{
				x11 = PX(disPtr[0]);
				y11 = PY(disPtr[0]);
				x21 = PX(disPtr[1]);
				y21 = PY(disPtr[1]);
				lpDis = disPtr[2];
			}
			// ����̱��е��������ߵķ�����õ㷽�򳡷���ļн�
			angle1 = GetAngle(ForkArr[i].x, ForkArr[i].y, (x11+x21)/2, (y11+y21)/2);
			angle2 = *(g_lpOrient + ForkArr[i].y*IMGW + ForkArr[i].x);
			angle3 = GetAngleDis(angle1, angle2);
			// ����ɶ۽ǣ����ʾ�������㷽����õ㷽�򳡷����෴
			if(angle3 > 90 && angle3 < 270)
			{
				g_Feature.MinutiaArr[EndNum + j].Direction = angle2 + 180;
			}
			else // �������ǣ����ʾ�������㷽����õ㷽�򳡷�����ͬ
			{
				g_Feature.MinutiaArr[EndNum + j].Direction = angle2;
			}
			// ��¼��������Ϊ�������Բ�뾶Ϊtrilen������������������ķ���
			a = g_Feature.MinutiaArr[EndNum + j].Direction / EPI + PI/4.0;
			x11 = EndArr[i].x + (int)(trilen*cos(a)+ 0.5);
			y11 = EndArr[i].y + (int)(trilen*sin(a)+ 0.5);
			g_Feature.MinutiaArr[EndNum + j].Triangle[0] = *(g_lpOrient + y11*IMGW + x11);
			a += PI*2/3.0;
			x11 = EndArr[i].x + (int)(trilen*cos(a)+ 0.5);
			y11 = EndArr[i].y + (int)(trilen*sin(a)+ 0.5);
			g_Feature.MinutiaArr[EndNum + j].Triangle[1] = *(g_lpOrient + y11*IMGW + x11);
			a += PI*2/3.0;
			x11 = EndArr[i].x + (int)(trilen*cos(a)+ 0.5);
			y11 = EndArr[i].y + (int)(trilen*sin(a)+ 0.5);
			g_Feature.MinutiaArr[EndNum + j].Triangle[2] = *(g_lpOrient + y11*IMGW + x11);

			g_Feature.MinutiaArr[EndNum + j].x = ForkArr[i].x; //������
			g_Feature.MinutiaArr[EndNum + j].y = ForkArr[i].y; //������
			g_Feature.MinutiaArr[EndNum + j].Type = VF_MINUTIA_FORK; // ����


			j++;
		}
		ForkNum = j; // �����Ŀ


	}

	// ���ѭ��32�ζ����ܵõ������������ʾͼ����������ش���
	if(loopnum >= 32)
	{
		EndNum = 0;
		ForkNum = 0;
		return 1;
	}
	// ��������Ŀ���ڶ˵����Ӳ����
	g_Feature.MinutiaNum = (BYTE)(EndNum+ForkNum);

	//  ��������Ŀ̫���򷵻ش���
	if(ForkNum + EndNum < 8)
	{
		EndNum = 0;
		ForkNum = 0;
		return 1;
	}

	feature = (LPVOID)new BYTE[sizeof(FEATURE)];
	memcpy(feature, &g_Feature, sizeof(FEATURE));

	return 0;
}



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

}*/