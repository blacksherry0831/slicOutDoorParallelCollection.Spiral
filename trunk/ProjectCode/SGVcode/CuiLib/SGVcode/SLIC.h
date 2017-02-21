#pragma once
//#//if !defined(_SLIC_H_INCLUDED_)
//#//define _SLIC_H_INCLUDED_
#include <platform_cui.h>


//#include "ImageMemData.h"
//#include "ImageData.h"
/**能够进行超像素分割的最大值*/
#define  Num_Hist 2048 
#define  UseTextureInColorWithCombine  FALSE 
/*----------------------------------------------------------------------------------------------------------------*/
/** 
*  SLIC   class. 
*  本类完成超像素和谱聚类的功能
*
*/
/*----------------------------------------------------------------------------------------------------------------*/
//#pragma once
class SLIC  
{
public:
	SLIC();
	SLIC(ImageMemData*  MemData_t);
	SLIC(ImageData*  MemData_t);
	virtual ~SLIC();
	//============================================================================
	// Superpixel segmentation for a given step size (superpixel size ~= step*step)
	//============================================================================
        void DoSuperpixelSegmentation_ForGivenSuperpixelSize(
        const unsigned int*                            ubuff,//Each 32 bit unsigned int contains ARGB pixel values.
		const int					width,
		const int					height,
		INT32*&						klabels,
		int&						numlabels,
                const int&					superpixelsize,
                const double&                                   compactness);

		void DoSuperpixelSegmentation_ForGivenSuperpixelSize_sitaMLxy(
			const unsigned int*         ubuff,//Each 32 bit unsigned int contains ARGB pixel values.
			const int					width,
			const int					height,
			INT32*&						klabels,
			int&						numlabels,
			const int&					superpixelsize);
		void DoSuperpixelSegmentation_ForGivenSuperpixelSize_sitaMLxyIncompletion(
			const unsigned int*         ubuff,
			const int					width,
			const int					height,
			INT32*&						klabels,
			int&						numlabels,
			const int&					superpixelsize);

	//============================================================================
	// Superpixel segmentation for a given number of superpixels
	//============================================================================
        void DoSuperpixelSegmentation_ForGivenNumberOfSuperpixels(
        const UINT32*                             ubuff,
		const int					width,
		const int					height,
		int*&						klabels,
		int&						numlabels,
                const int&			K,//required number of superpixels
                const double&      compactness);//10-20 is a good value for CIELAB space

