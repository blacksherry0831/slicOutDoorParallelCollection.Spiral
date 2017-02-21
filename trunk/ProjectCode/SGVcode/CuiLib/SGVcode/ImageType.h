#pragma once
#ifndef ImageTypeCui
#define ImageTypeCui
//#include <memory>
//#include "cv.h"
#ifndef ASSERT
#include <assert.h>
#define ASSERT(x) assert(x)
#endif

/*------------------------------------------------------------------------------------------------------------*/
/**天空分类，能量+位置+色彩。
*色彩距离D（lab）能量距离（HL LH HH）
*62751 =	250	   25
*48998 =	220
*12%
*/
/*------------------------------------------------------------------------------------------------------------*/
#ifndef TRUE
#define TRUE 1
#endif
#ifndef  FALSE
#define FALSE 0
#endif
/*----------------------------------------------------------------------*/
#define MAX_SP_NUM    (3000)
#define  MAXLINEDATA (2000) 
/*----------------------------------------------------------------------*/
#define Unclassify (-2)
#define  Remove (-1)
#if 1
#define  Unknow (0)   
#endif
#define  Ground11  (-11)
#define  Ground12  (-12)
#define  Ground13  (-13)
#define  Ground (1)
#define  GroundCv   cvScalar(Ground,0,0,0)
#define  GroundPending  (-14)
#if 0
#define  NotSky   (1.5)
#endif
#define  Vertical (2)
#define  VerticalCv  cvScalar(Vertical,0,0,0)
#define  VerticalPending (-21)
#if 0
#define  NotGround (2.5)
#endif 
#define  Sky  (3)
#define  SkyCv  cvScalar(Sky,0,0,0)
#define  SkyPending (-31)
#ifdef Vertical 
#if Vertical 
#define Vertical_Tree      (4)
#define Vertical_Building  (5)
#endif
#endif 
/*------------------------------------------------------------------------------------------------------------*/
/**
*建筑类型
*/
/*------------------------------------------------------------------------------------------------------------*/
#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif
/************************************************************************/
#ifndef BuildingType_Enum
#define BuildingType_Enum
enum BuildingType{
	BT_bigbuildingVP=0,/**<大建筑需要找灭点分割直线*/
	BT_bigbuindingFront=1,/**<（已弃用）*/
	BT_trees=2,/**<数目环境（已弃用）*/
	BT_unknow=3/**<未知*/
};
#endif
/*----------------------------------------------------------------------*/
/**
*
/*----------------------------------------------------------------------*/
#ifndef INT32
typedef signed int INT32;
#endif
#ifndef  UINT32
typedef unsigned int UINT32;
#endif
#ifndef  UINT
typedef unsigned int UINT;
#endif
/*----------------------------------------------------------------------*/
/**
*超像素的小波能量数据及图块分类
*/
/*----------------------------------------------------------------------*/
typedef struct EnergyHarr{
	double  Energy_LH;/**<一次小波LH能量*/
	int     Size_LH;
	double  Energy_LL;/**<一次小波LL能量*/
	int     Size_LL;
	double  Energy_HH;/**<一次小波HH能量*/
	int     Size_HH;
	double  Energy_HL;/**<一次小波HL能量*/
	int     Size_HL;

	double  EnergyL_HH;/**<二次小波HH能量*/
	double  EnergyL_HL;/**<二次小波HL能量*/
	double  EnergyL_LL;/**<二次小波LL能量*/
	double  EnergyL_LH;/**<二次小波LH能量*/
	double  EnergyHighsigma;/**<小波LH ，HH，HL的综合能量*/
	//INT32  Sky_Candidate;/**<小波能量特性是否是天空候选*/
	//INT32  Category_Sky;/**<本超像素是否是天空*/
	UINT32  harrDepth;/**<当前Harr输出能量的小波层数，HarryDepth<=2*/
	int   EnergyGrade;/**<能量等级*/
}Energy_Harr;
/*----------------------------------------------------------------------*/
/**
*代表超像素图块的 Lab颜色\n
*以点代面
*/
/*----------------------------------------------------------------------*/
typedef  struct labColor{
	double  L_Color;/**<L颜色通道*/
	double  A_Color;/**<A颜色通道*/
	double  B_color;/**<B 颜色通道*/
	double  PixelNum;/**<如果是一个图块（超像素块），表示图块总像素个数*/
}LabColor;
/*-----------------------------------------------------------------------------*/
/**Lab聚类属性*/
/*-----------------------------------------------------------------------------*/
#ifndef labColorCategory_Struct
#define labColorCategory_Struct
typedef struct labColorCategory{
	LabColor  lab_color;/**<超像素颜色（为兼容老版本）*/
	double L;/**<超像素L的平均分量*/
	double A;/**<超像素A的平均分量*/
	double B;/**<超像素B的平均分量*/
	double Category;/**<超像素的天空,立面,地面,分类属性 */
	double L_Dst2Sky;/**<与最似天空的颜色距离*/
}LabColorCategory;
#endif
/*----------------------------------------------------------------------*/
/**
*超像素的范围，面积，和它的分类
*/
/*----------------------------------------------------------------------*/
/**<超像素块一部分在水平线之下*/
#define SpPos_UnderHorizontalLine (0x40)
/**<超像素块一部分在水平线之上*/
#define SpPos_UpHorizontalLine    (0x10)
/**<超像素穿过水平线*/
#define SpPos_CrossHorizontalLine (SpPos_UnderHorizontalLine|SpPos_UpHorizontalLine)
/**<超像素块靠图像左边界*/
#define SpPosBorder_LEFT  (0x01)
/**<超像素块靠图像右边界*/
#define SpPosBorder_RIGHT  (0x02)
/**<超像素块靠图像上边界*/
#define SpPosBorder_TOP  (0x04)
/**<超像素块靠图像下边界*/
#define SpPosBorder_BOTTOM  (0x08)
/**<超像素属性*/
typedef struct spGuid{
	UINT GUIDX;
	UINT GUIDY;	 
}SP_GUID;

