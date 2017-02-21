#pragma once
/*------------------------------------------------------------------------------------------------------------*/
/**
*
*ֱ�ߵ�ֱ��ͼ���ݽṹ
*/
/*------------------------------------------------------------------------------------------------------------*/
#define  AngLeDivided (180/4)
typedef struct histdata{
	float hist_all[AngLeDivided];/**<����ֱ�ߵ�ֱ��ͼ*/
	float hist_sky[AngLeDivided];/**<���ֱ�ߵ�ֱ��ͼ*/
	float hist_ground[AngLeDivided];/**<����ֱ�ߵ�ֱ��ͼ*/
	float hist_vertical[AngLeDivided];/**<��ֱ�����ֱ��ͼ*/
	float hist_Multiply[AngLeDivided];/**<���������˻���ֱ��ͼ�����ã�*/
	float hist_all_count[AngLeDivided];/**<ֱ�߼���ֱ��ͼ*/
}HistData;
#define  HistNUM   5
/*------------------------------------------------------------------------------------------------------------*/
/**
*
*ֱ�ߵ�ֱ��ͼ���ݽṹ
*/
/*------------------------------------------------------------------------------------------------------------*/
#define AngLeDivided180 (180/1)
typedef struct{
	float hist_all[AngLeDivided180];/**<����ֱ�ߵ�ֱ��ͼ*/
	float hist_all_sum;/**<ֱ�߳����ܺ�*/
	float hist_sky[AngLeDivided180];/**<���ֱ�ߵ�ֱ��ͼ*/
	float hist_ground[AngLeDivided180];/**<����ֱ�ߵ�ֱ��ͼ*/
	float hist_vertical[AngLeDivided180];/**<��ֱ�����ֱ��ͼ*/
	float hist_Multiply[AngLeDivided180];/**<���������˻���ֱ��ͼ�����ã�*/
	float hist_all_count[AngLeDivided180];/**<ֱ�߼���ֱ��ͼ*/
	float hist_all_count_sum;/**<ֱ�������ܺ�*/
}HistData180;
#define  HistNUM_INDOOR   5
/*------------------------------------------------------------------------------------------------------------*/
/**
*
*ֱ�ߵ�ֱ��ͼ���ݽṹ
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
	vector<float> hist_all;/**<����ֱ�ߵ�ֱ��ͼ*/
	vector<float> hist_sky;/**<���ֱ�ߵ�ֱ��ͼ*/
	vector<float> hist_ground;/**<����ֱ�ߵ�ֱ��ͼ*/
	vector<float> hist_vertical;/**<��ֱ�����ֱ��ͼ*/
	vector<float> hist_Multiply;/**<���������˻���ֱ��ͼ�����ã�*/
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


