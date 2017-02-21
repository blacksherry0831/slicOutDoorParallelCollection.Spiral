#include "StdAfx.h"
#include "FileNameSplit.h"

#if _MSC_VER
#include "SVMcode/dirent.h"
#endif

/*---------------------------------------------------*/
/**
*
*构造
*/
/*---------------------------------------------------*/
FileNameSplit::FileNameSplit(void)
{
#if USE_POINTDATA
this->line_point.clear();
#endif
}
/*---------------------------------------------------*/
/**
*
*
*/
/*---------------------------------------------------*/
FileNameSplit::~FileNameSplit(void)
{
}
#if _MSC_VER&&Use_CString
/*---------------------------------------------------*/
/**
*从文件全名名中获得文件名
*
*@param FileName 文件全名 
*@param Ext true 文件扩展名是否包含 
*@return 文件名  
*/
/*---------------------------------------------------*/
CString FileNameSplit::GetFileTitleFromFileName(CString FileName, BOOL Ext)   
{   
	int Where;   
	Where = FileName.ReverseFind('//');  
	if (Where == -1)  
		Where = FileName.ReverseFind('/');  
	CString FileTitle = FileName.Right(FileName.GetLength() - 1 - Where);  
	if (!Ext)  
	{  
		int Which = FileTitle.ReverseFind('.');   
		if (Which != -1)   
			FileTitle = FileTitle.Left(Which);   
	}   
	return FileTitle;   
} 
/*---------------------------------------------------*/
/**
*获得文件名
*
*@param pathname 文件路径 
*return 文件名  
*/
/*---------------------------------------------------*/
CString FileNameSplit::GetFileName(CString pathname) 
{ 	int i;
	for(i=pathname.GetLength()-1; i>=0; i-- ) 
	{ 
		if( pathname[i]=='\\' ) 
			break; 
	} 
	return pathname.Mid( i+1 ); 
}
/*---------------------------------------------------*/
/**
*从文件全名获得
*@parame pathname  
*/
/*---------------------------------------------------*/
CString FileNameSplit::GetPath(CString pathname)  
{
	int i = 0, j=0;  
	while( i<pathname.GetLength() )  
	{  
		if( pathname[i]=='//'||pathname[i]=='\\' )  
			j = i;  
		i++;  
	}  
	return pathname.Left( j+1 );  
}
/*---------------------------------------------------*/
/**
*分解字符串
*@param filepath_t 文件全名 
*
*/
/*---------------------------------------------------*/
void FileNameSplit::Parse(CString filepath_t){
	// this->VerifyAndRepairPicture(filepath_t);
	this->ReadData2Mem(filepath_t);
}
/*---------------------------------------------------*/
/**
*分解文件名、文件路径、读取图片
*@param filepath_t  
*/
/*---------------------------------------------------*/
void FileNameSplit::ReadData2Mem(CString filepath_t)
{
	this->clear();
	this->filefullpath=filepath_t;
	this->filefullname=GetFileName(filepath_t);
	this->filepath=GetPath(filepath_t);
	this->filename=GetFileTitleFromFileName(filefullname,FALSE);
	this->ImagePath=filepath.Left(filepath.Find(_T("OrgImg")));
	this->XmlFileName=this->filename+_T(".xml");
	this->XmlCfgPath=this->ImagePath+_T("ParamImg\\")+this->XmlFileName;
	//this->GetXmlCfgData(this->XmlCfgPath);
	this->XmlSavePath=this->ImagePath+_T("PointData.xml");
}

#endif
/*---------------------------------------------------*/
/**
*
*
*/
/*---------------------------------------------------*/
void FileNameSplit::clear(void){
	
}

#if _MSC_VER&&Use_CString
/*---------------------------------------------------*/
/**
*
*
*/
/*---------------------------------------------------*/
int FileNameSplit::GetXmlCfgData(CString filename)
{
	
	return 0;
}

