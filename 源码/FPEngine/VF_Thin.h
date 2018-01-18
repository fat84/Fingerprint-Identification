#ifndef	__VFTHIN_H__
#define __VFTHIN_H__

#include "VF_Type.h"
#include "VF_Global.h"
#include "VF_Function.h"

//
//	imageThin: ͼ��ϸ������
//
sint32 imageThin(uint8 *lpBits, sint32 Width, sint32 Height)
{
/////////////////////////////////////////////////////////////////
//	lpBits: [in, out] Ҫϸ����ͼ�����ݻ����� 
//	Width: [in] Ҫϸ����ͼ����
//	Height: [in] Ҫϸ����ͼ��߶�
/////////////////////////////////////////////////////////////////
	uint8  erasetable[256]={
			0,0,1,1,0,0,1,1,             1,1,0,1,1,1,0,1,
			1,1,0,0,1,1,1,1,             0,0,0,0,0,0,0,1,
			0,0,1,1,0,0,1,1,             1,1,0,1,1,1,0,1,
			1,1,0,0,1,1,1,1,             0,0,0,0,0,0,0,1,
			1,1,0,0,1,1,0,0,             0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,             0,0,0,0,0,0,0,0,
			1,1,0,0,1,1,0,0,             1,1,0,1,1,1,0,1,
			0,0,0,0,0,0,0,0,             0,0,0,0,0,0,0,0,
			0,0,1,1,0,0,1,1,             1,1,0,1,1,1,0,1,
			1,1,0,0,1,1,1,1,             0,0,0,0,0,0,0,1,
			0,0,1,1,0,0,1,1,             1,1,0,1,1,1,0,1,
			1,1,0,0,1,1,1,1,             0,0,0,0,0,0,0,0,
			1,1,0,0,1,1,0,0,             0,0,0,0,0,0,0,0,
			1,1,0,0,1,1,1,1,             0,0,0,0,0,0,0,0,
			1,1,0,0,1,1,0,0,             1,1,0,1,1,1,0,0,
			1,1,0,0,1,1,1,0,             1,1,0,0,1,0,0,0
	};
	sint32		x,y;
	sint32      num;
	BOOL        Finished;
	uint8       nw,n,ne,w,e,sw,s,se;
	uint8       *lpPtr = NULL;
	uint8       *lpTempPtr = NULL;	

	memcpy((void *)g_lpTemp, (void *)lpBits, Width*Height);
	
	//������־�óɼ�
	Finished=FALSE;
	while(!Finished){ //��û�н���
              //������־�óɼ�
            Finished=TRUE;
       //�Ƚ���ˮƽ�����ϸ��
              for (y=1;y<Height-1;y++)
			  { //ע��Ϊ��ֹԽ�磬y�ķ�Χ��1���߶�-2
                     //lpPtrָ��ԭͼ���ݣ�lpTempPtrָ����ͼ����

                     lpPtr=(uint8 *)lpBits+y*Width;
                     lpTempPtr=(uint8 *)g_lpTemp+y*Width;

                     x=1; //ע��Ϊ��ֹԽ�磬x�ķ�Χ��1�����-2

                     while(x<Width-1)
					 {
                            if(*(lpPtr+x)==0)
							{ //�Ǻڵ��������
                                   w = *(lpPtr+x-1);  //���ڵ�
                                   e = *(lpPtr+x+1);  //���ڵ�

                                   if( (w==255)|| (e==255)){ 

									//������������ھ���������һ���ǰ׵�Ŵ���

                                          nw=*(lpPtr+x+Width-1); //�����ڵ�

                                          n=*(lpPtr+x+Width); //���ڵ�

                                          ne=*(lpPtr+x+Width+1); //�����ڵ�

                                          sw=*(lpPtr+x-Width-1); //�����ڵ�

                                          s=*(lpPtr+x-Width); //���ڵ�

                                          se=*(lpPtr+x-Width+1); //�����ڵ�

                                          //��������

                            num=nw/255+n/255*2+ne/255*4+w/255*8+e/255*16+
								sw/255*32+s/255*64+se/255*128;

                                          if(erasetable[num]==1){ //���������ɾ��

												//��ԭͼ�������н��úڵ�ɾ��

                                                 *(lpPtr+x)=255; 

													//���ͼ�иúڵ�Ҳɾ��

                                                 *(lpTempPtr+x)=255; 

                                                 Finished=FALSE; //�иĶ���������־�óɼ�

                                                 x++; //ˮƽ��������һ������

                                          }

                                   }

                            }

                            x++; //ɨ����һ������

                     }

              }

       //�ٽ��д�ֱ�����ϸ��

              for (x=1;x<Width-1;x++){ //ע��Ϊ��ֹԽ�磬x�ķ�Χ��1�����-2

                     y=1; //ע��Ϊ��ֹԽ�磬y�ķ�Χ��1���߶�-2

                     while(y<Height-1){

                            lpPtr=lpBits+y*Width;

                            lpTempPtr=g_lpTemp+y*Width;

                            if(*(lpPtr+x)==0){ //�Ǻڵ��������

                                   n=*(lpPtr+x+Width);

                                   s=*(lpPtr+x-Width);

                                   if( (n==255)|| (s==255)){

//������������ھ���������һ���ǰ׵�Ŵ���

                                          nw=*(lpPtr+x+Width-1);

                                          ne=*(lpPtr+x+Width+1);

                                          w=*(lpPtr+x-1);

                                          e=*(lpPtr+x+1);

                                          sw=*(lpPtr+x-Width-1);

                                          se=*(lpPtr+x-Width+1);

                                          //��������

										num=nw/255+n/255*2+ne/255*4+w/255*8+e/255*16+

											sw/255*32+s/255*64+se/255*128;

                                          if(erasetable[num]==1){ //���������ɾ��

//��ԭͼ�������н��úڵ�ɾ��

                                                 *(lpPtr+x)=255; 

//���ͼ�иúڵ�Ҳɾ��

                                                 *(lpTempPtr+x)=255; 

                                                 Finished=FALSE; //�иĶ���������־�óɼ�

                                                 y++;//��ֱ��������һ������

                                          }

                                   }

                            }

                            y++; //ɨ����һ������

                     }

              } 

	}
	
	memcpy((void *)lpBits, (void *)g_lpTemp, Width*Height);

	return 0;
}

