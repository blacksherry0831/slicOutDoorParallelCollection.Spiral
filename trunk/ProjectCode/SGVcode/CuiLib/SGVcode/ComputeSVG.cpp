#include "StdAfx.h"
#include "module_all_cui.h"
//#include "ComputeSVG.h"
//#include "cui_GeneralImgProcess.h"
//#include <algorithm>
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#if  _MSC_VER
double* ComputeSVG:: cui_pYweight_GVS=NULL;
#endif
///////////////////////////////////////////////

#define  WeightZoom (10000)

////////////////////////////////////////////////
/*--------------------------------------------------------------------------------------------------------*/
/**
*构造函数，完成变量初始化
*导入用于计算的中间变量
*/
/*-------------------------------------------------------------------------------------------------------*/
ComputeSVG::ComputeSVG(ImageMemData* MemData_t)
{
	cui_pYweight_GVS=NULL;
    this->InitParam();
    this->pMD=MemData_t;
    storage=cvCreateMemStorage(0);
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*初始化成员变量
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG::InitParam(void)
{
    cui_ImgData=NULL;
     cui_Width=0;
	 cui_Height=0;
	 cui_ImgLables=NULL;
	 cui_ImgLables_SVG=NULL;
	 cui_NumLabels=0; 
	 cui_pYweight_GVS=NULL;
	 cui_G_V_S_Num_Category=NULL;
	 cui_Matrix_Category_Lable=NULL;
	 Category_Force=NULL;
	 const double Confusion_M_t[3][3]={{0.78,0.22,0.00},{0.09,0.89,0.02},{0.00,0.10,0.90}};
	 memcpy(Confusion_M,Confusion_M_t,sizeof(Confusion_M_t));
	 Cui_lab_hsv_Image=NULL;	
	 h_l_plane=NULL;
	 s_a_plane=NULL;
	 v_b_plane=NULL;
	 E_matrix=NULL;
	 Lab_Color_Category=NULL;
	 p_SpProperty=NULL;
	 gray_Color_contour_img=nullptr;
	 lines=nullptr;
	 gray_binary_img=nullptr;
	 gray_img=nullptr;
	 /**/
	 cui_pYweight_G=NULL;
	 cui_pYweight_V=NULL;
	 cui_pYweight_S=NULL;
	 /**/

	
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*构造函数，完成变量初始化（已弃用）
*/
/*-------------------------------------------------------------------------------------------------------*/
ComputeSVG::ComputeSVG(void)
{
	this->InitParam();
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*析构函数 释放内存
*/
/*-------------------------------------------------------------------------------------------------------*/
ComputeSVG::~ComputeSVG(void)
{
	  this->ClearMemory();
	  if (storage!=nullptr){
		  cvReleaseMemStorage(&storage);
	  }
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*清理计算过程中分配的内存
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG::ClearMemory(void)
{
	if(cui_Matrix_Category_Lable!=nullptr){
		delete  []cui_Matrix_Category_Lable;
		cui_Matrix_Category_Lable=NULL;
	}
	if (cui_ImgLables!=nullptr){
		delete []cui_ImgLables;
		cui_ImgLables=NULL;
	}
	if (cui_ImgData!=nullptr){
		delete []cui_ImgData;
		cui_ImgData=NULL;
	}
	if (cui_pYweight_GVS!=nullptr){
		delete []cui_pYweight_GVS;
		cui_pYweight_GVS=NULL;
	}
	if(cui_G_V_S_Num_Category!=nullptr){
		delete []cui_G_V_S_Num_Category;
		cui_G_V_S_Num_Category=NULL;
	}
	if (Category_Force!=nullptr){
		delete []Category_Force;
		Category_Force=NULL;
	}
	if (E_matrix!=nullptr){
		delete[] E_matrix;
		E_matrix=NULL;
	}
	if (cui_ImgLables_SVG!=nullptr){
		delete[] cui_ImgLables_SVG;
		cui_ImgLables_SVG=NULL;
	}
	if (Lab_Color_Category!=nullptr){
		delete[] Lab_Color_Category;
		Lab_Color_Category=NULL;
	}
	if(p_SpProperty!=nullptr){
		delete [] p_SpProperty;
		 p_SpProperty=NULL;
	}
	if (Cui_lab_hsv_Image!=nullptr){
		cvReleaseImage(&Cui_lab_hsv_Image);
	}
	if (h_l_plane!=nullptr){
		cvReleaseImage(&h_l_plane);
	}  
	if (s_a_plane!=nullptr){
		cvReleaseImage(&s_a_plane);
	}  
	if (v_b_plane!=nullptr){
		cvReleaseImage(&v_b_plane);
	}  
	if (gray_Color_contour_img!=nullptr){
		cvReleaseImage(&gray_Color_contour_img);
	}
	
	if (gray_img!=nullptr){
		cvReleaseImage(&gray_img);
	}
	if (gray_binary_img!=nullptr){
		cvReleaseImage(&gray_binary_img);
	}
	/*---------------------*/
	delete[] cui_pYweight_G;
	delete[] cui_pYweight_V;
	delete[] cui_pYweight_S;
	/*---------------------*/
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*导入计算用的图像数据
*
*@param ImgData  图像数据
*@param Width    图像宽度
*@param Height   图像高度
*@param Lables   超像素的图块分类信息
*@param NumLabels 超像素的个数
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG::GetImgData(UINT32* ImgData,int Width,int Height,int*  Lables,int NumLabels)
{
	this->ClearMemory();

	cui_Width=Width;
	cui_Height=Height;
	cui_NumLabels=NumLabels;   
	///////////
	cui_ImgData=new UINT32[Width*Height];
	memcpy(cui_ImgData,ImgData,sizeof(UINT32)*Width*Height);
	cui_ImgLables=new int[Width*Height];	
	memcpy(cui_ImgLables,Lables,sizeof(int)*Width*Height);
	cui_ImgLables_SVG=new int[Width*Height];	
	memcpy(cui_ImgLables_SVG,Lables,sizeof(int)*Width*Height);
    cui_pYweight_GVS=new double[3*Height];
	memset(cui_pYweight_GVS,0,sizeof(double)*Height*3);
	/***********************************************************/
	cui_G_V_S_Num_Category=new double[NumLabels*10];
	memset(cui_G_V_S_Num_Category,0,sizeof(double)*NumLabels*10);
	
	p_SpProperty=new SP_PROPERTY[NumLabels]; 
	memset(p_SpProperty,0,sizeof(SP_PROPERTY)*NumLabels);

	Lab_Color_Category=new LabColorCategory[NumLabels];
	memset(Lab_Color_Category,0,sizeof(LabColorCategory)*NumLabels);
	/***********************************************************/
	cui_Matrix_Category_Lable=new INT32[NumLabels];
	memset(cui_Matrix_Category_Lable,0,sizeof(INT32)*NumLabels);
	Category_Force=new INT32[NumLabels];
	memset(Category_Force,0,sizeof(INT32)*NumLabels);
	E_matrix=new UINT32[cui_Width*cui_Height];
	memset(E_matrix,0,sizeof(UINT32)*cui_Height*cui_Width);
	cui_GeneralImgProcess::CuiSetNighbour_E_matrix(cui_ImgLables,cui_NumLabels,E_matrix,cui_Width,cui_Height,"Compute_matrix.matrix","");
	/****************************************************************/
	IplImage cui_org_image;
	cvInitImageHeader(&cui_org_image,cvSize(cui_Width,cui_Height),8,4);
	cui_org_image.imageData=(char*)cui_ImgData;
	//cvShowImage("cc",&cui_org_image);
	Cui_lab_hsv_Image=cvCreateImage(cvGetSize(&cui_org_image),IPL_DEPTH_8U,3);	
	h_l_plane=cvCreateImage(cvGetSize(&cui_org_image),IPL_DEPTH_8U,1);
	s_a_plane=cvCreateImage(cvGetSize(&cui_org_image),IPL_DEPTH_8U,1);
	v_b_plane=cvCreateImage(cvGetSize(&cui_org_image),IPL_DEPTH_8U,1);	
	cvCvtColor(&cui_org_image,Cui_lab_hsv_Image,CV_BGR2Lab);
	cvCvtPixToPlane(Cui_lab_hsv_Image,h_l_plane,s_a_plane,v_b_plane,NULL);
	//cvShowImage("cc",v_b_plane);
	/***********权重数组初始化**********************************************************************************************/
//#if 0
//	this->FillWeightArrayFM();
//#else
//	this->FillWeightArrayZlm();
//#endif
	cui_pYweight_G=new double[cui_Height];
	cui_pYweight_V=new double[cui_Height];
	cui_pYweight_S=new double[cui_Height];
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*确定一个超像素的天空，立面，地面的分类情况
*
*@param sp 超像素的标号
*@note
*G_E=∑_(i=r_b)^(r_t)〖n_i G(i-H_G)〗                        
*S_E=∑_(i=r_b)^(r_t)〖n_i S(i-H_G)〗                        
*V_E=∑_(i=r_b)^(r_t)〖n_i V(i-H_G)〗                 
*/
/*-------------------------------------------------------------------------------------------------------*/
double ComputeSVG::classifyOneSuperpixel(int sp)
{
	double classify_t=-1,max_w=0;
	int max_wi=0;  //？？？？？？？？？？？？
	for (register int i=0;i<3;i++){
		  if (cui_G_V_S_Num_Category[10*sp+i]>=max_w) {
			  max_w=cui_G_V_S_Num_Category[10*sp+i];
			  max_wi=i;
		  }
		  
	}	  
	 double  sub_min_value=1,sub_value=0;
	for (register int ii=0;ii<3;ii++){
		if (ii==max_wi){ //BUG//////////
			continue;
		}		  
		sub_value=cui_G_V_S_Num_Category[10*sp+max_wi]-cui_G_V_S_Num_Category[10*sp+ii];		 
		if (sub_value<=sub_min_value){
			sub_min_value=sub_value;
		}	  
		
	}
	/*********************************/
	//正值，最大值分类
	//存在0值，比较
	/*********************************/
#if 0
  if (sub_min_value<0.01){
		classify_t=Unknow;	 //unknow
	}else{
		if (max_wi==0){
			//G
			classify_t=Ground;
		}else if (max_wi==1){
			//V
			classify_t=Vertical;
		}else if (max_wi==2){
			//S
			classify_t=Sky;
		}
	}
#endif
	  if (max_wi==0){
		  //G
		  classify_t=Ground;
	  }else if (max_wi==1){
		  //V
		  classify_t=Vertical;
	  }else if (max_wi==2){
		  //S
		  classify_t=Sky;
	  }	
	return classify_t;
}

/*--------------------------------------------------------------------------------------------------------*/
/**
*分割天空，立面，地面（已弃用）
*@deprecated
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG::separateSVG(void)
{ 		
		for(register int hi=0;hi<cui_Height;hi++){
			for (register int j=0;j<cui_Width;j++){	
		
					int sp=cui_ImgLables[hi*cui_Width+j];//行号就是超像素标号
						 //hi..超像素纵坐标
					  cui_G_V_S_Num_Category[10*sp+0]+=cui_pYweight_GVS[hi*3+0];	//G
					  cui_G_V_S_Num_Category[10*sp+1]+=cui_pYweight_GVS[hi*3+1];	//V
					  cui_G_V_S_Num_Category[10*sp+2]+=cui_pYweight_GVS[hi*3+2];	//S
					  cui_G_V_S_Num_Category[10*sp+3]+=1; //NUM
					  cui_G_V_S_Num_Category[10*sp+4]=0;//类别

				}
			  
			}
		/****************************************************/
		{
			char data_t[1024];
			ofstream outfile;
			outfile.open("Category1Matrix_G_V_S_Num_.data",ios::out);
			for( int i = 0; i <cui_NumLabels; i++ ){
				for( int j = 0; j <5; j++ ){
					double value_t= cui_G_V_S_Num_Category[i*10+j];
					sprintf_s(data_t,1024," %10.2e ",value_t);
					outfile<<data_t;
				}
				outfile<<endl;
			} 
			outfile.close();
		}
		/*****************************************************/
		//计算权重
		for (register int sp=0;sp<cui_NumLabels;sp++){
			for (register int wj=0;wj<3;wj++){
				  cui_G_V_S_Num_Category[10*sp+wj]= cui_G_V_S_Num_Category[10*sp+wj]/cui_G_V_S_Num_Category[10*sp+3];
			}
		}
		//分类
		for (register int sp=0;sp<cui_NumLabels;sp++){
			cui_G_V_S_Num_Category[10*sp+4]=cui_Matrix_Category_Lable[sp]=classifyOneSuperpixel(sp);
		}
		/*****************************************************/
		{
			char data_t[1024];
			ofstream outfile;
			outfile.open("Category2Matrix_G_V_S_Num_.data",ios::out);
			for(register int i = 0; i <cui_NumLabels; i++ ){
				for(register int j = 0; j <5; j++ ){
					double value_t= cui_G_V_S_Num_Category[i*10+j];
					sprintf_s(data_t,1024," %0.3f ",value_t);
					outfile<<data_t;
				}
				outfile<<endl;
			} 
			outfile.close();
		}
		/*****************************************************/
	   cui_GeneralImgProcess::CuiSaveImgWithPoints(cui_ImgData,cui_ImgLables,cui_Matrix_Category_Lable,cui_Width,cui_Height,true,pMD,"");
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*填充图像在高度方向cui_Height上的数组（模糊矩阵的方式）
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG::FillWeightArrayFM(void){				
	for (register int i=0;i<cui_Height;i++){
		if (i<=cui_Height/2){
			for (register int j=0;j<3;j++){	
				cui_pYweight_GVS[i*3+j]=i*(Confusion_M[j][1]-Confusion_M[j][2])/(cui_Height/2.0)+Confusion_M[j][2];
			}
		}else{
			for (register int j=0;j<3;j++){	
				cui_pYweight_GVS[i*3+j]=(i-cui_Height/2)*(Confusion_M[j][0]-Confusion_M[j][1])/(cui_Height/2.0)+Confusion_M[j][1];
			}


		}

	}
	/************************************************************************************************************************/
	{
		char data_t[1024];																			
		ofstream outfile;								   
		outfile.open("Matrix_Weight_GVS.data",ios::out);
		for( int i = 0; i <cui_Height; i++ ){
			for( int j = 0; j <3; j++ ){
				double value_t=cui_pYweight_GVS[i*3+j];
				sprintf_s(data_t,1024," %0.2e ",value_t);
				outfile<<data_t;										
			}
			outfile<<endl;			 
		} 
		outfile.close();
	}
	/************************************************************************************************************************/
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*填充图像在高度方向cui_Height上的数组（三个函数）
*模糊函数分类时间: 1.910214（秒）
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG::FillWeightArrayZlm(double horizontal_line_scale){
#if _MSC_VER
    LARGE_INTEGER litmp;
	LONGLONG QPart1,QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// 获得初始值	
#endif
 /*---------------------------------------------------------------------------*/
	double horizontal_line=this->cui_Height*horizontal_line_scale;
	this->cui_HorizontalLine=horizontal_line;
#if IN_DOOR&&1
	this->FillWeightArrayG_InDoor(horizontal_line,1);
	this->FillWeightArrayS_InDoor(horizontal_line,1);
#endif
	this->FillWeightArrayV(horizontal_line,1);
#if OUT_DOOR||0
	this->FillWeightArrayG(horizontal_line,1);
	this->FillWeightArrayS(horizontal_line,1);
#endif	
	/************************************************************************************************************************/
	{
		char data_t[1024];																			
		ofstream outfile;								   
		outfile.open("Matrix_Weight_GVS_zlm.data",ios::out);
		for( int i = 0; i <cui_Height; i++ ){
			for( int j = 0; j <3; j++ ){
				double value_t=cui_pYweight_GVS[i*3+j];
				sprintf_s(data_t,1024," %0.2e ",value_t);
				outfile<<data_t;										
			}
			outfile<<endl;			 
		} 
		outfile.close();
	}
	/************************************************************************************************************************/
	memset(Lab_Color_Category,0,sizeof(LabColorCategory)*cui_NumLabels);
#if 1
	for(register int hi=0;hi<cui_Height;hi++){
		for (register int j=0;j<cui_Width;j++){	

			int sp=cui_ImgLables[hi*cui_Width+j];//行号就是超像素标号
			//hi..超像素纵坐标
			cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+0]+=cui_pYweight_GVS[hi*3+0];	//G
			cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+1]+=cui_pYweight_GVS[hi*3+1];	//V
			cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+2]+=cui_pYweight_GVS[hi*3+2];	//S
			cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+3]+=1; //NUM
			cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+4]=0;//类别
			
			Lab_Color_Category[sp].L+=cvGetReal2D(h_l_plane,hi,j);

			cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+A_COLOR]+=cvGetReal2D(s_a_plane,hi,j);
			cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+B_COLOR]+=cvGetReal2D(v_b_plane,hi,j);
			
			if (hi==0){
				cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+5]=Category_Force[sp]|=SKY_Force;//类别
			}else if (hi==cui_Height-1){
				cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+5]=Category_Force[sp]|=GROUND_Force;
			}if (hi==horizontal_line){
				//cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+5]=Category_Force[sp]|=VERTICAL_Force;
			}

		}

	}
