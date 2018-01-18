#ifndef	__VFZOOM_H__
#define __VFZOOM_H__

#include "VF_Type.h"
#include "VF_Global.h"

//
// ��ȡָ��ͼ���Ƶ��Ϣ�������㷽��
//
void zoomout()
{
	sint32	x, y;
	uint8	*lpSrc;
	uint8	*lpRst;
	sint32	sum;
	sint32	SiteD8[8] = {IMGW-1, IMGW, IMGW+1, 1, -IMGW+1, -IMGW, -IMGW-1, -1};
	
	// ��Ե����
	for(y = 0; y < IMGH; y+=2)
	{
		lpSrc = g_lpOrgFinger + y*IMGW;
		lpRst = g_lpTemp + (y/2)*(IMGW/2);
		*lpRst = *lpSrc;
		lpSrc = g_lpOrgFinger + y*IMGW + IMGW - 1;
		lpRst = g_lpTemp + (y/2)*(IMGW/2) + (IMGW/2 - 1);
		*lpRst = *lpSrc;
	}
	for(x = 0; x < IMGW; x+=2)
	{
		lpSrc = g_lpOrgFinger + x;
		lpRst = g_lpTemp + x/2;
		*lpRst = *lpSrc;
		lpSrc = g_lpOrgFinger + (IMGH-1)*IMGW + x;
		lpRst = g_lpTemp + (IMGH/2-1)*(IMGW/2) + x/2;
		*lpRst = *lpSrc;
	}

	// �Ǳ�Ե�����ø�˹ģ����ȡ��Ƶ��Ϣ
	for(y = 2; y < IMGH-2; y+=2)
	{
		for(x = 2; x < IMGW-2; x+=2)
		{
			lpSrc = g_lpOrgFinger + y*IMGW + x;
			lpRst = g_lpTemp + (y/2)*(IMGW/2) + x/2;
			sum = *lpSrc*4 + *(lpSrc + SiteD8[0]) +
				  *(lpSrc + SiteD8[1])*2 + *(lpSrc + SiteD8[2]) +
				  *(lpSrc + SiteD8[3])*2 + *(lpSrc + SiteD8[4]) +
				  *(lpSrc + SiteD8[5])*2 + *(lpSrc + SiteD8[6]) +
				  *(lpSrc + SiteD8[7])*2;
			sum = sum>>4;
			*lpRst = (uint8)sum;
		}
	}
	

}

#endif