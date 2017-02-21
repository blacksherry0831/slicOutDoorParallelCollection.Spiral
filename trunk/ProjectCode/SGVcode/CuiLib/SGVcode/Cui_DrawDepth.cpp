#include "StdAfx.h"
#include "module_all_cui.h"
//#include "Cui_DrawDepth.h"

/*----------------------------------------------------------------*/
/**
*���캯��\n
*��ʼ����Ա����
*
*
*
*/
/*----------------------------------------------------------------*/
Cui_DrawDepth::Cui_DrawDepth(void)
{
	this->InitParameter();
}
/*----------------------------------------------------------------*/
/**
*���캯��\n
*��ʼ����Ա����
*
*@param MemData_t �����õ��м����
*/
/*----------------------------------------------------------------*/
Cui_DrawDepth::Cui_DrawDepth(ImageMemData* MemData_t)
{
	this->pMD=MemData_t;
	this->InitParameter();
}
/*----------------------------------------------------------------*/
/**
*��ʼ������Ա����
*
*
*/
/*----------------------------------------------------------------*/
void Cui_DrawDepth::InitParameter(void)
{
   this->cui_ImgData=NULL;
   this->cui_ImgLables_SVG=NULL;
   this->cui_DepthImg=NULL;
}
/*----------------------------------------------------------------*/
/**
*���캯��
*
*
*/
/*----------------------------------------------------------------*/
Cui_DrawDepth::~Cui_DrawDepth(void)
{
	this->ClearImgMemory();
}
/*----------------------------------------------------------------*/
/**
*�����ڴ�
*
*
*/
/*----------------------------------------------------------------*/
void Cui_DrawDepth::ClearImgMemory(void)
{
	this->ReleaseImgBuffer();
}
/*----------------------------------------------------------------*/
/**
*�ͷ�ͼ������õ��ڴ�
*
*
*/
/*----------------------------------------------------------------*/
void Cui_DrawDepth::ReleaseImgBuffer(void)
{
	 if (cui_ImgData){
		 delete []cui_ImgData;
		 cui_ImgData=NULL;
	 }
	 if (cui_ImgLables_SVG){
		 delete []cui_ImgLables_SVG;
		 cui_ImgLables_SVG=NULL;
	 }
	 if (cui_DepthImg){
		 cvReleaseImage(&cui_DepthImg);
	 }
}
/*----------------------------------------------------------------*/
/**
*����ͼ�����ݺ�����������ķ�������
*
*@param ImgData		ͼ�����
*@param Width		ͼ�������
*@param Height		ͼ�����߶�
*@param LablesSVG   ��ա����桢����ı궨����
*
*/
/*----------------------------------------------------------------*/
void Cui_DrawDepth::GetImageData(
	UINT32* ImgData,
	int Width,
	int Height,
	int *LablesSVG)
{
	this->ClearImgMemory();
	this->cui_Width=Width;
	this->cui_Height=Height;
	cui_ImgData=new UINT32[Width*Height];
	memcpy(cui_ImgData,ImgData,sizeof(UINT32)*Width*Height);
	
	cui_ImgLables_SVG=new int[Width*Height];	
	memcpy(cui_ImgLables_SVG,LablesSVG,sizeof(int)*Width*Height);
	cui_DepthImg=cvCreateImage(cvSize(cui_Width,cui_Height),IPL_DEPTH_8U,4);

}
/*----------------------------------------------------------------*/
/**
*���ݽǶ���Lab�ռ�����Ӧ����ɫ
*
*@param  light L���� 
*@param  angle_color AB������ϵ�еĽǶ�
*@return ��ɫֵ
*/
/*----------------------------------------------------------------*/
CvScalar  Cui_DrawDepth::GetAngleColor(double light,double angle_color){
	CvMat mat;
	CvScalar color;	   
	unsigned char  color_4[4];
#if 1
   	angle_color=angle_color*CV_PI/180;
	/*assert(angle_color<=180);*/
	color_4[0]=light;//L
	color_4[1]=100*cos(angle_color)+255/2;
	color_4[2]=100*sin(angle_color)+255/2;
	color_4[3]=0;
	cvInitMatHeader(&mat,1,1,CV_8UC3,&color_4);
	cvCvtColor(&mat,&mat,CV_Lab2BGR);
#else
	assert(angle_color<=180);
	color_4[0]=angle_color;
	color_4[1]=255;
	color_4[2]=255;
	color_4[3]=0;
	cvInitMatHeader(&mat,1,1,CV_8UC3,&color_4);
	cvCvtColor(&mat,&mat,CV_HSV2BGR);
#endif


	// b g r a
	// l a b 0
	color=cvScalar(color_4[0],color_4[1],color_4[2],255);

	return color;
}
/*----------------------------------------------------------------*/
/**
*�����յ���ɫ\n
*�����ɫΪ�̶�ɫ
*@return ��ɫֵ
*/
/*----------------------------------------------------------------*/
CvScalar Cui_DrawDepth::GetSkyColor(void)
{
	//��չ̶���ɫ
#if 1
	return GetAngleColor(250,220);
#else
	return GetAngleColor(90);
#endif

}
/*----------------------------------------------------------------*/
/**
*�����յ���ɫ\n
*�����ɫΪ�̶�ɫ
*@return ��ɫֵ
*/
/*----------------------------------------------------------------*/
CvScalar Cui_DrawDepth::GetSkyColor_InDoor(double angle_color)
{
	//��չ̶���ɫ

	//return GetAngleColor(250,220);

	return GetAngleColor(180,angle_color);
}
/*----------------------------------------------------------------*/
/**
*����������ɫ
*
*@param  angle_color AB������ϵ�еĽǶ�
*@return ��ɫֵ
*/
/*----------------------------------------------------------------*/
CvScalar Cui_DrawDepth::GetVerticalColor(double angle_color)
{
   //
	return GetAngleColor(120,angle_color);
}
/*----------------------------------------------------------------*/
/**
*��õ������ɫ�ݶ�
*
*@param  angle_color AB������ϵ�еĽǶ�
*@return ��ɫֵ
*/
/*----------------------------------------------------------------*/
CvScalar Cui_DrawDepth::GetGroundColor(double angle_color)
{
	//��չ̶���ɫ
	return GetAngleColor(120,angle_color);
}
/*----------------------------------------------------------------*/
/**
*�����������
*@param  focus  ���� 
*@param  height_over_ground �����ظ߶�
*@param film_height         ����߶�
*@param  horizontal_pos ��ƽ��λ��
*/
/*----------------------------------------------------------------*/
void Cui_DrawDepth::SetCameraParameter(
	double focus,
	double height_over_ground,
	double film_height,
	float horizontal_pos)
{
   this->camera_data.Camera_focus=focus;
   this->camera_data.Camera_film_height=film_height;
   ///////////////////////////////////////////////////
   this->camera_data.height_over_ground=height_over_ground;
   ///////////////////////////////////////////////////
   this->camera_data.img_height=this->cui_Height;
   this->camera_data.img_focus=camera_data.Camera_focus*camera_data.img_height/camera_data.Camera_film_height;
   this->camera_data.horizontal_pos=horizontal_pos;
   /////////////////////////////////////////////////////
}
/*----------------------------------------------------------------*/
/**
*���ͼ�ϵ�����ɫ�������ϵĽǶ�
*
*@param x ͼ�ϵ��X����
*@param y ͼ�ϵ��Y����
*return ��ɫ�ļ�����Ƕ�
*/
/*----------------------------------------------------------------*/
double Cui_DrawDepth::GetPointAngleD0(int x,int y)
{
	//0--90du
	double  ColorDrange[2]={93,-50};
	double  AlphaAngle[2];	
	AlphaAngle[0]=atan(camera_data.img_height*(1-camera_data.horizontal_pos)/camera_data.img_focus)*180/CV_PI;
	AlphaAngle[1]=0;
   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    double alpha_t=atan((y-camera_data.img_height*camera_data.horizontal_pos)/camera_data.img_focus)*180/CV_PI;
 	double alpha_angle=ColorDrange[0]-1.0*(AlphaAngle[0]-alpha_t)/(AlphaAngle[0]-AlphaAngle[1])*(ColorDrange[0]-ColorDrange[1]);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return alpha_angle;
}
/*----------------------------------------------------------------*/
/**
*���ͼ�ϵ�����ɫ�������ϵĽǶ�
*
*@param x ͼ�ϵ��X����
*@param y ͼ�ϵ��Y����
*return ��ɫ�ļ�����Ƕ�
*/
/*----------------------------------------------------------------*/
double Cui_DrawDepth::GetPointAngleD0_InDoorSky(int x,int y)
{
	//0--90du
	double  ColorDrange[2]={180+5,270-15};
	double  AlphaAngle[2];	
	AlphaAngle[0]=atan(camera_data.img_height*(1-camera_data.horizontal_pos)/camera_data.img_focus)*180/CV_PI;
	AlphaAngle[1]=0;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	double alpha_t=atan((y-camera_data.img_height*camera_data.horizontal_pos)/camera_data.img_focus)*180/CV_PI;
	double alpha_angle=ColorDrange[0]-1.0*(AlphaAngle[0]-alpha_t)/(AlphaAngle[0]-AlphaAngle[1])*(ColorDrange[0]-ColorDrange[1]);
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return alpha_angle;
}
/*----------------------------------------------------------------*/
/**
*���ͼ�ϵ�����ɫ�������ϵĽǶȣ������ã�
*
*@param x ͼ�ϵ��X����
*@param y ͼ�ϵ��Y����
*@return ��ɫ�ļ�����Ƕ�
*/
/*----------------------------------------------------------------*/
double Cui_DrawDepth::GetPointAngleD0_forZlm(int x,int y)
{
	//0--90du
	double  ColorDrange[2]={45,-50};
	double  AlphaAngle[2];	
	AlphaAngle[0]=cui_Height;
	AlphaAngle[1]=0;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	double alpha_angle=y*(ColorDrange[0]-ColorDrange[1])/AlphaAngle[0]+ColorDrange[1];
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return alpha_angle;
}
/*----------------------------------------------------------------*/
/**
*���Ƶ������\n
*@image html image136.jpg �Ӿ�����ϵͳ����ģ�� 
*@note Y����λ��--�ӽ�--��ɫ
*
*/
/*----------------------------------------------------------------*/
void Cui_DrawDepth::DrawGroundDepth(void)
{
	unsigned int Y_SegPosition;
	SP_PROPERTY G_Preperty=sp_preperty[2];

	for (register int x=0;x<cui_Width;x++){
		//Y_SegPosition=GetSegmentPoint(x);
		for (register int y=cui_Height-1;y>=0;y--){
			 if (cui_ImgLables_SVG[y*cui_Width+x]==Ground){
				 double  angule_t=this->GetPointAngleD0(x,y);
				// double angule_t=Drange[1]+1.0*(y-G_Preperty.min_y)/(G_Preperty.max_y-G_Preperty.min_y)*(Drange[0]-Drange[1]);
				 CvScalar color_t=this->GetGroundColor(angule_t);
				 cvDrawCircle(cui_DepthImg,cvPoint(x,y),1,color_t,-1);
			 }

		}

	}
}
/*----------------------------------------------------------------*/
/**
*���Ƶ�����ȵ���һ�ַ���
*
*
*/
/*----------------------------------------------------------------*/
void Cui_DrawDepth::DrawGroundDepth_forZlm(void)
{
	unsigned int Y_SegPosition;
	SP_PROPERTY G_Preperty=sp_preperty[2];

	for (register int x=0;x<cui_Width;x++){
		//Y_SegPosition=GetSegmentPoint(x);
		for (register int y=cui_Height-1;y>=0;y--){
			if (cui_ImgLables_SVG[y*cui_Width+x]==Ground){
				double  angule_t=this->GetPointAngleD0_forZlm(x,y);
				// double angule_t=Drange[1]+1.0*(y-G_Preperty.min_y)/(G_Preperty.max_y-G_Preperty.min_y)*(Drange[0]-Drange[1]);
				CvScalar color_t=this->GetGroundColor(angule_t);
				cvDrawCircle(cui_DepthImg,cvPoint(x,y),1,color_t,-1);
			}

		}

	}
}
/*----------------------------------------------------------------*/
/**
*�����������
*
*
*/
/*----------------------------------------------------------------*/
void Cui_DrawDepth::DrawVerticalDepth(void)
{
	unsigned int Y_SegPosition;
	CvScalar color_t;
	for (register int x=0;x<cui_Width;x++){
		 bool is_color_get=false;
		for (register int y=cui_Height-1;y>=0;y--){
			if (cui_ImgLables_SVG[y*cui_Width+x]==Vertical){
				if (is_color_get==false){
					color_t=this->GetGroundColor(this->GetPointAngleD0(x,y));
					is_color_get=true;
				}
				 cvDrawCircle(cui_DepthImg,cvPoint(x,y),1,color_t,-1);
			}	

		}

	}
}
/*----------------------------------------------------------------*/
/**
*�������������һ�ַ���
*
*
*/
/*----------------------------------------------------------------*/
void Cui_DrawDepth::DrawVerticalDepth_forZlm(void)
{
	unsigned int Y_SegPosition;
	CvScalar color_t;
	for (register int x=0;x<cui_Width;x++){
		bool is_color_get=false;
		for (register int y=cui_Height-1;y>=0;y--){
			if (cui_ImgLables_SVG[y*cui_Width+x]==Vertical){
				
					color_t=this->GetGroundColor(this->GetPointAngleD0_forZlm(x,y));

				cvDrawCircle(cui_DepthImg,cvPoint(x,y),1,color_t,-1);
			}	

		}

	}
}
/*----------------------------------------------------------------*/
/**
*������յ����ͼ
*
*
*/
/*----------------------------------------------------------------*/
void Cui_DrawDepth::DrawSkyDepth(void)
{
	unsigned int Y_SegPosition;
	for (register int x=0;x<cui_Width;x++){
		//Y_SegPosition=GetSegmentPoint(x);
		for (register int y=cui_Height-1;y>=0;y--){
			if (cui_ImgLables_SVG[y*cui_Width+x]==Sky){
				CvScalar color_t=this->GetSkyColor();
				cvDrawCircle(cui_DepthImg,cvPoint(x,y),1,color_t,-1);
			}	

		}

	}
}
/*----------------------------------------------------------------*/
/**
*������յ����ͼ
*
*
*/
/*----------------------------------------------------------------*/
void Cui_DrawDepth::DrawSkyDepth_InDoor(void)
{
	unsigned int Y_SegPosition;
	for (register int x=0;x<cui_Width;x++){
		//Y_SegPosition=GetSegmentPoint(x);
		for (register int y=cui_Height-1;y>=0;y--){
			if (cui_ImgLables_SVG[y*cui_Width+x]==Sky){
#if TRUE
				double  angule_t=this->GetPointAngleD0_InDoorSky(x,y);
				CvScalar color_t=this->GetSkyColor_InDoor(angule_t);
#endif
#if FALSE
				double  angule_t=this->GetPointAngleD0(x,cui_Height-y);
				CvScalar color_t=this->GetGroundColor(angule_t);
#endif
				
				 cvDrawCircle(cui_DepthImg,cvPoint(x,y),1,color_t,-1);
			}	

		}

	}
}
/*----------------------------------------------------------------*/
/**
*�������ͼ
*
*
*/
/*----------------------------------------------------------------*/
void Cui_DrawDepth::DrawDepth(void)
{
	this->SetSpProperty();	
#if OUT_NOGROUND_IMG
	this->DrawGroundDepth_forZlm();
#else
	this->DrawGroundDepth();
#endif

#if OUT_NOGROUND_IMG
	this->DrawVerticalDepth_forZlm();
#else
	this->DrawVerticalDepth();	
#endif

	this->DrawSkyDepth();
/*****************����ͼƬ*********************************************************************************/

#if OUT_NOGROUND_IMG

//	cui_GeneralImgProcess::DrawContoursAroundSegments((unsigned int *&)cui_DepthImg->imageData,cui_ImgLables_SVG,cui_DepthImg->width,cui_DepthImg->height,0x00000000);
	/*************************************************/
	string file_name;
	char fname[_MAX_FNAME];
	_splitpath(FileReadFullPath.c_str(), NULL, NULL, fname, NULL);
	file_name=FileWritePath+fname+"Depth.jpg";
	cvSaveImage(file_name.c_str(),cui_DepthImg);
	/*************************************************************************************************/
	
#else

	
#if 0
	//INT32 Matrix_Category_Lable_SVG[4]={0,Ground,-1,Sky};
#else
	//INT32 Matrix_Category_Lable_SVG[4]={0,-1,-1,Sky};
#endif
	
	//cui_GeneralImgProcess::Cui_CombinationImgSVG(cui_ImgData,cui_ImgLables,cui_ImgLables_SVG,pMatrix_Category_Lable_SVG,cui_Width,cui_Height,"","","");
	cui_GeneralImgProcess::CuiSaveImgWithPoints((unsigned int *&)cui_DepthImg->imageData,
		cui_ImgLables_SVG,nullptr,
		cui_Width,cui_Height,false,
		pMD,"Depth.jpg");
	
#endif	
	
	

}
/*----------------------------------------------------------------*/
/**
*�������ͼ
*���������ˮƽ������չ�ҽ�Ϊƽ�����Ӿ�����ϵͳ����ȷ�ķ����ԣ���ͼ����\n
*ԵΪ3D�ռ�����Ϸ�����ԵΪ3D�ռ�����·�������С�׳���ԭ����Ӿ�ϵͳ��\n
*��ģ����ͼ��ʾ�����������Ϣ��ͼ���е�������λ�õ�͸��ͶӰ��ϵ���£�\n
*@image html image_detph_camera.png ���������Ϣ��ͼ���е�������λ�õ�͸��ͶӰ��ϵ 
*
*
*@note   
*/
/*----------------------------------------------------------------*/
void Cui_DrawDepth::DrawDepth_Indoor(void)
{
	this->SetSpProperty();	
#if OUT_NOGROUND_IMG
	this->DrawGroundDepth_forZlm();
#else
	this->DrawGroundDepth();
#endif

#if OUT_NOGROUND_IMG
	this->DrawVerticalDepth_forZlm();
#else
	this->DrawVerticalDepth();	
#endif

	this->DrawSkyDepth_InDoor();
	/*****************����ͼƬ*********************************************************************************/

#if OUT_NOGROUND_IMG

	//	cui_GeneralImgProcess::DrawContoursAroundSegments((unsigned int *&)cui_DepthImg->imageData,cui_ImgLables_SVG,cui_DepthImg->width,cui_DepthImg->height,0x00000000);
	/*************************************************/
	string file_name;
	char fname[_MAX_FNAME];
	_splitpath(FileReadFullPath.c_str(), NULL, NULL, fname, NULL);
	file_name=FileWritePath+fname+"Depth.jpg";
	cvSaveImage(file_name.c_str(),cui_DepthImg);
	/*************************************************************************************************/

#else
	/////////////////////////////////////////////////////////////////
	string file_name;
	char fname[_MAX_FNAME];
#if _MSC_VER
	_splitpath(pMD->FileReadFullPath.c_str(), NULL, NULL, fname, NULL);
#elif __GNUC__
	cui_GeneralImgProcess::_splitpath(pMD->FileReadFullPath.c_str(), NULL, NULL, fname, NULL);
#else
	 ASSERT(0);
#endif	
	file_name=pMD->FileWritePath+fname+"DepthNoLables.jpg";
	cvSaveImage(file_name.c_str(),cui_DepthImg);
	/////////////////////////////////////////////////////////////////
	cui_GeneralImgProcess::CuiSaveImgWithPoints(
		(UINT32*)cui_DepthImg->imageData,
		cui_ImgLables_SVG,
		nullptr,
		cui_Width,
		cui_Height,
		false,
		pMD,
		"Depth.jpg");
	////////////////////////////////////////////////////////////////////
#endif
}
/*----------------------------------------------------------------*/
/**
*���ó����ص�����
*
*
*/
/*----------------------------------------------------------------*/
void Cui_DrawDepth::SetSpProperty(void)
{
	memset(sp_preperty,0,sizeof(sp_preperty));
	/******************************************/
	for (register int x=0;x<cui_Width;x++){
		//Y_SegPosition=GetSegmentPoint(x);
		for (register int y=cui_Height-1;y>=0;y--){

			if (cui_ImgLables_SVG[y*cui_Width+x]==Sky){
			    if (sp_preperty[0].IsInit_SquareMeter==0){
					sp_preperty[0].IsInit_SquareMeter=1;
					sp_preperty[0].max_x=sp_preperty[0].min_x=x;
					sp_preperty[0].max_y=sp_preperty[0].min_y=y;
					sp_preperty[0].SPCategory=Sky;
				}else{
					 if (sp_preperty[0].min_y>=y){
						 sp_preperty[0].min_y=y;
					 }
					 if (sp_preperty[0].max_y<=y){
						 sp_preperty[0].max_y=y;
					 }
				}
			     

			}else if (cui_ImgLables_SVG[y*cui_Width+x]==Vertical){
			
				if (sp_preperty[1].IsInit_SquareMeter==0){
					sp_preperty[1].IsInit_SquareMeter=1;
					sp_preperty[1].max_x=sp_preperty[1].min_x=x;
					sp_preperty[1].max_y=sp_preperty[1].min_y=y;
					sp_preperty[1].SPCategory=Sky;
				}else{
					if (sp_preperty[1].min_y>=y){
						sp_preperty[1].min_y=y;
					}
					if (sp_preperty[1].max_y<=y){
						sp_preperty[1].max_y=y;
					}
				}
			} 
			else if(cui_ImgLables_SVG[y*cui_Width+x]==Ground){
				
				if (sp_preperty[2].IsInit_SquareMeter==0){
					sp_preperty[2].IsInit_SquareMeter=1;
					sp_preperty[2].max_x=sp_preperty[2].min_x=x;
					sp_preperty[2].max_y=sp_preperty[2].min_y=y;
					sp_preperty[2].SPCategory=Sky;
				}else{
					if (sp_preperty[2].min_y>=y){
						sp_preperty[2].min_y=y;
					}
					if (sp_preperty[2].max_y<=y){
						sp_preperty[2].max_y=y;
					}
				}

			}


		}
	}
	/******************************************/

}
/*----------------------------------------------------------------*/