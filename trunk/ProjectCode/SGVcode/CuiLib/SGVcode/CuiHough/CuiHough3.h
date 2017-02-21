#pragma once
#include <platform_cui.h>
//typedef unsigned int UINT32;
//typedef signed int INT32;
//using namespace std;
//#include <cv.h>
//#include <highgui.h>
//#include <math.h>
//#if _MSC_VER
//#include <STDIO.H>
//#endif
//#include <stdio.h>
//#include <iostream>
#include "SGVcode/ImageType.h"
#include "SGVcode/ImageMemData.h"
#include "SGVcode/SceneDetermine.h"
/*------------------------------------------------------------------------------------------------------------*/
/**
*灭点数据结构
*/
/*------------------------------------------------------------------------------------------------------------*/
typedef struct{
	CvPoint	 point;/**<灭点中心*/
	float   sigema;/**<灭点集一个sigema范围*/
}VpPoint;
/*------------------------------------------------------------------------------------------------------------*/
/**
*查找灭点所需的点集，数据结构
*/
/*------------------------------------------------------------------------------------------------------------*/
class VanishingPointData{
public:
	double Vp_angle_up;/**<灭点角度上限*/
	double Vp_angle_down;/**<灭点角度下限*/
	int   Effective_cluster;/**<直线集的有效组*/
	int  hist_pos;/**<直方图位置*/
	double Vp_height;/**<直方图高度*/
	CvPoint	 Vpoint;/**<灭点中心*/
	vector<CvPoint> PreparePoint;/**<灭点预备点*/
	vector<LINEDATA> PrepareLine;/**<预备的直线*/

};
/*------------------------------------------------------------------------------------------------------------*/
/**
*灭点位置
*/
/*------------------------------------------------------------------------------------------------------------*/
enum VanishPosition{
	LEFT,/**<灭点在图像中线的左*/
	LEFT_OUT,/**<灭点在图像外的左*/
	LEFT_IN,/**<灭点在在图像内左*/
	MIDDLE,/**<灭点在图像中线附近*/
	RIGHT_IN,/**<灭点在图像内的右*/
	RIGHT_OUT,/**<灭点在图像外的右*/
	RIGHT/**<灭点在图像右*/
};
/*------------------------------------------------------------------------------------------------------------*/
/**
*需要寻找灭点分割图像时，分割用的直线数据
*包括单灭点，双灭点多种情况
*/
/*------------------------------------------------------------------------------------------------------------*/
typedef struct{
	/*部分变量的命名存在重复*/
	CvPoint   VanishingPoint;/**<单灭点时，为其中一个灭点，双灭点时，为影响力较大的灭点*/
	int       VPointPosition;/**<灭点的位置定义*/
	LINEDATA  GndSegLine[3];/**<代表三条（不一定是三条）分割线*/
	double    Line2VPdst[3];/**<直线到中间线的距离*/
	int    VPointPositionIndex[3];/**<灭点位置的索引*/
	//0 L
	//1	H
	//2	V
	/**
	*新数据结构
	*数组中的灭点与位置关系是相对应的
	*@{*/
	int  img_width;/**<图像的高度用于计算灭点位置*/
	CvPoint  VanishPoint[2];/**<两个灭点的位置*/
	VanishPosition  VPposition[2];/**<灭点位置信息*/
	int      VPnumber;/**<图像的高度用于计算灭点位置*/
	VpPoint  vpPoint[2];/**<两个灭点的位置，包含sigema*/
	/**@}*/
}GroundSegment; 

/*------------------------------------------------------------------------------------------------------------*/
/**
*本类实现对图像的hough 变换\n
*如果有需要就寻找灭点分割图像\n
*通过对图像中被判别为立面物体的图块进行Hogh变换，\n
*并通过基于直线方向角度的统计直方图，\n
*寻找曼哈顿方向信息的强度及特征形态，\n
*进而判断出图中是否存在大型近距离建筑物，\n
*如果不存在则结束对地面的修正，\n
*如果存在则进入下一步；\n
*/
/*------------------------------------------------------------------------------------------------------------*/
class CuiHough3
{
private:
	ImageMemData* pMD;/**<图像数据及计算的中间变量*/
	CvMemStorage* storage;/**<opencv计算用的中间内存*/ 
	CvSeq* lines;/**<opencv的序列，占用CvMemStorage，内存空间*/
private:
	int findLongestLine(IplImage* src);
	int CuiUseCany2binaryzation(IplImage* src_unknow);	
	void ClearImgMemory(void);	
	void InitParam(void);
public:
	CuiHough3(ImageMemData* MemData_t);
	CuiHough3(void);
	~CuiHough3(void);

public:
	BuildingType   ImageEenvironment;/**<建筑类型*/
	LINEDATA *cui_line_srcimg;/**<在原图上Hough变换得到的直线*/
	int    cui_line_src_len;/**<原图上直线的数量*/
	LINEDATA *cui_line_contour;/**<在轮廓上进行Hough变换得到的直线*/
	int   cui_line_contour_len;/**<轮廓上直线的数量*/
	LINEDATA *cui_line_VGcontour;/**<在立面和地面边界上Hough变换得到的直线*/
	int  cui_line_VGcontour_len;/**<立面和地面边界上直线的数量*/
	HistData  hist_scrimg,hist_contours,hist_contours_VG;/**<分别为原图、轮廓、地面和立面边界的直方图数据*/

