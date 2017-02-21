#pragma once
#include "ImageMemData.h"
#include "ColorAnalyzeOnLab.h"
/*--------------------------------------------------------*/
/**
*颜色分析
*获得超像素块中最具代表性的颜色
*\n（平均值）
*/
/*--------------------------------------------------------*/
class ColorAnalyzeOnLab
{
private:
	ImageMemData* pMD;/**<图像数据*/
public:
	ColorAnalyzeOnLab(ImageMemData* MemData_t);
	~ColorAnalyzeOnLab(void);
public:
	LabColor GetImageColorMostly();
public:
};
/*--------------------------------------------------------*/
/**
*
*/
/*--------------------------------------------------------*/

