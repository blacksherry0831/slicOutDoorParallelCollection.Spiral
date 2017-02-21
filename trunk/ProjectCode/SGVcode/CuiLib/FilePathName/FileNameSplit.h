#pragma once
#include <platform_cui.h>
#include "tinyxml2/tinyxml2.h"
using namespace tinyxml2;

/*---------------------------------------------------*/
#define  Use_CString FALSE
/*---------------------------------------------------*/
/**
*FileNameSplit �ֽ�ȫ·���ַ���
*@note ʹ��Parse�ֽ�\n
*��������������
*/
/*---------------------------------------------------*/
class FileNameSplit
{
public:
FileNameSplit(void);
~ FileNameSplit(void);
public:
#if _MSC_VER&&Use_CString
    CString  filepath;/**<�ļ�·��*/
	CString  filefullpath;/**<�ļ�·��+�ļ���+��չ��*/
	CString filename;/**<�ļ���+��չ��*/
	CString filefullname;/**<�ļ�ȫ��*/
	CString filetruename;/**<ȥ������ַ������ļ���*/
	CString  ImagePath;/**<ͼ��·��*/
	CString  XmlCfgPath;/**<Xml�����ļ�·��*/
	CString  XmlFileName;/**<Xml�ļ���*/
	CString  XmlSavePath;/**<Xml����·��*/
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
	vector<CvPoint> line_point;/**<ֱ���ϵĵ㼯��*/
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
