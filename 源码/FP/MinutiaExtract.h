// MinutiaExtract.h: interface for the MinutiaExtract class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MINUTIAEXTRACT_H__F76D271A_F5FD4_4DBA_B826_7B57F2574483__INCLUDED_)
#define AFX_MINUTIAEXTRACT_H__F76D271A_F5FD4_4DBA_B826_7B57F2574483__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "UtilityFunction.h"
#include "math.h"

// Ϊ��ʡʱ�䣬�̾��������������ò��
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

/*
void verify(FEATUREPTR lpFeature, FEATUREPTR lpTemplate, PMATCHRESULT lpMatchResult, VF_FLAG matchMode);
void globalMatch(FEATUREPTR lpFeature, FEATUREPTR lpTemplate, PMATCHRESULT lpMatchResult, VF_FLAG matchMode);
void alignMatch(FEATUREPTR lpAlignFeature, FEATUREPTR lpTemplate, PMATCHRESULT lpMatchResult, VF_FLAG matchMode);
void align(FEATUREPTR lpFeature, FEATUREPTR lpAlignedFeature, MINUTIAPTR lpFeatureCore, int rotation, int transx, int transy);
*/

int getOriChange(int angle1, int angle2, char flag)
{
	int  d;

	d = angle2 - angle1;
	if(flag >= 1)
	{
		if(d < 0)
		{
			d += 10;
		}

	}
	else if(flag <= -1)
	{
		if(d > 0)
		{
			d -= 10;
		}
	}
	
	return d;
}


int getSingular(BYTE *lpOrient, int Width, int Height, int *lpNum, PPOINT lpArr, char flag)
{
	int    x, y, i;
	int    num;
	int    sum1, sum2;
	int    d;
	unsigned char *pOriMap;

	int    oriV;
	int    a1, a2;
	DBLPOINT  singuArr[30];

	int    value;
	double dis;
	bool   bFound = false;
	bool   fg = false;
	int    D3[8][2] = {
		-1,-1, -1,0,  -1,1,  0,1,  1,1,  1,0,  1,-1,  0,-1
	};
	int    D5[12][2] = {
		-2,-1, -2,0,  -2,1, -1,2,  0,2,  1,2, 2,1,  2,0,  2,-1, 1,-2, 0,-2, -1,-2
	};
	int    D7[24][2] = {
		-3,-3, -3,-2, -3,-1, -3,0, -3,1, -3,2, -3,3, -2,3, -1,3, 0,3, 1,3, 2,3, 3,3, 3,2, 3,1, 3,0,
			3,-1, 3,-2, 3,-3, 2,-3, 1,-3, 0,-3, -1,-3, -2,-3
	};
	

	num = 0;

	memset(singuArr, 0, sizeof(singuArr));				//��ʼ����������飬����

	for(y = 3; y < Height-3; y++)
	{
		for(x = 3; x < Width-3; x++)					//��������ͼ
		{
			pOriMap = lpOrient + y*Width + x;			//���ĳ��ķ���ָ��
			oriV = *pOriMap;							//��ȡĳ��ķ��򳡵�ֵ

			if(oriV == 255)								//���Ǳ���������������һ��ѭ��
			{
				continue;
			}

			fg = false;									
			for(i = 0; i < 24; i++)
			{
				if(pOriMap[D7[i][1]*Width + D7[i][0]]==255)
				{
					fg = true;
					break;
				}
			}
			if(fg)
			{
				continue;
			}

			sum1 = 0;
			for(i = 0; i < 8; i++)
			{
				a1 = pOriMap[D3[i][1]*Width + D3[i][0]]/24;
				a2 = pOriMap[D3[(i+1)%8][1]*Width + D3[(i+1)%8][0]]/24;
				d = getOriChange(a1, a2, flag);
				
				if(abs(d) > 5)
				{
					break;
				}
				
				sum1 += d;
			}
			sum2 = 0;
			for(i = 0; i < 12; i++)
			{
				a1 = pOriMap[D5[i][1]*Width + D5[i][0]]/24;
				a2 = pOriMap[D5[(i+1)%12][1]*Width + D5[(i+1)%12][0]]/24;
				d = getOriChange(a1, a2, flag);
				
				if(abs(d) > 5)
				{
					break;
				}
				
				sum2 += d;
			}
		    
			if(flag == -1)									//���ĵ�
			{
				value = -10;
			}
			else if(flag == 1)								//���ǵ�
			{
				value = 10;
			}
			if(sum2 == value && sum1 == value)
			{
				bFound = false;
				for(i = 0; i < num; i++)
				{
					dis = sqrt((double)((x - singuArr[i].x)*(x - singuArr[i].x) +
						(y - singuArr[i].y)*(y - singuArr[i].y)));
					if(dis < 4)
					{
						singuArr[i].x = (singuArr[i].x + x)/2.0;
						singuArr[i].y = (singuArr[i].y + y)/2.0;
						bFound = true;
						break;
					}

				}
				if(!bFound)
				{
					singuArr[num].x = x;
					singuArr[num].y = y;
					num++;
					if(num >= MAX_SINGULARYNUM)				//���������������������ƣ���ֹͣѰ�������
					{
						goto next;
					}
				}

			}
		}
	}

next:

	*lpNum = num;											//�����ĸ���
	for(i = 0; i < num; i++)								//���ҵ�������������������
	{
		lpArr[i].x = (long)singuArr[i].x;
		lpArr[i].y = (long)singuArr[i].y;
	}

	return 0;
}


