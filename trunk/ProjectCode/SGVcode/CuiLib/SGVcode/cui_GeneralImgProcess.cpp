#include "StdAfx.h"
#include "module_all_cui.h"
//#include "cui_GeneralImgProcess.h"
//#include "Harr/CuiHarrTransformLet.h"
/*----------------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------------*/

#ifdef _DEBUG
 int cui_GeneralImgProcess::SAVE_DEBUG_2DISK=TRUE;
#else
 int cui_GeneralImgProcess::SAVE_DEBUG_2DISK=FALSE;
#endif

 int cui_GeneralImgProcess::SAVE_IMAGE_2DISK=TRUE;
 //struct pt_sem cui_GeneralImgProcess::SEM_CPU_NUMS;
#if _MSC_VER

 HANDLE cui_GeneralImgProcess::H_SEM_CPU_NUMS = NULL;  

#endif
/*----------------------------------------------------------------*/
/**
*���캯��\n
*
*
*/
/*----------------------------------------------------------------*/
cui_GeneralImgProcess::cui_GeneralImgProcess(void)
{
}
/*----------------------------------------------------------------*/
/**
*��������
*
*
*/
/*----------------------------------------------------------------*/

cui_GeneralImgProcess::~cui_GeneralImgProcess(void)
{
}
/*----------------------------------------------------------------*/
/**
*�����ҵĳ����ر�Ŵ�0������N
*
*@param [in out] CuiImgLables_t �����ر궨ͼƬ������
*@param width �궨����Ŀ��
*@param height �궨����ĳ���
*@param sp_num �����صĸ���
*
*/
/*----------------------------------------------------------------*/
int cui_GeneralImgProcess::AdjustLabFrom0toN(
	int *CuiImgLables_t,
	int width,
	int height,
	int sp_num)
{

	/******** ��׼��lable*******************************************************/
	vector<int> cui_t(sp_num,-1);
#if 1
	int i_new=0;
	for (register int mi=0;mi<height;mi++){
		for (register int mj=0;mj<width;mj++){
			int i_old=CuiImgLables_t[mi*width+mj];

			if (cui_t[i_old]==-1){
				cui_t[i_old]=i_new++;
			}			
			 CuiImgLables_t[mi*width+mj]=cui_t[i_old];
		}
	}
	return i_new;
#endif
}
/*-------------------------------------------------------------------------------------------*/
//11��9��- 11:03-��ͼ57S
//void cui_GeneralImgProcess::Cui_Combination_ImgLabsOld(unsigned int*	ubuff,

//11��9��- 11:03-��ͼ24S
/*----------------------------------------------------------------*/
/**
*�Թ�Ϊͬһ��ĳ����ؽ��кϲ�����\n
*ͬʱ�������ݵ�Ӳ��
*
*@param	[in] ubuff ԭͼ�����
*@param	[in out]label �����ر궨����
*@param	[in] category ÿ�������ض�Ӧ�ķ������
*@param	[in] cui_Matrix_W ���ƾ���W		
*@param	numlabels �����صĸ���
*@param	width  ͼƬ�Ŀ��
*@param	height ͼƬ�ĸ߶�
*@param	SuperpixelThread ��Ϊͬһ��ĳ����ص�������ֵ�������ã�
*@param	 pMD ͼ�����������ݣ�������Ҫ���ñ���·����
*
*/
/*----------------------------------------------------------------*/
void cui_GeneralImgProcess::Cui_Combination_ImgLabs2(
	unsigned int*			ubuff,
	int*					labels,
	INT32*			   category,
	const double*          cui_Matrix_W,		
	int&				numlabels,
	const int&				width,
	const int&				height,
	double SuperpixelThread,
	ImageMemData* pMD)
{   
	/*************************************************************/
	bool IS_Force_CombineLabs;
	if (cui_Matrix_W){ 
#if _DEBUG
		if(cui_GeneralImgProcess::SAVE_DEBUG_2DISK){
			char data_t[1024];
			ofstream outfile;
			outfile.open("Matrix_W_Combin.data",ios::out);
			for(register int i = 0; i <numlabels; i++ ){
				for(register int j = 0; j <numlabels; j++ ){
					double value_t=cui_Matrix_W[i*numlabels+j];

					sprintf(data_t," %0.2f ",value_t);

					outfile<<data_t;
				}
				outfile<<endl;
			} 
			outfile.close();
		}
#endif	
		IS_Force_CombineLabs=false;
	}else{
		IS_Force_CombineLabs=true;
	}

	/*************�ռ����������************************************/
	int *CuiImgLables_t=NULL;
	SP_PROPERTY *p_SpProperty=NULL;
	CuiImgLables_t=new int[width*height];
	p_SpProperty=new SP_PROPERTY[numlabels];
	memcpy_s(CuiImgLables_t,sizeof(int)*width*height,labels,sizeof(int)*width*height);
	/**************************************************************************************/
	cui_GeneralImgProcess::CalculateAllSpPropertyRange(labels,width,height,p_SpProperty,numlabels);
	/***********************�ϲ���ͬLable*************************************************/
	for (register int spi=0;spi<numlabels;spi++){
		for (register int spj=spi+1;spj<numlabels;spj++){ 
			//������i,j ����ͬһ�ࣻ
			if (category[spi]== category[spj]){
				//�ж�����				
				if (IS_Force_CombineLabs||cui_Matrix_W[spi*numlabels+spj]>SuperpixelThread){
					//���ƶȴ���0.9��
					/*************FindSetValue***************************/
#if 1

					int SetValue=0;
					for(register int x=p_SpProperty[spi].min_x;x<=p_SpProperty[spi].max_x;x++){
						for (register int y=p_SpProperty[spi].min_y;y<=p_SpProperty[spi].max_y;y++){
							if (labels[y*width+x]==spi){
								SetValue=CuiImgLables_t[y*width+x];
								goto CUI_NEXT;
							}
						}
					}
#endif
					
CUI_NEXT:	;
					/**************SetValue*************************/
#if 1
					for(register int x=p_SpProperty[spj].min_x;x<=p_SpProperty[spj].max_x;x++){
						for (register int y=p_SpProperty[spj].min_y;y<=p_SpProperty[spj].max_y;y++){
							if (labels[y*width+x]==spj){

								CuiImgLables_t[y*width+x]=SetValue;

							}
						}
					}
#endif
					
					/****************************************/
				}
			}
		}
	} 
#if 1
/******** ��׼��lable*******************************************************/
numlabels=AdjustLabFrom0toN(CuiImgLables_t,width,height,numlabels);
/***************************************************************/
#endif
	

	memcpy_s(labels,sizeof(int)*width*height,CuiImgLables_t,sizeof(int)*width*height);
	delete []CuiImgLables_t;
	delete []p_SpProperty;
	//cui_GeneralImgProcess::SaveSuperpixelLabels(labels,width,height);
	/**************************************************************/
}
/*---------------------------------------------------------------*/
/**
*                                                                    
*
**/
/*---------------------------------------------------------------*/
void cui_GeneralImgProcess::Cui_Combination_ImgLabs2(
	unsigned int*			ubuff,
	int*					labels,
	INT32*			   category,
	const double*          cui_Matrix_W,		
	int&				numlabels,
	const int&				width,
	const int&				height,
	double SuperpixelThread,
	ImageData* pMD)
{   
	/*************************************************************/
	bool IS_Force_CombineLabs;
	if (cui_Matrix_W){ 
#if _DEBUG
		if(cui_GeneralImgProcess::SAVE_DEBUG_2DISK){
					char data_t[1024];
					ofstream outfile;
					outfile.open("Matrix_W_Combin.data",ios::out);
					for(register int i = 0; i <numlabels; i++ ){
						for(register int j = 0; j <numlabels; j++ ){
							double value_t=cui_Matrix_W[i*numlabels+j];
							sprintf(data_t," %0.2f ",value_t);
							outfile<<data_t;
						}
						outfile<<endl;
					} 
					outfile.close();
		}
#endif
		IS_Force_CombineLabs=false;
	}else{
		IS_Force_CombineLabs=true;
	}

	/*************�ռ����������************************************/
	int *CuiImgLables_t=NULL;
	SP_PROPERTY *p_SpProperty=NULL;
	CuiImgLables_t=new int[width*height];
	p_SpProperty=new SP_PROPERTY[numlabels];
	memcpy_s(CuiImgLables_t,sizeof(int)*width*height,labels,sizeof(int)*width*height);
	/**************************************************************************************/
	cui_GeneralImgProcess::CalculateAllSpPropertyRange(labels,width,height,p_SpProperty,numlabels);
	/***********************�ϲ���ͬLable*************************************************/
	for (register int spi=0;spi<numlabels;spi++){
		for (register int spj=spi+1;spj<numlabels;spj++){ 
			//������i,j ����ͬһ�ࣻ
			if (category[spi]== category[spj]){
				//�ж�����				
				if (IS_Force_CombineLabs||cui_Matrix_W[spi*numlabels+spj]>SuperpixelThread){
					//���ƶȴ���0.9��
					/*************FindSetValue***************************/
#if 1

					int SetValue=0;
					for(register int x=p_SpProperty[spi].min_x;x<=p_SpProperty[spi].max_x;x++){
						for (register int y=p_SpProperty[spi].min_y;y<=p_SpProperty[spi].max_y;y++){
							if (labels[y*width+x]==spi){
								SetValue=CuiImgLables_t[y*width+x];
								goto CUI_NEXT;
							}
						}
					}
#endif

CUI_NEXT:	;
					/**************SetValue*************************/
#if 1
					for(register int x=p_SpProperty[spj].min_x;x<=p_SpProperty[spj].max_x;x++){
						for (register int y=p_SpProperty[spj].min_y;y<=p_SpProperty[spj].max_y;y++){
							if (labels[y*width+x]==spj){

								CuiImgLables_t[y*width+x]=SetValue;

							}
						}
					}
#endif

					/****************************************/
				}
			}
		}
	} 
#if 1
	/******** ��׼��lable*******************************************************/
	numlabels=AdjustLabFrom0toN(CuiImgLables_t,width,height,numlabels);
	/***************************************************************/
#endif


	memcpy_s(labels,sizeof(int)*width*height,CuiImgLables_t,sizeof(int)*width*height);
	delete []CuiImgLables_t;
	delete []p_SpProperty;

#if _DEBUG
	cui_GeneralImgProcess::SaveSuperpixelLabels(labels,width,height);
	cui_GeneralImgProcess::CuiSaveImgWithContours(ubuff,labels,width,height,pMD->FileReadFullPath,pMD->FileWritePath,"");
	/**************************************************************/
#endif
	
}
/*---------------------------------------------------------------*/
/**
* ������ͬ                                                                    
* ����
* ͬһ����
**/
/*---------------------------------------------------------------*/
void cui_GeneralImgProcess::Cui_Combination_ImgLabsNew(
	vector<vector<int> > simlarSp,
	ImageData* pMD)
{
#if 0
UINT32* Matrix_E=pMD->Matrix_E;
	
	int sameNumSp=simlarSp.size();
	for (int i=0;i<sameNumSp;i++){
		int sameNumlabels=simlarSp[i].size();
		for(int spi=0;spi<sameNumlabels;spi++){
			for (int spj=spi+1;spi<sameNumlabels;spi++){
				if (Matrix_E[spi*sameNumlabels+spj]!=0){
					pMD->SpSet.Combine2in1(
						simlarSp[i][spi],
						simlarSp[i][spj],
						false);
				}				
			}
		}
	}
	pMD->SpSet.AdjustSpSet();
	pMD->SetImgLabels();
#endif
	
}
/*----------------------------------------------------------------*/
/**
*�Թ�Ϊͬһ��ĳ����ؽ��кϲ�����\n
*ͬʱ�������ݵ�Ӳ��
*
*@param	[in] ubuff ԭͼ�����
*@param	[in out]label �����ر궨����
*@param	[in] category ÿ�������ض�Ӧ�ķ������
*@param	[in] cui_Matrix_W ���ƾ���W		
*@param	numlabels �����صĸ���
*@param	width  ͼƬ�Ŀ��
*@param	height ͼƬ�ĸ߶�
*@param	SuperpixelThread ��Ϊͬһ��ĳ����ص�������ֵ�������ã�
*@param	 pMD ͼ�����������ݣ�������Ҫ���ñ���·����
*
*/
/*----------------------------------------------------------------*/
void cui_GeneralImgProcess::Cui_Combination_ImgLabsWithTextureInColor(
	unsigned int*			ubuff,
	int*					labels,
	INT32*			   category,
	const double*          cui_Matrix_W,		
	int&				numlabels,
	const int&				width,
	const int&				height,
	double SuperpixelThread,
	ImageMemData* pMD)
{   
	DetermineColorRank(pMD,8);
	/*************************************************************/
	bool IS_Force_CombineLabs;
	if (cui_Matrix_W){ 
		if(cui_GeneralImgProcess::SAVE_DEBUG_2DISK){
			char data_t[1024];
			ofstream outfile;
			outfile.open("Matrix_W_Combin.data",ios::out);
			for(register int i = 0; i <numlabels; i++ ){
				for(register int j = 0; j <numlabels; j++ ){
					double value_t=cui_Matrix_W[i*numlabels+j];
					sprintf(data_t," %0.2f ",value_t);
					outfile<<data_t;
				}
				outfile<<endl;
			} 
			outfile.close();
		}
		
		IS_Force_CombineLabs=false;
	}else{
		IS_Force_CombineLabs=true;
	}

	/*************�ռ����������************************************/
	int *CuiImgLables_t=NULL;
	SP_PROPERTY *p_SpProperty=NULL;
	CuiImgLables_t=new int[width*height];
	p_SpProperty=new SP_PROPERTY[numlabels];
	memcpy_s(CuiImgLables_t,sizeof(int)*width*height,labels,sizeof(int)*width*height);
	/**************************************************************************************/
	cui_GeneralImgProcess::CalculateAllSpPropertyRange(labels,width,height,p_SpProperty,numlabels);
	/***********************�ϲ���ͬLable*************************************************/
	for (register int spi=0;spi<numlabels;spi++){
		for (register int spj=spi+1;spj<numlabels;spj++){ 
			//������i,j ����ͬһ�ࣻ
			if (category[spi]== category[spj]){
				//�ж�����				
				if (IS_Force_CombineLabs||cui_Matrix_W[spi*numlabels+spj]>SuperpixelThread){

					if(pMD->p_SpProperty[spi].harr_energy.EnergyGrade==pMD->p_SpProperty[spj].harr_energy.EnergyGrade){				
	//���ƶȴ���0.9��
					/*************FindSetValue***************************/
#if 1

					int SetValue=0;
					for(register int x=p_SpProperty[spi].min_x;x<=p_SpProperty[spi].max_x;x++){
						for (register int y=p_SpProperty[spi].min_y;y<=p_SpProperty[spi].max_y;y++){
							if (labels[y*width+x]==spi){
								SetValue=CuiImgLables_t[y*width+x];
								goto CUI_NEXT;
							}
						}
					}
#endif
					
CUI_NEXT:	;
					/**************SetValue*************************/
#if 1
					for(register int x=p_SpProperty[spj].min_x;x<=p_SpProperty[spj].max_x;x++){
						for (register int y=p_SpProperty[spj].min_y;y<=p_SpProperty[spj].max_y;y++){
							if (labels[y*width+x]==spj){

								CuiImgLables_t[y*width+x]=SetValue;

							}
						}
					}
#endif					
				    }					
				}				
			}
		}
	} 
#if 1
/******** ��׼��lable*******************************************************/
numlabels=AdjustLabFrom0toN(CuiImgLables_t,width,height,numlabels);
/***************************************************************/
#endif
	

	memcpy_s(labels,sizeof(int)*width*height,CuiImgLables_t,sizeof(int)*width*height);
	delete []CuiImgLables_t;
	delete []p_SpProperty;
	cui_GeneralImgProcess::SaveSuperpixelLabels(labels,width,height);
	cui_GeneralImgProcess::CuiSaveImgWithContours(ubuff,labels,width,height,pMD->FileReadFullPath,pMD->FileWritePath,"");
	/**************************************************************/
}
/*----------------------------------------------------------------*/
/**
*�Թ���õĳ����ؿ飬������գ����棬����ķ������\n
*�������ݵ�Ӳ��
*
*@param [in] ubuff      ͼ�����
*@param [in] labels     �����ر궨����
*@param [out] labels_SVG 
*@param [in] category ��ա����桢����Ĺ������
*@param width     ͼƬ���
*@param height    ͼƬ�߶�
*@param filereadfullpath  ԭͼ·��
*@param filewritepath  �ļ�����·��
*@param  fileadd      ����ͼƬʱ�ĸ����ַ���
*
*/
/*----------------------------------------------------------------*/
void cui_GeneralImgProcess::Cui_CombinationImgSVG(
	UINT32* ubuff,
	int* labels,
	int* labels_SVG,
	INT32*			   category,
	const int&				width,
	const int&				height,
	string        filereadfullpath,
	string        filewritepath,
	string fileadd)
{
	
			// 1 //2 //3
#if 0
	    /********************unknow****G**********V************S******/ 
	    const UINT32 color[10]={0x00000000,0x004D9D7B,0x00ff0000,0x004B4587};
	    //const UINT32 color[4]={0x7f000000,0x00ff0000,0x004D9D7B,0x004B4587};
		/******************black**********r***********g*******b******/
#else
			//G-1 //V-2 //S-3
	        INT32* catesvg=InitSP2SVGcategory();
#endif
		/********************************************************/
	for (register int i=0;i<height;i++){
		for (register int j=0;j<width;j++){
			int sp_t=labels[i*width+j];
			int cate_i=category[sp_t];

			ASSERT(catesvg[cate_i]>=1&&catesvg[cate_i]<=5);
#if 0
			if (cate_i==Vertical_Tree||cate_i==Vertical_Building){
				TRACE("Vertical Tree or Building sp %d==cate %d\n ",sp_t,cate_i);
			}
#endif
			
			
			labels_SVG[i*width+j]=catesvg[cate_i];
		}
	}
	/********************************************************/
			delete[] catesvg;
}
/*-------------------------------------------------------------------------------------------*/
/**
*
*@param category ��������   
*@param size ���鳤��   
*
*/
/*-------------------------------------------------------------------------------------------*/
INT32* cui_GeneralImgProcess::InitSP2SVGcategory(INT32* category,const int size)
{
#if 0
	static  INT32 Matrix_Category_Lable_SVG[10]={0,Ground,Vertical,Sky};
	
#else
	INT32* Matrix_Category_Lable_SVG=new INT32[size+10];
	Matrix_Category_Lable_SVG[0]=0;
	Matrix_Category_Lable_SVG[1]=Ground;
	Matrix_Category_Lable_SVG[2]=Vertical;
	Matrix_Category_Lable_SVG[3]=Sky;
	
#endif

	if (category==nullptr){
		category=(INT32*)Matrix_Category_Lable_SVG;
		//size=sizeof(Matrix_Category_Lable_SVG)/sizeof(INT32);
	}
	memset(category,0,sizeof(INT32)*(size+10));
	category[Sky]=Sky;
	category[Ground]=Ground;
	category[Vertical]=Vertical;
	category[Vertical_Tree]=Vertical_Tree;
	category[Vertical_Building]=Vertical_Building;
	return category;

}
/*-------------------------------------------------------------------------------------------*/
/**
*��ʼ����ɫ��\n
*��ɫ��Ϊȫ�־�̬����
*@param tab ��ɫ��
*-��ɫ��ֵ
*@param size ��ɫ��Ĵ�С
*@return ��ɫ��
*@retval ��̬�� ���tab==nullptr
*@retval �Ǿ�̬�� ���tab!=nullptr
*/
/*-------------------------------------------------------------------------------------------*/
UINT32* cui_GeneralImgProcess::InitColorTable(UINT32* tab, int size)
{
	 static  UINT32 ColorTab[10]={UnknowColorPNG,GroundColorPNG,VerticalColorPNG,SkyColorPNG,VerticalTreeColorPNG,VerticalBuildingColorPNG};
	if (tab==nullptr){
		tab=(UINT32*)ColorTab;
		size=sizeof(ColorTab)/sizeof(UINT32);
	}
	 for (int i=0;i<size;i++){
		 tab[i]=BlackColorPNG;
	 }
	 tab[Unknow]=UnknowColorPNG;
	 tab[Sky]=SkyColorPNG;
	 tab[Vertical]=VerticalColorPNG;
	 tab[Ground]=GroundColorPNG;
	 tab[Vertical_Tree]=VerticalTreeColorPNG;
	 tab[Vertical_Building]=VerticalBuildingColorPNG;
	 return tab;
}
/*----------------------------------------------------------------*/
/**
*����ͼƬ������ա����桢����ı궨��Ϣ���ﵽͼƬ��
*
*@param [in] ubuff      ͼ�����
*@param [in] labels     �����ر궨����
*@param [in] category ��ա����桢����Ĺ������
*@param width     ͼƬ���
*@param height    ͼƬ�߶�
*@param withcontours  �궨��ա����桢����ʱ���Ƿ�������
*@param filereadfullpath �ļ���ȡ·��
*@param filewritepath  �ļ�д��·��
*@param  fileadd  ����ͼƬʱ�ĸ����ַ���
*
*/
/*----------------------------------------------------------------*/
void cui_GeneralImgProcess::CuiSaveImgWithPoints(
	UINT32*			ubuff,
	int*			labels,
	INT32*			category,
	const int&		width,
	const int&		height,
	bool			withcontours,
	ImageMemData*	pMD,
	string fileadd)
{
	UINT32 * imgbuf_t=new UINT32[width*height];
	IplImage *imgdata_t;
	imgdata_t=cvCreateImageHeader(cvSize(width,height),8,4);
	imgdata_t->imageData=(char*)imgbuf_t;
	memcpy(imgbuf_t,ubuff,sizeof(UINT32)*width*height);
	INT32*	category_new=NULL;
	if (category==nullptr){
		category_new=InitSP2SVGcategory();
		category=category_new;
	}
	/********************unknow****G**********V************S******/
#if  OUT_NOGROUND_IMG
	const UINT32 color[10]={0xff000000,0xffff0000,0xffff0000,0xff4B4587};
#else
	UINT32 *color=InitColorTable();
#endif
	/******************black**********r***********g*******b******/
	/*****�������******************************************/
		   for (register int i=0;i<height;i+=2){
			   for (register int j=0;j<width;j+=1){
				    int sp_t=labels[i*width+j];
					int color_i=  category[sp_t];
					if (color_i<0){
						imgbuf_t[i*width+j]=imgbuf_t[i*width+j];
					}else{
						imgbuf_t[i*width+j]=color[color_i];
					}
#if FALSE
					if (color_i==Vertical_Tree){
						TRACE("Tree\n");
					}
					if (color_i==Vertical_Building){
						TRACE("Building\n");
					}
#endif
					
			   }
		   }
	/**************��������*********************************/
			if (withcontours){
				DrawContoursAroundSegments(imgbuf_t,labels,width,height,BlackColorPNG);
			}

#if CUI_DRAW_SC_INDEX
		  if (category!=category_new){
				cvLine(imgdata_t,cvPoint(0,pMD->Seg_HorizontalLinePos),cvPoint(imgdata_t->width,pMD->Seg_HorizontalLinePos),cvScalar(255,255,255,255),3);
				float HgPos= pMD->Seg_HorizontalLinePos+pMD->PgOffset*pMD->ImgHeight;
				float HsPos= pMD->Seg_HorizontalLinePos-pMD->PsOffset*pMD->ImgHeight;
				cvLine(imgdata_t,cvPoint(0,HgPos),cvPoint(imgdata_t->width,HgPos),cvScalar(255,255,255,255),3);
				cvLine(imgdata_t,cvPoint(0,HsPos),cvPoint(imgdata_t->width,HsPos),cvScalar(255,255,255,255),3);
				DrawTextOnImage(imgbuf_t,labels,width,height); 
		  }
		
#endif   	  
		
		  cvReleaseImageHeader(&imgdata_t);
	/***********************************************/
	if (fileadd==""){
		fileadd=cuiGetCurrentTime();
	}
	CuiSaveImageData(imgbuf_t,width,height,pMD->FileReadFullPath,pMD->FileWritePath, 1,fileadd);
	delete []imgbuf_t;
	delete []category_new;
}
/*----------------------------------------------------------------*/
/**
*����ͼƬ������ա����桢����ı궨��Ϣ���ﵽͼƬ��
*
*@param [in] ubuff      ͼ�����
*@param [in] labels     �����ر궨����
*@param [in] category ��ա����桢����Ĺ������
*@param width     ͼƬ���
*@param height    ͼƬ�߶�
*@param withcontours  �궨��ա����桢����ʱ���Ƿ�������
*@param filereadfullpath �ļ���ȡ·��
*@param filewritepath  �ļ�д��·��
*@param  fileadd  ����ͼƬʱ�ĸ����ַ���
*
*/
/*----------------------------------------------------------------*/
void cui_GeneralImgProcess::CuiSaveImgWithPoints(
	UINT32*			ubuff,
	int*			labels,
	INT32*			category,
	const int&		width,
	const int&		height,
	bool			withcontours,
	string        filereadfullpath,
	string        filewritepath,
	string fileadd)
{

	if (SAVE_IMAGE_2DISK==FALSE){
			printf("SaveImgWithPoints: --not-- \n");
		return ;
	}else{
				IplImage *imgdata_t;
				INT32*	 category_new=NULL;
				UINT32 * imgbuf_t=new UINT32[width*height];
				imgdata_t=cvCreateImageHeader(cvSize(width,height),8,4);
				imgdata_t->imageData=(char*)imgbuf_t;
				memcpy(imgbuf_t,ubuff,sizeof(UINT32)*width*height);
				if (category==nullptr){
					category_new=InitSP2SVGcategory();
					category=category_new;
				}
				/********************unknow****G**********V************S******/
#if  OUT_NOGROUND_IMG
				const UINT32 color[10]={0xff000000,0xffff0000,0xffff0000,0xff4B4587};
#else
				UINT32 *color=InitColorTable();
#endif
				/******************black**********r***********g*******b******/
				/*****�������******************************************/
				for (register int i=0;i<height;i+=2){
					for (register int j=0;j<width;j+=1){
						int sp_t=labels[i*width+j];
						int color_i=  category[sp_t];
						if (color_i<0){
							imgbuf_t[i*width+j]=imgbuf_t[i*width+j];
						}else{
							imgbuf_t[i*width+j]=color[color_i];
						}
#if FALSE
						if (color_i==Vertical_Tree){
							TRACE("Tree\n");
						}
						if (color_i==Vertical_Building){
							TRACE("Building\n");
						}
#endif

					}
				}
				/**************��������*********************************/
				if (withcontours){
					DrawContoursAroundSegments(imgbuf_t,labels,width,height,BlackColorPNG);
				}

#if CUI_DRAW_SC_INDEX
				if (category!=category_new){
					if (SAVE_DEBUG_2DISK){
						//cvLine(imgdata_t,cvPoint(0,pMD->Seg_HorizontalLinePos),cvPoint(imgdata_t->width,pMD->Seg_HorizontalLinePos),cvScalar(255,255,255,255),3);
						cvLine(imgdata_t,cvPoint(0,height/2),cvPoint(imgdata_t->width,height/2),cvScalar(255,255,255,255),3);
						/*float HgPos= pMD->Seg_HorizontalLinePos+pMD->PgOffset*pMD->ImgHeight;
						float HsPos= pMD->Seg_HorizontalLinePos-pMD->PsOffset*pMD->ImgHeight;
						cvLine(imgdata_t,cvPoint(0,HgPos),cvPoint(imgdata_t->width,HgPos),cvScalar(255,255,255,255),3);
						cvLine(imgdata_t,cvPoint(0,HsPos),cvPoint(imgdata_t->width,HsPos),cvScalar(255,255,255,255),3)*/
					}		
					DrawTextOnImage(imgbuf_t,labels,width,height); 
				}

#endif   	  

				cvReleaseImageHeader(&imgdata_t);
				/***********************************************/
				if (fileadd==""){
					fileadd=cuiGetCurrentTime();
				}
				CuiSaveImageData(imgbuf_t,width,height,filereadfullpath,filewritepath, 1,fileadd);
				delete []imgbuf_t;
				delete []category_new;

	}
	
}
/*----------------------------------------------------------------*/
/**
*��ͼ����󱣴浽Ӳ��
*
*@param 	imgBuffer  ͼ�����
*@param     width      ͼ����
*@param 	height     ͼ��߶�
*@param 	outFilename  ͼ��Ҫ������ļ���
*@param 	saveLocation  ͼ��Ҫ�����·��
*@param 	format     ͼƬ��ʽ
*@param 	str       ����ͼƬʱ�ĸ����ַ���
*
*/
/*----------------------------------------------------------------*/
void cui_GeneralImgProcess::CuiSaveImageData(
	UINT32*				imgBuffer,
	int					width,
	int					height,
	string&				outFilename,
	string&				saveLocation,
	int					format,
	const string&		str)
{
	if (SAVE_IMAGE_2DISK==FALSE){
		printf("SaveImageData: --not-- \n");
	}else{
		int sz = width*height;
		IplImage *Save_Image_t;
		Save_Image_t=cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,4);
		memcpy(Save_Image_t->imageData,imgBuffer,4*width*height);
		//-----------------------------------------
		// Prepare path and save the result images
		//-----------------------------------------
		string path = saveLocation;
		char fname[_MAX_FNAME];
		_splitpath(outFilename.c_str(), NULL, NULL, fname, NULL);
		path += fname;

		if( 0 != str.compare("") ) path.append(str);
		if( 1 == format ) path.append(".png");
		if( 0 == format ) path.append(".bmp");
	
		printf("cvSaveImage: %s \n",path.c_str());
		cvSaveImage(path.c_str(),Save_Image_t);
		cvReleaseImage(&Save_Image_t);
	}

	

}
/*----------------------------------------------------------------*/
/**
*��õ�ǰʱ��\n
*������ӵ��ļ����ϱ��ʱ���
*
*@return ��ǰʱ���ַ���
*/
/*----------------------------------------------------------------*/
inline string cui_GeneralImgProcess::cuiGetCurrentTime(void)
{
	SLIC slic;
	return slic.cuiGetCurrentTime();
}
/*----------------------------------------------------------------*/
/**
*CString ת string
* 
*@param  Cstring�ַ��� 
*@return string�ַ���
*
*/
/*----------------------------------------------------------------*/
#if Use_CString&&_MSC_VER
string cui_GeneralImgProcess::ConvertCS2string(CString cstring)
{
	CStringA stra(cstring.GetBuffer(0));
	cstring.ReleaseBuffer();	
	std::string cui_t=stra;		
	stra.ReleaseBuffer();
	return cui_t;
}



