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
*������ݽṹ
*/
/*------------------------------------------------------------------------------------------------------------*/
typedef struct{
	CvPoint	 point;/**<�������*/
	float   sigema;/**<��㼯һ��sigema��Χ*/
}VpPoint;
/*------------------------------------------------------------------------------------------------------------*/
/**
*�����������ĵ㼯�����ݽṹ
*/
/*------------------------------------------------------------------------------------------------------------*/
class VanishingPointData{
public:
	double Vp_angle_up;/**<���Ƕ�����*/
	double Vp_angle_down;/**<���Ƕ�����*/
	int   Effective_cluster;/**<ֱ�߼�����Ч��*/
	int  hist_pos;/**<ֱ��ͼλ��*/
	double Vp_height;/**<ֱ��ͼ�߶�*/
	CvPoint	 Vpoint;/**<�������*/
	vector<CvPoint> PreparePoint;/**<���Ԥ����*/
	vector<LINEDATA> PrepareLine;/**<Ԥ����ֱ��*/

};
/*------------------------------------------------------------------------------------------------------------*/
/**
*���λ��
*/
/*------------------------------------------------------------------------------------------------------------*/
enum VanishPosition{
	LEFT,/**<�����ͼ�����ߵ���*/
	LEFT_OUT,/**<�����ͼ�������*/
	LEFT_IN,/**<�������ͼ������*/
	MIDDLE,/**<�����ͼ�����߸���*/
	RIGHT_IN,/**<�����ͼ���ڵ���*/
	RIGHT_OUT,/**<�����ͼ�������*/
	RIGHT/**<�����ͼ����*/
};
/*------------------------------------------------------------------------------------------------------------*/
/**
*��ҪѰ�����ָ�ͼ��ʱ���ָ��õ�ֱ������
*��������㣬˫���������
*/
/*------------------------------------------------------------------------------------------------------------*/
typedef struct{
	/*���ֱ��������������ظ�*/
	CvPoint   VanishingPoint;/**<�����ʱ��Ϊ����һ����㣬˫���ʱ��ΪӰ�����ϴ�����*/
	int       VPointPosition;/**<����λ�ö���*/
	LINEDATA  GndSegLine[3];/**<������������һ�����������ָ���*/
	double    Line2VPdst[3];/**<ֱ�ߵ��м��ߵľ���*/
	int    VPointPositionIndex[3];/**<���λ�õ�����*/
	//0 L
	//1	H
	//2	V
	/**
	*�����ݽṹ
	*�����е������λ�ù�ϵ�����Ӧ��
	*@{*/
	int  img_width;/**<ͼ��ĸ߶����ڼ������λ��*/
	CvPoint  VanishPoint[2];/**<��������λ��*/
	VanishPosition  VPposition[2];/**<���λ����Ϣ*/
	int      VPnumber;/**<ͼ��ĸ߶����ڼ������λ��*/
	VpPoint  vpPoint[2];/**<��������λ�ã�����sigema*/
	/**@}*/
}GroundSegment; 

/*------------------------------------------------------------------------------------------------------------*/
/**
*����ʵ�ֶ�ͼ���hough �任\n
*�������Ҫ��Ѱ�����ָ�ͼ��\n
*ͨ����ͼ���б��б�Ϊ���������ͼ�����Hogh�任��\n
*��ͨ������ֱ�߷���Ƕȵ�ͳ��ֱ��ͼ��\n
*Ѱ�������ٷ�����Ϣ��ǿ�ȼ�������̬��\n
*�����жϳ�ͼ���Ƿ���ڴ��ͽ����뽨���\n
*���������������Ե����������\n
*��������������һ����\n
*/
/*------------------------------------------------------------------------------------------------------------*/
class CuiHough3
{
private:
	ImageMemData* pMD;/**<ͼ�����ݼ�������м����*/
	CvMemStorage* storage;/**<opencv�����õ��м��ڴ�*/ 
	CvSeq* lines;/**<opencv�����У�ռ��CvMemStorage���ڴ�ռ�*/
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
	BuildingType   ImageEenvironment;/**<��������*/
	LINEDATA *cui_line_srcimg;/**<��ԭͼ��Hough�任�õ���ֱ��*/
	int    cui_line_src_len;/**<ԭͼ��ֱ�ߵ�����*/
	LINEDATA *cui_line_contour;/**<�������Ͻ���Hough�任�õ���ֱ��*/
	int   cui_line_contour_len;/**<������ֱ�ߵ�����*/
	LINEDATA *cui_line_VGcontour;/**<������͵���߽���Hough�任�õ���ֱ��*/
	int  cui_line_VGcontour_len;/**<����͵���߽���ֱ�ߵ�����*/
	HistData  hist_scrimg,hist_contours,hist_contours_VG;/**<�ֱ�Ϊԭͼ�����������������߽��ֱ��ͼ����*/

	GroundSegment GndSegment;/**<�ָ�����ֱ�߽ṹ*/
	LINEDATA  cui_line_cpy[MAXLINEDATA];/**<������ֱ������*/
	//vertical
	VanishingPointData VP_V_L_UP[2];/**<ֱ��ͼ��൥����ֱ������*/
	VanishingPointData VP_V_R_DOWN[2];/**<ֱ��ͼ�Ҳ൥����ֱ������*/
	//VanishingPointData *VP_ptr[2];
	//ground*vertical
	VanishingPointData VP_GV_L_UP[2];/**<������������ֱ��ͼ��൥����ֱ������*/
	VanishingPointData VP_GV_H_H[2];/**<������������ֱ��ͼˮƽ�߸���ֱ������*/
	VanishingPointData VP_GV_R_DOWN[2];/**<������������ֱ��ͼ�Ҳ൥����ֱ������*/
	
	vector<CvPoint>	GV_Segment_point;/**<����Ѱ�����ĵ㼯*/

	

	IplImage* src_img;/**<ԭʼͼ�����ݣ�4ͨ��*/
	IplImage* gray_img;/**<ԭͼ��Ӧ�ĻҶ�ͼ�񣬵�ͨ��*/
	IplImage* gray_Color_src_img;/**<�Ҷ�ͼ���Ӧ�Ĳ�ɫͼ�񣨵����ã�*/
	IplImage* gray_Color_contour_img;/**<�Ҷ�ͼ���Ӧ������ͼ�������ã�*/
	IplImage* gray_binary_img;/**<�ҶȾ���ֵ�����ͼ��*/
	/*****************************/
	UINT32 *cui_ImgData;/**<ͼ������*/
	UINT32 *cui_ImgData_Contour;/**<ͼ����������*/
	UINT32 *cui_ImgData_Contour_GV;/**<ͼ��ĵ��棬�������������*/
	int cui_Width;/**<ͼ����*/
	int cui_Height;/**<ͼ��߶�*/
	int*  cui_ImgLables;/**<ͼ�����ض�Ӧ��Lables*/
	int*  DistortionIMGLables;/**<�����ã�*/
	int*  cui_ImgLables_SVG;/**<��ա����桢����ķ������*/
	int cui_NumLabels;/**<�����ظ���*/
	INT32 *cui_Matrix_Category_Lable;/**<�����ؿ�ķ������*/
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

