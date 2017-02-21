#include "StdAfx.h"
/*------------------------------*/
#include "module_all_cui.h"
#include "ComputeSVG2.h"
#include "cui_GeneralImgProcess.h"
//#include <algorithm>
///////////////////////////////////////////////
#define  WeightZoom (10000)
////////////////////////////////////////////////
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
/*--------------------------------------------------------------------------------------------------------*/
/**
*构造函数，完成变量初始化
*导入用于计算的中间变量
*/
/*-------------------------------------------------------------------------------------------------------*/
ComputeSVG2::ComputeSVG2(ImageData* MemData_t)
{
	TRACE_FUNC();
	this->pIMD=MemData_t;
	pIMD->GetMatrixE();
	Lab2Point_AB_Dst.resize(pIMD->slic_current_num);
	Lab_L_Dst.resize(pIMD->slic_current_num);
	Lab2Zero_L_Dst.resize(pIMD->slic_current_num);
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*初始化成员变量
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG2::InitParam(void)
{
   
	
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*构造函数，完成变量初始化（已弃用）
*/
/*-------------------------------------------------------------------------------------------------------*/
ComputeSVG2::ComputeSVG2(void)
{
	this->InitParam();
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*析构函数 释放内存
*/
/*-------------------------------------------------------------------------------------------------------*/
ComputeSVG2::~ComputeSVG2(void)
{

}
/*--------------------------------------------------------------------------------------------------------*/
/**
*清理计算过程中分配的内存
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG2::ClearMemory(void)
{
	
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*导入计算用的图像数据
*
*@param ImgData  图像数据
*@param Width    图像宽度
*@param Height   图像高度
*@param Lables   超像素的图块分类信息
*@param pIMD->slic_current_num 超像素的个数
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG2::FillWeightArrayZlmParallel(void)
{

	pIMD->FillWeightArrayZlmParallel();	
#if SaveContours2Disk
	pIMD->SaveImgWithPointsCompute("fuzzy");
#endif
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG2::FillWeightArrayZlm(void)
{
	TRACE_FUNC();
    float horizontal_line=pIMD->Seg_HorizontalLinePosScale*pIMD->ImgHeight;
	this->FillWeightArrayG_InDoor20150603(horizontal_line,1);
	this->FillWeightArrayS_InDoor20150603(horizontal_line,1);
	this->FillWeightArrayV_New(horizontal_line,1);	
#if _DEBUG
	if (cui_GeneralImgProcess::SAVE_DEBUG_2DISK==TRUE){
		char data_t[1024];																			
		ofstream outfile;								   
		outfile.open("Matrix_Weight_GVS_zlm_cuda.data",ios::out);
		for( int i = 0; i <pIMD->ImgHeight; i++ ){
			{
				double value_t=i;
				sprintf_s(data_t,1024," %0.2e ",value_t);
				outfile<<data_t;		
			}
			{
				double value_t=pIMD->pYweight_G[i];
				sprintf_s(data_t,1024," %0.2e ",value_t);
				outfile<<data_t;										
			}
			{
				double value_t=pIMD->pYweight_V[i];
				sprintf_s(data_t,1024," %0.2e ",value_t);
				outfile<<data_t;										
			}
			{
				double value_t=pIMD->pYweight_S[i];
				sprintf_s(data_t,1024," %0.2e ",value_t);
				outfile<<data_t;										
			}
			outfile<<endl;			 
		} 
		outfile.close();
	}
#endif
	 vector<int>	fuzzyCategory(pIMD->slic_current_num);

	this->CalculateSpClassification(fuzzyCategory.data());
	for (register int spi=0;spi<pIMD->slic_current_num;spi++){
		pIMD->SpSet.SpPropertySet[spi].fuzzyCategory=fuzzyCategory[spi];
		pIMD->SpSet.SpPropertySet[spi].ComputeCategory=fuzzyCategory[spi];
	}
#if SaveContours2Disk
	pIMD->SaveImgWithPointsCompute("fuzzy");
#endif
#if _DEBUG
	if (cui_GeneralImgProcess::SAVE_DEBUG_2DISK==TRUE){
		char data_t[1024];																			
		ofstream outfile;								   
		outfile.open("Matrix_Weight_GVS_zlm_cuda.data",ios::out);
		for( int i = 0; i <pIMD->ImgHeight; i++ ){
			{
				double value_t=i;
				sprintf_s(data_t,1024," %0.2e ",value_t);
				outfile<<data_t;		
			}
			{
				double value_t=pIMD->pYweight_G[i];
				sprintf_s(data_t,1024," %0.2e ",value_t);
				outfile<<data_t;										
			}
			{
				double value_t=pIMD->pYweight_V[i];
				sprintf_s(data_t,1024," %0.2e ",value_t);
				outfile<<data_t;										
			}
			{
				double value_t=pIMD->pYweight_S[i];
				sprintf_s(data_t,1024," %0.2e ",value_t);
				outfile<<data_t;										
			}
			outfile<<endl;			 
		} 
		outfile.close();
	}
#endif

}
/*--------------------------------------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG2::CalculateSpClassification(int* Matrix_Category_Lable_host)
{
	TRACE_FUNC();
  double*  SkyWeightSp_host=pIMD->SkyWeightSp_host;
  double*  VerticalWeightSp_host=pIMD->VerticalWeightSp_host;
  double*  GroundWeightSp_host=pIMD->GroundWeightSp_host;
#if 1
	double* SVG_SpSum_host=new double[pIMD->slic_current_num];
	memset(SVG_SpSum_host,0,sizeof(double)*pIMD->slic_current_num);
	memset(SkyWeightSp_host,0,sizeof(double)*pIMD->slic_current_num);
	memset(VerticalWeightSp_host,0,sizeof(double)*pIMD->slic_current_num);
	memset(GroundWeightSp_host,0,sizeof(double)*pIMD->slic_current_num);
	for (register int x=0;x<pIMD->ImgWidth;x++){
		for (register int y=0;y<pIMD->ImgHeight;y++){
			int pixelIdx=y*pIMD->ImgWidth+x;
			int spi=pIMD->src_ImgLabels[pixelIdx];
			SkyWeightSp_host[spi]+=pIMD->pYweight_S[y];
			VerticalWeightSp_host[spi]+=pIMD->pYweight_V[y];
			GroundWeightSp_host[spi]+=pIMD->pYweight_G[y];
			SVG_SpSum_host[spi]+=1;
		}
	}
	for (register int spi=0;spi<pIMD->slic_current_num;spi++){
		SkyWeightSp_host[spi]/=SVG_SpSum_host[spi];
		VerticalWeightSp_host[spi]/=SVG_SpSum_host[spi];
		GroundWeightSp_host[spi]/=SVG_SpSum_host[spi];
#if 1
		//////////////////////////////////////////////////////////////
		if ((SkyWeightSp_host[spi]>=VerticalWeightSp_host[spi])
			&&(SkyWeightSp_host[spi]>=GroundWeightSp_host[spi])){
				Matrix_Category_Lable_host[spi]=Sky;
		}
		//////////////////////////////////////////////////////////////
		if ((GroundWeightSp_host[spi]>=VerticalWeightSp_host[spi])
			&&(GroundWeightSp_host[spi]>=SkyWeightSp_host[spi])){
				Matrix_Category_Lable_host[spi]=Ground;
		}
		//////////////////////////////////////////////////////////////
		if ((VerticalWeightSp_host[spi]>=SkyWeightSp_host[spi])
			&&(VerticalWeightSp_host[spi]>=GroundWeightSp_host[spi])){
				Matrix_Category_Lable_host[spi]=Vertical;
		}	
		////////////////////////////////////////////////////////////
#endif
	}
	delete [] SVG_SpSum_host;
#endif
	
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG2::separateSVG_Zlm(void)
{
	TRACE_FUNC();

	if (pIMD->SpSet.SpPropertySet[0].SpIndex!=0)
	{
		pIMD->initSpSet();
	}


#if 0
this->FillWeightArrayZlmParallel();
#else
this->FillWeightArrayZlm();
#endif
	

	


	this->ForceSkyGroundVertical();

	pIMD->CombinationImgSVG();

#if SaveContours2Disk
	pIMD->SaveImgSVGCompute("_BT_unknow");
	pIMD->SaveSuperpixelLabelsImagePNG();
#endif
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*
*
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG2::ForceSkyGroundVertical(void)
{
	TRACE_FUNC();
	this->zlm_ForceSky();
	
#if SaveContours2Disk
	pIMD->SaveImgWithPointsCompute();
#endif

this->zlm_ForceVertical();
this->zlm_ForceGround();

#if 1
	this->RemoveSomeUnreasonableClassification();
#endif

#if SaveContours2Disk
	pIMD->SaveImgWithPointsCompute("_FinalCompute_");
#endif
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*
*
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG2::zlm_ForceSky(void)
{
	TRACE_FUNC();
	float L_Distance=65;
	double    sky_energy_threshold=GetSkyHarrEnergyThreshold();
	double	  sky_Light_Threshold=GetSkyHarrLightThreshold();
	int sky_Effect_Index=CalculateMostEffectiveSky(sky_Light_Threshold,sky_energy_threshold);
	Init2SkyEffective(sky_Effect_Index);
	RemoveUnEffectiveSkySP(sky_Effect_Index,L_Distance);
    DivideSkyV(sky_energy_threshold,sky_Light_Threshold,L_Distance,Sky_Lab_Distance);
	this->RemoveSkyBySquareMeter();
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*
*
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG2::zlm_ForceVertical(void)
{
	//this->zlm_ForceVertical_De();
	//this->zlm_ForceVertical_WidthHeight(); 
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*
*
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG2::zlm_ForceGround(void)
{
	//地面连续
	TRACE_FUNC();
	this->RemoveGroundByPosition2();
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*
*
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG2::Init2SkyEffective(int sky_Effect_Index)
{
	TRACE_FUNC();
		pIMD->ImageGetSeedsLabxy_cuda();
		for (register int sp=pIMD->slic_current_num-1;sp>=0;sp--){
			   //到原点距离
			   Lab2Zero_L_Dst[sp]=sqrtl(powl(pIMD->kseedsa[sp],2)+powl(pIMD->kseedsb[sp],2));
			if (sky_Effect_Index!=-1){
				//计算颜色ab距离
				Lab2Point_AB_Dst[sp]=sqrtl(powl(pIMD->kseedsa[sp]-pIMD->kseedsa[sky_Effect_Index],2)+powl(pIMD->kseedsb[sp]-pIMD->kseedsb[sky_Effect_Index],2));
				//计算L距离	  
				Lab_L_Dst[sp]=fabsl(pIMD->SpSet.SpPropertySet[sp].harr_energy.Energy_LL-pIMD->SpSet.SpPropertySet[sky_Effect_Index].harr_energy.Energy_LL);
			}
	}
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*
*
*/
/*-------------------------------------------------------------------------------------------------------*/
double ComputeSVG2::GetSkyHarrLightThreshold(void)
{
	double	  sky_Light_Threshold;
	vector<double> energysort;
	energysort.reserve(pIMD->slic_current_num);
	for (register int spi=pIMD->slic_current_num-1;spi>=0;spi--){
		if (pIMD->SpSet.SpPropertySet[spi].fuzzyCategory!=Ground){
			energysort.push_back(pIMD->SpSet.SpPropertySet[spi].harr_energy.Energy_LL);
		}
	}
	sky_Light_Threshold=cui_GeneralImgProcess::GetAverageValue(energysort.data(),energysort.size());
#ifdef  Gray_Sky
#if Gray_Sky
	sky_Light_Threshold=0;
#endif	
#endif
	return sky_Light_Threshold;
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*
*
*/
/*-------------------------------------------------------------------------------------------------------*/
double ComputeSVG2::GetSkyHarrEnergyThreshold(void)
{
	TRACE_FUNC();
	double    sky_energy_threshold=0;	
	vector<double> sky_harr_energy;
	sky_harr_energy.clear();
	sky_harr_energy.reserve(pIMD->slic_current_num);

	for(register int spi=0;spi<pIMD->slic_current_num;spi++){
		if (pIMD->SpSet.SpPropertySet[spi].fuzzyCategory!=Ground){
			sky_harr_energy.push_back(pIMD->SpSet.SpPropertySet[spi].harr_energy.EnergyHighsigma);
		}
	}

	double sky_energy_avg,sky_energy_variance,sky_energy_up,sky_energy_down;
	cui_GeneralImgProcess::GetVarianceValue(sky_harr_energy.data(),sky_harr_energy.size(),&sky_energy_avg,&sky_energy_variance);
	sky_energy_up=(double)Sky_Energy_UP;
	sky_energy_down=(double)Sky_Energy_DOWN;
	/////////////////////////////////////////////////////////////////////////////////////
	sky_energy_threshold=std::min(sky_energy_avg,sky_energy_up);
	sky_energy_threshold=std::max(sky_energy_threshold,sky_energy_down);
	return sky_energy_threshold;
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*
*
*/
/*-------------------------------------------------------------------------------------------------------*/
int  ComputeSVG2::CalculateMostEffectiveSky(double sky_Light_Threshold,double sky_energy_threshold)
{
	TRACE_FUNC();
	int sky_Effect_Square_Index=-1;
	int	sky_Effect_vein_Index=0;
	ULONGLONG	sky_SquareMeter=0;
#if 1

	float sky_Vein=255;
	/**************************************************************************************/

	for (register int sp=pIMD->slic_current_num-1;sp>=0;sp--){

		assert(pIMD->SpSet.SpPropertySet[sp].fuzzyCategory>=0);

		if (((pIMD->SpSet.SpPropertySet[sp].BorderCategory&spBorderCategoryTop)==spBorderCategoryTop)
				&&(pIMD->SpSet.SpPropertySet[sp].fuzzyCategory==Sky)){

						//根据能量重新确定候选关系
						if ((pIMD->SpSet.SpPropertySet[sp].harr_energy.EnergyHighsigma<=Sky_Energy_DOWN)){
							if ((pIMD->SpSet.SpPropertySet[sp].harr_energy.Energy_LL-sky_Light_Threshold>=-1*sky_Light_Threshold/4)){
								    pIMD->SpSet.SpPropertySet[sp].Sky_Candidate=true;
							}else{
									pIMD->SpSet.SpPropertySet[sp].Sky_Candidate=false;
							}
						}else{
							if ((pIMD->SpSet.SpPropertySet[sp].harr_energy.Energy_LL>sky_Light_Threshold)&&
								(pIMD->SpSet.SpPropertySet[sp].harr_energy.EnergyHighsigma<sky_energy_threshold)){
										pIMD->SpSet.SpPropertySet[sp].Sky_Candidate=true;					
							}else{
									    pIMD->SpSet.SpPropertySet[sp].Sky_Candidate=false;
							}
						}
#if 1
			/*****************************面积最大**************************************/
			if ((pIMD->SpSet.SpPropertySet[sp].Sky_Candidate==true)&&
				(pIMD->SpSet.SpPropertySet[sp].SpSize>sky_SquareMeter)){
					sky_SquareMeter=pIMD->SpSet.SpPropertySet[sp].SpSize;
					sky_Effect_Square_Index=sp;
			}	
			/*******************************纹理最小************************************/
			if ((pIMD->SpSet.SpPropertySet[sp].Sky_Candidate==true)&&
				(pIMD->SpSet.SpPropertySet[sp].harr_energy.EnergyHighsigma<sky_Vein))
			{
				sky_Vein=pIMD->SpSet.SpPropertySet[sp].harr_energy.EnergyHighsigma;
				sky_Effect_vein_Index=sp;
			}
			/***************************************************************************/		
#endif
					}
				
	}
	/***********最大面积，第一候选天空***************************************************************************/
	/***********去掉不合适的候选*********************************************************************************/
	
#endif
#if 0
	pIMD->SaveImgWithPointsCompute();
#endif
	return (int)sky_Effect_Square_Index;
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*与最似天空比较，移除不合理的天空图块
*@param sky_Effect_Index  最似天空的索引值
*@param L_distance	LAB分量中L分量的距离
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG2::RemoveUnEffectiveSkySP(ULONGLONG sky_Effect_Index,float L_distance)
{
	TRACE_FUNC();
	if (sky_Effect_Index<0) 
		return;
	//除去与候选天空差异较大的天空候选块
	for (register int sp=0;sp<pIMD->slic_current_num;sp++){
#if 1
		if (pIMD->SpSet.SpPropertySet[sp].Sky_Candidate==true){
				//天空候选
				if (Lab_L_Dst[sp]<L_distance){
					pIMD->SpSet.SpPropertySet[sp].Sky_Candidate=true;
				}else{
					pIMD->SpSet.SpPropertySet[sp].Sky_Candidate=false;
				}	
	   }	
#endif	
	}
#if 0
	pIMD->SaveImgWithPointsCompute();
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
void ComputeSVG2::DivideSkyV(
	double sky_energy,
	double sky_Light_Threshold,
	double L_distance, 
	double AB_distance)
{
	/*能这样写是因为超像素图块标号是自上而下的===天空也是自上而下的*/
	for (register int spi=0;spi<pIMD->slic_current_num;spi++){	
			if (pIMD->SpSet.SpPropertySet[spi].Sky_Candidate==true){

				for (register int spj=0;spj<pIMD->slic_current_num;spj++){

					if (pIMD->Matrix_E[spi*pIMD->slic_current_num+spj]!=0){
						//与天空候选相邻
						if(pIMD->SpSet.SpPropertySet[spj].fuzzyCategory!=Ground){							
							//同时为天空
							if (Is_Sky_Block(spj,sky_Light_Threshold,sky_energy,AB_distance,L_distance)
								&&(CalculateSpSimilar(spi,spj)>0.9)){ 	
								pIMD->SpSet.SpPropertySet[spj].Sky_Candidate=true;
							}else{
								pIMD->SpSet.SpPropertySet[spj].Sky_Candidate=false;								
							}						
						}
					}
		       }
	       }
	}
	for (register int sp=0;sp<pIMD->slic_current_num;sp++){
		if ((pIMD->SpSet.SpPropertySet[sp].fuzzyCategory!=Ground)){
			////////////////
			if ((pIMD->SpSet.SpPropertySet[sp].harr_energy.Energy_LL>0)&&
				 (pIMD->SpSet.SpPropertySet[sp].Sky_Candidate==true)){		

				pIMD->SpSet.SpPropertySet[sp].ComputeCategory=Sky;
			}else{
				pIMD->SpSet.SpPropertySet[sp].ComputeCategory=Vertical;			
			}
			/////////////////
		}
	}
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*
*
*/
/*-------------------------------------------------------------------------------------------------------*/
bool ComputeSVG2::Is_Sky_Block(
	int spj,
	float sky_Light_Threshold,
	float sky_energy_Threshold,
	float AB_distance,
	float L_distance)
{
	bool IsSkyBlock=false;
#if 1
	bool light_condition=(pIMD->SpSet.SpPropertySet[spj].harr_energy.Energy_LL>sky_Light_Threshold);
	bool energy_condition=(pIMD->SpSet.SpPropertySet[spj].harr_energy.EnergyHighsigma<sky_energy_Threshold);
	bool AB_color_condition=((Lab2Point_AB_Dst[spj]<AB_distance)||(Lab2Zero_L_Dst[spj]<AB_distance));				    
	/*bool L_color_condition=((Lab_Color_Category[spj].L_Dst2Sky<L_distance)||(Lab_Color_Category[spj].L>=250));	*/


	bool light_condition2=(pIMD->SpSet.SpPropertySet[spj].harr_energy.Energy_LL-sky_Light_Threshold>=-1*sky_Light_Threshold/4);

	bool energy_condition2=((pIMD->SpSet.SpPropertySet[spj].harr_energy.EnergyHighsigma<=Sky_Energy_DOWN));

	bool AB_color_condition2=((Lab2Zero_L_Dst[spj]<5)||(Lab2Point_AB_Dst[spj]<AB_distance));


	if (light_condition&&energy_condition&&AB_color_condition==true){
		IsSkyBlock=true;
	}
	if (light_condition2&&energy_condition2&&AB_color_condition2==true){
		IsSkyBlock=true;
	}
#endif
	return IsSkyBlock;
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*
*
*/
/*-------------------------------------------------------------------------------------------------------*/
double ComputeSVG2::CalculateSpSimilar(int spi,int spj)
{

	float Blike=0;
	Energy_Harr  sp_1=pIMD->SpSet.SpPropertySet[spi].harr_energy;
	Energy_Harr  sp_2=pIMD->SpSet.SpPropertySet[spj].harr_energy;
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
	float sum1=powl(sp_1.Energy_LL,2)+powl(sp_1.Energy_LH,2)+powl(sp_1.Energy_HL,2)+powl(sp_1.Energy_HH,2);
	float sum2=powl(sp_2.Energy_LL,2)+powl(sp_2.Energy_LH,2)+powl(sp_2.Energy_HL,2)+powl(sp_2.Energy_HH,2);
	sum1=sqrtl(sum1);
	sum2=sqrtl(sum2);
	///////////////////////////////////
	if ((int)sum1!=0&&(int)sum2!=0){
		Blike=((sp_1.Energy_LL*sp_2.Energy_LL)+ 
			(sp_1.Energy_LH*sp_2.Energy_LH)+ 
			(sp_1.Energy_HL*sp_2.Energy_HL)+ 
			(sp_1.Energy_HH*sp_2.Energy_HH))/(sum1*sum2);
	}
	if ((int)sum1==0||(int)sum2==0){
		//其中一个为0
		Blike=0;
	}
	if ((int)sum1==0&&(int)sum2==0) {
		//同时为0
		Blike=1;
	}

#endif

	return Blike;
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*
*
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG2::RemoveSomeUnreasonableClassification(void)
{
	//天空的聚类方法决定了，天空不纯在悬浮
#if 1
	
#endif
#if 1
	
	//this->zlm_ForceVertical_SG();
#endif	
#if 1
	//地面的模糊结果，分块地面的重新分类（类似天空聚类）
	
#endif
#if 0
	//去掉悬浮的立面
	this->RemoveVerticalByNeighbor();
#endif
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*
*
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG2::RemoveSkyBySquareMeter(void)
{
#if 1
	vector<int> Category(pIMD->slic_current_num,-1);
	vector<vector<int>>  SkyBlocks(pIMD->slic_current_num);
	vector<float>  squareMeter(pIMD->slic_current_num,-1);
	float ThresholdSquareMetercurrent=pIMD->ImgWidth*pIMD->ImgHeight*0.01;
	/////////////////////////////////////////////////////////////
	for (register int spi=0;spi<pIMD->slic_current_num;spi++){
		if (pIMD->SpSet.SpPropertySet[spi].ComputeCategory==Sky){
			if (Category[spi]==-1){
				Category[spi]=spi;
			}			
			for (register int spj=spi;spj<pIMD->slic_current_num;spj++){
				if((pIMD->SpSet.SpPropertySet[spj].ComputeCategory==Sky)
					&&(0!=pIMD->Matrix_E[spi*pIMD->slic_current_num+spj])){
						Category[spj]=Category[spi];
				}				
			}
		}
	}
	///////////////////////////////////////////////////////////////
	for (register int spi=0;spi<pIMD->slic_current_num;spi++){
		if (Category[spi]!=-1){
			int blockIdx=Category[spi];
			SkyBlocks[blockIdx].push_back(spi);
		}

	}
	/////////////////////////////////////////////////////////////////
	for (int sbi=0;sbi<SkyBlocks.size();sbi++){
		for (int si=0;si<SkyBlocks[sbi].size();si++){
			squareMeter[sbi]+=pIMD->SpSet.SpPropertySet[SkyBlocks[sbi][si]].SpSize;
		}
	}
	///////////////////////////////////////////////////////////////////
	for (int smi=0;smi<squareMeter.size();smi++){
		if ((squareMeter[smi]<ThresholdSquareMetercurrent)&&(SkyBlocks[smi].size()>0)){
			for (int si=0;si<SkyBlocks[smi].size();si++){
				pIMD->SpSet.SpPropertySet[SkyBlocks[smi][si]].ComputeCategory=Vertical;
			}
		}		
	}
	////////////////////////////////////////////////////////////////
#endif
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*
*
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG2::zlm_ForceVertical_SG(void)
{
	/*************************************************************************************************/
	/*************************************************************************************************/
	//穿过视平线的地面即为立面 (穿越面积在一定比例之上)
	float UpSquare=0;
	float DownSquare=0;	
	for (register int sp=0;sp<pIMD->slic_current_num;sp++){

			if ((pIMD->SpSet.SpPropertySet[sp].BorderCategory&spBorderCategoryTop)==spBorderCategoryTop){					

					if(pIMD->SpSet.SpPropertySet[sp].ComputeCategory==Ground){	
						double DeVG=CalculateVGDifference(sp);
						if (DeVG<0.47){
							//分割上边沿大于0.1设为立面
							pIMD->SpSet.SpPropertySet[sp].ComputeCategory=Vertical;
						}
					}

		}

	}
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*
*
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG2::zlm_ForceVertical_WidthHeight(void)
{
	vector<float>  HeightWidthScale(pIMD->slic_current_num,-1);
	pIMD->CalculateAllSpPropertyRange();
	for (int spi=0;spi<pIMD->slic_current_num;spi++){
		if (pIMD->SpSet.SpPropertySet[spi].ComputeCategory==Ground){
			HeightWidthScale[spi]=1.0*pIMD->SpSet.SpPropertySet[spi].height/pIMD->SpSet.SpPropertySet[spi].width;
			if (HeightWidthScale[spi]>2){
				pIMD->SpSet.SpPropertySet[spi].ComputeCategory=Vertical;
			}
		}
	}
	
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*
*0.15
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG2::zlm_ForceVertical_De(void)
{
	/*************************************************************************************************/
	/*************************************************************************************************/
	//穿过视平线的地面即为立面 (穿越面积在一定比例之上)
	float UpSquare=0;
	float DownSquare=0;	
	for (register int sp=0;sp<pIMD->slic_current_num;sp++){

			if(pIMD->SpSet.SpPropertySet[sp].fuzzyCategory==Ground){	
				double DeVG=CalculateVGDifference(sp);
				if (DeVG<0.15){
					//分割上边沿大于0.1设为立面
					pIMD->SpSet.SpPropertySet[sp].ComputeCategory=Vertical;
				}
			}

		}

	
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*
*
*/
/*-------------------------------------------------------------------------------------------------------*/
void ComputeSVG2::RemoveGroundByPosition2(void)
{
	TRACE_FUNC();

	for (register int sp=pIMD->slic_current_num-1;sp>=0;sp--){
		if (((pIMD->SpSet.SpPropertySet[sp].BorderCategory&spBorderCategoryBottom)==spBorderCategoryBottom)
			&&(pIMD->SpSet.SpPropertySet[sp].ComputeCategory==Ground)){
				pIMD->SpSet.SpPropertySet[sp].Gnd_Candidate=true;
		}
	}
	/************重新分类***************************************************************************/
	for (register int i=pIMD->slic_current_num-1;abs(i)<pIMD->slic_current_num;i--){
		int spi=abs(i);
		if (pIMD->SpSet.SpPropertySet[spi].Gnd_Candidate==true){
			for (register int j=pIMD->slic_current_num-1;j>0;j--){	
				if ((pIMD->SpSet.SpPropertySet[j].ComputeCategory==Ground)
					&&(0!=pIMD->Matrix_E[spi*pIMD->slic_current_num+j])){					
						pIMD->SpSet.SpPropertySet[j].Gnd_Candidate=true;
				}			

			}
		}
	}
	/**********************************************************************************************/
	for (register int spi=pIMD->slic_current_num-1;spi>=0;spi--){
		if (pIMD->SpSet.SpPropertySet[spi].fuzzyCategory==Ground){
			if (pIMD->SpSet.SpPropertySet[spi].Gnd_Candidate==true){
				pIMD->SpSet.SpPropertySet[spi].ComputeCategory=Ground;
			}else{
				pIMD->SpSet.SpPropertySet[spi].ComputeCategory=Vertical;
			}
		}
	}
	/**********************************************************************************************/
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*
*
*/
/*-------------------------------------------------------------------------------------------------------*/
double ComputeSVG2::CalculateVGDifference(int spi)
{
	/*-------------------------------------------*/
	double Pg=pIMD->GroundWeightSp_host[spi];//G
	double Pv=pIMD->VerticalWeightSp_host[spi];//V
	double Ps=pIMD->SkyWeightSp_host[spi];//S
	/*-------------------------------------------*/
	double De=fabs(Pv-Pg)/fabs(std::max(Pv,Pg));
	return De;
}
/*--------------------------------------------------------------------------------------------------------*/
/**
*
*
*/
/*-------------------------------------------------------------------------------------------------------*/
void  ComputeSVG2::FillWeightArrayS_InDoor20150603(double horizontal_line, double n)
{
	TRACE_FUNC();
	float HgPos= pIMD->Seg_HorizontalLinePos+pIMD->PgOffset*pIMD->ImgHeight;
	float HsPos= pIMD->Seg_HorizontalLinePos-pIMD->PsOffset*pIMD->ImgHeight;
	float Hg=HgPos-pIMD->Seg_HorizontalLinePos;
	float H0=pIMD->Seg_HorizontalLinePos;
	float HC=pIMD->Seg_HorizontalLinePos-HsPos;
	int   H=pIMD->ImgHeight;
	//////////////////////////////////////////////////////////////////////
	for (int i=0;i<H;i++){
		int x=i-H0;
		int YIdx=i;
		pIMD->pYweight_S[YIdx]=Sx_InDoor20150603(x,1,H0,HC,H)*WeightZoom;
	}
	///////////////////////////////////////////////////////////////////////
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
void  ComputeSVG2::FillWeightArrayG_InDoor20150603(double horizontal_line, double n)
{
	TRACE_FUNC();
	float HgPos= pIMD->Seg_HorizontalLinePos+pIMD->PgOffset*pIMD->ImgHeight;
	float HsPos= pIMD->Seg_HorizontalLinePos-pIMD->PsOffset*pIMD->ImgHeight;
	float Hg=HgPos-pIMD->Seg_HorizontalLinePos;
	float H0=pIMD->Seg_HorizontalLinePos;
	float HC=pIMD->Seg_HorizontalLinePos-HsPos;
	int   H=pIMD->ImgHeight;

	for (int i=0;i<H;i++){
		int x=i-H0;
		int YIdx=i;
		pIMD->pYweight_G[YIdx]=Gx_InDoor20150603(x,1,H0,Hg,H)*WeightZoom;
	}

}
/*--------------------------------------------------------------------*/
/**
*
*
*/
/*--------------------------------------------------------------------*/
inline double ComputeSVG2::Gx_InDoor20150603(int y,int n,float H0,float Hg,int H)
{
	double Pn=1.0*(2*n+2)/(2*n+1);
	double Gy=0;
	double C_g=Pn/powl(H-Hg-H0,Pn);
	double yHg=0;

	if (y>=Hg){
		yHg=powl(y-Hg,1.0/(2*n+1));
	}else if(y<Hg){
		yHg=-1*powl(Hg-y,1.0/(2*n+1));
	}
	Gy=C_g*yHg;
	return Gy;
}
/*--------------------------------------------------------------------*/
/**
*
*
*/
/*--------------------------------------------------------------------*/
inline double ComputeSVG2::Sx_InDoor20150603(int y,int n,float H0,float Hc,int H)
{
	double Pn=1.0*(2*n+2)/(2*n+1);
	double Gy=0;
	double C_g=Pn/powl(H0-Hc,Pn);
	double yHg=0;

	if (y<=-1*Hc){
		yHg=powl(-1*y-Hc,1.0/(2*n+1));
	}else if(y>-1*Hc){
		yHg=-1*powl(y+Hc,1.0/(2*n+1));
	}
	Gy=C_g*yHg;
	return Gy;
}
/*--------------------------------------------------------------------*/
/**
*
*
*/
/*--------------------------------------------------------------------*/
void ComputeSVG2::FillWeightArrayV_New(double horizontal_line, double n)
{
	TRACE_FUNC();
	/*-------------清零---------------------*/
	for (register int i=0;i<pIMD->ImgHeight;i++){
			pIMD->pYweight_V[i]=0;
	}
	/*-------------清零---------------------*/
	/*
	2015年6月3日13:43:13
	this->FillWeightArrayV_Gaussian(horizontal_line,n,0.5,6);
	this->FillWeightArrayV_Gaussian(horizontal_line,n,48,2);
	*/
	/*this->FillWeightArrayV_Gaussian(horizontal_line,n,0.5,2);
	this->FillWeightArrayV_Gaussian(horizontal_line,n,12,2);*/
	/*室外参数6 ---2*/
	this->FillWeightArrayV_Gaussian(horizontal_line,n,6,2);
}
/*--------------------------------------------------------------------*/
/**
*
*
*/
/*--------------------------------------------------------------------*/
void ComputeSVG2::FillWeightArrayV_Gaussian(double horizontal_line, double n,double SigmaScale,double WeightScale)
{
	TRACE_FUNC();
	{
#if 1
		//室外原始和室内叠加
		double sigma;
		double L,L_1,L_2;
		L_1=horizontal_line; 
		L_2=pIMD->ImgHeight-horizontal_line;
		L=(L_1>L_2)?L_2:L_1;
		sigma=L/SigmaScale;

		for (register int i=-L;i<L;i++){
			if ((horizontal_line-i)>=0&&(horizontal_line-i)<pIMD->ImgHeight){

				double constant_t=(1/(sqrt(2*M_PI)*sigma))*WeightZoom*WeightScale;
				double variable_t= exp(-powl(i,2)/powl(sigma,2));
				double result=constant_t*variable_t;
				pIMD->pYweight_V[(int)horizontal_line-i]+=constant_t*variable_t;
			}
		}
#endif
	}	
}
/*--------------------------------------------------------------------*/
/**
*
*
*/
/*-----n---------------------------------------------------------------*/