#endif

/*----------------------------------------------------------------*/
/**
*��ԭͼ�ϻ��Ƴ���������
*
*@param [in ouut]	ubuff ԭͼ�����
*@param	[in]  labels  ͼ��ĳ����ر궨����
*@param		width  ͼ����
*@param		height ͼ��߶�
*@param		color  ������������ɫ��32Ϊint��[a r g b]
*
*/
/*----------------------------------------------------------------*/
void cui_GeneralImgProcess::DrawContoursAroundSegments(
	unsigned int*			ubuff,
	int*					labels,
	const int&				width,
	const int&				height,
	const unsigned int&		color )
{
#if 0
const int dx8[8] = {-1, -1,  0,  1, 1, 1, 0, -1};
	const int dy8[8] = { 0, -1, -1, -1, 0, 1, 1,  1};

	int sz = width*height;
	vector<bool> istaken(sz, false);
	vector<int> contourx(sz);
	vector<int> contoury(sz);
	int mainindex(0);int cind(0);
	for(register int j = 0; j < height; j++ )
	{
		for(register int k = 0; k < width; k++ ){
									int np(0);
									for(register int i = 0; i < 8; i++ ){
										int x = k + dx8[i];
										int y = j + dy8[i];

										if( (x >= 0 && x < width) && (y >= 0 && y < height) ){
													int index = y*width + x;

													//if( false == istaken[index] )//comment this to obtain internal contours
													{
														if( labels[mainindex] != labels[index] ) np++;
													}
										}
									}
									  ///////////////////////////////////////////////////
									if( np > 1 ){
										contourx[cind] = k;
										contoury[cind] = j;
										istaken[mainindex] = true;
										//img[mainindex] = color;
										cind++;
									}
									////////////////////////////////////////////////////
						  			mainindex++;
		        }
	}

	int numboundpix = cind;//int(contourx.size());
	for(register int j = 0; j < numboundpix; j++ ){
				int ii = contoury[j]*width + contourx[j];
				ubuff[ii] =color;//2pixel����

				for(register int n = 0; n < 8; n++ ){
					int x = contourx[j] + dx8[n];
					int y = contoury[j] + dy8[n];
					if( (x >= 0 && x < width) && (y >= 0 && y < height) ){
						int ind = y*width + x;
						if(!istaken[ind]) 
							ubuff[ind] =0xff000000; //2pixel����
							//ubuff[ind] =color;
					}
				}
	}
#else
DrawContoursAroundSegments_cuda(ubuff,labels,width,height,color);
#endif
}
/*----------------------------------------------------------------*/
/**
*��ԭͼ�ϻ��Ƴ���������
*
*@param [in ouut]	ubuff ԭͼ�����
*@param	[in]  labels  ͼ��ĳ����ر궨����
*@param		width  ͼ����
*@param		height ͼ��߶�
*@param		color  ������������ɫ��32Ϊint��[a r g b]
*
*/
/*----------------------------------------------------------------*/
void cui_GeneralImgProcess::DrawContoursAroundSegments_2PixelBlack(
	unsigned int*			ubuff,
	int*					labels,
	const int&				width,
	const int&				height)
{

}
/*----------------------------------------------------------------*/
/**
*��ԭͼ�ϻ��Ƴ���������
*
*@param [in ouut]	ubuff ԭͼ�����
*@param	[in]  labels  ͼ��ĳ����ر궨����
*@param		width  ͼ����
*@param		height ͼ��߶�
*@param		color  ������������ɫ��32Ϊint��[a r g b]
*
*/
/*----------------------------------------------------------------*/
void cui_GeneralImgProcess::DrawContoursAroundSegments_4PixelBlack(
	unsigned int*			ubuff,
	int*					labels,
	const int&				width,
	const int&				height)
{

}
/*-------------------------------------------------------------------------------------------*/
/**
*����ͼ��Ӳ��ʱ����ͼ�������
*ͼ��ubuff������仯
*
*@param  ubuff  ͼ��ԭʼ����
*@param  labels ͼ��ĳ����ر궨Labels
*@param	 width  ͼ����
*@param	 height ͼ��߶�
*@param	 filereadfullpath  ͼ��Ķ�ȡ·��
*@param  filewritepath    ͼ���д��·��
*@param  fileadd       ��ӵ�ͼ���ļ����еĶ����ַ���
*
*/
/*-------------------------------------------------------------------------------------------*/
void cui_GeneralImgProcess::CuiSaveImgWithContours(
	UINT32* ubuff,
	int* labels,
	const int&				width,
	const int&				height,
	string        filereadfullpath,
	string        filewritepath,
	string fileadd)
{
#if _MSC_VER&&_DEBUG
LARGE_INTEGER litmp;
	LONGLONG QPart1,QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// ��ü�������ʱ��Ƶ��
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// ��ó�ʼֵ
#endif
	if (SAVE_IMAGE_2DISK==FALSE){
	    printf("SaveImgWithContours: --not-- \n");
	}else{
	unsigned int * imgbuf_t=new unsigned int[width*height];
	memcpy(imgbuf_t,ubuff,sizeof(unsigned int)*width*height);
	
	 DrawContoursAroundSegments(imgbuf_t,labels,width,height,BlackColorPNG); 
#if CUI_DRAW_SC_INDEX
	 DrawTextOnImage( imgbuf_t,labels,width,height);
#endif
	
	if (fileadd==""){
		fileadd=cuiGetCurrentTime();
	}
	CuiSaveImageData(imgbuf_t,width,height,filereadfullpath,filewritepath, 1,fileadd);
	delete []imgbuf_t;
	}
#if _MSC_VER&&_DEBUG
    QueryPerformanceCounter(&litmp);
	QPart2 = litmp.QuadPart;//�����ֵֹ
	dfMinus = (double)(QPart2-QPart1);
	dfTim = dfMinus / dfFreq;// ��ö�Ӧ��ʱ��ֵ����λΪ��
	/*************************************************************/
	TRACE("\n ����ͼƬʱ��: %f���룩",dfTim);
#endif
	
}
/*----------------------------------------------------------------*/
/**
*�������ر�ţ����浽Ӳ��
*
*@param [in] labels �����ر궨����
*@param		width ������
*@param		height ����߶�
*@param		filename Ҫ������ļ���
*@param		path   �ļ�·��
*
*/
/*----------------------------------------------------------------*/
void cui_GeneralImgProcess::SaveSuperpixelLabels(
	INT32*					labels,
	const int&					width,
	const int&					height,
	const string&				filename,
	const string&				path) 
{ 
#if CUI_SAVE_LABLES 
#ifdef WINDOWS
	char fname[256];
	char extn[256];
	_splitpath(filename.c_str(), NULL, NULL, fname, extn);
	string temp = fname;
	string finalpath = path + temp + string(".dat");
#else
	string nameandextension = filename+cuiGetCurrentTime();
	size_t pos = filename.find_last_of("/");
	if(pos != string::npos)//if a slash is found, then take the filename with extension
	{
		nameandextension = filename.substr(pos+1);
	}
	string newname = nameandextension.replace(nameandextension.rfind(".")+1, 3, "dat");//find the position of the dot and replace the 3 characters following it.
	string finalpath = path+newname;
#endif

	int sz = width*height;
	ofstream outfile;
	outfile.open(finalpath.c_str(),ios::out);


	/* int **cui_label;*/
	INT cui_label_value;
	char data_t[10];
	/*cui_label=&labels;*/

	for( int i = 0; i <height; i++ ){
		for( int j = 0; j <width; j++ ){
			cui_label_value=labels[i*width+j];
			sprintf(data_t,"%3d ",cui_label_value);

			outfile<<data_t;
		}
		outfile<<endl;
	}




	outfile.close(); 
#endif
}
/*----------------------------------------------------------------*/
/**
*�������ر�Ż��Ƶ�ͼƬ�ϣ������浽Ӳ��\n
* ��������δ�޸�
*
*@param [in] 	ubuff   ͼ�����
*@param [in]	labels �����ر궨���� 
*@param 	width  ͼƬ���
*@param 	height ͼƬ�߶�
*
*/
/*----------------------------------------------------------------*/
void cui_GeneralImgProcess::DrawTextOnImage(
	unsigned int*			ubuff,
	int*					labels,
	const int&				width,
	const int&				height)
{
	IplImage *imgdata_t;
	imgdata_t=cvCreateImageHeader(cvSize(width,height),8,4);
	imgdata_t->imageData=(char*)ubuff;
	/**********************************/
	INT32 sp_index[4096];
	char  text_buff_t[1024];
	memset(sp_index,0,sizeof(sp_index));
	const int xs_t[4]={10,10,-10,-10};
	const int ys_t[4]={10,-10,10,-10};
	for (register int i=0;i<height;i++){
		for (register int j=0;j<width;j++){
			   if (sp_index[labels[i*width+j]]==0){
				 
				   for (register int ni=0;ni<4;ni++){
					   int shift_y=i+ys_t[ni];
					   int shift_x=j+xs_t[ni];
					   if ((shift_x >= 0 && shift_x <width) && (shift_y >= 0 && shift_y < height)){  
								   if (labels[i*width+j]==labels[shift_y*width+shift_x]){

								   CvFont font;
								   cvInitFont(&font,CV_FONT_VECTOR0,0.4,0.4,0,1,8);
								   //��ͼ������ʾ�ı��ַ���
								   sprintf(text_buff_t,"%d",labels[i*width+j]);
								   cvPutText(imgdata_t,text_buff_t,cvPoint(shift_x+3,shift_y+5),&font,cvScalar(255,255,0,255));
				   					cvCircle(imgdata_t,cvPoint(shift_x,shift_y),1, cvScalar(0,0,255,255),2);
								   sp_index[labels[i*width+j]]=1;
								   break;
								}
					   }
					   

				   }
				 
			   }
				
		}
	}
	/**********************************/
	cvReleaseImageHeader(&imgdata_t);
}
/*----------------------------------------------------------------*/
/**
*���ݳ����صı궨���󣬼��㳬���ؿ�֮������ڹ�ϵ
*
*@param	 [in]	labels  �����ر궨����
*@param	   CuiNumLabels �����ظ��� 
*@param	 [out]   Cui_Matrix_E ���ƾ���
*@param          width  ͼƬ���
*@param          height ͼƬ�߶�
*@param	         filename Ҫ������ļ���
*@param	  path   �ļ�·��
*
*/
/*----------------------------------------------------------------*/
int cui_GeneralImgProcess::CuiSetNighbour_E_matrix(
	int*					labels,
	const  int CuiNumLabels,
	UINT32 *Cui_Matrix_E,
	const int				width,
	const int				height,	
	const string				filename,
	const string				path)
{
	TRACE_FUNC();

	int num_devices=IsCudaExist();
	if (num_devices<=0){
#if 1
	{
	const int dx8[8] = {-1, -1,  0,  1, 1, 1, 0, -1};
	const int dy8[8] = { 0, -1, -1, -1, 0, 1, 1,  1};
	int mainindex(0);int cind(0);
	 int DoCount=0;
	if (Cui_Matrix_E){
	   memset(Cui_Matrix_E,0,CuiNumLabels*CuiNumLabels*sizeof(UINT32));
	}else{
		return DoCount;
	}

	for (register int ii=0;ii<CuiNumLabels;ii++) {
		Cui_Matrix_E[ii*CuiNumLabels+ii]=0;
	}

	for(register  int j = 0; j <height; j++ ){
		for(register  int k = 0; k <width; k++ ){
			int np(0);
			for(register  int i = 0; i < 8; i++ ){
				int x = k + dx8[i];
				int y = j + dy8[i];

				if( (x >= 0 && x < width) && (y >= 0 && y <height) ){
					int index = y*width + x;
					//if( false == istaken[index] )//comment this to obtain internal contours
					{

						if( labels[mainindex] != labels[index] ) {
							UINT32  matrix_x=labels[index];
							UINT32  matrix_y=labels[mainindex];

							Cui_Matrix_E[matrix_x*CuiNumLabels+matrix_y]=++Cui_Matrix_E[matrix_y*CuiNumLabels+matrix_x];
							if (Cui_Matrix_E[matrix_x*CuiNumLabels+matrix_y]>999*2){
								Cui_Matrix_E[matrix_x*CuiNumLabels+matrix_y]=Cui_Matrix_E[matrix_y*CuiNumLabels+matrix_x]=999*2;
							}
						}
					}
				}
			}
			///////////////////////////////////////////////////
			////////////////////////////////////////////////////
			mainindex++;
		}
	}

#if 1
	for(register  int i = 0; i <CuiNumLabels; i++ ){
		for(register  int j = 0; j <CuiNumLabels; j++ ){			
			Cui_Matrix_E[i*CuiNumLabels+j]=Cui_Matrix_E[i*CuiNumLabels+j]>0?1:0;
		}

	}
#endif
	}
#endif
	}else{
		memset(Cui_Matrix_E,0,CuiNumLabels*CuiNumLabels*sizeof(UINT32));
		Get_Nighbour_E_matrix_cuda(labels,CuiNumLabels,Cui_Matrix_E,width,height,"","");
	}

#if _DEBUG
#ifdef WINDOWS
	char fname[256];
	char extn[256];
	_splitpath(filename.c_str(), NULL, NULL, fname, extn);
	string temp = fname;
	string finalpath = path + temp + string(".dat");
#else
	string nameandextension = filename;
	size_t pos = filename.find_last_of("/");
	if(pos != string::npos)//if a slash is found, then take the filename with extension
	{
		nameandextension = filename.substr(pos+1);
	}
	string newname = nameandextension.replace(nameandextension.rfind(".")+1, 3, "dat");//find the position of the dot and replace the 3 characters following it.
	string finalpath = path+newname;
#endif
	{
		char data_t[1024];
		ofstream outfile;
		outfile.open(finalpath.c_str(),ios::out);
		for(register  int i = 0; i <CuiNumLabels; i++ ){
			for(register  int j = 0; j <CuiNumLabels; j++ ){
				UINT32 cui_label_value=Cui_Matrix_E[i*CuiNumLabels+j];
				sprintf(data_t,"%4d",cui_label_value);
				outfile<<data_t;
			}
			outfile<<endl;
		} 
		outfile.close();
	}
#endif	
	return 0;
}
/*----------------------------------------------------------------*/
/**
*��D����
*
*@param [out]  Cui_Matrix_D	  D(��)����
*@param [in]   Cui_Matrix_EW  ���ƾ���
*@param  CuiNumLabels �����ظ���
*
*/
/*----------------------------------------------------------------*/
void cui_GeneralImgProcess::CuiSetNighbour_D_matrix(
	double *Cui_Matrix_D,	
	UINT32 *Cui_Matrix_EW,
	const  int& CuiNumLabels)
{
	if (Cui_Matrix_D){
		memset(Cui_Matrix_D,0,sizeof(double)*CuiNumLabels*CuiNumLabels);
	}
	
	
	/**************************************/ 		
	for (register int wi=0;wi<CuiNumLabels;wi++){
		double sum=0;
		for (register int wj=0;wj<CuiNumLabels;wj++){ 
			sum+=Cui_Matrix_EW[wi*CuiNumLabels+wj];
		}
		Cui_Matrix_D[wi*CuiNumLabels+wi]=sum;
	}	

	/***************************************/
#if _DEBUG
	if(cui_GeneralImgProcess::SAVE_DEBUG_2DISK){
		char data_t[1024];
		ofstream outfile;
		outfile.open("Matrix_D.data",ios::out);
		for(register int i = 0; i <CuiNumLabels; i++ ){
			for(register int j = 0; j <CuiNumLabels; j++ ){
				double value_t=Cui_Matrix_D[i*CuiNumLabels+j];
				sprintf(data_t," %0.2e ",value_t);
				outfile<<data_t;
			}
			outfile<<endl;
		} 
		outfile.close();
	}
#endif

}
/*----------------------------------------------------------------*/
/**
*����ͼ������ӹ�ϵ������ȫ��Χ�ĳ����ؿ���о���
*
@param	[in out] labels   �����ر궨����
@param  [in out] NumLabels �����ظ��� 
*@param          width  ������
*@param          height ����߶�
@param	[out] category  �м����������Ϊ�����ظ���
*
*/
/*----------------------------------------------------------------*/
void cui_GeneralImgProcess::LinkSurroundClassification(
	int*					labels,
	const  int& NumLabels,
	const int&				width,
	const int&				height,	
	INT32*			   category){
	double* Matrix_D;
	UINT32 *Matrix_E;
	Matrix_E=new UINT32[NumLabels*NumLabels];
	Matrix_D=new double[NumLabels*NumLabels];
	for (register int i=0;i<NumLabels;i++){
		category[i]=i;
	}
	CuiSetNighbour_E_matrix(labels,NumLabels,Matrix_E,width,height,"Matrix_Surround_E.data","");
	CuiSetNighbour_D_matrix(Matrix_D,Matrix_E,NumLabels);
	for(register int spi=0;spi<NumLabels;spi++){
		if (Matrix_D[spi*NumLabels+spi]==1){
			  for (register int spj=0;spj<NumLabels;spj++){
				    if (Matrix_E[spi*NumLabels+spj]==1){
						 //spi spj ��Χ��ϵ
						 //�ϲ���һ��
						category[spi]=category[spj]=(category[spi]>category[spj])?category[spj]:category[spi];
						break;
				    }
			  }
		}

	}
	delete[] Matrix_D;
	delete[] Matrix_E;
	/*******************************************/
	if(cui_GeneralImgProcess::SAVE_DEBUG_2DISK){
		char data_t[1024];
		ofstream outfile;
		outfile.open("CategoryMatrix_Surround.data",ios::out);
		for(register int i = 0; i <NumLabels; i++ ){
			
				double value_t= category[i];
				sprintf(data_t," %d ",(int)value_t);
				outfile<<data_t;

			outfile<<endl;
		} 
		outfile.close();
	}
}
/*----------------------------------------------------------------*/
/**
*��ȡ�����ؿ�֮����������
*
*@param   [out]  Matrix_W_Vein �������ƶȾ���
*@param    int   NumLabels    �����صĸ���
*@param   [in] p_SpProperty   ÿ�������ص�ͼ��λ����Ϣ
*@param   [in] pMD         �������м����
*
*/
/*----------------------------------------------------------------*/
void cui_GeneralImgProcess::CuiSetNighbour_W_Vein_matrix(
	double *Matrix_W_Vein,
	const  int NumLabels,
	SP_PROPERTY *p_SpProperty,
	ImageMemData *pMD)
{
	//����û�п��ǣ����ڹ�ϵ--���ʱʹ����
	for (register int spi=0;spi<NumLabels;spi++){
		for (register int spj=spi;spj<NumLabels;spj++){
	           Matrix_W_Vein[spi*NumLabels+spj]=Matrix_W_Vein[spj*NumLabels+spi]=CalculateSpSimilar(spi,spj,p_SpProperty);
		}
	}
#if 0
//��ƽ�����²��ֵľ��࣬
	for (int spi=0;spi<NumLabels;spi++){
		if (SpPos_UnderHorizontalLine==p_SpProperty[spi].IS_UnderHorizontalLine){
			for (register int spj=spi;spj<NumLabels;spj++){
				if ((SpPos_UnderHorizontalLine==p_SpProperty[spj].IS_UnderHorizontalLine)&&(spi!=spj)){
					Matrix_W_Vein[spi*NumLabels+spj]=Matrix_W_Vein[spj*NumLabels+spi]=1;
				}
				
			}
		}
	}
#endif
	

}
/*----------------------------------------------------------------*/
/**
*�����������ƶȾ������ɫ���ƶȾ������һ���ۺϵ����ƾ���
*
*@param [in] Matrix_W_Color  ��ɫ���ƾ���
*@param [in] Matrix_W_Vein   �������ƾ���
*@param [out]Matrix_W_Multi  �ۺϾ���
*@param spcount             �����ظ���
*@param Vein_threshold_SkyV ��������ա��������Ƶ���ֵ
*@param Vein_threshold_GND  �����е������Ƶ���ֵ
*@param  Color_threshold_SkyV ��ɫ����ա��������Ƶ���ֵ
*@param Color_threshold_GND   ��ɫ�е������Ƶ���ֵ
*@param *p_SpProperty        �������صķ���
*
*/
/*----------------------------------------------------------------*/
void cui_GeneralImgProcess::AdjustNighbour_W(
	double *Matrix_W_Color,
	double *Matrix_W_Vein,
	double *Matrix_W_Multi,
	int spcount,
	float Vein_threshold_SkyV,
	float Vein_threshold_GND,
	float Color_threshold_SkyV,
	float Color_threshold_GND,
	SP_PROPERTY *p_SpProperty)
{
#if 0
	for (int i=0;i<spcount;i++){
		for (int j=0;j<spcount;j++){
			if (Matrix_W_Vein[i*spcount+j]<Vein_threshold){
				//ȥ����ɫ���ƶ�
				Matrix_W_Multi[i*spcount+j]=0;
			}else{
				//������ɫ����
				Matrix_W_Multi[i*spcount+j]=Matrix_W_Color[i*spcount+j];
			}
		}
	}

	for (int i=0;i<spcount;i++){
		for (int j=0;j<spcount;j++){			
			if (i!=j){
				//�����ǲ��������
				Matrix_W_Multi[i*spcount+j]=Matrix_W_Color[i*spcount+j]*0.5+Matrix_W_Vein[i*spcount+j]*0.5;
			}
				
		}
	}
#endif
	for (int i=0;i<spcount;i++){
		for (int j=0;j<spcount;j++){
#if 1
			//��������
			if ((i!=j)&&
				(SpPos_UpHorizontalLine==p_SpProperty[i].IS_UnderHorizontalLine)&&
				(SpPos_UpHorizontalLine==p_SpProperty[j].IS_UnderHorizontalLine)){

								if ((Matrix_W_Color[i*spcount+j]>=Color_threshold_SkyV)&&
								(Matrix_W_Vein[i*spcount+j]>= Vein_threshold_SkyV)){
									//��տ����
									 Matrix_W_Multi[i*spcount+j]=Matrix_W_Multi[j*spcount+i]=1;
								}else{
									 Matrix_W_Multi[i*spcount+j]=Matrix_W_Multi[j*spcount+i]=0;
								}


				
			}else if ((i!=j)&&
				(SpPos_UnderHorizontalLine==p_SpProperty[i].IS_UnderHorizontalLine)&&
				(SpPos_UnderHorizontalLine==p_SpProperty[j].IS_UnderHorizontalLine)){

							if ((Matrix_W_Color[i*spcount+j]>=Color_threshold_GND)&&
								(Matrix_W_Vein[i*spcount+j]>=Vein_threshold_GND))
								{
									//��������
									Matrix_W_Multi[i*spcount+j]=Matrix_W_Multi[j*spcount+i]=1;
								} 
								else
								{
									//��������
									Matrix_W_Multi[i*spcount+j]=Matrix_W_Multi[j*spcount+i]=0;
								}



					
			}else{
				  //
				Matrix_W_Multi[i*spcount+j]=Matrix_W_Multi[j*spcount+i];
			}


#endif
		}
	}


}
/*----------------------------------------------------------------*/
/**
*���ͼ����ɫ������������������������һ�����������������Ĵ���
*
*@param contour ����һ������������ͼƬ����
*@param fill_img  Ҫ����������ͼ��
*@param filldata �����ɫ
*
*/
/*----------------------------------------------------------------*/
#define  TestGetContour2Fill 0
void cui_GeneralImgProcess::GetContour2Fill(
	const IplImage* contour,
	IplImage* fill_img,
	const CvScalar filldata)
{

		int i=0;
		int mode=CV_RETR_CCOMP;        //��ȡ������ģʽ
		int contoursNum=0;            //��ȡ��������Ŀ
		CvScalar externalColor;
		CvScalar holeColor;
		CvMemStorage * storage=cvCreateMemStorage(0);    //��ȡ������Ҫ�Ĵ������� 0ΪĬ��64KB
		CvSeq * pcontour=0;  //��ȡ����������ָ��	
		IplImage * src=cvCloneImage(contour);
		IplImage * pImg=cvCreateImage(cvGetSize(src),src->depth,1);	
		if (src->nChannels==4){
			cvCvtColor(src,pImg,CV_RGBA2GRAY);    //��ͼ��ת��Ϊ�Ҷ�
		}else if (src->nChannels==3){
			cvCvtColor(src,pImg,CV_RGB2GRAY);    //��ͼ��ת��Ϊ�Ҷ�
		}else{
			ASSERT(0);
		}
		//--------------��������----------------
		mode=CV_RETR_LIST;
		contoursNum=cvFindContours(pImg,storage,&  pcontour,sizeof(CvContour),mode,CV_CHAIN_APPROX_NONE);
		cout<<contoursNum<<" "<<endl;
		//--------------������----------------
#if TestGetContour2Fill
			IplImage* ImgTest=cvCreateImage(cvGetSize(fill_img),fill_img->depth,fill_img->nChannels);
			cvSaveImage("svg.jpg",pImg);
			cvSaveImage("ImgTest.jpg",ImgTest);
			cvSaveImage("lab.jpg",src);
#endif

		if (contoursNum>1){
			for (;pcontour!=0;pcontour=pcontour->h_next)
			{
				holeColor=filldata;
				externalColor=filldata;
				cvDrawContours(fill_img,pcontour,externalColor,holeColor,1,CV_FILLED,8);
#if TestGetContour2Fill
				cvSaveImage("svg.png",src);
				cvDrawContours(ImgTest,pcontour,WhiteColorPNGCv,WhiteColorPNGCv,1,CV_FILLED,8);
				cvSaveImage("ImgTest.jpg",ImgTest);
#endif
			}
		}
#if TestGetContour2Fill

		cvSaveImage("svg.png",pImg);
		cvReleaseImage(&ImgTest);
#endif		
		cvReleaseImage(&src);
		cvReleaseImage(&pImg);
		cvReleaseMemStorage(&storage);

}
/*----------------------------------------------------------------*/
/**
*���ͼ����ɫ������������������������һ�����������������Ĵ���
*
*@param contour ����һ������������ͼƬ����
*@param fill_img  Ҫ����������ͼ��
*@param filldata �����ɫ
*
*/
/*----------------------------------------------------------------*/
void cui_GeneralImgProcess::GetContour2Fill(
	const IplImage* contour,
	int OriginX,
	int OriginY,
	IplImage* fill_img,
	const CvScalar filldata)
{
#if TRUE
	int i=0;
	int mode=CV_RETR_CCOMP;        //��ȡ������ģʽ
	int contoursNum=0;            //��ȡ��������Ŀ
	CvScalar externalColor;
	CvScalar holeColor;
	CvMemStorage * storage=cvCreateMemStorage(0);    //��ȡ������Ҫ�Ĵ������� 0ΪĬ��64KB
	CvSeq * pcontour=0;  //��ȡ����������ָ��	
	//IplImage * src=cvCloneImage(contour);
	IplImage * pImg=cvCreateImage(cvGetSize(contour),contour->depth,1);	
	cvCvtColor(contour,pImg,CV_RGB2GRAY);    //��ͼ��ת��Ϊ�Ҷ�
	//--------------��������----------------
	mode=CV_RETR_LIST;
	contoursNum=cvFindContours(pImg,storage,&  pcontour,sizeof(CvContour),mode,CV_CHAIN_APPROX_NONE);
	//cout<<contoursNum<<" "<<endl;
	//--------------������----------------
	if (contoursNum>1){
		printf("%d��������START",contoursNum);
		for (;pcontour!=0;pcontour=pcontour->h_next)
		{
			holeColor=filldata;
			externalColor=filldata;
			cvDrawContours(fill_img,pcontour,externalColor,holeColor,1,CV_FILLED,8,cvPoint(OriginX,OriginY));
          
			if (cui_GeneralImgProcess::SAVE_DEBUG_2DISK)		
				printf("#");
          

		}
		if (cui_GeneralImgProcess::SAVE_DEBUG_2DISK)
			printf("\n");
	}
//	cvReleaseImage(&src);
	cvReleaseImage(&pImg);
	cvReleaseMemStorage(&storage);
#endif
}
/*----------------------------------------------------------------*/
/**
*���ͼ����ɫ������������������Ƶ���һ��ͼ����
*
*@param contour ����һ������������ͼƬ����(ͼ�鼶��)
*@param fill_img  ����������Ŀ��ͼ��
*@param filldata �����ɫ
*@param thickness �����ߴ�
*@param [out] C  �ܳ�
*@param [out] S  ���
*/
/*----------------------------------------------------------------*/
double cui_GeneralImgProcess::GetContour2Draw(
	const   IplImage* contour, 
	IplImage* fill_img,
	const CvScalar filldata,
	int thickness,
	float*		C,
	float*		S)
{
	    double P_cs=0;
		int i=0;
		int mode=CV_RETR_CCOMP;        //��ȡ������ģʽ
		int contoursNum=0;            //��ȡ��������Ŀ
		CvScalar externalColor;
		CvScalar holeColor;
		CvMemStorage * storage=cvCreateMemStorage(0);    //��ȡ������Ҫ�Ĵ������� 0ΪĬ��64KB
		CvSeq * pcontour=0;  //��ȡ����������ָ��	
		IplImage * src=cvCloneImage(contour);
		IplImage * pImg=cvCreateImage(cvGetSize(src),src->depth,1);
	
		cvCvtColor(src,pImg,CV_RGB2GRAY);    //��ͼ��ת��Ϊ�Ҷ�
#if 0		
		cvNamedWindow("src",CV_WINDOW_AUTOSIZE);
		cvNamedWindow("pcontour",CV_WINDOW_AUTOSIZE);
		cvShowImage("src",src);
#endif
		
	//	cvThreshold(pImg,pImg,180,255,CV_THRESH_BINARY);    //��ֵ��
		//--------------��������----------------
		mode=CV_RETR_LIST;
		contoursNum=cvFindContours(pImg,storage,&pcontour,sizeof(CvContour),mode,CV_CHAIN_APPROX_NONE);
		double length =0; 
		double area=0;		
		cout<<contoursNum<<" "<<endl;
		//--------------������----------------
		if (contoursNum>=1){
			for (;pcontour!=0;pcontour=pcontour->h_next)
			{
				holeColor=cvScalar(0,0,0,0);
				externalColor=filldata;
				cvDrawContours(fill_img,pcontour,externalColor,holeColor,1,thickness);
				if (CV_IS_SEQ_CLOSED(pcontour)){
					 length+=cvArcLength(pcontour); 
					 area+=cvContourArea(pcontour);
				}else{
					ASSERT(0);
				}
			}
		}
		
#if 0
		cvShowImage("pcontour",fill_img);
#endif
		ASSERT(length);
		ASSERT(area);
		P_cs=length/area;
		if (C){
			*C=length;
		}
		if (S){
			*S=area;
		}
		cvReleaseImage(&src);
		cvReleaseImage(&pImg);
		cvReleaseMemStorage(&storage);
		return P_cs;

}
/*----------------------------------------------------------------*/
/**
*����ȫ��Χ�ĳ����ؿ�����������������
*
*
*@param   ubuff		ԭʼͼ�����
*@param   labels	�����ر궨����	
*@param   NumLabels	ͼƬ����
*@param   width     ͼƬ���
*@param   height    ͼƬ�߶�
*@param   filereadfullpath �ļ���ȡ·��
*@param   filewritepath    �ļ�д��·��
*/
/*----------------------------------------------------------------*/