int getSingularMinutia(BYTE *lpOrient, int Width, int Height, char flag, LPVOID feature)
{
	int    x, y, i;
	int    num;
	int    sum1, sum2;
	int    d;
	unsigned char *pOriMap;

	int    oriV;
	int    a1, a2;
	DBLPOINT  singuArr[30];

	int    value;
	double dis;
	bool   bFound = false;
	bool   fg = false;
	int    D3[8][2] = {
		-1,-1, -1,0,  -1,1,  0,1,  1,1,  1,0,  1,-1,  0,-1
	};
	int    D5[12][2] = {
		-2,-1, -2,0,  -2,1, -1,2,  0,2,  1,2, 2,1,  2,0,  2,-1, 1,-2, 0,-2, -1,-2
	};
	int    D7[24][2] = {
		-3,-3, -3,-2, -3,-1, -3,0, -3,1, -3,2, -3,3, -2,3, -1,3, 0,3, 1,3, 2,3, 3,3, 3,2, 3,1, 3,0,
			3,-1, 3,-2, 3,-3, 2,-3, 1,-3, 0,-3, -1,-3, -2,-3
	};

	FEATURE *g_Feature = (FEATURE*)(feature);
	

	num = 0;

	memset(singuArr, 0, sizeof(singuArr));				//��ʼ����������飬����

	for(y = 3; y < Height-3; y++)
	{
		for(x = 3; x < Width-3; x++)					//��������ͼ
		{
			pOriMap = lpOrient + y*Width + x;			//���ĳ��ķ���ָ��
			oriV = *pOriMap;							//��ȡĳ��ķ��򳡵�ֵ

			if(oriV == 255)								//���Ǳ���������������һ��ѭ��
			{
				continue;
			}

			fg = false;									
			for(i = 0; i < 24; i++)
			{
				if(pOriMap[D7[i][1]*Width + D7[i][0]]==255)
				{
					fg = true;
					break;
				}
			}
			if(fg)
			{
				continue;
			}

			sum1 = 0;
			for(i = 0; i < 8; i++)
			{
				a1 = pOriMap[D3[i][1]*Width + D3[i][0]]/24;
				a2 = pOriMap[D3[(i+1)%8][1]*Width + D3[(i+1)%8][0]]/24;
				d = getOriChange(a1, a2, flag);
				
				if(abs(d) > 5)
				{
					break;
				}
				
				sum1 += d;
			}
			sum2 = 0;
			for(i = 0; i < 12; i++)
			{
				a1 = pOriMap[D5[i][1]*Width + D5[i][0]]/24;
				a2 = pOriMap[D5[(i+1)%12][1]*Width + D5[(i+1)%12][0]]/24;
				d = getOriChange(a1, a2, flag);
				
				if(abs(d) > 5)
				{
					break;
				}
				
				sum2 += d;
			}
		    
			if(flag == -1)									//���ĵ�
			{
				value = -10;
			}
			else if(flag == 1)								//���ǵ�
			{
				value = 10;
			}
			if(sum2 == value && sum1 == value)
			{
				bFound = false;
				for(i = 0; i < num; i++)
				{
					dis = sqrt((double)((x - singuArr[i].x)*(x - singuArr[i].x) +
						(y - singuArr[i].y)*(y - singuArr[i].y)));
					if(dis < 4)
					{
						singuArr[i].x = (singuArr[i].x + x)/2.0;
						singuArr[i].y = (singuArr[i].y + y)/2.0;
						bFound = true;
						break;
					}

				}
				if(!bFound)
				{
					singuArr[num].x = x;
					singuArr[num].y = y;
					num++;
					if(num >= MAX_SINGULARYNUM)				//���������������������ƣ���ֹͣѰ�������
					{
						goto next;
					}
				}

			}
		}
	}

next:

	int  trilen = 16;
	int N = 0;											//�����ĸ���
	for(i = 0; i < num; i++)								//���ҵ�������������������
	{
		//lpArr[*lpNum + i].x = (long)singuArr[i].x;
		//lpArr[*lpNum + i].y = (long)singuArr[i].y;	

		int x = (long)singuArr[i].x;
		int y = (long)singuArr[i].y;
		
		//if (*(g_lpDivide + y * Width + x) == 255) continue;

		if (((x + 0.0) / Width < 0.1) || ((x + 0.0) / Width > 0.9)) 
			continue;

		if (((y + 0.0) / Height < 0.1 ) || ((y + 0.0) / Height > 0.9))
			continue;

		

		g_Feature->MinutiaArr[g_Feature->MinutiaNum + N].x = x;
		g_Feature->MinutiaArr[g_Feature->MinutiaNum + N].y = y;

		g_Feature->MinutiaArr[g_Feature->MinutiaNum + N].Type = (flag == -1) ? VF_MINUTIA_CORE : VF_MINUTIA_DELTA;				
		g_Feature->MinutiaArr[g_Feature->MinutiaNum + N].Direction = 0;				

		N++;
	}

	g_Feature->MinutiaNum += N;

	return 0;
}




