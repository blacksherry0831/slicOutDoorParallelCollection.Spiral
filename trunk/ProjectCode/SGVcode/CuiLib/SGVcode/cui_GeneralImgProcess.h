#pragma once
/*----------------------------------------------------------------*/
#include <platform_cui.h>
/*----------------------------------------------------------------*/

#include "ImageType.h"
#include "ImageMemData.h"
#include "ImageData.h"
/*----------------------------------------------------------------*/
#ifdef __GNUC__
#include "ImageData.h"
#endif




#define SaveImg2Disk  TRUE
#define SaveHistgram2Disk       FALSE
#define SaveAverageImg2Disk     FALSE
/*----------------------------------------------------------------*/
/**
*本类实现图像分割，与小波能量计算中，相对独立的一些算法
*
*
*/
/*----------------------------------------------------------------*/
class cui_GeneralImgProcess
{
public:
	cui_GeneralImgProcess(void);
	~cui_GeneralImgProcess(void);
public:
	static int SAVE_IMAGE_2DISK;
	static int SAVE_DEBUG_2DISK;
	//static struct pt_sem SEM_CPU_NUMS;
#if _MSC_VER
	static	HANDLE H_SEM_CPU_NUMS;  
#endif

public:
public:
static int AdjustLabFrom0toN(int *CuiImgLables_t,
	int width,
	int height,
	int sp_num);

static	void Cui_Combination_ImgLabs2(
	unsigned int*			ubuff,
	int*					labels,
	INT32*		   category,
	const double*          cui_Matrix_W,		
	int&				numlabels,
	const int&				width,
	const int&				height,
	double SuperpixelThread,
	ImageMemData* pMD);
	static	void CuiSaveImageData(UINT32*				imgBuffer,
		int					width,
		int					height,
		string&				outFilename,
		string&				saveLocation,
		int					format,
		const string&		str);
	static	string cuiGetCurrentTime(void);
#if Use_CString&&_MSC_VER
	static	string ConvertCS2string(CString cstring);
#endif
	static	void DrawContoursAroundSegments(
		unsigned int*			ubuff,
		int*					labels,
		const int&				width,
		const int&				height,
		const unsigned int&				color );

	void DrawContoursAroundSegments_2PixelBlack(
		unsigned int*			ubuff,
		int*					labels,
		const int&				width,
		const int&				height);
	void DrawContoursAroundSegments_4PixelBlack(
		unsigned int*			ubuff,
		int*					labels,
		const int&				width,
		const int&				height);