void cui_GeneralImgProcess::InSideClusteringByopencv(
	UINT32* ubuff,
	int*					labels,
	int& NumLabels,
	const int&				width,
	const int&				height,
	string        filereadfullpath,
	string        filewritepath)
{
#if _MSC_VER
    LARGE_INTEGER litmp;
	LONGLONG QPart1,QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// ��ü�������ʱ��Ƶ��
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// ��ó�ʼֵ
#endif
	//////////////////////////////////////////////////////////////
#if 1
	IplImage Img_Contour_t;
	int* Contour_t=new int[width*height];
	cvInitImageHeader(&Img_Contour_t,cvSize(width,height),IPL_DEPTH_8U,4);
	Img_Contour_t.imageData=(char *)Contour_t;
	IplImage Img_labels;
	cvInitImageHeader(&Img_labels,cvSize(width,height),IPL_DEPTH_8U,4);
	Img_labels.imageData=(char *)labels;	
	///////////////////////////////////////////	
	ASSERT(sizeof(int)==4);
	//////////////////////////////////////////
	for (register int sp=0;sp<NumLabels;sp++){
		
		for (register int y=0;y<height;y++){
			for (register int x=0;x<width;x++){
				if (labels[width*y+x]==sp){
					Contour_t[width*y+x]=0xffffffff;				
				}else{
					Contour_t[width*y+x]=0x0;
				}
			}
		}
		//�������ͼ���������
		GetContour2Fill(&Img_Contour_t,&Img_labels,cvScalar(sp&0xff,(sp>>8)&0xff,(sp>>16)&0xff,(sp>>24)&0xff));
	//	cvWaitKey(0);
	}

	
	/******** ��׼��lable*******************************************************/
	memcpy_s(Contour_t,sizeof(int)*width*height,labels,sizeof(int)*width*height);
	NumLabels=AdjustLabFrom0toN(Contour_t,width,height,NumLabels);
	/***************************************************************/

	memcpy_s(labels,sizeof(int)*width*height,Contour_t,sizeof(int)*width*height);
	delete []Contour_t;
	cui_GeneralImgProcess::SaveSuperpixelLabels(labels,width,height);
	/**************************************************************/
#endif
	//////////////////////////////////////////////////////////////
#if _MSC_VER
    QueryPerformanceCounter(&litmp);
	QPart2 = litmp.QuadPart;//�����ֵֹ
	dfMinus = (double)(QPart2-QPart1);
	dfTim = dfMinus / dfFreq;// ��ö�Ӧ��ʱ��ֵ����λΪ��
	TRACE("\n ȫ��Χ: %f���룩",dfTim);
#endif
	
}
/*----------------------------------------------------------------*/
/**
*���double�����е����ֵ
*
*@param Data double ���� 
*@param DataNum ���鳤��
*@return �����е����ֵ
*/
/*----------------------------------------------------------------*/
 double cui_GeneralImgProcess::GetMaxValue(double* Data, long DataNum)
{
	double *Data_cp=new double[DataNum];
	double  max_value;
	memcpy(Data_cp,Data,sizeof(double)*DataNum);
	std::sort(Data_cp,Data_cp+DataNum,greater<double>());
	max_value=Data_cp[0];
	delete []Data_cp;
	return  max_value;
}
 /*----------------------------------------------------------------*/
 /**
 *���float�����е����ֵ
 *
 *@param Data float ���� 
 *@param DataNum ���鳤��
 *@return �����е����ֵ
 */
 /*----------------------------------------------------------------*/