#else
	//double *cui_G_V_S_Num_Category_t=new double[cui_NumLabels*G_V_S_Num_Category];
	//memset(cui_G_V_S_Num_Category_t,0,sizeof(double)*cui_NumLabels*G_V_S_Num_Category);
	//memcpy(cui_G_V_S_Num_Category_t,cui_G_V_S_Num_Category,sizeof(double)*cui_NumLabels*G_V_S_Num_Category);
	//for(register int i=0;i<cui_Width;i++){
	//	for (register int hj=0;hj<cui_Height;hj++){	

	//		int sp=cui_ImgLables[hj*cui_Width+i];//行号就是超像素标号
	//		//hi..超像素纵坐标
	//		cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+0]+=cui_pYweight_GVS[hi*3+0];	//G
	//		cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+1]+=cui_pYweight_GVS[hi*3+1];	//V
	//		cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+2]+=cui_pYweight_GVS[hi*3+2];	//S
	//		cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+3]+=1; //NUM
	//		cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+4]=0;//类别
	//		cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+A_COLOR]+=cvGetReal2D(s_a_plane,hj,i);
	//		cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+B_COLOR]+=cvGetReal2D(s_b_plane,hj,i);
	//		double debug_t=cvGetReal2D(s_a_plane,hj,i);
	//		if (hi==0){
	//			cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+5]|=SKY_Force;//类别
	//		}else if (hi==cui_Height-1){
	//			cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+5]|=GROUND_Force;
	//		}else if (hi==(int)horizontal_line){
	//			cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+5]|=VERTICAL_Force;
	//		}

	//	}

	//}
#endif	
#if 1
	{
		char data_t[1024];
		ofstream outfile;
		outfile.open("Category2Matrix_L_A_B.data",ios::out);
		for(register int i = 0; i <cui_NumLabels; i++ ){
			{
				double value_t= Lab_Color_Category[i].L;
				sprintf_s(data_t,1024," %0.3f ",value_t);
				outfile<<data_t;
				value_t= Lab_Color_Category[i].L_Dst2Sky;
				sprintf_s(data_t,1024," %0.3f ",value_t);
				outfile<<data_t;
			}
			outfile<<endl;
		} 
		outfile.close();
	}	/****************************************************/
	{
		char data_t[1024];
		ofstream outfile;
		outfile.open("Category1Matrix_G_V_S_Num_zlm.data",ios::out);
		for( int i = 0; i <cui_NumLabels; i++ ){
			for( int j = 0; j <10; j++ ){
				double value_t= cui_G_V_S_Num_Category[i*10+j];
				sprintf_s(data_t,1024," %10.2e ",value_t);
				outfile<<data_t;
			}
			outfile<<endl;
		} 
		outfile.close();
	}
#endif

	/************************
	cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+A_COLOR]=cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+A_COLOR]sum;
	cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+B_COLOR]=cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+A_COLOR]sum;
	*****************************/
	////计算权重
	//for (register int sp=0;sp<cui_NumLabels;sp++){
	//	for (register int wj=0;wj<3;wj++){
	//		cui_G_V_S_Num_Category[10*sp+wj]= cui_G_V_S_Num_Category[10*sp+wj]/cui_G_V_S_Num_Category[10*sp+3];
	//	}
	//}
	//归一化

	for (register int sp=0;sp<cui_NumLabels;sp++){
		for (register int wj=0;wj<3;wj++){
			if (cui_G_V_S_Num_Category[10*sp+wj]<=0){
				cui_G_V_S_Num_Category[10*sp+wj]=0;
			}

		}
		double sum=cui_G_V_S_Num_Category[10*sp+0]+cui_G_V_S_Num_Category[10*sp+1]+cui_G_V_S_Num_Category[10*sp+2];
		cui_G_V_S_Num_Category[10*sp+0]=cui_G_V_S_Num_Category[10*sp+0]/sum;
		cui_G_V_S_Num_Category[10*sp+1]=cui_G_V_S_Num_Category[10*sp+1]/sum;
		cui_G_V_S_Num_Category[10*sp+2]=cui_G_V_S_Num_Category[10*sp+2]/sum;
		/**********************根据色系分类************************************************/
		Lab_Color_Category[sp].L=Lab_Color_Category[sp].L/cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+SquareMeter];
		cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+A_COLOR]=cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+A_COLOR]/cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+SquareMeter]-127;
		cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+B_COLOR]=cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+B_COLOR]/cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+SquareMeter]-127;
	    cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+Lab_2_0_Dst]=sqrtl(pow(cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+A_COLOR],2)+pow(cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+B_COLOR],2));	
		/**********************************************************************/
	}

	/*****************************************************/
	for (register int sp=0;sp<cui_NumLabels;sp++){
		cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+4]=cui_Matrix_Category_Lable[sp]=classifyOneSuperpixel(sp);
	}
	//模糊聚类的结果
	cui_GeneralImgProcess::CuiSaveImgWithPoints(cui_ImgData,cui_ImgLables,cui_Matrix_Category_Lable,
		cui_Width,cui_Height,true,
		pMD,"");
	/*****************************************************/
#if _MSC_VER
/*---------------------------------------------------------------------------*/
	QueryPerformanceCounter(&litmp);
	QPart2 = litmp.QuadPart;//获得中止值
	dfMinus = (double)(QPart2-QPart1);
	dfTim = dfMinus / dfFreq;// 获得对应的时间值，单位为秒
	TRACE("\n 模糊函数分类时间: %f（秒）",dfTim);
	//模糊函数分类时间: 1.910214（秒）
#endif

}
/*--------------------------------------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG::FillWeightArrayZlmParallel(double horizontal_line_scale)
{
#if _MSC_VER
	LARGE_INTEGER litmp;
	LONGLONG QPart1,QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// 获得初始值	
#endif

	/*---------------------------------------------------------------------------*/
	double horizontal_line=this->cui_Height*horizontal_line_scale;
	this->cui_HorizontalLine=horizontal_line;	
	{	
#if 0
		classify_SkyVerticalGround_cuda(
			horizontal_line,
			cui_Matrix_Category_Lable,
			cui_pYweight_S, 
			cui_pYweight_V,		 
			cui_pYweight_G,
			cui_ImgLables,
			cui_NumLabels,
			cui_Width,
			cui_Height);
#endif
		/*****************************************************/
		//模糊聚类的结果
		cui_GeneralImgProcess::CuiSaveImgWithPoints(cui_ImgData,cui_ImgLables,cui_Matrix_Category_Lable,
			cui_Width,cui_Height,true,
			pMD,"");
		/*****************************************************/
	}
#if 1
 	memset(Lab_Color_Category,0,sizeof(LabColorCategory)*cui_NumLabels);
	for(register int hi=0;hi<cui_Height;hi++){
		for (register int j=0;j<cui_Width;j++){	

			int sp=cui_ImgLables[hi*cui_Width+j];//行号就是超像素标号
			//hi..超像素纵坐标

			cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+0]+=cui_pYweight_G[hi];	//G
			cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+1]+=cui_pYweight_V[hi];	//V
			cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+2]+=cui_pYweight_S[hi];	//S


			cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+3]+=1; //NUM
			cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+4]=0;//类别
			
			Lab_Color_Category[sp].L+=cvGetReal2D(h_l_plane,hi,j);

			cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+A_COLOR]+=cvGetReal2D(s_a_plane,hi,j);
			cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+B_COLOR]+=cvGetReal2D(v_b_plane,hi,j);
			
			if (hi==0){
				cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+5]=Category_Force[sp]|=SKY_Force;//类别
			}else if (hi==cui_Height-1){
				cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+5]=Category_Force[sp]|=GROUND_Force;
			}if (hi==horizontal_line){
				//cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+5]=Category_Force[sp]|=VERTICAL_Force;
			}

		}

	}

	for (register int sp=0;sp<cui_NumLabels;sp++){
		for (register int wj=0;wj<3;wj++){
			if (cui_G_V_S_Num_Category[10*sp+wj]<=0){
				cui_G_V_S_Num_Category[10*sp+wj]=0;
			}

		}
		double sum=cui_G_V_S_Num_Category[10*sp+0]+cui_G_V_S_Num_Category[10*sp+1]+cui_G_V_S_Num_Category[10*sp+2];
		cui_G_V_S_Num_Category[10*sp+0]=cui_G_V_S_Num_Category[10*sp+0]/sum;
		cui_G_V_S_Num_Category[10*sp+1]=cui_G_V_S_Num_Category[10*sp+1]/sum;
		cui_G_V_S_Num_Category[10*sp+2]=cui_G_V_S_Num_Category[10*sp+2]/sum;
		/**********************根据色系分类************************************************/
		Lab_Color_Category[sp].L=Lab_Color_Category[sp].L/cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+SquareMeter];
		cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+A_COLOR]=cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+A_COLOR]/cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+SquareMeter]-127;
		cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+B_COLOR]=cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+B_COLOR]/cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+SquareMeter]-127;
	    cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+Lab_2_0_Dst]=sqrtl(pow(cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+A_COLOR],2)+pow(cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+B_COLOR],2));	
		/**********************************************************************/
	}
	/*****************************************************/
	for (register int sp=0;sp<cui_NumLabels;sp++){
		cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+4]=cui_Matrix_Category_Lable[sp];
	}
#endif
#if _MSC_VER
    /*---------------------------------------------------------------------------*/
	QueryPerformanceCounter(&litmp);
	QPart2 = litmp.QuadPart;//获得中止值
	dfMinus = (double)(QPart2-QPart1);
	dfTim = dfMinus / dfFreq;// 获得对应的时间值，单位 为秒
	TRACE("\n 模糊函数分类时间: %f（秒）",dfTim);
	//模糊函数分类时间: 2.077450 2.183845 1.978618（秒）
#endif
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*提取所有基于模糊分布密度函数算法判定的天空、立面物体的图块进行1层小波变换\n
*计算天空、立面（除地面）的小波能量\n
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG::CalculateSkyVHarrEnergy(void)
{
#if 0
memset(harr_sp_energy,0,sizeof(harr_sp_energy)); 
	cui_GeneralImgProcess::CalculateAllSpPropertyRange(cui_ImgLables,cui_Width,cui_Height,p_SpProperty,cui_NumLabels);
	/******确定候选天空*******************************************************************/
	for (register int sp=cui_NumLabels-1;sp>=0;sp--){
		if ((cui_G_V_S_Num_Category[10*sp+4]!=Ground)){
			           INT32 debug_t=Category_Force[sp];
					if (Category_Force[sp]&SKY_Force){
						//以下为天空候选 
#if USESTATIC
						CalculateBlockEnergy2_S(
							sp,
							harr_sp_energy,
							p_SpProperty,
							cui_ImgLables,
							cui_ImgData,
							cui_Width,
							cui_Height);
#else
						CalculateBlockEnergy2(sp);
#endif
						 harr_sp_energy[sp].Sky_Candidate=Sky;
						/* if (sp==18){
							 double debug_t=cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+Lab_2_P_Dst];
							 double debug_t1=cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+Lab_2_0_Dst];
						 }*/
					}else{
#if USESTATIC
						CalculateBlockEnergy2_S(
							sp,
							harr_sp_energy,
							p_SpProperty,
							cui_ImgLables,
							cui_ImgData,
							cui_Width,
							cui_Height);
#else
						CalculateBlockEnergy2(sp);
#endif
						
						  harr_sp_energy[sp].Sky_Candidate=0;
						  Category_Force[sp]=Category_Force[sp]&(~SKY_Force);
					}
		}
	}
	/*************************************************************************************/	
#endif
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG::CalculateSkyVHarrEnergy_S(
	int cui_NumLabels,
	Energy_Harr* harr_sp_energy,
	SP_PROPERTY *p_SpProperty,
	int*  cui_ImgLables,
	UINT32* cui_ImgData,
	int cui_Width,
	int cui_Height,
	double* cui_G_V_S_Num_Category,
	INT32*  Category_Force)
{
#if 0
memset(harr_sp_energy,0,sizeof(harr_sp_energy)); 
	cui_GeneralImgProcess::CalculateAllSpPropertyRange(cui_ImgLables,cui_Width,cui_Height,p_SpProperty,cui_NumLabels);
	/******确定候选天空*******************************************************************/
	for (register int sp=cui_NumLabels-1;sp>=0;sp--){
		if ((cui_G_V_S_Num_Category[10*sp+4]!=Ground)){
			           INT32 debug_t=Category_Force[sp];
					if (Category_Force[sp]&SKY_Force){
						//以下为天空候选 
						CalculateBlockEnergy2_S(
							sp,
							harr_sp_energy,
							p_SpProperty,
							cui_ImgLables,
							cui_ImgData,
							cui_Width,
							cui_Height);
						 harr_sp_energy[sp].Sky_Candidate=Sky;
						/* if (sp==18){
							 double debug_t=cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+Lab_2_P_Dst];
							 double debug_t1=cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+Lab_2_0_Dst];
						 }*/
					}else{
						CalculateBlockEnergy2_S(
							sp,
							harr_sp_energy,
							p_SpProperty,
							cui_ImgLables,
							cui_ImgData,
							cui_Width,
							cui_Height);
						  harr_sp_energy[sp].Sky_Candidate=0;
						  Category_Force[sp]=Category_Force[sp]&(~SKY_Force);
					}
		}
	}
	/*************************************************************************************/	
#endif
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*天空聚类，计算出最有效的属于天空的超像素图块
*
*@param  sky_Light_Threshold 天空超像素的亮度阈值
*@param  sky_energy   天空超像素的能量阈值
*@return 最有效的天空图块
*/
/*-------------------------------------------------------------------------------------------------------*/
int ComputeSVG::CalculateMostEffectiveSky(double sky_Light_Threshold,double sky_energy)
{
ULONGLONG sky_Effect_Square_Index=0, sky_Effect_vein_Index=0,sky_SquareMeter=0;
#if 0
	float sky_Vein=255;
	/**************************************************************************************/

	for (register int sp=cui_NumLabels-1;sp>=0;sp--){
		if ((cui_G_V_S_Num_Category[10*sp+4]!=Ground)){
			/***************************/
			if (harr_sp_energy[sp].Sky_Candidate==Sky){
				//根据能量重新确定候选关系
				if ((harr_sp_energy[sp].EnergyHighsigma<=Sky_Energy_DOWN)){
						if ((harr_sp_energy[sp].EnergyL_LL-sky_Light_Threshold>=-1*sky_Light_Threshold/4)){
							cui_G_V_S_Num_Category[10*sp+4]=cui_Matrix_Category_Lable[sp]=harr_sp_energy[sp].Category_Sky=Sky;
							harr_sp_energy[sp].Sky_Candidate=Sky;
						}else{
							cui_G_V_S_Num_Category[10*sp+4]=cui_Matrix_Category_Lable[sp]=harr_sp_energy[sp].Category_Sky=Vertical;
							harr_sp_energy[sp].Sky_Candidate=0; 
							Category_Force[sp]=Category_Force[sp]&(~SKY_Force);
						}			

				}else {
					
					if ((harr_sp_energy[sp].EnergyL_LL>sky_Light_Threshold)&&
						(harr_sp_energy[sp].EnergyHighsigma<sky_energy)){

							cui_G_V_S_Num_Category[10*sp+4]=cui_Matrix_Category_Lable[sp]=harr_sp_energy[sp].Category_Sky=Sky;
							harr_sp_energy[sp].Sky_Candidate=Sky;
					}else{
						cui_G_V_S_Num_Category[10*sp+4]=cui_Matrix_Category_Lable[sp]=harr_sp_energy[sp].Category_Sky=Vertical;
						harr_sp_energy[sp].Sky_Candidate=0; 
						Category_Force[sp]=Category_Force[sp]&(~SKY_Force);
					}

				}		

			}
			/**********面积最大*****************/
			if ((harr_sp_energy[sp].Sky_Candidate==Sky)&&
				(cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+SquareMeter]>sky_SquareMeter)){
					sky_SquareMeter=cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+SquareMeter];
					sky_Effect_Square_Index=sp;
			}	
			/**********纹理最小************************/
			if ((harr_sp_energy[sp].Sky_Candidate==Sky)&&
				 harr_sp_energy[sp].EnergyHighsigma<sky_Vein)
			{
			   sky_Vein=harr_sp_energy[sp].EnergyHighsigma;
			   sky_Effect_vein_Index=sp;
			}
		}
	}
	/***********最大面积，第一候选天空***************************************************************************/
	/***********去掉不合适的候选*********************************************************************************/
#endif
		
	
#if 0
	 return sky_Effect_vein_Index;
#else
	 return sky_Effect_Square_Index;
#endif
	
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*                                                                      
*
**/
/*--------------------------------------------------------------------------------------------------------*/
int ComputeSVG::CalculateMostEffectiveSky_S(
	double sky_Light_Threshold,
	double sky_energy,
	int cui_NumLabels,
	double* cui_G_V_S_Num_Category,
	Energy_Harr* harr_sp_energy,
	INT32 * cui_Matrix_Category_Lable,
	INT32*  Category_Force)
{
	ULONGLONG sky_Effect_Square_Index=0, sky_Effect_vein_Index=0,sky_SquareMeter=0;
#if 0

	float sky_Vein=255;
	/**************************************************************************************/

	for (register int sp=cui_NumLabels-1;sp>=0;sp--){
		if ((cui_G_V_S_Num_Category[10*sp+4]!=Ground)){
			/***************************/
			if (harr_sp_energy[sp].Sky_Candidate==Sky){
				//根据能量重新确定候选关系
				if ((harr_sp_energy[sp].EnergyHighsigma<=Sky_Energy_DOWN)){
					if ((harr_sp_energy[sp].EnergyL_LL-sky_Light_Threshold>=-1*sky_Light_Threshold/4)){
						cui_G_V_S_Num_Category[10*sp+4]=cui_Matrix_Category_Lable[sp]=harr_sp_energy[sp].Category_Sky=Sky;
						harr_sp_energy[sp].Sky_Candidate=Sky;
					}else{
						cui_G_V_S_Num_Category[10*sp+4]=cui_Matrix_Category_Lable[sp]=harr_sp_energy[sp].Category_Sky=Vertical;
						harr_sp_energy[sp].Sky_Candidate=0; 
						Category_Force[sp]=Category_Force[sp]&(~SKY_Force);
					}			

				}else {

					if ((harr_sp_energy[sp].EnergyL_LL>sky_Light_Threshold)&&
						(harr_sp_energy[sp].EnergyHighsigma<sky_energy)){

							cui_G_V_S_Num_Category[10*sp+4]=cui_Matrix_Category_Lable[sp]=harr_sp_energy[sp].Category_Sky=Sky;
							harr_sp_energy[sp].Sky_Candidate=Sky;
					}else{
						cui_G_V_S_Num_Category[10*sp+4]=cui_Matrix_Category_Lable[sp]=harr_sp_energy[sp].Category_Sky=Vertical;
						harr_sp_energy[sp].Sky_Candidate=0; 
						Category_Force[sp]=Category_Force[sp]&(~SKY_Force);
					}

				}		

			}
			/**********面积最大*****************/
			if ((harr_sp_energy[sp].Sky_Candidate==Sky)&&
				(cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+SquareMeter]>sky_SquareMeter)){
					sky_SquareMeter=cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+SquareMeter];
					sky_Effect_Square_Index=sp;
			}	
			/**********纹理最小************************/
			if ((harr_sp_energy[sp].Sky_Candidate==Sky)&&
				harr_sp_energy[sp].EnergyHighsigma<sky_Vein)
			{
				sky_Vein=harr_sp_energy[sp].EnergyHighsigma;
				sky_Effect_vein_Index=sp;
			}
		}
	}
	/***********最大面积，第一候选天空***************************************************************************/
	/***********去掉不合适的候选*********************************************************************************/
