#pragma once
/*------------------------------------------------------------------------------------------------------------*/
/**
*
*直线的直方图数据结构
*/
/*------------------------------------------------------------------------------------------------------------*/
#define  AngLeDivided (180/4)
typedef struct histdata{
	float hist_all[AngLeDivided];/**<所有直线的直方图*/
	float hist_sky[AngLeDivided];/**<天空直线的直方图*/
	float hist_ground[AngLeDivided];/**<地面直线的直方图*/
	float hist_vertical[AngLeDivided];/**<垂直立面的直方图*/
	float hist_Multiply[AngLeDivided];/**<立面与地面乘积的直方图（弃用）*/
	float hist_all_count[AngLeDivided];/**<直线计数直方图*/
}HistData;
#define  HistNUM   5
/*------------------------------------------------------------------------------------------------------------*/
/**
*
*直线的直方图数据结构
*/
/*------------------------------------------------------------------------------------------------------------*/
#define AngLeDivided180 (180/1)
typedef struct{
	float hist_all[AngLeDivided180];/**<所有直线的直方图*/
	float hist_all_sum;/**<直线长度总和*/
	float hist_sky[AngLeDivided180];/**<天空直线的直方图*/
	float hist_ground[AngLeDivided180];/**<地面直线的直方图*/
	float hist_vertical[AngLeDivided180];/**<垂直立面的直方图*/
	float hist_Multiply[AngLeDivided180];/**<立面与地面乘积的直方图（弃用）*/
	float hist_all_count[AngLeDivided180];/**<直线计数直方图*/
	float hist_all_count_sum;/**<直线数量总和*/
}HistData180;
#define  HistNUM_INDOOR   5
/*------------------------------------------------------------------------------------------------------------*/
/**
*
*直线的直方图数据结构
*/
/*------------------------------------------------------------------------------------------------------------*/
class ManhattanHistogram
{
public:
	ManhattanHistogram(unsigned int Dim);
	ManhattanHistogram(HistData histdata,unsigned int Dim);
	ManhattanHistogram(HistData180 histdata,unsigned int Dim);
	~ManhattanHistogram(void);
public:
	HistData histdata45;
	HistData180 histdata180;
public:
	unsigned int  HistDim;
	vector<float> hist_all;/**<所有直线的直方图*/
	vector<float> hist_sky;/**<天空直线的直方图*/
	vector<float> hist_ground;/**<地面直线的直方图*/
	vector<float> hist_vertical;/**<垂直立面的直方图*/
	vector<float> hist_Multiply;/**<立面与地面乘积的直方图（弃用）*/
	vector<float> hist_all_count;
private:
	void AllocaeMemory(unsigned int Dim);
public:
	void AdjustTo5Peak(void);
	bool IsContourRectangle(void);
	bool IsContourstrip(void);
};
/*------------------------------------------------------------------------------------------*/
/**
*
*
*/
/*-----------------------------------------------------------------------------------------*/


