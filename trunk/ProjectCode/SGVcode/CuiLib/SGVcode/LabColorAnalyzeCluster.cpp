#include "StdAfx.h"
#include "module_all_cui.h"
//#include "LabColorAnalyzeCluster.h"
/*---------------------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------------------*/
LabColorAnalyzeCluster::LabColorAnalyzeCluster(ImageMemData* MemData_t)
{
	this->pMD=MemData_t;
}
/*---------------------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------------------*/
LabColorAnalyzeCluster::~LabColorAnalyzeCluster(void)
{

}
/*---------------------------------------------------------------------------*/
/**
*①采用传统的图像分割聚类算法(如：超像素谱聚类等)将图像分割成一定密度和大小的图块；\n
*②计算每个聚类图块平均色彩向量值，并将向量投影到ab平面上；\n
*③计算每个聚类图块平均色彩向量值投影在ab平面上向量的模长，并\n
*④根据向量的模长将其归于不同的测度空间，如图4所示；\n
*⑤对相邻图块类采用式(1)进行向量间的夹角的计算；\n
*⑥以公式(1)(2)(3)为判据，将符合条件的图块进行聚类；\n
*⑦重复2～5步，直到收敛。\n
*
*
*
*/
/*----------------------------------------------------------------------------*/
void LabColorAnalyzeCluster::ColorPointIteration(void)
{
#if IN_DOOR
int * Matrix_Category_Lable=pMD->Matrix_Category_Lable_InDoor.get();
#endif
#if OUT_DOOR
int * Matrix_Category_Lable=pMD->Matrix_Category_Lable.get();
#endif
	memset(Matrix_Category_Lable,0,sizeof(INT32)*pMD->slic_current_num);
	do{ 

		double T_Similar=0;
		this->GetMatrix_E();
		this->GetMatrix_W();
		T_Similar=this->Find_MaxSimilar();

		if (T_Similar>0.5){
#if IN_DOOR
cui_GeneralImgProcess::Cui_Combination_ImgLabs2(
				pMD->Src_ImgData,
				pMD->ImgLables.get(),
				pMD->Matrix_Category_Lable_InDoor.get(),
				pMD->Matrix_W_InDoor.get(),
				pMD->slic_current_num,
				pMD->ImgWidth,
				pMD->ImgHeight,
				0.5,
				pMD);
#endif
#if OUT_DOOR
cui_GeneralImgProcess::Cui_Combination_ImgLabs2(
	pMD->Src_ImgData,
	pMD->ImgLables.get(),
	pMD->Matrix_Category_Lable.get(),
	pMD->Matrix_W_Color,
	pMD->slic_current_num,
	pMD->ImgWidth,
	pMD->ImgHeight,
	0.5,
	pMD);
#endif
		}else{
				break;
		}

	}while(TRUE);

#if TRUE
	cui_GeneralImgProcess::InSideClusteringByopencv(
		pMD->Src_ImgData,
		pMD->ImgLables.get(),
		pMD->slic_current_num,
		pMD->ImgWidth,
		pMD->ImgHeight,
		pMD->FileReadFullPath,
		pMD->FileWritePath);
#endif
	cui_GeneralImgProcess::CuiSaveImgWithContours(
		pMD->Src_ImgData,
		pMD->ImgLables.get(),
		pMD->ImgWidth,
		pMD->ImgHeight,
		pMD->FileReadFullPath,
		pMD->FileWritePath,
		"ColorCluster");
	pMD->DrawContours();
	///////////////////////////////////////////////
	pMD->SaveColorIterationNum();
}

