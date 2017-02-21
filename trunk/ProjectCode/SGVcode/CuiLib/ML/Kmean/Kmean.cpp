#include "stdafx.h"
#include "Kmean.h"
#include "module_all_cui.h"
#define MAX_CLUSTERS (8) 
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
Kmean::Kmean(IplImage *img,ImageMemData* pMD,IplImage* imgmask)
{
	this->pMD=pMD;
	this->ChangetoLab=false;
	if (img->nChannels==3){
		this->img3ch=cvCloneImage(img);
	}

	if (img->nChannels==4){
		this->img3ch=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,3);
		cvCvtColor(img,img3ch,CV_BGRA2BGR);
	}
	this->imgmask=cvCloneImage(imgmask);
	this->mainColor=cvCloneImage(this->img3ch);
//	this->labimg=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,3);
	
//	cvCvtColor(this->img3ch,this->labimg,CV_BGR2Lab);
#if TRUE
cvSmooth(this->img3ch,this->img3ch,CV_GAUSSIAN,3,3,0,0);
#endif	
#if 0
    cvShowImage("bgr",img3ch);
	cvShowImage("lab",labimg);
	cvWaitKey(0);
#endif
	unsigned long int size;  
	lablesImg=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,3);
	cvZero(lablesImg);
////////////////////////////////////////////////////////////	
	if (this->imgmask==nullptr){
		size = img->width*img->height;//取得图片大小  
	}
	if (this->imgmask!=nullptr)	{
		size=this->GetEffectiveColorNumber();
	}
	lablesMat = cvCreateMat (size, 1, CV_32SC1);//32位1通道的矩阵  
	points3ch = cvCreateMat (size, 1, CV_32FC3); //32位3通道的矩阵 
	//到三通道图像的数据  
	  this->FillPointsMat();
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/