int IMGW;

int IMGH;

BYTE *g_lpOrgFinger;

void setup(int W, int H, BYTE *lpData)
{
	IMGW = W;
	IMGH = H;
	g_lpOrgFinger = lpData;
}
//
//	IsFork: �ж�ĳ���Ƿ�Ϊ���
//
bool   IsFork(BYTE *lpNow)
{
/////////////////////////////////////////////////////////////////////////////
//	lpNow : [in] ��ǰ��ĵ�ַ
/////////////////////////////////////////////////////////////////////////////
	int i, sum;
	// ĳ����Χ8����ĵ�ַƫ��
	int	SiteD8[8] = {IMGW-1, IMGW, IMGW+1, 1, -IMGW+1, -IMGW, -IMGW-1, -1};
	
	// 8������������������Ĳ�ľ���ֵ�ĺ����Ϊ6*255��Ϊ���
	sum = 0;
	for(i = 0; i < 8; i++)
	{
		sum += abs(*(lpNow + SiteD8[(i+1)%8]) - *(lpNow + SiteD8[i]));
	}
	if(sum == 255*6)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//
//	IsEnd: �ж�ĳ���Ƿ�Ϊ�˵�
//
bool   IsEnd(BYTE *lpNow)
{
/////////////////////////////////////////////////////////////////////////////
//	lpNow : [in] ��ǰ��ĵ�ַ
/////////////////////////////////////////////////////////////////////////////
	int i, sum;
	// ĳ����Χ8����ĵ�ַƫ��
	int	SiteD8[8] = {IMGW-1, IMGW, IMGW+1, 1, -IMGW+1, -IMGW, -IMGW-1, -1};

	// 8������������������Ĳ�ľ���ֵ�ĺ����Ϊ2*255��Ϊ�˵�
	sum = 0;
	for(i = 0; i < 8; i++)
	{
		sum += abs(*(lpNow + SiteD8[(i+1)%8]) - *(lpNow + SiteD8[i]));
	}
	if(sum == 255*2)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//
//	GetNext: �������ϸ��ݵ�ǰ���ǰ�����ַ�ҵ���һ����ĵ�ַ
//
int  GetNext(BYTE *lpNow, BYTE *lpLast, BYTE **lppNext)
{
/////////////////////////////////////////////////////////////////////////////
//	lpNow : [in] ��ǰ��ĵ�ַ
//	lpLast : [in] ǰ����ĵ�ַ
//	lppNext : [out] ��һ����ĵ�ַ��ָ��
/////////////////////////////////////////////////////////////////////////////
	int  i;
	BYTE   *lpSrc = NULL;
	int  n;
	BYTE   *Temp[8];	// ĳ����Χ8����ĵ�ַ
	int	SiteD8[8] = {IMGW-1, IMGW, IMGW+1, 1, -IMGW+1, -IMGW, -IMGW-1, -1};

	*lppNext = NULL;
	
	// �ҵ���ǰ����Χ���Ǻڵ㲢�Ҳ���ǰ����ĺڵ㣬���浽����
	n = 0;
	for(i = 0; i < 8; i++)
	{
		lpSrc = lpNow + SiteD8[i];
		if(*lpSrc == 0 && lpSrc != lpLast)
		{
			Temp[n] = lpSrc;
			n++;
		}
	}
	
	if(n == 0)	// û���ҵ������ڵ����ʾû�к�̵�
	{
		*lppNext = NULL;
		return 1;
	}
	else if(n == 1) // �ҵ���һ��������Ϊ��̵�
	{
		*lppNext = Temp[0];
		return 0;
	}
	else // �ҵ�������򷵻ش���
	{
		*lppNext = NULL;
		return 1;
	}
	
	return 0;
}

//
//	GetByDis: ���������ҵ����뵱ǰ�˵�Ϊd�ĵ�ĵ�ַ
//
int  GetByDis(BYTE *lpEnd, BYTE **lppPos, int d)
{
/////////////////////////////////////////////////////////////////////////////
//	lpEnd : [in] ��ǰ�˵�ĵ�ַ
//	lppPos : [out] Ŀ�ĵ�ĵ�ַ(����ھ���d���ж˵���߲�㣬�򷵻����ǵ��ڵ�)
//	d : [in] ���루������
/////////////////////////////////////////////////////////////////////////////
	int  i, n;
	BYTE   *lpNow = NULL;
	BYTE   *lpLast = NULL;
	BYTE   *lpNext = NULL;
	int	SiteD8[8] = {IMGW-1, IMGW, IMGW+1, 1, -IMGW+1, -IMGW, -IMGW-1, -1};

	*lppPos = NULL;
	
	// �ҵ���ǰ����Χ���Ǻڵ㲢�Ҳ���ǰ����ĺڵ㣬���浽����
	n = 0;
	for(i = 0; i < 8; i++)
	{
		if(*(lpEnd + SiteD8[i]) == 0)
		{
			lpNow = lpEnd + SiteD8[i];
			n++;
		}
	}
	if(n != 1) // ��Ŀ�������1�򷵻ش���
	{
		return 1;
	}
	// �ҵ��ĺڵ��ַΪlpNow
	// �˵���Ϊǰ����,��ַ���浽lpLast
	lpLast = lpEnd; 
	// ѭ����d�κ�̵�
	for(i = 0; i < d; i++)
	{
		// ����ǲ��������ѭ��
		if(IsFork(lpNow))
		{
			break;
		}
		// �ɹ��ҵ���һ��������ĵ�ǰ���ǰ����
		if(GetNext(lpNow, lpLast, &lpNext) == 0)
		{
			lpLast = lpNow;
			lpNow = lpNext;
		}
		else
		{
			break;
		}

	}
	// ����ҵ��ĵ�Ϊ��㣬�򷵻���ǰ����
	if(IsFork(lpNow))
	{
		*lppPos = lpLast;
	}	
	else	// ���򷵻ص�ǰ��
	{
		*lppPos = lpNow;
	}

	return 0;
}

//
//	PX: ���ݵ�ַ�õ���ǰ��ĺ�����
//
int  PX(BYTE *lpPos)
{
/////////////////////////////////////////////////////////////////////////
//	lpPos: [in] ��ǰ���ַ
/////////////////////////////////////////////////////////////////////////
	return ((lpPos - g_lpOrgFinger) % IMGW);
}

//
//	PY: ���ݵ�ַ�õ���ǰ���������
//
int  PY(BYTE *lpPos)
{
/////////////////////////////////////////////////////////////////////////
//	lpPos: [in] ��ǰ���ַ
/////////////////////////////////////////////////////////////////////////
	return ((lpPos - g_lpOrgFinger) / IMGW);
}

//
//	GetJiajiao: �������Ƕȵļн�(0 - 90)
//
int  GetJiajiao(int angle1, int angle2)
{
/////////////////////////////////////////////////////////////////////////
//	angle1: [in] �Ƕ�һ ��0 - 180��
//	angle2: [in] �Ƕȶ� ��0 - 180��
/////////////////////////////////////////////////////////////////////////
	int  a;

	a = abs(angle1-angle2);
	if(a > 90)
	{
		a = 180-a;
	}
	return a;
}

//
//	AngleAbs360: �������Ƕȵļн�(0 - 180)
//
int  AngleAbs360(int angle1, int angle2)
{
/////////////////////////////////////////////////////////////////////////
//	angle1: [in] �Ƕ�һ ��0 - 360��
//	angle2: [in] �Ƕȶ� ��0 - 360��
/////////////////////////////////////////////////////////////////////////
	int a;
	a = abs(angle1 - angle2);
	if(a > 180)
	{
		return (360 - a);
	}
	else
	{
		return a;
	}
}

//
//	GetAngleDis�� �����Ƕȼ�ľ���(0-360)  (��ʱ�뷽��)
//
int  GetAngleDis(int angleBegin, int angleEnd)
{
/////////////////////////////////////////////////////////////////////////
//	angleBegin: [in] ��ʼ�Ƕ� ��0 - 360��
//	angleEnd: [in] �ս�Ƕ� ��0 - 360��
/////////////////////////////////////////////////////////////////////////
	int  a;

	a = angleEnd-angleBegin;
	if(a < 0)
	{
		a += 360;
	}
	return a;
}

//
//	GetAngle�� ������������x�᷽��ĽǶ�
//
int  GetAngle(int x0, int y0, int x1, int y1)
{
/////////////////////////////////////////////////////////////////////////
//	x0: [in] ��һ���������
//	y0: [in] ��һ����������
//	x1: [in] �ڶ����������
//	y1: [in] �ڶ�����������
/////////////////////////////////////////////////////////////////////////
	double angle;

	angle = atan2((double)(y1-y0), (double)(x1-x0));
	// ����ת����0 - 2*PI
	if(angle < 0)
	{
		angle += 2*PI;
	}
	// ����ת��Ϊ�Ƕ�
	return (int)(angle*EPI + 0.5);
}

//
//	DIndex: �ԽǶȽ����������õ�������������(0-12)
//
int  DIndex(int angle)
{
/////////////////////////////////////////////////////////////////////////
//	angle: [in] �Ƕ� ��0 - 180��
/////////////////////////////////////////////////////////////////////////
	if(angle >= 173 || angle < 8)
	{
		return 0;
	}
	else
	{
		return ((angle-8)/15 + 1);
	}
}

int  getMinutia(BYTE *g_lpOrgFinger, BYTE *g_lpOrient, LPVOID feature, int IMGW, int IMGH)
{
	setup(IMGW, IMGH, g_lpOrgFinger);

	FEATURE *g_Feature = (FEATURE*)(feature);

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
				g_Feature->MinutiaArr[j].Direction = angle2 + 180;
			}
			else // �������ǣ����ʾ�������㷽����õ㷽�򳡷�����ͬ
			{
				g_Feature->MinutiaArr[j].Direction = angle2;
			}
			a = g_Feature->MinutiaArr[j].Direction / EPI + PI/4.0;
			x11 = EndArr[i].x + (int)(trilen*cos(a)+ 0.5);
			y11 = EndArr[i].y + (int)(trilen*sin(a)+ 0.5);
			g_Feature->MinutiaArr[j].Triangle[0] = *(g_lpOrient + y11*IMGW + x11);
			a += PI*2/3.0;
			x11 = EndArr[i].x + (int)(trilen*cos(a)+ 0.5);
			y11 = EndArr[i].y + (int)(trilen*sin(a)+ 0.5);
			g_Feature->MinutiaArr[j].Triangle[1] = *(g_lpOrient + y11*IMGW + x11);
			a += PI*2/3.0;
			x11 = EndArr[i].x + (int)(trilen*cos(a)+ 0.5);
			y11 = EndArr[i].y + (int)(trilen*sin(a)+ 0.5);
			g_Feature->MinutiaArr[j].Triangle[2] = *(g_lpOrient + y11*IMGW + x11);

			g_Feature->MinutiaArr[j].x = EndArr[i].x;	//������
			g_Feature->MinutiaArr[j].y = EndArr[i].y;	//������
			g_Feature->MinutiaArr[j].Type = VF_MINUTIA_END;	//����

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
				g_Feature->MinutiaArr[EndNum + j].Direction = angle2 + 180;
			}
			else // �������ǣ����ʾ�������㷽����õ㷽�򳡷�����ͬ
			{
				g_Feature->MinutiaArr[EndNum + j].Direction = angle2;
			}
			// ��¼��������Ϊ�������Բ�뾶Ϊtrilen������������������ķ���
			a = g_Feature->MinutiaArr[EndNum + j].Direction / EPI + PI/4.0;
			x11 = EndArr[i].x + (int)(trilen*cos(a)+ 0.5);
			y11 = EndArr[i].y + (int)(trilen*sin(a)+ 0.5);
			g_Feature->MinutiaArr[EndNum + j].Triangle[0] = *(g_lpOrient + y11*IMGW + x11);
			a += PI*2/3.0;
			x11 = EndArr[i].x + (int)(trilen*cos(a)+ 0.5);
			y11 = EndArr[i].y + (int)(trilen*sin(a)+ 0.5);
			g_Feature->MinutiaArr[EndNum + j].Triangle[1] = *(g_lpOrient + y11*IMGW + x11);
			a += PI*2/3.0;
			x11 = EndArr[i].x + (int)(trilen*cos(a)+ 0.5);
			y11 = EndArr[i].y + (int)(trilen*sin(a)+ 0.5);
			g_Feature->MinutiaArr[EndNum + j].Triangle[2] = *(g_lpOrient + y11*IMGW + x11);

			g_Feature->MinutiaArr[EndNum + j].x = ForkArr[i].x; //������
			g_Feature->MinutiaArr[EndNum + j].y = ForkArr[i].y; //������
			g_Feature->MinutiaArr[EndNum + j].Type = VF_MINUTIA_FORK; // ����


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
	g_Feature->MinutiaNum = (BYTE)(EndNum+ForkNum);

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




#endif // !defined(AFX_MINUTIAEXTRACT_H__F76D271A_5FD4_4DBA_B826_7B57F2574483__INCLUDED_)



