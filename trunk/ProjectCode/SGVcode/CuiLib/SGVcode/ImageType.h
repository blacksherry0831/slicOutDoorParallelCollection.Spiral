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
/**��շ��࣬����+λ��+ɫ�ʡ�
*ɫ�ʾ���D��lab���������루HL LH HH��
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
*��������
*/
/*------------------------------------------------------------------------------------------------------------*/
#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif
/************************************************************************/
#ifndef BuildingType_Enum
#define BuildingType_Enum
enum BuildingType{
	BT_bigbuildingVP=0,/**<������Ҫ�����ָ�ֱ��*/
	BT_bigbuindingFront=1,/**<�������ã�*/
	BT_trees=2,/**<��Ŀ�����������ã�*/
	BT_unknow=3/**<δ֪*/
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
*�����ص�С���������ݼ�ͼ�����
*/
/*----------------------------------------------------------------------*/
typedef struct EnergyHarr{
	double  Energy_LH;/**<һ��С��LH����*/
	int     Size_LH;
	double  Energy_LL;/**<һ��С��LL����*/
	int     Size_LL;
	double  Energy_HH;/**<һ��С��HH����*/
	int     Size_HH;
	double  Energy_HL;/**<һ��С��HL����*/
	int     Size_HL;

	double  EnergyL_HH;/**<����С��HH����*/
	double  EnergyL_HL;/**<����С��HL����*/
	double  EnergyL_LL;/**<����С��LL����*/
	double  EnergyL_LH;/**<����С��LH����*/
	double  EnergyHighsigma;/**<С��LH ��HH��HL���ۺ�����*/
	//INT32  Sky_Candidate;/**<С�����������Ƿ�����պ�ѡ*/
	//INT32  Category_Sky;/**<���������Ƿ������*/
	UINT32  harrDepth;/**<��ǰHarr���������С��������HarryDepth<=2*/
	int   EnergyGrade;/**<�����ȼ�*/
}Energy_Harr;
/*----------------------------------------------------------------------*/
/**
*��������ͼ��� Lab��ɫ\n
*�Ե����
*/
/*----------------------------------------------------------------------*/
typedef  struct labColor{
	double  L_Color;/**<L��ɫͨ��*/
	double  A_Color;/**<A��ɫͨ��*/
	double  B_color;/**<B ��ɫͨ��*/
	double  PixelNum;/**<�����һ��ͼ�飨�����ؿ飩����ʾͼ�������ظ���*/
}LabColor;
/*-----------------------------------------------------------------------------*/
/**Lab��������*/
/*-----------------------------------------------------------------------------*/
#ifndef labColorCategory_Struct
#define labColorCategory_Struct
typedef struct labColorCategory{
	LabColor  lab_color;/**<��������ɫ��Ϊ�����ϰ汾��*/
	double L;/**<������L��ƽ������*/
	double A;/**<������A��ƽ������*/
	double B;/**<������B��ƽ������*/
	double Category;/**<�����ص����,����,����,�������� */
	double L_Dst2Sky;/**<��������յ���ɫ����*/
}LabColorCategory;
#endif
/*----------------------------------------------------------------------*/
/**
*�����صķ�Χ������������ķ���
*/
/*----------------------------------------------------------------------*/
/**<�����ؿ�һ������ˮƽ��֮��*/
#define SpPos_UnderHorizontalLine (0x40)
/**<�����ؿ�һ������ˮƽ��֮��*/
#define SpPos_UpHorizontalLine    (0x10)
/**<�����ش���ˮƽ��*/
#define SpPos_CrossHorizontalLine (SpPos_UnderHorizontalLine|SpPos_UpHorizontalLine)
/**<�����ؿ鿿ͼ����߽�*/
#define SpPosBorder_LEFT  (0x01)
/**<�����ؿ鿿ͼ���ұ߽�*/
#define SpPosBorder_RIGHT  (0x02)
/**<�����ؿ鿿ͼ���ϱ߽�*/
#define SpPosBorder_TOP  (0x04)
/**<�����ؿ鿿ͼ���±߽�*/
#define SpPosBorder_BOTTOM  (0x08)
/**<����������*/
typedef struct spGuid{
	UINT GUIDX;
	UINT GUIDY;	 
}SP_GUID;

typedef struct spProperty{
#if _DEBUG||TRUE
	UINT SPIndex;/**<�����ر��*/
	SP_GUID SPGuid;
#endif	
	unsigned char IS_UnderHorizontalLine;/**<ͼ����ˮƽ�ߵ�λ�ù�ϵ*/
	UINT32   PosBorder;/**<�����ԭʼͼ���λ��*/;
	int IsInit_SquareMeter;/**<�Ƿ��ʼ��,ͬʱ���泬���ص����*/
	/*********/
	int min_x;/**<��Χ�����صľ�����X���ϵ���Сֵ*/
	int min_y;/**<��Χ�����صľ�����Y���ϵ���Сֵ*/
	int max_x;/**<��Χ�����صľ�����X���ϵ����ֵ*/
	int max_y;/**<��Χ�����صľ�����Y���ϵ����ֵ*/
	int width;
	int height;
	/*********/	
	int  SPCategory;/**<�����ط���*/
	INT32  Category_Force;/**<����ǿ�����*/
	/*********/  
	Energy_Harr  harr_energy;/**<������С������*/
	LabColor    lab_color;
}SP_PROPERTY;
/*----------------------------------------------------------------------*/
/**
*ֱ�����ݣ�����hough�任�����ֱ��
*/
/*----------------------------------------------------------------------*/
typedef struct lineData{
	CvPoint pointOne;/**<ԭʼ�߶ε�һ�˵�*/
	CvPoint pointTwo;/**<ԭʼ�߶ε�һ�˵�*/
	CvPoint	AdjPoint1;/**<�߶���ͼ��߽�Ľ��㣬��ͼ��*/
	CvPoint	AdjPoint2;/**<�߶���ͼ��߽�Ľ��㣬��ͼ��*/
	double  LineLength;/**<�߶ε�ԭʼ����*/
	double   p;/**<*/
	double   a;/**<ֱ�߷���y=a*x+b;��a������ֵ*/
	double   b;/**<ֱ�߷���y=a*x+b;��b������ֵ*/
	double   a_org;/**<ֱ�߷���y=a*x+b;��b��ԭʼֵ*/
	double   b_org;/**<ֱ�߷���y=a*x+b;��b��ԭʼֵ*/
	double  alpha_radian;/**<ֱ����X��ļнǣ����ȱ�ʾ*/
	double  alpha_angle;/**<ֱ����X��ļнǣ��Ƕȱ�ʾ*/
	int     Category;/**<ֱ�ߵ�λ�÷��࣬��գ����棬����֮һ*/
	int     Used2FindVP;/**<ֱ������Ѱ�ҽǵ�*/
	bool    Used2CutImg;/**<ֱ�����ڷָ����*/
}LINEDATA;
/*------------------------------------------------------------------------------------------------------------*/
/**
*�Գ�����ͼ����������ա���������ĳ�����ͼ����м���\n
*�����ʹ��Hough�任�����и�ĳ����ؿ�
*/
/*------------------------------------------------------------------------------------------------------------*/
class SVGcount{
public:
	float SkyCount;/**<������յ����ظ���*/
	float VerticalCount;/**<������������ظ���*/
	float GroundCount;/**<������������ظ���*/
public:
/**
*��ʼ������
*/
	SVGcount(){
		SkyCount=0;
		VerticalCount=0;
		GroundCount=0;
	}
public:
/**
*���ݳ����ظ����������Ǹ�������
*  -ͼ������
*    -#Sky      ���
*    -#Vertical ����
*    -#Ground   ����
*@return ������ͼ������
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
*��ɫ���� RGB ��ͨ��
*/
/*----------------------------------------------------------------------*/
#define WhiteColorRGB  (0x00ffffff)
#define BlackColorRGB  (0x00000000)
/*----------------------------------------------------------------------*/
/**
*��ɫ����RGBA ��ͨ��
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
*��ɫ����--OpenCv cvScalar
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
 // �������������
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
	/*ǰ���*/
	double *start;
	double len;
	float angle_up;
	float angle_down;
	/*����*/
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