#include "StdAfx.h"
#include "PointWithDistance.h"
#ifdef __GNUC__
#include "tinyxml2/tinyxml2.h"
#endif
#if _MSC_VER
#include "SVMcode/dirent.h"
#endif

#include "tinyxml2/tinyxml2.h"
using namespace tinyxml2;
/*---------------------------------------------------*/
/**
*
*
*/
/*---------------------------------------------------*/
PointWithDistance::PointWithDistance(void)
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

PointWithDistance::~PointWithDistance(void)
{
}
/*---------------------------------------------------*/
/**
*
*
*/
/*---------------------------------------------------*/
#if Use_CString&&_MSC_VER
CString PointWithDistance::GetFileTitleFromFileName(CString FileName, BOOL Ext)   
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
*
*
*/
/*---------------------------------------------------*/
CString PointWithDistance::GetFileName(CString pathname) 
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
*
*
*/
/*---------------------------------------------------*/
CString PointWithDistance::GetPath(CString pathname)  
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
*
*
*/
/*---------------------------------------------------*/
void PointWithDistance::Parse(CString filepath_t){
	// this->VerifyAndRepairPicture(filepath_t);
	ASSERT(filepath_t!="");
	this->ReadData2Mem(filepath_t);
}
/*---------------------------------------------------*/
/**
*
*
*/
/*---------------------------------------------------*/
void PointWithDistance::ReadData2Mem(CString filepath_t)
{
	this->clear();
	this->filefullpath=filepath_t;
	this->filefullname=GetFileName(filepath_t);
	this->filepath=GetPath(filepath_t);
	this->filename=GetFileTitleFromFileName(filefullname,FALSE);
	this->ImagePath=filepath.Left(filepath.Find(_T("OrgImg")));
	this->XmlFileName=this->filename+_T(".xml");
	this->XmlCfgPath=this->ImagePath+_T("ParamImg\\")+this->XmlFileName;
	this->GetXmlCfgData(this->XmlCfgPath);
	this->XmlSavePath=this->ImagePath+_T("PointData.xml");
}
#endif
/*---------------------------------------------------*/
/**
*
*
*/
/*---------------------------------------------------*/
void PointWithDistance::clear(void){
	
}
/*---------------------------------------------------*/
/**
*
*
*/
/*---------------------------------------------------*/
#if Use_CString&&_MSC_VER
int PointWithDistance::GetXmlCfgData(CString filename)
{
	ASSERT(filename!="");
	string xml_cgf_path=this->ConvertCS2string(filename); 
	CvFileStorage *fsW=cvOpenFileStorage(xml_cgf_path.c_str(),0, CV_STORAGE_READ );	
	CvFileNode* RootNode=cvGetRootFileNode(fsW);
	CvFileNode* DataNode=cvGetFileNodeByName(fsW,0,"Data");
	this->point.Head_Angle_H=cvReadRealByName(fsW,DataNode,"Head_Angle_H",0);
	this->point.Head_Angule_V=cvReadRealByName(fsW,DataNode,"Head_Angule_V",0);
	this->point.dst_direct=cvReadRealByName(fsW,DataNode,"Dst_Dir",0);
	this->point.dst_laser=cvReadRealByName(fsW,DataNode,"Dst_Laser",0);
	this->point.YunTaiAngle=cvReadRealByName(fsW,DataNode,"YunTaiAngle",0);
#if 0
this->point.Head_Angule_V=acos(this->point.dst_direct/this->point.dst_laser)*180/3.141592653;
#endif
	cvReleaseFileStorage(&fsW);
	return 0;
}
/*---------------------------------------------------*/
/**
*
*
*/
/*---------------------------------------------------*/
string PointWithDistance::ConvertCS2string(CString cstring)
{
	CStringA stra(cstring.GetBuffer(0));
	cstring.ReleaseBuffer();	
	std::string cui_t=stra;		
	stra.ReleaseBuffer();
	return cui_t;
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
void PointWithDistance::SaveImgFile2()
{

	if (line_point.size()<2){
		return;
	}
	this->CalculatePointOnImg();
	/********************************/
	string xml_save_path;
#if Use_CString&&_MSC_VER
   xml_save_path=ConvertCS2string(this->XmlSavePath);
#endif
#if linux||__linux||__linux__
   xml_save_path="linux.xml";
#endif

	CvFileStorage *fsW=cvOpenFileStorage(xml_save_path.c_str(),0,CV_STORAGE_READ|CV_STORAGE_APPEND|CV_STORAGE_FORMAT_XML);
	CvFileNode* RootNode=cvGetRootFileNode(fsW);
	CvFileNode* Data=this->IsNodExist(fsW);
	if (Data==NULL){
		cvStartWriteStruct(fsW,"Data",CV_NODE_MAP,NULL,cvAttrList(0,0));
#if  Use_CString&&_MSC_VER
		cvWriteString(fsW,"File",ConvertCS2string(this->filename).c_str());	
#endif		
		cvWriteReal(fsW,"Head_Angle_H",this->point.Head_Angle_H);	
		cvWriteReal(fsW,"Head_Angule_V",this->point.Head_Angule_V);
		cvWriteReal(fsW,"Dst_Dir",this->point.dst_direct);
		cvWriteReal(fsW,"YunTaiAngle",this->point.YunTaiAngle);
		cvWriteReal(fsW,"Dst_Laser",this->point.dst_laser);

		cvWriteReal(fsW,"X_pos",this->point.Xscale);
		cvWriteReal(fsW,"Y_pos",this->point.Yscale);

		cvEndWriteStruct(fsW);
	}
	cvReleaseFileStorage(&fsW);
}
/*---------------------------------------------------*/
/**
*
*
*/
/*---------------------------------------------------*/
CvFileNode* PointWithDistance::IsNodExist(CvFileStorage *fsW)
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
#if _MSC_VER&&Use_CString
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
/*-----------------------------------------------------------------------------
CString filepath=make_R_param_img_dir();
CString filename=filename_t+_T(".xml");
CString filefullpath=filepath+time+filename;
string xml_save_path=ConvertCS2string(filefullpath);

CvFileStorage *fsW=cvOpenFileStorage(xml_save_path.c_str(),0,CV_STORAGE_APPEND);		
cvStartWriteStruct(fsW,"Data",CV_NODE_MAP,NULL,cvAttrList(0,0));

cvWriteReal(fsW,"Head_Angle_H", Current_angle_h);	
cvWriteReal(fsW,"Head_Angule_V",(int)Current_angle_v);	
//标定点
#if CameraMiddle
cvWriteString(fsW,"CameraPos","Middle");
#else
cvWriteString(fsW,"CameraPos","Side");
#endif
cvWriteReal(fsW,"Dst_Dir",Current_distance_direct);
cvWriteReal(fsW,"YunTaiAngle",Yuntai_angle);
cvWriteReal(fsW,"Dst_Laser",Current_distance_laser);

for (unsigned int i=0;i<this->PointCalibration.point.size();i++){
	cvStartWriteStruct(fsW,"PointData",CV_NODE_MAP,NULL,cvAttrList(0,0));

	cvWriteReal(fsW,"X_pos",PointCalibration.point[i].Xscale);	
	cvWriteReal(fsW,"Y_pos",PointCalibration.point[i].Yscale);							
	cvWriteReal(fsW,"Dst_Dir",PointCalibration.point[i].dst_direct);
	cvWriteReal(fsW,"YunTaiAngle",PointCalibration.point[i].YunTaiAngle);
	cvWriteReal(fsW,"Dst_Laser",PointCalibration.point[i].dst_laser);
	cvWriteReal(fsW,"Des_Index",PointCalibration.point[i].index);
	cvWriteString(fsW,"Des_name",PointCalibration.point[i].PointNane.c_str());

	cvEndWriteStruct(fsW);
}
cvEndWriteStruct(fsW);
cvReleaseFileStorage(&fsW);
--------------------------------------------------------------------------------*/
/*---------------------------------------------------*/
/**
*
*
*/
/*---------------------------------------------------*/
void PointWithDistance::CalculatePointOnImg(void)
{
	float avg_x=0;
	float avg_y=0;
	for (int i=0;i<line_point.size();i++){
		avg_x+=line_point[i].x;
		avg_y+=line_point[i].y;

	}

	avg_x=avg_x/line_point.size();
	avg_y=avg_y/line_point.size();
	
	this->point.Xscale=avg_x;
	this->point.Yscale=avg_y;

}
/*---------------------------------------------------*/
/**
*
*
*/
/*---------------------------------------------------*/
void PointWithDistance::SaveImgFile(void)
{
	if (line_point.size()<2){
		return;
	}
	this->CalculatePointOnImg();
	string xml_save_path;
#if Use_CString&&_MSC_VER
	xml_save_path=ConvertCS2string(this->XmlSavePath);
#endif
#if linux||__linux||__linux__
	xml_save_path="linuxxml.xml";
#endif
	/***************************************/
	 tinyxml2::XMLDocument  myDocument;
	 myDocument.LoadFile(xml_save_path.c_str()); 
#if 0
 //声明
	 tinyxml2::XMLDeclaration *pDeclaration =myDocument.ToDeclaration();
	 if (pDeclaration==NULL){
		 pDeclaration=myDocument.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
		 myDocument.LinkEndChild(pDeclaration); 
	 }
#endif
		 
	 //根节点
	 tinyxml2::XMLElement *pRootEle=myDocument.RootElement();
	 if (pRootEle==NULL){
		 pRootEle = myDocument.NewElement("DataSet");
		 myDocument.LinkEndChild(pRootEle);
	 }
	 string rootvalue=pRootEle->Value();
	 if (rootvalue!="DataSet")
	 {
		 myDocument.DeleteNode(pRootEle);
		 pRootEle = myDocument.NewElement("DataSet");
		 myDocument.LinkEndChild(pRootEle);
	 }

	 //元素节点
	  tinyxml2::XMLElement *pdata= pRootEle->FirstChildElement("Data");  
	  while(pdata){
		  string attr=pdata->Attribute("File");
#if _MSC_VER&&Use_CString
        if (attr==ConvertCS2string(filename)){
			 break;			
		 }
#endif
#if linux||__linux||__linux__
		break;
#endif

		  pdata=pdata->NextSiblingElement();  
	  }
	  if (pdata==NULL){
		  //原本没有添加
		  pdata = myDocument.NewElement("Data");
#if _MSC_VER&&Use_CString
    pdata->SetAttribute("File",ConvertCS2string(filename).c_str());
#endif
		

		  pRootEle->LinkEndChild(pdata); 
		  XMLElement *p_elt=myDocument.NewElement("X_pos");
		  pdata->LinkEndChild(p_elt);

				p_elt=myDocument.NewElement("Y_pos");
		  pdata->LinkEndChild(p_elt);

				p_elt=myDocument.NewElement("Dst_Dir");
		  pdata->LinkEndChild(p_elt);

				 p_elt=myDocument.NewElement("YunTaiAngle");
		  pdata->LinkEndChild(p_elt);

				p_elt=myDocument.NewElement("Dst_Laser");
		  pdata->LinkEndChild(p_elt);

		  p_elt=myDocument.NewElement("Head_Angle_H");
		  pdata->LinkEndChild(p_elt);

		  p_elt=myDocument.NewElement("Head_Angule_V");
		  pdata->LinkEndChild(p_elt);


	  }
	  
	  {
		  //原本就有修改
		  char textbuffer[1024];
		  XMLElement *p_elt=pdata->FirstChildElement("X_pos");
		  p_elt->SetText("");
		   sprintf(textbuffer,"%f",point.Xscale);
		  tinyxml2::XMLText *content = myDocument.NewText(textbuffer);  
		  p_elt->LinkEndChild(content); 

		  p_elt=pdata->FirstChildElement("Y_pos");
		   p_elt->SetText("");
		   sprintf(textbuffer,"%f",point.Yscale);
		 content = myDocument.NewText( textbuffer);  
		  p_elt->LinkEndChild(content);

		  p_elt=pdata->FirstChildElement("Dst_Dir");
		  p_elt->SetText("");
		   sprintf(textbuffer,"%f",point.dst_direct);
		  content = myDocument.NewText( textbuffer);  
		  p_elt->LinkEndChild(content);

		  p_elt=pdata->FirstChildElement("YunTaiAngle");
		   p_elt->SetText("");
		   sprintf(textbuffer,"%f",point.YunTaiAngle);
		 content = myDocument.NewText( textbuffer);  
		  p_elt->LinkEndChild(content);

		  p_elt=pdata->FirstChildElement("Dst_Laser");
		   p_elt->SetText("");
		   sprintf(textbuffer,"%f",point.dst_laser);
		 content = myDocument.NewText( textbuffer);  
		  p_elt->LinkEndChild(content);

		  p_elt=pdata->FirstChildElement("Head_Angle_H");
		  p_elt->SetText("");
		  sprintf(textbuffer,"%f",point.Head_Angle_H);
		  content = myDocument.NewText( textbuffer);  
		  p_elt->LinkEndChild(content);

		  p_elt=pdata->FirstChildElement("Head_Angule_V");
		  p_elt->SetText("");
		  sprintf(textbuffer,"%f",point.Head_Angule_V);
		  content = myDocument.NewText( textbuffer);  
		  p_elt->LinkEndChild(content);


	  }
	 myDocument.SaveFile(xml_save_path.c_str());
	/***************************************/
}
/*---------------------------------------------------*/
/**
*
*
*/
/*---------------------------------------------------*/

/*---------------------------------------------------*/
/**
*
*
*/
/*---------------------------------------------------*/