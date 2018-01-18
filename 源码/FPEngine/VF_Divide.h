#ifndef	__VFDIVIDE_H__
#define __VFDIVIDE_H__

#include "VF_Type.h"
#include "VF_Global.h"
#include "VF_Smooth.h"

//
// divide: ���ݷ��򳡷�ֵ���ָ�ָ��ͼ��ǰ������
//
sint32 divide(sint32  r, sint32 threshold)
{
///////////////////////////////////////////////////////////////////////
//	r: [in] �Է�ֵͼ��߶�ƽ���˲����˲����뾶
//  threshold: [in] �ָ����ֵ
///////////////////////////////////////////////////////////////////////
	sint32	x, y;
	sint32  num = 0;

	// �Է��򳡷�ֵͼ����и߶�ƽ���˲�
	smooth(g_lpDivide, g_lpTemp, r, 2);
	
	// ͼ���Ե������Ϊ����
	for(y = 0; y < IMGH; y++)
	{
		*(g_lpDivide + y*IMGW) = 255;
		*(g_lpDivide + y*IMGW + IMGW - 1) = 255;
	}
	for(x = 0; x < IMGW; x++)
	{
		*(g_lpDivide + x) = 255;
		*(g_lpDivide + (IMGH-1)*IMGW + x) = 255;
	}

	for(y = 1; y < IMGH-1; y++)
	{
		for(x = 1; x < IMGW-1; x++)
		{
			// ���ݷ�ֵ����ֵ��С�ж��Ƿ�Ϊ��������
			if(*(g_lpTemp + y*IMGW + x) < threshold)
			{
				*(g_lpDivide + y*IMGW + x) = 255;
			}
			else
			{
				*(g_lpDivide + y*IMGW + x) = 0;
				num++;
			}
		}
	}

	// ���ǰ���������С���������ʮ��֮һ�����ʾǰ������̫С�����ش���
	if(num < IMGSIZE/10)
		return 1;
	else
		return 0;

}

//
//	clearEdge: �������
//
void clearEdge()
{
	sint32	x, y;
	sint32  temp;
	temp = 0;

	for(y = 0; y < IMGH; y++)
	{
		for(x = 0; x < IMGW; x++)
		{	
			// ����Ǳ����������øõ�Ϊ�׵�
			if(*(g_lpDivide + temp + x) == 255)
			{
				*(g_lpOrgFinger + temp + x) = 255;
				*(g_lpOrient + temp + x) = 255;
			}
		}
		temp += IMGW;
	}
}


#endif