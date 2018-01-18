#ifndef	__VFGETMINUTIA_H__
#define __VFGETMINUTIA_H__

#include "VF_Type.h"
#include "VF_Global.h"
#include "VF_Function.h"

//
//	getMinutia: ��ȡָ������
//
sint32  getMinutia()
{
	sint32	x, y, i, j, k, n;
	sint32  temp;
	sint32  sum;
	uint8   *lpNow;
	uint8   *lpDis;
	sint32  r = 16;		// ���������㿿��ǰ����Ե���������
	double  d, d1, d2, d0, a;
	sint32  dGate = 16;
	bool    flag;
	sint32  x11, y11;
	sint32  x21, y21;
	sint32  angle1, angle2, angle3, angle4;
	uint8   *tempPtr[8];
	uint8   *disPtr[3];
	uint8   *lpOri;
	sint32  tempForkNum;
	sint32  tempEndNum;
	sint32  ForkNum;
	sint32  EndNum;
	// ��ʱ����������
	MINUTIA     tempFork[MAXRAWMINUTIANUM];
	MINUTIA     tempEnd[MAXRAWMINUTIANUM];
	MINUTIA     ForkArr[MAXRAWMINUTIANUM];
	MINUTIA     EndArr[MAXRAWMINUTIANUM];
	// ĳ��ΪԲ�ģ��뾶Ϊ5��Բ�����е�ĵ�ַƫ��
	sint32	SiteR5[28] = {
		-5, IMGW-5, 2*IMGW-5, 3*IMGW-4, 4*IMGW-3, 5*IMGW-2, 5*IMGW-1, 5*IMGW,
		5*IMGW+1, 5*IMGW+2, 4*IMGW+3, 3*IMGW+4, 2*IMGW+5, IMGW+5, 5, -IMGW+5,
		-2*IMGW+5, -3*IMGW+4, -4*IMGW+3, -5*IMGW+2, -5*IMGW+1, -5*IMGW,
		-5*IMGW-1, -5*IMGW-2, -4*IMGW-3, -3*IMGW-4, -2*IMGW-5, -IMGW-5
	}; 
	// ĳ����Χ8����ĵ�ַƫ��
	sint32	SiteU8[8] = {IMGW-1, IMGW, IMGW+1, 1, -IMGW+1, -IMGW, -IMGW-1, -1};

	// ��ʼ����ʱ����������
	memset((void *)&tempFork[0], 0, MAXRAWMINUTIANUM*sizeof(MINUTIA));
	memset((void *)&tempEnd[0], 0, MAXRAWMINUTIANUM*sizeof(MINUTIA));
	memset((void *)&ForkArr[0], 0, MAXRAWMINUTIANUM*sizeof(MINUTIA));
	memset((void *)&EndArr[0], 0, MAXRAWMINUTIANUM*sizeof(MINUTIA));


	bool    bGood = false;	// �����ֵ�Ƿ�����ã�ʹ��������Ŀ������Χ��
	sint32  densD = 7;	// �������������������ľ���
	sint32  loopnum;	// ѭ������
	sint32  trilen = 16;
	// ��һ��Ѱ�����ж˵�Ͳ��	
	ForkNum = 0; 
	EndNum = 0; 
	temp = 17 * IMGW;
	for(y = 17; y < IMGH-17; y++)
	{
		for(x = 17; x < IMGW-17; x++)
		{
			lpNow = g_lpOrgFinger + temp + x;//ԭͼ��ָ��
			lpOri = g_lpOrient + y*IMGW + x;//ԭͼ����ָ��
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
					sum += GetJiajiao(*(lpOri+SiteR5[(i+1)%28]), *(lpOri+SiteR5[i]));//������������ļн�
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
			lpNow = g_lpOrgFinger + EndArr[i].y * IMGW + EndArr[i].x;
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

			//�����㻥��ƥ�䣬��������Χ����Է�������
			a = g_Feature.MinutiaArr[j].Direction / EPI + PI/4.0;
			x11 = EndArr[i].x + (sint32)(trilen*cos(a)+ 0.5);
			y11 = EndArr[i].y + (sint32)(trilen*sin(a)+ 0.5);
			g_Feature.MinutiaArr[j].Triangle[0] = *(g_lpOrient + y11*IMGW + x11);
			a += PI*2/3.0;
			x11 = EndArr[i].x + (sint32)(trilen*cos(a)+ 0.5);
			y11 = EndArr[i].y + (sint32)(trilen*sin(a)+ 0.5);
			g_Feature.MinutiaArr[j].Triangle[1] = *(g_lpOrient + y11*IMGW + x11);
			a += PI*2/3.0;
			x11 = EndArr[i].x + (sint32)(trilen*cos(a)+ 0.5);
			y11 = EndArr[i].y + (sint32)(trilen*sin(a)+ 0.5);
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
			x11 = EndArr[i].x + (sint32)(trilen*cos(a)+ 0.5);
			y11 = EndArr[i].y + (sint32)(trilen*sin(a)+ 0.5);
			g_Feature.MinutiaArr[EndNum + j].Triangle[0] = *(g_lpOrient + y11*IMGW + x11);
			a += PI*2/3.0;
			x11 = EndArr[i].x + (sint32)(trilen*cos(a)+ 0.5);
			y11 = EndArr[i].y + (sint32)(trilen*sin(a)+ 0.5);
			g_Feature.MinutiaArr[EndNum + j].Triangle[1] = *(g_lpOrient + y11*IMGW + x11);
			a += PI*2/3.0;
			x11 = EndArr[i].x + (sint32)(trilen*cos(a)+ 0.5);
			y11 = EndArr[i].y + (sint32)(trilen*sin(a)+ 0.5);
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
	g_Feature.MinutiaNum = (uint8)(EndNum+ForkNum);

	//  ��������Ŀ̫���򷵻ش���
	if(ForkNum + EndNum < 8)
	{
		EndNum = 0;
		ForkNum = 0;
		return 1;
	}

	return 0;
}

/*

//
//	getSingular�� ��ȡ���ĵ����ǵ�
//
void  getSingular()
{
	sint32  x, y, i, j;
	sint32  DSite8[8][8];
	sint32  DSiteR1[8] = {-1, BOW-1, BOW, BOW+1, 1, -BOW+1, -BOW, -BOW-1};
	bool    flag, flag2, flag3, fg;
	sint32  temp;
	uint8   *lpDir;
	uint8	*lpCore;
	sint32  asum;
	sint32  mina;
	sint32  minindex;
	sint32  DAngleSum[8];
	sint32  ad;
	double  dis;
	VF_MINUTIAPTR    core;
	VF_MINUTIAPTR    delta;
	sint32	CoreNum, DeltaNum;
	//  �뾶Ϊ15��Բ��100���������Բ�ĵĵ�ַƫ��
	sint32 SiteR15[100] = {
		0*IMGW-15, 1*IMGW-15, 2*IMGW-15, 3*IMGW-15, 4*IMGW-15, 4*IMGW-14, 5*IMGW-14, 
		6*IMGW-14, 7*IMGW-13, 8*IMGW-13, 8*IMGW-12, 9*IMGW-12, 10*IMGW-11, 11*IMGW-11, 
		11*IMGW-10, 12*IMGW-9, 12*IMGW-8, 13*IMGW-8, 13*IMGW-7, 14*IMGW-6, 14*IMGW-5, 
		14*IMGW-4, 15*IMGW-4, 15*IMGW-3, 15*IMGW-2, 15*IMGW-1, 15*IMGW+0, 15*IMGW+1, 
		15*IMGW+2, 15*IMGW+3, 14*IMGW+3, 14*IMGW+4, 14*IMGW+5, 13*IMGW+6, 13*IMGW+7, 
		12*IMGW+7, 12*IMGW+8, 11*IMGW+9, 11*IMGW+10, 10*IMGW+10, 9*IMGW+11, 8*IMGW+11, 
		8*IMGW+12, 7*IMGW+12, 6*IMGW+13, 5*IMGW+13, 4*IMGW+13, 4*IMGW+14, 3*IMGW+14, 
		2*IMGW+14, 1*IMGW+14, 0*IMGW+14, -1*IMGW+14, -2*IMGW+14, -3*IMGW+14, -3*IMGW+13, 
		-4*IMGW+13, -5*IMGW+13, -6*IMGW+12, -7*IMGW+12, -7*IMGW+11, -8*IMGW+11, -9*IMGW+10, 
		-10*IMGW+10, -10*IMGW+9, -11*IMGW+8, -11*IMGW+7, -12*IMGW+7, -12*IMGW+6, -13*IMGW+5, 
		-13*IMGW+4, -13*IMGW+3, -14*IMGW+3, -14*IMGW+2, -14*IMGW+1, -14*IMGW+0, -14*IMGW-1, 
		-14*IMGW-2, -14*IMGW-3, -14*IMGW-4, -13*IMGW-4, -13*IMGW-5, -13*IMGW-6, -12*IMGW-7, 
		-12*IMGW-8, -11*IMGW-8, -11*IMGW-9, -10*IMGW-10, -10*IMGW-11, -9*IMGW-11, -8*IMGW-12, 
		-7*IMGW-12, -7*IMGW-13, -6*IMGW-13, -5*IMGW-14, -4*IMGW-14, -3*IMGW-14, -3*IMGW-15, 
		-2*IMGW-15, -1*IMGW-15
	};
	// Բ��100������Բ�����ߵĽǶȣ�Բ��ˮƽ���ཻ��ߵĽ���Ϊ��ʼ�㣩
	sint32    OriR15[100] = {
		180, 176, 172, 168, 165, 164, 160, 156, 151, 148, 146, 143, 137, 135, 
		132, 126, 123, 121, 118, 113, 109, 105, 104, 101, 97, 93, 90, 86, 
		82, 78, 77, 74, 70, 65, 61, 59, 56, 50, 47, 45, 39, 36, 
		33, 30, 24, 21, 17, 15, 12, 8, 4, 0, 355, 351, 347, 347, 
		342, 338, 333, 329, 327, 323, 318, 314, 311, 306, 302, 300, 296, 291, 
		287, 282, 282, 278, 274, 269, 265, 261, 257, 254, 252, 248, 245, 239, 
		236, 233, 230, 224, 222, 219, 213, 210, 208, 204, 199, 195, 192, 191, 
		187, 183
	};

	core = (VF_MINUTIAPTR)(g_lpMemory + sizeof(VF_FEATURE));
	delta = (VF_MINUTIAPTR)(g_lpMemory + sizeof(VF_FEATURE) + 
								sizeof(VF_MINUTIA)*5);

  // ������8�������ϵ�8����ĵ�ַƫ��
	for(i = 0; i < 8; i++)
	{
		DSite8[0][i] = i+1;
		DSite8[1][i] = (i+1)*(IMGW + 1);
		DSite8[2][i] = (i+1)*(IMGW);
		DSite8[3][i] = (i+1)*(IMGW - 1);
		DSite8[4][i] = -(i+1);
		DSite8[5][i] = -(i+1)*(IMGW + 1);
		DSite8[6][i] = -(i+1)*(IMGW);
		DSite8[7][i] = (i+1)*(-IMGW + 1);
	}

	CoreNum = 0;
	DeltaNum = 0;
	
	flag2 = true;
	flag3 = true;

	temp = VF_BW*IMGW;
	for(y = VF_BW; y <= IMGH-VF_BW; y++)
	{
		for(x = VF_BW; x <= IMGW-VF_BW; x++)
		{
			// ����Ƿ�Ϊ��Ե����
			lpDir = g_lpBlockOrient + (y/VF_BW)*BOW + x/VF_BW;
			if(*lpDir == 255)
			{
				continue;
			}
			flag = true;
			for(i = 0; i < 8; i++)
			{
				if(*(lpDir + DSiteR1[i]) == 255)
				{
					flag = false;
					break;
				}				
			}
			if(!flag)
			{
				continue;
			}

			// ����Ƿ�õ㷽��仯���ң��仯���Ҳ��п����������
			asum = 0;
			for(i = 0; i < 8; i++)
			{
				ad = GetJiajiao((sint32)(*(lpDir+DSiteR1[(i+1)%8])),
								(sint32)(*(lpDir+DSiteR1[i])));
				asum += ad;
			}
			if(abs(asum - 180) > 60)
			{
				continue;
			}
			
			// ���8�˸����������ϵ�8�����Ƿ�Ϊ��Ч������
			lpDir = g_lpOrient + temp + x;
			if(*lpDir == 255)
			{
				continue;
			}
			flag = true;
			for(i = 0; i < 8 && flag; i++)
			{
				for(j = 0; j < 8 && flag; j++)
				{
					if(*(lpDir + DSite8[i][j]) == 255)
					{
						flag = false;
						break;
					}
				}
			}
			if(!flag)
			{
				continue;
			}
			// ���ڶ�Ȧ�ϵļнǺ��Ƿ�Ƚϴ��������������ֵӦ�ýϴ�
			asum = 0;
			for(i = 0; i < 8; i++)
			{
				asum += GetJiajiao((sint32)(*(lpDir+DSite8[(i+1)%8][2])),
					(sint32)(*(lpDir+DSite8[i][2])));
			}

			if(asum < 90)
			{
				continue;
			}
			
			// ��7��Ȧ��8�������ϵĽǶȱ仯�͡����ĵ�Ƕȱ仯��Ӧ��Ϊ180�����ǵ�Ϊ-180
			for(i = 0; i < 8; i++)
			{
				DAngleSum[i] = 0;
			}
			for(j = 1; j < 8; j++)
			{
				for(i = 0; i < 8; i++)
				{
					ad = AngleSub((sint32)(*(lpDir+DSite8[(i+1)%8][j])),
									(sint32)(*(lpDir+DSite8[i][j])));
					if(ad > 90 || ad < -90)
					{
						continue;
					}

					DAngleSum[j] += ad;
				}
			}
			// �ж��Ƿ�Ϊ���ĵ㡣Ҫ��7��Ȧ�ϵĽǶȱ仯�Ͷ���180
			flag = true;
			for(i = 1; i < 8; i++)
			{
				if(DAngleSum[i] != 180)
				{
					flag = false;
					break;
				}
			}
			//�ǵĻ����¼����������
			if(flag && flag2)
			{
				if(CoreNum == 0)
				{
					core[CoreNum].x = x;
					core[CoreNum].y = y;
					core[CoreNum].Type = 1;
					CoreNum++;
				}
				else
				{
					// �����С�������Ƿ������ĵ㣬�еĻ�������������ƽ����Ϊ���ĵ�����
					fg = true;
					for(i = 0; i < CoreNum; i++)
					{
						dis = sqrt((double)((core[i].x/core[i].Type - x)*
							(core[i].x/core[i].Type - x) +
							(core[i].y/core[i].Type - y)*
							(core[i].y/core[i].Type - y)));
						if(dis < 8)
						{
							core[i].x += x;
							core[i].y += y;
							core[i].Type++;
							fg = false;
							break;
						}
					}
					if(fg)
					{
						core[CoreNum].x = x;
						core[CoreNum].y = y;
						core[CoreNum].Type = 1;
						CoreNum++;
						if(CoreNum > VF_MAXCORENUM)
						{
							flag2 = false;
						}

					}
				}
				continue;
			}
			// �ж��Ƿ�Ϊ���ǵ㡣Ҫ��7��Ȧ�ϵĽǶȱ仯�Ͷ���-180

			flag = true;
			for(i = 1; i < 8; i++)
			{
				if(DAngleSum[i] != -180)
				{
					flag = false;
					break;
				}
			}
			if(flag && flag3)
			{
				//*lpTemp = 255;
				if(DeltaNum == 0)
				{
					delta[DeltaNum].x = x;
					delta[DeltaNum].y = y;
					delta[DeltaNum].Type = 1;
					DeltaNum++;
				}
				else
				{
					// �����С�������Ƿ������ǵ㣬�еĻ�������������ƽ����Ϊ���ǵ�����
					fg = true;
					for(i = 0; i < DeltaNum; i++)
					{
						dis = sqrt((double)((delta[i].x/delta[i].Type - x)*
							(delta[i].x/delta[i].Type - x) +
							(delta[i].y/delta[i].Type - y)*
							(delta[i].y/delta[i].Type - y)));
						if(dis < 8)
						{
							delta[i].x += x;
							delta[i].y += y;
							delta[i].Type++;
							fg = false;
							break;
						}
					}
					if(fg)
					{
						delta[DeltaNum].x = x;
						delta[DeltaNum].y = y;
						delta[DeltaNum].Type = 1;
						DeltaNum++;
						if(DeltaNum > VF_MAXDELTANUM)
						{
							flag3 = false;
						}

					}
				}
			}

		}
		temp += IMGW;
	}
	// ���ĵ�̫����ȥ�������
	if(DeltaNum > VF_MAXDELTANUM)
	{
		DeltaNum = VF_MAXDELTANUM;
	}

	// ���ǵ�̫����ȥ�������
	if(CoreNum > VF_MAXCORENUM)
	{
		CoreNum = VF_MAXCORENUM;
	}
	
	// �����ĵ㷽��. Բ����Բ�����е��������õ㷽��н���С�������ĵ㷽��Ϊ�õ㷽��
	for(i = 0; i < CoreNum; i++)
	{
		core[i].x /= core[i].Type;
		core[i].y /= core[i].Type;
		core[i].Type = VF_MINUTIA_CORE;
		lpCore = g_lpOrient + core[i].y*IMGW + core[i].x;
		mina = 360;
		for(j = 0; j < 100; j++)
		{
			lpDir = lpCore + SiteR15[j];
			temp = OriR15[j];
			if(temp >= 180)
				temp -=180;
			temp = GetJiajiao(temp, *lpDir);
			if(temp < mina)
			{
				mina = temp;
				minindex = j;
			}
		}
		// �н���СֵҪС��6��
		if(mina < 6)
		{
			lpDir = lpCore + SiteR15[minindex];
			if(OriR15[minindex] >= 180)
				core[i].Direction = (*lpDir + 180);
			else
				core[i].Direction = *lpDir;
		}
		else
		{
			core[i].Direction = 400;
		}
	}

	for(i = 0; i < DeltaNum; i++)
	{
		delta[i].x /= delta[i].Type;
		delta[i].y /= delta[i].Type;
		delta[i].Type = VF_MINUTIA_DELTA;
	}

	g_lpFeature->CoreNum = (uint8)CoreNum;
	g_lpFeature->DeltaNum = (uint8)DeltaNum;

	memcpy((void *)g_lpFeature->CoreArr, (void *)&core[0], CoreNum*sizeof(VF_MINUTIA));

	memcpy((void *)g_lpFeature->DeltaArr, (void *)&delta[0], DeltaNum*sizeof(VF_MINUTIA));

}


*/

#endif