#endif
	
#if 0
	return sky_Effect_vein_Index;
#else
	return sky_Effect_Square_Index;
#endif

}
/*--------------------------------------------------------------------------------------------------------*/
/**
*聚类天空，分离天空和立面
*@param sky_energy			天空超像素的能量阈值
*@param sky_Light_Threshold 天空超像素的亮度阈值
*@param L_distance			LAB分量中L分量的距离
*@param AB_distance			LAB分量中AB分量的距离
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG::DivideSkyV(
	double sky_energy,
	double sky_Light_Threshold,
	double L_distance, 
	double AB_distance)
{	
#if 0
cui_GeneralImgProcess::CalculateAllSpPropertyRange(cui_ImgLables,pMD->ImgWidth,pMD->ImgHeight,pMD->p_SpProperty,cui_NumLabels);
	cui_GeneralImgProcess::CalculateAllSpBlockEnergy(cui_NumLabels,pMD->p_SpProperty,pMD->Src_ImgData,cui_ImgLables,pMD->ImgWidth,pMD->ImgHeight);
	/*****************************************************/
	for (register int spi=0;spi<cui_NumLabels;spi++){		
			if (harr_sp_energy[spi].Sky_Candidate==Sky){
				//天空候选
				  for (register int spj=0;spj<cui_NumLabels;spj++){
				  	  if (E_matrix[spi*cui_NumLabels+spj]!=0){
						  //与天空候选相邻
						  if (cui_G_V_S_Num_Category[10*spj+4]!=Ground){							
										  //同时为天空
									  if ( this->Is_Sky_Block(spj,sky_Light_Threshold,sky_energy,AB_distance,L_distance)
										    &&(cui_GeneralImgProcess::CalculateSpSimilar(spi,spj,pMD->p_SpProperty)>0.9)){ 	
												  cui_G_V_S_Num_Category[10*spj+4]=cui_Matrix_Category_Lable[spj]=harr_sp_energy[spj].Category_Sky=Sky;
												  harr_sp_energy[spj].Sky_Candidate=Sky;
									
									    }else{
													cui_G_V_S_Num_Category[10*spj+4]=cui_Matrix_Category_Lable[spj]=harr_sp_energy[spj].Category_Sky=Vertical;
												    harr_sp_energy[spj].Sky_Candidate=0;
										 }						
								  }
				  	  }else{
					  //不相邻。
				      }

				  }
			}
	}
	/**************************************************************************************/
	//对所有非候选都设置为0
	for (register int sp=0;sp<cui_NumLabels;sp++){
		if ((cui_G_V_S_Num_Category[10*sp+4]!=Ground)){
			if ((harr_sp_energy[sp].EnergyL_LL>0)&&
				 (harr_sp_energy[sp].Sky_Candidate==Sky)){
			    
				/* if (sp==18){
					 double debug_t=cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+Lab_2_P_Dst];
					 double debug_t1=cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+Lab_2_0_Dst];
				 } */
				 cui_G_V_S_Num_Category[10*sp+4]=cui_Matrix_Category_Lable[sp]=harr_sp_energy[sp].Category_Sky=Sky;
			}else{
			/*	if (sp==18){
					double debug_t=cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+Lab_2_P_Dst];
					double debug_t1=cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+Lab_2_0_Dst];
				}*/
				 cui_G_V_S_Num_Category[10*sp+4]=cui_Matrix_Category_Lable[sp]=harr_sp_energy[sp].Category_Sky=Vertical;
			}
		}
	}
#endif
	

}
/*--------------------------------------------------------------------------------------------------------*/
/**
*聚类天空，分离天空和立面
*@param sky_energy			天空超像素的能量阈值
*@param sky_Light_Threshold 天空超像素的亮度阈值
*@param L_distance			LAB分量中L分量的距离
*@param AB_distance			LAB分量中AB分量的距离
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG::DivideSkyV_S(
	double sky_energy,
	double sky_Light_Threshold,
	double L_distance, 
	double AB_distance,
	int* cui_ImgLables,
	int cui_NumLabels,
	ImageMemData* pMD,
	Energy_Harr* harr_sp_energy,
	UINT32* E_matrix,
	double* cui_G_V_S_Num_Category)
{	
#if 0
cui_GeneralImgProcess::CalculateAllSpPropertyRange(cui_ImgLables,pMD->ImgWidth,pMD->ImgHeight,pMD->p_SpProperty,cui_NumLabels);
	cui_GeneralImgProcess::CalculateAllSpBlockEnergy(cui_NumLabels,pMD->p_SpProperty,pMD->Src_ImgData,cui_ImgLables,pMD->ImgWidth,pMD->ImgHeight);
	/*****************************************************/
	for (register int spi=0;spi<cui_NumLabels;spi++){		
			if (harr_sp_energy[spi].Sky_Candidate==Sky){
				//天空候选
				  for (register int spj=0;spj<cui_NumLabels;spj++){
				  	  if (E_matrix[spi*cui_NumLabels+spj]!=0){
						  //与天空候选相邻
						  if (cui_G_V_S_Num_Category[10*spj+4]!=Ground){							
										  //同时为天空
									  if ( this->Is_Sky_Block(spj,sky_Light_Threshold,sky_energy,AB_distance,L_distance)
										    &&(cui_GeneralImgProcess::CalculateSpSimilar(spi,spj,pMD->p_SpProperty)>0.9)){ 	
												  cui_G_V_S_Num_Category[10*spj+4]=cui_Matrix_Category_Lable[spj]=harr_sp_energy[spj].Category_Sky=Sky;
												  harr_sp_energy[spj].Sky_Candidate=Sky;
									
									    }else{
													cui_G_V_S_Num_Category[10*spj+4]=cui_Matrix_Category_Lable[spj]=harr_sp_energy[spj].Category_Sky=Vertical;
												    harr_sp_energy[spj].Sky_Candidate=0;
										 }						
								  }
				  	  }else{
					  //不相邻。
				      }

				  }
			}
	}
	/**************************************************************************************/
	//对所有非候选都设置为0
	for (register int sp=0;sp<cui_NumLabels;sp++){
		if ((cui_G_V_S_Num_Category[10*sp+4]!=Ground)){
			if ((harr_sp_energy[sp].EnergyL_LL>0)&&
				 (harr_sp_energy[sp].Sky_Candidate==Sky)){
			    
				/* if (sp==18){
					 double debug_t=cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+Lab_2_P_Dst];
					 double debug_t1=cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+Lab_2_0_Dst];
				 } */
				 cui_G_V_S_Num_Category[10*sp+4]=cui_Matrix_Category_Lable[sp]=harr_sp_energy[sp].Category_Sky=Sky;
			}else{
			/*	if (sp==18){
					double debug_t=cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+Lab_2_P_Dst];
					double debug_t1=cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+Lab_2_0_Dst];
				}*/
				 cui_G_V_S_Num_Category[10*sp+4]=cui_Matrix_Category_Lable[sp]=harr_sp_energy[sp].Category_Sky=Vertical;
			}
		}
	}

#endif
	
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*与最似天空比较，移除不合理的天空图块
*@param sky_Effect_Index  最似天空的索引值
*@param L_distance	LAB分量中L分量的距离
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG::RemoveUnEffectiveSkySP(ULONGLONG sky_Effect_Index,float L_distance)
{
#if 0
//除去与候选天空差异较大的天空候选块
	for (register int sp=0;sp<cui_NumLabels;sp++){
		if ((cui_G_V_S_Num_Category[10*sp+4]!=Ground)){
			 if ((harr_sp_energy[sp].Sky_Candidate==Sky)){
			   //天空候选
				if (((Lab_Color_Category[sp].L_Dst2Sky>L_distance))){
					////////////////////////////////////////////////////
					cui_G_V_S_Num_Category[10*sp+4]=cui_Matrix_Category_Lable[sp]=harr_sp_energy[sp].Category_Sky=Vertical;
					harr_sp_energy[sp].Sky_Candidate=0; 
					Category_Force[sp]=Category_Force[sp]&(~SKY_Force);
					////////////////////////////////////////////////////
				}
			 }		
		} 		
	}		
#endif
	
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*与最似天空比较，移除不合理的天空图块
*@param sky_Effect_Index  最似天空的索引值
*@param L_distance	LAB分量中L分量的距离
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG::RemoveUnEffectiveSkySP_S(
	ULONGLONG sky_Effect_Index,
	float L_distance,
	int cui_NumLabels,
	double* cui_G_V_S_Num_Category,
	Energy_Harr* harr_sp_energy,
	LabColorCategory* Lab_Color_Category,
	INT32 * cui_Matrix_Category_Lable,
	INT32*  Category_Force)
{
#if 0
	//除去与候选天空差异较大的天空候选块
	for (register int sp=0;sp<cui_NumLabels;sp++){
		if ((cui_G_V_S_Num_Category[10*sp+4]!=Ground)){
			if ((harr_sp_energy[sp].Sky_Candidate==Sky)){
				//天空候选
				if (((Lab_Color_Category[sp].L_Dst2Sky>L_distance))){
					////////////////////////////////////////////////////
					cui_G_V_S_Num_Category[10*sp+4]=cui_Matrix_Category_Lable[sp]=harr_sp_energy[sp].Category_Sky=Vertical;
					harr_sp_energy[sp].Sky_Candidate=0; 
					Category_Force[sp]=Category_Force[sp]&(~SKY_Force);
					////////////////////////////////////////////////////
				}
			}		
		} 		
	}	
#endif
	
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*完成对天空，立面、地面分离
*本类中最重要的函数
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG::separateSVG_Zlm(void)
{
	//填充密度函数
#if UES_CUDA
this->FillWeightArrayZlmParallel(pMD->Seg_HorizontalLinePosScale);
#else
this->FillWeightArrayZlm(pMD->Seg_HorizontalLinePosScale);
#endif	
	zlm_ForceSkyGroundVertical();
	//第一次分类的结果 
	cui_GeneralImgProcess::CuiSaveImgWithPoints(
		cui_ImgData,
		cui_ImgLables,
		cui_Matrix_Category_Lable,
		cui_Width,
		cui_Height,
		true,
		pMD,
		"");
	/*****************************************************/
	{
		char data_t[1024];
		ofstream outfile;
		outfile.open("Category2Matrix_G_V_S_Num_zlm.data",ios::out);
		for(register int i = 0; i <cui_NumLabels; i++ ){
			for(register int j = 0; j <10; j++ ){
				double value_t= cui_G_V_S_Num_Category[i*10+j];
				sprintf_s(data_t,1024," %0.3f ",value_t);
				outfile<<data_t;
			}
			outfile<<endl;
		} 
		outfile.close();
	}
	/*****************************************************/
	//聚类设置天空聚类轮廓
	cui_GeneralImgProcess::Cui_CombinationImgSVG(cui_ImgData,cui_ImgLables,
		cui_ImgLables_SVG,
		cui_Matrix_Category_Lable,
		cui_Width,cui_Height,"","","");

}
/*--------------------------------------------------------------------------------------------------------*/
/**
*利用模糊函数的结果分离天空、立面、地面
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG::zlm_ForceSkyGroundVertical(void)
{
 	    this->zlm_ForceSky();
		this->RemoveSomeUnreasonableClassification();		
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*利用模糊函数的结果分离天空、立面、地面
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG::zlm_ForceSkyGroundVertical_InDoor(void)
{
	this->InitSPLabProperty();
#if TRUE
	this->GetPsPgIteration();	
#endif
	this->FuzzyClassification();
#if 0
	this->ForceSky_InDoor();
	this->ForceGround_InDoor();
#endif


this->ForceVertical_InDoor_DFS();

cui_GeneralImgProcess::CuiSaveImgWithPoints(
	pMD->Src_ImgData,
	cui_ImgLables,
	pMD->Matrix_Category_Lable_InDoor.get(),
	pMD->ImgWidth,
	pMD->ImgHeight,
	true,
	pMD,
	"");
#if 0
	//这里有BUG--要重调--2014年3月12日18:58:18
	
	/*地面由root一直迭代下去，符合条件的设置地面Flag，最后检查Flag*/
	this->InitSPLabProperty();
	ForceGround_InDoor_DFS(pMD);
#endif
	cui_GeneralImgProcess::CuiSaveImgWithPoints(
		pMD->Src_ImgData,
		cui_ImgLables,
		pMD->Matrix_Category_Lable_InDoor.get(),
		pMD->ImgWidth,
		pMD->ImgHeight,
		true,
		pMD,
		"");
#if 1
//去掉悬浮于地面的立面
	this->RemoveSunpendVonG_MustHaveS();
#endif

#if 1	
	this->ForceSky_InDoor_DFS();
#endif

	//第一次分类的结果
	cui_GeneralImgProcess::CuiSaveImgWithPoints(
		pMD->Src_ImgData,
		cui_ImgLables,
		pMD->Matrix_Category_Lable_InDoor.get(),
		pMD->ImgWidth,
		pMD->ImgHeight,true,
		pMD,"");
	

}
/*--------------------------------------------------------------------------------------------------------*/
/**
*利用模糊函数的结果分离天空、立面、地面
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG::zlm_ForceSkyGroundVertical_InDoor3_31(void)
{
	this->InitSPLabProperty();
#if TRUE
	this->GetPsPgIteration();	
#endif

	this->FuzzyClassification();


	this->ForceVertical_InDoor_DFS();

	cui_GeneralImgProcess::CuiSaveImgWithPoints(
		pMD->Src_ImgData,
		cui_ImgLables,
		pMD->Matrix_Category_Lable_InDoor.get(),
		pMD->ImgWidth,
		pMD->ImgHeight,
		true,
		pMD,
		"");
#if TRUE
	//这里有BUG--要重调--2014年3月12日18:58:18

	/*地面由root一直迭代下去，符合条件的设置地面Flag，最后检查Flag*/
	this->InitSPLabProperty();
	ForceGround_InDoor_DFS(pMD);
	cui_GeneralImgProcess::CuiSaveImgWithPoints(
		pMD->Src_ImgData,
		cui_ImgLables,
		pMD->Matrix_Category_Lable_InDoor.get(),
		pMD->ImgWidth,
		pMD->ImgHeight,
		true,
		pMD,
		"");
	//去掉悬浮于地面的立面
	this->RemoveSunpendVonG_MustHaveS();
#else
	this->InitSPLabProperty(); 
	this->RemoveSunpendVonG_MustHaveS();
#endif

#if TRUE	
	this->ForceSky_InDoor_DFS();
#endif

	this->DecideFloatingSky_InDoor_DFS();
	//第一次分类的结果
	cui_GeneralImgProcess::CuiSaveImgWithPoints(
		pMD->Src_ImgData,
		cui_ImgLables,
		pMD->Matrix_Category_Lable_InDoor.get(),
		pMD->ImgWidth,
		pMD->ImgHeight,true,
		pMD,"");


}
/*--------------------------------------------------------------------------------------------------------*/
/**
*利用模糊函数的结果分离天空、立面、地面
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG::zlm_ForceSkyGroundVertical_InDoor4_16(void)
{
	this->InitSPLabProperty();
#if TRUE
	this->GetPsPg_DFS();	
#endif

	this->FuzzyClassification();


	this->ForceVertical_InDoor_DFS();

	cui_GeneralImgProcess::CuiSaveImgWithPoints(
		pMD->Src_ImgData,
		cui_ImgLables,
		pMD->Matrix_Category_Lable_InDoor.get(),
		pMD->ImgWidth,
		pMD->ImgHeight,
		true,
		pMD,
		"");
#if TRUE
	//这里有BUG--要重调--2014年3月12日18:58:18

	/*地面由root一直迭代下去，符合条件的设置地面Flag，最后检查Flag*/
	this->InitSPLabProperty();
	ForceGround_InDoor_DFS(pMD);
	cui_GeneralImgProcess::CuiSaveImgWithPoints(
		pMD->Src_ImgData,
		cui_ImgLables,
		pMD->Matrix_Category_Lable_InDoor.get(),
		pMD->ImgWidth,
		pMD->ImgHeight,
		true,
		pMD,
		"");
	//去掉悬浮于地面的立面
	this->RemoveSunpendVonG_MustHaveS();
