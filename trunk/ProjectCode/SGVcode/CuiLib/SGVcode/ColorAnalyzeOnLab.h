#pragma once
#include "ImageMemData.h"
#include "ColorAnalyzeOnLab.h"
/*--------------------------------------------------------*/
/**
*��ɫ����
*��ó����ؿ�����ߴ����Ե���ɫ
*\n��ƽ��ֵ��
*/
/*--------------------------------------------------------*/
class ColorAnalyzeOnLab
{
private:
	ImageMemData* pMD;/**<ͼ������*/
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