	GroundSegment GndSegment;/**<分割地面的直线结构*/
	LINEDATA  cui_line_cpy[MAXLINEDATA];/**<调试用直线数组*/
	//vertical
	VanishingPointData VP_V_L_UP[2];/**<直方图左侧单调增直线数组*/
	VanishingPointData VP_V_R_DOWN[2];/**<直方图右侧单调减直线数组*/
	//VanishingPointData *VP_ptr[2];
	//ground*vertical
	VanishingPointData VP_GV_L_UP[2];/**<地面与立面上直方图左侧单调增直线数组*/
	VanishingPointData VP_GV_H_H[2];/**<地面与立面上直方图水平线附近直线数组*/
	VanishingPointData VP_GV_R_DOWN[2];/**<地面与立面上直方图右侧单调减直线数组*/
	
	vector<CvPoint>	GV_Segment_point;/**<用于寻找灭点的点集*/

	

	IplImage* src_img;/**<原始图像数据，4通道*/
	IplImage* gray_img;/**<原图对应的灰度图像，单通道*/
	IplImage* gray_Color_src_img;/**<灰度图像对应的彩色图像（调试用）*/
	IplImage* gray_Color_contour_img;/**<灰度图像对应的轮廓图（调试用）*/
	IplImage* gray_binary_img;/**<灰度经二值化后的图像*/
	/*****************************/
	UINT32 *cui_ImgData;/**<图像数据*/
	UINT32 *cui_ImgData_Contour;/**<图像轮廓数据*/
	UINT32 *cui_ImgData_Contour_GV;/**<图像的地面，立面的轮廓数据*/
	int cui_Width;/**<图像宽度*/
	int cui_Height;/**<图像高度*/
	int*  cui_ImgLables;/**<图像超像素对应的Lables*/
	int*  DistortionIMGLables;/**<（弃用）*/
	int*  cui_ImgLables_SVG;/**<天空、立面、地面的分类矩阵*/
	int cui_NumLabels;/**<超像素个数*/
	INT32 *cui_Matrix_Category_Lable;/**<超像素块的分类矩阵*/
public:
	void GetImageData(string img_filename);	
	void GetImageData(IplImage* imgscr);
	void GetImageData(UINT32* ImgData,int Width,int Height,int*  Lables,int *LablesSVG,int NumLabels,INT32* Category);
	void SetDistortionIMGLables(int* ImgLables);
	//////////////////////////////////////////////////////
	void GetAllhoughLineData(IplImage **src_img_t,IplImage **gray_Color_img_t,LINEDATA **plinedata,int *pline_len,int method);
	void GetAllHoughLineByContour(void);
	void GetAllHoughLineByContourVG(void);
	void GetAllHoughLineBySourceImg(void);

	void cui_ShowImageLine(string filename,int directline,IplImage *gray_Color_img,LINEDATA *cui_line_data,int line_len);
static	CvScalar  GetGradientColor(double angle_color);
	int Getthickness(int category_t);
	void DrawHistogramOnImg(string winname,HistData* hist_data);
	
	void ClassifyLine(LINEDATA *line_data,int line_len,IplImage *gray_Color_img,string add_t);
	int CheckLineArea(CvPoint point1,CvPoint point2);
	

static	void GetHistogramSVG2(string filename,LINEDATA* linedata,int linelen,HistData *hist_data);
		