/*---------------------------------------------------------------------------*/
/**
*计算相邻矩阵
*
*
*/
/*----------------------------------------------------------------------------*/
void LabColorAnalyzeCluster::GetMatrix_E(void)
{
#if IN_DOOR
 cui_GeneralImgProcess::CuiSetNighbour_E_matrix(
	  pMD->ImgLables.get(),
	  pMD->slic_current_num,
	  pMD->Matrix_E_InDoor.get(),
	  pMD->ImgWidth,
	  pMD->ImgHeight,
	  "InDoor",
	  "InDoor");
}
#endif
#if OUT_DOOR
cui_GeneralImgProcess::CuiSetNighbour_E_matrix(
	pMD->ImgLables.get(),
	pMD->slic_current_num,
	pMD->Matrix_E.get(),
	pMD->ImgWidth,
	pMD->ImgHeight,
	"OutDoor",
	"OutDoor");
}
#endif
/*---------------------------------------------------------------------------*/
/**
*根据色彩感知模型计算相似矩阵
*
*
*/
/*---------------------------------------------------------------------------*/
void LabColorAnalyzeCluster::GetMatrix_W(void)
{
	/*计算相似矩阵*/
	pMD->InitSPLabProperty();
	SP_PROPERTY* SpProperty=pMD->p_SpProperty;
#if IN_DOOR
	UINT32* Matrix_E=pMD->Matrix_E_InDoor.get();
	double* Matrix_W=pMD->Matrix_W_InDoor.get();
#endif
#if OUT_DOOR
	UINT32* Matrix_E=pMD->Matrix_E.get();
	double* Matrix_W=pMD->Matrix_W_Color;
#endif	
	memset(Matrix_W,0,sizeof(double)*pMD->slic_current_num*pMD->slic_current_num);
	for(register int i = 0; i <pMD->slic_current_num; i++ ){
		for(register int j = i+1; j <pMD->slic_current_num; j++ ){

			if (0!=(UINT32)Matrix_E[i*pMD->slic_current_num+j]){	
				
				double Wij;
				if (LabIsSameColor(SpProperty[i].lab_color,SpProperty[j].lab_color)){
					Wij=1;
				}else{
					Wij=0;
				}
				
				
				Matrix_W[j*pMD->slic_current_num+i]= Matrix_W[i*pMD->slic_current_num+j]=Wij;
			}
		}
	}
	
	 cui_GeneralImgProcess::SaveMatrix_W("","Matrix_W_SpColor.data",pMD->slic_current_num,Matrix_W);
}
/*---------------------------------------------------------------------------*/
/**
*
*@return 返回相似矩阵的最大值  
*
*/
/*---------------------------------------------------------------------------*/
double LabColorAnalyzeCluster::Find_MaxSimilar(void)
{

	double Max_Similar=0;
#if IN_DOOR
double *Matrix_W=pMD->Matrix_W_InDoor.get();
#endif
#if OUT_DOOR
double *Matrix_W=pMD->Matrix_W_Color;
#endif	

	for (register int spi=0;spi<pMD->slic_current_num;spi++){
		for (register int spj=spi+1;spj<pMD->slic_current_num;spj++){

			if (Matrix_W[spi*pMD->slic_current_num+spj]>=Max_Similar){
				Max_Similar=Matrix_W[spi*pMD->slic_current_num+spj];

			}
		}

	}
	return Max_Similar;
}
/*---------------------------------------------------------------------------*/
/**
*
*
*
*/
/*---------------------------------------------------------------------------*/
 bool LabColorAnalyzeCluster::ColorNearWhiteBlack(LabColor c)
{
	ASSERT(c.A_Color>=-127&&c.A_Color<=128);
	ASSERT(c.B_color>=-127&&c.B_color<=128);
	float Length2ORG=sqrtl(pow(c.A_Color,2)+pow(c.B_color,2));
	if (Length2ORG<=CylinderBlackWhite){
		return true;
	}else{
		return  false;
	}
	
}
 /*---------------------------------------------------------------------------*/
 /**
 *
 *
 *
 */
 /*---------------------------------------------------------------------------*/
 float LabColorAnalyzeCluster::LabDistanceAB(LabColor ci,LabColor cj)
 {
	 float abdst=sqrtl(pow(ci.A_Color-cj.A_Color,2)+pow(ci.B_color-cj.B_color,2));
	 ASSERT(abdst<=361&&abdst>=0);
	 return abdst;
 }
/*---------------------------------------------------------------------------*/
/**
*
*
*
*/
/*---------------------------------------------------------------------------*/
 float LabColorAnalyzeCluster::LabDistanceLAB(LabColor ci,LabColor cj)
 {
	 float labdst=sqrtl(pow(ci.A_Color-cj.A_Color,2)+pow(ci.B_color-cj.B_color,2)+pow(ci.L_Color-cj.L_Color,2));
	 ASSERT(labdst<=442&&labdst>=0);
	 return labdst;
 }
/*---------------------------------------------------------------------------*/
/**
*
*
*@ note IMG_4095.jpg ---天花板地面不穿通 -4.89
*
*/
/*---------------------------------------------------------------------------*/
bool LabColorAnalyzeCluster::LabIsSameColorInOut(LabColor ci,LabColor cj,bool Enable)
 {
	 if (LabDistanceLAB(ci,cj)>=8){
	 	return false;
	 }
	 float DstLab=LabDistanceLAB(ci,cj);
	 if (DstLab<=3){
		 return true;
	 }
   if (Enable){
	   float DstAngle=LabDistanceABangleInOut(ci,cj);
	   if(DstAngle<=ABInCircleDst){
		   return true; 
	   }
   }
	 return false;
 }
 /*---------------------------------------------------------------------------*/
 /**
 *
 *IMG_4095.jpg ---天花板地面不穿通 -4.89
 *
 */
 /*---------------------------------------------------------------------------*/
