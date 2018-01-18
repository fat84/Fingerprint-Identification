#ifndef	__VFTYPE_H__
#define __VFTYPE_H__

/* Basic Types */
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef short sint16;
typedef unsigned long uint32;
typedef int sint32;
//typedef long sint32;
typedef char sint8;

#if defined(WIN32)
typedef unsigned __int64 uint64; /* MSVC++ 5/6 declaration */
#else
#ifdef __arch64__
typedef unsigned long uint64;
#else
typedef unsigned long long uint64; /* gcc 2.7.2 declaration */
#endif
#endif

typedef struct tagMatchResult { 
  sint32    Similarity; 
  sint32    Rotation; 
  sint32    TransX;
  sint32    TransY;
  sint32	MMCount;
} MATCHRESULT, *PMATCHRESULT; 


/*
	VF_RETURN
	This data type is returned by most functions.
*/
typedef uint32 VF_RETURN;

typedef uint8  VF_FLAG;


#define VF_OK (0)

/*
	VF_MINUTIAE_TYPE
*/

#define VF_MINUTIA_END        1	// �˵�
#define VF_MINUTIA_FORK       2	// ���
#define VF_MINUTIA_CORE		  3 //���ĵ�
#define VF_MINUTIA_DELTA	  4 //���ǵ�

// ��ȷ�ȶ�ģʽ����Ҫ���������ȶԴ�������
#define	VF_MATCHMODE_VERIFY			1
// ���ٱȶ�ģʽ����Ҫ���ڴ����ȶԴ�������
#define	VF_MATCHMODE_IDENTIFY		2



#endif