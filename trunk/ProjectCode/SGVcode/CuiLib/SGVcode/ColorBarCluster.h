#pragma once
#include <platform_cui.h>
/*-------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------*/
class ColorBarCluster
{
public:
	ColorBarCluster(ImageData* pIMDt);
	~ColorBarCluster(void);
public:
	ImageData* pIMD; 
	vector<vector<int>> SimilarSet;
public:
	void Clustering(void);
	void Clustering_ByHistogramMaxHist_NoIterationColor(int ColorangleSpan);
	void Clustering_ByHistogramOneColorGray(void);
	void HistogramOne2Matrix_Category_Lable(INT32 *Matrix_Category_Lable,KseedsHistData& seeddata);
	void HistogramOne2Matrix_Category_Lable(INT32 *Matrix_Category_Lable,KseedsHistColorGray& seeddata);
public:
	void HistColorGray2SimilarSet(KseedsHistColorGray seeddata);
	void HistogramRange2Matrix_Category_Lable_SameLink(
		INT32 *Matrix_Category_Lable,
		KseedsHistData& seeddata,
		vector<vector<int>>& SameClassLink,
		int ColorangleSpan);
	void HistogramRange2Matrix_Category_Lable_SameLink(
		KseedsHistData& seeddata,
		vector<vector<int>>& SameClassLink,
		int ColorangleSpan);
};
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/

