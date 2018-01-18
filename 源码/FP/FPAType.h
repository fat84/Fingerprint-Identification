#ifndef	__FPATYPE_H__
#define __FPATYPE_H__

typedef struct tagMatchResult {
  int    Similarity; 
  int    Rotation; 
  int    TransX;
  int    TransY;
  int	MMCount;
} MATCHRESULT, *PMATCHRESULT; 


/*
	VF_RETURN
	This data type is returned by most functions.
*/

#define NULL 0
typedef unsigned char       BYTE;

typedef int VF_RETURN;

typedef BYTE VF_FLAG;


#define VF_OK (0)

/*
	VF_MINUTIAE_TYPE
*/

#define VF_MINUTIA_END        1	// 端点
#define VF_MINUTIA_FORK       2	// 叉点

// 精确比对模式，主要用在少量比对次数场合
#define	VF_MATCHMODE_VERIFY			1
// 快速比对模式，主要用在大量比对次数场合
#define	VF_MATCHMODE_IDENTIFY		2

#endif