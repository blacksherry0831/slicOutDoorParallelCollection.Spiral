#pragma once
#include <SGVcode/ImageMemData.h>
/*------------------------------------------*/
/**
*亮度差
*角度差
*向量的模长
*圆柱
*/
/*------------------------------------------*/
#ifndef Param_LabColorAnalyzeCluster
#define Param_LabColorAnalyzeCluster
/**<亮度距离差-内环小一点*/
#define  L_OutLightDstDelta  (18)
#define  L_InLightDstDelta   (9)
/**<角度距离差*/
#define  ABOutCircleDst		(8)
#define  ABInCircleDst		(8)
/**<模长距离差*/
#define  VectorLengthDeltaIn       (18)
#define  VectorLengthDeltaOut      (12)
/**<圆柱所在[统计得出]*/
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
*Androutsos等人通过实验对HSV颜色空间进行了大致划分，
*亮度大于75%并且饱和度大于20%为亮彩色区域，亮度小于25%为黑色区域，
*亮度大于75%并且饱和度小于20%为白色区域，其他为彩色区域。
*
*/
/*---------------------------------------------------------------------------*/
class LabColorAnalyzeCluster_Veins
{
private:
	ImageMemData* pMD;/**<需要的图像数据和图像计算用的中间变量*/
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
