#ifndef	__FPAGLOBAL_H__
#define __FPAGLOBAL_H__



#include "math.h"


#define     MAXMINUTIANUM			60			// 最大特征点数
#define     MAXRAWMINUTIANUM        100			// 包含虚假特征点的初始特征点最大数目

int      g_MinutiaNum = 0;					// 特征点数之和

#define  PI          3.141593
#define  EPI         57.29578

// 以某点为中心的12个量化方向上7个点位置的横纵偏移
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



// 特征点结构
typedef struct tagMinutiae { 
  int    x;				// 横坐标
  int    y;				// 纵坐标
  int    Direction;		// 方向
  int	Triangle[3];	// 特征点为中心外接圆半径为定值的正三角形三个顶点的方向
  int    Type;			// 类型
} MINUTIA, *MINUTIAPTR; 

// 指纹特征(模板)结构
typedef struct tagFeature{
	int		MinutiaNum;					// 特征点数
	MINUTIA		MinutiaArr[MAXMINUTIANUM];	// 特征点数组
} FEATURE, *FEATUREPTR;

FEATURE    g_Feature;		// 当前指纹的指纹特征

#define SAFE_FREE(x) { if (x != NULL) free((void *)x); x = NULL; }

#define WIDTHBYTES(i)    ((i+3)/4*4)


#endif