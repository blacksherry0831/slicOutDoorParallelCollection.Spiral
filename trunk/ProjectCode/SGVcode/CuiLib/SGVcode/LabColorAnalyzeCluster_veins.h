#pragma once
#include <SGVcode/ImageMemData.h>
/*------------------------------------------*/
/**
*���Ȳ�
*�ǶȲ�
*������ģ��
*Բ��
*/
/*------------------------------------------*/
#ifndef Param_LabColorAnalyzeCluster
#define Param_LabColorAnalyzeCluster
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
#endif
/*------------------------------------------*/
/**
*IMG_4095.jpg===
*
*/
/*------------------------------------------*/

/*---------------------------------------------------------------------------*/
/**
*Androutsos����ͨ��ʵ���HSV��ɫ�ռ�����˴��»��֣�
*���ȴ���75%���ұ��Ͷȴ���20%Ϊ����ɫ��������С��25%Ϊ��ɫ����
*���ȴ���75%���ұ��Ͷ�С��20%Ϊ��ɫ��������Ϊ��ɫ����
*
*/
/*---------------------------------------------------------------------------*/
class LabColorAnalyzeCluster_Veins
{
private:
	ImageMemData* pMD;/**<��Ҫ��ͼ�����ݺ�ͼ������õ��м����*/
public:
	LabColorAnalyzeCluster_Veins(ImageMemData* MemData_t);
	~LabColorAnalyzeCluster_Veins(void);
	void ColorPointIteration(void);
	void ColorPointIteration_Veins(void);
	void GetMatrix_E(void);
	void GetMatrix_W(void);
	void GetMatrix_W_Veins(void);
	void AdjustNighbour_W(
		double *Matrix_W_Color,
		double *Matrix_W_Vein,
		double *Matrix_W_Multi,
		int spcount,
		float Vein_threshold_SkyV,
		float Vein_threshold_GND,
		float Color_threshold_SkyV,
		float Color_threshold_GND,
		SP_PROPERTY *p_SpProperty);
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