		void DoSuperpixelSegmentation_ForGivenNumberOfSuperpixels_sitaMLxy(
			const UINT32*               ubuff,
			const int					width,
			const int					height,
			int*&						klabels,
			int&						numlabels);
	//============================================================================
	// Supervoxel segmentation for a given step size (supervoxel size ~= step*step*step)
	//============================================================================
	void DoSupervoxelSegmentation(
		unsigned int**&		ubuffvec,
		const int&					width,
		const int&					height,
		const int&					depth,
		int**&						klabels,
		int&						numlabels,
                const int&					supervoxelsize,
                const double&                                   compactness);
	//============================================================================
	// Save superpixel labels in a text file in raster scan order
	//============================================================================
	void SaveSuperpixelLabels(
		int*&					labels,
		const int&					width,
		const int&					height,
		const string&				filename="SuperpixelLabels.data",
		const string&				path="");
	//============================================================================
	// Save supervoxel labels in a text file in raster scan, depth order
	//============================================================================
	void SaveSupervoxelLabels(
		const int**&				labels,
		const int&					width,
		const int&					height,
		const int&					depth,
		const string&				filename,
		const string&				path);
	//============================================================================
	// Function to draw boundaries around superpixels of a given 'color'.
	// Can also be used to draw boundaries around supervoxels, i.e layer by layer.
	//============================================================================
public:
	double* Cui_Matrix_W_Vein;
public:
	string FileReadFullPath;
	string FileWritePath;
private:
	//============================================================================
	// The main SLIC algorithm for generating superpixels
	//============================================================================
	void PerformSuperpixelSLIC(
		vector<double>&				kseedsl,
		vector<double>&				kseedsa,
		vector<double>&				kseedsb,
		vector<double>&				kseedsx,
		vector<double>&				kseedsy,
		int*&						klabels,
		const int&					STEP,
                const vector<double>&		edgemag,
		const double&				m = 10.0);
	//============================================================================
	// The main SLIC algorithm for generating supervoxels
	//============================================================================
	void PerformSupervoxelSLIC(
		vector<double>&				kseedsl,
		vector<double>&				kseedsa,
		vector<double>&				kseedsb,
		vector<double>&				kseedsx,
		vector<double>&				kseedsy,
		vector<double>&				kseedsz,
		int**&						klabels,
		const int&					STEP,
		const double&				compactness);
	//============================================================================
	// Pick seeds for superpixels when step size of superpixels is given.
	//============================================================================
	void GetLABXYSeeds_ForGivenStepSize(
		vector<double>&				kseedsl,
		vector<double>&				kseedsa,
		vector<double>&				kseedsb,
		vector<double>&				kseedsx,
		vector<double>&				kseedsy,
		const int&					STEP,
		const bool&					perturbseeds,
		const vector<double>&		edgemag);
	//============================================================================
	// Pick seeds for supervoxels
	//============================================================================
	void GetKValues_LABXYZ(
		vector<double>&				kseedsl,
		vector<double>&				kseedsa,
		vector<double>&				kseedsb,
		vector<double>&				kseedsx,
		vector<double>&				kseedsy,
		vector<double>&				kseedsz,
		const int&					STEP);
	//============================================================================
	// Move the superpixel seeds to low gradient positions to avoid putting seeds
	// at region boundaries.
	//============================================================================
	void PerturbSeeds(
		vector<double>&				kseedsl,
		vector<double>&				kseedsa,
		vector<double>&				kseedsb,
		vector<double>&				kseedsx,
		vector<double>&				kseedsy,
		const vector<double>&		edges);
	//============================================================================
	// Detect color edges, to help PerturbSeeds()
	//============================================================================
	void DetectLabEdges(
		const double*				lvec,
		const double*				avec,
		const double*				bvec,
		const int&					width,
		const int&					height,
		vector<double>&				edges);
	//============================================================================
	// sRGB to XYZ conversion; helper for RGB2LAB()
	//============================================================================
	/*void RGB2XYZ(
		const int&					sR,
		const int&					sG,
		const int&					sB,
		double&						X,
		double&						Y,
		double&						Z);*/
	//============================================================================
	// sRGB to CIELAB conversion (uses RGB2XYZ function)
	//============================================================================
	/*void RGB2LAB(
		const int&					sR,
		const int&					sG,
		const int&					sB,
		double&						lval,
		double&						aval,
		double&						bval);*/
	//============================================================================
	// sRGB to CIELAB conversion for 2-D images
	//============================================================================
	void DoRGBtoLABConversion(
		const unsigned int*&		ubuff,
		double*&					lvec,
		double*&					avec,
		double*&					bvec);
	//============================================================================
	// sRGB to CIELAB conversion for 3-D volumes
	//============================================================================
	void DoRGBtoLABConversion(
		unsigned int**&				ubuff,
		double**&					lvec,
		double**&					avec,
		double**&					bvec);
	//============================================================================
	// Post-processing of SLIC segmentation, to avoid stray labels.
	//============================================================================
	void EnforceLabelConnectivity(
		const int*					labels,
		const int					width,
		const int					height,
		int*&						nlabels,//input labels that need to be corrected to remove stray labels
		int&						numlabels,//the number of labels changes in the end if segments are removed
		const int&					K); //the number of superpixels desired by the user
	//============================================================================
	// Post-processing of SLIC supervoxel segmentation, to avoid stray labels.
	//============================================================================
	void EnforceSupervoxelLabelConnectivity(
		int**&						labels,//input - previous labels, output - new labels
		const int&					width,
		const int&					height,
		const int&					depth,
		int&						numlabels,
		const int&					STEP);
public:
	/***********************************************************************************/
	static	wstring							Narrow2Wide(
		const string&		narrowString);

	static	string							Wide2Narrow(
		const wstring&		wideString);
	/************************************************************************************/

private:
	int										m_width;
	int										m_height;
	int										m_depth;

	/*double*									m_lvec;
	double*									m_avec;
	double*									m_bvec;*/