float cui_GeneralImgProcess::GetMaxValue(float* Data, long DataNum)
 {
	 float *Data_cp=new float[DataNum];
	 float  max_value;
	 memcpy(Data_cp,Data,sizeof( float)*DataNum);
	 std::sort(Data_cp,Data_cp+DataNum,greater<float>());
	 max_value=Data_cp[0];
	 delete []Data_cp;
	 return  max_value;
 }
/*----------------------------------------------------------------*/
 /**
 *���float�����е����ֵ
 *
 *@param Data float ���� 
 *@param DataNum ���鳤��
 *@return �����е����ֵ
 */
 /*----------------------------------------------------------------*/
int cui_GeneralImgProcess::GetMaxValue(int* Data, long DataNum)
 {
	 int *Data_cp=new int[DataNum];
	 int  max_value;
	 memcpy(Data_cp,Data,sizeof(int)*DataNum);
	 std::sort(Data_cp,Data_cp+DataNum,greater<int>());
	 max_value=Data_cp[0];
	 delete []Data_cp;
	 return  max_value;
 }
 /*----------------------------------------------------------------*/
 /**
 *���float�����е����ֵ
 *
 *@param Data float ���� 
 *@param DataNum ���鳤��
 *@return �����е����ֵ
 */
 /*----------------------------------------------------------------*/
float cui_GeneralImgProcess::GetMinValue(float* Data,long DataNum)
 {
	 float *Data_cp=new float[DataNum];
	 float  min_value;
	 memcpy(Data_cp,Data,sizeof( float)*DataNum);
	 std::sort(Data_cp,Data_cp+DataNum,greater<float>());
	 min_value=Data_cp[DataNum-1];
	 delete []Data_cp;
	 return  min_value;
 }
 /*----------------------------------------------------------------*/
 /**
 *���float�����е����ֵ
 *
 *@param Data float ���� 
 *@param DataNum ���鳤��
 *@return �����е����ֵ
 */
 /*----------------------------------------------------------------*/
float cui_GeneralImgProcess::GetMinValue(double* Data,long DataNum)
 {
	 double *Data_cp=new double [DataNum];
	 double min_value;
	 memcpy(Data_cp,Data,sizeof(double)*DataNum);
	 std::sort(Data_cp,Data_cp+DataNum,greater<double>());
	 min_value=Data_cp[DataNum-1];
	 delete []Data_cp;
	 return  min_value;
 }
/*----------------------------------------------------------------*/
 /**
 *���float�����е����ֵ
 *
 *@param Data float ���� 
 *@param DataNum ���鳤��
 *@return �����е����ֵ
 */
 /*----------------------------------------------------------------*/
int cui_GeneralImgProcess::GetMinValue(int* Data, long DataNum)
 {
	 int *Data_cp=new int[DataNum];
	 int  max_value;
	 memcpy(Data_cp,Data,sizeof(int)*DataNum);
	 std::sort(Data_cp,Data_cp+DataNum,std::greater<int>());
	 max_value=Data_cp[DataNum-1];
	 delete []Data_cp;
	 return  max_value;
 }
 /*----------------------------------------------------------------*/
 /**
 *����㵽��ľ���
 *
 *@param point1 ��1
 *@param point2 ��2
 *@return ����֮��ľ���
 */
 /*----------------------------------------------------------------*/
 double cui_GeneralImgProcess::P2PointDiatance(CvPoint point1, CvPoint point2)
 {
	 double dst_t;
	 dst_t=sqrtl(powl(point1.x-point2.x,2)+powl(point1.y-point2.y,2));
	 return dst_t;
 }
/*----------------------------------------------------------------*/
/**
*���double�����о�ֵ
*
*@param Data double ���� 
*@param DataNum ���鳤��
*@return �����о�ֵ
*/
/*----------------------------------------------------------------*/
 double cui_GeneralImgProcess::GetAverageValue(double* Data, int DataNum)
 {
	 double sum=0;
	 ASSERT(DataNum>0);
	 for (int i=0;i<DataNum;i++){
		 sum+=Data[i];
	 }
	 sum/=DataNum;
	 return sum;
 }
/*----------------------------------------------------------------*/
/**
*���double�����о�ֵ
*
*@param Data double ���� 
*@param DataNum ���鳤��
*@return �����о�ֵ
*/
/*----------------------------------------------------------------*/
double cui_GeneralImgProcess::GetAverageValue(float* Data, int DataNum)
{
	 double sum=0;
	 ASSERT(DataNum>0);
	 for (int i=0;i<DataNum;i++){
		 sum+=Data[i];
	 }
	 sum/=DataNum;
	 return sum;
 }
 /*----------------------------------------------------------------*/
 /**
 *��double�������
 *
 *@param Data double ���� 
 *@param DataNum ���鳤��
 *@return ������͵�ֵ
 */
 /*----------------------------------------------------------------*/
 double cui_GeneralImgProcess::GetSumValue(double* Data, int DataNum)
 {
	 double sum=0;
	 for (int i=0;i<DataNum;i++){
		 sum+=Data[i];
	 }
	 return sum;
 }
 /*----------------------------------------------------------------*/
 /**
 *��float�������
 *
 *@param Data float ���� 
 *@param DataNum ���鳤��
 *@return ������͵�ֵ
 */
 /*----------------------------------------------------------------*/
 double cui_GeneralImgProcess::GetSumValue(float* Data, int DataNum)
 {
	 double sum=0;
	 for (int i=0;i<DataNum;i++){
		 sum+=Data[i];
	 }
	 return sum;
 }
 /*----------------------------------------------------------------*/
 /**
 *��double�������䷽��
 *
 *@param [in] Data double����
 *@param DataNum ���鳤��
 *@param [out]avg ��ֵ
 *@param [out]variance ����
 *@return ����ķ���
 */
 /*----------------------------------------------------------------*/
double cui_GeneralImgProcess:: GetVarianceValue(
	double* Data, 
	int DataNum,
	double *avg,
	double *variance){

	  *avg=GetAverageValue(Data,DataNum);
	  *variance=0;
	  for (int i=0;i<DataNum;i++){
		 *variance+=pow(*avg-Data[i],2);
	  }
	  *variance/=DataNum;
	  *variance=sqrtl(*variance); 
	  return *variance;
}
 /*----------------------------------------------------------------*/
 /**
 *��float�������䷽��
 *
 *@param [in] Data double����
 *@param DataNum ���鳤��
 *@param [out]avg ��ֵ
 *@param [out]variance ����
 *@return ����ķ���
 */
 /*----------------------------------------------------------------*/
double cui_GeneralImgProcess:: GetVarianceValue(
	float* Data, 
	int DataNum,
	float *avg,
	float *variance){

	  *avg=GetAverageValue(Data,DataNum);
	  *variance=0;
	  for (int i=0;i<DataNum;i++){
		 *variance+=pow(*avg-Data[i],2);
	  }
	  *variance/=DataNum;
	  *variance=sqrtl(*variance); 
	  return *variance;
}
/*----------------------------------------------------------------*/
/**
*�������������ֵ������
*@param [in] data ����
*@param size ���鳤��
*@param [out] sort �������ֵ������
*@param sort_num sort����Ĵ�С
*
*/
/*----------------------------------------------------------------*/

void cui_GeneralImgProcess::GetMaxValueIndex(
	float* data,
	float size,
	int* sort, 
	int sort_num)
{
	
#if 1
     float* data_t=new float[(LONGLONG)size];
	 memcpy(data_t,data,sizeof(float)*((LONGLONG)size));
	/*****Ѱ����ֵ***************************************************************/
	for (int sj=0;sj<sort_num;sj++){
		
		float max_value=data[0];
		int max_value_i=0;
		/*******************************/
		for (register int i=0;i<size;i++){
			if (data_t[i]>max_value){
				max_value=data_t[i];
				max_value_i=i;
			}
		}
		/*******************************/
		sort[sj]=max_value_i;
		 data_t[max_value_i]=0;

	}
	/*******************************************************************************/
	delete []data_t;
#else
	float data_t[AngLeDivided];
	memcpy(data_t,data,sizeof(float)*size);
	/*****Ѱ����ֵ***************************************************************/
	for (int sj=0;sj<sort_num;sj++){

		float max_value=data[0];
		int max_value_i=0;
		/*******************************/
		for (register int i=0;i<size;i++){

			if (data_t[i]>max_value){
				max_value=data_t[i];
				max_value_i=i;
			}
		}
		/*******************************/
		sort[sj]=max_value_i;
		data_t[max_value_i]=0;

	}
	/*******************************************************************************/
	delete []data_t;
#endif
	
}
/*----------------------------------------------------------------*/
/**
*����������ֵ������������
*
*@param [in] data ����
*@param size  ���鳤��
*@param [out] sort �������ֵ����
*@param sort_num ���������С
*
*/
/*----------------------------------------------------------------*/
void cui_GeneralImgProcess::GetMaxValueIndexdouble(
	double* data, 
	double size, 
	int* sort,
	int sort_num)
{
	double* data_t=new double[(LONGLONG)size];
	memcpy(data_t,data,sizeof(double)*((LONGLONG)size));
	/*****Ѱ����ֵ***************************************************************/
	for (int sj=0;sj<sort_num;sj++){

		float max_value=data[0];
		int max_value_i=0;
		/*******************************/
		for (register int i=0;i<size;i++){
			if (data_t[i]>max_value){
				max_value=data_t[i];
				max_value_i=i;
			}
		}
		/*******************************/
		sort[sj]=max_value_i;
		data_t[max_value_i]=0;

	}
	/*******************************************************************************/
	delete []data_t;
}
/*----------------------------------------------------------------*/
/**
*����������ֵ
*
*@param [in] data ����
*@param size �����С
*@return ��λֵ��С
*/
/*----------------------------------------------------------------*/

double cui_GeneralImgProcess::GetMiddleValue(double* data, int size)
{
	double *Data_cp=new double [size];
	double  m_value;
	memcpy(Data_cp,data,sizeof(double)*size);
	std::sort(Data_cp,Data_cp+size,greater<double>());
	m_value=Data_cp[size/2];
	delete []Data_cp;
	return  m_value;
}
/*----------------------------------------------------------------*/
/**
*��ֱ�߼��ϻ��Ƶ��հ׵�ͼƬ�ϣ������浽Ӳ��\n
*������
*
*@param linedata ֱ������
*@param width  ͼƬ���
*@param height ͼƬ�߶�
*@param filename �ļ���
*/
/*----------------------------------------------------------------*/
 void cui_GeneralImgProcess::DrawLine2BlackImg(
	 vector<LINEDATA> linedata,
	 int width,
	 int height,
	 string filename){
	  
	  CvFont font;
	  char text_buff_t[1024];
	  IplImage *black_img=cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,4);
	  cvZero(black_img);
	  for(unsigned int i=0;i<linedata.size();i++){
		cvLine(black_img,linedata[i].AdjPoint1,linedata[i].AdjPoint2,CV_RGB(125,125,0));
	  }
	  ///////////////////////////////////////////////////////////////////////////////////
	  for (unsigned int i=0;i<linedata.size();i++){			
		  cvLine(black_img,linedata[i].pointOne,linedata[i].pointTwo,CV_RGB(0,255,255),2);
			
			cvInitFont(&font,CV_FONT_VECTOR0,0.4,0.4,0,1,8);
			//��ͼ������ʾ�ı��ַ���
			sprintf(text_buff_t,"%d---%0.0f---%0.0f",i,linedata[i].b,linedata[i].LineLength);
			cvPutText(black_img,text_buff_t,linedata[i].pointOne,&font,CV_RGB(255,0,0));
	  }
	  
	  filename+=".jpg";
	  cvSaveImage(filename.c_str(),black_img);
	  cvReleaseImage(&black_img);

}
/*-------------------------------------------------------------------------------------------*/
/**
*������\n
*-��ͼ��ͼ����
*@param linedata ֱ������
*@param linelen ֱ������
*@param img ͼ��
*@param filename �ļ���
*/
/*-------------------------------------------------------------------------------------------*/
 void cui_GeneralImgProcess::DrawLine2Img(LINEDATA* linedata, int linelen, IplImage* img,string filename)
 {
	 CvFont font;
	 IplImage* src_img=cvCloneImage(img);
	 char text_buff_t[1024];
#if 0
 for(unsigned int i=0;i<linelen;i++){
		 cvLine(src_img,linedata[i].AdjPoint1,linedata[i].AdjPoint2,CV_RGB(125,125,0));
	 }
#endif



	 ///////////////////////////////////////////////////////////////////////////////////
	 for (unsigned int i=0;i<linelen;i++){			;
		 	
		 CvScalar color=HoughAnalyze::GetGradientColor(linedata[i].alpha_angle); 
		 cvLine(src_img,linedata[i].pointOne,linedata[i].pointTwo,color,2);
#if 0
		 cvInitFont(&font,CV_FONT_VECTOR0,0.4,0.4,0,1,8);
		 //��ͼ������ʾ�ı��ַ���
		 sprintf(text_buff_t,"%d---%0.0f---%0.0f",i,linedata[i].b,linedata[i].LineLength);
		 cvPutText(black_img,text_buff_t,linedata[i].pointOne,&font,CV_RGB(255,0,0));
#endif
	 }

	 filename+=".jpg";
	 cvSaveImage(filename.c_str(),src_img);
	 cvReleaseImage(&src_img);
	
 }
