#include "StdAfx.h"
#include "module_all_cui.h"
//#include "SpectralClustering.h"
//#if __GUNC__||linux||__linux||__linux__
//#if 
//
//#endif
//#define memcpy_s(D,Ds,S,Ss)  {memcpy(D,S,Ss)}
//#endif

/*----------------------------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------------------------*/
SpectralClustering::SpectralClustering(void)
{

}
/*----------------------------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------------------------*/
SpectralClustering::~SpectralClustering(void)
{

}
/*----------------------------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------------------------*/
void SpectralClustering::DrawHistogramOnImg(void)
{

}
/*----------------------------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------------------------*/
void SpectralClustering::CombinationImgLabs_inIteration(
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
	UINT32* LinkConn=pMD->LinkConnection.get();
	UINT32* V=pMD->Matrix_Visit.get();

	int *CuiImgLables_t=new int[width*height];
	SP_PROPERTY *p_SpProperty=new SP_PROPERTY[numlabels];

	memcpy_s(CuiImgLables_t,sizeof(int)*width*height,labels,sizeof(int)*width*height);
	
	Init_CombineLink(pMD);
	cui_GeneralImgProcess::CalculateAllSpPropertyRange(labels,width,height,p_SpProperty,numlabels);
	for (int spi=0;spi<numlabels;spi++){
		/*深序遍历*/
		if ( (LinkConn[spi]==TRUE)			
			&&(V[spi]==FALSE)){
				TRACE("深序遍历Root=%d\n",spi);
				DFS_4First2End(spi,ubuff,labels,cui_Matrix_W,numlabels,width,height,SuperpixelThread,p_SpProperty,CuiImgLables_t,pMD);

		}
	}
#if 1
	/******** 标准化lable*******************************************************/
	numlabels=cui_GeneralImgProcess::AdjustLabFrom0toN(CuiImgLables_t,width,height,numlabels);
	/***************************************************************/
#endif

#if _MSC_VER
   memcpy_s(labels,sizeof(int)*width*height,CuiImgLables_t,sizeof(int)*width*height);
#else
   memcpy(labels,CuiImgLables_t,sizeof(int)*width*height);
#endif
	
	
	delete []CuiImgLables_t;
	delete []p_SpProperty;
	cui_GeneralImgProcess::SaveSuperpixelLabels(labels,width,height);
	cui_GeneralImgProcess::CuiSaveImgWithContours(ubuff,labels,width,height,pMD->FileReadFullPath,pMD->FileWritePath,"");
}
/*----------------------------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------------------------*/
void SpectralClustering::DFS_4First2End(
	int                        spi,
	unsigned int*			ubuff,
	int*					labels,
	const double*          cui_Matrix_W,		
	int&				numlabels,
	const int&				width,
	const int&				height,
	double SuperpixelThread,
	SP_PROPERTY *p_SpProperty,
	int *CuiImgLables_t,
	ImageMemData* pMD){
#if 0
/*************************************************************/
		bool IS_Force_CombineLabs;
		if (cui_Matrix_W){ 
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
			IS_Force_CombineLabs=false;
		}else{
			IS_Force_CombineLabs=true;
		}
#endif
		UINT32* V=pMD->Matrix_Visit.get();
		if (V[spi]==FALSE){
			V[spi]=TRUE;
		}else{
			ASSERT(0);
		}		
		/***********************合并相同Lable*************************************************/
		
			for (register int spj=0;spj<numlabels;spj++){ 
				//超像素i,j 属于同一类；
				{
					//判断相邻				
					if (cui_Matrix_W[spi*numlabels+spj]>SuperpixelThread){
						//相似度大于0.9；
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
						if (V[spj]==FALSE){
							DFS_4First2End(spj,ubuff,labels,cui_Matrix_W,numlabels,width,height,SuperpixelThread,p_SpProperty,CuiImgLables_t,pMD);
						}
						
					}
				}
			}
		
		/**************************************************************/


}
/*----------------------------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------------------------*/
void SpectralClustering::Init_CombineLink(ImageMemData* pMD){
	UINT32* LinkConn=pMD->LinkConnection.get();
	UINT32* V=pMD->Matrix_Visit.get();
	memset(V,FALSE,sizeof(UINT32)*pMD->slic_current_num);	
	for (int spi=0;spi<pMD->slic_current_num;spi++){
		LinkConn[spi]=TRUE;
	}
}
/*----------------------------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------------------------*/