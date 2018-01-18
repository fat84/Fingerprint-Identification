#ifndef	__FPAGLOBAL_H__
#define __FPAGLOBAL_H__



#include "math.h"


#define     MAXMINUTIANUM			60			// �����������
#define     MAXRAWMINUTIANUM        100			// �������������ĳ�ʼ�����������Ŀ

int      g_MinutiaNum = 0;					// ��������֮��

#define  PI          3.141593
#define  EPI         57.29578

// ��ĳ��Ϊ���ĵ�12������������7����λ�õĺ���ƫ��
int  g_DSite[12][7][2] = {
	-3, 0,  -2, 0,  -1, 0,   0, 0,   1, 0,   2, 0,   3, 0,
	-3,-1,  -2,-1,  -1, 0,   0, 0,   1, 0,   2, 1,   3, 1,
	-3,-2,  -2,-1,  -1,-1,   0, 0,   1, 1,   2, 1,   3, 2,
	-3,-3,  -2,-2,  -1,-1,   0, 0,   1, 1,   2, 2,   3, 3,
	-2,-3,  -1,-2,  -1,-1,   0, 0,   1, 1,   1, 2,   2, 3,
	-1,-3,  -1,-2,   0,-1,   0, 0,   0, 1,   1, 2,   1, 3,
	 0,-3,   0,-2,   0,-1,   0, 0,   0, 1,   0, 2,   0, 3,
	-1, 3,  -1, 2,   0, 1,   0, 0,   0,-1,   1,-2,   1,-3,
	-2, 3,  -1, 2,  -1, 1,   0, 0,   1,-1,   1,-2,   2,-3,
	-3, 3,  -2, 2,  -1, 1,   0, 0,   1,-1,   2,-2,   3,-3,
	-3, 2,  -2, 1,  -1, 1,   0, 0,   1,-1,   2,-1,   3,-2,
	-3, 1,  -2, 1,  -1, 0,   0, 0,   1, 0,   2,-1,   3,-1
};



// ������ṹ
typedef struct tagMinutiae { 
  int    x;				// ������
  int    y;				// ������
  int    Direction;		// ����
  int	Triangle[3];	// ������Ϊ�������Բ�뾶Ϊ��ֵ������������������ķ���
  int    Type;			// ����
} MINUTIA, *MINUTIAPTR; 

// ָ������(ģ��)�ṹ
typedef struct tagFeature{
	int		MinutiaNum;					// ��������
	MINUTIA		MinutiaArr[MAXMINUTIANUM];	// ����������
} FEATURE, *FEATUREPTR;

FEATURE    g_Feature;		// ��ǰָ�Ƶ�ָ������

#define SAFE_FREE(x) { if (x != NULL) free((void *)x); x = NULL; }

#define WIDTHBYTES(i)    ((i+3)/4*4)


#endif