/*-------------------------------------------------------------------------------------------*/
/**
*
*���㳬���ر�ŵ�����
*
*@param labels   labels ͼ�����
*@param width    labels�Ŀ��
*@param height   labels�ĸ߶�
*@param *p_SpProperty  ָ��labels����������Ե�ָ��
*@param SpNumbers labels�г����صĸ���
*
*/
/*-------------------------------------------------------------------------------------------*/
 void cui_GeneralImgProcess::CalculateAllSpPropertyRange(	
	 INT32*					labels,
	 const int&				width,
	 const int&				height,
	 SP_PROPERTY *p_SpProperty,
	 int   SpNumbers)
 {
	 for (int spi=0;spi<SpNumbers;spi++){
		 p_SpProperty[spi].max_x=0;
		 p_SpProperty[spi].min_x=0;
		 p_SpProperty[spi].max_y=0;
		 p_SpProperty[spi].min_y=0;
		 p_SpProperty[spi].IsInit_SquareMeter=0;
		 p_SpProperty[spi].SPCategory=0;
		 p_SpProperty[spi].PosBorder=0;
#ifdef _DEBUG
		 p_SpProperty[spi].SPIndex=spi;
#endif
	 }
	 /******************************************/
	 for (register int x=0;x<width;x++){
		 //Y_SegPosition=GetSegmentPoint(x);
		 for (register int y=height-1;y>=0;y--){
			 ///////////////////////////////////////
			 ASSERT((y*width+x)<(width*height));
			INT32 spi=labels[y*width+x];
#if 1		
			//���㷶Χ
			 ASSERT(spi>=0&&spi<SpNumbers);
				 if (p_SpProperty[spi].IsInit_SquareMeter==0){
					 p_SpProperty[spi].IsInit_SquareMeter=1;
					 p_SpProperty[spi].max_x=p_SpProperty[spi].min_x=x;
					 p_SpProperty[spi].max_y=p_SpProperty[spi].min_y=y;
					 p_SpProperty[spi].SPCategory=spi;
				 }else{
					 if (p_SpProperty[spi].min_y>=y){
						 p_SpProperty[spi].min_y=y;
					 }
					 if (p_SpProperty[spi].max_y<=y){
						p_SpProperty[spi].max_y=y;
					 }
					 if (p_SpProperty[spi].min_x>=x){
						 p_SpProperty[spi].min_x=x;
					 }
					 if (p_SpProperty[spi].max_x<=x){
						 p_SpProperty[spi].max_x=x;
					 }
					  p_SpProperty[spi].IsInit_SquareMeter++;
				 }
				 ///////////////////////////////////////
#endif
				
#if TRUE
				 //�������λ��
				 if (x==0){
						p_SpProperty[spi].PosBorder|=SpPosBorder_LEFT;
				 }
				 if (x==width-1){
						p_SpProperty[spi].PosBorder|=SpPosBorder_RIGHT;
				 }
				 if (y==0){
					p_SpProperty[spi].PosBorder|=SpPosBorder_TOP;
				 }
				 if (y==height-1){
					p_SpProperty[spi].PosBorder|=SpPosBorder_BOTTOM;
				 }
#endif
			
			
         }
	}
	 /*--------------------------------------------------------------*/
#if _DEBUG
	  for (int spi=0;spi<SpNumbers;spi++){
		 p_SpProperty[spi].SPGuid.GUIDX=p_SpProperty[spi].min_x;
		 p_SpProperty[spi].SPGuid.GUIDY=p_SpProperty[spi].min_y;
	  }
#endif
	  /*-------------------------------------------------------------*/
	 //����ʹ���ܱ�8 ����
	 for (int spi=0;spi<SpNumbers;spi++){
		 int width_t= p_SpProperty[spi].max_x-p_SpProperty[spi].min_x;
		 int height_t=p_SpProperty[spi].max_y-p_SpProperty[spi].min_y;
		 if (width_t%8!=0){
				int add_width=8-width_t%8;
				if (p_SpProperty[spi].min_x-add_width>=0){
					p_SpProperty[spi].min_x-=add_width;
				}else if (p_SpProperty[spi].max_x+add_width<width){
					p_SpProperty[spi].max_x+=add_width;
				}

		 }
		 if (height_t%8!=0){
				 int add_height=8-height_t%8;
				 if (p_SpProperty[spi].min_y-add_height>=0){
					 p_SpProperty[spi].min_y-=add_height;
				 }else if (p_SpProperty[spi].max_y+add_height<height){
					 p_SpProperty[spi].max_y+=add_height;
				 }
		 }
	 }
	// C2bmpsize.width=C2bmpsize.width-C2bmpsize.width%8+8;
	// C2bmpsize.height=C2bmpsize.height-C2bmpsize.height%8+8;	
	 ////////////////////////
	
 }
 /*----------------------------------------------------------------*/
 /**
 *�������г����ؿ�ķ�Χ���ԣ����Լ��ټ���
 *
 *@param   labels �����ر궨����
 *@param   width  �궨������
 *@param   height �궨����߶�
 *@param   p_SpProperty �����ؿ������
 *@param   SpNumbers   �����ؿ�ĸ���
 *@param   horizontal_line_pos ��ƽ�ߵ�λ��
 *
 */
 /*----------------------------------------------------------------*/
 void cui_GeneralImgProcess::CalculateAllSpPropertyPostitonByHLine(	
	int*					labels,
	 const int&				width,
	 const int&				height,
	 SP_PROPERTY *p_SpProperty,
	 int   SpNumbers,
	 float horizontal_line_pos)
 {
	 /******************************************/
	 //�������ˮƽ��λ��
	 for (int i=0;i<SpNumbers;i++){
		 p_SpProperty[i].IS_UnderHorizontalLine=0;
	 }
	 /******************************************/
	 for (register int x=0;x<width;x++){
		 //Y_SegPosition=GetSegmentPoint(x);
		 for (register int y=height-1;y>=0;y--){
			 ///////////////////////////////////////
			 int spi=labels[y*width+x];
			 //���㷶Χ
			 ASSERT(spi>=0&&spi<SpNumbers);
#if IN_DOOR
			 if (y>horizontal_line_pos){
				 p_SpProperty[spi].IS_UnderHorizontalLine|=SpPos_UnderHorizontalLine;
			 }else if (y==horizontal_line_pos){
				  p_SpProperty[spi].IS_UnderHorizontalLine|=SpPos_CrossHorizontalLine;
			 }else if (y<horizontal_line_pos){
				 p_SpProperty[spi].IS_UnderHorizontalLine|=SpPos_UpHorizontalLine;
			 }
#endif
#if OUT_DOOR
			if (y>=horizontal_line_pos){
				p_SpProperty[spi].IS_UnderHorizontalLine|=SpPos_UnderHorizontalLine;
			}else{
				p_SpProperty[spi].IS_UnderHorizontalLine|=SpPos_UpHorizontalLine;
			}
#endif
		 }
	 }

 }
/*----------------------------------------------------------------*/
/**
*����ͼ���С������
*
*@param []   imgBufferOrigin
*@param []   imgBufferBorder
*@param      Width
*@param      Height
*
*/
/*----------------------------------------------------------------*/
 Energy_Harr cui_GeneralImgProcess::CalculateImageHarrEnergy(
	 UINT32* imgBufferOrigin,
	 UINT32* imgBufferBorder,
	 int Width,
	 int Height)
{
	 double  proportion_t=0;
	 CuiHarrTransformLet Harr_Origin,Harr_Border;
	 Energy_Harr harr_origin_energy;
	 /**********************************************/
	 for(register int x=0;x<Width;x++){
		 for (register int y=0;y<Height;y++){
			 /////////////////////////////////
			 if (imgBufferBorder[y*Width+x]&&0x00ffffff==0x00ffffff){
				 proportion_t+=1;
			 }else{
				
			 }

		 }
	 }
	 /**********************************************/
	 proportion_t=proportion_t/(Width*Height);
	/**********************************************/
	Harr_Origin.CuiGetImageData(imgBufferOrigin,Width,Height,proportion_t);
	Harr_Border.CuiGetImageData(imgBufferBorder,Width,Height,proportion_t);

	Harr_Origin.CuiWaveletTrans(1);
	Harr_Border.CalculateBoderImg(1);//�������α����������
	Harr_Origin.CuiAdjustBoderByContour(Harr_Border.img_dst_wavelet);//ȥ��Ӧͼ���Ե��������α����
	Harr_Origin.CuiCalculateEnergyALL();
	//Harr_Border.CuiCalculateEnergyALL();
	Harr_Origin.CuiGetAllEnergy(&harr_origin_energy);
	//Harr_Border.CuiGetAllEnergy(&harr_boder_energy);	
#if CUI_SAVE_HARR_IMG&&_DEBUG
	Harr_Border.CuiSaveImg(); 
	Harr_Origin.CuiSaveImg();
#endif
	return harr_origin_energy;
}
 /*----------------------------------------------------------------*/
 /**
 *���㵥�������ؿ��С������
 *
 *
 *@param sp				�����ؿ�ı��
 *@param p_SpProperty    �����ؿ����������
 *@param  cui_ImgData    ͼ�����
 *@param cui_ImgLables   �����ر궨����
 *@param cui_Width       ͼ����
 *@param cui_Height      ͼ��߶�
 */
 /*----------------------------------------------------------------*/
 void cui_GeneralImgProcess::CalculateSpBlockEnergy2(
	 int sp,
	 SP_PROPERTY *p_SpProperty,
	 UINT32 * cui_ImgData,
	 int *cui_ImgLables,
	 int cui_Width,
	 int cui_Height)
{
#if _MSC_VER &&_DEBUG
	LARGE_INTEGER litmp;
	LONGLONG QPart1,QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;		
#endif
#if 1
	 CuiHarrTransformLet Harr_Origin,Harr_Border;
	 UINT32 *Origin_img=NULL,*Border_img=NULL;
	 double  proportion_t=0;
	 Energy_Harr harr_origin_energy;
	 int img_width=p_SpProperty[sp].max_x-p_SpProperty[sp].min_x;
	 int img_height=p_SpProperty[sp].max_y-p_SpProperty[sp].min_y;

		Origin_img=new UINT32[img_width*img_height];
		Border_img=new UINT32[img_width*img_height];
		ASSERT(img_width*img_height>0);
	/*********************************************/
	 for(register int x=p_SpProperty[sp].min_x;x<p_SpProperty[sp].max_x;x++){
		 for (register int y=p_SpProperty[sp].min_y;y<p_SpProperty[sp].max_y;y++){
			 /////////////////////////////////
			 ASSERT(x>=0&&x<cui_Width);
			 ASSERT(y>=0&&y<cui_Height);
			 /////////////////////////////////
			 int x_t=x-p_SpProperty[sp].min_x;
			 int y_t=y-p_SpProperty[sp].min_y;
			 /////////////////////////////////
			 if (cui_ImgLables[y*cui_Width+x]==sp){
				 Origin_img[y_t*img_width+x_t]=cui_ImgData[y*cui_Width+x];
				 Border_img[y_t*img_width+x_t]=0x00ffffff;
				 proportion_t+=1;
			 }else{
				 Origin_img[y_t*img_width+x_t]=0;
				 Border_img[y_t*img_width+x_t]=0;
			 }

		 }
	 }
	 /**********************************************/
	 proportion_t=proportion_t/(img_width*img_height);
	 /**********************************************/
	 Harr_Origin.CuiGetImageData(Origin_img,img_width,img_height,proportion_t);
	 Harr_Border.CuiGetImageData(Border_img,img_width,img_height,proportion_t);

 					    

	 Harr_Origin.CuiWaveletTrans(1);//0.4ms

	 



	 Harr_Border.CalculateBoderImg(1);//�������α����������==0.9ms




	 Harr_Origin.CuiAdjustBoderByContour(Harr_Border.img_dst_wavelet);//ȥ��Ӧͼ���Ե��������α���� 4.6ms




	 Harr_Origin.CuiCalculateEnergyALL();//2.9ms



	 Harr_Origin.CuiGetAllEnergy(&harr_origin_energy);//0.00087969494ms




#if CUI_SAVE_HARR_IMG &&_DEBUG
	 Harr_Border.CuiSaveImg(); 
	 Harr_Origin.CuiSaveImg();
#endif
	 p_SpProperty[sp].harr_energy.EnergyL_LL=harr_origin_energy.EnergyL_LL;
	 p_SpProperty[sp].harr_energy.EnergyL_LH=harr_origin_energy.EnergyL_LH;
	 p_SpProperty[sp].harr_energy.EnergyL_HL=harr_origin_energy.EnergyL_HL;
	 p_SpProperty[sp].harr_energy.EnergyL_HH=harr_origin_energy.EnergyL_HH;
	 p_SpProperty[sp].harr_energy.Energy_LL=harr_origin_energy.Energy_LL;
	 p_SpProperty[sp].harr_energy.Energy_LH=harr_origin_energy.Energy_LH;
	 p_SpProperty[sp].harr_energy.Energy_HL=harr_origin_energy.Energy_HL;
	 p_SpProperty[sp].harr_energy.Energy_HH=harr_origin_energy.Energy_HH;
	 p_SpProperty[sp].harr_energy.EnergyHighsigma=harr_origin_energy.EnergyHighsigma;
	 /**********************************************/

	 delete[]Origin_img;
	 delete[]Border_img;
#endif
#if _MSC_VER &&_DEBUG
QueryPerformanceCounter(&litmp);
QPart2 = litmp.QuadPart;
dfMinus = (double)(QPart2-QPart1);
dfTim = dfMinus / dfFreq;
float mstime=dfTim*1000;	
#endif
}
 /*----------------------------------------------------------------*/
 /**
 *�������г����ؿ��С������
 *
 *
 *@param  num_sp		 �����ؿ�ĸ���
 *@param p_SpProperty    �����ؿ����������
 *@param  cui_ImgData    ͼ�����
 *@param cui_ImgLables   �����ر궨����
 *@param cui_Width       ͼ����
 *@param cui_Height      ͼ��߶�
 */
 /*----------------------------------------------------------------*/
 void cui_GeneralImgProcess::CalculateAllSpBlockEnergy(
	 int num_sp,
	 SP_PROPERTY *p_SpProperty,
	 UINT32 * cui_ImgData,
	 int *cui_ImgLables,
	 int cui_Width,
	 int cui_Height){
#if _MSC_VER &&_DEBUG
	LARGE_INTEGER litmp;
	LONGLONG QPart1,QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// ��ü�������ʱ��Ƶ��
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// ��ó�ʼֵ
#endif

#if 1

	 for (register int i=0;i<num_sp;i++){
		CalculateSpBlockEnergy2(i,
			p_SpProperty,
			cui_ImgData,
			cui_ImgLables,
			cui_Width,
			cui_Height);
	 }	
#endif
#if _MSC_VER &&_DEBUG
        QueryPerformanceCounter(&litmp);	
		QPart2 = litmp.QuadPart;//�����ֵֹ	
		dfMinus = (double)(QPart2-QPart1);	
		dfTim = dfMinus / dfFreq;// ��ö�Ӧ��ʱ��ֵ����λΪ��	
		//Energy_LH = 1463.9357125974948
#endif
		
 }
 /*----------------------------------------------------------------*/
 /**
 *�������������ؿ�֮���������\n 
 *���㷽��Ϊ���Ƚ����������ؿ�һ��С��������B�Ͼ���
 *
 *@param  sp1 ��һ
 *@param  sp2 ���
 *@param [in]  p_SpProperty �����ؿ��Ӧ����������
 *@return �����ؿ�֮���������
 @note
 *��άС��������������(e_LL e_LH��e_HL��e_HH )��\n
 *����e_LL��������ͼ�������������ԡ�e_LH,e_HL,e_HH��������ͼ���Ƶ��������\n
 */
 /*----------------------------------------------------------------*/
  float cui_GeneralImgProcess::CalculateSpSimilar(
	  int sp1, 
	  int sp2,
	  SP_PROPERTY *p_SpProperty)
{
	 SP_PROPERTY sp_1=p_SpProperty[sp1];
	 SP_PROPERTY sp_2=p_SpProperty[sp2];
	    float Blike=0;
#if 0
 ///////////////////////////////////
	 double sum1=sp_1.harr_energy.Energy_LL+sp_1.harr_energy.Energy_LH+sp_1.harr_energy.Energy_HL+sp_1.harr_energy.Energy_HH;
	 double sum2=sp_2.harr_energy.Energy_LL+sp_2.harr_energy.Energy_LH+sp_2.harr_energy.Energy_HL+sp_2.harr_energy.Energy_HH;
	 ///////////////////////////////////
	 double Blike=(sqrtl(sp_1.harr_energy.Energy_LL*sp_2.harr_energy.Energy_LL)+ 
		           sqrtl(sp_1.harr_energy.Energy_LH*sp_2.harr_energy.Energy_LH)+ 
				   sqrtl(sp_1.harr_energy.Energy_HL*sp_2.harr_energy.Energy_HL)+ 
				   sqrtl(sp_1.harr_energy.Energy_HH*sp_2.harr_energy.Energy_HH))/(sum1*sum2);
#else
	 ///////////////////////////////////
	  float sum1=pow(sp_1.harr_energy.Energy_LL,2)+pow(sp_1.harr_energy.Energy_LH,2)+pow(sp_1.harr_energy.Energy_HL,2)+pow(sp_1.harr_energy.Energy_HH,2);
	  float sum2=pow(sp_2.harr_energy.Energy_LL,2)+pow(sp_2.harr_energy.Energy_LH,2)+pow(sp_2.harr_energy.Energy_HL,2)+pow(sp_2.harr_energy.Energy_HH,2);
	 sum1=sqrt(sum1);
	 sum2=sqrt(sum2);
	 ///////////////////////////////////
	 if ((int)sum1!=0&&(int)sum2!=0){
		  Blike=((sp_1.harr_energy.Energy_LL*sp_2.harr_energy.Energy_LL)+ 
		 (sp_1.harr_energy.Energy_LH*sp_2.harr_energy.Energy_LH)+ 
		 (sp_1.harr_energy.Energy_HL*sp_2.harr_energy.Energy_HL)+ 
		 (sp_1.harr_energy.Energy_HH*sp_2.harr_energy.Energy_HH))/(sum1*sum2);
	 }
	 if ((int)sum1==0||(int)sum2==0){
		  //����һ��Ϊ0
		 Blike=0;
	 }
	 if ((int)sum1==0&&(int)sum2==0) {
		 //ͬʱΪ0
		Blike=1;
	 }
	
#endif
	
	 return Blike;

 }