	void CalculateEffectiveLine(LINEDATA* linedata,int linelen,HistData *hist_data,IplImage* gray_color_img,string file_add);
	void ImgPreprocess(IplImage **src_img_t,int method);
	void RemoveLineByHist_Multiply(float threshold,HistData *hist_data);
	static	void RemoveLineVertical90(LINEDATA* line_data,int line_len);
	BuildingType DecideEnvironment(HistData hist_data);
	void AdjustLine(LINEDATA *cui_line_data,int cui_line_len);
	void Find4VanishingPoint(HistData *hist_data);
	void Find2Direction(void);
	void MatchingVP(void);
	void DrawVPonImg(int GNDSegline=0);
	int SelectVPLineLeftorRight(void);
	void FindVPbyLeftorRightCross(void);
	VpPoint FindVPby2ClusterLine(VanishingPointData * VP_dat0,VanishingPointData* VP_dat1);
	
	
static	double DistancePoint2Line(CvPoint point, LINEDATA line);
	void GetHist3effectiveDir(float *hist,VanishingPointData* VP_Lt,VanishingPointData* VP_Rt);
	void GetHist4EffectiveLinecluster(float *hist,VanishingPointData* VP_V_Lt,VanishingPointData* VP_V_Rt);
	
	void GetLine4cluster2determineVP(float *hist,VanishingPointData* VP_V_L_UPt,VanishingPointData* VP_V_R_DOWNt);
	void GetLine2cluster2determineVP(float *hist,VanishingPointData* VP_V_L_t,int UporDown);
	
	void EnsureVPLineCluster(void);
	void HoughLineClassifyGV(void);
	void HoughLineClassifyGV2(void);
	void NatureClassifyGV(void);
	void ClassifyGV(void);
	unsigned int GetSegmentPoint(int X_coordinate);
	unsigned int GetSegmentPointCompare2VGcontrous(int X_coordinate);
	void SetFileReadSavePath(string WritePath, string ReadPath);
	void GetVPoint(void);

	void GetPrepareLine(HistData* hist_data);
	void Get4ClusterPrepareLine(HistData* hist_data);
   /*****************************************************************************************************************/
	void GetGroundSegmentLine(HistData *hist_data);
	void Get1GroundSegmentLine(HistData *hist_data,LINEDATA *line_data,int line_len);
	void Get2GroundSegmentLine(HistData *hist_data,LINEDATA *line_data,int line_len);
	void GetGroundSegmentLineUp(HistData *hist_data,LINEDATA *line_data,int line_len);
	void GetGroundSegmentLineDown(HistData *hist_data,LINEDATA *line_data,int line_len);
	LINEDATA GetGroundSegmentLineUporDown(HistData *hist_data,LINEDATA *line_data,int line_len ,int UporDown,VpPoint vppoint);
	void GetGroundSegmentLineHorizontal(HistData *hist_data,LINEDATA *line_data,int line_len);
    LINEDATA  GetGroundSegmentLineByDirect(LINEDATA *line_data,int line_len,VanishingPointData *VP_lINE,VpPoint vppoint);
   	LINEDATA  SelectCutLinebyIntercept(vector<LINEDATA> PrepareLine,vector<CvPoint> preparepoint);
	LINEDATA SelectCutLinebyIntercept2(vector<LINEDATA> PrepareLine,vector<CvPoint> preparepoint);
	void AdjustCutLineLength(vector<LINEDATA> *PrepareLine,int radius);
	bool Adjust2LinePosition(vector<LINEDATA> *PrepareLine,int li,int lj); 
	void standardLineStructure(LINEDATA  *line);
	/*****************************************************************************************************************/
	
	/*********************************************************/
	VpPoint SelectVPbyMedianIteration(vector<CvPoint> point_t,int type);
	void DrawCloudVPointOnIMG(vector<CvPoint> CloudPointL,vector<CvPoint> CloudPointR,VpPoint CenterPoint,int type);
	CvPoint GetCloudMediaPoint(vector<CvPoint> point_t);
	CvPoint GetCloudAvgPoint(vector<CvPoint> point_t);
	/**********************************************************/

	void DetermineSceneCategory(HistData *hist_data,LINEDATA* linedata,int linelen);
static void   RemoveHistData0and90(float *hist_data,int hist_size);	
static	int CheckVPNumberbyHist(HistData* hist_data,double scale=0.75);
void   SetPoint2GndData(GroundSegment * gnddata,VpPoint p0,VpPoint p1,int point_num,int Img_width);
void   GetPeakValueGroup(float* hist_data,int hist_size,int* group,int group_num);
void SetGVsegmentPoint(void);
double Line2VGContourDistance(LINEDATA line);
double Line2VPContourDistance(LINEDATA line);
/******************************/
VpPoint GetLeftVPoint(void);
VpPoint GetRightVPoint(void);
/******************************/
void CalculateEffetiveVPoint(int line_in_use);
void GetClassifyMethod(void);
LINEDATA Get1HorizontalLine(HistData histdata);
void  Use1HorizontalLine2CutGV(LINEDATA line);
static void UseEyeLevel2AdjustSVG(int* cui_ImgLables_SVG,int Width,int Height,LINEDATA horLine,VpPoint *vpPoint,int EyeLevel);
};