//
//	thin : �Ե�ǰָ��ͼ�����ϸ������
//
sint32 thin()
{
	imageThin(g_lpOrgFinger, IMGW, IMGH);

	return 0;
}

//
//	thinClear: ���ϸ��ͼ���ж̰���ë��
//
sint32  thinClear(sint32  len)
{
/////////////////////////////////////////////////////////////////
//	len: [in] �̰���ë�̵���󳤶�
/////////////////////////////////////////////////////////////////
	sint32	x, y;
	sint32  i, n, num;
	sint32  temp;
	uint8   *Line[25];
	uint8   *lpNow = NULL;
	uint8   *lpLast = NULL;
	uint8   *lpNext = NULL;
	uint8   *tempPtr[8];
	sint32	SiteD8[8] = {IMGW-1, IMGW, IMGW+1, 1, -IMGW+1, -IMGW, -IMGW-1, -1};

	temp = 0;
	for(y = 0; y < IMGH; y++)
	{
		for(x = 0; x < IMGW; x++)
		{
			lpNow = g_lpOrgFinger + temp + x;
			if(*lpNow != 0)
			{
				continue;
			}
			Line[0] = lpNow;
			// ͳ�Ƶ�ǰ�����Χ�ڵ����
			n = 0;
			for(i = 0; i < 8; i++)
			{
				lpNext = lpNow + SiteD8[i];
				if(*lpNext == 0)
				{
					tempPtr[n] = lpNext;
					n++;
				}
			}
			// �ڵ����Ϊ�㣬��ʾ��ǰ���ǹµ㣬��Ϊ��ɫ
			if(n == 0)
			{
				*lpNow = 255;
				continue;
			}
			// �ڵ����Ϊ1����ʾΪ�˵�
			else if(n == 1)
			{
				num = 0;
				lpLast = lpNow;
				lpNow = tempPtr[0];
				// �����߸���len����
				for(i = 0; i < len; i++)
				{
					// ����������������ѭ��
					if(IsFork(lpNow))
					{
						break;
					}
					num++;
					Line[num] = lpNow;
					if(GetNext(lpNow, lpLast, &lpNext) == 0)
					{
						lpLast = lpNow;
						lpNow = lpNext;
					}
					else // ��������쳣����ѭ��
					{
						break;
					}

				}
				
				// ���߽϶̣���ʾΪ�̰�������ë��
				if(num < len)
				{
					for(i = 0; i <= num; i++)
					{
						*Line[i] = 255;
					}
				}

			}
		}
		temp += IMGW;
	}

	return 0;
}

#endif