	static void CuiSaveImgWithPoints(
		UINT32* ubuff,
		int* labels,
		INT32*               category,
		const int&				width, 	
		const int&				height,
		bool            withcontours,
		ImageMemData*	pMD,
		string fileadd); 
	static void CuiSaveImgWithPoints(
		UINT32* ubuff,
		int* labels,
		INT32*               category,
		const int&				width, 	
		const int&				height,
		bool            withcontours,
		string        filereadfullpath,
		string        filewritepath,
		string fileadd); 
	static void CuiSaveImgWithContours(UINT32* ubuff,
		int* labels,
		const int&				width,
		const int&				height,
		string        filereadfullpath,
		string        filewritepath,
		string fileadd);
   static	void SaveSuperpixelLabels(
		int*					labels,
		const int&					width,
		const int&					height,
		const string&				filename="SuperpixelLabels.data",
		const string&				path="");
  static void DrawTextOnImage(unsigned int*			ubuff,
	   int*					labels,
	   const int&				width,
	   const int&				height);
 static int CuiSetNighbour_E_matrix(
	 int*				labels,
	 const  int         CuiNumLabels,
	 UINT32*            Cui_Matrix_E,
	 const int			width,
	 const int			height,	
	 const string		filename,
	 const string		path);
 static int CuiSetNighbour_E_matrix_UINT32(
	 int*					labels,
	 const  int& CuiNumLabels,
	 UINT32 *Cui_Matrix_E,
	 const int&				width,
	 const int&				height,	
	 const string&				filename,
	 const string&				path);
 static void LinkSurroundClassification(
	 int*					labels,
	 const  int& NumLabels,
	 const int&				width,
	 const int&				height,	
	 INT32*			   category);
static void CuiSetNighbour_D_matrix(
	 double *Cui_Matrix_D,	
	 UINT32 *Cui_Matrix_EW,
	 const  int& CuiNumLabels);
static void InSideClustering(UINT32* ubuff,int*	labels,
	int& NumLabels,
	const int&				width,
	const int&				height,
	string        filereadfullpath,
	string        filewritepath);
static void Cui_CombinationImgSVG(
	UINT32* ubuff,
	int* labels,
	int* labels_SVG,
	INT32*			   category,
	const int&				width,
	const int&				height,
	string        filereadfullpath,
	string        filewritepath,
	string fileadd);
static void InSideClusteringByopencv(
	UINT32* ubuff,
	int*					labels,
	int& NumLabels,
	const int&				width,
	const int&				height,
	string        filereadfullpath,
	string        filewritepath);
/*----------------------------------------*/
static double GetMaxValue(double* Data,long DataNum);
static float  GetMaxValue(float* Data, long DataNum);
static int    GetMaxValue(int* Data, long DataNum);

static float  GetMinValue(float* Data, long DataNum);
static float  GetMinValue(double* Data,long DataNum);
static int    GetMinValue(int* Data, long DataNum);

static double P2PointDiatance(CvPoint point1, CvPoint point2);
static double GetAverageValue(double* Data, int DataNum);
static double GetAverageValue(float* Data, int DataNum);
static double GetSumValue(double* Data, int DataNum);
static double GetSumValue(float* Data, int DataNum);
static double GetVarianceValue(double* Data, int DataNum,double *avg,double *variance);
static double GetVarianceValue(float* Data, int DataNum,float *avg,float *variance);
/*----------------------------------------*/
static void GetMaxValueIndex(float* data, float size, int* sort, int sort_num);
static void GetMaxValueIndexdouble(double* data, double size, int* sort, int sort_num);
static int GetZeroCount(float*data,int Dim);
static int GetZeroCountContinueWeight(float*data,int Dim);
static void DrawLine2BlackImg(
	vector<LINEDATA> linedata,
	int width,
	int height,
	string filemane);

static void DrawLine2Img(
	LINEDATA* linedata, 
	int linelen, 
	IplImage* img,
	string filename);

static double GetMiddleValue(double* data, int size);

static void GetContour2Fill(
	const IplImage* contour,
	IplImage* fill_img,
	const CvScalar filldata);

static void GetContour2Fill(
	const IplImage* contour,
	int OriginX,
	int OriginY,
	IplImage* fill_img,
	const CvScalar filldata);

static double GetContour2Draw(
	const   IplImage* contour, 
    IplImage* fill_img,
	const CvScalar filldata,
	int thinkness=2,
	float*		C=NULL,
	float*		S=NULL);
/*----------------------------------------*/
static void CalculateAllSpPropertyRange( 
	INT32*					labels,
	const int&				width,
	const int&				height,
	SP_PROPERTY *p_SpProperty,
	int   SpNumbers);

static void CalculateAllSpPropertyPostitonByHLine(	
	int*					labels,
	const int&				width,
	const int&				height,
	SP_PROPERTY *p_SpProperty,
	int   SpNumbers,
	float horizontal_line_pos);

static  Energy_Harr CalculateImageHarrEnergy(
	UINT32* imgBufferOrigin,
	UINT32* imgBufferBorder,
	int Width,
	int Height);

static void CalculateSpBlockEnergy2(
	int sp,
	SP_PROPERTY *p_SpProperty,
	UINT32 * cui_ImgData,
	int *cui_ImgLables,
	int cui_Width,
	int cui_Height);

static void CalculateAllSpBlockEnergy(
	int num_sp,
	SP_PROPERTY *p_SpProperty,
	UINT32 * cui_ImgData,
	int *cui_ImgLables,
	int cui_Width,
	int cui_Height);
/*----------------------------------------*/
static float CalculateSpSimilar(int sp1, int sp2,SP_PROPERTY *p_SpProperty);

static void CuiSetNighbour_W_Vein_matrix(
	double *Matrix_W_Vein,
	const  int NumLabels,
	SP_PROPERTY *p_SpProperty,
	ImageMemData *pMD);

static void AdjustNighbour_W(double *Matrix_W_Color,
	double *Matrix_W_Vein,
	double *Matrix_W_Multi,
	int spcount,
	float Vein_threshold_SkyV,
	float Vein_threshold_GND,
	float Color_threshold_SkyV,
	float Color_threshold_GND,
	SP_PROPERTY *p_SpProperty);
/*----------------------------------------*/
static void InitPNGData(UINT32 *imgData,
	int width, 
	int height, 
	UINT32 value);
static UINT32* InitColorTable(UINT32* tab=nullptr, int size=0);
static INT32* InitSP2SVGcategory(INT32* category=nullptr, int size=0);
public:
static	float LabDistanceAB(LabColor ci,LabColor cj);
static	float LabDistanceA(LabColor ci,LabColor cj);
static	float LabDistanceB(LabColor ci,LabColor cj);
static	float LabDistanceL(LabColor ci,LabColor cj);
static  float LabDistanceABangle(LabColor ci,LabColor cj);

static void ConvertImg2Eighth4Ch(IplImage **src);
static void ConvertImg3ChTo4Ch(IplImage **src);

static void GetSVPoint(vector<CvPoint>& Points,ImageMemData* pMD);
static void GetVGPoint(vector<CvPoint>& Points,ImageMemData* pMD);

static bool IsLabClorGroundNear(LabColor ci,LabColor cj,float DThreshold,float AThreshold);
static bool IsLabClorGround_Insurance(LabColor ci,LabColor cj);

static void SaveMatrix_W(string path,string filename,int slic_current_num,double* Matrix_W);

static void SaveMatrix_Float(string path,string filename,int slic_current_num,float* Matrix_W);
static void SaveVector_Double(string path,string filename,int slic_current_num,double* Matrix_W);

static void FillHoleOnSVGLables(ImageMemData*,bool sky,bool vertical,bool ground,bool BorderSky);
static void DrawS_V_G_Lables_BorderLine(IplImage *img,UINT32 category);

static  void Get_Kseeds_Histogram(
	vector<double> kseedsl,
	vector<double> kseedsa,
	vector<double> kseedsb,
	vector<double> kseedsx,
	vector<double> kseedsy,
	KseedsHistData& seeddata,
	int* CuiImgLables,
	int ImgWidth,
	int ImgHeight,
	string FileReadFullPath,
	string FileWritePath);
static void Draw_Kseeds_AverageImg(
	vector<double> kseedsl,
	vector<double> kseedsa,
	vector<double> kseedsb,
	int* CuiImgLables,
	int ImgWidth,
	int ImgHeight,
	string FileReadFullPath,
	string FileWritePath);
static  void Get_Kseeds_Histogram(
	vector<double> kseedsl,
	vector<double> kseedsa,
	vector<double> kseedsb,
	vector<double> kseedsx,
	vector<double> kseedsy,
	KseedsHistColorGray& seeddata,
	int* CuiImgLables,
	int ImgWidth,
	int ImgHeight,
	string FileReadFullPath,
	string FileWritePath);
static  void Draw_Kseeds_Histogram(
	vector<double> kseedsl,
	vector<double> kseedsa,
	vector<double> kseedsb,
	vector<double> kseedsx,
	vector<double> kseedsy,
	KseedsHistData& seeddata,
	int* CuiImgLables,
	int ImgWidth,
	int ImgHeight,
	string FileReadFullPath,
	string FileWritePath);
static void Cui_Combination_ImgLabsWithTextureInColor(
	unsigned int*			ubuff,
	int*					labels,
	INT32*			   category,
	const double*          cui_Matrix_W,		
	int&				numlabels,
	const int&				width,
	const int&				height,
	double SuperpixelThread,
	ImageMemData* pMD);
static void Cui_Combination_ImgLabs2(
	unsigned int*			ubuff,
	int*					labels,
	INT32*			   category,
	const double*          cui_Matrix_W,		
	int&				numlabels,
	const int&				width,
	const int&				height,
	double SuperpixelThread,
	ImageData* pMD);
static void Cui_Combination_ImgLabsNew(
	vector<vector<int> > simlarSp,
	ImageData* pMD);
static void  DetermineColorRank(ImageMemData* pMD,int RankNum);
static void  ShowImgLabels(int* ImgLabels,int Width,int Height);

static UINT THreadSuperPixel_CUDA_CollectionMethods(LPVOID lpParam,vector<string> picvec,string saveLocation,int m_spcount);
static void THreadSuperPixel_DoOneImage(string picvec,string saveLocation,int m_spcount);
static unsigned THreadSuperPixel_DoOneImage_win(LPVOID lpParam);
static int	get_CPU_core_num(); 

static void _splitpath(const char *path, char *drive, char *dir, char *fname, char *ext);
static void _split_whole_name(const char *whole_name, char *fname, char *ext);

};