/*-------------------------------------------------------------------------------------------*/
/**
*��ʼ��PNGͼƬ����
*@param  imgData ͼ�����
*@param width    ͼ����
*@param  height   ͼ��߶�
*@param  value   ���ֵ
*/
/*-------------------------------------------------------------------------------------------*/
 void cui_GeneralImgProcess::InitPNGData(
	 UINT32* imgData,
	 int width, 
	 int height,
	 UINT32 value)
 {
	 for (register int x=0;x<width;x++){
		 for (register int y=0;y<height;y++){
			 imgData[y*width+x]=value;
		 }
	 }
 }
/*-------------------------------------------------------------------------------------------*/
/**
*����LAB��������B����
*@param ci i���
*@param cj j���
*@return B����
*/
/*-------------------------------------------------------------------------------------------*/
 float cui_GeneralImgProcess::LabDistanceAB(LabColor ci,LabColor cj)
 {
	 float abdst=sqrtl(pow(ci.A_Color-cj.A_Color,2)+pow(ci.B_color-cj.B_color,2));
	 ASSERT(abdst<=361&&abdst>=0);
	 float Adst=fabsf(ci.A_Color-cj.A_Color);
	 ASSERT(Adst<=255&&Adst>=0);
	 float Bdst=fabsf(ci.B_color-cj.B_color);
	 ASSERT(Bdst<=255&&Bdst>=0);
	 return abdst;
 }
/*-------------------------------------------------------------------------------------------*/
/**
*
*����LAB��������A����
*@param ci i�����ɫ
*@param cj j�����ɫ
*@return A����
*/
/*-------------------------------------------------------------------------------------------*/
 float cui_GeneralImgProcess::LabDistanceA(LabColor ci,LabColor cj)
 {
	
	 float Adst=fabsf(ci.A_Color-cj.A_Color);
	 ASSERT(Adst<=255&&Adst>=0);
	 return Adst;
 }
/*-------------------------------------------------------------------------------------------*/
/**
*
*����LAB��������B����
*@param ci i�����ɫ
*@param cj j�����ɫ
*@return B����
*/
/*-------------------------------------------------------------------------------------------*/
 float cui_GeneralImgProcess::LabDistanceB(LabColor ci,LabColor cj)
 {
	 float Bdst=fabsf(ci.B_color-cj.B_color);
	 ASSERT(Bdst<=255&&Bdst>=0);
	 return Bdst;
 }
/*-------------------------------------------------------------------------------------------*/
/**
*����LAB��������L����
*@param ci i���
*@param cj j���
*@return L����
*/
/*-------------------------------------------------------------------------------------------*/
 float cui_GeneralImgProcess::LabDistanceL(LabColor ci,LabColor cj)
 {
	 float abdst=0;
	 abdst=ci.L_Color-cj.L_Color;
	 return abdst;
 }
/*-------------------------------------------------------------------------------------------*/
/**
*����LAB�����أ������ļн�
*@param ci i��ɫ��
*@param cj j��ɫ��
*@return i,j�����н�
*/
/*-------------------------------------------------------------------------------------------*/
 float cui_GeneralImgProcess::LabDistanceABangle(LabColor ci,LabColor cj)
 {
	ASSERT(ci.A_Color>=-127&&ci.A_Color<=128);
	ASSERT(cj.B_color>=-127&&cj.B_color<=128);
	float I_Length=sqrtl(pow(ci.A_Color,2)+pow(ci.B_color,2));
	ASSERT(I_Length!=0);
	float J_Length=sqrtl(pow(cj.A_Color,2)+pow(cj.B_color,2));
	ASSERT(J_Length!=0);
	if ((I_Length<=10)
		&&(J_Length<=10)){
			return 0;
	}
	float IJ_DotProduct=ci.A_Color*cj.A_Color+ci.B_color*cj.B_color;
	float CosIJ=IJ_DotProduct/(I_Length*J_Length);
	float Angle=acos(CosIJ)*180/CV_PI;
	return CosIJ;
 }
/*-------------------------------------------------------------------------------------------*/
/**
*��ͼ��ת����8��������4ͨ��
*@param [in][out] src
*/
/*-------------------------------------------------------------------------------------------*/
 void cui_GeneralImgProcess::ConvertImg2Eighth4Ch(IplImage **src)
 {
	 TRACE_FUNC();
	 CvSize SizeOld=cvGetSize(*src);
	 CvSize SizeNew=cvGetSize(*src);
	 int depth=(*src)->depth;
	 int nChannels=(*src)->nChannels;
	
	 ASSERT(src!=nullptr);
	 ASSERT(*src!=nullptr);
	 if (SizeOld.width%8!=0){
		SizeNew.width=SizeOld.width-SizeOld.width%8+8;
	 }
	 if (SizeOld.height%8!=0){
		 SizeNew.height=SizeOld.height-SizeOld.height%8+8;	
	 }	
	 if ((SizeOld.width==SizeNew.width)
		 &&(SizeOld.height==SizeNew.height)){
		 return;
	 }
		IplImage *temp=cvCloneImage(*src);
		cvReleaseImage(src);
		*src=cvCreateImage(SizeNew,depth,nChannels);
		cvResize(temp,*src);
		cvReleaseImage(&temp);
#if 1
		ConvertImg3ChTo4Ch(src);
	//	cvSaveImage("A.png",*src);
#endif
		
 }
/*-------------------------------------------------------------------------------------------*/
/**
*��ͼ�����ͨ��ת������ͨ��
*@param [in][out] src ��ͨ����ͼ��
*/
/*-------------------------------------------------------------------------------------------*/
 void cui_GeneralImgProcess::ConvertImg3ChTo4Ch(IplImage **src)
 {
	 TRACE_FUNC();

	 ASSERT(src!=nullptr);
	 ASSERT(*src!=nullptr);

	 CvSize SizeOld=cvGetSize(*src);
	 int depth=(*src)->depth;
	 int nChannels=(*src)->nChannels;



	 if (nChannels==3){
		 IplImage *temp=cvCloneImage(*src);
		 cvReleaseImage(src);
		 *src=cvCreateImage(SizeOld,depth,4);
		 cvCvtColor(temp,*src,CV_BGR2BGRA);
		 cvReleaseImage(&temp);

	 }
	
 }
/*-------------------------------------------------------------------------------------------*/
/**
*������桢����ı߽���
*
*@param  Points  �߽����ϵĵ㼯��
*@param  pMD  ͼ������
*
*/
/*-------------------------------------------------------------------------------------------*/
 void cui_GeneralImgProcess::GetVGPoint(vector<CvPoint>& Points,ImageMemData* pMD)
 {
	 cui_GeneralImgProcess::Cui_CombinationImgSVG(
		 pMD->Src_ImgData,
		 pMD->ImgLables.get(),
		 pMD->p_ImgLables_SVG,
		 pMD->Matrix_Category_Lable_InDoor.get(),
		 pMD->ImgWidth,
		 pMD->ImgHeight,
		 "","","");
	 /*-----------------------------------------------*/
	  INT32* cui_ImgLables_SVG_t=pMD->p_ImgLables_SVG;
	 for (register int x=0;x<pMD->ImgWidth;x++){
		 for (register int y=0;y<pMD->ImgHeight;y++){
			 if (cui_ImgLables_SVG_t[y*pMD->ImgWidth+x]==Sky){
				 cui_ImgLables_SVG_t[y*pMD->ImgWidth+x]=Vertical;
			 }
		 }
	 }
	 UINT32* Contour_Split=new UINT32[pMD->ImgWidth*pMD->ImgHeight]; 
	 cui_GeneralImgProcess::InitPNGData(
		 Contour_Split,
		 pMD->ImgWidth,
		 pMD->ImgHeight,
		 BlackColorPNG);
	cui_GeneralImgProcess::DrawContoursAroundSegments(
		Contour_Split,
		cui_ImgLables_SVG_t,
		pMD->ImgWidth,
		pMD->ImgHeight,
		WhiteColorPNG);
	
	Points.clear();
	/*cui_ImgData_Contour_GVȡ�ֽ��ߵĵ�*/
	for (register int x=0;x<pMD->ImgWidth;x++){
		for (register int y=0;y<pMD->ImgHeight;y++){
			if (0!=(Contour_Split[y*pMD->ImgWidth+x]&0x00ffffff)){

				Points.push_back(cvPoint(x,y));			   
				//break;
			}
		}
	}
	delete[]Contour_Split;
 }
/*-------------------------------------------------------------------------------------------*/
/**
*�����ա�����ı߽���
*
*@param  Points  �߽����ϵĵ㼯��
*@param  pMD  ͼ������
*
*/
/*-------------------------------------------------------------------------------------------*/
 void cui_GeneralImgProcess::GetSVPoint(vector<CvPoint>& Points,ImageMemData* pMD)
 {
	 cui_GeneralImgProcess::Cui_CombinationImgSVG(
		 pMD->Src_ImgData,
		 pMD->ImgLables.get(),
		 pMD->p_ImgLables_SVG,
		 pMD->Matrix_Category_Lable_InDoor.get(),
		 pMD->ImgWidth,
		 pMD->ImgHeight,
		 "",
		 "",
		 "");
	 /*-----------------------------------------------*/
	 INT32* cui_ImgLables_SVG_t=pMD->p_ImgLables_SVG;
	 for (register int x=0;x<pMD->ImgWidth;x++){
		 for (register int y=0;y<pMD->ImgHeight;y++){
			 if (cui_ImgLables_SVG_t[y*pMD->ImgWidth+x]==Ground){
				 cui_ImgLables_SVG_t[y*pMD->ImgWidth+x]=Vertical;
			 }
		 }
	 }
	 UINT32* Contour_Split=new UINT32[pMD->ImgWidth*pMD->ImgHeight]; 
	 cui_GeneralImgProcess::InitPNGData(
		 Contour_Split,
		 pMD->ImgWidth,
		 pMD->ImgHeight,
		 BlackColorPNG);
	 cui_GeneralImgProcess::DrawContoursAroundSegments(
		 Contour_Split,
		 cui_ImgLables_SVG_t,
		 pMD->ImgWidth,
		 pMD->ImgHeight,
		 WhiteColorPNG);

	 Points.clear();
	 /*cui_ImgData_Contour_GVȡ�ֽ��ߵĵ�*/
	 for (register int x=0;x<pMD->ImgWidth;x++){
		 for (register int y=0;y<pMD->ImgHeight;y++){
			 if (0!=(Contour_Split[y*pMD->ImgWidth+x]&0x00ffffff)){

				 Points.push_back(cvPoint(x,y));			   
				 //break;
			 }
		 }
	 }
	 delete[]Contour_Split;
 }
/*-------------------------------------------------------------------*/
/**
*�Ƚ���ɫ������ƶ�
*@param ci iɫ���Labֵ
*@param cj jɫ���Labֵ
*@param DThreshold ŷʽ��ֵ
*@param AThreshold �Ƕ���ֵ
*@return
*@retval true  ����
*@retval false ������
*@note 2014��4��16��10:10:49 ---IMG_4070_Num_1000_Factor_8.png---1.2����ֵ
*/
/*-------------------------------------------------------------------*/
 bool cui_GeneralImgProcess::IsLabClorGroundNear(LabColor ci,LabColor cj,float DThreshold,float AThreshold)
 {
	 float abLike=cui_GeneralImgProcess::LabDistanceAB(ci,cj);
	 float Alike=cui_GeneralImgProcess::LabDistanceA(ci,cj);
	 float Blike=cui_GeneralImgProcess::LabDistanceB(ci,cj);
	 float ABangle=cui_GeneralImgProcess::LabDistanceABangle(ci,cj);
	 TRACE("D=%fA=%f\n ",abLike,ABangle);
	 ASSERT(DThreshold<=10);
	 ASSERT(AThreshold>=0.71);
	 if ((abLike<=DThreshold)
		 ||(ABangle>=AThreshold)){
			 return true;
	 }
	 return false;
 }
/*-------------------------------------------------------------------*/
/**
*Lab ��ɫ���ƶȱȽ�
*@param ci iɫ���Labֵ
*@param cj jɫ���Labֵ
*@return  
*@retval true ��ɫ������
*@retval false ��ɫ�鲻����
*@note \n
*�����㷨�ж�\n
*2014��4��16��10:10:49 ---IMG_4070_Num_1000_Factor_8.png---1.2����ֵ
*/
/*-------------------------------------------------------------------*/
 bool cui_GeneralImgProcess::IsLabClorGround_Insurance(LabColor ci,LabColor cj)
 {
	 return IsLabClorGroundNear(ci,cj,1.0,0.98);
 }
/*-------------------------------------------------------------------*/
/**
*�������ƾ���Ӳ��
*@param path				���浽Ӳ�̵�·��
*@param filename			�ļ���
*@param slic_current-num	���ƾ����ά�� ����==��
*@param Matrix_W			���ƾ���
*/
/*-------------------------------------------------------------------*/
 void cui_GeneralImgProcess::SaveMatrix_W(string path,string filename,int slic_current_num,double* Matrix_W)
{

	if (SAVE_DEBUG_2DISK)
	{
	/********************************************************/
		#ifdef WINDOWS
			char fname[256];
			char extn[256];
			_splitpath(filename.c_str(), NULL, NULL, fname, extn);
			string temp = fname;
			string finalpath = path + temp + string(".dat");
		#else
			
			string nameandextension =filename;
			size_t pos = filename.find_last_of("/");
			if(pos != string::npos)//if a slash is found, then take the filename with extension
			{
				nameandextension = filename.substr(pos+1);
			}
			string newname = nameandextension.replace(nameandextension.rfind(".")+1, 3, "dat");//find the position of the dot and replace the 3 characters following it.
			string finalpath = path+newname;
		#endif
			char data_t[1024];
			ofstream outfile;
			outfile.open(finalpath.c_str(),ios::out);
			for(register int i = 0; i <slic_current_num; i++ ){
				for(register int j = 0; j <slic_current_num; j++ ){
					double value_t=Matrix_W[i*slic_current_num+j];
					sprintf(data_t,"%+20.12e ",value_t);
					outfile<<data_t;
				}
				outfile<<endl;
			} 
			outfile.close();
/********************************************************/
	}

}
/*-------------------------------------------------------------------*/
/**
*�������ƾ���Ӳ��
*@param path				���浽Ӳ�̵�·��
*@param filename			�ļ���
*@param slic_current-num	���ƾ����ά�� ����==��
*@param Matrix_W			���ƾ���
*/
/*-------------------------------------------------------------------*/
void cui_GeneralImgProcess::SaveMatrix_Float(string path,string filename,int slic_current_num,float* Matrix_W)
{
	/********************************************************/
#ifdef WINDOWS
	char fname[256];
	char extn[256];
	_splitpath(filename.c_str(), NULL, NULL, fname, extn);
	string temp = fname;
	string finalpath = path + temp + string(".dat");
#else

	string nameandextension =filename;
	size_t pos = filename.find_last_of("/");
	if(pos != string::npos)//if a slash is found, then take the filename with extension
	{
		nameandextension = filename.substr(pos+1);
	}
	string newname = nameandextension.replace(nameandextension.rfind(".")+1, 3, "dat");//find the position of the dot and replace the 3 characters following it.
	string finalpath = path+newname;
#endif
	char data_t[1024];
	ofstream outfile;
	outfile.open(finalpath.c_str(),ios::out);
	for(register int i = 0; i <slic_current_num; i++ ){
		for(register int j = 0; j <slic_current_num; j++ ){
			double value_t=Matrix_W[i*slic_current_num+j];
			sprintf(data_t,"%+e ",value_t);
			outfile<<data_t;
		}
		outfile<<endl;
	} 
	outfile.close();
	/********************************************************/
}
/*-------------------------------------------------------------------*/
/**
*�������ƾ���Ӳ��
*@param path				���浽Ӳ�̵�·��
*@param filename			�ļ���
*@param slic_current-num	���ƾ����ά�� ����==��
*@param Matrix_W			���ƾ���
*/
/*-------------------------------------------------------------------*/
void cui_GeneralImgProcess::SaveVector_Double(string path,string filename,int slic_current_num,double* Matrix_W)
{
	/********************************************************/
#ifdef WINDOWS
	char fname[256];
	char extn[256];
	_splitpath(filename.c_str(), NULL, NULL, fname, extn);
	string temp = fname;
	string finalpath = path + temp + string(".dat");
#else

	string nameandextension =filename;
	size_t pos = filename.find_last_of("/");
	if(pos != string::npos)//if a slash is found, then take the filename with extension
	{
		nameandextension = filename.substr(pos+1);
	}
	string newname = nameandextension.replace(nameandextension.rfind(".")+1, 3, "dat");//find the position of the dot and replace the 3 characters following it.
	string finalpath = path+newname;
#endif
	char data_t[1024];
	ofstream outfile;
	outfile.open(finalpath.c_str(),ios::out);
	for(register int i = 0; i <slic_current_num; i++ ){
		{
			double value_t=Matrix_W[i];
			sprintf(data_t,"%+20.12e ",value_t);
			outfile<<data_t;
		}
		outfile<<endl;
	} 
	outfile.close();
	/********************************************************/
}
/*-------------------------------------------------------------------*/
/**
*���ͬ�Ļ�
*�޷����������Χ�������
*@param pMD ����ͼ��
*@param sky ���ͬ�Ļ����
*@param vertical ����ͬ�Ļ����
*@param ground ����ͬ�Ļ�
*@param bodersky ����Եͼ�����
*/
/*-------------------------------------------------------------------*/
 void cui_GeneralImgProcess::FillHoleOnSVGLables(ImageMemData* pMD,bool sky,bool vertical,bool ground,bool BorderSky){
	 INT32* Lables_SVG=pMD->p_ImgLables_SVG;	
	 IplImage *Lables_SVG_Img=cvCreateImage(cvSize(pMD->ImgWidth,pMD->ImgHeight),IPL_DEPTH_8U,4);
	 IplImage *Lables_S_Img=cvCreateImage(cvSize(pMD->ImgWidth,pMD->ImgHeight),IPL_DEPTH_8U,4);
	 IplImage *Lables_V_Img=cvCreateImage(cvSize(pMD->ImgWidth,pMD->ImgHeight),IPL_DEPTH_8U,4);
	 IplImage *Lables_G_Img=cvCreateImage(cvSize(pMD->ImgWidth,pMD->ImgHeight),IPL_DEPTH_8U,4);
	 ASSERT(Lables_SVG_Img->imageSize==pMD->ImgHeight*pMD->ImgWidth*sizeof(INT32));
	 memcpy(Lables_SVG_Img->imageData,Lables_SVG,Lables_SVG_Img->imageSize);
	 INT32* Lables_S=(INT32*)Lables_S_Img->imageData;
	 INT32* Lables_V=(INT32*)Lables_V_Img->imageData;
	 INT32* Lables_G=(INT32*)Lables_G_Img->imageData;
#if TRUE
	 if (sky==true){
		 for (int x=0;x<pMD->ImgWidth;x++){
			 for (int y=0;y<pMD->ImgHeight;y++){
				 unsigned long index=y*pMD->ImgWidth+x;
				 ASSERT(index<pMD->ImgWidth*pMD->ImgHeight);
				 if (Lables_SVG[index]==Sky){
					 Lables_S[index]=WhiteColorPNG;
				 }else{
					 Lables_S[index]=BlackColorPNG;
				 }
			 }
		 }
		 if (BorderSky==true){
			 DrawS_V_G_Lables_BorderLine(Lables_S_Img,Sky);
		 }
		 cui_GeneralImgProcess::GetContour2Fill(Lables_S_Img,Lables_SVG_Img,SkyCv);
		 memcpy(Lables_SVG,Lables_SVG_Img->imageData,Lables_SVG_Img->imageSize);
	 }
	 if (vertical==true){
		 for (int x=0;x<pMD->ImgWidth;x++){
			 for (int y=0;y<pMD->ImgHeight;y++){
				 unsigned long index=y*pMD->ImgWidth+x;
				 ASSERT(index<pMD->ImgWidth*pMD->ImgHeight);
				 if (Lables_SVG[index]==Vertical){
					 Lables_V[index]=WhiteColorPNG;
				 }else{
					 Lables_V[index]=BlackColorPNG;
				 }
			 }
		 }
		 cui_GeneralImgProcess::GetContour2Fill(Lables_V_Img,Lables_SVG_Img,VerticalCv);
		 memcpy(Lables_SVG,Lables_SVG_Img->imageData,Lables_SVG_Img->imageSize);
	 }
	 if (ground==true){
		 for (int x=0;x<pMD->ImgWidth;x++){
			 for (int y=0;y<pMD->ImgHeight;y++){
				 unsigned long index=y*pMD->ImgWidth+x;
				 ASSERT(index<pMD->ImgWidth*pMD->ImgHeight);
				 if (Lables_SVG[index]==Ground){
					 Lables_G[index]=WhiteColorPNG;
				 }else{
					 Lables_G[index]=BlackColorPNG;
				 }
			 }
		 }
		 cui_GeneralImgProcess::GetContour2Fill(Lables_G_Img,Lables_SVG_Img,GroundCv);
		 memcpy(Lables_SVG,Lables_SVG_Img->imageData,Lables_SVG_Img->imageSize);
	 }
#endif
	 cvReleaseImage(&Lables_SVG_Img);
	 cvReleaseImage(&Lables_S_Img);
	 cvReleaseImage(&Lables_V_Img);
	 cvReleaseImage(&Lables_G_Img);

	 SplitTreeandBuilding  STB(pMD->p_ImgLables_SVG,pMD->ImgLables.get(),pMD);
	 STB.ReBuildingImgLab();
 }