#else
	this->InitSPLabProperty();
	this->RemoveSunpendVonG_MustHaveS();
#endif

#if TRUE	
	this->ForceSky_InDoor_DFS();
#endif

	this->DecideFloatingSky_InDoor_DFS();
	//第一次分类的结果
	cui_GeneralImgProcess::CuiSaveImgWithPoints(
		pMD->Src_ImgData,
		cui_ImgLables,
		pMD->Matrix_Category_Lable_InDoor.get(),
		pMD->ImgWidth,
		pMD->ImgHeight,true,
		pMD,"");


}
/*--------------------------------------------------------------------------------------------------------*/
/**
*利用模糊函数的结果分离天空、立面、地面
*
*
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG::zlm_ForceSkyGroundVertical_InDoor4_26(void)
{	
#if FALSE
	this->FuzzyClassification();
	HsHg hshg(pMD);
	hshg.Get_HsHg_By_SV_VG();
	this->FuzzyClassification();
#endif
	this->GetPsPgIteration_NearBigFarSmall_5m4d();
	this->FuzzyClassification(true);
#if 0
this->RemoveSunpendVonG_MustHaveS();

	this->ForceSky_InDoor_DFS();//去掉不连续的天空
#endif
	//第一次分类的结果
	cui_GeneralImgProcess::CuiSaveImgWithPoints(
		pMD->Src_ImgData,
		pMD->ImgLables.get(),
		pMD->Matrix_Category_Lable_InDoor.get(),
		pMD->ImgWidth,
		pMD->ImgHeight,true,
		pMD,"");
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*利用模糊函数的结果分离天空部分
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG::zlm_ForceSky(void)
{
	ULONGLONG sky_Effect_Index=0;
	double    sky_energy_threshold=0,sky_Light_Threshold=0;
#if USESTATIC 
	CalculateSkyVHarrEnergy_S(
		cui_NumLabels,
		harr_sp_energy,
		p_SpProperty,
		cui_ImgLables,
		cui_ImgData,
		cui_Width,
		cui_Height,
		cui_G_V_S_Num_Category,
		Category_Force);
#else
    this->CalculateSkyVHarrEnergy();
#endif
	////////////////////////////////////////////////////////////////////////////////////
	double 	energysort[MAX_SP_NUM ],energy_length=0;
	for (register int sp=cui_NumLabels-1;sp>=0;sp--){
		if ((cui_G_V_S_Num_Category[10*sp+4]!=Ground)){
			energysort[(UINT)energy_length++]=harr_sp_energy[sp].EnergyL_LL;
		}
	}
	sky_Light_Threshold=cui_GeneralImgProcess::GetAverageValue(energysort,energy_length); //
	//////////////////////////////////////////////////////////////////////////////////////
	vector<double> sky_harr_energy;
	for(register int sp=0;sp<cui_NumLabels;sp++){
		if ((cui_G_V_S_Num_Category[10*sp+4]!=Ground)){
				sky_harr_energy.push_back(harr_sp_energy[sp].EnergyHighsigma);
		}

	}
	double sky_energy_avg,sky_energy_variance,sky_energy_up,sky_energy_down;
	cui_GeneralImgProcess::GetVarianceValue(sky_harr_energy.data(),sky_harr_energy.size(),&sky_energy_avg,&sky_energy_variance);
	sky_energy_up=(double)Sky_Energy_UP;
	sky_energy_down=(double)Sky_Energy_DOWN;
	/////////////////////////////////////////////////////////////////////////////////////
	sky_energy_threshold=std::min(sky_energy_avg,sky_energy_up);
	sky_energy_threshold=std::max(sky_energy_threshold,sky_energy_down);

	//sky_energy_threshold=sky_energy_avg-1.0*sky_energy_variance;
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef  Gray_Sky
#if Gray_Sky
   sky_Light_Threshold=0;
#endif	
#endif
   /*-----------------------------------------------------------------------------*/
#if USESTATIC
   sky_Effect_Index=CalculateMostEffectiveSky_S(
	   sky_Light_Threshold,
	   sky_energy_threshold,
	   cui_NumLabels,
	   cui_G_V_S_Num_Category,
	   harr_sp_energy,
	   cui_Matrix_Category_Lable,
	   Category_Force);
#else
   sky_Effect_Index=CalculateMostEffectiveSky(sky_Light_Threshold,sky_energy_threshold);
#endif


	if (sky_Effect_Index!=-1){
		//计算颜色ab距离
			for (register int sp=cui_NumLabels-1;sp>=0;sp--){
				cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+Lab_2_P_Dst]=sqrtl(pow(cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+A_COLOR]-cui_G_V_S_Num_Category[G_V_S_Num_Category*sky_Effect_Index+A_COLOR],2)+pow(cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+B_COLOR]-cui_G_V_S_Num_Category[G_V_S_Num_Category*sky_Effect_Index+B_COLOR],2));
			}
			//计算L距离
			for (register int sp=cui_NumLabels-1;sp>=0;sp--){		  
				Lab_Color_Category[sp].L_Dst2Sky=sqrtl(pow(Lab_Color_Category[sp].L-Lab_Color_Category[sky_Effect_Index].L,2));
			}
	}
	
#if 1
	{
		char data_t[1024];
		ofstream outfile;
		outfile.open("Category2Matrix_L_A_B.data",ios::out);
		for(register int i = 0; i <cui_NumLabels; i++ ){
			{
				double value_t= Lab_Color_Category[i].L;
				sprintf_s(data_t,1024," %0.3f ",value_t);
				outfile<<data_t;
				value_t= Lab_Color_Category[i].L_Dst2Sky;
				sprintf_s(data_t,1024," %0.3f ",value_t);
				outfile<<data_t;
			}
			outfile<<endl;
		} 
		outfile.close();
	}
	/**************************************************************************************/
	{
		char data_t[1024];
		ofstream outfile;
		outfile.open("Category2Matrix_G_V_S_Num_zlm.data",ios::out);
		for(register int i = 0; i <cui_NumLabels; i++ ){
			for(register int j = 0; j <10; j++ ){
				double value_t= cui_G_V_S_Num_Category[i*10+j];
				sprintf_s(data_t,1024," %0.3f ",value_t);
				outfile<<data_t;
			}
			outfile<<endl;
		} 
		outfile.close();
	}
#endif
#if USESTATIC
    RemoveUnEffectiveSkySP_S(
		sky_Effect_Index,
		60,
		cui_NumLabels,
		cui_G_V_S_Num_Category,
		harr_sp_energy,
		Lab_Color_Category,
		cui_Matrix_Category_Lable,
		Category_Force);
	 DivideSkyV_S(
		 sky_energy_threshold,
		 sky_Light_Threshold,
		 60,
		 Sky_Lab_Distance,
		 cui_ImgLables,
		 cui_NumLabels,
		 pMD,
		 harr_sp_energy,
		 E_matrix,
		 cui_G_V_S_Num_Category);
#else
	RemoveUnEffectiveSkySP(sky_Effect_Index,60);
	DivideSkyV(sky_energy_threshold,sky_Light_Threshold,60,Sky_Lab_Distance);
#endif
	

}	
/*--------------------------------------------------------------------------------------------------------*/
/**
*计算超像素块的小波能量（已弃用）
*@param sp 超像素快的索引
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG::CalculateBlockEnergy(int sp)
{
   	CuiHarrTransformLet Harr_Origin,Harr_Border;
	UINT32 *Origin_img,*Border_img;
    double  proportion_t=0;
	Energy_Harr harr_boder_energy,harr_origin_energy;
	Origin_img=new UINT32[cui_Width*cui_Height];
	Border_img=new UINT32[cui_Width*cui_Height];
	 /*********************************************/
	for(register int hi=0;hi<cui_Height;hi++){
		for (register int j=0;j<cui_Width;j++){	

			if (cui_ImgLables[hi*cui_Width+j]==sp){
				Origin_img[hi*cui_Width+j]=cui_ImgData[hi*cui_Width+j];
				Border_img[hi*cui_Width+j]=0x00ffffff;
				proportion_t+=1;
			}else{
			    Origin_img[hi*cui_Width+j]=0;
				Border_img[hi*cui_Width+j]=0;
			}
			
		}
	}
	proportion_t=proportion_t/(cui_Width*cui_Height);
	/**********************************************/
	Harr_Origin.CuiGetImageData(Origin_img,cui_Width,cui_Height,proportion_t);
	Harr_Border.CuiGetImageData(Border_img,cui_Width,cui_Height,proportion_t);

	Harr_Origin.CuiWaveletTrans(1);
	Harr_Border.CalculateBoderImg(1);
	Harr_Origin.CuiAdjustBoderByContour(Harr_Border.img_dst_wavelet);
	Harr_Origin.CuiCalculateEnergyALL();
	//Harr_Border.CuiCalculateEnergyALL();
	Harr_Origin.CuiGetAllEnergy(&harr_origin_energy);
	//Harr_Border.CuiGetAllEnergy(&harr_boder_energy);	
#if CUI_SAVE_HARR_IMG
	Harr_Border.CuiSaveImg(); 
	Harr_Origin.CuiSaveImg();