Kmean::~Kmean(void)
{
//	cvReleaseImage(&labimg);
	cvReleaseImage(&img3ch);
	cvReleaseImage(&lablesImg);
		
	cvReleaseMat(&lablesMat);
	cvReleaseMat(&points3ch);
	cvReleaseImage(&mainColor);
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
void Kmean::GetImgLables(int NumClusters,int method)
{
	this->NumCluster=NumClusters;
	cvKMeans2 (points3ch,NumClusters, lablesMat,  
		cvTermCriteria (CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 10, 0.1));  
	//拆分为8类聚合，最大迭代次数是10，精度是1.0  
	this->ShowImg();
	this->FillAreaInOrgImg(NumClusters,method);
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
void Kmean::FillPointsMat()
{
	 pointsPos.clear();
	IplImage*img=this->img3ch;
	if (this->imgmask==nullptr)	{
		for (int x=0;x<img3ch->width;x++){
			for (int y=0;y<img3ch->height;y++){
				unsigned long int i=x+y*img3ch->width;
				pointsPos.push_back(cvPoint(x,y));
				points3ch->data.fl[i*3] = (unsigned char)img->imageData[i*3];  
				points3ch->data.fl[i*3 + 1] = (unsigned char)img->imageData[i*3 + 1];  
				points3ch->data.fl[i*3 + 2] = (unsigned char)img->imageData[i*3 + 2];   
			}
		}
	}
	if (this->imgmask!=nullptr){
		ASSERT(this->imgmask->nChannels==4);
		ASSERT(this->imgmask->depth==IPL_DEPTH_8U);
		UINT32 *ImgMaskData=(UINT32 *)this->imgmask->imageData;
		for (int x=0;x<img3ch->width;x++){
			for (int y=0;y<img3ch->height;y++){
				unsigned long int i=x+y*img3ch->width;
				if (ImgMaskData[i]==BlackColorPNG||ImgMaskData[i]==BlackColorRGB){
						pointsPos.push_back(cvPoint(x,y));
						unsigned long PointIndex=pointsPos.size()-1;
						points3ch->data.fl[PointIndex*3] = (unsigned char)img->imageData[i*3];  
						points3ch->data.fl[PointIndex*3 + 1] = (unsigned char)img->imageData[i*3 + 1];  
						points3ch->data.fl[PointIndex*3 + 2] = (unsigned char)img->imageData[i*3 + 2];   
				}else if (ImgMaskData[i]==WhiteColorPNG||ImgMaskData[i]==WhiteColorRGB){
						;
				}else{
					UINT32 Color=ImgMaskData[i];
					ASSERT(FALSE);
				}
				
			}
		}
	}

}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
void Kmean::FillAreaInOrgImg(int NumClusters,int method)
{
	if (this->imgmask==nullptr){
	unsigned long int i; 
	unsigned long int size=img3ch->width*img3ch->height;
#if TRUE
if (method==KMean_Method_ORGColor){
#if TRUE
	CvMat *color = cvCreateMat (NumClusters, 1, CV_32FC3);//8行1列的三通道浮点矩阵  
	CvMat *count = cvCreateMat (NumClusters, 1, CV_32SC1);//8行1列的单通道整数矩阵，用作计数  
	cvSetZero (color);  
	cvSetZero (count);  
	for ( i = 0; i < size; i++)  
	{  
		int idx = lablesMat->data.i[i];  //类别
		int j = ++count->data.i[idx];  //类别对应的颜色
		color->data.fl[idx * 3 ] = color->data.fl[idx * 3 ] * (j - 1) / j + points3ch->data.fl[i * 3 ] / j;  
		color->data.fl[idx * 3 + 1] = color->data.fl[idx * 3 + 1] * (j - 1) / j + points3ch->data.fl[i * 3 + 1] / j;  
		color->data.fl[idx * 3 + 2] = color->data.fl[idx * 3 + 2] * (j - 1) / j + points3ch->data.fl[i * 3 + 2] / j;  
	}  

	//把处理过的数据打回imgA  
	for (i = 0; i < size; i++)  
	{  
		int idx = lablesMat->data.i[i];  
		lablesImg->imageData[i * 3 ] = (char) color->data.fl[idx * 3 ];  
		lablesImg->imageData[i * 3 + 1] = (char) color->data.fl[idx * 3 + 1];  
		lablesImg->imageData[i * 3 + 2] = (char) color->data.fl[idx * 3 + 2];  
	}  
	cvReleaseMat(&color);
	cvReleaseMat(&count);
#endif
	
}else if (method==KMean_Method_BLOCK){
#if TRUE
Scalar colorTab[] =     //因为最多只有5类，所以最多也就给5个颜色
	{
		Scalar(0, 0, 255),
		Scalar(0,255,0),
		Scalar(255,100,100),
		Scalar(255,0,255),
		Scalar(0,255,255),
		Scalar(255,0,0),
		Scalar(0,0,0)
	};
	for (int x=0;x<img3ch->width;x++){
		for (int y=0;y<img3ch->height;y++){

			i=x+y*img3ch->width;
			int clusterIdx  = lablesMat->data.i[i]; ;
			CvPoint point=cvPoint(x,y);
			cvCircle(lablesImg,point, 1, colorTab[clusterIdx], CV_FILLED);
		}
	}
#endif
}else{
	ASSERT(FALSE);
}
#endif
	}

	if (this->imgmask!=nullptr){
		unsigned long int i; 
		unsigned long int size;
		UINT32 *ImgData=(UINT32*)this->imgmask->imageData;
#if (TRUE)
		if (method==KMean_Method_ORGColor){
#if TRUE
			CvMat *color = cvCreateMat (NumClusters, 1, CV_32FC3);//8行1列的三通道浮点矩阵  
			CvMat *count = cvCreateMat (NumClusters, 1, CV_32SC1);//8行1列的单通道整数矩阵，用作计数  
			ColorCategory.clear();
			ColorCategory.resize(NumClusters);
			cvSetZero (color);  
			cvSetZero (count);  
			size=lablesMat->cols*lablesMat->rows;
			for ( i = 0; i < size; i++)  
			{  
				int idx = lablesMat->data.i[i];  //类别
				int j = ++count->data.i[idx];  //类别对应的颜色
				color->data.fl[idx * 3 ] = color->data.fl[idx * 3 ] * (j - 1) / j + points3ch->data.fl[i * 3 ] / j;  
				color->data.fl[idx * 3 + 1] = color->data.fl[idx * 3 + 1] * (j - 1) / j + points3ch->data.fl[i * 3 + 1] / j;  
				color->data.fl[idx * 3 + 2] = color->data.fl[idx * 3 + 2] * (j - 1) / j + points3ch->data.fl[i * 3 + 2] / j;  
				int x=this->pointsPos[i].x;
				int y=this->pointsPos[i].y;
				ASSERT(x>=0&&x<=pMD->l_plane->width);
				ASSERT(y>=0&&y<=pMD->l_plane->height);
				ColorCategory[idx].PixelNum++;
				ColorCategory[idx].L_Color+=cvGetReal2D(pMD->l_plane,y,x);
				ColorCategory[idx].A_Color+=cvGetReal2D(pMD->a_plane,y,x);
				ColorCategory[idx].B_color+=cvGetReal2D(pMD->b_plane,y,x);
				
			}
			//////////////////////////////////////////////////////////////////////////////
			for (int ci=0;ci<NumClusters;ci++){
				ColorCategory[ci].L_Color/=ColorCategory[ci].PixelNum;
				ColorCategory[ci].A_Color/=ColorCategory[ci].PixelNum;
				ColorCategory[ci].B_color/=ColorCategory[ci].PixelNum;
				
			}
			/////////////////////////////////////////////////////////////////////////////////
			//把处理过的数据打回imgA  
			for (i = 0; i < size; i++)  
			{  
				int idx = lablesMat->data.i[i];  
				int x=this->pointsPos[i].x;
				int y=this->pointsPos[i].y;
				int img_i=y*lablesImg->width+x;
				lablesImg->imageData[img_i * 3 ] = (char) color->data.fl[idx * 3 ];  
				lablesImg->imageData[img_i * 3 + 1] = (char) color->data.fl[idx * 3 + 1];  
				lablesImg->imageData[img_i * 3 + 2] = (char) color->data.fl[idx * 3 + 2];  
			}  
			cvReleaseMat(&color);
			cvReleaseMat(&count);
#endif

		}else if (method==KMean_Method_BLOCK){
#if TRUE
			Scalar colorTab[] =     //因为最多只有5类，所以最多也就给5个颜色
			{
				Scalar(0, 0, 255),
				Scalar(0,255,0),
				Scalar(255,100,100),
				Scalar(255,0,255),
				Scalar(0,255,255),
				Scalar(255,0,0),
				Scalar(0,0,0)
			};
			for (int x=0;x<img3ch->width;x++){
				for (int y=0;y<img3ch->height;y++){

					i=x+y*img3ch->width;
					int clusterIdx  = lablesMat->data.i[i]; ;
					CvPoint point=cvPoint(x,y);
					cvCircle(lablesImg,point, 1, colorTab[clusterIdx], CV_FILLED);
				}
			}
#endif
		}else{
			ASSERT(FALSE);
		}
#endif
	}
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
void Kmean::ShowImg(void)
{
#if KM_ShowImage
#if TRUE
	if (ChangetoLab==true){
		cvCvtColor(lablesImg,lablesImg,CV_Lab2BGR);
		cvCvtColor(img3ch,img3ch,CV_Lab2BGR);
		cvCvtColor(mainColor,mainColor,CV_Lab2BGR);
	}
#endif
	cvSaveImage("KMlables.jpg",this->imgmask);
	cvSaveImage("km.jpg",lablesImg);
	cvShowImage("km",lablesImg);
	cvSaveImage("src.jpg",this->img3ch);
	cvShowImage("src",this->img3ch);
	cvSaveImage("mainColor.jpg",this->mainColor);
	cvShowImage("mainColor",this->mainColor);
#if TRUE
	if (ChangetoLab==true){
		cvCvtColor(lablesImg,lablesImg,CV_BGR2Lab);
		cvCvtColor(img3ch,img3ch,CV_BGR2Lab);
		cvCvtColor(mainColor,mainColor,CV_BGR2Lab);
	}
#endif
	cvWaitKey(0);
#endif

}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
void Kmean::ChangeColor2LabModule()
{
	if (this->ChangetoLab==false){
		this->ChangetoLab=true;
		cvCvtColor(this->img3ch,this->img3ch,CV_BGR2Lab);
		cvCvtColor(this->mainColor,this->mainColor,CV_BGR2Lab);
		this->FillPointsMat();
	}
	
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
LabColor Kmean::GetMaxAreaClassifications()
{

	LabColor color;
	this->GetImgLables(3,KMean_Method_ORGColor);
	int max_i=0;
	int max_value=this->ColorCategory[0].PixelNum;
	for (int i=0;i<this->NumCluster;i++){
		if (this->ColorCategory[i].PixelNum>max_value){
			max_i=i;
			max_value=this->ColorCategory[i].PixelNum;
		}
	}
	color=this->ColorCategory[max_i];
#if TRUE
	CvScalar s;
	for (int x=0;x<mainColor->width;x++){
		for (int y=0;y<mainColor->height;y++){
			s=cvGet2D(mainColor,y,x); // get the (i,j) pixel value
			printf("B=%f, G=%f, R=%f ",s.val[0],s.val[1],s.val[2]);
			s.val[0]=color.L_Color;
			s.val[1]=color.A_Color;
			s.val[2]=color.B_color;
			cvSet2D(mainColor,y,x,s);//set the (i,j) pixel value
		}
	}
#endif
	this->ShowImg();
	return  color;
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
unsigned long Kmean::GetEffectiveColorNumber(){
	unsigned  long  BlackCount=0;
	unsigned  long  WhiteCount=0;
	UINT32* ImgMaskData=(UINT32*)this->imgmask->imageData;
	for (int x=0;x<img3ch->width;x++){
		for (int y=0;y<img3ch->height;y++){
			unsigned long int i=x+y*img3ch->width;
			if (ImgMaskData[i]==BlackColorPNG||ImgMaskData[i]==BlackColorRGB){
				BlackCount++;
			}else if (ImgMaskData[i]==WhiteColorPNG||ImgMaskData[i]==WhiteColorRGB){
				WhiteCount++;
			}else{
				UINT32 Color=ImgMaskData[i];
				ASSERT(FALSE);
			}

		}
	}
	return BlackCount;
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/

