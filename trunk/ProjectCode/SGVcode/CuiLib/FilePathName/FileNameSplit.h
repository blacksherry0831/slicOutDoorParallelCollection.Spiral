#pragma once
#include <platform_cui.h>
#include "tinyxml2/tinyxml2.h"
using namespace tinyxml2;

/*---------------------------------------------------*/
#define  Use_CString FALSE
/*---------------------------------------------------*/
/**
*FileNameSplit 分解全路径字符串
*@note 使用Parse分解\n
*其他函数均弃用
*/
/*---------------------------------------------------*/
class FileNameSplit
{
public:
FileNameSplit(void);
~ FileNameSplit(void);
public:
#if _MSC_VER&&Use_CString
    CString  filepath;/**<文件路径*/
	CString  filefullpath;/**<文件路径+文件名+扩展名*/
	CString filename;/**<文件名+扩展名*/
	CString filefullname;/**<文件全名*/
	CString filetruename;/**<去除标记字符串的文件名*/
	CString  ImagePath;/**<图像路径*/
	CString  XmlCfgPath;/**<Xml配置文件路径*/
	CString  XmlFileName;/**<Xml文件名*/
	CString  XmlSavePath;/**<Xml保存路径*/
#endif
#if _MSC_VER&&Use_CString
	CString GetFileTitleFromFileName(CString FileName, BOOL Ext);   
	CString GetFileName(CString pathname);
	CString GetPath(CString pathname);
	static	CString Convertstring2CS(string cstring);
	static	string  ConvertCS2string(CString cstring);
	void Parse(CString filepath_t);
	void ReadData2Mem(CString filepath_t);
	int GetXmlCfgData(CString filename);
#endif
	vector<CvPoint> line_point;/**<直线上的点集合*/
	void clear(void);
public:
static string toLowerCase(const string& in);
static void getFilesInDirectory(const string& dirName, vector<string>& fileNames, const vector<string>& validExtensions);


	
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