	double**								m_lvecvec;
	double**								m_avecvec;
	double**								m_bvecvec;
private:
	ImageMemData* pMD;/**<图像数据及计算的中间变量*/
	ImageData* pIMD;
public:	
	UINT32*  CuiImgData;/**<原始图像*/
	int CuiWidth;
	int CuiHeight;
	IplImage * CuiCvImage;
	IplImage * Cui_ARGB_Image;
public:
	void CuiFindSaveNighbour_E_matrix(
		const UINT32&               SelfNighbour=0,
		const string&				filename="NighbourMatrixPixels.data",
		const string&				path="");
	
	void CuiFindSaveSimilar_W_matrix2(const string& filename="Matrix_W2.data",const string&	path="");
	void CuiFindSaveSimilar_W_matrix2_2016_09_26(const string& filename="Matrix_W2.data",const string&	path="");

	void CuiFindSaveDgeree_D_matrix(void);
	void CuiFindSaveDgeree_D_matrix2016_09_26(void);

	void CuiFindSaveLaplace_L_matrix(void);
	void CuiFindSaveLaplace_L_matrix_2016_09_26(void);

	void CuiFindSave_L_Eigenvalue(void);
	void CuiFindSave_L_Eigenvalue_2016_09_26(void);

	void CuiGetImageData(string filename,string filesavepath);
private:
	void CuiGetImageData(void);
	void CuiGetImageDataPIMD(void);
public:
	void CuiDoSuperpixelSegmentation_ForGivenNumberOfSuperpixels(  int& K, double&  compactness,int savelable=0); 
	void DoSuperpixelSegmentation_ForGivenNumberOfSuperpixels_sitaMLxy(int savelable=0); 
	void Cui_Kmean_Cluster(UINT EigenvectorNum, UINT ClusterNum);
	
	void Cui_B_Cluster(UINT EigenvectorNum, UINT ClusterNum,double Threshold);
	void Cui_B_Cluster_2016_09_27(UINT EigenvectorNum, UINT ClusterNum,double Threshold);
	
	bool Cui_Spectral_Clustering_KM(UINT EigenvectorNum, UINT ClusterNum,double Similar_Threshold);
	
	bool Cui_Spectral_Clustering_B(double EigenvectorNumPercent=0.1, double ClusterPercent=0.1,double Threshold=Spectral_Clustering_B_Threshold);
	
	bool SLIC::Cui_Spectral_Clustering_B_2016_09_26(
		double EigenvectorNumPercent=0.1, 
		double ClusterPercent=0.1,
		double Threshold=Spectral_Clustering_B_Threshold);

	string cuiGetCurrentTime(void);
	void CuiSaveImageData(UINT*&				imgBuffer,
		int					width,
		int					height,
		string&				outFilename,
		string&				saveLocation,
		int					format,
		const string&		str);
	/**************谱聚类中间变量*****************/
			//迭代结果;
	        int*  CuiImgLables;/**<lable图块（超像素） 图像width*height	32位int型数据 从零开始计数*/	
			
			/**<lable 中图块（超像素）的个数*/	
			/********直方图使用**求解W矩阵*********************/			
			IplImage * Cui_lab_hsv_Image;/**<Lab格式的图像文件*/
			IplImage *h_l_plane;/**<Lab格式中，L代表分量的图像文件*/
			IplImage *s_a_plane;/**<Lab格式中，A代表分量的图像文件*/
			IplImage *v_b_plane;/**<Lab格式中，B代表分量的图像文件*/
		
			/**************谱聚类中间变量*****************/			
			UINT32 *Cui_Matrix_E;/**<相邻矩阵*/
			double *Cui_Matrix_D;/**<度矩阵*/
			double *Cui_Matrix_W;/**<相似矩阵W*/
			double *Cui_Matrix_L;/**<拉普拉斯矩阵*/
			float *Cui_MatrixEigenVector_L;/**<L矩阵的N个特征值，按特征值循序排列*/
			double *CUi_MatrixEigenValue_L;/**<L矩阵的N个特征值,按顺序排列*/
			float *Cui_Matrix_Category_Simple;/**<前K个特征值存储的矩阵*/
			INT32 *Cui_Matrix_Category_Lable;/**<Labe图像各块的分类矩阵 */
	/*******************************/		
	int					m_K;	
public:
			double Cui_Find_MaxSimilar(void);
			void Cui_Min_Cluster(void);
			void Cui_SurroundClassification(void);
			
