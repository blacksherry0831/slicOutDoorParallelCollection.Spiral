#pragma once
//#include <SGVcode/ImageMemData.h>
/*------------------------------------------*/
/**
*���Ȳ�
*�ǶȲ�
*������ģ��
*Բ��
*/
/*------------------------------------------*/
/**<���Ⱦ����-�ڻ�Сһ��*/
#define  L_OutLightDstDelta  (18)
#define  L_InLightDstDelta   (9)
/**<�ǶȾ����*/
#define  ABOutCircleDst		(8)
#define  ABInCircleDst		(8)
/**<ģ�������*/
#define  VectorLengthDeltaIn       (18)
#define  VectorLengthDeltaOut      (12)
/**<Բ������[ͳ�Ƶó�]*/
#define  CylinderBlackWhite     (16)
/*---------------------------------------------------------------------------*/
/**
*����ɫ�ʶ�߶�ģ��
*@image image036.jpg ɫ�ʶ�߶�ͼ��֪ģ�� 
*��a=0��b=0ΪԲ�ģ�Ϊ�뾶��Բ���彫CIELABɫ�ʿռ�ָ����������\n
*-���Ƽ��㷽��\n
*-#Բ���ڲ������Ȳ�����Ҫ���Ƽ���\n
*-#Բ������ýǶ�����Ҫ���Ƽ���\n
*
*@note
*Androutsos����ͨ��ʵ���HSV��ɫ�ռ�����˴��»��֣�
*���ȴ���75%���ұ��Ͷȴ���20%Ϊ����ɫ��������С��25%Ϊ��ɫ����
*���ȴ���75%���ұ��Ͷ�С��20%Ϊ��ɫ��������Ϊ��ɫ����
*
*/
/*---------------------------------------------------------------------------*/
class LabColorAnalyzeCluster
{
private:
	ImageMemData* pMD;/**<��Ҫ��ͼ�����ݺ�ͼ������õ��м����*/
public:
	LabColorAnalyzeCluster(ImageMemData* MemData_t);
	~LabColorAnalyzeCluster(void);
	void ColorPointIteration(void);
	void GetMatrix_E(void);
	void GetMatrix_W(void);
	double Find_MaxSimilar(void);
public:
	static bool ColorNearWhiteBlack(LabColor c);
	static float LabDistanceAB(LabColor ci,LabColor cj);
	static float LabDistanceLAB(LabColor ci,LabColor cj);
	

	static bool LabIsSameColorIn(LabColor ci,LabColor cj,bool Enable);
	static bool LabIsSameColorOut(LabColor ci,LabColor cj,bool Enable);
	static bool LabIsSameColorInOut(LabColor ci,LabColor cj,bool Enable);

	static bool LabIsSameColor(LabColor ci,LabColor cj);
	

	static float LabDistanceABangleIn(LabColor ci,LabColor cj);
	static float LabDistanceABangleOut(LabColor ci,LabColor cj);
	static float LabDistanceABangleInOut(LabColor ci,LabColor cj);

	static float LabDistanceLABangle(LabColor ci,LabColor cj);
};
/*-------------------------------------------------------------------------------------------*/
/**
*
*
*/
/*-------------------------------------------------------------------------------------------*/
