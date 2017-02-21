#include "StdAfx.h"
#include "modules.h"
#include "LabColorAnalyzeCluster_Veins.h"
/*---------------------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------------------*/
LabColorAnalyzeCluster_Veins::LabColorAnalyzeCluster_Veins(ImageMemData* MemData_t)
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
LabColorAnalyzeCluster_Veins::~LabColorAnalyzeCluster_Veins(void)
{

}
/*---------------------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------------------*/
void LabColorAnalyzeCluster_Veins::ColorPointIteration(void)
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
	pMD->SaveColorIterationNum();
}
/*---------------------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------------------*/
void LabColorAnalyzeCluster_Veins::ColorPointIteration_Veins(void)
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
		this->GetMatrix_W_Veins();
#if IN_DOOR
		UINT32* Matrix_E=pMD->Matrix_E_InDoor.get();
		double* Matrix_W=pMD->Matrix_W_InDoor.get();
#endif
#if OUT_DOOR
		UINT32* Matrix_E=pMD->Matrix_E.get();
		double* Matrix_W=pMD->Matrix_W_Color;
#endif	
		this->AdjustNighbour_W(Matrix_W,pMD->Matrix_W_Vein,Matrix_W,pMD->slic_current_num,
			0.6,
			Iteration__Threshold_Vein_GND,
			1,
			1,
			pMD->p_SpProperty);

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
	pMD->SaveColorIterationNum();
}
/*---------------------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------------------*/
void LabColorAnalyzeCluster_Veins::GetMatrix_E(void)
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
*
*
*
*/
/*---------------------------------------------------------------------------*/
void LabColorAnalyzeCluster_Veins::GetMatrix_W(void)
{	
	/*�������ƾ���*/
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
				if (LabIsSameColor(SpProperty[i].lab_color,SpProperty[j].lab_color))
				{
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
*
*
*/
/*---------------------------------------------------------------------------*/
double LabColorAnalyzeCluster_Veins::Find_MaxSimilar(void)
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
 bool LabColorAnalyzeCluster_Veins::ColorNearWhiteBlack(LabColor c)
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
 float LabColorAnalyzeCluster_Veins::LabDistanceAB(LabColor ci,LabColor cj)
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
 float LabColorAnalyzeCluster_Veins::LabDistanceLAB(LabColor ci,LabColor cj)
 {
	 float labdst=sqrtl(pow(ci.A_Color-cj.A_Color,2)+pow(ci.B_color-cj.B_color,2)+pow(ci.L_Color-cj.L_Color,2));
	 ASSERT(labdst<=442&&labdst>=0);
	 return labdst;
 }
 /*---------------------------------------------------------------------------*/
 /**
 *
 *IMG_4095.jpg ---�컨����治��ͨ -4.89
 *
 */
 /*---------------------------------------------------------------------------*/
bool LabColorAnalyzeCluster_Veins::LabIsSameColorInOut(LabColor ci,LabColor cj,bool Enable)
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
 *IMG_4095.jpg ---�컨����治��ͨ -4.89
 *
 */
 /*---------------------------------------------------------------------------*/
bool LabColorAnalyzeCluster_Veins::LabIsSameColorIn(LabColor ci,LabColor cj,bool Enable)
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
*IMG_4095.jpg ---�컨����治��ͨ -4.89
*
*/
/*---------------------------------------------------------------------------*/
bool LabColorAnalyzeCluster_Veins::LabIsSameColorOut(LabColor ci,LabColor cj,bool Enable)
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
 *
 *
 *
 */
 /*---------------------------------------------------------------------------*/
bool LabColorAnalyzeCluster_Veins::LabIsSameColor(LabColor ci,LabColor cj)
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
	// true ==i,j, �⻷
	//false ==i,j  һ���ڻ�
	if (IsDoubleOut==true){
		//ͬʱ���⻷
		return LabIsSameColorOut(ci,cj,true);
		
	}else if (IsDoubleIn==true){
		//ͬʱ���ڻ�
	   return LabIsSameColorIn(ci,cj,true);
	
	}else{
		////��һ�����ڻ�,һ�����⻷
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
 float LabColorAnalyzeCluster_Veins::LabDistanceABangleIn(LabColor ci,LabColor cj)
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
 float LabColorAnalyzeCluster_Veins::LabDistanceABangleOut(LabColor ci,LabColor cj)
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
 float LabColorAnalyzeCluster_Veins::LabDistanceABangleInOut(LabColor ci,LabColor cj)
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
 float LabColorAnalyzeCluster_Veins::LabDistanceLABangle(LabColor ci,LabColor cj)
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
 void LabColorAnalyzeCluster_Veins::GetMatrix_W_Veins(void)
 {
	 cui_GeneralImgProcess::CalculateAllSpPropertyRange(pMD->ImgLables.get(),pMD->ImgWidth,pMD->ImgHeight,pMD->p_SpProperty,pMD->slic_current_num);
	 cui_GeneralImgProcess::CalculateAllSpBlockEnergy(pMD->slic_current_num,pMD->p_SpProperty,pMD->Src_ImgData,pMD->ImgLables.get(),pMD->ImgWidth,pMD->ImgHeight);
	 cui_GeneralImgProcess::CalculateAllSpPropertyPostitonByHLine(pMD->ImgLables.get(),pMD->ImgWidth,pMD->ImgHeight,pMD->p_SpProperty,pMD->slic_current_num,pMD->Seg_HorizontalLinePos);
	 cui_GeneralImgProcess::CuiSetNighbour_W_Vein_matrix(pMD->Matrix_W_Vein,pMD->slic_current_num,pMD->p_SpProperty,pMD);//����������
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
 void LabColorAnalyzeCluster_Veins::AdjustNighbour_W(
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
/*-------------------------------------------------------------------------------------------*/
/**
*
*
*/
/*-------------------------------------------------------------------------------------------*/