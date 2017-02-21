#pragma once
#include "ImageType.h"
/*----------------------------------------------------------------*/
/**
*�������ص����ݽṹ
*
*
*/
/*----------------------------------------------------------------*/
typedef struct{
	double Camera_focus;/**<�������*/	
	double Camera_film_height;/**<����Ľ�Ƭ����*/	
	///////////////////
	double height_over_ground;/**<��������ĸ߶�*/	
	/////////////////////
	double img_height;/**<ͼ��ĸ߶�*/	
	double img_focus;/**<ͼ��Ľ���*/	
	////////////////////
	double Camera_H_angle;/**<���������ˮƽ��ļн�*/	
	float horizontal_pos;/**<ˮƽ��λ��*/	
}CameraData;
/*----------------------------------------------------------------*/
/**
*���ฺ�����ͼƬ�����ͼ
*@image html color_wheel.jpg ɫ�� 
*ɫ���ݶ�ȡ��ɫ��
*
*/
/*----------------------------------------------------------------*/
class Cui_DrawDepth
{
public:
	Cui_DrawDepth(ImageMemData* MemData_t);
	Cui_DrawDepth(void);
	~Cui_DrawDepth(void);

private:
	ImageMemData* pMD;/**<�м�������ڴ�ͨ�õ����ݽṹ*/	
public:
	///////////////////////
	UINT32 *cui_ImgData;/**<ͼ�����*/	
	int*  cui_ImgLables_SVG;/**<��ա����桢����궨����*/	
	IplImage *cui_DepthImg;/**<���ͼ*/	
	int cui_Width;/**<ͼ��Ŀ��*/	
	int cui_Height;/**<ͼ��ĸ߶�*/	
	CameraData camera_data;/**<���������*/	
	SP_PROPERTY sp_preperty[3];/**<��ա����桢�������������ؿ������*/	
public:
	void DrawDepth(void);
	void DrawDepth_Indoor(void);
	void GetImageData(
		UINT32* ImgData,int Width,int Height,
		int *LablesSVG);
	void SetCameraParameter(double focus, double height_over_ground,double film_height,float horizontal_pos);
public:
private:
	void SetFileReadSavePath(string WritePath, string ReadPath);
	void ClearImgMemory(void);

	void InitParameter(void);
	void ReleaseImgBuffer(void);

	
	
	static	CvScalar GetAngleColor(double light,double angle_color);

	CvScalar GetSkyColor(void);
	CvScalar GetSkyColor_InDoor(double angle_color);
	CvScalar GetVerticalColor(double angle_color);
	CvScalar GetGroundColor(double angle_color);

	double GetPointAngleD0(int x,int y);
	double GetPointAngleD0_forZlm(int x,int y);
	double GetPointAngleD0_InDoorSky(int x,int y);

	void DrawGroundDepth(void);
	void DrawVerticalDepth(void);
	void DrawVerticalDepth_forZlm(void);
	void DrawGroundDepth_forZlm(void);
	void DrawSkyDepth(void);
	void DrawSkyDepth_InDoor(void);
	void SetSpProperty(void);
};

