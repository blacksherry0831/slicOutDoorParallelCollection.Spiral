#pragma once
#include <platform_cui.h>
//#include <cv.h>

#include "SGVcode/ImageMemData.h"
//#if (CV_MAJOR_VERSION==2)&&(CV_MINOR_VERSION==4)
//#include <opencv2/ml/ml.hpp>
//#endif
//#if (CV_MAJOR_VERSION==3)
//#include <opencv2/ml.hpp> 
//#endif
/************************************************************************/
/*      
#if (CV_MAJOR_VERSION==2)&&(CV_MINOR_VERSION==4)

#endif	
#if (CV_MAJOR_VERSION==3)

#endif
*/
/************************************************************************/
class SeparateSpBlock
{
private:
	ImageMemData* pMD;/**<��Ҫ��ͼ�����ݺ�ͼ������õ��м����*/
	
#if (CV_MAJOR_VERSION==2)&&(CV_MINOR_VERSION==4)
   CvSVM SvmTest;/**<֧��������ģ��*/
#endif
#if (CV_MAJOR_VERSION==3)  
   Ptr<ml::SVM> SvmTest; /**<֧��������ģ��*/
#endif	
public:
	UINT32* ImgData;/**<ͼ������*/
	int*    ImgLab;/**<ͼ��궨Lables*/
	INT32 * Matrix_Category_Lable;/**<�궨�ĳ����صı궨��*/
public:
	SeparateSpBlock(UINT32* imgdata_t,int*  ImgLab_t,ImageMemData* MemData_t);
	~SeparateSpBlock(void);
public:
	void SeparateSp(void);
	void SeparateSp_SvmPredict(void);
};

