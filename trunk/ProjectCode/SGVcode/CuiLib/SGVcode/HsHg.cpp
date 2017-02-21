#include "StdAfx.h"
#include "module_all_cui.h"
//#include "HsHg.h"
/*-------------------------------------------------------------------*/
	
/*-------------------------------------------------------------------*/
/**
*依据分类好的图像（天空立面地面）
*初始化本类
*@param pMD_t  图像
*/
/*-------------------------------------------------------------------*/
HsHg::HsHg(ImageMemData* pMD_t)
{
	this->pMD=pMD_t;
	this->PgOffset_Last=0;
	this->PsOffset_Last=0;
#if TRUE
	this->CameraHeight=1.11;/**<2014年3月24日14:57:35-激光实测*/	
	this->ImgFocus=pMD->ImgHeight*1.0;
	this->ImgHeight=pMD->ImgHeight;
	this->CameraOffAngle=0;
#endif
#if TRUE
	this->RoomLength=6;
	this->RoomHeight=4-0.5;
	this->RoomHeight2Top=RoomHeight-CameraHeight;
#endif	

		Matrix_Category_Lable_SkyBoder=std::shared_ptr<INT32>(new INT32[pMD->slic_current_num]);
		Matrix_Category_Lable_GndBoder=std::shared_ptr<INT32>(new INT32[pMD->slic_current_num]);
}
/*-------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------*/
HsHg::~HsHg(void)
{

}
/*-------------------------------------------------------------------*/
/**
*获得天空上的
*/
/*-------------------------------------------------------------------*/
float HsHg::GetHs_InDoor(float DeflectionAngle)
{
	float Hs=0;
	if (DeflectionAngle==0){
		Hs=(CameraHeight*ImgFocus)/(RoomLength)/ImgHeight;
		Hs=1.0*(432-288)/864;
	}else{

	}
	return Hs;
}
/*-------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------*/
float HsHg::GetHg_InDoor(float DeflectionAngle)
{
	float Hg=0;

	if (DeflectionAngle==0){
		Hg=(RoomHeight2Top*ImgFocus)/(RoomLength)/ImgHeight;
		Hg=1.0*(445-432)/864;
	}else{

	}
	return Hg;
}
/*-------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------*/
float  HsHg::GetHs_ByIteration(void)
{
	//最大值
	int *ImgLab=pMD->ImgLables.get();
	INT32 * Matrix_Category_Lable=pMD->Matrix_Category_Lable_InDoor.get();
	this->ForceSky_InDoor_DFS();
	float Hs=0;
	for (int y=pMD->ImgHeight-1;y>=0;y--){
		for (int x=0;x<pMD->ImgWidth;x++){		
			int sp=ImgLab[y*pMD->ImgWidth+x];
			if (Matrix_Category_Lable[sp]==Sky){
				Hs=(Hs<=y)?y:Hs;
			}
		}
	}	
	float HsLimit=ImgFocus*tan(CameraOffAngle)+pMD->Seg_HorizontalLinePos;
	Hs=(HsLimit<Hs)?HsLimit:Hs;

	Hs=(pMD->Seg_HorizontalLinePos-Hs)/(pMD->ImgHeight);
	return Hs;
}
/*-------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------*/
float  HsHg::GetHg_ByIteration(void)
{
	//最小值
	int *ImgLab=pMD->ImgLables.get();
	INT32 * Matrix_Category_Lable=pMD->Matrix_Category_Lable_InDoor.get();
#if 0
	this->ForceGround_InDoor_DFS();
#else
	ComputeSVG::ForceGround_InDoor_DFS(pMD);
#endif
	
	float Hg=pMD->ImgHeight;
	for (int y=0;y<pMD->ImgHeight;y++){
	for (int x=0;x<pMD->ImgWidth;x++){
			int sp=ImgLab[y*pMD->ImgWidth+x];
			if (Matrix_Category_Lable[sp]==Ground){
				Hg=(Hg>=y)?y:Hg;
			}
		}
	}	
	
	
	float HgLimit=ImgFocus*tan(CameraOffAngle)+pMD->Seg_HorizontalLinePos;
	Hg=(HgLimit>Hg)?HgLimit:Hg;
	
	Hg=(Hg-pMD->Seg_HorizontalLinePos)/(pMD->ImgHeight);
	return Hg;
}
/*-------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------*/
void   HsHg::ForceGround_InDoor_DFS(void)
{
	UINT32* LinkConn=pMD->LinkConnection.get();
	UINT32* V=pMD->Matrix_Visit.get();
	this->InitDFSLinkConnection_Ground();
	memset(V,FALSE,sizeof(UINT32)*pMD->slic_current_num);
#if IN_DOOR	
	for (int spi=0;spi<pMD->slic_current_num;spi++){
		/*深序遍历*/
		if ( (LinkConn[spi]==TRUE)			
			&&(V[spi]==FALSE)){
				TRACE("深序遍历Root=%d\n",spi);
				this->DFS_4Ground2Up(spi);
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
*
*/
/*-------------------------------------------------------------------*/
void   HsHg::InitDFSLinkConnection_Ground(void)
{
	UINT32* LinkConn=pMD->LinkConnection.get();
	INT32 * Matrix_Category_Lable=pMD->Matrix_Category_Lable_InDoor.get();
	UINT32* Matrix_E=pMD->Matrix_E_InDoor.get();
	INT32* PositionCategoryForce=pMD->PositionCategoryForce.get();

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
}
/*-------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------*/
void HsHg::DFS_4Ground2Up(int spi)
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
			if ((Matrix_Category_Lable[spj]==Ground)
				||(Matrix_Category_Lable[spj]==GroundPending))
			{
				float abLike=cui_GeneralImgProcess::LabDistanceAB(SpProperty[spj].lab_color,rootColor);
				TRACE(" %d&&%dAB_LIKKE=%f ",spi,spj,abLike);
				float Threshold=1.5;/*小于1.7*/
				if (abLike<=Threshold){
					//与底块相似
					Matrix_Category_Lable[spj]=GroundPending;
					DFS_4Ground2Up(spj); //对访问的邻接顶点递归调用
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
*
*/
/*-------------------------------------------------------------------*/
void  HsHg::ForceSky_InDoor_DFS(void)
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
/*-------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------*/
void  HsHg::InitDFSLinkConnection_Sky(void)
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
/*-------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------*/
void HsHg::DFS_4Sky2Down(int spi)
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
/*-------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------*/
float HsHg::GetHs_InDoor_Geometry(float DeflectionAngle)
{

	

	float Hs=0;
	if (DeflectionAngle==0){
		Hs=(CameraHeight*ImgFocus)/(RoomLength)/ImgHeight;
		Hs=1.0*(432-288)/864;
	}else{

	}
	return Hs;
}
/*-------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------*/
float HsHg::GetHg_InDoor_Geometry(float DeflectionAngle)
{
	

	float Hg=0;

	if (DeflectionAngle==0){
		Hg=(RoomHeight2Top*ImgFocus)/(RoomLength)/ImgHeight;
		Hg=1.0*(445-432)/864;
	}else{

	}
	return Hg;
}
/*-------------------------------------------------------------------*/
/**
*
*获取Hs,Hg的位置。
*预估地面长度。
*假设光学中心在图像中心。
*/
/*-------------------------------------------------------------------*/
void HsHg::Get_HsHg_ByIteration(void)
{
	float HsOffset=this->GetHs_ByIteration()*pMD->ImgHeight;
#if 0
/*---------------------------------------------------------------------------*/
	cui_GeneralImgProcess::CuiSaveImgWithPoints(
		pMD->Src_ImgData,
		pMD->ImgLables.get(),
		pMD->Matrix_Category_Lable_InDoor.get(),
		pMD->ImgWidth,
		pMD->ImgHeight,
		true,
		pMD,"");
#endif
	float HgOffset=this->GetHg_ByIteration()*pMD->ImgHeight;
#if 0
/*---------------------------------------------------------------------------*/

	cui_GeneralImgProcess::CuiSaveImgWithPoints(
		pMD->Src_ImgData,
		pMD->ImgLables.get(),
		pMD->Matrix_Category_Lable_InDoor.get(),
		pMD->ImgWidth,
		pMD->ImgHeight,
		true,
		pMD,"");
#endif	
#if 0
	/*---------------------------------------------------------------------------*/
	float Ksg=1.0*RoomHeight2Top/CameraHeight;
	float HsOffset_c=Ksg*HgOffset;
	float HgOffset_c=HsOffset/Ksg;	
	/*---------------------------------------------------------------------------*/
	pMD->PgOffset=(HgOffset+HgOffset_c)/pMD->ImgHeight;
	pMD->PsOffset=(HsOffset+HsOffset_c)/pMD->ImgHeight;
	/*---------------------------------------------------------------------------*/
#else
	pMD->PgOffset=HgOffset/pMD->ImgHeight;
	this->RoomLength=CameraHeight*ImgFocus/HgOffset;
	HsOffset=RoomHeight2Top/RoomLength*ImgFocus;
	pMD->PsOffset=HsOffset/pMD->ImgHeight;
#endif
#if TRUE
	cui_GeneralImgProcess::CuiSaveImgWithPoints(
		pMD->Src_ImgData,
		pMD->ImgLables.get(),
		pMD->Matrix_Category_Lable_InDoor.get(),
		pMD->ImgWidth,
		pMD->ImgHeight,
		true,
		pMD,"");
#endif	
}
/*-------------------------------------------------------------------*/
/**
*@deprecated 弃用
*/
/*-------------------------------------------------------------------*/
void HsHg::Get_HsHg_By_SV_VG(void)
{
	vector<CvPoint> VGPoints;
	vector<int> VGPointY;
	vector<CvPoint> SVPoints;
	vector<int> SVPointY;
	cui_GeneralImgProcess::GetVGPoint(VGPoints,pMD);
	cui_GeneralImgProcess::GetSVPoint(SVPoints,pMD);
	for (int i=0;i<VGPoints.size();i++){
		VGPointY.push_back(VGPoints[i].y);
	}
	for (int i=0;i<SVPoints.size();i++){
		SVPointY.push_back(SVPoints[i].y);
	}

	{
		/*------------------------------------------------*/
		float Hs=cui_GeneralImgProcess::GetMaxValue(SVPointY.data(),SVPointY.size());
		float HsLimit=ImgFocus*tan(CameraOffAngle)+pMD->Seg_HorizontalLinePos;
		Hs=(HsLimit<Hs)?HsLimit:Hs;
		pMD->PsOffset=(pMD->Seg_HorizontalLinePos-Hs)/(pMD->ImgHeight);
		/*------------------------------------------------*/
	}

	{
		float Hg=cui_GeneralImgProcess::GetMinValue(VGPointY.data(),VGPointY.size());
		float HsLimit=ImgFocus*tan(CameraOffAngle)+pMD->Seg_HorizontalLinePos;
		Hg=(HsLimit>Hg)?HsLimit:Hg;
		pMD->PgOffset=(Hg-pMD->Seg_HorizontalLinePos)/(pMD->ImgHeight);
		/*------------------------------------------------*/
	}
	
}
/*-------------------------------------------------------------------*/
/**
*使用天花板，地面的梯形透视方法。\n
*获得地面，天花板的边界
*/
/*-------------------------------------------------------------------*/
void HsHg::Get_HsHg_By_NBig_FSmall_5m4d(void)
{
	vector<float> SkyY;
	vector<float> GndY;

	vector<float> SkyYData;
	vector<float> GndYData;
	this->StatisticsTrapezoidHgHs_5m4d(SkyY,GndY);
	SkyYData.clear();
	GndYData.clear();
	for (int i=0;i<SkyY.size();i++){
		if (SkyY[i]!=-1){
			ASSERT(SkyY.size()!=0);
			SkyYData.push_back(SkyY[i]);
		}
	}
	for (int i=0;i<GndY.size();i++){
		if (GndY[i]!=-1){
			ASSERT(GndY.size()!=0);
			GndYData.push_back(GndY[i]);
		}
	}
#if FALSE
//获得均值方差
	float Sky_avg;
	float Sky_dev;
	cui_GeneralImgProcess::GetVarianceValue(SkyYData.data(),SkyYData.size(),&Sky_avg,&Sky_dev);
	float Sky_avg_1d96dev=Sky_avg-1.96*Sky_dev;

	float Gnd_avg;
	float Gnd_dev;
	cui_GeneralImgProcess::GetVarianceValue(GndYData.data(),GndYData.size(),&Gnd_avg,&Gnd_dev);
	float Gnd_avg_1d96dev=Gnd_avg-1.96*Gnd_dev;
#else
	//获得均值方差
	float Sky_avg=cui_GeneralImgProcess::GetMaxValue(SkyYData.data(),SkyYData.size());
	float Sky_dev=0;;
	//////////////////////////////////
	for (int i=0;i<SkyYData.size();i++){
		Sky_dev+=pow(Sky_avg-SkyYData[i],2);
	}
	Sky_dev/=SkyYData.size();
	Sky_dev=sqrtl(Sky_dev); 
	float Sky_avg_1d96dev=Sky_avg-1.96*Sky_dev;
	float Sky_avg_1d0dev=Sky_avg-1.0*Sky_dev;
	/////////////////////////////////////
	

	float Gnd_avg=cui_GeneralImgProcess::GetMaxValue(GndYData.data(),GndYData.size());
	float Gnd_dev=0;
	////////////////////////////////
	for (int i=0;i<GndYData.size();i++){
		Gnd_dev+=pow(Gnd_avg-GndYData[i],2);
	}
	Gnd_dev/=GndYData.size();
	Gnd_dev=sqrtl(Gnd_dev); 
	////////////////////////////////////////////
	float Gnd_avg_1d96dev=Gnd_avg-1.96*Gnd_dev;
	float Gnd_avg_1d0dev=Gnd_avg-1.0*Gnd_dev;
#endif
#if FALSE
	for (int i=0;i<SkyY.size()/2;i++){
		if (SkyY[i]>Sky_avg_1d96dev){
			//保留
		}else{
			SkyY[i]=-1;
		}
	}
	for (int i=GndY.size()/2;i<GndY.size();i++){
		if (GndY[i]>Gnd_avg_1d96dev){
			//保留
		}else{
			GndY[i]=-1;
		}
	}
#else
for (int i=0;i<SkyY.size()/2;i++){
		if ((SkyY[i]>Sky_avg_1d96dev)
			&&(SkyY[i]<Sky_avg_1d0dev)){
			//保留
		}else{
			SkyY[i]=-1;
		}
	}
	for (int i=GndY.size()/2;i<GndY.size();i++){
		if ((GndY[i]>Gnd_avg_1d96dev)
			&&(GndY[i]<Gnd_avg_1d0dev)){
			//保留
		}else{
			GndY[i]=-1;
		}
	}
#endif
	
	/////////////////////////////////////////////////////////////
	float Hs;
	vector<float> HsData;
	for (int i=0;i<SkyY.size()/2;i++){
		if (SkyY[i]>0){
			HsData.push_back(i);
		}
	}
	////////////////////////////////////////////////////////////
	float Hg;
	vector<float> HgData;
	for (int i=GndY.size()/2;i<GndY.size();i++){
		if (GndY[i]>0){
			HgData.push_back(i);
		}
	}
	if (HsData.size()!=0){
		/*------------------------------------------------*/
		float Hs=cui_GeneralImgProcess::GetMaxValue(HsData.data(),HsData.size());
		float HsLimit=ImgFocus*tan(CameraOffAngle)+pMD->Seg_HorizontalLinePos;
		Hs=(HsLimit<Hs)?HsLimit:Hs;
		pMD->PsOffset=(pMD->Seg_HorizontalLinePos-Hs)/(pMD->ImgHeight);
		/*------------------------------------------------*/
	}else{
		pMD->PsOffset=pMD->PsOffset;
	}

	if (HgData.size()!=0){
		float Hg=cui_GeneralImgProcess::GetMinValue( HgData.data(), HgData.size());
		float HsLimit=ImgFocus*tan(CameraOffAngle)+pMD->Seg_HorizontalLinePos;
		Hg=(HsLimit>Hg)?HsLimit:Hg;
		pMD->PgOffset=(Hg-pMD->Seg_HorizontalLinePos)/(pMD->ImgHeight);
		/*------------------------------------------------*/
	}else{
		pMD->PgOffset=pMD->PgOffset;
	}

}
/*-------------------------------------------------------------------*/
/**
*非几何方法获得地面，天花板的边界线
*/
/*-------------------------------------------------------------------*/
void HsHg::Get_HsHg_ByIterationNoGeometric(void)
{
	
#if 0
	float HgOffset=this->GetHg_ByIteration()*pMD->ImgHeight;
#else
	float HgOffset=this->GetHg_Nogeometry()*pMD->ImgHeight;
#endif
	float HsOffset=this->GetHs_Nogeometry()*pMD->ImgHeight;
	pMD->PgOffset=HgOffset/pMD->ImgHeight;
	pMD->PsOffset=HsOffset/pMD->ImgHeight;

#if TRUE
	cui_GeneralImgProcess::CuiSaveImgWithPoints(
		pMD->Src_ImgData,
		pMD->ImgLables.get(),
		pMD->Matrix_Category_Lable_InDoor.get(),
		pMD->ImgWidth,
		pMD->ImgHeight,
		true,
		pMD,"");
#endif	

}
/*-------------------------------------------------------------------*/
/**
*不使用几何方法获得地面分界线
*/
/*-------------------------------------------------------------------*/
float  HsHg::GetHg_Nogeometry(void)
{
	//最大值
	float   Hg=pMD->Seg_HorizontalLinePos;
	int*    ImgLab=pMD->ImgLables.get();
	INT32*  Matrix_Category_Lable=pMD->Matrix_Category_Lable_InDoor.get();
	UINT32* Matrix_E=pMD->Matrix_E_InDoor.get();
	SP_PROPERTY* SpProperty=pMD->p_SpProperty;

#if UseVerticalTop
	float Hs_Up=pMD->Seg_HorizontalLinePos;
	float Hs_Down=pMD->Seg_HorizontalLinePos;
	for (int y=pMD->ImgHeight-1;y>=0;y--){
		for (int x=0;x<pMD->ImgWidth;x++){		
			int sp=ImgLab[y*pMD->ImgWidth+x];
			if (Matrix_Category_Lable[sp]==Vertical){
				Hs_Up=(Hs_Up>=y)?y:Hs_Up;
				Hs_Down=(Hs_Down<=y)?y:Hs_Down;
			}
		}
	}	
	float Hs=Hs_Up;
#endif	
#if UseVerticalSpTopAvg
	vector<double> Ypos;
	/*----------------------------------------------*/
	cui_GeneralImgProcess::CalculateAllSpPropertyRange(
		pMD->ImgLables.get(),
		pMD->ImgWidth,
		pMD->ImgHeight,
		pMD->p_SpProperty,
		pMD->slic_current_num);
	cui_GeneralImgProcess::CuiSetNighbour_E_matrix(
		pMD->ImgLables.get(),
		pMD->slic_current_num,
		pMD->Matrix_E_InDoor.get(),
		pMD->ImgWidth,pMD->ImgHeight,
		"InDoor","InDoor");
	/*----------------------------------------------*/
	for (int spi=0;spi<pMD->slic_current_num;spi++){
		if (Matrix_Category_Lable[spi]==Vertical){
			for (int spj=0;spj<pMD->slic_current_num;spj++){
				if ((Matrix_E[spi*pMD->slic_current_num+spj]==TRUE)
					&&(Matrix_Category_Lable[spj]==Ground)){
						Ypos.push_back(SpProperty[spi].max_y);
						break;
				}
			}
		}
	}
	/*----------------------------------------------*/
	if (Ypos.size()>0){
		Hg=cui_GeneralImgProcess::GetAverageValue(Ypos.data(),Ypos.size());
	}else{
		Hg=pMD->ImgHeight;//已到达bottom位置
	}

#endif	

	float HsLimit=ImgFocus*tan(CameraOffAngle)+pMD->Seg_HorizontalLinePos;
	Hg=(HsLimit>Hg)?HsLimit:Hg;

	Hg=(Hg-pMD->Seg_HorizontalLinePos)/(pMD->ImgHeight);
	return Hg;
}
/*-------------------------------------------------------------------*/
/**
*不使用几何方法获得天花板的分界线
*/
/*-------------------------------------------------------------------*/
float  HsHg::GetHs_Nogeometry(void)
{
	//最大值
	float   Hs=pMD->Seg_HorizontalLinePos;
	int*    ImgLab=pMD->ImgLables.get();
	INT32*  Matrix_Category_Lable=pMD->Matrix_Category_Lable_InDoor.get();
	UINT32* Matrix_E=pMD->Matrix_E_InDoor.get();
	SP_PROPERTY* SpProperty=pMD->p_SpProperty;
	
#if UseVerticalTop
	float Hs_Up=pMD->Seg_HorizontalLinePos;
	float Hs_Down=pMD->Seg_HorizontalLinePos;
	for (int y=pMD->ImgHeight-1;y>=0;y--){
		for (int x=0;x<pMD->ImgWidth;x++){		
			int sp=ImgLab[y*pMD->ImgWidth+x];
			if (Matrix_Category_Lable[sp]==Vertical){
				Hs_Up=(Hs_Up>=y)?y:Hs_Up;
				Hs_Down=(Hs_Down<=y)?y:Hs_Down;
			}
		}
	}	
	float Hs=Hs_Up;
#endif	
#if UseVerticalSpTopAvg
	vector<double> Ypos;
	/*----------------------------------------------*/
	cui_GeneralImgProcess::CalculateAllSpPropertyRange(
		pMD->ImgLables.get(),
		pMD->ImgWidth,
		pMD->ImgHeight,
		pMD->p_SpProperty,
		pMD->slic_current_num);
	cui_GeneralImgProcess::CuiSetNighbour_E_matrix(
		pMD->ImgLables.get(),
		pMD->slic_current_num,
		pMD->Matrix_E_InDoor.get(),
		pMD->ImgWidth,pMD->ImgHeight,
		"InDoor","InDoor");
	/*----------------------------------------------*/
	for (int spi=0;spi<pMD->slic_current_num;spi++){
		if (Matrix_Category_Lable[spi]==Vertical){
			for (int spj=0;spj<pMD->slic_current_num;spj++){
				if ((Matrix_E[spi*pMD->slic_current_num+spj]==TRUE)
					&&(Matrix_Category_Lable[spj]==Sky)){
						Ypos.push_back(SpProperty[spi].min_y);
						break;
				}
			}
		}
	}
	/*----------------------------------------------*/
	if (Ypos.size()>0){
		Hs=cui_GeneralImgProcess::GetAverageValue(Ypos.data(),Ypos.size());
	}else{
		Hs=0;//已到达top位置
	}
	
#endif	

	float HsLimit=ImgFocus*tan(CameraOffAngle)+pMD->Seg_HorizontalLinePos;
	Hs=(HsLimit<Hs)?HsLimit:Hs;

	Hs=(pMD->Seg_HorizontalLinePos-Hs)/(pMD->ImgHeight);
	return Hs;
}
/*-------------------------------------------------------------------*/
/**
*判断迭代是否收敛
*/
/*-------------------------------------------------------------------*/
bool HsHg::IsConvergence(void)
{
	float PsOffset=this->GetHs_Nogeometry();
	float PgOffset=this->GetHg_Nogeometry();

	float sub_s=fabsl(pMD->PsOffset-PsOffset);
	float sub_g=fabsl(pMD->PgOffset-PgOffset);
	if (sub_s<0.01&&sub_g<0.01){
		return true;
	}else{
		return false;
	}
	
}
/*-------------------------------------------------------------------*/
/**
*判断迭代是否收敛
*/
/*-------------------------------------------------------------------*/
bool HsHg::IsBorderLineConvergence_5m4d(void)
{
	float PsOffset=pMD->PsOffset;
	float PgOffset=pMD->PgOffset;
	bool IsConvergence=false;
	this->Get_HsHg_By_NBig_FSmall_5m4d();
	float sub_s=fabsl(pMD->PsOffset-PsOffset);
	float sub_g=fabsl(pMD->PgOffset-PgOffset);
	if (sub_s<0.01&&sub_g<0.01){
		IsConvergence=true;
	}else{
		IsConvergence=false;
	}

	//this->IsBoderCategoryChange_5m4d();
	return IsConvergence;
}
/*-------------------------------------------------------------------*/
/**
*检查边界（地面边界，天空边界）上的分类是否相同
*/
/*-------------------------------------------------------------------*/
bool HsHg::IsBoderCategoryChange_5m4d(void)
{
#if 0
INT32* Matrix_Category_Lable=pMD->Matrix_Category_Lable_InDoor.get();
	INT32* PositionCategoryForce=pMD->PositionCategoryForce.get();
	INT32* SkyBoder=Matrix_Category_Lable_SkyBoder.get();
	INT32* GndBoder=Matrix_Category_Lable_GndBoder.get();

	for (int i=0;i<pMD->slic_current_num;i++){
		if ((PositionCategoryForce[i]&SKY_Force)!=0){
			if (SkyBoder[i]!=Matrix_Category_Lable[i]){
				return true;
			}
		}
		if ((PositionCategoryForce[i]&GROUND_Force)!=0){
			 if ( GndBoder[i]!=Matrix_Category_Lable[i]){
				 return true;
			 }
			
		}

	}

	return false;
#endif
		int i=0;
		
		for (int i=0;i<this->HgHsLine.size();i++){
			if (this->HgHsLine[i].SkyGndCategoryChange==true){
				ASSERT(this->HgHsLine.size()>=1);
				pMD->PgOffset=this->HgHsLine[i-1].PgOffset;
				pMD->PsOffset=this->HgHsLine[i-1].PsOffset;
				//跳出循环
				return true;

			}
		}

		//继续执行循环
		return false;

}

/*-------------------------------------------------------------------*/
/**
*记录地面、天空边界属性
*/
/*-------------------------------------------------------------------*/
void HsHg::RecordBoderCategory_5m4d(void)
{
	INT32* Matrix_Category_Lable=pMD->Matrix_Category_Lable_InDoor.get();
	//TOP BOTTOM 属性
	INT32* PositionCategoryForce=pMD->PositionCategoryForce.get();
	INT32* SkyBoder=Matrix_Category_Lable_SkyBoder.get();
	INT32* GndBoder=Matrix_Category_Lable_GndBoder.get();
	memset(SkyBoder,0,sizeof(INT32)*pMD->slic_current_num);
	memset(GndBoder,0,sizeof(INT32)*pMD->slic_current_num);
	for (int i=0;i<pMD->slic_current_num;i++){
		if ((PositionCategoryForce[i]&SKY_Force)!=0){
			SkyBoder[i]=Matrix_Category_Lable[i];
		}
	    if ((PositionCategoryForce[i]&GROUND_Force)!=0){
			GndBoder[i]=Matrix_Category_Lable[i];
	    }

	}

}
/*-------------------------------------------------------------------*/
/**
*检查边界（地面边界，天空边界）上的分类是否变化\n
*同时，保存数据到HgHsLine
*/
/*-------------------------------------------------------------------*/
void HsHg::CheackBoderCategory_5m4d(void)
{
	INT32* Matrix_Category_Lable=pMD->Matrix_Category_Lable_InDoor.get();
	INT32* PositionCategoryForce=pMD->PositionCategoryForce.get();
	INT32* SkyBoder=Matrix_Category_Lable_SkyBoder.get();
	INT32* GndBoder=Matrix_Category_Lable_GndBoder.get();
	bool IsChange=false;
	for (int i=0;i<pMD->slic_current_num;i++){
		if ((PositionCategoryForce[i]&SKY_Force)!=0){
			if (SkyBoder[i]!=Matrix_Category_Lable[i]){
				IsChange=true;
			}
		}
		if ((PositionCategoryForce[i]&GROUND_Force)!=0){
			if ( GndBoder[i]!=Matrix_Category_Lable[i]){
				 IsChange=true;
			}

		}

	}
	HgHsLine.push_back(HgHsLinePos(pMD->PsOffset,pMD->PgOffset,IsChange));
	
}
/*-------------------------------------------------------------------*/
/**
*判断迭代是否收敛
*/
/*-------------------------------------------------------------------*/
bool HsHg::IsConvergence_5m4d(void)
{
	float PsOffset=pMD->PsOffset;
	float PgOffset=pMD->PgOffset;
	//到达收敛条件(则退出)
	if (this->IsBorderLineConvergence_5m4d()){
		this->IsBoderCategoryChange_5m4d();
		return true;
	}

	if (this->IsBoderCategoryChange_5m4d()){
		return true;
	}
	return false;


}
/*-------------------------------------------------------------------*/
/**
*利用天花板和地面的梯形透视得到梯形各处的值
*@param SkyY 
*@param GndY 
*@note  天花板自上而下梯形\n
*       地面自下而上梯形
*/
/*-------------------------------------------------------------------*/
void HsHg::StatisticsTrapezoidHgHs_5m4d(vector<float>& SkyY,vector<float>& GndY)
{
	SkyY.clear();
	GndY.clear();
	SkyY.resize(pMD->ImgHeight,-1);
	GndY.resize(pMD->ImgHeight,-1);
	int* Lables=pMD->ImgLables.get();
	INT32* Category=pMD->Matrix_Category_Lable_InDoor.get();

	float HgPos= pMD->Seg_HorizontalLinePos+pMD->PgOffset*pMD->ImgHeight;
	float HsPos= pMD->Seg_HorizontalLinePos-pMD->PsOffset*pMD->ImgHeight;

	for (int x=0;x<pMD->ImgWidth;x++){
		for (int y=0;y<HsPos;y++){
			int  PixelIndex=y*pMD->ImgWidth+x;
			if (PixelIndex>=0&&PixelIndex<pMD->ImgHeight*pMD->ImgWidth){
				int spi=Lables[PixelIndex];
				if (Category[spi]==Sky){
					SkyY[y]+=1;
				}
			}
			
		}
	}

	for (int y=HgPos;y<pMD->ImgHeight;y++){
			for(int x=0;x<pMD->ImgWidth;x++){
				int  PixelIndex=y*pMD->ImgWidth+x;
				if (PixelIndex>=0&&PixelIndex<pMD->ImgHeight*pMD->ImgWidth){
						int spi=Lables[PixelIndex];
						if (Category[spi]==Ground){
							GndY[y]+=1;
						}
				}
				

			}		
	}
}
/*-------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------*/