typedef struct spProperty{
#if _DEBUG||TRUE
	UINT SPIndex;/**<超像素标号*/
	SP_GUID SPGuid;
#endif	
	unsigned char IS_UnderHorizontalLine;/**<图块与水平线的位置关系*/
	UINT32   PosBorder;/**<相对于原始图像的位置*/;
	int IsInit_SquareMeter;/**<是否初始化,同时保存超像素的面积*/
	/*********/
	int min_x;/**<包围超像素的矩阵在X轴上的最小值*/
	int min_y;/**<包围超像素的矩阵在Y轴上的最小值*/
	int max_x;/**<包围超像素的矩阵在X轴上的最大值*/
	int max_y;/**<包围超像素的矩阵在Y轴上的最大值*/
	int width;
	int height;
	/*********/	
	int  SPCategory;/**<超像素分类*/
	INT32  Category_Force;/**<超像强制类别*/
	/*********/  
	Energy_Harr  harr_energy;/**<超像素小波能量*/
	LabColor    lab_color;
}SP_PROPERTY;
/*----------------------------------------------------------------------*/
/**
*直线数据，保存hough变换检测后的直线
*/
/*----------------------------------------------------------------------*/
typedef struct lineData{
	CvPoint pointOne;/**<原始线段的一端点*/
	CvPoint pointTwo;/**<原始线段的一端点*/
	CvPoint	AdjPoint1;/**<线段与图像边界的交点，绘图用*/
	CvPoint	AdjPoint2;/**<线段与图像边界的交点，绘图用*/
	double  LineLength;/**<线段的原始长度*/
	double   p;/**<*/
	double   a;/**<直线方程y=a*x+b;中a的修正值*/
	double   b;/**<直线方程y=a*x+b;中b的修正值*/
	double   a_org;/**<直线方程y=a*x+b;中b的原始值*/
	double   b_org;/**<直线方程y=a*x+b;中b的原始值*/
	double  alpha_radian;/**<直线与X轴的夹角，弧度表示*/
	double  alpha_angle;/**<直线与X轴的夹角，角度表示*/
	int     Category;/**<直线的位置分类，天空，立面，地面之一*/
	int     Used2FindVP;/**<直线用于寻找角点*/
	bool    Used2CutImg;/**<直线用于分割地面*/
}LINEDATA;
/*------------------------------------------------------------------------------------------------------------*/
/**
*对超像素图块内属于天空、立面或地面的超像素图块进行计数\n
*针对因使用Hough变换而被切割的超像素块
*/
/*------------------------------------------------------------------------------------------------------------*/
class SVGcount{
public:
	float SkyCount;/**<归属天空的像素个数*/
	float VerticalCount;/**<归属立面的像素个数*/
	float GroundCount;/**<归属地面的像素个数*/
public:
/**
*初始化变量
*/
	SVGcount(){
		SkyCount=0;
		VerticalCount=0;
		GroundCount=0;
	}
public:
/**
*根据超像素个数中最大的那个，计算
*  -图块属性
*    -#Sky      天空
*    -#Vertical 立面
*    -#Ground   地面
*@return 超像素图块属性
*
*/
	int GetSVGproperty(){
		if (SkyCount>=VerticalCount&&SkyCount>=GroundCount)
		{
			return Sky;
		}
		if (VerticalCount>=SkyCount&&VerticalCount>=GroundCount)
		{
			return Vertical;
		}
		if (GroundCount>=SkyCount&&GroundCount>=VerticalCount)
		{
			return Ground;
		}
		ASSERT(FALSE);
		return -1;
	}
};
/*----------------------------------------------------------------------*/
/**
*颜色定义 RGB 三通道
*/
/*----------------------------------------------------------------------*/
#define WhiteColorRGB  (0x00ffffff)
#define BlackColorRGB  (0x00000000)
/*----------------------------------------------------------------------*/
/**
*颜色定义RGBA 四通道
*/
/*----------------------------------------------------------------------*/
#define BlackColorPNG (0xff000000)
#define WhiteColorPNG (0xffffffff)
#define SkyColorPNG      (0xD04B4587)
#define VerticalColorPNG (0xE0ff0000)
#define GroundColorPNG   (0xF04D9D7B)
#define UnknowColorPNG      (0xff000000)
#define VerticalTreeColorPNG (0xff00ff00)
#define VerticalBuildingColorPNG (0xffff0000)
/*----------------------------------------------------------------------*/
/**
*颜色定义--OpenCv cvScalar
*/
/*----------------------------------------------------------------------*/
#define BlackColorPNGCv     cvScalar(0x00,0x00,0x00,0xff)
#define WhiteColorPNGCv	    cvScalar(0xff,0xff,0xff,0xff)
#define SkyColorPNGCv       cvScalar(0x87,0x45,0x4B,0xD0)
#define VerticalColorPNGCv  cvScalar(0x00,0x00,0xff,0xE0)
#define GroundColorPNGCv    cvScalar(0x7B,0x9D,0x4D,0xF0)
/*----------------------------------------------------------------------*/

