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
*�ٲ��ô�ͳ��ͼ��ָ�����㷨(�磺�������׾����)��ͼ��ָ��һ���ܶȺʹ�С��ͼ�飻\n
*�ڼ���ÿ������ͼ��ƽ��ɫ������ֵ����������ͶӰ��abƽ���ϣ�\n
*�ۼ���ÿ������ͼ��ƽ��ɫ������ֵͶӰ��abƽ����������ģ������\n
*�ܸ���������ģ��������ڲ�ͬ�Ĳ�ȿռ䣬��ͼ4��ʾ��\n
*�ݶ�����ͼ�������ʽ(1)����������ļнǵļ��㣻\n
*���Թ�ʽ(1)(2)(3)Ϊ�оݣ�������������ͼ����о��ࣻ\n
*���ظ�2��5����ֱ��������\n
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
*�������ھ���
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
*����ɫ�ʸ�֪ģ�ͼ������ƾ���
*
*
*/
/*---------------------------------------------------------------------------*/
void LabColorAnalyzeCluster::GetMatrix_W(void)
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
*@return �������ƾ�������ֵ  
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
*@ note IMG_4095.jpg ---�컨����治��ͨ -4.89
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
 *IMG_4095.jpg ---�컨����治��ͨ -4.89
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
*IMG_4095.jpg ---�컨����治��ͨ -4.89
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
*����������ɫ֮������ƶ�
*@param ci I����ɫ 
*@param cj J����ɫ 
*@retval true ����ɫ���� 
*@retval false ����ɫ������ 
*@note
*    CIELABɫ�ʿռ��е���ɫ������abƽ����ͶӰģ������������ɫ���Ӿ������ϵ�ɫ�ʱ���\n
*�ȣ���CIELABɫ�ʿռ��е���ɫ������abƽ����ͶӰģ��Խ���������֪����ɫ��Խ���ޣ���\n
*ʱ������������֪��Ҫ����������������ֵ���ɫ������������ϵ�����������нϴ�ı�\n
*����Χ���������ɫ�ʱ��ͶȲ���ʱ������ɫ������abƽ����ͶӰģ���϶�ʱ������������\n
*ʶ����Ҫ������������������ȱ仯������ɫ����������ɫ�ʶ�߶�ͼ��֪ģ�����ǻ�������\n
*���Ӿ��ڲ�ͬ��ɫ�ʱ��Ͷ�����¶�������ɫ���ֲ��ò�ͬ�ĳ߶ȣ��Լ����������ɫ�ʵ���\n
*֪��Ҫͨ������淴����׵Ĳ����������ֱ�ԭ���������ġ���ɫ�ʶ�߶�ͼ��֪ģ���У���\n
*����ɫ�ʱ��ͶȽϸ�ʱ������ɫ������abƽ����ͶӰ��ģ������ ʱ������ɫ����Ľ����Բ��\n
*�ж����ȵ����ƺ�С������ģ��֮�� �����ȴ���һ���̶ȵ�Լ����������ɫ������abƽ����Ͷ\n
*Ӱģ��С�� ʱ����ɫ����Ľ����Բ���в����мн�Լ����������ֱ�������������ȵ�Լ����\n
*�Ӷ�����˱��ͶȲ�������£���ɫ���������ֶȡ�\n
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