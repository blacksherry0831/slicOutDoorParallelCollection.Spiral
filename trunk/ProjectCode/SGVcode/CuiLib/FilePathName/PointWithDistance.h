#pragma once
#include <platform_cui.h>
/*---------------------------------------------------*/
/*---------------------------------------------------*/
#ifndef TRUE
#define TRUE 1
#endif
#define USE_POINTDATA  TRUE
/*---------------------------------------------------*/
/**
*
*
*/
/*---------------------------------------------------*/
#if USE_POINTDATA
class PointData
{
public:
	float Xscale;
	float Yscale;

	float Head_Angle_H;
	float Head_Angule_V;
	float dst_direct;
	float YunTaiAngle;
	float dst_laser;

	float horizontal_pos;
	CvPoint img_optic_center;
	int img_height;
	int img_focus;

	PointData(){
		this->Xscale=0;
		this->Yscale=0;

		img_height=768;
		img_optic_center=cvPoint( 5.10795532e+002 ,3.86674835e+002);
		img_focus=8.0/3.5712*768;
		Head_Angle_H=0;
	}
	/*---------------------------------------------------*/
	/**
	*
	*
	*/
	/*---------------------------------------------------*/
public:
	float CalculateHorLinePos(void){
		double delta=img_focus*tan(ChangeDegree2Radian(Head_Angule_V));
		horizontal_pos=img_optic_center.y+delta;
		horizontal_pos/=img_height;
		return 	horizontal_pos;

	}
	/*---------------------------------------------------*/
	/**
	*
	*
	*/
	/*---------------------------------------------------*/
	float ChangeDegree2Radian(float angule)
	{
		return angule*CV_PI/180.0;
	}
	
#if Use_CString&&_MSC_VER
    void SaveImgandParam(CString filepath,CString filename){

		CString filefullpath=filepath+filename;
		string xml_save_path=ConvertCS2string(filefullpath);
		/*---------------------------------------*/
		CvFileStorage *fsW=cvOpenFileStorage(xml_save_path.c_str(),0,CV_STORAGE_APPEND);		
		cvStartWriteStruct(fsW,"Data",CV_NODE_MAP,NULL,cvAttrList(0,0));

		cvWriteReal(fsW,"Head_Angle_H", Head_Angle_H);	
		cvWriteReal(fsW,"Head_Angule_V",Head_Angule_V);	
		//±ê¶¨µã

		cvWriteReal(fsW,"Dst_Dir",dst_direct);
		cvWriteReal(fsW,"YunTaiAngle",YunTaiAngle);
		cvWriteReal(fsW,"Dst_Laser",dst_laser);

		cvWriteReal(fsW,"HorPos",horizontal_pos);
		cvWriteReal(fsW,"CenterPointX",img_optic_center.x);
		cvWriteReal(fsW,"CenterPointY",img_optic_center.y);
		cvWriteReal(fsW,"img_height",img_height);
		cvWriteReal(fsW,"img_focus",img_focus);

		cvEndWriteStruct(fsW);
		cvReleaseFileStorage(&fsW);

	}
	string ConvertCS2string(CString cstring)
	{
		CStringA stra(cstring.GetBuffer(0));
		cstring.ReleaseBuffer();	
		std::string cui_t=stra;		
		stra.ReleaseBuffer();
		return cui_t;
	}
#endif

};
#endif

/*---------------------------------------------------*/
/**
*
*
*/
/*---------------------------------------------------*/
class PointWithDistance
{
public:
#if USE_POINTDATA
	PointData point;
#endif

#if Use_CString&&_MSC_VER
	CString  filepath;
	CString  filefullpath;
	CString filename;
	CString filefullname;
	CString filetruename;
	CString  ImagePath;
	CString  XmlCfgPath;
	CString  XmlFileName;
	CString  XmlSavePath;
	CString GetFileTitleFromFileName(CString FileName, BOOL Ext);   
	CString GetFileName(CString pathname);
	CString GetPath(CString pathname);
	void Parse(CString filepath_t);
	void ReadData2Mem(CString filepath_t);
    string ConvertCS2string(CString cstring);
	int GetXmlCfgData(CString filename);
#endif	
public:
	vector<CvPoint> line_point;
	PointWithDistance(void);
	~PointWithDistance(void);
	void clear(void);
public:	
	void SaveImgFile();
	CvFileNode* IsNodExist(CvFileStorage *);
	void CalculatePointOnImg(void);
	void SaveImgFile2(void);
	void SaveImgandParam(void);
};
/*---------------------------------------------------*/
/**
*
*
*/
/*---------------------------------------------------*/
