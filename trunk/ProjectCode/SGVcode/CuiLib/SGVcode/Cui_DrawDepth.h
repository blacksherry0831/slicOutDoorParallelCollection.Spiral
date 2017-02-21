#pragma once
#include "ImageType.h"
/*----------------------------------------------------------------*/
/**
*摄像机相关的数据结构
*
*
*/
/*----------------------------------------------------------------*/
typedef struct{
	double Camera_focus;/**<相机焦距*/	
	double Camera_film_height;/**<相机的胶片长度*/	
	///////////////////
	double height_over_ground;/**<相机离地面的高度*/	
	/////////////////////
	double img_height;/**<图像的高度*/	
	double img_focus;/**<图像的焦距*/	
	////////////////////
	double Camera_H_angle;/**<相机光轴与水平面的夹角*/	
	float horizontal_pos;/**<水平线位置*/	
}CameraData;
/*----------------------------------------------------------------*/
/**
*本类负责绘制图片的深度图
*@image html color_wheel.jpg 色环 
*色彩梯度取自色环
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
	ImageMemData* pMD;/**<中间变量和内存通用的数据结构*/	
public:
	///////////////////////
	UINT32 *cui_ImgData;/**<图像矩阵*/	
	int*  cui_ImgLables_SVG;/**<天空、立面、地面标定矩阵*/	
	IplImage *cui_DepthImg;/**<深度图*/	
	int cui_Width;/**<图像的宽度*/	
	int cui_Height;/**<图像的高度*/	
	CameraData camera_data;/**<摄像机参数*/	
	SP_PROPERTY sp_preperty[3];/**<天空、立面、地面三个超像素块的属性*/	
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