#endif
	harr_sp_energy[sp].EnergyL_LL=harr_origin_energy.EnergyL_LL;
	harr_sp_energy[sp].EnergyL_LH=harr_origin_energy.EnergyL_LH;
	harr_sp_energy[sp].EnergyL_HL=harr_origin_energy.EnergyL_HL;
	harr_sp_energy[sp].EnergyL_HH=harr_origin_energy.EnergyL_HH;
	harr_sp_energy[sp].Energy_LL=harr_origin_energy.Energy_LL;
	harr_sp_energy[sp].Energy_LH=harr_origin_energy.Energy_LH;
	harr_sp_energy[sp].Energy_HL=harr_origin_energy.Energy_HL;
	harr_sp_energy[sp].Energy_HH=harr_origin_energy.Energy_HH;
	harr_sp_energy[sp].EnergyHighsigma=harr_origin_energy.EnergyHighsigma;
	/**********************************************/
	delete[]Origin_img;
	delete[]Border_img;
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*计算超像素块的小波能量（充分去除边界上小波能量带来的误差）
*@param sp 超像素快的索引
*@note
*采用l_2范数计算各图块平均能量测度\n
*   E=√(1/N_p  ∑_(i=r_b)^(r_t)∑_(j=c_l)^(c_r)R^2 (i,j))\n   
*其中：Np为图块像素个数，rb为图块的最下延，rt为图块的最上延，\n
*cl为第i行图块最左边，cr为第i行图块最右边，R(i,j)为(i,j)点处的单层小波采样值\n
*，值得注意的是在计算各图块平均能量测度时需要去除应图块边缘所产生的伪能量\n
*（注：所谓的伪能量是由于图块的不规则分割边界所形成的能力，它不能代表图块本身的纹理特征）。\n
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG::CalculateBlockEnergy2(int sp)
{
	CuiHarrTransformLet Harr_Origin,Harr_Border;
	UINT32 *Origin_img,*Border_img;
	double  proportion_t=0;
	Energy_Harr harr_boder_energy,harr_origin_energy;
	int img_width=p_SpProperty[sp].max_x-p_SpProperty[sp].min_x;
	int img_height=p_SpProperty[sp].max_y-p_SpProperty[sp].min_y;
	Origin_img=new UINT32[img_width*img_height];//原图 
	Border_img=new UINT32[img_width*img_height];//轮廓
	/*********************************************/
	for(register int x=p_SpProperty[sp].min_x;x<p_SpProperty[sp].max_x;x++){
		for (register int y=p_SpProperty[sp].min_y;y<p_SpProperty[sp].max_y;y++){
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

	Harr_Origin.CuiWaveletTrans(1);
	Harr_Border.CalculateBoderImg_Block(1);
#if CUI_SAVE_HARR_IMG
	Harr_Border.CuiSaveImg(); 
	Harr_Origin.CuiSaveImg();
#endif
	//Harr_Origin.CuiAdjustBoder(Harr_Border.img_dst_wavelet);
	Harr_Origin.CuiAdjustBoderByContour(Harr_Border.img_dst_wavelet);
	Harr_Origin.CuiCalculateEnergyALL();
	//Harr_Border.CuiCalculateEnergyALL();
	Harr_Origin.CuiGetAllEnergy(&harr_origin_energy);
	//Harr_Border.CuiGetAllEnergy(&harr_boder_energy);	
#if CUI_SAVE_HARR_IMG
	Harr_Border.CuiSaveImg(); 
	Harr_Origin.CuiSaveImg();
#endif
	harr_sp_energy[sp].EnergyL_LL=harr_origin_energy.EnergyL_LL;
	harr_sp_energy[sp].EnergyL_LH=harr_origin_energy.EnergyL_LH;
	harr_sp_energy[sp].EnergyL_HL=harr_origin_energy.EnergyL_HL;
	harr_sp_energy[sp].EnergyL_HH=harr_origin_energy.EnergyL_HH;
	harr_sp_energy[sp].Energy_LL=harr_origin_energy.Energy_LL;
	harr_sp_energy[sp].Energy_LH=harr_origin_energy.Energy_LH;
	harr_sp_energy[sp].Energy_HL=harr_origin_energy.Energy_HL;
	harr_sp_energy[sp].Energy_HH=harr_origin_energy.Energy_HH;
	harr_sp_energy[sp].EnergyHighsigma=harr_origin_energy.EnergyHighsigma;
	/**********************************************/
	delete[]Origin_img;
	delete[]Border_img;
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*计算超像素块的小波能量（充分去除边界上小波能量带来的误差）
*@param sp 超像素快的索引
*@note
*采用l_2范数计算各图块平均能量测度\n
*   E=√(1/N_p  ∑_(i=r_b)^(r_t)∑_(j=c_l)^(c_r)R^2 (i,j))\n   
*其中：Np为图块像素个数，rb为图块的最下延，rt为图块的最上延，\n
*cl为第i行图块最左边，cr为第i行图块最右边，R(i,j)为(i,j)点处的单层小波采样值\n
*，值得注意的是在计算各图块平均能量测度时需要去除应图块边缘所产生的伪能量\n
*（注：所谓的伪能量是由于图块的不规则分割边界所形成的能力，它不能代表图块本身的纹理特征）。\n
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG::CalculateBlockEnergy2_S(
	int sp,
	Energy_Harr* harr_sp_energy,
	SP_PROPERTY *p_SpProperty,
	int*  cui_ImgLables,
	UINT32* cui_ImgData,
	int cui_Width,
	int cui_Height)
{
#if 1
    CuiHarrTransformLet Harr_Origin,Harr_Border;
	UINT32 *Origin_img,*Border_img;
	double  proportion_t=0;
	Energy_Harr harr_boder_energy,harr_origin_energy;
	int img_width=p_SpProperty[sp].max_x-p_SpProperty[sp].min_x;
	int img_height=p_SpProperty[sp].max_y-p_SpProperty[sp].min_y;
	Origin_img=new UINT32[img_width*img_height];//原图 
	Border_img=new UINT32[img_width*img_height];//轮廓
	/*********************************************/
	for(register int x=p_SpProperty[sp].min_x;x<p_SpProperty[sp].max_x;x++){
		for (register int y=p_SpProperty[sp].min_y;y<p_SpProperty[sp].max_y;y++){
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

	Harr_Origin.CuiWaveletTrans(1);
	Harr_Border.CalculateBoderImg(1);
	//Harr_Origin.CuiAdjustBoder(Harr_Border.img_dst_wavelet);
	Harr_Origin.CuiAdjustBoderByContour(Harr_Border.img_dst_wavelet);
	Harr_Origin.CuiCalculateEnergyALL();
	//Harr_Border.CuiCalculateEnergyALL();
	Harr_Origin.CuiGetAllEnergy(&harr_origin_energy);
	//Harr_Border.CuiGetAllEnergy(&harr_boder_energy);	
#if CUI_SAVE_HARR_IMG
	Harr_Border.CuiSaveImg(); 
	Harr_Origin.CuiSaveImg();
#endif
	harr_sp_energy[sp].EnergyL_LL=harr_origin_energy.EnergyL_LL;
	harr_sp_energy[sp].EnergyL_LH=harr_origin_energy.EnergyL_LH;
	harr_sp_energy[sp].EnergyL_HL=harr_origin_energy.EnergyL_HL;
	harr_sp_energy[sp].EnergyL_HH=harr_origin_energy.EnergyL_HH;
	harr_sp_energy[sp].Energy_LL=harr_origin_energy.Energy_LL;
	harr_sp_energy[sp].Energy_LH=harr_origin_energy.Energy_LH;
	harr_sp_energy[sp].Energy_HL=harr_origin_energy.Energy_HL;
	harr_sp_energy[sp].Energy_HH=harr_origin_energy.Energy_HH;
	harr_sp_energy[sp].EnergyHighsigma=harr_origin_energy.EnergyHighsigma;
	/**********************************************/
	delete[]Origin_img;
	delete[]Border_img;
#endif
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*对地面部分进行聚类
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG::zlm_ForceGround(void){
	static double num_t[2][MAX_SP_NUM ];
	memset(num_t,0,sizeof(num_t));
	//全部
	for (register int mi=0;mi<cui_Height;mi++){
		for (register int mj=0;mj<cui_Width;mj++){
				num_t[0][cui_ImgLables[mi*cui_Width+mj]]++;
		}
	}
	//一半以下
	for (register int mi=cui_HorizontalLine;mi<cui_Height;mi++){
		for (register int mj=0;mj<cui_Width;mj++){
			num_t[1][cui_ImgLables[mi*cui_Width+mj]]++;
		}
	}
	for (register int sp=0;sp<cui_NumLabels;sp++){
		if (num_t[0][sp]==num_t[1][sp]){
			cui_G_V_S_Num_Category[10*sp+4]=cui_Matrix_Category_Lable[sp]=Ground;
		}
	}
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*对立面部分进行聚类\n
*@note 穿过地平线都为立面
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG::zlm_ForceVertical(void)
{


}
/*--------------------------------------------------------------------------------------------------------*/
/**
*
*
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG::zlm_ForceVertical_SG(void)
{	
	
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*填充地面权重数组\n
*@param horizontal_line 水平线位置
*@param n 拟合函数的变量n
*@note 
*构建地面模糊分布密度函数G：\n
*当H_G≥H_S时：令G(x)=C_G x^(1/(2n+1)) 且∫_(-H_G)^0*G(x) dx=1 \n
*则得C_G=-((2n+2)/(2n+1))*H_G^(-(2n+2)/(2n+1))\n
*当H_G<H_S时：G(x)=-S(x)\n
*即                  G(x)={(C_G x^(1/(2n+1))       当H_G≥H_S 时\n
*						  { -S(x)              	   当H_G<H_S 时)\n
*其中： n=1,2,3……N，N∈正整数，本文中的实验取n=1。
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG::FillWeightArrayG(double horizontal_line, double n)
{
	double C_1,C_2;
	double L_1,L_2;	
	double debug_t;
	L_1=cui_Height-horizontal_line;
	L_2=horizontal_line;
	C_1=(2*n+2.0)/((2*n+1)*powl(L_1,(2*n+2)/(2*n+1.0)));
	C_2=(2*n+2.0)/((2*n+1)*powl(L_2,(2*n+2)/(2*n+1.0)));

	for (register int i=-L_1;i<L_2;i++){
		int x=i;
		int y=horizontal_line-x-1;
		 if (y>=0&&y<cui_Height){
		  				if (i<0){
						//奇函数
					       cui_pYweight_GVS[(y)*3+0]=debug_t=-1*(-1*C_1*powl(-1*x,1.0/(2*n+1)))*WeightZoom;

						}else{
							cui_pYweight_GVS[(y)*3+0]=debug_t=-1*C_2*powl(x,1.0/(2*n+1))*WeightZoom;
						}

		 }
	
		
	}

}
/*--------------------------------------------------------------------------------------------------------*/
/**
*填充立面权重数组
*@param horizontal_line 水平线位置
*@param n 拟合函数的变量n
*@note
*立面物体模糊分布密度函数V：\n
*V(x)=1/(√2π σ) e^(-x^2/σ^2 ) \n
*σ=H_G/3 当H_G≥H_S 时 \n
*σ=H_S/3 当H_G<H_S  时 \n
* 
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG::FillWeightArrayV(double horizontal_line, double n)
{
#if 0
	double delta;
	double L,L_1,L_2;
	L_1=horizontal_line;
	L_2=cui_Height-horizontal_line;
	L=(L_1>L_2)?L_2:L_1;
	delta=L/3;


	for (register int i=-L;i<L;i++){
	   if ((horizontal_line-i)>=0&&(horizontal_line-i)<cui_Height){
		  cui_pYweight_GVS[((int)horizontal_line-i)*3+1]=
			  (1/(sqrt(2*M_PI)*delta))*exp(-powl(i,2)/powl(delta,2))*WeightZoom;
	  }
	}
#endif
	double delta;
	double L,L_1,L_2;
	L_1=horizontal_line;
	L_2=cui_Height-horizontal_line;
	L=(L_1>L_2)?L_2:L_1;
	delta=L/3;

	

	for (register int i=-L;i<L;i++){

		int x=i;
		int y=0-x+horizontal_line;

		if (y>=0&&y<cui_Height){
			cui_pYweight_GVS[y*3+1]=(1/(sqrt(2*M_PI)*delta))*exp(-powl(x,2)/powl(delta,2))*WeightZoom;
		}
	}

}
/*--------------------------------------------------------------------------------------------------------*/
/**
*填充天空权重数组
*@param horizontal_line 水平线位置
*@param n 拟合函数的变量n
*@note 
*构建天空模糊分布密度函数S：
*当H_G<H_S时：令S(x)=C_S x^(1/(2n+1)) 且∫_0^(H_S)〖G(x)〗 dx=1 \n 
*则得C_S=((2n+2)/(2n+1)) 〖H_S〗^(-(2n+2)/(2n+1))\n
*当H_G≥H_S时：S(x)=-G(x)\n
*即                  S(x)={(C_S x^(1/(2n+1)) 当H_G<H_S 时\n
*                         { -G(x)            当H_G≥H_S 时\n                  
*其中： n=1,2,3……N，N∈正整数，本文中的实验取n=1。\n
*
*/
/*---------------------------------------------------------------------------------------------------*/
void ComputeSVG::FillWeightArrayS(double horizontal_line, double n)
{

	double C_1,C_2;
	double L_1,L_2;
	L_1=cui_Height-horizontal_line;
	L_2=horizontal_line;
	C_1=(2*n+2.0)/((2*n+1)*powl(L_1,(2*n+2)/(2*n+1.0)));
	C_2=(2*n+2.0)/((2*n+1)*powl(L_2,(2*n+2)/(2*n+1.0)));

#if 0
for (register int i=-L_1;i<0;i++){
		int x=i;
		int y=horizontal_line-x-1;
		 if ((horizontal_line-i-1)>=0&&(horizontal_line-i-1)<cui_Height) {
				cui_pYweight_GVS[((int)horizontal_line-i-1)*3+2]=-1*C_1*powl(-i,1.0/(2*n+1))*WeightZoom;
		  }
	}
	for (register int i=0;i<L_2;i++){

		 if ((horizontal_line-i-1)>=0&&(horizontal_line-i-1)<cui_Height){
		        cui_pYweight_GVS[((int)horizontal_line-i-1)*3+2]=C_2*powl(i,1.0/(2*n+1))*WeightZoom;
		  }
	}
#else
	for (register int i=-L_1;i<L_2;i++){
		int x=i;
		int y=horizontal_line-x-1;
		if (y>=0&&y<cui_Height){
			if (x<0){
				cui_pYweight_GVS[(y)*3+2]=-1*C_1*powl(-x,1.0/(2*n+1))*WeightZoom;
			}else if (x>=0){
				cui_pYweight_GVS[(y)*3+2]=C_2*powl(x,1.0/(2*n+1))*WeightZoom;
			}else{
				assert(0);
			}
			
		}
	}
#endif
	
}
/*---------------------------------------------------------------------------------------------------*/
/**
*地面重力场视觉模糊分布密度函数G
*
*@param horizontal_line 水平线位置 
*@param n  
*@note 
*@image html image_fuzzy_sky_ground_vertical.jpg 图像中视平线以及天花板和地面可能出现的区域 
*/
/*---------------------------------------------------------------------------------------------------*/
void  ComputeSVG::FillWeightArrayG_InDoor(double horizontal_line, double n)
{
#if TRUE
	horizontal_line=pMD->Seg_HorizontalLinePos;
	int bottom=pMD->Seg_HorizontalLinePos-pMD->ImgHeight;
	int top=pMD->Seg_HorizontalLinePos;
	for (int x=bottom;x<top;x++){
		if ((horizontal_line-x-1)>=0&&(horizontal_line-x-1)<pMD->ImgHeight){

			cui_pYweight_GVS[((int)horizontal_line-x-1)*3+0]=Gx_InDoor(x,1,pMD->Seg_HorizontalLinePosScale,pMD->PgOffset,pMD->ImgHeight)*WeightZoom;
		}
	}
#endif
}
/*---------------------------------------------------------------------------------------------------*/
/**
*天花板重力场视觉模糊分布密度函数
*@note
*@image html image_fuzzy_sky_ground_vertical.jpg 图像中视平线以及天花板和地面可能出现的区域 
*/
/*---------------------------------------------------------------------------------------------------*/
void  ComputeSVG::FillWeightArrayS_InDoor(double horizontal_line, double n)
{
#if TRUE
	horizontal_line=pMD->Seg_HorizontalLinePos;
	int bottom=pMD->Seg_HorizontalLinePos-pMD->ImgHeight;
	int top=pMD->Seg_HorizontalLinePos;
	for (int x=bottom;x<top;x++){
		if ((horizontal_line-x-1)>=0&&(horizontal_line-x-1)<pMD->ImgHeight){

			cui_pYweight_GVS[((int)horizontal_line-x-1)*3+2]=Sx_InDoor(x,1,pMD->Seg_HorizontalLinePosScale,pMD->PsOffset,pMD->ImgHeight)*WeightZoom;
		}
	}
#endif
	
}
/*---------------------------------------------------------------------------------------------------*/
/**
*去除不合理的超像素天空图块\n
*主要根据位置关系
*/
/*---------------------------------------------------------------------------------------------------*/
void ComputeSVG::RemoveSomeUnreasonableClassification(void)
{
	//天空的聚类方法决定了，天空不纯在悬浮
#if 1
	this->RemoveSkyBySquareMeter();
#endif
#if 1
    //this->zlm_ForceVertical(); 
	this->zlm_ForceVertical_SG();
#endif	
#if 1
	//地面的模糊结果，分块地面的重新分类（类似天空聚类）
	this->RemoveGroundByPosition2();
#endif
#if 0
	//去掉悬浮的立面
	this->RemoveVerticalByNeighbor();
#endif	
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*根据天空的面积，移除面积较小的天空
*本方法存在一些问题
*有一种递归的方法可以解决，参考数据结构之图的访问
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG::RemoveSkyBySquareMeter(void)
{


}
#if 0
void ComputeSVG::FindandSetNeighbour(INT32 *Category,INT32 spnum,int sp_start,double* GVS_Category,UINT32* E_matrix)
{
	for (register int spj=spnum;spj>=0;spj--){

		if (GVS_Category[10*spj+4]==Ground){					
			if (0!=E_matrix[sp_start*spnum+spj]){
				Category[spj]=Category[sp_start];
				FindandSetNeighbour(Category,spnum,spj,GVS_Category,E_matrix);
			}			   
		}


	}	
}
#endif
/*--------------------------------------------------------------------------------------------------------*/
/**
*移除不合理的地面超像素块（已弃用）
*本方法存在一些问题
*有一种递归的方法可以解决，参考数据结构之图的访问
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG::RemoveGroundByPosition(void)
{
	INT32  Category_t[1024];//地面连接关系新分类 
	memset(Category_t,-1,sizeof(Category_t));
/************重新分类***************************************************************************/
for (register int spi=cui_NumLabels-1;spi>=0;spi--){	 
	   if (cui_G_V_S_Num_Category[10*spi+4]==Ground){
			 /*****地面重新分类***********/
				   if (Category_t[spi]==-1){
					   Category_t[spi]=spi;
				   }		
				   
				   for (register int spj=spi;spj>=0;spj--){
					   if (cui_G_V_S_Num_Category[10*spj+4]==Ground){					
						   if (0!=E_matrix[spi*cui_NumLabels+spj]){
							   Category_t[spj]=Category_t[spi];
						   }			   
					   }

				   }

	 }
#if 0
  /*****没有发现分类*******************************************************************/
	   double category_value[1024];
	   for(int i=0;i<cui_NumLabels;i++){
	   		category_value[i]=Category_t[i];
	   }  
	   double max_value=cui_GeneralImgProcess::GetMaxValue(category_value,cui_NumLabels);
	   if (-1==max_value){
		   continue;
		   //continue 语句PASS
	   }
/*****删除不合理分类*******************************************************************/
#endif
}	

 #if 0
   /*********去除歧义位置****************************************************************************/
	   INT32 Y_position_max[1024]; 
	   memset(Y_position_max,-1,sizeof(Y_position_max));
	   for (register int x=0;x<cui_Width;x++){
		   for (register int y=0;y<cui_Height;y++){
			   if (Category_t[cui_ImgLables[y*cui_Width+x]]!=-1){
				    		 if (y>Y_position_max[Category_t[cui_ImgLables[y*cui_Width+x]]]){
								 Y_position_max[Category_t[cui_ImgLables[y*cui_Width+x]]]=y;
								 //寻找最大的Y坐标
							 }

			    }
		   }
	   }
	   for (register int i=cui_NumLabels-1;i>=0;i--){
		   if ((Y_position_max[i]>=0)&&
			   (Y_position_max[i]<cui_Height-1)){
				for (register int spi=0;spi<cui_NumLabels;spi++){
					if (Category_t[spi]==i){
					  	  cui_G_V_S_Num_Category[10*spi+4]=cui_Matrix_Category_Lable[spi]=harr_sp_energy[spi].Category_Sky=Vertical;
					}
				}
		   }

	   }
 	  /***************************************************/
#endif

}
/*--------------------------------------------------------------------------------------------------------*/
/**
*移除不合理的地面超像素块
*本方法存在一些问题
*有一种递归的方法可以解决，参考数据结构之图的访问
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG::RemoveGroundByPosition2(void)
{

}
/*--------------------------------------------------------------------------------------------------------*/
/**
* 去掉天空上的立面
*移除不合理的立面
*悬空的立面会被移除（已弃用）
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG::RemoveVerticalByNeighbor(void)
{

	 
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*确定一个超像素块的天空、地面、立面
*
*@param lables 超像素块的标定信息
*@param width  超像素快的宽度
*@param height 超像素快的高度
*@param sp    超像素标号
*@return  超像素所属的分类
*/
/*-------------------------------------------------------------------------------------------------------*/
int ComputeSVG::ClassifyOneSPlCategory(int *lables,int width,int height,int sp)
{
	double sky_w=0,vertical_w=0,gnd_w=0;
	for (int x=0;x<width;x++){
		for(int y=0;y<height;y++){
		   if (lables[y*width+x]==sp){
			  gnd_w+=cui_pYweight_GVS[y*3+0];	//G
			  vertical_w+=cui_pYweight_GVS[y*3+1];	//V
			  sky_w+=cui_pYweight_GVS[y*3+2];	//S
			   
		   }
		}
	}
	if (gnd_w>vertical_w){
		return Ground;

	}else if (gnd_w<=vertical_w){
		return Vertical;
	}else{
		ASSERT(FALSE);
		return -1;
	}
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*根据超像素快的以下属性判断其是否是天空
*@param spj
*@param sky_Light_Threshold
*@param sky_energy_Threshold
*@param AB_distance
*@param L_distanc
*@return true是天空超像素块；false不是天空超像素块
*/
/*-------------------------------------------------------------------------------------------------------*/
bool ComputeSVG::Is_Sky_Block(int spj,float sky_Light_Threshold,float sky_energy_Threshold,float AB_distance,float L_distance)
{
	bool IsSkyBlock=false;
    bool light_condition=(harr_sp_energy[spj].EnergyL_LL>sky_Light_Threshold);
	bool energy_condition=(harr_sp_energy[spj].EnergyHighsigma<sky_energy_Threshold);
	bool AB_color_condition=((cui_G_V_S_Num_Category[G_V_S_Num_Category*spj+Lab_2_0_Dst]<AB_distance)||(cui_G_V_S_Num_Category[G_V_S_Num_Category*spj+Lab_2_P_Dst]<AB_distance));				    
	bool L_color_condition=((Lab_Color_Category[spj].L_Dst2Sky<L_distance)||(Lab_Color_Category[spj].L>=250));	


	 bool light_condition2=(harr_sp_energy[spj].EnergyL_LL-sky_Light_Threshold>=-1*sky_Light_Threshold/4);

	 bool energy_condition2=((harr_sp_energy[spj].EnergyHighsigma<=Sky_Energy_DOWN));
					    
	 bool AB_color_condition2=((cui_G_V_S_Num_Category[G_V_S_Num_Category*spj+Lab_2_0_Dst]<5)||(cui_G_V_S_Num_Category[G_V_S_Num_Category*spj+Lab_2_P_Dst]<AB_distance));


	if (light_condition&&energy_condition&&AB_color_condition&&L_color_condition==true){
		IsSkyBlock=true;
	}
	if (light_condition2&&energy_condition2&&AB_color_condition2&&L_color_condition==true){
		IsSkyBlock=true;
	}
		
	return IsSkyBlock;
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*根据超像素快的以下属性判断其是否是天空
*@param spj
*@param sky_Light_Threshold
*@param sky_energy_Threshold
*@param AB_distance
*@param L_distanc
*@return true是天空超像素块；false不是天空超像素块
*/
/*-------------------------------------------------------------------------------------------------------*/
bool ComputeSVG::Is_Sky_Block_S(
	int spj,
	float sky_Light_Threshold,
	float sky_energy_Threshold,
	float AB_distance,
	float L_distance,
	Energy_Harr* harr_sp_energy,
	double* cui_G_V_S_Num_Category,
	LabColorCategory* Lab_Color_Category)
{
	bool IsSkyBlock=false;
	bool light_condition=(harr_sp_energy[spj].EnergyL_LL>sky_Light_Threshold);
	bool energy_condition=(harr_sp_energy[spj].EnergyHighsigma<sky_energy_Threshold);
	bool AB_color_condition=((cui_G_V_S_Num_Category[G_V_S_Num_Category*spj+Lab_2_0_Dst]<AB_distance)||(cui_G_V_S_Num_Category[G_V_S_Num_Category*spj+Lab_2_P_Dst]<AB_distance));				    
	bool L_color_condition=((Lab_Color_Category[spj].L_Dst2Sky<L_distance)||(Lab_Color_Category[spj].L>=250));	


	bool light_condition2=(harr_sp_energy[spj].EnergyL_LL-sky_Light_Threshold>=-1*sky_Light_Threshold/4);

	bool energy_condition2=((harr_sp_energy[spj].EnergyHighsigma<=Sky_Energy_DOWN));

	bool AB_color_condition2=((cui_G_V_S_Num_Category[G_V_S_Num_Category*spj+Lab_2_0_Dst]<5)||(cui_G_V_S_Num_Category[G_V_S_Num_Category*spj+Lab_2_P_Dst]<AB_distance));


	if (light_condition&&energy_condition&&AB_color_condition&&L_color_condition==true){
		IsSkyBlock=true;
	}
	if (light_condition2&&energy_condition2&&AB_color_condition2&&L_color_condition==true){
		IsSkyBlock=true;
	}

	return IsSkyBlock;
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*计算图块被水平线分割后在水平线之上和水平线之下的面积
*
*@param [out] UpSquare    在水平线之上的面积
*@param [out] DownSquare  在水平线之下的面积  
*@param sp   超像素快的标号
*
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG::CalculateUpandDownSquarebyHor(float* UpSquare, float* DownSquare, int  sp)
{
	*UpSquare=0;
	*DownSquare=0;
#if OUT_DOOR
for (unsigned int x=0;x<cui_Width;x++){
		for (unsigned int y=0;y<cui_Height;y++){
			if (cui_ImgLables[y*cui_Width+x]==sp){

				if (y>=pMD->Seg_HorizontalLinePos){
					  *DownSquare+=1;
				}else{
					  *UpSquare+=1;
				}

			}
		}
	}
#endif

#if IN_DOOR

int ImgWidth=pMD->ImgWidth;
int ImgHeight=pMD->ImgHeight;
int* ImgLables=pMD->ImgLables.get();

for (unsigned int x=0;x< ImgWidth;x++){
	for (unsigned int y=0;y<ImgHeight;y++){
		if (ImgLables[y*cui_Width+x]==sp){

			if (y>=pMD->Seg_HorizontalLinePos){
				*DownSquare+=1;
			}else{
				*UpSquare+=1;
			}

		}
	}
}
#endif
	
}
/*-------------------------------------------------------------------*/
/**
*模糊分类超像素图块
*@param SaveNumInfo\n
*true 保存模糊矩阵
*false 不保存模糊矩阵
@note 所谓室外重力场分布密度函数模型，主要是假设天空和地面图块将一直延\n
*伸至无穷远处，即接近图像视平线位置的函数模型。通过构建如下的重力场室外\n
*场景图像模糊分布密度函数，并以此分布密度函数初步分类室内场景谱聚类图块\n
*为天空、立面及地面。(注：对于室内空间来说天空的类别即对应于室内空间的\n
*天花板)\n
*
*/
/*-------------------------------------------------------------------*/
void ComputeSVG::FuzzyClassification(bool SaveNumInfo)
{
#if IN_DOOR
//填充密度函数
	this->FillWeightArrayZlm(pMD->Seg_HorizontalLinePosScale);
	this->InitSPForceCategory();
	this->CalcuateSpWeightByY();

	/*****************************************************/
	INT32 * Matrix_Category_Lable=pMD->Matrix_Category_Lable_InDoor.get();
	for (register int sp=0;sp<pMD->slic_current_num;sp++){
		Matrix_Category_Lable[sp]=classifyOneSuperpixel(sp);
	}
#if TRUE
	if (SaveNumInfo){
		int VerticalNum=0;
		for (register int sp=0;sp<pMD->slic_current_num;sp++){
			if (Matrix_Category_Lable[sp]==Vertical){
				VerticalNum++;
			}
		}
		pMD->SaveFuzzyVerticalNum(VerticalNum);
	}

#endif
	
	cui_GeneralImgProcess::CuiSaveImgWithPoints(
		pMD->Src_ImgData,
		cui_ImgLables,
		pMD->Matrix_Category_Lable_InDoor.get(),
		pMD->ImgWidth,
		pMD->ImgHeight,
		true,
		pMD,
		"");
	//模糊聚类的结果
	UINT32 StripIsVerticalNum=this->StripIsVertical();
#if TRUE
	if (SaveNumInfo){
		pMD->SaveStripSPVerticalNum(StripIsVerticalNum);	
	}	
#endif
//	this->IsBoderCategoryChange_5m4d();
	//this->CrossEyeLineIsVertical();		
	cui_GeneralImgProcess::CuiSaveImgWithPoints(
		pMD->Src_ImgData,
		cui_ImgLables,
		pMD->Matrix_Category_Lable_InDoor.get(),
		pMD->ImgWidth,
		pMD->ImgHeight,
		true,
		pMD,
		"");
#if TRUE
	cui_GeneralImgProcess::Cui_CombinationImgSVG(
		pMD->Src_ImgData,
		pMD->ImgLables.get(),
		pMD->p_ImgLables_SVG,
		pMD->Matrix_Category_Lable_InDoor.get(),
		pMD->ImgWidth,
		pMD->ImgHeight,
		"","","");
#endif
	cui_GeneralImgProcess::FillHoleOnSVGLables(pMD,true,true,true,true);
#endif
#if OUT_DOOR
	this->FillWeightArrayZlm(pMD->Seg_HorizontalLinePosScale);

	this->InitSPForceCategory();
	this->CalcuateSpWeightByY();

	/*****************************************************/
	INT32 * Matrix_Category_Lable=pMD->Matrix_Category_Lable_InDoor.get();
	for (register int sp=0;sp<pMD->slic_current_num;sp++){
		Matrix_Category_Lable[sp]=classifyOneSuperpixel(sp);
	}
#endif

}
/*-------------------------------------------------------------------*/
/**
*获得立面上的上边界和下边界
*/
/*-------------------------------------------------------------------*/
void ComputeSVG::GetPsPgIteration(void)
{
	INT32 * Matrix_Category_Lable=pMD->Matrix_Category_Lable_InDoor.get();
	HsHg hshg(pMD);
		this->FuzzyClassification();
	do{
		
#if 0
		hshg.Get_HsHg_ByIteration();
#else
		hshg.Get_HsHg_ByIterationNoGeometric();
#endif
		this->FuzzyClassification();

	}while(!hshg.IsConvergence());
}
/*-------------------------------------------------------------------*/
/**
*通过迭代的方法获得立面，上边界和下边界
*
*@note  
*@image html image092.jpg 重力场中室内场景视觉认知模型\n
*人类在室内空间中的视线范围受到室内墙面的限制，因此天花板和地面图像不可\n
*能延伸到趋向无穷远的距离，即在成像靶面上它们图像几乎不可能出现在视平线\n
*的位置上或穿越视平线，其可能出现的极限位置通常与视平线之间有一定的间隔\n
*距离。因此对天花板进行推理的概率分布函数分布密度值从人类视场角最上延的\n
*最大值逐渐降低到人类视场角最下延，其在c点处的概率密度值为零；对地面推理\n
*的概率分布函数分布密度值从人类视场角最下延的最大值逐渐降低到人类视场角\n
*最上延，其在g点上的概率密度值为零；对立面物体推理的概率分布密度值从视平\n
*线上的最大值向上、下两个方向逐渐降低，直至人类视场角的最上与最下延，其\n
*分布密度值接近于零。需要指出的是本发明所构建的天花板和地面密度函数的概\n
*率密度可以小于零，其目的是用以实现对天花板、地面判断的否定。\n
*/
/*-------------------------------------------------------------------*/
void ComputeSVG::GetPsPgIteration_NearBigFarSmall_5m4d(void)
{

	HsHg hshg(pMD);
	this->FuzzyClassification();

	hshg.RecordBoderCategory_5m4d();

	hshg.Get_HsHg_By_NBig_FSmall_5m4d();

	{			
		hshg.CheackBoderCategory_5m4d();

	}
	hshg.IsConvergence_5m4d();
#if 0

#endif
}
/*-------------------------------------------------------------------*/
/**
*获得立面的上边界和下边界
*/
/*-------------------------------------------------------------------*/
void ComputeSVG::GetPsPg_DFS(void)
{
	INT32 * Matrix_Category_Lable=pMD->Matrix_Category_Lable_InDoor.get();

	this->FuzzyClassification();
	do{
			UINT32* LinkConn=pMD->LinkConnection.get();
			UINT32* V=pMD->Matrix_Visit.get();
			memset(V,FALSE,sizeof(UINT32)*pMD->slic_current_num);
			this->InitDFSLinkConnection_Vertical();
#if IN_DOOR
			for (int spi=0;spi<pMD->slic_current_num;spi++){
				/*深序遍历*/
				memset(V,FALSE,sizeof(UINT32)*pMD->slic_current_num);
				if ((LinkConn[spi]==TRUE)){
						TRACE("深序遍历Root=%d\n",spi);
						this->DFS_4Vertical2UporDown_ByColor(spi);
				}
			}
#endif
	}while(FALSE);
	
#if TRUE
	HsHg hshg(pMD);
	hshg.Get_HsHg_By_SV_VG();
#endif
	cui_GeneralImgProcess::CuiSaveImgWithPoints(
		pMD->Src_ImgData,
		cui_ImgLables,
		pMD->Matrix_Category_Lable_InDoor.get(),
		pMD->ImgWidth,
		pMD->ImgHeight,
		true,
		pMD,
		"");
}
/*------------------------------------------------------------------------------------------------------*/
/**
*室内分割算法
*
*
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG::separateSVG_InDoor(void)
{
	ASSERT(pMD->ImgHeight==this->cui_Height);
	ASSERT(pMD->ImgWidth==this->cui_Width);
	INT32 * Matrix_Category_Lable=pMD->Matrix_Category_Lable_InDoor.get();
	
	zlm_ForceSkyGroundVertical_InDoor4_26();
	
	/*****************************************************/
	{
		char data_t[1024];
		ofstream outfile;
		outfile.open("Category2Matrix_G_V_S_Num_zlm.data",ios::out);
		for(register int i = 0; i <pMD->slic_current_num; i++ ){
			for(register int j = 0; j <10; j++ ){
				double value_t= cui_G_V_S_Num_Category[i*10+j];
				sprintf_s(data_t,1024," %0.3f ",value_t);
				outfile<<data_t;
			}
			outfile<<endl;
		} 
		outfile.close();
	}
	/*****************************************************/
	//聚类设置天空聚类轮廓
	cui_GeneralImgProcess::Cui_CombinationImgSVG(
		pMD->Src_ImgData,
		pMD->ImgLables.get(),
		pMD->p_ImgLables_SVG,
		pMD->Matrix_Category_Lable_InDoor.get(),
		pMD->ImgWidth,
		pMD->ImgHeight,
		"","","");
	this->VerticalMustHaveSupport_Pixel();
	this->OverHorLineMustVertical();
	/*************************************************/
	string file_add="InDoor";
	cui_GeneralImgProcess::CuiSaveImgWithPoints(
		pMD->Src_ImgData,
		pMD->p_ImgLables_SVG,
		nullptr,
		pMD->ImgWidth,
		pMD->ImgHeight,
		(bool)OUT_NOGROUND_IMG,pMD,file_add);
	/*************************************************/
}
/*------------------------------------------------------------------------------------------------------*/
/**
*室内分割算法
*天空聚类
*
*/
/*-------------------------------------------------------------------------------------------------------*/

void ComputeSVG::ForceSky_InDoor(void)
{
	HoughAnalyze ha(pMD);
	HistData180 histVall=ha.GetHist180VerticalAll(); 

	HistData180 *hist=pMD->Histogram180.get();
	INT32 * Matrix_Category_Lable=pMD->Matrix_Category_Lable_InDoor.get();
	for (int spi=0;spi<pMD->slic_current_num;spi++){
		if (Matrix_Category_Lable[spi]==Sky){
#if 0
			//块天空和立面标准的接近--归为立面
			if (SpectralClusterInDoor::Calculate2HistBLikelyByManhattan(histVall,hist[spi],AngLeDivided180)>0.8){
				Matrix_Category_Lable[spi]=Vertical;
			}
#endif
#if 0
			if (SpectralClusterInDoor::Calculate2HistBLikely(histVall,hist[spi],AngLeDivided180)>0.7){
				Matrix_Category_Lable[spi]=Vertical;
			}
#endif
#if 1
			if (SceneDetermine::IsVerticalEnergyFocus(hist[spi],AngLeDivided180)){
				Matrix_Category_Lable[spi]=Vertical;
			}
#endif

		}
	}
}
/*------------------------------------------------------------------------------------------------------*/
/**
*地面聚类
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG::ForceGround_InDoor(void)
{
	HoughAnalyze ha(pMD);
	HistData180 histVall=ha.GetHist180VerticalAll();

	HistData180 *hist=pMD->Histogram180.get();
	INT32 * Matrix_Category_Lable=pMD->Matrix_Category_Lable_InDoor.get();
	for (int spi=0;spi<pMD->slic_current_num;spi++){
		if (Matrix_Category_Lable[spi]==Ground){
#if 0
			//块天空和立面标准的接近--归为立面
			if (SpectralClusterInDoor::Calculate2HistBLikelyByManhattan(histVall,hist[spi],AngLeDivided180)>0.8){
				Matrix_Category_Lable[spi]=Vertical;
			}
#endif
#if 0
			if (SpectralClusterInDoor::Calculate2HistBLikely(histVall,hist[spi],AngLeDivided180)>0.7){
				Matrix_Category_Lable[spi]=Vertical;
			}
#endif
#if 1
			if (SceneDetermine::IsVerticalEnergyFocus(hist[spi],AngLeDivided180)){
				Matrix_Category_Lable[spi]=Vertical;
			}
#endif


		}
	}
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*立面聚类
*/
/*------------------------------------------------------------------------------------------------------------*/
void ComputeSVG::DFS_4Vertical2UporDown(int spi)
{
#if TRUE
	static UINT32 DFS_Depth=0;
	DFS_Depth++;
	for (int i=DFS_Depth;i>=0;i--){
		TRACE("-");
	}
	TRACE("%d\n ", spi);   //打印顶点，也可以其他操作
#endif

	UINT32* V=pMD->Matrix_Visit.get();
	UINT32* Matrix_E=pMD->Matrix_E_InDoor.get();
	V[spi] = TRUE;

#if TRUE
	INT32 * Matrix_Category_Lable=pMD->Matrix_Category_Lable_InDoor.get();
	HistData180 *hist=pMD->Histogram180.get();
	if(DFS_Depth==1){
		Matrix_Category_Lable[spi]=Vertical;
	}else{
		//在天空和立面之间深序遍历
		if ((Matrix_Category_Lable[spi]==Sky)
			&&(SceneDetermine::IsVerticalEnergyFocus(hist[spi],AngLeDivided180)==TRUE)){
				Matrix_Category_Lable[spi]=Vertical;
		}else{
			//保持原来属性
		}
	}

#endif
	if(Matrix_Category_Lable[spi]==Vertical){
		for (int spj=0;spj<pMD->slic_current_num;spj++){
			if((V[spj]==FALSE)
				&&(spi!=spj)
				&&(Matrix_E[spi*pMD->slic_current_num+spj]==TRUE))
			{
				DFS_4Vertical2UporDown(spj);           //对访问的邻接顶点递归调用
			}
		}
	}

#if TRUE
	DFS_Depth--;
#endif

}
/*------------------------------------------------------------------------------------------------------------*/
/**
*立面聚类
*/
/*------------------------------------------------------------------------------------------------------------*/
void ComputeSVG::DFS_4Vertical2UporDown_ByColor(int spi)
{
#if TRUE
	static UINT32 DFS_Depth=0;
	DFS_Depth++;
	for (int i=DFS_Depth;i>=0;i--){
		TRACE("-");
	}
	TRACE("%d\n ", spi);   //打印顶点，也可以其他操作
#endif
	SP_PROPERTY *SpProperty=pMD->p_SpProperty;
	UINT32* V=pMD->Matrix_Visit.get();
	UINT32* Matrix_E=pMD->Matrix_E_InDoor.get();
	V[spi] = TRUE;
	static  LabColor rootColor; 
#if TRUE
	INT32 * Matrix_Category_Lable=pMD->Matrix_Category_Lable_InDoor.get();
	HistData180 *hist=pMD->Histogram180.get();
	
	if(DFS_Depth==1){
		rootColor=SpProperty[spi].lab_color;
	}else{
		;
	}
#endif
	if(Matrix_Category_Lable[spi]==Vertical){
		for (int spj=0;spj<pMD->slic_current_num;spj++){
			if((V[spj]==FALSE)
				&&(spi!=spj)
				&&(Matrix_E[spi*pMD->slic_current_num+spj]==TRUE)){

				TRACE(" %d&&%dAB_LIKKE:  ",spi,spj);
				bool IsNear=cui_GeneralImgProcess::IsLabClorGroundNear(SpProperty[spj].lab_color,rootColor,1.2,0.95);
				if (IsNear)	{
					Matrix_Category_Lable[spj]=Vertical;
					DFS_4Vertical2UporDown(spj);
					//对访问的邻接顶点递归调用
				}
				
			}
		}
	}

#if TRUE
	DFS_Depth--;
#endif

}
/*------------------------------------------------------------------------------------------------------------*/
/**
*立面聚类
*/
/*------------------------------------------------------------------------------------------------------------*/
void ComputeSVG::ForceVertical_InDoor_DFS(void)
{
	UINT32* LinkConn=pMD->LinkConnection.get();
	UINT32* V=pMD->Matrix_Visit.get();
	memset(V,FALSE,sizeof(UINT32)*pMD->slic_current_num);
	this->InitDFSLinkConnection_Vertical();
#if IN_DOOR
	HoughAnalyze ha(pMD);	 
	ha.GetHist180All();
	
	for (int spi=0;spi<pMD->slic_current_num;spi++){
		/*深序遍历*/
		if ( (LinkConn[spi]==TRUE)			
			  &&(V[spi]==FALSE)){
				  TRACE("深序遍历Root=%d\n",spi);
				this->DFS_4Vertical2UporDown(spi);
		}
	}
#endif
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*地面从下向上聚类
*@param pMD 
*/
/*------------------------------------------------------------------------------------------------------------*/
void ComputeSVG::ForceGround_InDoor_DFS(ImageMemData* pMD)
{
	UINT32* LinkConn=pMD->LinkConnection.get();
	UINT32* V=pMD->Matrix_Visit.get();
	InitDFSLinkConnection_Ground(pMD);
	memset(V,FALSE,sizeof(UINT32)*pMD->slic_current_num);
#if IN_DOOR	
	for (int spi=0;spi<pMD->slic_current_num;spi++){
		/*深序遍历*/
		if ( (LinkConn[spi]==TRUE)			
			  &&(V[spi]==FALSE)){
				  TRACE("深序遍历Root=%d\n",spi);
				memset(V,FALSE,sizeof(UINT32)*pMD->slic_current_num);
				DFS_4Ground2Up(spi,pMD);
		}

	}
	INT32 * Matrix_Category_Lable=pMD->Matrix_Category_Lable_InDoor.get();
	for (int spj=0;spj<pMD->slic_current_num;spj++){
		if (Matrix_Category_Lable[spj]==GroundPending){
			Matrix_Category_Lable[spj]=Ground;
		}else if(Matrix_Category_Lable[spj]==Ground){
			Matrix_Category_Lable[spj]=Vertical;
		}else{
			;
		}
	}
#endif
}
/*-------------------------------------------------------------------*/
/**
*去掉不连续的天空
*/
/*-------------------------------------------------------------------*/
void ComputeSVG::ForceSky_InDoor_DFS(void)
{
	UINT32* LinkConn=pMD->LinkConnection.get();
	UINT32* V=pMD->Matrix_Visit.get();
	this->InitDFSLinkConnection_Sky();
	memset(V,FALSE,sizeof(UINT32)*pMD->slic_current_num);
#if IN_DOOR	
	for (int spi=0;spi<pMD->slic_current_num;spi++){
		/*深序遍历*/
		if ( (LinkConn[spi]==TRUE)			
			&&(V[spi]==FALSE)){
				TRACE("深序遍历Root=%d\n",spi);
				this->DFS_4Sky2Down(spi);
		}

	}

	INT32 * Matrix_Category_Lable=pMD->Matrix_Category_Lable_InDoor.get();
	for (int spj=0;spj<pMD->slic_current_num;spj++){
		if (Matrix_Category_Lable[spj]==SkyPending){
			Matrix_Category_Lable[spj]=Sky;
		}else if(Matrix_Category_Lable[spj]==Sky){
			Matrix_Category_Lable[spj]=Vertical;
		}else{
			;
		}
	}
#endif

}
/*------------------------------------------------------------------------------------------------------------*/
/**
*从天空向下聚类
*@param spi 天空聚类起始条件
*/
/*------------------------------------------------------------------------------------------------------------*/
void ComputeSVG::DFS_4Sky2Down(int spi)
{
#if TRUE
	static UINT32 DFS_Depth=0;
	DFS_Depth++;
	for (int i=DFS_Depth;i>=0;i--){
		TRACE("-");
	}
	TRACE("%d\n ", spi);   //打印顶点，也可以其他操作
#endif	
	UINT32* Matrix_E=pMD->Matrix_E_InDoor.get();
	INT32 * Matrix_Category_Lable=pMD->Matrix_Category_Lable_InDoor.get();
	UINT32* V=pMD->Matrix_Visit.get();
	SP_PROPERTY *SpProperty=pMD->p_SpProperty;
	INT32* PositionCategoryForce=pMD->PositionCategoryForce.get();

	V[spi] = TRUE;

	if (DFS_Depth==1){
		Matrix_Category_Lable[spi]=SkyPending;
	}

	for (int spj=0;spj<pMD->slic_current_num;spj++){
		if((V[spj]==FALSE)
			&&(spi!=spj)
			&&(Matrix_E[spi*pMD->slic_current_num+spj]==TRUE))
		{
			if ((Matrix_Category_Lable[spj]==Sky)
				||(Matrix_Category_Lable[spj]==SkyPending))
			{				
					Matrix_Category_Lable[spj]=SkyPending;
					DFS_4Sky2Down(spj); //对访问的邻接顶点递归调用
			}



		}
	}

#if TRUE
	DFS_Depth--;
#endif
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*计算立面聚类的连接矩阵
*/
/*------------------------------------------------------------------------------------------------------------*/
void ComputeSVG::InitDFSLinkConnection_Vertical(void)
{
	UINT32* LinkConn=pMD->LinkConnection.get();
	INT32 * Matrix_Category_Lable=pMD->Matrix_Category_Lable_InDoor.get();
	UINT32* Matrix_E=pMD->Matrix_E_InDoor.get();
	/*初始化连接矩阵*/
	memset(LinkConn,0,sizeof(UINT32)*pMD->slic_current_num);
	/*发现E矩阵*/
	
	for (int spi=0;spi<pMD->slic_current_num;spi++){
		for (int spj=0;spj<pMD->slic_current_num;spj++){
			if (  (spi!=spj)
				&&(Matrix_E[spi*pMD->slic_current_num+spj]!=0)
				&&(Matrix_Category_Lable[spi]==Vertical)
				&&(Matrix_Category_Lable[spj]!=Vertical)){
					/*相邻块 一个是 立面另一个不是*/
					LinkConn[spi]=TRUE;
					break;

			}
		}
	}
	this->InitSPLabProperty();
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*计算地面聚类的起始条件
*/
/*------------------------------------------------------------------------------------------------------------*/
void ComputeSVG::InitDFSLinkConnection_Ground(ImageMemData* pMD)
{
	UINT32* LinkConn=pMD->LinkConnection.get();
	INT32 * Matrix_Category_Lable=pMD->Matrix_Category_Lable_InDoor.get();
	UINT32* Matrix_E=pMD->Matrix_E_InDoor.get();
	INT32* PositionCategoryForce=pMD->PositionCategoryForce.get();
//	InitSPLabProperty();
	/*初始化连接矩阵*/
	memset(LinkConn,0,sizeof(UINT32)*pMD->slic_current_num);
	/*发现E矩阵*/
#if IN_DOOR
	for (int spi=0;spi<pMD->slic_current_num;spi++){
		
			if ( (PositionCategoryForce[spi]==GROUND_Force)
				  &&(Matrix_Category_Lable[spi]==Ground)
				){				
					LinkConn[spi]=TRUE;
			 }
		
	}
#endif
	ASSERT(pMD->PgOffset>=0&&pMD->PgOffset<=0.5);
	float Hg=pMD->PgOffset*pMD->ImgHeight+pMD->Seg_HorizontalLinePos;
	cui_GeneralImgProcess::CalculateAllSpPropertyPostitonByHLine(
		pMD->ImgLables.get(),
		pMD->ImgWidth,
		pMD->ImgHeight,
		pMD->p_SpProperty,
		pMD->slic_current_num,
		Hg);
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*初始化天空聚类的起始条件
*/
/*------------------------------------------------------------------------------------------------------------*/
void ComputeSVG::InitDFSLinkConnection_Sky(void)
{
	UINT32* LinkConn=pMD->LinkConnection.get();
	INT32 * Matrix_Category_Lable=pMD->Matrix_Category_Lable_InDoor.get();
	UINT32* Matrix_E=pMD->Matrix_E_InDoor.get();
	INT32* PositionCategoryForce=pMD->PositionCategoryForce.get();

	/*初始化连接矩阵*/
	memset(LinkConn,FALSE,sizeof(UINT32)*pMD->slic_current_num);
	/*发现E矩阵*/
#if IN_DOOR
	for (int spi=0;spi<pMD->slic_current_num;spi++){

		if ( (PositionCategoryForce[spi]==SKY_Force)
			&&(Matrix_Category_Lable[spi]==Sky)
			){				
				LinkConn[spi]=TRUE;
		}

	}
#endif
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*初始化超像素的
*/
/*------------------------------------------------------------------------------------------------------------*/
void ComputeSVG::InitSPForceCategory(void)
{
#if IN_DOOR
	/*Matrix_E 全局不变**只需做一次*/
	cui_GeneralImgProcess::CuiSetNighbour_E_matrix(
		pMD->ImgLables.get(),
		pMD->slic_current_num,
		pMD->Matrix_E_InDoor.get(),
		pMD->ImgWidth,pMD->ImgHeight,
		"InDoor","InDoor");
#endif
	
#if IN_DOOR
	INT32* PositionCategoryForce=pMD->PositionCategoryForce.get();
	double horizontal_line=pMD->ImgHeight*pMD->Seg_HorizontalLinePosScale;
	int*   ImgLables=pMD->ImgLables.get();
	memset(PositionCategoryForce,0,sizeof(INT32)*pMD->slic_current_num);
	
	for(register int hi=0;hi<cui_Height;hi++){
		for (register int j=0;j<cui_Width;j++){	

			int sp= ImgLables[hi*cui_Width+j];//行号就是超像素标号
			if (hi==0){
				PositionCategoryForce[sp]|=SKY_Force;//类别
			}else if (hi==cui_Height-1){
				PositionCategoryForce[sp]|=GROUND_Force;
			}if (hi==horizontal_line){
				//cui_G_V_S_Num_Category[G_V_S_Num_Category*sp+5]=Category_Force[sp]|=VERTICAL_Force;
			}

		}

	}
#endif
}
/*-------------------------------------------------------------------*/
/**
*计算从地面向上的聚类
*@param spi 地面聚类起始图块
*@param pMD 
*
*/
/*-------------------------------------------------------------------*/
void ComputeSVG::DFS_4Ground2Up(int spi,ImageMemData* pMD)
{
#if TRUE
	static UINT32 DFS_Depth=0;
	DFS_Depth++;
	for (int i=DFS_Depth;i>=0;i--){
		TRACE("-");
	}
	TRACE("%d\n ", spi);   //打印顶点，也可以其他操作
#endif	
	UINT32* Matrix_E=pMD->Matrix_E_InDoor.get();
	INT32 * Matrix_Category_Lable=pMD->Matrix_Category_Lable_InDoor.get();
	UINT32* V=pMD->Matrix_Visit.get();
	SP_PROPERTY *SpProperty=pMD->p_SpProperty;
	INT32* PositionCategoryForce=pMD->PositionCategoryForce.get();
	static  LabColor rootColor; 
	 
		V[spi] = TRUE;

		if (DFS_Depth==1){
			Matrix_Category_Lable[spi]=GroundPending;
			rootColor=SpProperty[spi].lab_color;
		}

		for (int spj=0;spj<pMD->slic_current_num;spj++){
			if((V[spj]==FALSE)
				&&(spi!=spj)
				&&(Matrix_E[spi*pMD->slic_current_num+spj]==TRUE))
			{
				if (
#if FALSE
					(Matrix_Category_Lable[spj]==Ground)
					||(Matrix_Category_Lable[spj]==GroundPending)
#else
					(Matrix_Category_Lable[spi]==Ground)||(Matrix_Category_Lable[spi]==GroundPending)
					 &&(Matrix_Category_Lable[spj]!=Sky)
					 &&(SpProperty[spj].IS_UnderHorizontalLine==SpPos_UnderHorizontalLine)					
#endif
					
					)
				{
						TRACE(" %d&&%dAB_LIKKE:  ",spi,spj);
						bool IsNear=cui_GeneralImgProcess::IsLabClorGroundNear(SpProperty[spi].lab_color,SpProperty[spj].lab_color,1.2,0.95);
						if (IsNear){
							//与底块相似
							Matrix_Category_Lable[spj]=GroundPending;
							DFS_4Ground2Up(spj,pMD); //对访问的邻接顶点递归调用
						}else{
							/*与底块不相似*/
							/**检测如果相邻--附上标志位，检测标志位定地面*/							
							/*Matrix_Category_Lable[spj]=Vertical;*/
						}
				}
				
				
				
			}
		}

#if TRUE
	DFS_Depth--;
#endif
}

/*-------------------------------------------------------------------*/
/**
*初始化超像素图块的Lab特性
*@deprecated
*@see void ImageMemData::SplitImgBGRALab(void)
*/
/*-------------------------------------------------------------------*/
void ComputeSVG::InitSPLabProperty(void){
 /**************************************************/
	cui_GeneralImgProcess::CalculateAllSpPropertyRange(
		pMD->ImgLables.get(),
		pMD->ImgWidth,
		pMD->ImgHeight,
		pMD->p_SpProperty,
		pMD->slic_current_num);
 /**************************************************/
   SP_PROPERTY *SpProperty=pMD->p_SpProperty;
   int* ImgLables=pMD->ImgLables.get();
   for (int spi=0;spi<pMD->slic_current_num;spi++){
	   int SM=SpProperty[spi].IsInit_SquareMeter;
	   SpProperty[spi].lab_color.L_Color=0;
	   SpProperty[spi].lab_color.A_Color=0;
	   SpProperty[spi].lab_color.B_color=0;
   }
  /**************************************************/
	for(register int hi=0;hi<cui_Height;hi++){
		for (register int j=0;j<cui_Width;j++){	

			int sp=ImgLables[hi*cui_Width+j];//行号就是超像素标号

			SpProperty[sp].lab_color.L_Color+=cvGetReal2D(h_l_plane,hi,j);
			SpProperty[sp].lab_color.A_Color+=cvGetReal2D(s_a_plane,hi,j);
			SpProperty[sp].lab_color.B_color+=cvGetReal2D(v_b_plane,hi,j);

		}

	}
	/**************************************************/
	for (int spi=0;spi<pMD->slic_current_num;spi++){
		int SM=SpProperty[spi].IsInit_SquareMeter;
		SpProperty[spi].lab_color.L_Color/=SM;
		SpProperty[spi].lab_color.A_Color=SpProperty[spi].lab_color.A_Color/SM-127;
		SpProperty[spi].lab_color.B_color=SpProperty[spi].lab_color.B_color/SM-127;
	}
	/**************************************************/
}
/*-------------------------------------------------------------------*/
/**
*除去在立面上的地面
*@deprecated 弃用
*@see void cui_GeneralImgProcess::FillHoleOnSVGLables()
*/
/*-------------------------------------------------------------------*/
void  ComputeSVG::DFS_RemoveVonG(int spi){
#if TRUE
	static UINT32 DFS_Depth=0;
	DFS_Depth++;
	for (int i=DFS_Depth;i>=0;i--){
		TRACE("-");
	}
	TRACE("%d\n ", spi);   //打印顶点，也可以其他操作
#endif	
	UINT32* Matrix_E=pMD->Matrix_E_InDoor.get();
	INT32 * Matrix_Category_Lable=pMD->Matrix_Category_Lable_InDoor.get();
	UINT32* V=pMD->Matrix_Visit.get();
	SP_PROPERTY *SpProperty=pMD->p_SpProperty;
	INT32* PositionCategoryForce=pMD->PositionCategoryForce.get();


	V[spi] = TRUE;
		
	Matrix_Category_Lable[spi]=VerticalPending;
		


	for (int spj=0;spj<pMD->slic_current_num;spj++){
		if((V[spj]==FALSE)
			&&(spi!=spj)
			&&(Matrix_E[spi*pMD->slic_current_num+spj]==TRUE))
		{
			if ((Matrix_Category_Lable[spj]==Vertical))
			{
				//Matrix_Category_Lable[spj]=VerticalPending;
				DFS_RemoveVonG(spj);
			}
			
		}
	}

#if TRUE
	DFS_Depth--;
#endif
}
/*-------------------------------------------------------------------*/
/**
*移除在悬浮在地面上的立面
*@deprecated 弃用
*@see void cui_GeneralImgProcess::FillHoleOnSVGLables()
*@note 必须有天空，否则无法工作
*/
/*-------------------------------------------------------------------*/
void  ComputeSVG::RemoveSunpendVonG_MustHaveS(void){
	UINT32* LinkConn=pMD->LinkConnection.get();
	INT32 * Matrix_Category_Lable=pMD->Matrix_Category_Lable_InDoor.get();
	UINT32* Matrix_E=pMD->Matrix_E_InDoor.get();
	/*初始化连接矩阵*/
	memset(LinkConn,0,sizeof(UINT32)*pMD->slic_current_num);
	if (IsLabelsHaveSky()==false){
		return;
	}
	/********************************************************************************/
	for (register int spi=0;spi<pMD->slic_current_num;spi++){		 
		if (Matrix_Category_Lable[spi]==Vertical){
			for (register int spj=0;spj<pMD->slic_current_num;spj++){

				if ((0!=Matrix_E[spi*pMD->slic_current_num+spj])&&(spi!=spj)){
					if (Matrix_Category_Lable[spj]==Sky){
						LinkConn[spi]=TRUE;
					}else if(Matrix_Category_Lable[spj]==Vertical){
						;

					}else if (Matrix_Category_Lable[spj]==Ground){
						;
					}else{
						ASSERT(0);//不应当命中
					}
					break;
				}

			}

		}	
	}
	/********************************************************************************/
#if IN_DOOR||OUT_DOOR
	UINT32* V=pMD->Matrix_Visit.get();
	memset(V,FALSE,sizeof(UINT32)*pMD->slic_current_num);
	for (int spi=0;spi<pMD->slic_current_num;spi++){
		/*深序遍历*/
		if ( (LinkConn[spi]==TRUE)			
			&&(V[spi]==FALSE)){
				TRACE("深序遍历Root=%d\n",spi);
				DFS_RemoveVonG(spi);
		}
	}
	/********************************************************************/
	for (int spj=0;spj<pMD->slic_current_num;spj++){
		if (Matrix_Category_Lable[spj]==VerticalPending){
			Matrix_Category_Lable[spj]=Vertical;
		}else if(Matrix_Category_Lable[spj]==Vertical){
			Matrix_Category_Lable[spj]=Ground;
		}else{
			;
		}
	}
#endif
	/********************************************************************************/

}
/*-------------------------------------------------------------------*/
/**
*判断全图是否有天空
*@retval true 有天空
*@retval false 无天空
*/
/*-------------------------------------------------------------------*/
bool ComputeSVG::IsLabelsHaveSky(void)
{
	
	INT32 * Matrix_Category_Lable=pMD->Matrix_Category_Lable_InDoor.get();
	/********************************************************************/
	for (register int spi=0;spi<pMD->slic_current_num;spi++){		 
		if (Matrix_Category_Lable[spi]==Sky){
			return true;
		}
	}
	return false;
}
/*-------------------------------------------------------------------*/
/**
*计算整个超像素图块的权重，通过Y轴上的投影
*/
/*-------------------------------------------------------------------*/
void ComputeSVG::CalcuateSpWeightByY(void)
{
#if IN_DOOR
	cui_GeneralImgProcess::CalculateAllSpPropertyRange(
		pMD->ImgLables.get(),
		pMD->ImgWidth,
		pMD->ImgHeight,
		p_SpProperty,
		pMD->slic_current_num);

		for (int spi=0;spi<pMD->slic_current_num;spi++){

				cui_G_V_S_Num_Category[G_V_S_Num_Category*spi+0]=0;	//G
				cui_G_V_S_Num_Category[G_V_S_Num_Category*spi+1]=0;	//V
				cui_G_V_S_Num_Category[G_V_S_Num_Category*spi+2]=0;	//S

				for (register int y=p_SpProperty[spi].min_y;y<p_SpProperty[spi].max_y;y++){

						cui_G_V_S_Num_Category[G_V_S_Num_Category*spi+0]+=cui_pYweight_GVS[y*3+0];	//G
						cui_G_V_S_Num_Category[G_V_S_Num_Category*spi+1]+=cui_pYweight_GVS[y*3+1];	//V
						cui_G_V_S_Num_Category[G_V_S_Num_Category*spi+2]+=cui_pYweight_GVS[y*3+2];	//S

				}
		}		
#endif
}
/*-------------------------------------------------------------------*/
/**
*地面，模糊计算
*@param x Y轴像素
*@param n 
*@param posHor 
*@param Pg 
*@param Height 图像高度
*@return x像素位置的权重
*/
/*-------------------------------------------------------------------*/
double ComputeSVG::Gx_InDoor(int x,int n,float posHor,float Pg,int Height)
{
	double Pn=1.0*(2*n+2)/(2*n+1);
#if 0
double C_g=Pn/Pow_Even(posHor*Height-Pg*Height,Pn);
double y=C_g*Pow_Odd(x-Pg,1.0/(2*n+1));
#else
	double C_g=Pn/Pow_Even(Height-posHor*Height+Pg*Height,Pn);
	double y=-1*C_g*Pow_Odd(x+Pg*Height,1.0/(2*n+1));
#endif
	ASSERT(Pg>=-1&&Pg<=1);
	//ASSERT(posHor>=-1&&posHor<=1);
	return y;
}
/*-------------------------------------------------------------------*/
/**
*天空，模糊计算
*@param x Y轴像素
*@param n 
*@param posHor 
*@param Pg 
*@param Height 图像高度
*@return x像素位置权重
*/
/*-------------------------------------------------------------------*/
double ComputeSVG::Sx_InDoor(int x,int n,float posHor,float Ps,int Height)
{
	double Pn=1.0*(2*n+2)/(2*n+1);
#if 0
	double C_g=Pn/Pow_Even(posHor*Height-Height+Ps*Height,Pn);	
	double y=-1*C_g*Pow_Odd(x+Ps,1.0/(2*n+1));
#else
	double C_g=Pn/Pow_Even(posHor*Height-Ps*Height,Pn);	
	double y=C_g*Pow_Odd(x-Ps*Height,1.0/(2*n+1));
#endif
	

	ASSERT(Ps>=-1&&Ps<=1);
//	ASSERT(posHor>=-1&&posHor<=1);
	return y;
}
/*-------------------------------------------------------------------*/
/**
*计算b的W次方
*
*/
/*-------------------------------------------------------------------*/
double ComputeSVG::Pow_Odd(double b,double w)
{
	//奇函数
	double r=0;
	if (b<0){
		r=-1*powl(-1*b,w);
	}else{
		r= powl(b,w);
	}
	return r;
}
/*-------------------------------------------------------------------*/
/**
*计算b的W次方
*
*/
/*-------------------------------------------------------------------*/
double ComputeSVG::Pow_Even(double b,double w)
{
	double r=0;
	if (b<0){
		r= powl(-1*b,w);
	}else{
		r= powl(b,w);
	}
	return r;
}
/*-------------------------------------------------------------------*/
/**
*在像素层面\n
*立面的像素
*/
/*-------------------------------------------------------------------*/
void ComputeSVG::VerticalMustHaveSupport_Pixel(void)
{
	INT32* ImgLables=pMD->p_ImgLables_SVG;
	for (int x=0;x<pMD->ImgWidth;x++){
		bool findV=false;
		for (int y=0;y<pMD->Seg_HorizontalLinePos;y++){
			int index=y*pMD->ImgWidth+x;
			if (index<pMD->ImgWidth*pMD->ImgHeight){
				if (ImgLables[index]==Vertical){
					findV=true;
				}
				if (findV==true){
					ImgLables[index]=Vertical;
				}
			}
				
		}
	}

}
/*-------------------------------------------------------------------*/
/**
*在像素层面\n
*立面的像素
*/
/*-------------------------------------------------------------------*/
void ComputeSVG::VerticalMustHaveSupport_SpPixel(void)
{
#if 0
	INT32* ImgLables=pMD->p_ImgLables_SVG;

	for (int x=0;x<pMD->ImgWidth;x++){
		bool findV=false;
		for (int y=0;y<pMD->Seg_HorizontalLinePos;y++){
			int index=y*pMD->ImgWidth+x;
			if (index<pMD->ImgWidth*pMD->ImgHeight){
				if (ImgLables[index]==Vertical){
					findV=true;
				}
				if (findV==true){
					ImgLables[index]=Vertical;
				}
			}

		}
	}
#endif
	static bool DoCalculate=true;
	if (DoCalculate){
		cui_GeneralImgProcess::CalculateAllSpPropertyRange(
			pMD->ImgLables.get(),
			pMD->ImgWidth,
			pMD->ImgHeight,
			pMD->p_SpProperty,
			pMD->slic_current_num);
		DoCalculate=false;
	}
	

	SP_PROPERTY *SpProperty=pMD->p_SpProperty;
	int VerticalYpos;
	int *Lables=pMD->ImgLables.get();
	int *Matrix_Category_Lable=pMD->Matrix_Category_Lable_InDoor.get();
	for (int x=0;x<pMD->ImgWidth;x++){
		bool findV=false;
		for (int y=0;y<pMD->Seg_HorizontalLinePos;y++){
			int index=y*pMD->ImgWidth+x;
			ASSERT(index<pMD->ImgWidth*pMD->ImgHeight);
			int SpCategory=Matrix_Category_Lable[Lables[index]];
				if (SpCategory==Vertical){
					findV=true;
					VerticalYpos=(SpProperty[Lables[index]].max_y+SpProperty[Lables[index]].min_y)/2;
				}
				if ((findV==true)
					&&(Matrix_Category_Lable[Lables[index]]==Sky)){
						int TempVerticalPos=(SpProperty[Lables[index]].max_y+SpProperty[Lables[index]].min_y)/2;
						if (TempVerticalPos>VerticalYpos){
							  Matrix_Category_Lable[Lables[index]]=Vertical;
						}
					
				}
		}
	}

}
/*-------------------------------------------------------------------*/
/**
*水平线之上必须是立面
*/
/*-------------------------------------------------------------------*/
void ComputeSVG::OverHorLineMustVertical(void)
{
	INT32* ImgLables=pMD->p_ImgLables_SVG;

	for (int x=0;x<pMD->ImgWidth;x++){
		bool findV=false;
		for (int y=0;y<pMD->Seg_HorizontalLinePos;y++){
			int index=y*pMD->ImgWidth+x;
			if (index<pMD->ImgWidth*pMD->ImgHeight){
				if (ImgLables[index]==Ground){
				ImgLables[index]=Vertical;	
				}				
			}

		}
	}

}
/*-------------------------------------------------------------------*/
/**
*天空聚类初始化
*
*/
/*-------------------------------------------------------------------*/
void ComputeSVG::DecideFloatingSkySP2V_Init(void)
{
	UINT32* LinkConn=pMD->LinkConnection.get();
	INT32 * Matrix_Category_Lable=pMD->Matrix_Category_Lable_InDoor.get();
	UINT32* Matrix_E=pMD->Matrix_E_InDoor.get();
	INT32* PositionCategoryForce=pMD->PositionCategoryForce.get();
	memset(LinkConn,FALSE,sizeof(UINT32)*pMD->slic_current_num);
	HoughAnalyze ha(pMD);	 
	ha.GetHist180All();
	HistData180 *hist=pMD->Histogram180.get();
	unsigned long SpSize=pMD->GetSpSizeAvg();
	SP_PROPERTY *SpProperty=pMD->p_SpProperty;
	for (int spi=0;spi<pMD->slic_current_num;spi++){
		if ((Matrix_Category_Lable[spi]==Sky)
			&&(SceneDetermine::IsSpRectangle(hist[spi],AngLeDivided180)==true)
			&&(SpProperty[spi].IsInit_SquareMeter>=1.5*SpSize)
			){
				Matrix_Category_Lable[spi]=Vertical;
#if IN_DOOR
				LinkConn[spi]=TRUE;
#endif
		}
	}

	

}
/*-------------------------------------------------------------------*/
/**
*深序遍历\n
*由最相似天空进行天空的聚类
*/
/*-------------------------------------------------------------------*/
void ComputeSVG::DecideFloatingSky_InDoor_DFS(void)
{
	UINT32* LinkConn=pMD->LinkConnection.get();
	INT32 * Matrix_Category_Lable=pMD->Matrix_Category_Lable_InDoor.get();
	UINT32* V=pMD->Matrix_Visit.get();
	this->DecideFloatingSkySP2V_Init();
	memset(V,FALSE,sizeof(UINT32)*pMD->slic_current_num);
#if IN_DOOR&&0	
	for (int spi=0;spi<pMD->slic_current_num;spi++){
		/*深序遍历*/
		if ( (LinkConn[spi]==TRUE)			
			&&(V[spi]==FALSE)){
				TRACE("深序遍历Root=%d\n",spi);
				this->DFS_4FloatingSky2Down(spi);
		}

	}

	
#endif
}
/*-------------------------------------------------------------------*/
/**
*深序遍历\n
*由最相似天空进行天空的聚类
*/
/*-------------------------------------------------------------------*/
void ComputeSVG::DFS_4FloatingSky2Down(int spi)
{
#if TRUE
	static UINT32 DFS_Depth=0;
	DFS_Depth++;
	for (int i=DFS_Depth;i>=0;i--){
		TRACE("-");
	}
	TRACE("%d\n ", spi);   //打印顶点，也可以其他操作
#endif	
	UINT32* Matrix_E=pMD->Matrix_E_InDoor.get();
	INT32 * Matrix_Category_Lable=pMD->Matrix_Category_Lable_InDoor.get();
	UINT32* V=pMD->Matrix_Visit.get();
	SP_PROPERTY *SpProperty=pMD->p_SpProperty;
	INT32* PositionCategoryForce=pMD->PositionCategoryForce.get();

	V[spi] = TRUE;

	if (DFS_Depth==1){
		ASSERT(Matrix_Category_Lable[spi]==Vertical);
	}

	int HeihtPos_I=SpProperty[spi].max_y;
	for (int spj=0;spj<pMD->slic_current_num;spj++){
		if((V[spj]==FALSE)
			&&(spi!=spj)
			&&(Matrix_E[spi*pMD->slic_current_num+spj]==TRUE))
		{
			int HeihtPos_J=SpProperty[spj].max_y;
			if ((Matrix_Category_Lable[spj]==Sky)
				&&(HeihtPos_J>=HeihtPos_I)
				&&(TRUE))
			{
				Matrix_Category_Lable[spj]=Vertical;
				DFS_4FloatingSky2Down(spj); //对访问的邻接顶点递归调用
			}
		}
	}
#if TRUE
	DFS_Depth--;
#endif
}
/*-------------------------------------------------------------------*/
/**
*将穿过视平线的图块算作地面
*/
/*-------------------------------------------------------------------*/
void ComputeSVG::CrossEyeLineIsVertical(void)
{
	int ImgWidth=pMD->ImgWidth;
	int ImgHeight=pMD->ImgHeight;
	int SlicNum=pMD->slic_current_num;
	INT32* Matrix_Category_Lable=pMD->Matrix_Category_Lable_InDoor.get();
	/*************************************************************************************************/
	for(register int hi=0;hi<ImgHeight;hi++){
		for (register int j=0;j<ImgWidth;j++){
			int sp=cui_ImgLables[hi*ImgWidth+j];//行号就是超像素标号
			if (hi==pMD->Seg_HorizontalLinePos){
				Matrix_Category_Lable[sp]=Vertical;
			}

		}

	}
#if 0
/*************************************************************************************************/
	//穿过视平线的地面即为立面 (穿越面积在一定比例之上)
	for (register int sp=0;sp<SlicNum;sp++){
		if (Matrix_Category_Lable[sp]==VERTICAL_Force){
				{
					float UpSquare,DownSquare;
					this->CalculateUpandDownSquarebyHor(&UpSquare,&DownSquare,sp);
					if ((UpSquare/(UpSquare+DownSquare)>0.1)){
						//分割上边沿大于0.1设为立面
						Matrix_Category_Lable[sp]=Vertical;
					}
			 }

		}
	}
	/****************************************************************************************************/
#endif
	

}
/*-------------------------------------------------------------------*/
/**
*获得所有的超像素图块的直线直方图\n
*并判断每一个图块是否是条状\n
*对于条状图块，将其归类为立面块
*@note   
*/
/*-------------------------------------------------------------------*/
UINT32 ComputeSVG::StripIsVertical(void)
{
	static bool DoOnce=false;
	UINT32 StripVerticalNum=0;
	int ImgWidth=pMD->ImgWidth;
	int ImgHeight=pMD->ImgHeight;
	int SlicNum=pMD->slic_current_num;
	INT32* Matrix_Category_Lable=pMD->Matrix_Category_Lable_InDoor.get();
	/********************************************************************/
	cui_GeneralImgProcess::CalculateAllSpPropertyPostitonByHLine(
		pMD->ImgLables.get(),
		pMD->ImgWidth,
		pMD->ImgHeight,
		pMD->p_SpProperty,
		pMD->slic_current_num,
		pMD->Seg_HorizontalLinePos);
	HoughAnalyze ha(pMD);	 
	if (DoOnce==false){
		ha.GetHist180All();
		DoOnce=true;
	}
	
	HistData180 *hist=pMD->Histogram180.get();
	/*********************************************************************/
	SP_PROPERTY* SpProperty=pMD->p_SpProperty;
	for (int i=0;i<pMD->slic_current_num;i++){
#if 0
if ((SpProperty[i].IS_UnderHorizontalLine==SpPos_UpHorizontalLine)
			||(SpProperty[i].IS_UnderHorizontalLine==SpPos_CrossHorizontalLine))
#endif
			{
				ManhattanHistogram mht(hist[i],180);
				if (mht.IsContourstrip()){
					Matrix_Category_Lable[i]=Vertical;
					StripVerticalNum++;
				}
				
			}
	}
	/*********************************************************************/
#if TRUE
//this->VerticalMustHaveSupport_SpPixel();
#endif
	return StripVerticalNum;
}
/*-------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------*/
double ComputeSVG::CalculateVGDifference(int spi)
{
#if 1
	double Pg=cui_G_V_S_Num_Category[G_V_S_Num_Category*spi+0];//G
	double Pv=cui_G_V_S_Num_Category[G_V_S_Num_Category*spi+1];//V
	double Ps=cui_G_V_S_Num_Category[G_V_S_Num_Category*spi+2];//S
#endif
		double De=fabs(Pv-Pg)/fabs(std::max(Pv,Pg));
		return De;
}
/*-------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------*/