bool LabColorAnalyzeCluster::LabIsSameColorIn(LabColor ci,LabColor cj,bool Enable)
 {
	 float DstLab=LabDistanceLAB(ci,cj);
	 if (DstLab<=3){
		 return true;
	 }
    if (Enable){
		 float DstAngle=LabDistanceABangleIn(ci,cj);
			 if(DstAngle<=ABInCircleDst){
	 			return true; 
			 }
    }
	
	 return false;
 }
/*---------------------------------------------------------------------------*/
/**
*
*IMG_4095.jpg ---天花板地面不穿通 -4.89
*
*/
/*---------------------------------------------------------------------------*/
bool LabColorAnalyzeCluster::LabIsSameColorOut(LabColor ci,LabColor cj,bool Enable)
 {
	 float DstLab=LabDistanceLAB(ci,cj);
	 if (DstLab<=3){
		 return true;
	 }
	 if (Enable){
		 float DstAngle=LabDistanceABangleOut(ci,cj);
		 if(DstAngle<=ABOutCircleDst){
			 return true; 
		 }
	 }	
	 return false;
 }
/*---------------------------------------------------------------------------*/
/**
*计算两个颜色之间的相似度
*@param ci I号颜色 
*@param cj J号颜色 
*@retval true 两颜色相似 
*@retval false 两颜色不相似 
*@note
*    CIELAB色彩空间中的颜色向量在ab平面上投影模长，表征着颜色在视觉感受上的色彩饱和\n
*度，即CIELAB色彩空间中的颜色向量在ab平面上投影模长越长则人类感知的颜色将越鲜艳，此\n
*时人类对物体的认知主要依靠物体表面所呈现的颜色，而物体表面上的亮度则可以有较大的变\n
*化范围。当物体的色彩饱和度不足时，即颜色向量在ab平面上投影模长较短时，人类对物体的\n
*识别主要依靠物体表面明暗亮度变化而非颜色。本发明的色彩多尺度图感知模型正是基于了人\n
*类视觉在不同的色彩饱和度情况下对物体颜色区分采用不同的尺度，以及人类对物体色彩的认\n
*知主要通过其表面反射光谱的波长差异来分辨原理构建起来的。在色彩多尺度图感知模型中，当\n
*物体色彩饱和度较高时，即颜色向量在ab平面上投影的模长大于 时，其颜色聚类的近似性测度\n
*中对亮度的限制很小，仅有模长之差 对亮度存在一定程度的约束；而当颜色向量在ab平面上投\n
*影模长小于 时，颜色聚类的近似性测度中不仅有夹角约束条件，还直接增加了其亮度的约束，\n
*从而提高了饱和度不足情况下，颜色聚类间的区分度。\n
*
*
*
*/
/*---------------------------------------------------------------------------*/
bool LabColorAnalyzeCluster::LabIsSameColor(LabColor ci,LabColor cj)
{	
#if 0
 if((ColorNearWhiteBlack(ci)==false)
		 &&(ColorNearWhiteBlack(cj)==false)){

	       if (LabDistanceABangle(ci,cj)<ABOutCircleDst){
			   return true;
	       }else{
			   return false;
		   }


	 }else{
		 return LabIsSameColor(ci,cj);
	 }
#endif
#if TRUE
	bool IsDoubleOut=(ColorNearWhiteBlack(ci)==false)&&(ColorNearWhiteBlack(cj)==false);
	bool IsDoubleIn=(ColorNearWhiteBlack(ci)==true)&&(ColorNearWhiteBlack(cj)==true);
	// true ==i,j, 外环
	//false ==i,j  一个内环
	if (IsDoubleOut==true){
		//同时在外环
		return LabIsSameColorOut(ci,cj,true);
		
	}else if (IsDoubleIn==true){
		//同时在内环
	   return LabIsSameColorIn(ci,cj,true);
	
	}else{
		////有一个在内环,一个在外环
	   return LabIsSameColorInOut(ci,cj,true);
	
	}
#endif
	
}
/*-------------------------------------------------------------------------------------------*/
/**
*
*
*/
/*-------------------------------------------------------------------------------------------*/
 float LabColorAnalyzeCluster::LabDistanceABangleIn(LabColor ci,LabColor cj)
{
	ASSERT(ci.A_Color>=-127&&ci.A_Color<=128);
	ASSERT(cj.B_color>=-127&&cj.B_color<=128);
	float I_Length=sqrtl(pow(ci.A_Color,2)+pow(ci.B_color,2));
	ASSERT(I_Length!=0);
	float J_Length=sqrtl(pow(cj.A_Color,2)+pow(cj.B_color,2));
	ASSERT(J_Length!=0);
	float Vlength=fabs(I_Length-J_Length);
	if (Vlength>=VectorLengthDeltaIn){
		return 180;
	}
	float LDst=fabsl(ci.L_Color-cj.L_Color);

	if (LDst>= L_InLightDstDelta){
		return 180;
	}
	float IJ_DotProduct=ci.A_Color*cj.A_Color+ci.B_color*cj.B_color;
	float CosIJ=IJ_DotProduct/(I_Length*J_Length);
	if (CosIJ>1){
		CosIJ-=1E-6F;
	}
	if (CosIJ<-1){
		CosIJ+=1E-6F;
	}
	ASSERT(CosIJ>=-1&&CosIJ<=1);
	float Angle=acos(CosIJ)*180/CV_PI;
	ASSERT(Angle>=0&&Angle<=181);
	return Angle;
}
  /*-------------------------------------------------------------------------------------------*/
 /**
 *
 *
 */
 /*-------------------------------------------------------------------------------------------*/
 float LabColorAnalyzeCluster::LabDistanceABangleOut(LabColor ci,LabColor cj)
{
	ASSERT(ci.A_Color>=-127&&ci.A_Color<=128);
	ASSERT(cj.B_color>=-127&&cj.B_color<=128);
	float I_Length=sqrtl(pow(ci.A_Color,2)+pow(ci.B_color,2));
	ASSERT(I_Length!=0);
	float J_Length=sqrtl(pow(cj.A_Color,2)+pow(cj.B_color,2));
	ASSERT(J_Length!=0);
	float Vlength=fabs(I_Length-J_Length);
	if (Vlength>=VectorLengthDeltaOut){
		return 180;
	}
	float LDst=fabsl(ci.L_Color-cj.L_Color);

	if (LDst>= L_OutLightDstDelta){
		return 180;
	}
	float IJ_DotProduct=ci.A_Color*cj.A_Color+ci.B_color*cj.B_color;
	float CosIJ=IJ_DotProduct/(I_Length*J_Length)-1E-6F;
	ASSERT(CosIJ>=-1&&CosIJ<=1);
	float Angle=acos(CosIJ)*180/CV_PI;
	ASSERT(Angle>=0&&Angle<=181);
	return Angle;
}
 /*-------------------------------------------------------------------------------------------*/
 /**
 *
 *
 */
 /*-------------------------------------------------------------------------------------------*/
 float LabColorAnalyzeCluster::LabDistanceABangleInOut(LabColor ci,LabColor cj)
 {

	ASSERT(ci.A_Color>=-127&&ci.A_Color<=128);
	ASSERT(cj.B_color>=-127&&cj.B_color<=128);
	float I_Length=sqrtl(pow(ci.A_Color,2)+pow(ci.B_color,2));
	ASSERT(I_Length!=0);
	float J_Length=sqrtl(pow(cj.A_Color,2)+pow(cj.B_color,2));
	ASSERT(J_Length!=0);
	float Vlength=fabs(I_Length-J_Length);
	if (Vlength>=VectorLengthDeltaOut){
		return 180;
	}
	
	float LDst=fabsl(ci.L_Color-cj.L_Color);
	
		if (LDst>= L_OutLightDstDelta){
			return 180;
		}
	
	float IJ_DotProduct=ci.A_Color*cj.A_Color+ci.B_color*cj.B_color;
	float CosIJ=IJ_DotProduct/(I_Length*J_Length)-1E-6F;
	ASSERT(CosIJ>=-1&&CosIJ<=1);
	float Angle=acos(CosIJ)*180/CV_PI;
	ASSERT(Angle>=0&&Angle<=181);
	return Angle;
 }
/*-------------------------------------------------------------------------------------------*/
/**
*
*
*/
/*-------------------------------------------------------------------------------------------*/
 float LabColorAnalyzeCluster::LabDistanceLABangle(LabColor ci,LabColor cj)
 {
	ASSERT(ci.A_Color>=-127&&ci.A_Color<=128);
	ASSERT(cj.B_color>=-127&&cj.B_color<=128);
	float I_Length=sqrtl(pow(ci.A_Color,2)+pow(ci.B_color,2)+pow(ci.L_Color,2));
	ASSERT(I_Length!=0);
	float J_Length=sqrtl(pow(cj.A_Color,2)+pow(cj.B_color,2)+pow(cj.L_Color,2));
	ASSERT(J_Length!=0);
	
	float IJ_DotProduct=ci.A_Color*cj.A_Color+ci.B_color*cj.B_color+ci.L_Color*cj.L_Color;
	float CosIJ=IJ_DotProduct/(I_Length*J_Length);
	ASSERT(CosIJ>=-1&&CosIJ<=1);
	float Angle=acos(CosIJ)*180/CV_PI;
	return  Angle;
 }
 /*-------------------------------------------------------------------------------------------*/
 /**
 *
 *
 */
 /*-------------------------------------------------------------------------------------------*/