#ifndef	__VFSMOOTH_H__
#define __VFSMOOTH_H__

#include "VF_Type.h"
#include "VF_Global.h"

//
//	smooth: ��ͼ����о�ֵƽ���˲�,��������
//
void smooth(uint8 *lpInBuffer, uint8 *lpOutBuffer, sint32 r, sint32 d)
{
////////////////////////////////////////////////////////////////////////
//	lpInBuffer: [in] Ҫƽ����ͼ�����ݻ�����
//	lpOutBuffer: [out] ƽ�����ͼ�����ݻ�����
//	r: [in] ƽ���˲����뾶
//	d: [in] ƽ���˲�������
////////////////////////////////////////////////////////////////////////
	sint32	x, y, i, j;
	uint8	*lpSrc;
	uint8	*lpRst;
	sint32	sum, num;
	
	if(d == 2)
	{
		for(y = 0; y < IMGH; y++)
		{
			for(x = 0; x < IMGW; x++)
			{
				lpSrc = lpInBuffer + y*IMGW + x;//ԭͼ��ָ��
				lpRst = lpOutBuffer + y*IMGW + x;//Ŀ��ͼ��ָ��
				sum = 0;						//���غ�����
				num = 0;						//������
				for(i = -r; i <= r; i+=2)		//������Ϊ��x��y���߳�Ϊ2r�ľ���������
				{
					if(i+y<0 || i+y>=IMGH)
						continue;
					for(j = -r; j <= r; j+=2)
					{
						if(j+x<0 || j+x>=IMGW)
							continue;
						sum += *(lpSrc + i*IMGW + j);//�����غ�
						num++;						//�����ظ���
					}
				}
				*lpRst = (uint8)(sum/num);			//��ƽ��
			}
		}
	}
	else
	{
		for(y = 0; y < IMGH; y++)
		{
			for(x = 0; x < IMGW; x++)
			{
				lpSrc = lpInBuffer + y*IMGW + x;
				lpRst = lpOutBuffer + y*IMGW + x;
				sum = 0; 
				num = 0;
				for(i = -r; i <= r; i++)
				{
					if(i+y<0 || i+y>=IMGH)
						continue;
					for(j = -r; j <= r; j++)
					{
						if(j+x<0 || j+x>=IMGW)
							continue;
						sum += *(lpSrc + i*IMGW + j);
						num++;
					}
				}
				*lpRst = (uint8)(sum/num);
			}
		}
	}

}


#endif