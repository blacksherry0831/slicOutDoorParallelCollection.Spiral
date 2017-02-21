#include "StdAfx.h"
#include "module_all_cui.h"
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
ColorBarCluster::ColorBarCluster(ImageData* pIMDt)
{
	 this->pIMD=pIMDt;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
ColorBarCluster::~ColorBarCluster(void)
{
	
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void ColorBarCluster::Clustering(void)
{
	TRACE_FUNC();
#if _MSC_VER &&_DEBUG
	LARGE_INTEGER litmp;
	LONGLONG QPart1,QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// 获得初始值
#endif
/*---------------------------------------------------*/
do {
	this->Clustering_ByHistogramOneColorGray();
	this->Clustering_ByHistogramMaxHist_NoIterationColor(10);
} while (0);

pIMD->SurroundClassification();

#if SaveAverageImg2Disk && _DEBUG
  pIMD->ImageGetSeedsLabxy_cuda();

cui_GeneralImgProcess::Draw_Kseeds_AverageImg(
	kseedsl,kseedsa,kseedsb,
	CuiImgLables,
	m_width,
	m_height,
	FileReadFullPath,
	FileWritePath);
#endif
#if _MSC_VER &&_DEBUG
	/*---------------------------------------------------*/
	QueryPerformanceCounter(&litmp);
	QPart2 = litmp.QuadPart;//获得中止值
	dfMinus = (double)(QPart2-QPart1);
	dfTim = dfMinus / dfFreq;// 获得对应的时间值，单位为秒
	TRACE("\n 单条多条聚类: %f（秒）",dfTim);
#endif

}

/*---------------------------------------------------------------------------------*/
/**
*将不同颜色角度统计到N维直方图中\n
*每一维直方图单独聚类，分别完成彩色聚类和灰度聚类
*
*/
/*---------------------------------------------------------------------------------*/
void ColorBarCluster::Clustering_ByHistogramOneColorGray(void)
{
	TRACE_FUNC();
#if _MSC_VER &&_DEBUG
    LARGE_INTEGER litmp;
	LONGLONG QPart1,QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// 获得初始值
#endif	
	TimeCountStart();
	/*---------------------------------------------------*/
#if	1
	int old_slic_num;
	int new_slic_num;
	/*---------------------------------------------------*/
	const int HistDimSPLABColor=60;
	const int HistDimSPLgray=16;
	do{
		old_slic_num=pIMD->slic_current_num;
		pIMD->ImageGetSeedsLabxy_cuda();
		//获取连接矩阵
		pIMD->GetMatrixE();
		//	绘制直方图
		//获得直方图数据
		KseedsHistColorGray seeddata;
		seeddata.ColorHist.resize(HistDimSPLABColor);
		seeddata.GrayHist.resize(HistDimSPLgray);

		cui_GeneralImgProcess::Get_Kseeds_Histogram(
			pIMD->kseedsl,
			pIMD->kseedsa,
			pIMD->kseedsb,
			pIMD->kseedsx,
			pIMD->kseedsy,
			seeddata,
			pIMD->src_ImgLabels,
			pIMD->ImgWidth,
			pIMD->ImgHeight,
			pIMD->FileReadFullPath,
			pIMD->FileWritePath);
		/**/
#if _DEBUG
		cui_GeneralImgProcess::Draw_Kseeds_Histogram(
			pIMD->kseedsl,
			pIMD->kseedsa,
			pIMD->kseedsb,
			pIMD->kseedsx,
			pIMD->kseedsy,
			seeddata.ColorHist,
			pIMD->src_ImgLabels,pIMD->ImgWidth,pIMD->ImgHeight,
			pIMD->FileReadFullPath,pIMD->FileWritePath);
		cui_GeneralImgProcess::Draw_Kseeds_Histogram(
			pIMD->kseedsl,
			pIMD->kseedsa,
			pIMD->kseedsb,
			pIMD->kseedsx,
			pIMD->kseedsy,
			seeddata.GrayHist,
			pIMD->src_ImgLabels,
			pIMD->ImgWidth,pIMD->ImgHeight,
			pIMD->FileReadFullPath,pIMD->FileWritePath);
#endif


		{
#if _MSC_VER && _DEBUG
			LARGE_INTEGER litmp;
			LONGLONG QPart1,QPart2;
			double dfMinus, dfFreq, dfTim;
			QueryPerformanceFrequency(&litmp);
			dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
			QueryPerformanceCounter(&litmp);
			QPart1 = litmp.QuadPart;// 获得初始值
			////////////////////////////////////////////////////
#endif
			
			HistColorGray2SimilarSet(seeddata);
			/////////////////////////////////////////////////////
#if _MSC_VER &&_DEBUG
			QueryPerformanceCounter(&litmp);
			QPart2 = litmp.QuadPart;//获得中止值
			dfMinus = (double)(QPart2-QPart1);
			dfTim = dfMinus / dfFreq;// 获得对应的时间值，单位为秒
			TRACE("\n 分类: %f（秒）",dfTim);		
#endif
			////////////////////////////////////
		}
		{
#if _MSC_VER &&_DEBUG
	       LARGE_INTEGER litmp;
			LONGLONG QPart1,QPart2;
			double dfMinus, dfFreq, dfTim;
			QueryPerformanceFrequency(&litmp);
			dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
			QueryPerformanceCounter(&litmp);
			QPart1 = litmp.QuadPart;// 获得初始值
			////////////////////////////////////////////////////
#endif
			pIMD->Combination_ImgLabs(SimilarSet,true,false,true);

#if _MSC_VER && _DEBUG
	        /////////////////////////////////////////////////////
			QueryPerformanceCounter(&litmp);
			QPart2 = litmp.QuadPart;//获得中止值
			dfMinus = (double)(QPart2-QPart1);
			dfTim = dfMinus / dfFreq;// 获得对应的时间值，单位为秒
			TRACE("\n 组合: %f（秒）",dfTim);
#endif
		
		}

#if	SaveContours2Disk
		pIMD->SaveImgWithContours();
#endif
		
		new_slic_num=pIMD->slic_current_num;
//	}while (old_slic_num!=new_slic_num);
		}while (0);
	//////////////////////////////////////
	/*---------------------------------------------------*/
#endif
	/*---------------------------------------------------*/
#if _MSC_VER &&_DEBUG
	QueryPerformanceCounter(&litmp);
	QPart2 = litmp.QuadPart;//获得中止值
	dfMinus = (double)(QPart2-QPart1);
	dfTim = dfMinus / dfFreq;// 获得对应的时间值，单位为秒
	TRACE("\n 单条聚类: %f（秒）",dfTim);
#endif
	TimeCountStop("######Gray&Color Bar Hist Cost Time (s) : ");
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void ColorBarCluster::Clustering_ByHistogramMaxHist_NoIterationColor(int ColorangleSpan)
{
#if _MSC_VER &&_DEBUG
    LARGE_INTEGER litmp;
	LONGLONG QPart1,QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// 获得初始值
#endif

	TimeCountStart();

	int HistDimSPLABColor=45;
	int old_slic_num;
	int new_slic_num;
	int IterationTimes=2-1;
	KseedsHistColorGray seeddata;
	seeddata.ColorHist.resize(HistDimSPLABColor);
	vector<vector<int>> SameLink;
	
	do{
		old_slic_num=pIMD->slic_current_num;
		seeddata.ColorHist.ClearUsedHistArray();
		/////////////////////////////////////
		pIMD->ImageGetSeedsLabxy_cuda();

		pIMD->GetMatrixE();

		cui_GeneralImgProcess::Get_Kseeds_Histogram(
			pIMD->kseedsl,
			pIMD->kseedsa,
			pIMD->kseedsb,
			pIMD->kseedsx,
			pIMD->kseedsy,
			seeddata,
			pIMD->src_ImgLabels,
			pIMD->ImgWidth,
			pIMD->ImgHeight,
			pIMD->FileReadFullPath,
			pIMD->FileWritePath);
#if _DEBUG
		cui_GeneralImgProcess::Draw_Kseeds_Histogram(
			pIMD->kseedsl,
			pIMD->kseedsa,
			pIMD->kseedsb,
			pIMD->kseedsx,
			pIMD->kseedsy,
			seeddata.GrayHist,
			pIMD->src_ImgLabels,
			pIMD->ImgWidth,pIMD->ImgHeight,
			pIMD->FileReadFullPath,pIMD->FileWritePath);
#endif
		
		do {
			SameLink.clear();
			do {
				seeddata.ColorHist.removeUsedHist();
				this->HistogramRange2Matrix_Category_Lable_SameLink(seeddata.ColorHist,SameLink,ColorangleSpan);
#if _DEBUG
				cui_GeneralImgProcess::Draw_Kseeds_Histogram(
					pIMD->kseedsl,
					pIMD->kseedsa,
					pIMD->kseedsb,
					pIMD->kseedsx,
					pIMD->kseedsy,
					seeddata.GrayHist,
					pIMD->src_ImgLabels,
					pIMD->ImgWidth,pIMD->ImgHeight,
					pIMD->FileReadFullPath,pIMD->FileWritePath);	
#endif
	
				/*组合图块*/
			} while (seeddata.ColorHist.IsRemoveALL()!=true);
		} while (0);

#if		UseTextureInColorWithCombine && _DEBUG
		cui_GeneralImgProcess::CalculateAllSpPropertyRange(
			CuiImgLables,
			pIMD->ImgWidth,pIMD->ImgHeight,
			pIMD->p_SpProperty,
			pIMD->slic_current_num);
		cui_GeneralImgProcess::CalculateAllSpBlockEnergy(
			pIMD->slic_current_num,
			pIMD->p_SpProperty,
			pIMD->Src_ImgData,
			CuiImgLables,
			pIMD->ImgWidth,pIMD->ImgHeight);
		cui_GeneralImgProcess::Cui_Combination_ImgLabsWithTextureInColor(
			CuiImgData,CuiImgLables,
			Matrix_Category_Lable,
			Matrix_W,
			pIMD->slic_current_num,
			pIMD->ImgWidth,pIMD->ImgHeight,
			0.9,pIMD);

#else
		
#endif
		pIMD->Combination_ImgLabs(SameLink,true,false,true);
#if SaveContours2Disk
		pIMD->SaveImgWithContours();
#endif
	/*	cui_GeneralImgProcess::Cui_Combination_ImgLabs2(
			pIMD->src_ImgBGRA,
			pIMD->src_ImgLabels,
			Matrix_Category_Lable,
			Matrix_W,
			pIMD->slic_current_num,
			pIMD->ImgWidth,pIMD->ImgHeight,
			0.9,pIMD);		*/
		////////////////////////////////
		new_slic_num=pIMD->slic_current_num;
	} 
	//while (old_slic_num!=new_slic_num);
	while (0);
	/////////////////////////////////////
	pIMD->ImageGetSeedsLabxy_cuda();
#if _DEBUG
	cui_GeneralImgProcess::Get_Kseeds_Histogram(
		pIMD->kseedsl,
		pIMD->kseedsa,
		pIMD->kseedsb,
		pIMD->kseedsx,
		pIMD->kseedsy,
		seeddata,
		pIMD->src_ImgLabels,
		pIMD->ImgWidth,
		pIMD->ImgHeight,
		pIMD->FileReadFullPath,
		pIMD->FileWritePath);
	seeddata.ColorHist.ClearUsedHistArray();
	cui_GeneralImgProcess::Draw_Kseeds_Histogram(
		pIMD->kseedsl,
		pIMD->kseedsa,
		pIMD->kseedsb,
		pIMD->kseedsx,
		pIMD->kseedsy,
		seeddata.GrayHist,
		pIMD->src_ImgLabels,
		pIMD->ImgWidth,pIMD->ImgHeight,
		pIMD->FileReadFullPath,pIMD->FileWritePath);
#endif

	TimeCountStop("######Multy Color Bar Cost Time (s) :");
#if _MSC_VER &&_DEBUG
    QueryPerformanceCounter(&litmp);
	QPart2 = litmp.QuadPart;//获得中止值
	dfMinus = (double)(QPart2-QPart1);
	dfTim = dfMinus / dfFreq;// 获得对应的时间值，单位为秒
	TRACE("\n 多条聚类: %f（秒）",dfTim);
#endif
	
}
/*---------------------------------------------------------------------------------*/
/**
*
*
*/
/*---------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------*/
/**
*
*
*/
/*---------------------------------------------------------------------------------*/
void ColorBarCluster::HistogramOne2Matrix_Category_Lable(INT32 *Matrix_Category_Lable,KseedsHistData& seeddata)
{
#if 1
	int HistDimSPLAB=seeddata.histDimSPLAB;
	INT32 Matrix_CL_TEST[1024];
	for(int i=0;i<pIMD->slic_current_num;i++){
		Matrix_Category_Lable[i]=i;
		Matrix_CL_TEST[i]=i;
	}
	for (int j=0;j<HistDimSPLAB;j++){
		int SamSP=seeddata.hist_AB_detail[j].size();
		for (int i=0;i<SamSP;i++){
			if (SamSP>=2&&i<SamSP-1){
				int orgIdx=seeddata.hist_AB_detail[j].at(0);
				int changeIdx=seeddata.hist_AB_detail[j].at(i+1);
				Matrix_Category_Lable[changeIdx]=orgIdx;
				Matrix_CL_TEST[changeIdx]=orgIdx;
			}
		}
	}
#endif	
}
/*---------------------------------------------------------------------------------*/
/**
*
*
*/
/*---------------------------------------------------------------------------------*/
void ColorBarCluster::HistogramOne2Matrix_Category_Lable(INT32 *Matrix_Category_Lable,KseedsHistColorGray& seeddata)
{

	int HistDimSPLABColor=seeddata.ColorHist.histDimSPLAB;
	int HistDimSPLGray=seeddata.GrayHist.histDimSPLAB;
	INT32 Matrix_CL_TEST[1024];
	for(int i=0;i<pIMD->slic_current_num;i++){
		Matrix_Category_Lable[i]=i;
		Matrix_CL_TEST[i]=i;
	}
#if 1	
	for (int j=0;j<HistDimSPLABColor;j++){
		int SamSP=seeddata.ColorHist.hist_AB_detail[j].size();
		for (int i=0;i<SamSP;i++){
			if (SamSP>=2&&i<SamSP-1){
				int orgIdx=seeddata.ColorHist.hist_AB_detail[j].at(0);
				int changeIdx=seeddata.ColorHist.hist_AB_detail[j].at(i+1);
				Matrix_Category_Lable[changeIdx]=orgIdx;
				Matrix_CL_TEST[changeIdx]=orgIdx;
			}
		}
	}
#endif	
#if 1
	for (int j=0;j<HistDimSPLGray;j++){
		int SamSP=seeddata.GrayHist.hist_AB_detail[j].size();
		for (int i=0;i<SamSP;i++){
			if (SamSP>=2&&i<SamSP-1){
				int orgIdx=seeddata.GrayHist.hist_AB_detail[j].at(0);
				int changeIdx=seeddata.GrayHist.hist_AB_detail[j].at(i+1);
				Matrix_Category_Lable[changeIdx]=orgIdx;
				Matrix_CL_TEST[changeIdx]=orgIdx;
			}
		}
	}
#endif	
}
/*---------------------------------------------------------------------------------*/
/**
*
*
*/
/*---------------------------------------------------------------------------------*/
void ColorBarCluster::HistColorGray2SimilarSet(KseedsHistColorGray seeddata)
{
	TRACE_FUNC();
	int vectorSize=0;
	int HistDimSPLABColor=seeddata.ColorHist.histDimSPLAB;
	int HistDimSPLGray=seeddata.GrayHist.histDimSPLAB;
	SimilarSet.clear();
#if 1	
	for ( register int j=0;j<HistDimSPLABColor;j++){
		int SamSP=seeddata.ColorHist.hist_AB_detail[j].size();
		if (SamSP>=2){
			vector<int>  simlarSp; 
			for (register int i=0;i<SamSP;i++){			
				int Idx=seeddata.ColorHist.hist_AB_detail[j].at(i);
				simlarSp.push_back(Idx);
			}		
			SimilarSet.push_back(simlarSp);
		}
	}
#endif	
#if 1
	for (register int j=0;j<HistDimSPLGray;j++){
		int SamSP=seeddata.GrayHist.hist_AB_detail[j].size();		
		if (SamSP>=2){
			vector<int>  simlarSp; 
			for (register int i=0;i<SamSP;i++){			
				int Idx=seeddata.GrayHist.hist_AB_detail[j].at(i);
				simlarSp.push_back(Idx);			
			}
		SimilarSet.push_back(simlarSp);
		}
	}
#endif	
}
/*---------------------------------------------------------------------------------*/
/**
*
*
*/
/*---------------------------------------------------------------------------------*/
void ColorBarCluster::HistogramRange2Matrix_Category_Lable_SameLink(
	INT32 *Matrix_Category_Lable,
	KseedsHistData& seeddata,
	vector<vector<int>>& SameClassLink,
	int ColorangleSpan)
{
	int HistDimSPLAB=seeddata.histDimSPLAB;
	int PieceNum=9;
	int cut_step=HistDimSPLAB/PieceNum;
	int range=ColorangleSpan/2;
	int stepLR=1.0*range/360*HistDimSPLAB;
	vector<HistRangeData> histrangedata_t(PieceNum);
	int sort_t[2];
	/*-----获取9个分组的最大值---------------------------------------------------------------------*/
	for(int h = 0; h <HistDimSPLAB; h++){			
		if (h%cut_step==0){
			int rangeIdx=h/cut_step;
			histrangedata_t[rangeIdx].start=&(seeddata.hist_AB_Count[h]);
			histrangedata_t[rangeIdx].len=cut_step;
			histrangedata_t[rangeIdx].angle_up=h*2;
			histrangedata_t[rangeIdx].angle_down=(h+cut_step)*2-1;
			histrangedata_t[rangeIdx].StepLR=stepLR;
			cui_GeneralImgProcess::GetMaxValueIndexdouble(histrangedata_t[rangeIdx].start,cut_step,sort_t,2);
			if (histrangedata_t[rangeIdx].start[sort_t[0]]==0){
				histrangedata_t[rangeIdx].maxHistIdx=-1;
				histrangedata_t[rangeIdx].maxHistLeft=0;
				histrangedata_t[rangeIdx].maxHistRight=0;
				histrangedata_t[rangeIdx].maxHistCenter=0;
			}else{
				histrangedata_t[rangeIdx].maxHistIdx=sort_t[0]+h;
				histrangedata_t[rangeIdx].maxHistLeft=&(histrangedata_t[rangeIdx].start[sort_t[0]-stepLR]);
				histrangedata_t[rangeIdx].maxHistRight=&(histrangedata_t[rangeIdx].start[sort_t[0]+stepLR]);
				histrangedata_t[rangeIdx].maxHistCenter=&(histrangedata_t[rangeIdx].start[sort_t[0]]);
				///////////////////////////////////////////////////////////////////////////////////
				histrangedata_t[rangeIdx].maxHistLeft=(histrangedata_t[rangeIdx].maxHistLeft<&seeddata.hist_AB_Count[0])?&seeddata.hist_AB_Count[0]:histrangedata_t[rangeIdx].maxHistLeft;
				histrangedata_t[rangeIdx].maxHistRight=(histrangedata_t[rangeIdx].maxHistRight>&seeddata.hist_AB_Count[seeddata.histDimSPLAB-1])?&seeddata.hist_AB_Count[seeddata.histDimSPLAB-1]:histrangedata_t[rangeIdx].maxHistRight;
				///////////////////////////////////////////////////////////////////////////////////
			}
		}		
	}
	/*-----对获取9个分组的最大值进行校正-------------------------------------------*/
	for (int i=0;i<PieceNum;i++){
		unsigned int Dim=(unsigned int)histrangedata_t[i].maxHistRight-(unsigned int)histrangedata_t[i].maxHistLeft;
		Dim=Dim/sizeof(double);
		if (Dim!=0){
			Dim+=1;
			cui_GeneralImgProcess::GetMaxValueIndexdouble(histrangedata_t[i].maxHistLeft,Dim,sort_t,sizeof(sort_t)/sizeof(int));
			if (histrangedata_t[i].maxHistCenter!=&histrangedata_t[i].maxHistLeft[sort_t[0]]){
				histrangedata_t[i].maxHistIdx=-1;
				histrangedata_t[i].maxHistLeft=0;
				histrangedata_t[i].maxHistRight=0;
				histrangedata_t[i].maxHistCenter=0;
			}
		}	
	}
	/*---------------记录参与本次聚类的直方图-------------------------------------------------------------*/
	for (int i=0;i<PieceNum;i++){
		if (histrangedata_t[i].maxHistIdx!=-1){
			int SameClassTop=histrangedata_t[i].maxHistIdx+histrangedata_t[i].StepLR;
			int SameClassDown=histrangedata_t[i].maxHistIdx-histrangedata_t[i].StepLR;
			if (SameClassDown<0){
				SameClassDown=0;
			}
			if (SameClassTop>HistDimSPLAB-1){
				SameClassTop=HistDimSPLAB-1;
			}
			for (int j=SameClassDown;j<=SameClassTop;j++){
				seeddata.hist_AB_CountRemove[j]=-1;
			}			
		}	
	}
	/*----------------------------------------------------------------------------*/
#if 0
	/*-----对获取9个分组的范围检查-------------------------------------------*/
	for (int i=0;i<PieceNum-1;i++){
		unsigned int Dim=(unsigned int)histrangedata_t[i].maxHistRight-(unsigned int)histrangedata_t[i].maxHistLeft;
		Dim=Dim/sizeof(double);
		if (Dim!=0){
			unsigned int range_1Right=(unsigned int)histrangedata_t[i].maxHistRight;
			unsigned int range_2Left=(unsigned int)histrangedata_t[i+1].maxHistLeft;
			if (range_1Right>range_2Left){
				unsigned int index1=histrangedata_t[i].maxHistIdx;
				unsigned int index2=histrangedata_t[i+1].maxHistIdx;
				double value_1=seeddata.hist_AB_Count[index1];
				double value_2=seeddata.hist_AB_Count[index2];
				if (value_1<value_2){
					histrangedata_t[i].maxHistIdx=-1;
					histrangedata_t[i].maxHistLeft=0;
					histrangedata_t[i].maxHistRight=0;
					histrangedata_t[i].maxHistCenter=0;
				}else{
					histrangedata_t[i+1].maxHistIdx=-1;
					histrangedata_t[i+1].maxHistLeft=0;
					histrangedata_t[i+1].maxHistRight=0;
					histrangedata_t[i+1].maxHistCenter=0;
				}
			}
		}	
	}
#endif
#if 1	
	/*-----对获取9个分组进行填充------------------------------------------*/
	for (int i=0;i<PieceNum;i++){
		if (histrangedata_t[i].maxHistIdx>0){
			int SameClassTop=histrangedata_t[i].maxHistIdx+histrangedata_t[i].StepLR;
			int SameClassDown=histrangedata_t[i].maxHistIdx-histrangedata_t[i].StepLR;
			if (SameClassDown<0){
				SameClassDown=0;
			}
			if (SameClassTop>HistDimSPLAB-1){
				SameClassTop=HistDimSPLAB-1;
			}
			/////////////////////////////////////////////
			SameClassLink.push_back(vector<int>());
			int SameClassLinkIdx=SameClassLink.size()-1;
			//////////////////////////////////////////////
			for (int j=SameClassDown;j<=SameClassTop;j++){
				int SamSP=seeddata.hist_AB_detail[j].size();
				for (int k=0;k<SamSP;k++){					
					int SameIdx_t=seeddata.hist_AB_detail[j].at(k);
					SameClassLink[SameClassLinkIdx].push_back(SameIdx_t);
				}
			}
		}
	}	
#endif
#if 0
	/*-----填充Labels矩阵-------------------------------------------------------------*/
	INT32 Matrix_CL_TEST[2048];
	///初始化Category
	for(int i=0;i<pMD->slic_current_num;i++){
		Matrix_Category_Lable[i]=i;
		Matrix_CL_TEST[i]=i;
	}
	/*--修正Category--------------------------------------------*/
	for (int j=0;j<SameClassLink.size();j++){
		int SamSP=SameClassLink[j].size();
		for (int i=0;i<SamSP;i++){
			if (SamSP>=2&&i<SamSP-1){
				int orgIdx=SameClassLink[j].at(0);
				int changeIdx=SameClassLink[j].at(i+1);
				Matrix_Category_Lable[changeIdx]=orgIdx;
				Matrix_CL_TEST[changeIdx]=orgIdx;
			}
		}
	}
#endif
}
/*---------------------------------------------------------------------------------*/
/**
*
*
*/
/*---------------------------------------------------------------------------------*/
void ColorBarCluster::HistogramRange2Matrix_Category_Lable_SameLink(
	KseedsHistData& seeddata,
	vector<vector<int>>& SameClassLink,
	int ColorangleSpan)
{
	int HistDimSPLAB=seeddata.histDimSPLAB;
	int PieceNum=9;
	int cut_step=HistDimSPLAB/PieceNum;
	int range=ColorangleSpan/2;
	int stepLR=1.0*range/360*HistDimSPLAB;
	vector<HistRangeData> histrangedata_t(PieceNum);
	int sort_t[2];
	/*-----获取9个分组的最大值---------------------------------------------------------------------*/
	for(int h = 0; h <HistDimSPLAB; h++){			
		if (h%cut_step==0){
			int rangeIdx=h/cut_step;
			histrangedata_t[rangeIdx].start=&(seeddata.hist_AB_Count[h]);
			histrangedata_t[rangeIdx].len=cut_step;
			histrangedata_t[rangeIdx].angle_up=h*2;
			histrangedata_t[rangeIdx].angle_down=(h+cut_step)*2-1;
			histrangedata_t[rangeIdx].StepLR=stepLR;
			cui_GeneralImgProcess::GetMaxValueIndexdouble(histrangedata_t[rangeIdx].start,cut_step,sort_t,2);
			if (histrangedata_t[rangeIdx].start[sort_t[0]]==0){
				histrangedata_t[rangeIdx].maxHistIdx=-1;
				histrangedata_t[rangeIdx].maxHistLeft=0;
				histrangedata_t[rangeIdx].maxHistRight=0;
				histrangedata_t[rangeIdx].maxHistCenter=0;
			}else{
				histrangedata_t[rangeIdx].maxHistIdx=sort_t[0]+h;
				histrangedata_t[rangeIdx].maxHistLeft=&(histrangedata_t[rangeIdx].start[sort_t[0]-stepLR]);
				histrangedata_t[rangeIdx].maxHistRight=&(histrangedata_t[rangeIdx].start[sort_t[0]+stepLR]);
				histrangedata_t[rangeIdx].maxHistCenter=&(histrangedata_t[rangeIdx].start[sort_t[0]]);
				///////////////////////////////////////////////////////////////////////////////////
				histrangedata_t[rangeIdx].maxHistLeft=(histrangedata_t[rangeIdx].maxHistLeft<&seeddata.hist_AB_Count[0])?&seeddata.hist_AB_Count[0]:histrangedata_t[rangeIdx].maxHistLeft;
				histrangedata_t[rangeIdx].maxHistRight=(histrangedata_t[rangeIdx].maxHistRight>&seeddata.hist_AB_Count[seeddata.histDimSPLAB-1])?&seeddata.hist_AB_Count[seeddata.histDimSPLAB-1]:histrangedata_t[rangeIdx].maxHistRight;
				///////////////////////////////////////////////////////////////////////////////////
			}
		}		
	}
	/*-----对获取9个分组的最大值进行校正-------------------------------------------*/
	for (int i=0;i<PieceNum;i++){
		unsigned int Dim=(unsigned int)histrangedata_t[i].maxHistRight-(unsigned int)histrangedata_t[i].maxHistLeft;
		Dim=Dim/sizeof(double);
		if (Dim!=0){
			Dim+=1;
			cui_GeneralImgProcess::GetMaxValueIndexdouble(histrangedata_t[i].maxHistLeft,Dim,sort_t,sizeof(sort_t)/sizeof(int));
			if (histrangedata_t[i].maxHistCenter!=&histrangedata_t[i].maxHistLeft[sort_t[0]]){
				histrangedata_t[i].maxHistIdx=-1;
				histrangedata_t[i].maxHistLeft=0;
				histrangedata_t[i].maxHistRight=0;
				histrangedata_t[i].maxHistCenter=0;
			}
		}	
	}
	/*---------------记录参与本次聚类的直方图-------------------------------------------------------------*/
	for (int i=0;i<PieceNum;i++){
		if (histrangedata_t[i].maxHistIdx!=-1){
			int SameClassTop=histrangedata_t[i].maxHistIdx+histrangedata_t[i].StepLR;
			int SameClassDown=histrangedata_t[i].maxHistIdx-histrangedata_t[i].StepLR;
			if (SameClassDown<0){
				SameClassDown=0;
			}
			if (SameClassTop>HistDimSPLAB-1){
				SameClassTop=HistDimSPLAB-1;
			}
			for (int j=SameClassDown;j<=SameClassTop;j++){
				seeddata.hist_AB_CountRemove[j]=-1;
			}			
		}	
	}
	/*----------------------------------------------------------------------------*/
#if 1	
	/*-----对获取9个分组进行填充------------------------------------------*/
	for (int i=0;i<PieceNum;i++){
		if (histrangedata_t[i].maxHistIdx>0){
			int SameClassTop=histrangedata_t[i].maxHistIdx+histrangedata_t[i].StepLR;
			int SameClassDown=histrangedata_t[i].maxHistIdx-histrangedata_t[i].StepLR;
			if (SameClassDown<0){
				SameClassDown=0;
			}
			if (SameClassTop>HistDimSPLAB-1){
				SameClassTop=HistDimSPLAB-1;
			}
			/////////////////////////////////////////////
			SameClassLink.push_back(vector<int>());
			int SameClassLinkIdx=SameClassLink.size()-1;
			//////////////////////////////////////////////
			for (int j=SameClassDown;j<=SameClassTop;j++){
				int SamSP=seeddata.hist_AB_detail[j].size();
				for (int k=0;k<SamSP;k++){					
					int SameIdx_t=seeddata.hist_AB_detail[j].at(k);
					SameClassLink[SameClassLinkIdx].push_back(SameIdx_t);
				}
			}
		}
	}	
#endif
}
/*---------------------------------------------------------------------------------*/
/**
*
*
*/
/*---------------------------------------------------------------------------------*/