			void InitParameter(void);
			void ExportMemData(void);
			//////////////////////////////////////////////////////
			void showDistance(vector<double> clustersize);
			void PerformSuperpixelSLIC_CUINEW(
				vector<double>&				kseedsl,
				vector<double>&				kseedsa,
				vector<double>&				kseedsb,
				vector<double>&				kseedsx,
				vector<double>&				kseedsy,
				int*&						klabels,
				const int&					STEP,
				const vector<double>&		edgemag,
				const double&				m = 10.0);
			void GetLABXYSeeds_ForGivenStepSize_Rectangle(
				vector<double>&				kseedsl,
				vector<double>&				kseedsa,
				vector<double>&				kseedsb,
				vector<double>&				kseedsx,
				vector<double>&				kseedsy,
				const int&					K,
				const bool&					perturbseeds,
				const vector<double>&		edgemag);

			void GetLABXYSeeds_ForGivenStepSize_Rectangle2(
				vector<double>&				kseedsl,
				vector<double>&				kseedsa,
				vector<double>&				kseedsb,
				vector<double>&				kseedsx,
				vector<double>&				kseedsy,
				int&					    NumLabels,
				const bool&					perturbseeds,
				const vector<double>&		edgemag,
				double*  m_lvec,
				double*  m_avec,
				double*  m_bvec);

			void SLIC::GetLABXYSeeds_Spiral(
				vector<double>&				kseedsl,
				vector<double>&				kseedsa,
				vector<double>&				kseedsb,
				vector<double>&				kseedsx,
				vector<double>&				kseedsy,
				int&					    NumLabels,
				const bool&					perturbseeds,
				const vector<double>&		edgemag,
				double*  m_lvec,
				double*  m_avec,
				double*  m_bvec);


			void ConvertLab2oml(
				double L,
				double A,
				double B,
				double X,
				double Y,
				double& sita_n,
				double& m_n,
				double& L_n,
				double& X_n,
				double& Y_n);
public:
	//vector<double> kseedsl;
	//vector<double> kseedsa;
	//vector<double> kseedsb;
	//vector<double> kseedsx;
	//vector<double> kseedsy;
				float alpha;
				float betta;
				float gama;
				float fai;
public:
	double CalculateNewDistance(
		double sita_n0,
		double m_n0,
		double L_n0,
		double X_n0,
		double Y_n0,
		double sita_n1,
		double m_n1,
		double L_n1,
		double X_n1,
		double Y_n1);
	unsigned int GetGCD(unsigned int a, unsigned int b);
	void FillImgWithSeeds(
		vector<double> kseedsl,
		vector<double> kseedsa,
		vector<double> kseedsb,
		vector<double> kseedsx,
		vector<double> kseedsy);
	void Clustering_ByHistogramOne(void);
	void Clustering_ByHistogramOneColorGray(void);
	void HistogramOne2Matrix_Category_Lable(INT32 *Matrix_Category_Lable,KseedsHistData& seeddata);
	void HistogramOne2Matrix_Category_Lable(INT32 *Matrix_Category_Lable,KseedsHistColorGray& seeddata);
	void Clustering_ByHistogramMaxHist(int ColorangleSpan);
	void Clustering_ByHistogramMaxHist_NoIteration(int ColorangleSpan);
	void Clustering_ByHistogramMaxHist_NoIterationColor(int ColorangleSpan);

	void HistogramRange2Matrix_Category_Lable(
		INT32 *Matrix_Category_Lable,
		INT32 *Matrix_Category_SP_USED,
		KseedsHistData& seeddata,
		int ColorangleSpan);

	void HistogramRange2Matrix_Category_Lable_SameLink(
		INT32 *Matrix_Category_Lable,
		KseedsHistData& seeddata,
		vector<vector<int>>& SameClassLink,
		int ColorangleSpan);

	void Clustering_New(void);

};

//#endif // !defined(_SLIC_H_INCLUDED_)