#ifndef __KseedsHistData__
#define __KseedsHistData__
 // 声明、定义语句
class  KseedsHistData{
public:
	KseedsHistData(int size=180)
	{
		this->resize(size);
	}
	void resize(int size)
	{
		this->histDimSPLAB=size;
		this->LAvg.resize(size);//0-100
		this->hist_AB_Count.resize(size);//-127--128
		this->hist_AB_CountRemove.resize(size);
		this->hist_AB_CountOrg.resize(size);//count
		this->AB_lengthAvg.resize(size);//128*1.414
		this->hist_AB_detail.resize(size);
	}
	void removeUsedHist(void)
	{
		for (int i=0;i<histDimSPLAB;i++){
			if (hist_AB_CountRemove[i]<0){
				hist_AB_Count[i]=0;
				hist_AB_CountRemove[i]-=1;
				this->hist_AB_detail[i].clear();
			}
		}
	}
	void Clear()
	{
		for (int i=0;i<this->histDimSPLAB;i++){
			this->LAvg[i]=0;//0-100
			this->hist_AB_Count[i]=0;//-127--128
			this->hist_AB_CountRemove[i]=0;
			this->hist_AB_CountOrg[i]=0;//count
			this->AB_lengthAvg[i];//128*1.414
			this->hist_AB_detail[i].clear();
		}
	}
	void ClearUsedHistArray(void)
	{
		for (int i=0;i<histDimSPLAB;i++){		
				hist_AB_CountRemove[i]=0;
		}
	}
	bool IsRemoveALL(void)
	{
		bool IsRemove=true;
		
		for (int i=0;i<histDimSPLAB;i++){
			if (hist_AB_Count[i]>0){
				IsRemove=false;
			}
		}
		return IsRemove;
	}
	~KseedsHistData()
	{

	}
public:
	int histDimSPLAB;
	vector<double> LAvg;//0-100
	vector<double> hist_AB_Count;//-127--128
	vector<double> hist_AB_CountRemove;
	vector<double> hist_AB_CountOrg;//count
	vector<double> AB_lengthAvg;//128*1.414
	vector< vector<int> > hist_AB_detail;
};

class KseedsHistColorGray{
public:
	KseedsHistData ColorHist;
	KseedsHistData GrayHist;
};


#else

#endif

/*----------------------------------------------------------------------*/
typedef struct  histRangeData{
	/*前半段*/
	double *start;
	double len;
	float angle_up;
	float angle_down;
	/*后半段*/
	int  maxHistIdx;
	int  StepLR;
	double* maxHistCenter;
	double* maxHistLeft;
	double* maxHistRight;
}HistRangeData;
/*----------------------------------------------------------------------*/
class ThreadDoOneImageData{
public:
	vector<string> picvec;
	string saveLocation;
	int m_spcount;
	int start;
	int step;
	ThreadDoOneImageData(vector<string> picvec,
							string saveLocation,
							int m_spcount,
							int start,
							int step){
		this->picvec=picvec;
		this->saveLocation=saveLocation;
		this->m_spcount=m_spcount;
		this->start=start;
		this->step=step;
	}
};
/*----------------------------------------------------------------------*/
#endif