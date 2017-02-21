#include "StdAfx.h"
#include "module_all_cui.h"
//#include "SplitTreeandBuilding.h"
/*-----------------------------------------------------------------------------*/
/**
*
*/
/*-----------------------------------------------------------------------------*/
SplitTreeandBuilding::SplitTreeandBuilding(
	INT32* ImgLables_SVG,
	int* ImgLables,
	ImageMemData* pMD_t)
{
	this->pMD=pMD_t;
	svgCount=nullptr;
	int *pMD_ImgLabes=pMD->ImgLables.get();
	memcpy(pMD_ImgLabes,ImgLables,sizeof(int)*pMD->ImgWidth*pMD->ImgHeight);
	memcpy(pMD->p_ImgLables_SVG,ImgLables_SVG,sizeof(INT32)*pMD->ImgWidth*pMD->ImgHeight);
}
/*-----------------------------------------------------------------------------*/
/**
*
*/
/*-----------------------------------------------------------------------------*/
SplitTreeandBuilding::SplitTreeandBuilding(ImageMemData* pMD_t)
{
	this->pMD=pMD_t;
	svgCount=nullptr;
}
/*-----------------------------------------------------------------------------*/
/**
*
*/
/*-----------------------------------------------------------------------------*/

SplitTreeandBuilding::~SplitTreeandBuilding(void)
{
	ReleaseMem();
}
/*-----------------------------------------------------------------------------*/
/**
*
*/
/*-----------------------------------------------------------------------------*/

void SplitTreeandBuilding::SlpitTreeBuilding(void)
{
	//已经分割完成 -要还原
	INT32* Matrix_Category_Lable=pMD->Matrix_Category_Lable.get();
	this->ReBuildingImgLab();
	
	HoughAnalyze ha(pMD);
	ha.GetHist180All();

	for (int spi=0;spi<pMD->slic_current_num;spi++){
		if (Matrix_Category_Lable[spi]==Vertical){
			if (IsTree(spi)){
				Matrix_Category_Lable[spi]=Vertical_Tree;
			}else if(IsBuilding(spi)){
				Matrix_Category_Lable[spi]=Vertical_Building;
			}else{
				ASSERT(FALSE);
			}

		}
		

	}
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
/*-----------------------------------------------------------------------------*/
/**
*
*/
/*-----------------------------------------------------------------------------*/

bool SplitTreeandBuilding::IsTree(int sp)
{
	HistData180* histdata=pMD->Histogram180.get();
    HistData180 histsp=histdata[sp];

	return SceneDetermine::IsBlockTree(histsp,AngLeDivided180);
}
/*-----------------------------------------------------------------------------*/
/**
*
*/
/*-----------------------------------------------------------------------------*/
bool SplitTreeandBuilding::IsBuilding(int sp)
{
	
	HistData180* histdata=pMD->Histogram180.get();
	HistData180 histsp=histdata[sp];

	return SceneDetermine::IsBlockBuilding(histsp,AngLeDivided180);
}
/*-----------------------------------------------------------------------------*/
/**
*
*/
/*-----------------------------------------------------------------------------*/
void SplitTreeandBuilding::ReBuildingImgLab(void)
{
	string file_add="ReBuild";
	this->AllocMem();
#if TRUE
	cui_GeneralImgProcess::CalculateAllSpPropertyRange(
		pMD->ImgLables.get(),
		pMD->ImgWidth,
		pMD->ImgHeight,
		pMD->p_SpProperty,
		pMD->slic_current_num);
#endif
	INT32 *ImgLables_SVG=pMD->p_ImgLables_SVG;
	SP_PROPERTY* p_SpProperty=pMD->p_SpProperty;
#if IN_DOOR
INT32* Matrix_Category_Lable=pMD->Matrix_Category_Lable_InDoor.get();
#endif
#if OUT_DOOR
INT32* Matrix_Category_Lable=pMD->Matrix_Category_Lable.get();
#endif
		for (int spi=0;spi<pMD->slic_current_num;spi++){

				for(register int x=p_SpProperty[spi].min_x;x<p_SpProperty[spi].max_x;x++){
					for (register int y=p_SpProperty[spi].min_y;y<p_SpProperty[spi].max_y;y++){
						
						if(ImgLables_SVG[x+y*pMD->ImgWidth]==Sky){
							svgCount[spi].SkyCount++;
						}else if(ImgLables_SVG[x+y*pMD->ImgWidth]==Vertical){
							svgCount[spi].VerticalCount++;
						}else if(ImgLables_SVG[x+y*pMD->ImgWidth]==Ground){
							svgCount[spi].GroundCount++;
						}else{
							INT32 color=ImgLables_SVG[x+y*pMD->ImgWidth];
							ASSERT(FALSE);
						}	
											
						
						}
					}
				Matrix_Category_Lable[spi]=svgCount[spi].GetSVGproperty();
				

	}
#if TRUE
	cui_GeneralImgProcess::CuiSaveImgWithPoints(
			pMD->Src_ImgData,
		    pMD->ImgLables.get(),
		    pMD->Matrix_Category_Lable.get(),
			pMD->ImgWidth,
			pMD->ImgHeight,
			true,
			pMD,
			"");
#endif	
	this->CalculateSVGImgLables();
#if TRUE
	cui_GeneralImgProcess::CuiSaveImgWithPoints(
		pMD->Src_ImgData,
		pMD->p_ImgLables_SVG,
		nullptr,
		pMD->ImgWidth,
		pMD->ImgHeight,
		(bool)OUT_NOGROUND_IMG,
		pMD,
		file_add);	
#endif
	

}
/*-----------------------------------------------------------------------------*/
/**
*
*/
/*-----------------------------------------------------------------------------*/
void SplitTreeandBuilding::AllocMem(void)
{
	ReleaseMem();
	this->svgCount=new SVGcount[pMD->slic_current_num];
}
/*-----------------------------------------------------------------------------*/
/**
*
*/
/*-----------------------------------------------------------------------------*/
void SplitTreeandBuilding::ReleaseMem(void)
{
	if (svgCount!=nullptr){
		delete[] svgCount;
		svgCount=nullptr;
	}
	
}
/*-----------------------------------------------------------------------------*/
/**
*重新绘制SVG
*/
/*-----------------------------------------------------------------------------*/
void SplitTreeandBuilding::CalculateSVGImgLables(void)
{
#if OUT_DOOR
	cui_GeneralImgProcess::Cui_CombinationImgSVG(
		pMD->Src_ImgData,
		pMD->ImgLables.get(),
		pMD->p_ImgLables_SVG,
		pMD->Matrix_Category_Lable.get(),
		pMD->ImgWidth,
		pMD->ImgHeight,
		"","","");
#endif
#if IN_DOOR
	cui_GeneralImgProcess::Cui_CombinationImgSVG(
		pMD->Src_ImgData,
		pMD->ImgLables.get(),
		pMD->p_ImgLables_SVG,
		pMD->Matrix_Category_Lable_InDoor.get(),
		pMD->ImgWidth,
		pMD->ImgHeight,
		"","","");
#endif
}
/*-----------------------------------------------------------------------------*/
/**
*
*/
/*-----------------------------------------------------------------------------*/