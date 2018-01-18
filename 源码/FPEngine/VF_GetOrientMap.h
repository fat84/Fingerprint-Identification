#ifndef	__VFGETORIENTMAP_H__
#define __VFGETORIENTMAP_H__

#include "VF_Type.h"
#include "VF_Global.h"

//
//	getOrientMap: ��ָ�Ʒ���
//
void getOrientMap(sint32  r)
{
/////////////////////////////////////////////////////////////////////////
//	r: [in] ��ʽ�������߷���Ĵ��ڰ뾶�������(8 - 16)
/////////////////////////////////////////////////////////////////////////
	sint32	x, y, i, j;
	sint32  vx, vy, lvx, lvy;
	uint8   *lpSrc = NULL;
	uint8   *lpDiv = NULL;
	uint8   *lpOri = NULL;
	sint32	angle, grad, gradSum, num;
	double  fAngle;

	for(y = 0; y < IMGH/2; y++)
	{
		for(x = 0; x < IMGW/2; x++)
		{
			lpDiv = g_lpDivide + 2*y*IMGW + 2*x;
			lpOri = g_lpOrient + 2*y*IMGW + 2*x;
			lvx = 0;
			lvy = 0;
			num = 0;
			gradSum = 0;
			for(i = -r; i <= r; i++)	// Ϊ����ٶȣ�����Ϊ2
			{
				if(y+i<1 || y+i>=IMGH/2-1)
					continue;
				for(j = -r; j <= r; j++)	// Ϊ����ٶȣ�����Ϊ2
				{
					if(x+j<1 || x+j>=IMGW/2-1)
						continue;
					lpSrc = g_lpTemp + (y+i)*(IMGW/2) + x+j;

					vx = *(lpSrc + IMGW/2 + 1) - *(lpSrc + IMGW/2 - 1) +
						 *(lpSrc + 1)*2 - *(lpSrc - 1)*2 +
						 *(lpSrc - IMGW/2 + 1) - *(lpSrc - IMGW/2 - 1);

					vy = *(lpSrc + IMGW/2 - 1) - *(lpSrc - IMGW/2 - 1) +
						 *(lpSrc + IMGW/2)*2 - *(lpSrc - IMGW/2)*2 +
						 *(lpSrc + IMGW/2 + 1) - *(lpSrc - IMGW/2 + 1);

					gradSum += (abs(vx)+abs(vy));
					lvx += vx * vy * 2;
					lvy += vx*vx - vy*vy;
					num++;
				}
			}
			if(num == 0)
				num = 1;
			// ���ֵ�����浽g_lpDivide�У����ڷָ�ǰ������
			grad = gradSum/num;
			
			if(grad > 255)
				grad = 255;
			*lpDiv = (uint8)grad;
			*(lpDiv + 1) = (uint8)grad;
			*(lpDiv + IMGW) = (uint8)grad;
			*(lpDiv + IMGW + 1) = (uint8)grad;

			// �󻡶�
			fAngle = atan2((double)lvy,(double) lvx);
			// �任��(0 - 2*pi)
			if(fAngle < 0)
			{
				fAngle += 2*PI;
			}
			// �����߽Ƕ�
			fAngle = (fAngle*EPI*0.5 + 0.5);
			angle = (sint32)fAngle;

			// ��Ϊ���� sobel���ӣ����ԽǶ�ƫת��135�ȣ�����Ҫ��ת��õĽǶ�
			angle -= 135;
			// �Ƕȱ任����0-180��
			if(angle <= 0)
			{
				angle += 180;
			}
			angle = 180-angle;
			// �������߽Ƕ�
			*lpOri = (uint8)angle;
			*(lpOri + 1) = (uint8)angle;
			*(lpOri + IMGW) = (uint8)angle;
			*(lpOri + IMGW + 1) = (uint8)angle;

		}
	}
	
}

#endif
