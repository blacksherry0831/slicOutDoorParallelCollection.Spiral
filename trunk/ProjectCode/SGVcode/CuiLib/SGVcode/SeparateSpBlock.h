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
	ImageMemData* pMD;/**<需要的图像数据和图像计算用的中间变量*/
	
#if (CV_MAJOR_VERSION==2)&&(CV_MINOR_VERSION==4)
   CvSVM SvmTest;/**<支持向量机模型*/
#endif
#if (CV_MAJOR_VERSION==3)  
   Ptr<ml::SVM> SvmTest; /**<支持向量机模型*/
#endif	
public:
	UINT32* ImgData;/**<图像数据*/
	int*    ImgLab;/**<图像标定Lables*/
	INT32 * Matrix_Category_Lable;/**<标定的超像素的标定集*/
public:
	SeparateSpBlock(UINT32* imgdata_t,int*  ImgLab_t,ImageMemData* MemData_t);
	~SeparateSpBlock(void);
public:
	void SeparateSp(void);
	void SeparateSp_SvmPredict(void);
};

