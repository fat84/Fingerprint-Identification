#ifndef	__VFBINARY_H__
#define __VFBINARY_H__

#include "VF_Type.h"
#include "VF_Global.h"
#include "VF_Function.h"

//
//	binary: ��ָ��ͼ����ж�ֵ��
//
sint32 binary()
{
	sint32 x, y;
	sint32 i;
	sint32 d = 0;
	sint32 sum = 0;
	// ���߷����ϵ�7�����Ȩֵ
	sint32 Hw[7] = {2, 2, 3, 4, 3, 2, 2};
	// ���߷���Ĵ�ֱ�����ϵ�7�����Ȩֵ
	sint32 Vw[7] = {1, 1, 1, 1, 1, 1, 1};
	sint32 hsum = 0;	// ���߷����ϵ�7����ļ�Ȩ��
	sint32 vsum = 0;	// ���߷���Ĵ�ֱ�����ϵ�7����ļ�Ȩ��
	sint32 Hv = 0;		// ���߷����ϵ�7����ļ�Ȩƽ��ֵ
	sint32 Vv = 0;		// ���߷���Ĵ�ֱ�����ϵ�7����ļ�Ȩƽ��ֵ
	sint32 temp = 0;
	uint8  *lpSrc = NULL;	// ָ��ͼ�����ص�ָ��
	uint8  *lpDir = NULL;	// ���߷���ָ��

	temp = 0;
	for(y = 0; y < IMGH; y++)
	{
		for(x = 0; x < IMGW; x++)
		{
			lpDir = g_lpOrient + temp + x;
			lpSrc = g_lpOrgFinger + temp + x;
			
			// ����õ�ǳ��ڣ�������ʱ���������øõ�Ϊ�ڵ㣬ֵΪ0
			if(*lpSrc < 4)
			{
				*(g_lpTemp + temp + x) = 0;
				continue;
			}
			// ���㷽������������Ϊ12������
			d = DIndex(*lpDir);
			
			// ���㵱ǰ�������߷����ϵļ�Ȩƽ��ֵ
			sum = 0;
			hsum = 0;
			for(i = 0; i < 7; i++)
			{
				// �����Ƿ�Խ��
				if(y+g_DSite[d][i][1] < 0 || y+g_DSite[d][i][1] >= IMGH ||
				   x+g_DSite[d][i][0] < 0 || x+g_DSite[d][i][0] >= IMGW)
				{
					continue;
				}
				sum += Hw[i]*(*(lpSrc + g_DSite[d][i][1]*IMGW + g_DSite[d][i][0]));
				hsum += Hw[i];
			}
			if(hsum != 0)
			{
				Hv = sum/hsum;
			}
			else
			{
				Hv = 255;
			}

			// ���߷���Ĵ�ֱ���������
			d = (d+6)%12;

			// ���㵱ǰ�������߷���Ĵ�ֱ�����ϵļ�Ȩƽ��ֵ
			sum = 0;
			vsum = 0;
			for(i = 0; i < 7; i++)
			{
				if(y+g_DSite[d][i][1] < 0 || y+g_DSite[d][i][1] >= IMGH ||
				   x+g_DSite[d][i][0] < 0 || x+g_DSite[d][i][0] >= IMGW)
				{
					continue;
				}
				sum += Vw[i]*(*(lpSrc + g_DSite[d][i][1]*IMGW + g_DSite[d][i][0]));
				vsum += Vw[i];
			}
			if(vsum != 0)
			{
				Vv = sum/vsum;
			}
			else
			{
				Vv = 255;
			}

			
			if(Hv < Vv)
			{
				// ���߷����ϼ�Ȩƽ��ֵ��С���õ�ǰ��Ϊ�ڵ�
				*(g_lpTemp + temp + x) = 0;
			}
			else
			{
				// ���߷����ϼ�Ȩƽ��ֵ�ϴ����õ�ǰ��Ϊ�׵�
				*(g_lpTemp + temp + x) = 255;
			}
		}
		temp += IMGW;
	}

	// ����ʱ�����������ݿ�����ԭʼͼ�����ݻ�����
	memcpy((void *)g_lpOrgFinger, (void *)g_lpTemp, IMGSIZE);
	

	return 0;

}

//
//	binary: �Զ�ֵ��ָ��ͼ�����ȥ����
//
void binaryClear()
{
	sint32 x, y, i;
	sint32 num = 0;
	bool   bWorking;
	sint32 temp;
	uint8  *lpSrc = NULL;
	bWorking = true;
	sint32  n = 0;
	// ĳ����Χ8����ĵ�ַƫ��
	sint32	SiteD8[8] = {IMGW-1, IMGW, IMGW+1, 1, -IMGW+1, -IMGW, -IMGW-1, -1};
	
	// ��ʼ����ʱ������
	memset((void *)g_lpTemp, 255, IMGSIZE);

	// ѭ������ֱ��������ϻ��ߴ����������8
	while(bWorking && n < 8)
	{
		bWorking = false;
		n++;
		temp = IMGW;
		for(y = 1; y < IMGH-1; y++)
		{
			for(x = 1; x < IMGW-1; x++)
			{
				// �����ĵ㲻����
				if(*(g_lpDivide + temp + x) == 255)
				{
					continue;
				}
				// ͳ�Ƶ�ǰ����Χ������ͬ���͵�ĸ���
				num = 0;
				lpSrc = g_lpOrgFinger + temp + x;
				for(i = 0; i < 8; i++)
				{
					if(*(lpSrc+SiteD8[i]) == *lpSrc)
					{
						num++;
					}
				}
				// ��ͬ�����С�ڶ���ı䵱ǰ������
				if(num < 2)
				{
					*(g_lpTemp+temp+x) = 255 - *lpSrc;
					bWorking = true;
				}
				else
				{
					*(g_lpTemp+temp+x) = *lpSrc;
				}

			}
			temp += IMGW;
		}
		// ��������������ԭʼͼ�񻺳���
		memcpy((void *)g_lpOrgFinger, (void *)g_lpTemp, IMGSIZE);
	}

}


#endif
