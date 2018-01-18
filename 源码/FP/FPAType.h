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

#define VF_MINUTIA_END        1	// �˵�
#define VF_MINUTIA_FORK       2	// ���

// ��ȷ�ȶ�ģʽ����Ҫ���������ȶԴ�������
#define	VF_MATCHMODE_VERIFY			1
// ���ٱȶ�ģʽ����Ҫ���ڴ����ȶԴ�������
#define	VF_MATCHMODE_IDENTIFY		2

#endif