#include "StdAfx.h"
#include "module_all_cui.h"
//#include "SeparateSpBlock.h"
/*------------------------------------------------------------------*/
//#ifdef (linux)||(__linux)||(__linux__)||(__GNUC__)
//#include "FilePathName/FileNameSplit.h"
//#endif
#ifdef __GNUC__
#include "FilePathName/FileNameSplit.h"
#include "SGVcode/cui_GeneralImgProcess.h"
#include "SpAnalyze.h"
#endif

/*------------------------------------------------------------------*/
/**
*载入SVM支持向量机的学习模型
*@param imgdata_t 图像数据
*@param ImgLab_t 图像Labels
*@param MemData_t 图像数据
*/
/*-----------------------------------------------------------------*/
SeparateSpBlock::SeparateSpBlock(UINT32* imgdata_t,int*  ImgLab_t,ImageMemData* MemData_t)
{
	this->ImgData=imgdata_t;
	this->ImgLab=ImgLab_t;
	this->pMD=MemData_t;
#if (CV_MAJOR_VERSION==2)&&(CV_MINOR_VERSION==4)
	SvmTest.load("SvmModule(BV)2014-05-11.xml");
#endif
#if (CV_MAJOR_VERSION==3)  
	printf("OpenCv 3.1.0 use svm");
	//ASSERT(0);
#endif	
	
}
/*------------------------------------------------------------------*/
/**
*
*/
/*------------------------------------------------------------------*/
SeparateSpBlock::~SeparateSpBlock(void)
{

}
/*------------------------------------------------------------------*/
/**
*将每一个超像素块保存到硬盘
*/
/*------------------------------------------------------------------*/
void SeparateSpBlock::SeparateSp(void)
{
	string file_add="ReBuild";
	SP_PROPERTY* p_SpProperty=pMD->p_SpProperty;
	INT32* Matrix_Category_Lable=pMD->Matrix_Category_Lable.get();
	int ImgWidth=pMD->ImgWidth;
	int ImgHeight=pMD->ImgHeight;
	FileNameSplit fns;
#if Use_CString&&_MSC_VER
fns.Parse(FileNameSplit::Convertstring2CS(pMD->FileReadFullPath));
#endif
	
#if TRUE
	cui_GeneralImgProcess::CalculateAllSpPropertyRange(
		pMD->ImgLables.get(),
		pMD->ImgWidth,
		pMD->ImgHeight,
		pMD->p_SpProperty,
		pMD->slic_current_num);
#endif
	
	for (int spi=0;spi<pMD->slic_current_num;spi++){
		{
			int img_width=p_SpProperty[spi].max_x-p_SpProperty[spi].min_x;
			int img_height=p_SpProperty[spi].max_y-p_SpProperty[spi].min_y;
			std::shared_ptr<UINT32> Origin_imgMEM=std::shared_ptr<UINT32>(new UINT32[img_width*img_height]);
			std::shared_ptr<UINT32> Origin_img_NoBoderMEM=std::shared_ptr<UINT32>(new UINT32[img_width*img_height]);
			UINT32 *Origin_img=Origin_imgMEM.get();
			UINT32 *Origin_img_NoBoder=Origin_img_NoBoderMEM.get();
			for(register int x=p_SpProperty[spi].min_x;x<p_SpProperty[spi].max_x;x++){
				for (register int y=p_SpProperty[spi].min_y;y<p_SpProperty[spi].max_y;y++){
					int x_t=x-p_SpProperty[spi].min_x;
					int y_t=y-p_SpProperty[spi].min_y;
					ASSERT(x>=0&&x<ImgWidth);
					ASSERT(y>=0&&y<ImgHeight);
					Origin_img[y_t*img_width+x_t]=ImgData[y*ImgWidth+x];
					if (ImgLab[y*ImgWidth+x]==spi){
						Origin_img_NoBoder[y_t*img_width+x_t]=ImgData[y*ImgWidth+x];
					}else{
						Origin_img_NoBoder[y_t*img_width+x_t]=BlackColorPNG;
					}

				}
			}

			char buff[1024];
			string filename;
			IplImage  Img;
			string category_sp;
			cvInitImageHeader(&Img,cvSize(img_width,img_height),8,4);
			if (Matrix_Category_Lable[spi]==Sky){
				category_sp="_sky_";
			}
			if (Matrix_Category_Lable[spi]==Ground){
				category_sp="_ground_";
			}
			if ((Matrix_Category_Lable[spi]==Vertical)
				||(Matrix_Category_Lable[spi]==Vertical_Tree)
				||(Matrix_Category_Lable[spi]==Vertical_Building)){
					category_sp="_vertical_";
			}
#if Use_CString&&_MSC_VER		
			sprintf(buff,"_SPa_%d.png",spi);
			filename=pMD->FileWritePath+FileNameSplit::ConvertCS2string(fns.filename)+category_sp+buff;		
			Img.imageData=(char *)Origin_img;
			cvSaveImage(filename.c_str(),&Img);
#endif

#if Use_CString&&_MSC_VER		
			sprintf(buff,"_SPb_%d.png",spi);
			filename=pMD->FileWritePath+FileNameSplit::ConvertCS2string(fns.filename)+category_sp+buff;		
			Img.imageData=(char *)Origin_img_NoBoder;
			cvSaveImage(filename.c_str(),&Img);
#endif
		}
		
		

	}

}
/*------------------------------------------------------------------*/
/**
*利用支持向量机的模型分类每一个图块
*/
/*------------------------------------------------------------------*/
void SeparateSpBlock::SeparateSp_SvmPredict(void)
{
	string file_add="ReBuild";
	SP_PROPERTY* p_SpProperty=pMD->p_SpProperty;
	INT32* Matrix_Category_Lable=pMD->Matrix_Category_Lable.get();
	int ImgWidth=pMD->ImgWidth;
	int ImgHeight=pMD->ImgHeight;
	FileNameSplit fns;
#if Use_CString&&_MSC_VER
  fns.Parse(FileNameSplit::Convertstring2CS(pMD->FileReadFullPath));
#endif
	


#if TRUE
	cui_GeneralImgProcess::CalculateAllSpPropertyRange(
		pMD->ImgLables.get(),
		pMD->ImgWidth,
		pMD->ImgHeight,
		pMD->p_SpProperty,
		pMD->slic_current_num);
#endif
	for (int spi=0;spi<pMD->slic_current_num;spi++){
		{
			int img_width=p_SpProperty[spi].max_x-p_SpProperty[spi].min_x;
			int img_height=p_SpProperty[spi].max_y-p_SpProperty[spi].min_y;
			std::shared_ptr<UINT32> Origin_imgMEM=std::shared_ptr<UINT32>(new UINT32[img_width*img_height]);
			std::shared_ptr<UINT32> Origin_img_NoBoderMEM=std::shared_ptr<UINT32>(new UINT32[img_width*img_height]);
			UINT32 *Origin_img=Origin_imgMEM.get();
			UINT32 *Origin_img_NoBoder=Origin_img_NoBoderMEM.get();
			for(register int x=p_SpProperty[spi].min_x;x<p_SpProperty[spi].max_x;x++){
				for (register int y=p_SpProperty[spi].min_y;y<p_SpProperty[spi].max_y;y++){
					int x_t=x-p_SpProperty[spi].min_x;
					int y_t=y-p_SpProperty[spi].min_y;
					ASSERT(x>=0&&x<ImgWidth);
					ASSERT(y>=0&&y<ImgHeight);
					Origin_img[y_t*img_width+x_t]=ImgData[y*ImgWidth+x];
					if (ImgLab[y*ImgWidth+x]==spi){
						Origin_img_NoBoder[y_t*img_width+x_t]=ImgData[y*ImgWidth+x];
					}else{
						Origin_img_NoBoder[y_t*img_width+x_t]=BlackColorPNG;
					}

				}
			}
			if ((img_width+ImgHeight)==0){
				continue;
			}
			char buff[1024];
			string filename;
			IplImage  Img_Spa;
			IplImage  Img_Spb;
			string category_sp;
			cvInitImageHeader(&Img_Spa,cvSize(img_width,img_height),8,4);
			Img_Spa.imageData=(char *)Origin_img;
			cvInitImageHeader(&Img_Spb,cvSize(img_width,img_height),8,4);
			Img_Spb.imageData=(char *)Origin_img_NoBoder;

			if (Matrix_Category_Lable[spi]==Sky){
				category_sp="_sky_";
			}
			if (Matrix_Category_Lable[spi]==Ground){
				category_sp="_ground_";
			}
			if ((Matrix_Category_Lable[spi]==Vertical)
				||(Matrix_Category_Lable[spi]==Vertical_Tree)
				||(Matrix_Category_Lable[spi]==Vertical_Building)){
					category_sp="_vertical_";
			}
#if Use_CString&&_MSC_VER	
			//原图
			sprintf(buff,"_SPa_%d.png",spi);
			filename=pMD->FileWritePath+FileNameSplit::ConvertCS2string(fns.filename)+category_sp+buff;		
			cvSaveImage(filename.c_str(),&Img_Spa);
#endif

#if Use_CString&&_MSC_VER	
			//无轮廓，黑边
			sprintf(buff,"_SPb_%d.png",spi);
			filename=pMD->FileWritePath+FileNameSplit::ConvertCS2string(fns.filename)+category_sp+buff;		
			cvSaveImage(filename.c_str(),&Img_Spb);
#endif
#if (CV_MAJOR_VERSION==2)&&(CV_MINOR_VERSION==4)
		if (Img_Spb.width!=0&&Img_Spb.height!=0){
			SpAnalyze Spb;
			vector<float> feature;
			ImageMemData MemData(&Img_Spb,"",0,0,0.5);
			Spb.IsSpFatorThin(&MemData);
			Spb.SpSA=SpSizeAttr::FAT;
			Spb.ExtractImageFeatureWithSrcCany(feature);
			ASSERT(feature.size()==Spb.GetFeatureSize());	
			CvMat m;
			cvInitMatHeader(&m, 1,feature.size(), CV_32FC1,feature.data()); 
			int catagory=SvmTest.predict(&m);
			if (Matrix_Category_Lable[spi]==Vertical){
				if (catagory==SvmCategoryBuilding){
					Matrix_Category_Lable[spi]=Vertical_Building;
				}else  if (catagory==SvmCategoryTree){
					Matrix_Category_Lable[spi]=Vertical_Tree;
				}else{
					ASSERT(FALSE);
				}
			}
		}
#endif
#if (CV_MAJOR_VERSION==3)  
			printf("OpenCv 3.1.0 use svm");
			ASSERT(0);
#endif	

		}
	}
#if TRUE
{
	/*--------------------------------------------*/
	string file_add="TreeBuilding";
	/*--------------------------------------------*/
	cui_GeneralImgProcess::Cui_CombinationImgSVG(
		pMD->Src_ImgData,
		pMD->ImgLables.get(),
		pMD->p_ImgLables_SVG,
		pMD->Matrix_Category_Lable.get(),
		pMD->ImgWidth,
		pMD->ImgHeight,
		"","","");
	/*--------------------------------------------*/	
	cui_GeneralImgProcess::CuiSaveImgWithPoints(
		pMD->Src_ImgData,
		pMD->p_ImgLables_SVG,
		nullptr,
		pMD->ImgWidth,
		pMD->ImgHeight,
		true,
		pMD,
		file_add+"C");	
	/*--------------------------------------------*/	
	cui_GeneralImgProcess::CuiSaveImgWithPoints(
		pMD->Src_ImgData,
		pMD->p_ImgLables_SVG,
		nullptr,
		pMD->ImgWidth,
		pMD->ImgHeight,
		(bool)OUT_NOGROUND_IMG,
		pMD,
		file_add);	
	}
#endif
}
/*------------------------------------------------------------------*/
/**
*
*/
/*------------------------------------------------------------------*/