/*---------------------------------------------------*/
/**
*CString 字符串转换到string字符串
*@param CString字符串  
*@return cstring string字符串 
*/
/*---------------------------------------------------*/
string FileNameSplit::ConvertCS2string(CString cstring)
{
	CStringA stra(cstring.GetBuffer(0));
	cstring.ReleaseBuffer();	
	std::string cui_t=stra;		
	stra.ReleaseBuffer();
	return cui_t;
}
/*---------------------------------------------------*/
/**
*string字符串转换到CString 字符串
*@param cstring string字符串 
*@return CString字符串  
*/
/*---------------------------------------------------*/
CString FileNameSplit::Convertstring2CS(string cstring)
{
	CStringA cstra=cstring.c_str();
	CString  cstrw(cstra);
	return cstrw;
}

#endif

/*-------------------------------------------------------------------*/
/**

<opencv>
	
	<Data>
	<File></File>
	<><>
	<><>
	<><>
	</Data>

<opencv>

*/
/*-------------------------------------------------------------------*/
void FileNameSplit::SaveImgFile2()
{


}
/*---------------------------------------------------*/
/**
*
*@deprecated
*/
/*---------------------------------------------------*/
CvFileNode* FileNameSplit::IsNodExist(CvFileStorage *fsW)
{
	
	CvFileNode* RootNode=cvGetRootFileNode(fsW);

	CvFileNode* DataNodes=cvGetFileNodeByName(fsW,0,"Data");

	if (RootNode==NULL||DataNodes==NULL){
		return NULL;
	}
	
	CvFileNode* TopNode=DataNodes;

	if( CV_NODE_IS_SEQ( TopNode->tag) )
	{
		CvSeq* seq = TopNode->data.seq;
		int i, total = seq->total;
		CvSeqReader reader;

		cvStartReadSeq( seq, &reader, 0 );
		for( i = 0; i < total; i++ )
		{
			CvFileNode* pt = (CvFileNode*)reader.ptr;
#if Use_CString&&_MSC_VER
			string filename_t=cvReadStringByName(fsW,pt,"File",NULL);
			if (filename_t==ConvertCS2string(this->filename)){
				return pt;

			}

#endif
			CV_NEXT_SEQ_ELEM( seq->elem_size, reader );
			
		}
	}

	return NULL;
}
/*-------------------------------------------------------------------------------------*/
/**
*获得目录下的所有文件
*@param dirName 目录名 
*@param fileNames 文件名 
*@param validExtensions 文件扩展名 
*/
/*-------------------------------------------------------------------------------------*/
void FileNameSplit::getFilesInDirectory(const string& dirName, vector<string>& fileNames, const vector<string>& validExtensions)
{
	printf("Opening directory %s\n", dirName.c_str());
	struct dirent* ep;
	size_t extensionLocation;
	DIR* dp = opendir(dirName.c_str());
	fileNames.clear();
	if (dp != NULL) {
		while ((ep = readdir(dp))) {
			// Ignore (sub-)directories like . , .. , .svn, etc.
			if (ep->d_type & DT_DIR) {
				continue;
			}
			extensionLocation = string(ep->d_name).find_last_of("."); // Assume the last point marks beginning of extension like file.ext
			// Check if extension is matching the wanted ones
			string tempExt = toLowerCase(string(ep->d_name).substr(extensionLocation + 1));
			if (find(validExtensions.begin(), validExtensions.end(), tempExt) != validExtensions.end()) {
				printf("Found matching data file '%s'\n", ep->d_name);
				fileNames.push_back((string) dirName + ep->d_name);
			} else {
				printf("Found file does not match required file type, skipping: '%s'\n", ep->d_name);
			}
		}
		(void) closedir(dp);
	} else {
		printf("Error opening directory '%s'!\n", dirName.c_str());
	}
	return;
}
/*-------------------------------------------------------------------------------------*/
/**
*字符串装换到小写
*@param in 输入字符串 
*@return 小写字符串
*/
/*-------------------------------------------------------------------------------------*/
string FileNameSplit::toLowerCase(const string& in)
{
	string t;
	for (string::const_iterator i = in.begin(); i != in.end(); ++i) {
		t += tolower(*i);
	}
	return t;
}
/*-------------------------------------------------------------------------------------*/
/*
*
*/
/*-------------------------------------------------------------------------------------*/