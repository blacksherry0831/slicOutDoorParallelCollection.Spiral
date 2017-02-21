#include "StdAfx.h"
#include "module_all_cui.h"
//#include "SpectralClusterInDoor.h"
/*----------------------------------------------------------------------------------*/
/**
*构造函数\n
*初始化形态谱聚类
*/
/*----------------------------------------------------------------------------------*/
SpectralClusterInDoor::SpectralClusterInDoor(ImageMemData* pMD_t)
{
	 this->pMD=pMD_t;
	 gray_Color_contour_img=nullptr;
	 storage=cvCreateMemStorage(0);
	 lines=nullptr;
	 gray_binary_img=nullptr;
	 gray_img=nullptr;

}
/*----------------------------------------------------------------------------------*/
/**
*析构函数\n
*释放内存
*/
/*----------------------------------------------------------------------------------*/
SpectralClusterInDoor::~SpectralClusterInDoor(void)
{
	if (gray_Color_contour_img!=nullptr){
		cvReleaseImage(&gray_Color_contour_img);
	}
	if (storage!=nullptr){
		cvReleaseMemStorage(&storage);
	}
	if (gray_img!=nullptr){
		cvReleaseImage(&gray_img);
	}
	if (gray_binary_img!=nullptr){
		cvReleaseImage(&gray_binary_img);
	}
}
/*----------------------------------------------------------------------------------*/
/**
*相邻矩阵
*/
/*----------------------------------------------------------------------------------*/
void SpectralClusterInDoor::GetMatrix_E(void)
{
  cui_GeneralImgProcess::CuiSetNighbour_E_matrix(
	  pMD->ImgLables.get(),
	  pMD->slic_current_num,
	  pMD->Matrix_E_InDoor.get(),
	  pMD->ImgWidth,pMD->ImgHeight,
	  "InDoor","InDoor");
}
/*----------------------------------------------------------------------------------*/
/**
*度矩阵
*/
/*----------------------------------------------------------------------------------*/
void SpectralClusterInDoor::GetMatrix_D(void)
{

	CvMat D_Matrix_t;

	double *Matrix_D=pMD->Matrix_D_InDoor.get();
	double *Matrix_W=pMD->Matrix_W_InDoor.get();
	memset(Matrix_D,0,sizeof(double)*pMD->slic_current_num*pMD->slic_current_num);
	/**************************************/ 		
	for (register int wi=0;wi<pMD->slic_current_num;wi++){
		double sum=0;
		for (register int wj=0;wj<pMD->slic_current_num;wj++){ 
			sum+=Matrix_W[wi*pMD->slic_current_num+wj];
		}
		Matrix_D[wi*pMD->slic_current_num+wi]=sum;
	}	

	/***************************************/
	cvInitMatHeader(&D_Matrix_t,pMD->slic_current_num,pMD->slic_current_num,CV_64FC1, Matrix_D);
	cvInvert(&D_Matrix_t,&D_Matrix_t,CV_SVD);
	cvPow(&D_Matrix_t,&D_Matrix_t,0.5);
}
/*----------------------------------------------------------------------------------*/
/**
*拉普拉斯矩阵
*/
/*----------------------------------------------------------------------------------*/
void SpectralClusterInDoor::GetMatrix_L(void)
{
	//////////////////////////////////////////////////////////////
	double* Matrix_L=pMD->Matrix_L_InDoor.get();
	UINT32* Matrix_E=pMD->Matrix_E_InDoor.get();
	double *Matrix_D=pMD->Matrix_D_InDoor.get();
	double *Matrix_W=pMD->Matrix_W_InDoor.get();
	memset(Matrix_L,0,sizeof(double)*pMD->slic_current_num*pMD->slic_current_num);
	//////////////////////////////////////////////////////////////
	CvMat D_t,W_t,L_t;
#if 0
	assert(sizeof(double)==8);
#endif
	
	cvInitMatHeader(&D_t,pMD->slic_current_num,pMD->slic_current_num,CV_64FC1,Matrix_D);
	cvInitMatHeader(&W_t,pMD->slic_current_num,pMD->slic_current_num,CV_64FC1,Matrix_W);
	cvInitMatHeader(&L_t,pMD->slic_current_num,pMD->slic_current_num,CV_64FC1,Matrix_L); 
#if 0	 
	cvSub(&D_t,&W_t,&L_t); 
#else
	CvMat *I_t,*cui_mat_t;
	cui_mat_t=cvCreateMat(pMD->slic_current_num,pMD->slic_current_num,CV_64FC1);
	I_t=cvCreateMat(pMD->slic_current_num,pMD->slic_current_num,CV_64FC1);
	cvSetIdentity(I_t);
#if _MSC_VER
    cvmMul(&D_t,&W_t,cui_mat_t);
	cvmMul(cui_mat_t,&D_t,&L_t);
#else
     ASSERT(0);
#endif
	

	cvSub(I_t,&L_t,&L_t); 
	cvReleaseMat(&I_t);
	cvReleaseMat(&cui_mat_t);

#endif

	/*************************/

	{
		char data_t[1024];
		ofstream outfile;
		outfile.open("Matrix_L_InDoor.data",ios::out);
		for( int i = 0; i <pMD->slic_current_num; i++ ){
			for( int j = 0; j <pMD->slic_current_num; j++ ){
				double value_t=Matrix_L[i*pMD->slic_current_num+j];
				sprintf(data_t," %0.2e ",value_t);
				outfile<<data_t;
			}
			outfile<<endl;
		} 
		outfile.close();
	}
}
/*----------------------------------------------------------------------------------*/
/**
*相似矩阵
*@deprecated
*/
/*----------------------------------------------------------------------------------*/
void SpectralClusterInDoor::GetMatrix_W(void)
{
#if TRUE
	cui_GeneralImgProcess::CalculateAllSpPropertyRange(
		pMD->ImgLables.get(),
		pMD->ImgWidth,
		pMD->ImgHeight,
		pMD->p_SpProperty,
		pMD->slic_current_num);

	 HoughAnalyze ha(pMD);
	 ha.GetHist180All();
	 HistData180 *hist=pMD->Histogram180.get();
#endif
	/*计算相似矩阵*/
	UINT32* Matrix_E=pMD->Matrix_E_InDoor.get();
	double* Matrix_W=pMD->Matrix_W_InDoor.get();
	memset(Matrix_W,0,sizeof(double)*pMD->slic_current_num*pMD->slic_current_num);
	for(register int i = 0; i <pMD->slic_current_num; i++ ){
		for(register int j = i+1; j <pMD->slic_current_num; j++ ){

			if (0!=(UINT32)Matrix_E[i*pMD->slic_current_num+j]){	

#if 0					
			
#else
				double B_distance=this->Calculate2HistBLikelyByManhattan(hist[i],hist[j],AngLeDivided180);
#endif				
				Matrix_W[j*pMD->slic_current_num+i]= Matrix_W[i*pMD->slic_current_num+j]=B_distance;
			}
		}
	}
	
 cui_GeneralImgProcess::SaveMatrix_W("","Matrix_W_InDoor.data",pMD->slic_current_num,Matrix_W);
}
/*----------------------------------------------------------------------------------*/
/**
*相似矩阵
*@see float Calculate2HistBLikelyByManhattan(ManhattanHistogram histOne,ManhattanHistogram histTwo)
*@note 
*比较两个曼哈顿直方图的相似度\n
*(1)对聚类图像中的所有图块边缘轮廓进行Hough直线提取；\n
*(2)在[0，180]角度区间范围，以 为一区间，将[0，180]区间分为45个小区间，并统计场景图像中各角度区间中\n
*的直线段总长，并归一化后形成统计直方图；\n
*@image html image_MHD_fivePeak.jpg 
*/
/*----------------------------------------------------------------------------------*/
void SpectralClusterInDoor::GetMatrix_W_1_44_84_96_136_180(void)
{
#if TRUE
	cui_GeneralImgProcess::CalculateAllSpPropertyRange(
		pMD->ImgLables.get(),
		pMD->ImgWidth,
		pMD->ImgHeight,
		pMD->p_SpProperty,
		pMD->slic_current_num);

	HoughAnalyze ha(pMD);
	ha.GetHist180All();
	HistData180 *hist=pMD->Histogram180.get();

#endif
	/*计算相似矩阵*/
	UINT32* Matrix_E=pMD->Matrix_E_InDoor.get();
	double* Matrix_W=pMD->Matrix_W_InDoor.get();
	memset(Matrix_W,0,sizeof(double)*pMD->slic_current_num*pMD->slic_current_num);
	for(register int i = 0; i <pMD->slic_current_num; i++ ){
		for(register int j = i+1; j <pMD->slic_current_num; j++ ){

			if (0!=(UINT32)Matrix_E[i*pMD->slic_current_num+j]){	

				//i ,j超像素相邻				

				ManhattanHistogram mh1(hist[i],AngLeDivided180);
				mh1.AdjustTo5Peak();
				ManhattanHistogram mh2(hist[j],AngLeDivided180);
				mh2.AdjustTo5Peak();

				double B_distance=this->Calculate2HistBLikelyByManhattan(mh1,mh2);				
				Matrix_W[j*pMD->slic_current_num+i]= Matrix_W[i*pMD->slic_current_num+j]=B_distance;

			}



		}
	}

	cui_GeneralImgProcess::SaveMatrix_W("","Matrix_W_InDoor.data",pMD->slic_current_num,Matrix_W);
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*计算曼哈顿相似度
*@param histOne  
*@param histTwo  
*@deprecated
*
*/
/*------------------------------------------------------------------------------------------------------------*/
 float SpectralClusterInDoor::Calculate2HistBLikely(
	 HistData180 histOne,
	 HistData180 histTwo,
	 int Dim)
 {
	 double SumHistOne=0;
	 double SumHistTwo=0;
	 double Hist1MutiHist2=0;
	 float Blike=0;
	 for(int i=0;i<Dim;i++){
		 SumHistOne+=pow(histOne.hist_all[i],2);
		 SumHistTwo+=pow(histTwo.hist_all[i],2);
		 Hist1MutiHist2+=histOne.hist_all[i]*histTwo.hist_all[i];

	 }
	 SumHistOne=sqrt(SumHistOne);
	 SumHistTwo=sqrt(SumHistTwo);
	/**/
	 if(SumHistOne*SumHistTwo==0){
		 return 0;
	 }else{
		 Blike= Hist1MutiHist2/(SumHistOne* SumHistTwo);
		 return Blike;
	 }
	
 }
/*------------------------------------------------------------------------------------------------------------*/
/**
*计算Manhattan直方图的相似度
*@param histOne  
*@param histTwo  
*@note 须同为矩形图块
*/
/*------------------------------------------------------------------------------------------------------------*/
 float SpectralClusterInDoor::Calculate2HistBLikely(
	 ManhattanHistogram histOne,
	 ManhattanHistogram histTwo)
 {
	 ASSERT(histOne.HistDim==histTwo.HistDim);
	  int Dim=histOne.HistDim;
	 double SumHistOne=0;
	 double SumHistTwo=0;
	 double Hist1MutiHist2=0;
	 float Blike=0;
	 for(int i=0;i<Dim;i++){
		 SumHistOne+=pow(histOne.hist_all[i],2);
		 SumHistTwo+=pow(histTwo.hist_all[i],2);
		 Hist1MutiHist2+=histOne.hist_all[i]*histTwo.hist_all[i];

	 }
	 SumHistOne=sqrt(SumHistOne);
	 SumHistTwo=sqrt(SumHistTwo);
	 /**/
	 if(SumHistOne*SumHistTwo==0){
		 return 0;
	 }else{
		 Blike= Hist1MutiHist2/(SumHistOne* SumHistTwo);
		 return Blike;
	 }

 }
/*------------------------------------------------------------------------------------------------------------*/
/**
*
*@param histOne  
*@param histTwo  
*
*/
/*------------------------------------------------------------------------------------------------------------*/
 float SpectralClusterInDoor::Calculate2HistBLikelyByManhattan(
	 HistData180 histOne,
	 HistData180 histTwo,
	 int Dim)

 {
#if TRUE
	 if ((SceneDetermine::IsHistRegularShapes(histOne))
		 &&(SceneDetermine::IsHistRegularShapes(histTwo))){

			 return Calculate2HistBLikely(histOne,histTwo,Dim);

	 }else{
		 return 0;
	 }
#else

	 return this->Calculate2HistBLikely(histOne,histTwo,Dim);

#endif
	 
	
	 
	

 }
/*------------------------------------------------------------------------------------------------------------*/
/**
*计算两个Manhattan直方图的相似度
*
*
*/
/*------------------------------------------------------------------------------------------------------------*/
 float SpectralClusterInDoor::Calculate2HistBLikelyByManhattan(
	 ManhattanHistogram histOne,
	 ManhattanHistogram histTwo)
 {
	 ASSERT(histOne.HistDim==histTwo.HistDim);
	 int Dim=histOne.HistDim;
#if TRUE
#endif

	 if ((histOne.IsContourRectangle())
		 &&(histTwo.IsContourRectangle())
		 &&(SceneDetermine:: IsManhattan(histOne))
		  &&(SceneDetermine:: IsManhattan(histTwo))		 
		 ){

			 return Calculate2HistBLikely(histOne,histTwo);

	 }else{
		 return 0;
	 }
		
 }
/*------------------------------------------------------------------------------------------------------------*/
/**
*轮廓形态谱聚类
*@param EigenvectorNumPercent 特征向量个数所占的比例 
*@param ClusterPercent 聚类目标个数 
*@param Threshold 谱聚类阈值 
*
*@note  室内场景中许多人造物体的轮廓在立面上呈现矩形形状，同时人造物体的颜色随意性较大，因此仅以颜色特征对\n
*图块进行聚类是无法实现的，但是基于重力场环境下，立面物体通常需要其他物体的支撑，因此立面上相邻的人造物体在\n
*图像上往往呈现相同的矩形轮廓形态，如书架上摆放的图书、立面上砖墙等。同时根据透视原理矩形的图像通常出现在立\n
*面方向上，而很少能出现在水平方向上出现\n
*
*/
/*------------------------------------------------------------------------------------------------------------*/
 bool SpectralClusterInDoor::Spectral_Clustering_B(
	 double EigenvectorNumPercent,
	 double ClusterPercent,
	 double Threshold)
	{
	 UINT EigenvectorNum=pMD->slic_current_num*EigenvectorNumPercent;
	 UINT ClusterNum=pMD->slic_current_num*ClusterPercent;
	 double	W_Threshold;
	 //10%作为图像聚类特征向量的维度(EigenvectorNumPercent=0.1)
	
	 this->GetMatrix_E();
	 this->GetMatrix_W_1_44_84_96_136_180();
	// this->GetMatrix_W();
	 this->GetMatrix_D();
	 this->GetMatrix_L();
	 this->Get_L_Eigenvalue();	 
/*-------------*/
 if(1){
		 this->B_Cluster(EigenvectorNum,ClusterNum,InDoorThreshold); 
		 cui_GeneralImgProcess::Cui_Combination_ImgLabs2(
			 pMD->Src_ImgData,
			 pMD->ImgLables.get(),
			 pMD->Matrix_Category_Lable_InDoor.get(),
			 pMD->Matrix_W_InDoor.get(),
			 pMD->slic_current_num,
			 pMD->ImgWidth,
			 pMD->ImgHeight,
			 Iteration__Threshold__InDoor,pMD);
		 ///////////////////////////////////////////////////
		 W_Threshold=Iteration__Threshold__InDoor;
	 }
/*--------------*/
#if 1
 do{ 
	 double T_Similar=0;
	 this->GetMatrix_E();
	 this->GetMatrix_W_1_44_84_96_136_180();
	 //this->GetMatrix_W();
	 int * Matrix_Category_Lable=pMD->Matrix_Category_Lable_InDoor.get();
	 memset(Matrix_Category_Lable,0,sizeof(INT32)*pMD->slic_current_num);

	 T_Similar=this->Find_MaxSimilar();
	 if (T_Similar>Iteration__Threshold__InDoor){
		 cui_GeneralImgProcess::Cui_Combination_ImgLabs2(
			 pMD->Src_ImgData,pMD->ImgLables.get(),
			 pMD->Matrix_Category_Lable_InDoor.get(),
			 pMD->Matrix_W_InDoor.get(),
			 pMD->slic_current_num,
			 pMD->ImgWidth,pMD->ImgHeight,
			 Iteration__Threshold__InDoor,pMD);
	 }else{	
#if 0
		 this->Cui_Combination_ImgLabs2(0.71);	//组合时依然使用相似度
		 static int i=0;
		 if (0==++i%2){
			 break;
		 }
#else
		 if (Iteration_Complete_Combine_Threshold){
			 cui_GeneralImgProcess::Cui_Combination_ImgLabs2(
				  pMD->Src_ImgData,pMD->ImgLables.get(),
				  pMD->Matrix_Category_Lable_InDoor.get(),
				  pMD->Matrix_W_InDoor.get(),
				  pMD->slic_current_num,
				  pMD->ImgWidth,pMD->ImgHeight,
				  Iteration__Threshold__InDoor,
				  pMD);
			 break;
		 }else{
			 break;
		 }
#endif

	 }

 }while(1);
#endif
/*--------------*/
#if 1
   cui_GeneralImgProcess::InSideClusteringByopencv(
	   pMD->Src_ImgData,
	   pMD->ImgLables.get(),
	   pMD->slic_current_num,
	   pMD->ImgWidth,pMD->ImgHeight,
	   pMD->FileReadFullPath,pMD->FileWritePath);
#endif
	
	 pMD->SaveShapeSpectralClusteringNum();
	 return false;
 }
/*------------------------------------------------------------------------------------------------------------*/
/**
*计算拉普拉斯矩阵的特征值
*
*
*/
/*------------------------------------------------------------------------------------------------------------*/
void SpectralClusterInDoor::Get_L_Eigenvalue(void)
{
	int CuiNumLabels=pMD->slic_current_num;
	double* Matrix_L=pMD->Matrix_L_InDoor.get();
	/////////////特征向量///////////////////////////////////////////////////////////////////
	float *MatrixEigenVector_L=pMD->MatrixEigenVector_L_InDoor.get();;
	memset(MatrixEigenVector_L,0,sizeof(float)*pMD->slic_current_num*pMD->slic_current_num);
	/////////////特征值///////////////////////////////////////////////////////////////////
	double* MatrixEigenValue_L=pMD->MatrixEigenValue_L_InDoor.get();
	memset(MatrixEigenValue_L,0,sizeof(double)*pMD->slic_current_num);
	////////////////////////////////////////////////////////////////////////////////
	CvMat E_vector_t,E_value_t,L_t;
	cvInitMatHeader(&E_vector_t,CuiNumLabels,CuiNumLabels,CV_32FC1,MatrixEigenVector_L);
	cvInitMatHeader(&E_value_t,CuiNumLabels,1,CV_64FC1,MatrixEigenValue_L);
	cvInitMatHeader(&L_t,CuiNumLabels,CuiNumLabels,CV_64FC1,Matrix_L); 
	cvEigenVV(&L_t,&E_vector_t,&E_value_t);//C是A的特征值(降序排列)，而B则是A的特征向量(每行)
	cvInvert(&E_vector_t,&E_vector_t);
	/******************************************/
	{
		char data_t[1024];
		ofstream outfile;
		outfile.open("Matrix_L_Vector.data",ios::out);
		for(register int i = 0; i <pMD->slic_current_num; i++ ){
			for(register int j = 0; j <pMD->slic_current_num; j++ ){
				double value_t=MatrixEigenVector_L[i*pMD->slic_current_num+j];
				sprintf(data_t," %10.2e ",value_t);
				outfile<<data_t;
			}
			outfile<<endl;
		} 
		outfile.close();
	}
	/******************************************8*/
	{
		char data_t[1024];
		ofstream outfile;
		outfile.open("Matrix_L_Value.data",ios::out);
		for(register int i = 0; i <pMD->slic_current_num; i++ ){

			double value_t=MatrixEigenValue_L[i];
			sprintf(data_t," %0.3e ",value_t);
			outfile<<data_t;		
			outfile<<endl;
		} 
		outfile.close();
	}


}
/*------------------------------------------------------------------------------------------------------------*/
/**
*B氏系数 谱聚类
*
*@param EigenvectorNum   特性相邻个数
*@param ClusterNum 聚类个数  
*@param Threshold B氏聚类阈值  
*@note  
*/
/*------------------------------------------------------------------------------------------------------------*/
void SpectralClusterInDoor::B_Cluster(UINT EigenvectorNum, UINT ClusterNum,double Threshold)
{
	EigenvectorNum=EigenvectorNum>2?EigenvectorNum:2;
	Threshold=Threshold>0.5?Threshold:0.5;
	/**********前K个特征值********************************/	
	 float* Matrix_Category_Simple=pMD->Matrix_Category_Simple_InDoor.get();
   	 memset(Matrix_Category_Simple,0,sizeof(float)*pMD->slic_current_num*EigenvectorNum);
	 float* MatrixEigenVector_L=pMD->MatrixEigenVector_L_InDoor.get();
	 int CuiNumLabels=pMD->slic_current_num;
	for (int i=0;i<pMD->slic_current_num;i++){  //row
		for (UINT j=0;j<EigenvectorNum;j++){	//col
			//N*K						   //N*N
			Matrix_Category_Simple[EigenvectorNum*i+j]=MatrixEigenVector_L[CuiNumLabels*i+j+CuiNumLabels-1-EigenvectorNum];

		}
	}

#if 1
	{
		char data_t[1024];
		memset(data_t,0,sizeof(data_t));
		ofstream outfile;
		outfile.open("B_Sample.dat",ios::out);
		for (int i=0;i<CuiNumLabels;i++){
			for (UINT j=0;j<EigenvectorNum;j++){
				//N*K						   //N*N

				double value_t_t=Matrix_Category_Simple[EigenvectorNum*i+j];
				sprintf(data_t,"%5.2f ",value_t_t);
				outfile<<data_t;

			}
			outfile<<endl;
		} 

		outfile.close();
	}
#endif
	for( int i = 0; i <CuiNumLabels; i++ ){
		CvMat normalize_t;
		cvInitMatHeader(&normalize_t,1,EigenvectorNum,CV_32FC1,&Matrix_Category_Simple[i*EigenvectorNum]);
		cvNormalize(&normalize_t,&normalize_t,1,0,CV_L1);	//a,b无用	
	} 
#if 1

	{
		char data_t[1024];
		ofstream outfile;
		outfile.open("B_sample_Normalize.data",ios::out);
		for( int i = 0; i <CuiNumLabels; i++ ){
			for( UINT j = 0; j <EigenvectorNum; j++ ){
				double value_t=Matrix_Category_Simple[i*EigenvectorNum+j];
				sprintf(data_t," %5.2f ",value_t);
				outfile<<data_t;
			}
			outfile<<endl;
		} 
		outfile.close();
	}
#endif	
	/************清空Lable****************************************/
	double* Matrix_W=pMD->Matrix_W_InDoor.get();
	INT32*  Matrix_Category_Lable=pMD->Matrix_Category_Lable_InDoor.get(); 
	memset(Matrix_Category_Lable,0xffffffff,sizeof(int)*CuiNumLabels-1);
	/****************************************************************/
	double  B_like_t=0;
	INT32  Cui_lable_t[MAX_SP_NUM ];
	memset(Cui_lable_t,-1,MAX_SP_NUM *sizeof(INT32));
	for (int i=0;i<CuiNumLabels;i++){
		for (int j=i+1;j<CuiNumLabels;j++){
			/****判断超像素i,j相似度*category=i行j行相似度**/
			if (Matrix_W[i*CuiNumLabels+j]>0){
				B_like_t=0;        	
				for (UINT col=0;col<EigenvectorNum;col++){
					//计算前K个特征值相似度
					B_like_t+=sqrtl(fabsl( Matrix_Category_Simple[EigenvectorNum*i+col]*Matrix_Category_Simple[EigenvectorNum*j+col])); 	  
				}
				if (Matrix_Category_Lable[i]==0xffffffff){
					Matrix_Category_Lable[i]=i;
				}
				if (B_like_t>Threshold){
					//相似度大于阈值，归属同一类
					double wenli_like=cui_GeneralImgProcess::CalculateSpSimilar(i,j,pMD->p_SpProperty);
					if(wenli_like>0.95){
						Matrix_Category_Lable[j]=Matrix_Category_Lable[i];
					}

				}
				memcpy(Cui_lable_t,Matrix_Category_Lable,sizeof(INT32)*CuiNumLabels);       	   	

				/*********************************************/
			}
		}
	}

#if 1
	{
		char data_t[1024];
		ofstream outfile;
		outfile.open("B_Lable.data",ios::out);
		for( int i = 0; i <CuiNumLabels; i++ ){

			INT32 value_t=Matrix_Category_Lable[i];
			sprintf(data_t,"%3d ",value_t);
			outfile<<data_t;
			outfile<<endl;
		}

	}
#endif	
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*
*绘制直方图
*
*/
/*------------------------------------------------------------------------------------------------------------*/
void SpectralClusterInDoor::DrawHistogramOnImg(void)
{

}
/*------------------------------------------------------------------------------------------------------------*/
/**
*查找相似矩阵内最大的相似值\n
*
*@note  
*@see ::Spectral_Clustering_B 
*/
/*------------------------------------------------------------------------------------------------------------*/
double SpectralClusterInDoor::Find_MaxSimilar(void)
{

	double Max_Similar=0;
	double *Matrix_W=pMD->Matrix_W_InDoor.get();

	for (register int spi=0;spi<pMD->slic_current_num;spi++){
		for (register int spj=spi+1;spj<pMD->slic_current_num;spj++){

			if (Matrix_W[spi*pMD->slic_current_num+spj]>=Max_Similar){
				Max_Similar=Matrix_W[spi*pMD->slic_current_num+spj];

			}
		}

	}
	return Max_Similar;
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*
*
*
*/
/*------------------------------------------------------------------------------------------------------------*/
