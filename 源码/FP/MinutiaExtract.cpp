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
	int  r = 16;		// 允许特征点靠近前景边缘的最近距离
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
	// 临时特征点数组
	MINUTIA     tempFork[MAXRAWMINUTIANUM];
	MINUTIA     tempEnd[MAXRAWMINUTIANUM];
	MINUTIA     ForkArr[MAXRAWMINUTIANUM];
	MINUTIA     EndArr[MAXRAWMINUTIANUM];
	// 某点为圆心，半径为5的圆上所有点的地址偏移
	int	SiteR5[28] = {
		-5, IMGW-5, 2*IMGW-5, 3*IMGW-4, 4*IMGW-3, 5*IMGW-2, 5*IMGW-1, 5*IMGW,
		5*IMGW+1, 5*IMGW+2, 4*IMGW+3, 3*IMGW+4, 2*IMGW+5, IMGW+5, 5, -IMGW+5,
		-2*IMGW+5, -3*IMGW+4, -4*IMGW+3, -5*IMGW+2, -5*IMGW+1, -5*IMGW,
		-5*IMGW-1, -5*IMGW-2, -4*IMGW-3, -3*IMGW-4, -2*IMGW-5, -IMGW-5
	}; 
	// 某点周围8个点的地址偏移
	int	SiteU8[8] = {IMGW-1, IMGW, IMGW+1, 1, -IMGW+1, -IMGW, -IMGW-1, -1};

	// 初始化临时特征点数组
	memset((void *)&tempFork[0], 0, MAXRAWMINUTIANUM*sizeof(MINUTIA));
	memset((void *)&tempEnd[0], 0, MAXRAWMINUTIANUM*sizeof(MINUTIA));
	memset((void *)&ForkArr[0], 0, MAXRAWMINUTIANUM*sizeof(MINUTIA));
	memset((void *)&EndArr[0], 0, MAXRAWMINUTIANUM*sizeof(MINUTIA));


	bool    bGood = false;	// 标记阈值是否调整好，使特征点数目在允许范围内
	int  densD = 7;	// 允许两个特征点间最近的距离
	int  loopnum;	// 循环次数
	int  trilen = 16;
	// 第一遍寻找所有端点和叉点	
	ForkNum = 0; 
	EndNum = 0; 
	temp = 17*IMGW;
	for(y = 17; y < IMGH-17; y++)
	{
		for(x = 17; x < IMGW-17; x++)
		{
			lpNow = g_lpOrgFinger + temp + x;
			lpOri = g_lpOrient + y*IMGW + x;
			// 不是黑点则不考虑
			if(*lpNow != 0)
			{
				continue;
			}
			// 是叉点
			if(IsFork(lpNow))
			{
				// 检查是否靠近边缘, 靠近边缘则不考虑
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
				// 检查该点周围方向场是否变化剧烈，变化剧烈则不考虑，因为很可能是虚假点
				sum = 0;
				for(i = 0; i < 28; i++)
				{
					sum += GetJiajiao(*(lpOri+SiteR5[(i+1)%28]), *(lpOri+SiteR5[i]));
				}
				if(sum > 96)
					flag = false;
				
				// 达到所有要求则记录下来该点
				if(flag)
				{
					ForkArr[ForkNum].x = x;
					ForkArr[ForkNum].y = y;
					ForkNum++;
					// 如果总数已经超过允许最大数目，则返回错误
					if(ForkNum >= MAXRAWMINUTIANUM)
					{
						ForkNum = 0;
						return 1;
					}

				}

			}
			else if(IsEnd(lpNow))	// 如果是端点
			{
				// 检查是否靠近边缘, 靠近边缘则不考虑
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

				// 检查该点周围方向场是否变化剧烈，变化剧烈则不考虑，因为很可能是虚假点
				sum = 0;
				for(i = 0; i < 28; i++)
				{
					sum += GetJiajiao(*(lpOri+SiteR5[(i+1)%28]), *(lpOri+SiteR5[i]));
				}
				if(sum > 96)
					flag = false;

				// 达到所有要求则记录下来该点
				if(flag)
				{
					EndArr[EndNum].x = x;
					EndArr[EndNum].y = y;
					EndNum++;
					// 如果总数已经超过允许最大数目，则返回错误
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

	// 初始化临时缓冲区，准备循环调整两个特征点间允许的最小距离，使之增大到一个合适的值
	// 因为如果特征点非常密集的地方很有可能是质量很差的地方
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

	// 循环调整
	bGood = false;
	loopnum = 0;	// 调整的次数
	while(!bGood && loopnum < 32) // 最多调整32次
	{
		loopnum++;	// 调整次数加一
		// 得到新的特征点数组
		for(i = 0; i < MAXRAWMINUTIANUM; i++)
		{
			EndArr[i] = tempEnd[i];
		}
		for(i = 0; i < MAXRAWMINUTIANUM; i++)
		{
			ForkArr[i] = tempFork[i];
		}
		// 新的特征点数
		ForkNum = tempForkNum;	// 叉点
		EndNum = tempEndNum;	// 端点

		// 去掉虚假的端点
		bGood = true;
		for(i = 0; i < EndNum-1; i++)
		{
			flag = false;
			for(j = i+1; j < EndNum; j++)
			{
				// 求两个端点的距离
				d = sqrt((double)((EndArr[i].x-EndArr[j].x)*(EndArr[i].x-EndArr[j].x) +
					(EndArr[i].y-EndArr[j].y)*(EndArr[i].y-EndArr[j].y)));
				// 距离足够大则检查下一个端点
				if(d > dGate && d > densD)
				{
					continue;
				}
				// 距离太小，则将其坐标置为（0，0）
				if(d <= densD)
				{
					EndArr[j].x = 0;
					EndArr[j].y = 0;
					flag = true;
					continue;
				}
				// 求第一个端点所在纹线的角度
				lpNow = g_lpOrgFinger + EndArr[i].y*IMGW + EndArr[i].x;
				if(GetByDis(lpNow, &lpDis, 8) != 0)
				{
					break;
				}
				angle1 = GetAngle(EndArr[i].x, EndArr[i].y, PX(lpDis), PY(lpDis));
				
				// 求第二个端点所在纹线的角度
				lpNow = g_lpOrgFinger + EndArr[j].y*IMGW + EndArr[j].x;
				if(GetByDis(lpNow, &lpDis, 8) != 0)
				{
					continue;
				}
				angle2 = GetAngle(EndArr[j].x, EndArr[j].y, PX(lpDis), PY(lpDis));

				// 求两个角度间的距离
				angle3 = GetAngleDis(angle1, angle2);
				
				// 如果两个角度间成锐角，则不是虚假特征点
				if(angle3 > 270 || angle3 < 90)
				{
					continue;
				}

				// 求两个端点连线的角度
				angle3 = GetAngle(EndArr[i].x, EndArr[i].y, EndArr[j].x, EndArr[j].y);
				// 求第一个端点纹线与连线的夹角
				angle3 = GetAngleDis(angle1, angle3);
				// 如果夹角较大则不是虚假点
				if(angle3 < 150 || angle3 > 210)
				{
					continue;
				}

				// 求第二个端点纹线与连线的夹角
				angle4 = GetAngle(EndArr[j].x, EndArr[j].y, EndArr[i].x, EndArr[i].y);
				angle4 = GetAngleDis(angle2, angle4);
				
				// 如果夹角较大则不是虚假点
				if(angle4 < 150 || angle4 > 210)
				{
					continue;
				}
				// 否则表示这两个点是同一条纹线上的断裂处的两个端点,坐标置原点
				EndArr[j].x = 0;
				EndArr[j].y = 0;
				flag = true;
			}
			if(flag)// 表示这两个点是同一条纹线上的断裂处的两个端点,坐标置原点
			{
				EndArr[i].x = 0;
				EndArr[i].y = 0;
			}
		}

		// 统计新的端点数目
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
			// 如果端点数目还是过多，则需要调整densD
			if(j >= MAXMINUTIANUM)
			{
				bGood = false;
				break;
			}

		}
		// 调整densD
		if(!bGood)
		{
			densD++; // 加一
			continue;
		}

		// 将端点装配到g_Feature
		j = 0;
		for(i = 0; i < EndNum; i++)
		{
			if(EndArr[i].x == 0 || EndArr[i].y == 0)
			{
				continue;
			}
			// 沿纹线找到第8个点
			lpNow = g_lpOrgFinger + EndArr[i].y*IMGW + EndArr[i].x;
			if(GetByDis(lpNow, &lpDis, 8) != 0)
			{
				continue;
			}
			// 求该点与端点连线的角度
			angle1 = GetAngle(EndArr[i].x, EndArr[i].y, PX(lpDis), PY(lpDis));
			angle2 = *(g_lpOrient + EndArr[i].y*IMGW + EndArr[i].x);
			angle3 = GetAngleDis(angle1, angle2);
			// 如果成钝角，则表示该特征点方向与该点方向场方向相反
			if(angle3 > 90 && angle3 < 270)
			{
				g_Feature.MinutiaArr[j].Direction = angle2 + 180;
			}
			else // 如果成锐角，则表示该特征点方向与该点方向场方向相同
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

			g_Feature.MinutiaArr[j].x = EndArr[i].x;	//横坐标
			g_Feature.MinutiaArr[j].y = EndArr[i].y;	//纵坐标
			g_Feature.MinutiaArr[j].Type = VF_MINUTIA_END;	//类型

			j++;

		}

		EndNum = j;	// 端点数目


		// 去掉距离太近的叉点
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

		// 统计新的真正的叉点
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
			// 检查是否真正叉点
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
			// 特征点总数大于允许最大数目则要调整densD
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

		// 装配叉点到g_Feature
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
			// 找到叉点三个方向纹线上距离为8的点
			for(k = 0; k < 3; k++)
			{
				GetByDis(tempPtr[k+1], &disPtr[k], 8);
			}
			for(k = 0; k < 4; k++)
			{
				*tempPtr[k] = 0;
			}
			// 求该三个点两两间的距离
			d0 = sqrt((double)((PX(disPtr[1]) - PX(disPtr[2]))*(PX(disPtr[1]) - PX(disPtr[2])) +
				(PY(disPtr[1]) - PY(disPtr[2]))*(PY(disPtr[1]) - PY(disPtr[2]))));
			d1 = sqrt((double)((PX(disPtr[0]) - PX(disPtr[2]))*(PX(disPtr[0]) - PX(disPtr[2])) +
				(PY(disPtr[0]) - PY(disPtr[2]))*(PY(disPtr[0]) - PY(disPtr[2]))));
			d2 = sqrt((double)((PX(disPtr[1]) - PX(disPtr[0]))*(PX(disPtr[1]) - PX(disPtr[0])) +
				(PY(disPtr[1]) - PY(disPtr[0]))*(PY(disPtr[1]) - PY(disPtr[0]))));

			// 距离最短的边对应的是叉线方向
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
			// 求最短边中点与叉点连线的方向与该点方向场方向的夹角
			angle1 = GetAngle(ForkArr[i].x, ForkArr[i].y, (x11+x21)/2, (y11+y21)/2);
			angle2 = *(g_lpOrient + ForkArr[i].y*IMGW + ForkArr[i].x);
			angle3 = GetAngleDis(angle1, angle2);
			// 如果成钝角，则表示该特征点方向与该点方向场方向相反
			if(angle3 > 90 && angle3 < 270)
			{
				g_Feature.MinutiaArr[EndNum + j].Direction = angle2 + 180;
			}
			else // 如果成锐角，则表示该特征点方向与该点方向场方向相同
			{
				g_Feature.MinutiaArr[EndNum + j].Direction = angle2;
			}
			// 纪录以特征点为中心外接圆半径为trilen的正三角形三个顶点的方向
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

			g_Feature.MinutiaArr[EndNum + j].x = ForkArr[i].x; //横坐标
			g_Feature.MinutiaArr[EndNum + j].y = ForkArr[i].y; //纵坐标
			g_Feature.MinutiaArr[EndNum + j].Type = VF_MINUTIA_FORK; // 类型


			j++;
		}
		ForkNum = j; // 叉点数目


	}

	// 如果循环32次都不能得到理想结果，则表示图像质量差，返回错误
	if(loopnum >= 32)
	{
		EndNum = 0;
		ForkNum = 0;
		return 1;
	}
	// 特征点数目等于端点数加叉点数
	g_Feature.MinutiaNum = (BYTE)(EndNum+ForkNum);

	//  特征点数目太少则返回错误
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
//	lpFeature: [in] 要比对的第一个指纹特征指针
//	lpTemplate: [in] 要比对的第二个指纹特征指针
//	lpMatchResult: [out] 比对结果指针
//	matchMode: [in] 比对模式
/////////////////////////////////////////////////////////////////////////////
	
	MATCHRESULT globalMatchResult;
	int		score = 0;
	
	//全局比对
	globalMatch(lpFeature, lpTemplate, &globalMatchResult, matchMode);
	*lpMatchResult = globalMatchResult;
	//如果匹配特征点数较少，则对比对结果相似度进行修正
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
//	lpFeature: [in] 要比对的第一个指纹特征指针
//	lpTemplate: [in] 要比对的第二个指纹特征指针
//	lpMatchResult: [out] 比对结果指针
//	matchMode: [in] 比对模式
/////////////////////////////////////////////////////////////////////////////
	int	m, n, a1, a2;
	int	rotation;		// 旋转角度
	int	transx, transy;	// 位置偏移
	FEATURE	alignFeature;	// 对齐后的指纹特征
	MATCHRESULT alignMax;	// 相似度最大的比对结果
	MATCHRESULT globalMatchResult;	// 比对结果
	int  agate = 8;		// 三角拓扑结构角度误差
	int  num = 0;
	// 初始化最好的比对结果
	alignMax.Similarity = 0;
	alignMax.MMCount = 0;
	alignMax.Rotation = 0;
	alignMax.TransX = 0;
	alignMax.TransY = 0;

	// 对相同类型的指纹特征两两作为同一个指纹特征进行对齐比对
	for(n = 0; n < lpTemplate->MinutiaNum; n++)
	{
		for(m = 0; m < lpFeature->MinutiaNum; m++)
		{
			// 不同类型则不比对
			if(lpFeature->MinutiaArr[m].Type != lpTemplate->MinutiaArr[n].Type)
				continue;
			if(matchMode == VF_MATCHMODE_IDENTIFY)
			{
				// 特征点三角拓扑结构比对，相似则进行配准
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
			// 旋转角度
			rotation = GetAngleDis(lpFeature->MinutiaArr[m].Direction, 
									lpTemplate->MinutiaArr[n].Direction);
			// 位置偏移
			transx = (lpTemplate->MinutiaArr[n].x - lpFeature->MinutiaArr[m].x);
			transy = (lpTemplate->MinutiaArr[n].y - lpFeature->MinutiaArr[m].y);
			
			// 将lpFeature与lpTemplate对齐
			align(lpFeature, &alignFeature, &lpFeature->MinutiaArr[m],
					rotation, transx, transy);
			// 将两个对齐的指纹特征进行比对
			alignMatch(&alignFeature, lpTemplate, &globalMatchResult, matchMode);
			// 如果比对结果比最好的比对结果更好，则更新alignMax
			if(globalMatchResult.Similarity > alignMax.Similarity)
			{
				alignMax.MMCount = globalMatchResult.MMCount;
				alignMax.Similarity = globalMatchResult.Similarity;
				alignMax.Rotation = rotation;
				alignMax.TransX = transx;
				alignMax.TransY = transy;
				
				// 如果是快速比对模式，则相似度达到一定程度则退出
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
	
	//最终比对结果
	*lpMatchResult = alignMax;


}




//
//	align: 将指纹特征按一定的角度和位置偏移进行坐标变换
//
void align(FEATUREPTR lpFeature, FEATUREPTR lpAlignedFeature, MINUTIAPTR lpFeatureCore,
				  int rotation, int transx, int transy)
{
///////////////////////////////////////////////////////////////////////
//  lpFeature:	[in] 要变换的指纹特征
//  lpAlignedFeature:	[out] 进行坐标变换后的指纹特征
//  lpFeatureCore:	[in] 旋转变换的中心特征点
//  rotation:	[in] 旋转角度
//  transx:	[in] 水平偏移
//  transy: [in] 垂直偏移
///////////////////////////////////////////////////////////////////////

	int  i;
	int  x, y;
	int  cx, cy;
	double  rota, sinv, cosv;

	// 复制整个结构信息
	*lpAlignedFeature = *lpFeature;
	
	// 坐标转换的中心点坐标
	cx = lpFeatureCore->x;
	cy = lpFeatureCore->y;
	// 旋转的弧度
	rota = rotation/EPI;
	// 旋转弧度的sin值
	sinv = sin(rota);
	// 旋转弧度的cos值
	cosv = cos(rota);
	for(i = 0; i < lpFeature->MinutiaNum; i++)
	{
		x = lpFeature->MinutiaArr[i].x;
		y = lpFeature->MinutiaArr[i].y;
		//  坐标转换后的新坐标
		lpAlignedFeature->MinutiaArr[i].x = (int)(cx + cosv*(x-cx) - sinv*(y-cy) + transx + 0.5);
		lpAlignedFeature->MinutiaArr[i].y = (int)(cy + sinv*(x-cx) + cosv*(y-cy) + transy + 0.5);
		// 旋转后特征点的新方向
		lpAlignedFeature->MinutiaArr[i].Direction = (lpFeature->MinutiaArr[i].Direction + rotation) % 360;
	}

}

//
// alignMatch: 两个坐标系对齐的指纹特征进行比对
//
void alignMatch(FEATUREPTR lpAlignFeature, FEATUREPTR lpTemplate, 
				PMATCHRESULT lpMatchResult, VF_FLAG matchMode)
{
	int	i, j;
	BYTE	flagA[MAXMINUTIANUM];	// 标记lpAlignFeature中特征点是否已经有匹配对象
	BYTE	flagT[MAXMINUTIANUM];	// 标记lpTemplate中特征点是否已经有匹配对象
	int	x1, y1, x2, y2;
	int  dis, angle;
	int  score, matchNum, s;
	int	num1, num2;
	int  step = 1;
	num1 = lpAlignFeature->MinutiaNum;
	num2 = lpTemplate->MinutiaNum;
	// 标记清零
	memset(flagA, 0, MAXMINUTIANUM);
	memset(flagT, 0, MAXMINUTIANUM);

	score = 0;		// 总分清零
	matchNum = 0;	// 匹配特征点数清零



	// 相同类型特征点的匹配分数
	for(i = 0; i < lpTemplate->MinutiaNum; i++)
	{
		if(flagT[i])	// 是否已有匹配对象
			continue;
		for(j = 0; j < lpAlignFeature->MinutiaNum; j++)
		{
			if(flagA[j])	// 是否已有匹配对象
				continue;
			// 特征点类型是否相同
			if(lpTemplate->MinutiaArr[i].Type != lpAlignFeature->MinutiaArr[j].Type)
				continue;
			// 特征点方向夹角
			angle = AngleAbs360(lpTemplate->MinutiaArr[i].Direction, 
								lpAlignFeature->MinutiaArr[j].Direction);
			// 夹角>=10则不匹配
			if(angle >= 10)
				continue;
			x1 = lpTemplate->MinutiaArr[i].x;
			y1 = lpTemplate->MinutiaArr[i].y;
			x2 = lpAlignFeature->MinutiaArr[j].x;
			y2 = lpAlignFeature->MinutiaArr[j].y;
			// 水平距离>=10则不匹配
			if(abs(x1-x2) >= 10)
				continue;
			// 垂直距离>=10则不匹配
			if(abs(y1-y2) >= 10)
				continue;
			// 两特征点间的距离
			dis = DisTbl[abs(y1-y2)][abs(x1-x2)];
			// 距离>=10则不匹配
			if(dis >= 10)
				continue;
			// 对这两个特征点做标记，表示已经有匹配对象
			flagA[j] = 1;
			flagT[i] = 1;
			// 总分加上此两个特征点的匹配分数
			score += (10-angle);
			score += (10-dis);
			// 匹配特征点数加一
			matchNum++;
			
			// 如果是快速比对模式
			if(matchMode == VF_MATCHMODE_IDENTIFY && matchNum >= 8)
			{
				// 计算相似度
				s = 4* score * matchNum * MAXMINUTIANUM / ((num1+num2)*(num1+num2));
				if(s > 100)  // 相似度足够大则返回比对结果
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
		// 由于图像处理的误差导致可能端点处理成叉点或叉点处理成端点，假设概率为50%，计算
		// 此种情况的分数
		for(i = 0; i < lpTemplate->MinutiaNum; i++)
		{
			if(flagT[i])	// 是否已有匹配对象
				continue;
			for(j = 0; j < lpAlignFeature->MinutiaNum; j++)
			{
				if(flagA[j])	// 是否已有匹配对象
					continue;
				// 是否类型不同
				if(lpTemplate->MinutiaArr[i].Type == lpAlignFeature->MinutiaArr[j].Type)
					continue;
				// 特征点方向夹角
				angle = AngleAbs360(lpTemplate->MinutiaArr[i].Direction, 
									lpAlignFeature->MinutiaArr[j].Direction);
				// 夹角>=10则不匹配
				if(angle >= 10)
					continue;
				x1 = lpTemplate->MinutiaArr[i].x;
				y1 = lpTemplate->MinutiaArr[i].y;
				x2 = lpAlignFeature->MinutiaArr[j].x;
				y2 = lpAlignFeature->MinutiaArr[j].y;

				// 水平距离>=10则不匹配
				if(abs(x1-x2) >= 10)
					continue;
				// 垂直距离>=10则不匹配
				if(abs(y1-y2) >= 10)
					continue;
				// 两特征点间的距离
				dis = DisTbl[abs(y1-y2)][abs(x1-x2)];
				// 距离>=10则不匹配
				if(dis >= 10)
					continue;
				// 对这两个特征点做标记，表示已经有匹配对象
				flagA[j] = 1;
				flagT[i] = 1;
				// 总分加上此两个特征点的匹配分数
				score += ((10-angle)/2);
				score += ((10-dis)/2);
				// 匹配特征点数加一
				matchNum++;

			}
		}								
	}
	// 计算相似度，返回比对结果
	s = 4* score * matchNum * MAXMINUTIANUM / ((num1+num2)*(num1+num2));

	lpMatchResult->MMCount = matchNum;
	lpMatchResult->Rotation = 0;
	lpMatchResult->Similarity = s;
	lpMatchResult->TransX = 0;
	lpMatchResult->TransX = 0;

}*/