/*-------------------------------------------------------------------*/
/**
*����������� ���� �ϵı�Ե����������ȡ
*@param[in] img ����ͼ��
*@param category �����λ��
*/
/*-------------------------------------------------------------------*/
void cui_GeneralImgProcess::DrawS_V_G_Lables_BorderLine(IplImage *img,UINT32 category)
{
	 //cvLine(Lables_S_Img,);
	vector<int> Y_0;
	vector<int> X_0;
	vector<int> X_W;
	vector<int> Y_W;
	 INT32* Lables=(INT32*)img->imageData;
	for (int x=0;x<img->width;x++){
		for (int y=0;y<img->height;y++){
			unsigned long index=y*img->width+x;
			ASSERT(index<img->width*img->height);
			if (Lables[index]==WhiteColorPNG){
				if (y==0){
					Y_0.push_back(x);
				}
				if (y==img->height-1){
					Y_W.push_back(x);
				}
				if (x==0){
					X_0.push_back(y);
				}
				if (x==img->width-1){
					X_W.push_back(y);
				}

			}else if (Lables[index]==BlackColorPNG){
				;
			}else{
				ASSERT(FALSE);
			}
		}
	}
	if(category==Sky){
		cvLine(img,cvPoint(0,0),cvPoint(img->width,0), WhiteColorPNGCv,2);
		cvLine(img,cvPoint(0,0),cvPoint(0,img->height), WhiteColorPNGCv,2);
		cvLine(img,cvPoint(img->width,0),cvPoint(img->width,img->height), WhiteColorPNGCv,2);
	}
#if TRUE
	if (SAVE_DEBUG_2DISK==TRUE){
		cvSaveImage("Sky_lab.jpg",img);
	}
	
#endif

}
/*-------------------------------------------------------------------------*/
/**
*��ø��������У�0=��ĸ���
*@param  data  ����
*@param  Dim   ����ά��
*/
/*-------------------------------------------------------------------------*/
int cui_GeneralImgProcess::GetZeroCount(float*data,int Dim)
{
	int ZeroCount=0;
	for (int i=0;i<Dim;i++){
		if (data[i]==0){
			ZeroCount++;
		}
	}
	return ZeroCount;
}
/*-------------------------------------------------------------------------*/
/**
*��ø��������У�0=��ĸ���*��Ȩ
*@param  data  ����
*@param  Dim   ����ά��
*/
/*-------------------------------------------------------------------------*/
int cui_GeneralImgProcess::GetZeroCountContinueWeight(float*data,int Dim)
{
	int ZeroCount=0;
	for (int di=0;di<Dim;di++){
		if (data[di]==0){
			int _start_i=di;
			while(0==data[++di]);
			di--;
			int _end_i=di;
			int weight=_end_i-_start_i;
			ZeroCount+=weight;
		}
	}
	return ZeroCount;
}
/*-------------------------------------------------------------------------*/
/**
*��ȡ����ͼ�����ӵĲ�ɫֱ��ͼ�ͻҶ�ֱ��ͼ
*@param [in] kseedsl   
*@param [in] kseedsa   
*@param [in] kseedsb   
*@param [in] kseedsx   
*@param [in] kseedsy   
*@param [out] seeddata   �Ҷ���ɫ
*@param [in]  CuiImgLables   ͼ��궨��Ϣ
*@param  ImgWidth   ͼ����
*@param  ImgHeight   ͼ��߶�
*@param  FileReadFullPath   �ļ���ȡ·��
*@param  FileWritePath      �ļ�д��·��
*/
/*-------------------------------------------------------------------------*/
void cui_GeneralImgProcess::Get_Kseeds_Histogram(
	vector<double> kseedsl,
	vector<double> kseedsa,
	vector<double> kseedsb,
	vector<double> kseedsx,
	vector<double> kseedsy,
	KseedsHistData& seeddata,
	int* CuiImgLables,
	int ImgWidth,
	int ImgHeight,
	string FileReadFullPath,
	string FileWritePath)
{
#if _MSC_VER
    LARGE_INTEGER litmp;
	LONGLONG QPart1,QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// ��ü�������ʱ��Ƶ��
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// ��ó�ʼֵ
#endif
	/*---------------------------------------------------*/
	{
	int HistDimSPLAB=seeddata.histDimSPLAB;
	//���������ӱ궨��ͼ��
	int vectorSize=kseedsa.size();
#if SaveAverageImg2Disk
	
	{
		int negcount;
		//opencv L, a, b����ȡֵ��Ϊ�� 0 - 255
		//origin L 0-100  a -127--128   b-127----128
		IplImage* img=cvCreateImage(cvSize(ImgWidth,ImgHeight),IPL_DEPTH_8U,3);
		for (int xi=0;xi<ImgWidth;xi++){
			for (int yj=0;yj<ImgHeight;yj++){
				int seedIdx=CuiImgLables[yj*ImgWidth+xi];
				assert(seedIdx>=0&&seedIdx<vectorSize);
				double L=kseedsl[seedIdx]*2.55;
				double A=kseedsa[seedIdx]+127;
				double B=kseedsb[seedIdx]+127;
				CvScalar v_lab =cvScalar(L,A,B);
				cvSet2D(img,yj,xi,v_lab);
			}
		}
		cvCvtColor(img,img,CV_Lab2BGR);
		FileNameSplit fns;
		fns.Parse(CString(FileReadFullPath.c_str()));
		string filesaveimg_t=FileWritePath+FileNameSplit::ConvertCS2string(fns.filename)+"SpSeeds.jpg";
		cvSaveImage(filesaveimg_t.c_str(),img);
		cvReleaseImage(&img);
	}
#endif
#if 1
	/////////////////////////////////////////////////////////////////
	for (int i=0;i<vectorSize;i++){
		double L=kseedsl[i];//0-100
		double A=kseedsa[i];//-127--128
		double B=kseedsb[i];//-127--128
		double AB_length=sqrt(A*A+B*B);
		double angle=atan2(B,A);//-PI---PI
		double angule_360=(angle+M_PI)*180/M_PI;
		assert(angule_360>=-0.1&&angule_360<=360.1);
		int angle_index=cvFloor(angule_360/360*HistDimSPLAB);
		//////////////////////////////////////////
		if (AB_length>1){
			seeddata.hist_AB_CountOrg[angle_index]+=1;
			seeddata.LAvg[angle_index]+=L;
			seeddata.hist_AB_Count[angle_index]+=1;
			seeddata.AB_lengthAvg[angle_index]+=AB_length;
			seeddata.hist_AB_detail[angle_index].push_back(i);
		}else{
			/*seeddata.hist_AB_CountOrg[seeddata.histWhiteIdx]+=1;
			seeddata.LAvg[seeddata.histWhiteIdx]+=L;
			seeddata.hist_AB_Count[seeddata.histWhiteIdx]+=1;
			seeddata.AB_lengthAvg[seeddata.histWhiteIdx]+=AB_length;
			seeddata.hist_AB_detail[seeddata.histWhiteIdx].push_back(i);*/
		}	
		//////////////////////////////////////////
	}
	//////////////////////////////////////////
	for (int j=0;j<HistDimSPLAB;j++){
		if (seeddata.hist_AB_Count[j]>0){
			seeddata.LAvg[j]=seeddata.LAvg[j]/seeddata.hist_AB_Count[j];
			seeddata.AB_lengthAvg[j]=seeddata.AB_lengthAvg[j]/seeddata.hist_AB_Count[j];
		}
	}
	//double AB_count_max=cui_GeneralImgProcess::GetMaxValue(&seeddata.AB_lengthAvg[0],HistDimSPLAB);
	////////////////////////////////////////////////////////////////////
	//int img_scale=2;
	//int bin_w=1800/HistDimSPLAB*img_scale;
	//int h_bins=HistDimSPLAB;
	//int height = 240*img_scale;
	//int scale = 2;  
	//int width =h_bins*bin_w;
	//
	//char  text_buff_t[1024];
	/////////////////////////////////////////////////////////////////////////////////////
	//for(int h = 0; h <h_bins; h++){
	//	/** ���ֱ��ͼ�е�ͳ�ƴ�����������ʾ��ͼ���еĸ߶� */
	//	float bin_val =hist_AB[h];
	//	int intensity =cvRound(bin_val*height/AB_count_max);
	//	intensity=intensity<0?0:intensity;
	//	/** ��õ�ǰֱ��ͼ�������ɫ��ת����RGB���ڻ��� */
	//	float sita=M_PI*(2*(h+0.5)/HistDimSPLAB-1);
	//	CvScalar color=cvScalar(seeddata.LAvg[h]*2.55,seeddata.AB_lengthAvg[h]*sin(sita)+127,seeddata.AB_lengthAvg[h]*cos(sita)+127);
	//	
	//	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	if (seeddata.hist_AB_CountRemove[h]==-1){
	//		
	//	}
	//	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	CvFont font;
	//	cvInitFont(&font,CV_FONT_VECTOR0,0.3,0.3,0,1,8);
	//	sprintf(text_buff_t,"%d",h*180/HistDimSPLAB);
	//	

	//}	
	/*for(int h = 0; h <h_bins; h++){	
		int cut_num=9;
		int cut_step=h_bins/cut_num;
		if (h%cut_step==0){
			
		}		
	}*/
	/////////////////////////////////////////////////////////////////////////////////////	
#endif
	}
	/*---------------------------------------------------*/
#if _MSC_VER
    QueryPerformanceCounter(&litmp);
	QPart2 = litmp.QuadPart;//�����ֵֹ
	dfMinus = (double)(QPart2-QPart1);
	dfTim = dfMinus / dfFreq;// ��ö�Ӧ��ʱ��ֵ����λΪ��
	TRACE("\n ͳ����ɫֱ��ͼ�ͻҶ�ֱ��ͼ: %f���룩",dfTim);
#endif
	

}
/*-------------------------------------------------------------------------*/
/**
*��������ͳ�ƵĻҶ�ֱ��ͼ
*��ɫֱ��ͼ��ڰ�ֱ��ͼ
*@param  [in]  kseedsl    ���ӵ�L����
*@param  [in]  kseedsa    ���ӵ�A����
*@param  [in]  kseedsb    ���ӵ�B����
*@param  [in]  CuiImgLables   ͼ���labels��Ϣ
*@param    ImgWidth           ͼ����
*@param    ImgHeight           ͼ��߶�
*@param    FileReadFullPath   �ļ���ȡ·��
*@param    FileWritePath      �ļ�����·��
*/
/*-------------------------------------------------------------------------*/
void cui_GeneralImgProcess::Draw_Kseeds_AverageImg(
	vector<double> kseedsl,
	vector<double> kseedsa,
	vector<double> kseedsb,
	int* CuiImgLables,
	int ImgWidth,
	int ImgHeight,
	string FileReadFullPath,
	string FileWritePath)
{
#if SaveAverageImg2Disk || _DEBUG
//���������ӱ궨��ͼ��
	int vectorSize=kseedsa.size();
	{
		int negcount;
		//opencv L, a, b����ȡֵ��Ϊ�� 0 - 255
		//origin L 0-100  a -127--128   b-127----128
		IplImage* img=cvCreateImage(cvSize(ImgWidth,ImgHeight),IPL_DEPTH_8U,3);
		for (int xi=0;xi<ImgWidth;xi++){
			for (int yj=0;yj<ImgHeight;yj++){
				int seedIdx=CuiImgLables[yj*ImgWidth+xi];
				assert(seedIdx>=0&&seedIdx<vectorSize);
				double L=kseedsl[seedIdx]*2.55;
				double A=kseedsa[seedIdx]+127;
				double B=kseedsb[seedIdx]+127;
				CvScalar v_lab =cvScalar(L,A,B);
				cvSet2D(img,yj,xi,v_lab);
			}
		}
		cvCvtColor(img,img,CV_Lab2BGR);
		FileNameSplit fns;
#if Use_CString
		fns.Parse(CString(FileReadFullPath.c_str()));
		string filesaveimg_t=FileWritePath+FileNameSplit::ConvertCS2string(fns.filename)+"SpSeeds.jpg";
#else
		string path= string(FileReadFullPath);
#if _MSC_VER
		int pos = path.find_last_of('\\');
#else
		int pos=0;
#endif	
		string file_name_t(path.substr(pos + 1) );

		string filesaveimg_t=FileWritePath+file_name_t+"SpSeeds.jpg";

#endif
		cvSaveImage(filesaveimg_t.c_str(),img);
		cvReleaseImage(&img);
	}
#endif
	
}
/*-------------------------------------------------------------------------*/
/**
*��ȡ����ͼ�����ӵĲ�ɫֱ��ͼ�ͻҶ�ֱ��ͼ
*@param [in] kseedsl   
*@param [in] kseedsa   
*@param [in] kseedsb   
*@param [in] kseedsx   
*@param [in] kseedsy   
*@param [out] seeddata   �Ҷ���ɫ
*@param [in]  CuiImgLables   ͼ��궨��Ϣ
*@param  ImgWidth   ͼ����
*@param  ImgHeight   ͼ��߶�
*@param  FileReadFullPath   �ļ���ȡ·��
*@param  FileWritePath      �ļ�д��·��
*
*/
/*-------------------------------------------------------------------------*/
void cui_GeneralImgProcess::Get_Kseeds_Histogram(
	vector<double> kseedsl,
	vector<double> kseedsa,
	vector<double> kseedsb,
	vector<double> kseedsx,
	vector<double> kseedsy,
	KseedsHistColorGray& seeddata,
	int* CuiImgLables,
	int ImgWidth,
	int ImgHeight,
	string FileReadFullPath,
	string FileWritePath)
{
	TRACE_FUNC();
	int HistDimSPLABColor=seeddata.ColorHist.histDimSPLAB;
	int HIstDimSPLGray=seeddata.GrayHist.histDimSPLAB;
	int vectorSize=kseedsa.size();
	seeddata.GrayHist.Clear();
	seeddata.ColorHist.Clear();
#if _DEBUG
	Draw_Kseeds_AverageImg(
		kseedsl,kseedsa,kseedsb,
		CuiImgLables,
		ImgWidth,
		ImgHeight,
		FileReadFullPath,
		FileWritePath);
#endif	

#if 1
	/////////////////////////////////////////////////////////////////
	for (int i=0;i<vectorSize;i++){
		double L=kseedsl[i];//0-100
		double A=kseedsa[i];//-127--128
		double B=kseedsb[i];//-127--128
		double AB_length=sqrt(A*A+B*B);
		double angle=atan2(B,A);//-PI---PI
		double angule_360=(angle+M_PI)*180/M_PI;
		assert(angule_360>=-0.1&&angule_360<=360.1);
		double	angule_idx_float=1.0*angule_360/360*HistDimSPLABColor;
		double	gray_idx_float=1.0*L/100*HIstDimSPLGray;
		int angle_index=cvRound(angule_idx_float);//�����������뷨
		int gray_index=cvRound(gray_idx_float);
		//////////////////////////////////////////
		if (gray_index==HIstDimSPLGray){	gray_index=HIstDimSPLGray-1;	}
		if (angle_index==HistDimSPLABColor){	angle_index=HistDimSPLABColor-1;	}
		//////////////////////////////////////////
		//4
		if (AB_length>2){
			//�Ƕ�ֱ��ͼ
			seeddata.ColorHist.hist_AB_CountOrg[angle_index]+=1;
			seeddata.ColorHist.LAvg[angle_index]+=L;
			seeddata.ColorHist.hist_AB_Count[angle_index]+=1;
			seeddata.ColorHist.AB_lengthAvg[angle_index]+=AB_length;
			seeddata.ColorHist.hist_AB_detail[angle_index].push_back(i);
		}else{
			//�Ҷ�ֱ��ͼ
			if (gray_index==seeddata.GrayHist.histDimSPLAB) gray_index--;
			seeddata.GrayHist.hist_AB_CountOrg[gray_index]+=1;
			seeddata.GrayHist.LAvg[gray_index]+=L;
			seeddata.GrayHist.hist_AB_Count[gray_index]+=1;
			seeddata.GrayHist.AB_lengthAvg[gray_index]+=AB_length;
			seeddata.GrayHist.hist_AB_detail[gray_index].push_back(i);
		}	
		//////////////////////////////////////////
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int j=0;j<HistDimSPLABColor;j++){
		if (seeddata.ColorHist.hist_AB_Count[j]>0){
			seeddata.ColorHist.LAvg[j]=seeddata.ColorHist.LAvg[j]/seeddata.ColorHist.hist_AB_Count[j];
			seeddata.ColorHist.AB_lengthAvg[j]=seeddata.ColorHist.AB_lengthAvg[j]/seeddata.ColorHist.hist_AB_Count[j];
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int j=0;j<HIstDimSPLGray;j++){
		if (seeddata.GrayHist.hist_AB_Count[j]>0){
			seeddata.GrayHist.LAvg[j]=seeddata.GrayHist.LAvg[j]/seeddata.GrayHist.hist_AB_Count[j];
			seeddata.GrayHist.AB_lengthAvg[j]=seeddata.GrayHist.AB_lengthAvg[j]/seeddata.GrayHist.hist_AB_Count[j];
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
}
/*-------------------------------------------------------------------------*/
/**
*��������ͳ�ƵĻҶ�ֱ��ͼ
*��ɫֱ��ͼ��ڰ�ֱ��ͼ
*@param  [in]  kseedsl    ���ӵ�L����
*@param  [in]  kseedsa    ���ӵ�A����
*@param  [in]  kseedsb    ���ӵ�B����
*@param  [in]  kseedsx    ���ӵ�X����
*@param  [in]  kseedsy    ���ӵ�Y����
*@param  [in]  seeddata    ������ɫֱ��ͼ�ͻҶ�ֱ��ͼ����
*@param  [in]  CuiImgLables   ͼ���labels��Ϣ
*@param    ImgWidth           ͼ����
*@param    ImgHeight           ͼ��߶�
*@param    FileReadFullPath   �ļ���ȡ·��
*@param    FileWritePath      �ļ�����·��
*
*/
/*-------------------------------------------------------------------------*/
void cui_GeneralImgProcess::Draw_Kseeds_Histogram(
	vector<double> kseedsl,
	vector<double> kseedsa,
	vector<double> kseedsb,
	vector<double> kseedsx,
	vector<double> kseedsy,
	KseedsHistData& seeddata,
	int* CuiImgLables,
	int ImgWidth,
	int ImgHeight,
	string FileReadFullPath,
	string FileWritePath)
{
	int HistDimSPLAB=seeddata.histDimSPLAB;
	int vectorSize=kseedsa.size();
#if SaveHistgram2Disk &&_DEBUG
	/////////////////////////////////////////////////////////////////
	double* hist_L=new double[HistDimSPLAB];//0-100
	double* hist_AB=new double[HistDimSPLAB];//-127--128
	double* hist_Count=new double[HistDimSPLAB];//count
	double* hist_AB_length=new double[HistDimSPLAB];//128*1.414
	memcpy(hist_L,seeddata.LAvg.data(),sizeof(double)*HistDimSPLAB);
	memcpy(hist_AB,seeddata.hist_AB_Count.data(),sizeof(double)*HistDimSPLAB);
	memcpy(hist_Count,seeddata.hist_AB_CountOrg.data(),sizeof(double)*HistDimSPLAB);
	memcpy(hist_AB_length,seeddata.AB_lengthAvg.data(),sizeof(double)*HistDimSPLAB);
	double AB_count_max=cui_GeneralImgProcess::GetMaxValue(&hist_AB[0],HistDimSPLAB);
	//////////////////////////////////////////////////////////////////
	int img_scale=2;
	int bin_w=1800/HistDimSPLAB*img_scale;
	int h_bins=HistDimSPLAB;
	int height = 240*img_scale;
	int scale = 2;  
	int width =h_bins*bin_w;
	IplImage* hist_img = cvCreateImage( cvSize(width,height), 8, 3 );
	cvRectangle( hist_img, cvPoint(0,0),cvPoint(width,height),cvScalar(255,255/2,255/2),-1, 8, 0 );
	char  text_buff_t[1024];
	/////////////////////////////////////////////////////////////////////////////////////
	for(int h = 0; h <h_bins; h++){
		/** ���ֱ��ͼ�е�ͳ�ƴ�����������ʾ��ͼ���еĸ߶� */
		float bin_val =hist_AB[h];
		int intensity =cvRound(bin_val*height/AB_count_max);
		intensity=intensity<0?0:intensity;
		/** ��õ�ǰֱ��ͼ�������ɫ��ת����RGB���ڻ��� */
		float sita=M_PI*(2*(h+0.5)/HistDimSPLAB-1);
		CvScalar color=cvScalar(hist_L[h]*2.55,hist_AB_length[h]*sin(sita)+127,hist_AB_length[h]*cos(sita)+127);
		cvRectangle( hist_img, cvPoint(h*bin_w,height),cvPoint((h+1)*bin_w,height - intensity),cvScalar(0,255/2,255/2),2);
		cvRectangle( hist_img, cvPoint(h*bin_w,height),cvPoint((h+1)*bin_w,height - intensity+1),color,-1, 8, 0 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if (seeddata.hist_AB_CountRemove[h]<0){
			cvRectangle( hist_img, cvPoint(h*bin_w,height),cvPoint((h+1)*bin_w,0),cvScalar(255/2,255/2,255/2),3);
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CvFont font;
		cvInitFont(&font,CV_FONT_VECTOR0,0.3,0.3,0,1,8);
		sprintf(text_buff_t,"%d",h);
		cvPutText(hist_img,text_buff_t,cvPoint(h*bin_w,height),&font,CV_RGB(255,255,255));

	}	
	if (seeddata.histDimSPLAB==180){
		for(int h = 0; h <h_bins; h++){	
			int cut_num=9;
			int cut_step=h_bins/cut_num;
			if (h%cut_step==0){
				cvLine(hist_img, cvPoint((h+0.5)*bin_w,height),cvPoint((h+0.5)*bin_w,0),cvScalar(180,128,128+70),1);
			}		
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////
	cvCvtColor(hist_img,hist_img,CV_Lab2BGR);	
	FileNameSplit fns;
	fns.Parse(CString(FileReadFullPath.c_str()));
	string time_t=cui_GeneralImgProcess::cuiGetCurrentTime();
	string filesaveimg=FileWritePath+FileNameSplit::ConvertCS2string(fns.filename)+"Histogram"+time_t+".jpg";
	cvSaveImage( filesaveimg.c_str(), hist_img );//��"H-S Histogtam"��������ʾͼ��
	cvReleaseImage(&hist_img);
	delete[] hist_L;//0-100
	delete[] hist_AB;//-127--128
	delete[] hist_Count;//count
	delete[] hist_AB_length;//128*1.414
#endif

}
/*-------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------*/
void  cui_GeneralImgProcess::DetermineColorRank(ImageMemData* pMD,int RankNum)
{
	float* EnergyHighsigmaArray=new float[pMD->slic_current_num];
	for(int i=0;i<pMD->slic_current_num;i++){	
		EnergyHighsigmaArray[i]=pMD->p_SpProperty[i].harr_energy.EnergyHighsigma;
	}
	float MaxEnergyHighsigma=cui_GeneralImgProcess::GetMaxValue(EnergyHighsigmaArray,pMD->slic_current_num);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for(int i=0;i<pMD->slic_current_num;i++){	
		pMD->p_SpProperty[i].harr_energy.EnergyGrade=pMD->p_SpProperty[i].harr_energy.EnergyHighsigma/MaxEnergyHighsigma*RankNum;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	delete[] EnergyHighsigmaArray;
}
/*-------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------*/
void cui_GeneralImgProcess::ShowImgLabels(int* ImgLabels,int Width,int Height)
{
IplImage *imglabels=cvCreateImage(cvSize(Width,Height),IPL_DEPTH_8U,4);
	 memcpy(imglabels->imageData,ImgLabels,sizeof(int)*Width*Height);
	 cvSaveImage("spLabels.jpg",imglabels);
	 cvReleaseImage(&imglabels);
}
/*-------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------*/
int cui_GeneralImgProcess::get_CPU_core_num() 
{ 
#if defined(WIN32) 
	SYSTEM_INFO info; 
	GetSystemInfo(&info); 
	return info.dwNumberOfProcessors/2; 
#elif defined(LINUX) || defined(SOLARIS) || defined(AIX) || defined(linux) || defined(__linux__)
	return get_nprocs();   //GNU fuction 
#else 
#error  ��֧�ֵĲ���ϵͳ 
#endif 
} 
/*-------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------*/
unsigned cui_GeneralImgProcess::THreadSuperPixel_DoOneImage_win(LPVOID lpParam)
{
	ThreadDoOneImageData* tdoid=(ThreadDoOneImageData*)lpParam;

	for(int k = tdoid->start; k <tdoid->picvec.size(); k+=tdoid->step){

		if(k>tdoid->picvec.size()){
			return 0;
		}
		THreadSuperPixel_DoOneImage(tdoid->picvec[k],tdoid->saveLocation,tdoid->m_spcount);
	}
#if __GUNC__||linux||__linux||__linux__

	 pthread_exit(0); 

#endif
	return 0;
}
/*-------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------*/
unsigned cui_GeneralImgProcess::THreadSuperPixel_DoOneImage_win_spiral(LPVOID lpParam)
{
	ThreadDoOneImageData* tdoid=(ThreadDoOneImageData*)lpParam;

	for(int k = tdoid->start; k <tdoid->picvec.size(); k+=tdoid->step){

		if(k>tdoid->picvec.size()){
			return 0;
		}
		THreadSuperPixel_DoOneImage_Spiral(tdoid->picvec[k],tdoid->saveLocation,tdoid->m_spcount);
	}
#if __GUNC__||linux||__linux||__linux__

	pthread_exit(0); 

#endif
	return 0;
}
/*-------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------*/
void cui_GeneralImgProcess::THreadSuperPixel_DoOneImage(string picvec,string saveLocation,int m_spcount)
{
#if _MSC_VER
	LARGE_INTEGER litmp;
	LONGLONG QPart1,QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// ��ü�������ʱ��Ƶ��
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// ��ó�ʼֵ
	/////////////////////////////////////////////
#endif


	
	TimeCountStart();
	{
		printf("1. ImageData \n");
		ImageData MemData(picvec,saveLocation,m_spcount,0.5);

		printf("2. GPU Super \n");
		SLIC slic(&MemData);
		slic.DoSuperpixelSegmentation_ForGivenNumberOfSuperpixels_sitaMLxy();//�õ�lable				

#if OUT_DOOR_HUMAN
		printf("3. Spectral Clustering \n");
		slic.Cui_Spectral_Clustering_B_2016_09_26();

		ComputeSVG2 svg(&MemData);
		svg.separateSVG_Zlm();

		MemData.SaveImgWithContours("ColorCluster");
		MemData.Draw_Kseeds_AverageImg();
#endif

#if OUT_DOOR_400_IMAGE_STABLE
		
		printf("3. ColorBarCluster \n");
		ColorBarCluster colorBarCluster(&MemData);
		colorBarCluster.Clustering();

		printf("4. ComputeSVG2 \n");
		ComputeSVG2 svg(&MemData);
		svg.separateSVG_Zlm();

#endif

		MemData.SeparateSp();

		

//#if	!(SaveContours2Disk)
//		MemData.SaveImgWithContours();			
//#endif

	}
	TimeCountStop("Do One Image Cost Time :");

#if _MSC_VER
	///////////////////////////////////////////////
	QueryPerformanceCounter(&litmp);
	QPart2 = litmp.QuadPart;//�����ֵֹ
	dfMinus = (double)(QPart2-QPart1);
	dfTim = dfMinus / dfFreq;// ��ö�Ӧ��ʱ��ֵ����λΪ��
	TRACE("\n ȫ��ʱ��: %f���룩",dfTim);
#endif
}
/*-------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------*/
void cui_GeneralImgProcess::THreadSuperPixel_DoOneImage_Spiral(string picvec,string saveLocation,int m_spcount)
{
	//2017��2��21��13:02:40
	
#if _MSC_VER
	LARGE_INTEGER litmp;
	LONGLONG QPart1,QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// ��ü�������ʱ��Ƶ��
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// ��ó�ʼֵ
	/////////////////////////////////////////////
#endif
	
	TimeCountStart();
	{
							printf("1. ImageData \n");
							ImageData MemData(picvec,saveLocation,m_spcount,0.5);


							MemData.GetThetaMLXYSeeds_ByCircle_UseSpiral();
							MemData.Draw_Kseeds_Spiral();

							

							printf("2. GPU Super \n");
							SLIC slic(&MemData);
							
							slic.DoSuperpixelSegmentation_ForGivenNumberOfSuperpixels_sitaMLxy();//�õ�lable				

							MemData.GetThetaMLXYSeeds_ByCircle_UseSpiral();
							MemData.Draw_Kseeds_Spiral_OnContour();

							MemData.GetMatrixE();

							/*	#if OUT_DOOR_HUMAN
							printf("3. Spectral Clustering \n");
							slic.Cui_Spectral_Clustering_B_2016_09_26();

							ComputeSVG2 svg(&MemData);
							svg.separateSVG_Zlm();

							MemData.SaveImgWithContours("ColorCluster");
							MemData.Draw_Kseeds_AverageImg();
							#endif

							#if OUT_DOOR_400_IMAGE_STABLE

							printf("3. ColorBarCluster \n");
							ColorBarCluster colorBarCluster(&MemData);
							colorBarCluster.Clustering();

							printf("4. ComputeSVG2 \n");
							ComputeSVG2 svg(&MemData);
							svg.separateSVG_Zlm();

							#endif
							MemData.SeparateSp();*/

							//#if	!(SaveContours2Disk)
							//		MemData.SaveImgWithContours();			
							//#endif

	}
	TimeCountStop("Do One Image Cost Time :");

#if _MSC_VER
	///////////////////////////////////////////////
	QueryPerformanceCounter(&litmp);
	QPart2 = litmp.QuadPart;//�����ֵֹ
	dfMinus = (double)(QPart2-QPart1);
	dfTim = dfMinus / dfFreq;// ��ö�Ӧ��ʱ��ֵ����λΪ��
	TRACE("\n ȫ��ʱ��: %f���룩",dfTim);
#endif
}
/*-------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------*/
UINT cui_GeneralImgProcess::THreadSuperPixel_CUDA_CollectionMethods(LPVOID lpParam,vector<string> picvec,string saveLocation,int m_spcount)
{
//struct pt_sem SEM_CPU_NUMS;
//PT_SEM_INIT(&SEM_CPU_NUMS,get_CPU_core_num()); //��ʼ���ź���Ϊ1����û����
	TimeCountStart();
	vector <double>  Super_Pixel_num_f;
	vector <double>  Do_Time_f;	
	int numPics( picvec.size() );
	/****************************************/
	printf("Start: CUDA_CollectionMethods \n");
	/****************************************/
	int CPU_NUMS=get_CPU_core_num();
	printf("Start: CPU nums %d \n",CPU_NUMS);
	
	vector<ThreadDoOneImageData *> data;


#if _MSC_VER
	/*if (H_SEM_CPU_NUMS==NULL){
		H_SEM_CPU_NUMS = CreateSemaphore(NULL, 1, 10, NULL); 
	}*/	
	//CPU_NUMS+=CPU_NUMS;	
	vector<HANDLE> handle;
	
	for(int k = 0; k <min(CPU_NUMS,picvec.size()); k++ ){	

		ThreadDoOneImageData* tdoid=new ThreadDoOneImageData(picvec,saveLocation,m_spcount,k,CPU_NUMS);
		HANDLE handle_t=::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)THreadSuperPixel_DoOneImage_win,tdoid,0,NULL); 
		handle.push_back(handle_t);
		data.push_back(tdoid);
	}

	for(int i=0;i<handle.size();i++){
		WaitForSingleObject(handle.at(i),INFINITE);
	}

	

#elif	__GUNC__||linux||__linux||__linux__

	vector<pthread_t> handle;
	
	for(int k = 0; k <std::min((int) CPU_NUMS,(int) picvec.size()); k++ ){	
		
		int i,ret;
		pthread_t handle_t;
		ThreadDoOneImageData* tdoid=new ThreadDoOneImageData(picvec,saveLocation,m_spcount,k,CPU_NUMS);
		ret=pthread_create(&handle_t,NULL,(void *)THreadSuperPixel_DoOneImage_win,tdoid); 
		handle.push_back(handle_t);
		data.push_back(tdoid);
	}

	for(int i=0;i<handle.size();i++){
		pthread_join(handle.at(i),NULL);
	}
#else 
	
	for(int k = 0; k < numPics; k++ ){	
		THreadSuperPixel_DoOneImage(picvec[k],saveLocation,m_spcount);
	}

#endif

	for (int j=0;j<data.size();j++){
		delete data.at(j);
	}	
	TimeCountStop("Threads Done : ");
	return 0;
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
UINT cui_GeneralImgProcess::THreadSuperPixel_CUDA_CollectionMethods_Spiral(LPVOID lpParam,vector<string> picvec,string saveLocation,int m_spcount)
{
//struct pt_sem SEM_CPU_NUMS;
//PT_SEM_INIT(&SEM_CPU_NUMS,get_CPU_core_num()); //��ʼ���ź���Ϊ1����û����
	TimeCountStart();
	vector <double>  Super_Pixel_num_f;
	vector <double>  Do_Time_f;	
	int numPics( picvec.size() );
	/****************************************/
	printf("Start: CUDA_CollectionMethods \n");
	/****************************************/
	int CPU_NUMS=get_CPU_core_num();

	printf("Start: CPU nums %d \n",CPU_NUMS);
	
	vector<ThreadDoOneImageData *> data;


#if _MSC_VER
	/*if (H_SEM_CPU_NUMS==NULL){
		H_SEM_CPU_NUMS = CreateSemaphore(NULL, 1, 10, NULL); 
	}*/	
	//CPU_NUMS+=CPU_NUMS;	
	vector<HANDLE> handle;
	
	for(int k = 0; k <min(CPU_NUMS,picvec.size()); k++ ){	

		ThreadDoOneImageData* tdoid=new ThreadDoOneImageData(picvec,saveLocation,m_spcount,k,CPU_NUMS);
		HANDLE handle_t=::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)THreadSuperPixel_DoOneImage_win_spiral,tdoid,0,NULL); 
		handle.push_back(handle_t);
		data.push_back(tdoid);
	}

	for(int i=0;i<handle.size();i++){
		WaitForSingleObject(handle.at(i),INFINITE);
	}

	

#elif	__GUNC__||linux||__linux||__linux__

	vector<pthread_t> handle;
	
	for(int k = 0; k <std::min((int) CPU_NUMS,(int) picvec.size()); k++ ){	
		
		int i,ret;
		pthread_t handle_t;
		ThreadDoOneImageData* tdoid=new ThreadDoOneImageData(picvec,saveLocation,m_spcount,k,CPU_NUMS);
		ret=pthread_create(&handle_t,NULL,(void *)THreadSuperPixel_DoOneImage_win,tdoid); 
		handle.push_back(handle_t);
		data.push_back(tdoid);
	}

	for(int i=0;i<handle.size();i++){
		pthread_join(handle.at(i),NULL);
	}
#else 
	
	for(int k = 0; k < numPics; k++ ){	
		THreadSuperPixel_DoOneImage(picvec[k],saveLocation,m_spcount);
	}

#endif

	for (int j=0;j<data.size();j++){
		delete data.at(j);
	}	
	TimeCountStop("Threads Done : ");
	return 0;
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
void cui_GeneralImgProcess::_splitpath(const char *path, char *drive, char *dir, char *fname, char *ext)
{
#if _MSC_VER
	::_splitpath(path,drive,dir,fname,ext);
#endif
#if linux||__linux__||__linux||__GNUC__
	char *p_whole_name;

	if (drive!=NULL) drive[0] = '\0';

	
	if (NULL == path)
	{
		if (dir!=NULL) dir[0] = '\0';
		if (fname!=NULL) fname[0] = '\0';
		if (ext!=NULL) ext[0] = '\0';
		return;
	}

	if ('/' == path[strlen(path)])
	{
		if (dir!=NULL) strcpy(dir, path);
		if (fname!=NULL) fname[0] = '\0';
		if (ext!=NULL)  ext[0] = '\0';
		return;
	}

	p_whole_name = rindex(path, '/');
	if (NULL != p_whole_name)
	{
		p_whole_name++;
		_split_whole_name(p_whole_name, fname, ext);

		if (dir!=NULL) snprintf(dir, p_whole_name - path, "%s", path);
	}
	else
	{
		_split_whole_name(path, fname, ext);
		if (dir!=NULL) dir[0] = '\0';
	}
#endif
}

/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/

void cui_GeneralImgProcess::_split_whole_name(const char *whole_name, char *fname, char *ext)
{
#if linux||__linux__||__linux||__GNUC__
	char *p_ext;

	p_ext = rindex(whole_name, '.');
	if (NULL != p_ext)
	{
		if (ext!=NULL) strcpy(ext, p_ext);
		if (fname!=NULL)  snprintf(fname, p_ext - whole_name + 1, "%s", whole_name);
	}
	else
	{
		ext[0] = '\0';
		if (fname!=NULL)  strcpy(fname, whole_name);
	}
#endif
}

/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/