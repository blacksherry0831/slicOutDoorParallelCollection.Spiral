#pragma once
/*---------------------------------------------------------------------------------*/
/** 
*  SpectralClustering   class. 
*  
*/
/*---------------------------------------------------------------------------------*/
class SpectralClustering
{
private:
	
public:
	SpectralClustering(void);
	~SpectralClustering(void);
public:
	void DrawHistogramOnImg(void);
	static	void CombinationImgLabs_inIteration(
		unsigned int*			ubuff,
		int*					labels,
		INT32*			   category,
		const double*          cui_Matrix_W,		
		int&				numlabels,
		const int&				width,
		const int&				height,
		double SuperpixelThread,
		ImageMemData* pMD);

	static  void DFS_4First2End(
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
		ImageMemData* pMD);
	static void Init_CombineLink(ImageMemData* pMD);
};

