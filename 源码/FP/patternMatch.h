#ifndef	__VFPATTERNMATCH_H__
#define __VFPATTERNMATCH_H__




#include "StdAfx.h"
#include <vector>
using namespace std;

#define CENTRALRADIUS 60


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
	char	flagA[MAXMINUTIANUM];	// 标记lpAlignFeature中特征点是否已经有匹配对象
	char	flagT[MAXMINUTIANUM];	// 标记lpTemplate中特征点是否已经有匹配对象
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
			// 此表明dis,angle越大，分数越小
			score += (10-angle);
			score += (10-dis);
			// 匹配特征点数加一
			matchNum++;
			
			// 如果是快速比对模式
			if(matchMode == VF_MATCHMODE_IDENTIFY && matchNum >= 8)
			{
				// 计算相似度
				s = 4 * score * matchNum * MAXMINUTIANUM / ((num1 + num2) * ( num1 + num2));
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
	s = 4 * score * matchNum * MAXMINUTIANUM / ((num1+num2)*(num1+num2));

	lpMatchResult->MMCount = matchNum;
	lpMatchResult->Rotation = 0;
	lpMatchResult->Similarity = s;
	lpMatchResult->TransX = 0;
	lpMatchResult->TransX = 0;

}



void coreMatch(FEATUREPTR lpFeature, FEATUREPTR lpTemplate, 
				  PMATCHRESULT lpMatchResult, VF_FLAG matchMode, int n, int m)
{
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

	FEATURE	alignFeature;	// 对齐后的指纹特征

	// 位置偏移
	int transx = (lpTemplate->MinutiaArr[n].x - lpFeature->MinutiaArr[m].x);
	int transy = (lpTemplate->MinutiaArr[n].y - lpFeature->MinutiaArr[m].y);


	for(int i = 0; i < lpFeature->MinutiaNum; i++)
	{
		for(int j = 0; j < lpTemplate->MinutiaNum; j++)
		{
			alignFeature.MinutiaNum = 0;
									
			if (lpFeature->MinutiaArr[i].Type == VF_MINUTIA_CORE || lpTemplate->MinutiaArr[j].Type == VF_MINUTIA_CORE) continue;
			if (lpFeature->MinutiaArr[i].Type == VF_MINUTIA_DELTA || lpTemplate->MinutiaArr[j].Type == VF_MINUTIA_DELTA) continue;
			
			int rotation = GetAngle(lpTemplate->MinutiaArr[j].x, lpTemplate->MinutiaArr[j].y, 
				lpFeature->MinutiaArr[i].x, lpFeature->MinutiaArr[i].y);
			
			align(lpFeature, &alignFeature, &lpFeature->MinutiaArr[i], rotation, transx, transy);			


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



void deltaMatch(FEATUREPTR lpFeature, FEATUREPTR lpTemplate, 
				  PMATCHRESULT lpMatchResult, VF_FLAG matchMode, vector<int>& n_delta, vector<int>& m_delta)
{


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

	FEATURE	alignFeature;	// 对齐后的指纹特征
	int n, m;

	for(int nn = 0; nn < n_delta.size(); nn++)
		for(int mm = 0; mm < m_delta.size(); mm++)
		{
	
			n = (int)n_delta[nn];
			m = (int)m_delta[mm];

			// 位置偏移
			int transx = (lpTemplate->MinutiaArr[n].x - lpFeature->MinutiaArr[m].x);
			int transy = (lpTemplate->MinutiaArr[n].y - lpFeature->MinutiaArr[m].y);


			for(int i = 0; i < lpFeature->MinutiaNum; i++)
			{
				for(int j = 0; j < lpTemplate->MinutiaNum; j++)
				{
					alignFeature.MinutiaNum = 0;
											
					if (lpFeature->MinutiaArr[i].Type == VF_MINUTIA_CORE || lpTemplate->MinutiaArr[j].Type == VF_MINUTIA_CORE) continue;
					if (lpFeature->MinutiaArr[i].Type == VF_MINUTIA_DELTA || lpTemplate->MinutiaArr[j].Type == VF_MINUTIA_DELTA) continue;

					
					int rotation = GetAngle(lpFeature->MinutiaArr[i].x, lpFeature->MinutiaArr[i].y, 
								lpTemplate->MinutiaArr[j].x, lpTemplate->MinutiaArr[j].y);
					
					align(lpFeature, &alignFeature, &lpFeature->MinutiaArr[i], rotation, transx, transy);			


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
		}
	

	//最终比对结果
	*lpMatchResult = alignMax;

}


int dist(int x0, int y0, int x1, int y1)
{
	return (int)sqrt((float)(x0 - x1) *(float) (x0 - x1) +(float) (y0 - y1) *(float) (y0 - y1));
}

void centralMatch(FEATUREPTR lpFeature, FEATUREPTR lpTemplate, PMATCHRESULT lpMatchResult, VF_FLAG matchMode)
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

	int nx = 0, ny = 0;
	for(n = 0; n < lpTemplate->MinutiaNum; n++) 
	{
		nx += lpTemplate->MinutiaArr[n].x;
		ny += lpTemplate->MinutiaArr[n].y;
	}

	nx = nx / lpTemplate->MinutiaNum;
	ny = ny / lpTemplate->MinutiaNum;


	int mx = 0, my = 0;
	for(m = 0; m < lpFeature->MinutiaNum; m++)
	{
		mx += lpFeature->MinutiaArr[m].x;
		my += lpFeature->MinutiaArr[m].y;
	}

	mx = mx / lpFeature->MinutiaNum;
	my = my / lpFeature->MinutiaNum;

	int Counter = 0;


	// 对相同类型的指纹特征两两作为同一个指纹特征进行对齐比对
	for(n = 0; n < lpTemplate->MinutiaNum; n++)
	{
		if(dist(nx, ny, lpTemplate->MinutiaArr[n].x, lpTemplate->MinutiaArr[n].y) > CENTRALRADIUS) continue;

		for(m = 0; m < lpFeature->MinutiaNum; m++)
		{
			// 不同类型则不比对
			if(lpFeature->MinutiaArr[m].Type != lpTemplate->MinutiaArr[n].Type)
				continue;

		
			if(dist(mx, my, lpFeature->MinutiaArr[m].x, lpFeature->MinutiaArr[m].y) > CENTRALRADIUS) continue;

			Counter++;

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

void  patternMatch(FEATUREPTR lpFeature, FEATUREPTR lpTemplate, PMATCHRESULT lpMatchResult, VF_FLAG matchMode)
{

	vector<int> n_core;
	vector<int> m_core;

	vector<int> n_delta;
	vector<int> m_delta;
	
	int n, m;

	for(n = 0; n < lpFeature->MinutiaNum; n++) {
		if (lpFeature->MinutiaArr[n].Type == VF_MINUTIA_CORE) {
			n_core.push_back(n);
		}

		/*if (lpFeature->MinutiaArr[n].Type == VF_MINUTIA_DELTA) {
			n_delta.push_back(n);
		}*/
	}

	for(m = 0; m < lpTemplate->MinutiaNum; m++) {
		if (lpTemplate->MinutiaArr[m].Type == VF_MINUTIA_CORE) {
			m_core.push_back(m);
		}

		/*if (lpTemplate->MinutiaArr[m].Type == VF_MINUTIA_DELTA) {
			m_delta.push_back(m);
		}*/
	}

	
	

	if (n_core.size() > 0 && m_core.size() > 0) 
	{
		for(int i = 0; i < n_core.size(); i++)
			for(int j = 0; j < m_core.size(); j++)
			{
				n = (int)n_core[i];

				m = (int)m_core[j];

				coreMatch(lpFeature, lpTemplate, lpMatchResult, matchMode, n, m);

				// 如果是快速比对模式，则相似度达到一定程度则退出
				if(matchMode == VF_MATCHMODE_IDENTIFY && lpMatchResult->MMCount >= 8)
				{
					if(lpMatchResult->Similarity > 100)
					{				
						return;
					}
				}
			}
	}

	if (n_delta.size() > 0 && m_delta.size() > 0) 
	{
		deltaMatch(lpFeature, lpTemplate, lpMatchResult, matchMode, n_delta, m_delta);

		// 如果是快速比对模式，则相似度达到一定程度则退出
		if(matchMode == VF_MATCHMODE_IDENTIFY && lpMatchResult->MMCount >= 8)
		{
			if(lpMatchResult->Similarity > 100)
			{				
				return;
			}
		}
	} 


	centralMatch(lpFeature, lpTemplate, lpMatchResult, matchMode);
	if(matchMode == VF_MATCHMODE_IDENTIFY && lpMatchResult->MMCount >= 8)
	{
		if(lpMatchResult->Similarity > 100)
		{				
			return;
		}
	}

		
	globalMatch(lpFeature, lpTemplate, lpMatchResult, matchMode);
	
}

#endif 
