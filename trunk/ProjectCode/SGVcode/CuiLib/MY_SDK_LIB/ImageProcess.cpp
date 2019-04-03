#include "StdAfx.h"
#include "ImageProcess.h"
#include "Base.h"


#ifdef min
#undef min
#endif

#define HOUGH_USE_CANNY  FALSE

#define ANGLE_UP (-10)
#define ANGLE_DOWN (-54)
#define ANGLE_DELTA (0)
#define ANGLE_HIST_NUM (11)



int ImageProcess::CRACK_MAX_SIZE = 1920;
int ImageProcess::CRACK_MAX_WIDTH = 1080;

ImageProcess::ImageProcess(void)
{

}


ImageProcess::~ImageProcess(void)
{

}


void ImageProcess::findTestArea(void)
{
	
}

float ImageProcess::findAvgFloat(float* arr, int N)
{
	//float avg=0;
	float sum_t=0;
	
	for (int i = 1; i < N; i++){
		sum_t+=arr[i];
	}
	
	return (sum_t/N);

}
int ImageProcess::findMaxFloatIdx(float* arr, int N)
{
	float max = arr[0];
	int   max_idx=0;
	for (int i = 1; i < N; i++)
		if (max < arr[i]){
			max = arr[i];
			max_idx=i;
		}


		return max_idx;
}

void ImageProcess::adjustFloatMatrix2_100000(int height,float* matrix)
{
	float sum_t=0;

	//float sum_max=height*255;

	for (int i=0;i<height;i++){
		sum_t+=matrix[i];
	}

	for (int i=0;i<height;i++){		
		matrix[i]=10000*matrix[i]/sum_t;	
	}

}

CvRect ImageProcess::findTestArea(IplImage *image_gray,std::string readfile_name_t)
{
	assert(image_gray!=NULL);
	assert(image_gray->nChannels==1);

	//处理单色灰度图像

	IplImage *image_temp=cvCreateImage(cvGetSize(image_gray),IPL_DEPTH_8U,1);
	float * image_Y_sum=new float[image_temp->height];
	CvRect rect_cut_t; 

	memset(image_Y_sum,0,sizeof(float)*image_temp->height);
	{
#if 0
		cvSmooth(image_gray,image_temp);
		cvSaveImage(Base::comine_str(readfile_name_t,"test_out_smooth.jpg").c_str(),image_temp);
#else
		cvCopy(image_gray,image_temp);
#endif
		
		
#if 1
		uchar* data=(uchar *)image_temp->imageData;  
		int step = image_temp->widthStep/sizeof(uchar);  

		for(int i=0;i<image_temp->height;i++)  
			for(int j=0;j<image_temp->width;j++)  
				image_Y_sum[i]+=data[i*step+j];  //在高度方向累加
#endif		
		adjustFloatMatrix2_100000(image_temp->height,image_Y_sum);

#if _DEBUG
		{
			char data_t[1024];
			std::ofstream outfile;
			outfile.open(Base::comine_str(readfile_name_t,"Matrix_Y_SUM.txt").c_str(),std::ios::out);
			for(register int i = 0; i <image_temp->height; i++ ){

				float value_t=image_Y_sum[i];
				sprintf(data_t," %f ",value_t);
				outfile<<i;
				outfile<<' ';
				outfile<<data_t;
				outfile<<std::endl;
			} 
			outfile.close();
		}
#endif
		int max_idx=findMaxFloatIdx(image_Y_sum,image_temp->height);
		const float avg_t=findAvgFloat(image_Y_sum,image_temp->height);

		int top_idx=max_idx-1;
		int bottom_idx=max_idx+1;

		while(
			((image_Y_sum[top_idx]<=image_Y_sum[top_idx+1])||(image_Y_sum[top_idx-1]<=image_Y_sum[top_idx+1]))||
			(image_Y_sum[top_idx]>avg_t*1.5)
			){			

			if (top_idx--<10)
			{
				break;
			}
			if (avg_t>image_Y_sum[top_idx]){
				break;
			}

		}

		while(
			((image_Y_sum[bottom_idx]<=image_Y_sum[bottom_idx-1])||(image_Y_sum[bottom_idx+1]<=image_Y_sum[bottom_idx-1]))||
			(image_Y_sum[bottom_idx]>avg_t*1.5)
			){

			if (bottom_idx++>image_gray->height-10)
			{
				break;
			}
			if (avg_t>image_Y_sum[bottom_idx]){
				break;
			}


		}

		int delta=std::min(max_idx-top_idx,bottom_idx-top_idx);

#if _DEBUG&&0
		cvDrawRect(image_gray,cvPoint(0,max_idx-delta),cvPoint(image_gray->width,max_idx+delta),cvScalar(255,255,255));
		cvDrawLine(image_gray,cvPoint(0,max_idx),cvPoint(image_gray->width,max_idx),cvScalar(0,0,0));
		cvSaveImage(Base::comine_str(readfile_name_t,"test_out_org.jpg").c_str(),image_gray);
#endif
		rect_cut_t.x=0;
		rect_cut_t.y=max_idx-delta;
		rect_cut_t.width=image_gray->width;
		rect_cut_t.height=delta*2;
		
	}
	delete[] image_Y_sum;
	cvReleaseImage(&image_temp);

	return rect_cut_t;
}
/*--------------------------------------------------------------*/
/**
*
*
*/
/*--------------------------------------------------------------*/
void ImageProcess::SaveCutImage(IplImage* img_t,CvRect cut_t,std::string readfile_name_t)
{
	cvSetImageROI(img_t , cut_t);

	IplImage* img_cut_t=cvCreateImage(cvSize(cut_t.width,cut_t.height),IPL_DEPTH_8U,img_t->nChannels);
	
	cvCopy(img_t,img_cut_t,0);
	{
#if _DEBUG
		cvSaveImage(Base::comine_str(readfile_name_t,"_cut_image.jpg").c_str(),img_cut_t);
#endif
	}
	cvReleaseImage(&img_cut_t);
	
	cvResetImageROI(img_t);
	
}
/*--------------------------------------------------------------*/
/**
*
*
*/
/*--------------------------------------------------------------*/
void ImageProcess::FilterByMeanStdDev(IplImage* image_org_rgb,char *show_win)
{
	IplImage* Igray=cvCreateImage(cvGetSize(image_org_rgb),IPL_DEPTH_8U,1);
	IplImage* Ibinary=cvCreateImage(cvGetSize(image_org_rgb),IPL_DEPTH_8U,1);
#if 0
	IplImage  *Ibinary_old=cvCreateImage(cvGetSize(image_org_rgb),IPL_DEPTH_8U,1);
	IplImage  *Ibinary_sub=cvCreateImage(cvGetSize(image_org_rgb),IPL_DEPTH_8U,1);
#endif	  


	int width_t=image_org_rgb->width;
	int height_t=image_org_rgb->height;
	cvSetZero(Ibinary);
	cvCvtColor(image_org_rgb,Igray,CV_RGB2GRAY);
	
	//cvSmooth(Igray,Igray,CV_GAUSSIAN,3,3);
	
	{
		CvScalar mean;
		CvScalar std_dev;

		for (int yi=0;yi<image_org_rgb->height;yi++){
			
			CvRect rect_t=cvRect(0,yi,width_t,1);

			cvSetImageROI(Igray,rect_t);

			cvAvgSdv(Igray,&mean,&std_dev);

			//正态分布有两个参数，即期望（均数）μ和标准差σ，σ^2为方差。

			double     mean_t = mean.val[0];  //均值
			double     stddev_t = std_dev.val[0]; //标准差

			double sgm=3;

			double down_t=mean_t-sgm*stddev_t;
			double up_t=mean_t+sgm*stddev_t;			
			
			for(int xi=0;xi<width_t;xi++){
				double value_t=cvGetReal2D(Igray,0,xi);//ROI
				if (value_t<down_t||value_t>up_t){
					cvSet2D(Ibinary,yi,xi,CV_RGB(255,255,255));
				}

			}
#if 0
			printf("Mean : %f  , Stddev : %f \n",mean_t,std_dev);
#endif			

			cvResetImageROI(Igray);

		}

	}

#if _DEBUG&&0

	double sum_t=0;
	for (int i=0;i<image_org_rgb->height;i++){
		
		sum_t=0;
		for(int j=0;j<image_org_rgb->width;j++){

			sum_t+=cvGetReal2D(Igray,i,j);

		}

		double avg_t=sum_t/image_org_rgb->width;
		printf("Avg : %f  \n",avg_t);

	}

#endif

#if _DEBUG
	cvShowImage("gray",Igray);
	cvShowImage("binary",Ibinary);
	cvShowImage("org",image_org_rgb);
#endif
	cvReleaseImage(&Igray);
	cvReleaseImage(&Ibinary);

}
/*--------------------------------------------------------------*/
/**
*
*
*/
/*--------------------------------------------------------------*/
IplImage* ImageProcess::FilterByMeanStdDev(IplImage* image_org_rgb)
{
	IplImage* Igray=cvCreateImage(cvGetSize(image_org_rgb),IPL_DEPTH_8U,1);	
	IplImage* Ibinary=cvCreateImage(cvGetSize(image_org_rgb),IPL_DEPTH_8U,1);

	int width_t=image_org_rgb->width;
	int height_t=image_org_rgb->height;
	cvSetZero(Ibinary);
	cvCvtColor(image_org_rgb,Igray,CV_RGB2GRAY);
	cvShowImage("gray",Igray);

#if 1
	cvSmooth(Igray,Igray,CV_MEDIAN,3,3);
#endif


	{
		CvScalar mean;
		CvScalar std_dev;

		for (int yi=0;yi<image_org_rgb->height;yi++){

			CvRect rect_t=cvRect(0,yi,width_t,1);
			cvSetImageROI(Igray,rect_t);
			cvAvgSdv(Igray,&mean,&std_dev);
			//正态分布有两个参数，即期望（均数）μ和标准差σ，σ^2为方差。
			double     mean_t = mean.val[0];  //均值
			double     stddev_t = std_dev.val[0]; //标准差
			double sgm=3;
			double down_t=mean_t-sgm*stddev_t;
			double up_t=mean_t+sgm*stddev_t;			

			for(int xi=0;xi<width_t;xi++){
				double value_t=cvGetReal2D(Igray,0,xi);//ROI
				if (value_t<down_t||value_t>up_t){
					cvSet2D(Ibinary,yi,xi,CV_RGB(255,255,255));
				}

			}
#if 0
			printf("Mean : %f  , Stddev : %f \n",mean_t,std_dev);
#endif
			cvResetImageROI(Igray);

		}

	}

	cvReleaseImage(&Igray);
	
	return Ibinary;
}
/*--------------------------------------------------------------*/
/**
*
*
*/
/*--------------------------------------------------------------*/
CV_IMPL CvSeq*
ImageProcess::cvHoughCircles_Binary( CvArr* src_image, void* circle_storage,
                int method, double dp, double min_dist,
                double param1, double param2,
                int min_radius, int max_radius )
{
    CvSeq* result = 0;

    CvMat stub, *img = (CvMat*)src_image;
    CvMat* mat = 0;
    CvSeq* circles = 0;
    CvSeq circles_header;
    CvSeqBlock circles_block;
    int circles_max = INT_MAX;    //输出最多圆形的数量，设为无穷多
   
#if 1
	//canny边缘检测中双阈值中的高阈值
    int canny_threshold = cvRound(param1);
#endif
    //累加器阈值
    int acc_threshold = cvRound(param2);

    img = cvGetMat( img, &stub );
    //确保输入图像是灰度图像
    if( !CV_IS_MASK_ARR(img))
        CV_Error( CV_StsBadArg, "The source image must be 8-bit, single-channel" );
    //内存空间是否存在
    if( !circle_storage )
        CV_Error( CV_StsNullPtr, "NULL destination" );
    //确保参数的正确性
#if 1
    if( dp <= 0 || min_dist <= 0 || canny_threshold <= 0 || acc_threshold <= 0 )
        CV_Error( CV_StsOutOfRange, "dp, min_dist, canny_threshold and acc_threshold must be all positive numbers" );
#endif
    //圆的最小半径要大于0
    min_radius = MAX( min_radius, 0 );
    //圆的最大半径如果小于0，则设最大半径为图像宽和长度的最大值，
    //如果最大半径小于最小半径，则设最大半径为最小半径加两个像素的宽度
    if( max_radius <= 0 )
        max_radius = MAX( img->rows, img->cols );
    else if( max_radius <= min_radius )
        max_radius = min_radius + 2;

    if( CV_IS_STORAGE( circle_storage ))
    {
        circles = cvCreateSeq( CV_32FC3, sizeof(CvSeq),
            sizeof(float)*3, (CvMemStorage*)circle_storage );
    }
    else if( CV_IS_MAT( circle_storage ))
    {
        mat = (CvMat*)circle_storage;

        if( !CV_IS_MAT_CONT( mat->type ) || (mat->rows != 1 && mat->cols != 1) ||
            CV_MAT_TYPE(mat->type) != CV_32FC3 )
            CV_Error( CV_StsBadArg,
            "The destination matrix should be continuous and have a single row or a single column" );

        circles = cvMakeSeqHeaderForArray( CV_32FC3, sizeof(CvSeq), sizeof(float)*3,
                mat->data.ptr, mat->rows + mat->cols - 1, &circles_header, &circles_block );
        circles_max = circles->total;
        cvClearSeq( circles );
    }
    else
        CV_Error( CV_StsBadArg, "Destination is not CvMemStorage* nor CvMat*" );
    //选择哪种算法检测圆，目前只有2-1霍夫变换
    switch( method )
    {
    case CV_HOUGH_GRADIENT:
        //调用icvHoughCirclesGradient函数
        icvHoughCirclesGradient( img, (float)dp, (float)min_dist,
                                min_radius, max_radius, canny_threshold,
                                acc_threshold, circles, circles_max );
          break;
    default:
        CV_Error( CV_StsBadArg, "Unrecognized method id" );
    }

    if( mat )
    {
        if( mat->cols > mat->rows )
            mat->cols = circles->total;
        else
            mat->rows = circles->total;
    }
    else
        result = circles;
    //输出圆
    return result;
}

#define hough_cmp_gt(l1,l2) (aux[l1] > aux[l2])

static CV_IMPLEMENT_QSORT_EX( icvHoughSortDescent32s, int, hough_cmp_gt, const int* )

void ImageProcess::icvHoughCirclesGradient( CvMat* img, float dp, float min_dist,
                         int min_radius, int max_radius,
                         int canny_threshold, int acc_threshold,
                         CvSeq* circles, int circles_max )
{
//参数：
//img: 输入图像
//dp: 识别精度,1.0表示按照原图精度
//min_dist: 圆心点位置识别精度
//min_radius: 所需要找的圆的最小半径
//max_radius：所需要找的圆的最大半径
//canny_threshold：canny算子的高阀值
//acc_threshold：累加器阀值，计数大于改阀值的点即被认为是可能的圆心
//circles: 保存找到的符合条件的所有圆
//circles_max: 最多需要的找到的圆的个数

	const int SHIFT = 10, ONE = 1 << SHIFT, R_THRESH = 30;
	cv::Ptr<CvMat> dx, dy;
	cv::Ptr<CvMat> edges, accum, dist_buf;
	std::vector<int> sort_buf;
	cv::Ptr<CvMemStorage> storage;
	
	int x, y, i, j, k, center_count, nz_count;
	float min_radius2 = (float)min_radius*min_radius;
	float max_radius2 = (float)max_radius*max_radius;
	int rows, cols, arows, acols;
	int astep, *adata;
	float* ddata;
	CvSeq *nz, *centers;
	float idp, dr;
	CvSeqReader reader;
	
	//canny算子求单像素二值化边缘，保存在edges变量中
	edges = cvCreateMat( img->rows, img->cols, CV_8UC1 );
	
#if HOUGH_USE_CANNY
	cvCanny( img, edges, MAX(canny_threshold/2,1), canny_threshold, 3 );
#else
	cvCopy(img,edges);
#endif

#if	0
	cvNamedWindow( "hough edges", 0 );
	cvShowImage("hough edges",edges);
	cvWaitKey(10);
#endif

#if _DEBUG
	cvSaveImage("hough_edges.png",edges);
#endif

	//sobel算子求水平和垂直方向的边缘，用于计算边缘点的法线方向
	dx = cvCreateMat( img->rows, img->cols, CV_16SC1 );
	dy = cvCreateMat( img->rows, img->cols, CV_16SC1 );
	cvSobel( img, dx, 1, 0, 3 );
	cvSobel( img, dy, 0, 1, 3 );
	
	//dp表示识别精度
	if( dp < 1.f )
		dp = 1.f;
	idp = 1.f/dp;
	
	//accum用作累加器，包含图像中每一个点的计数。图像中每一个点都有一个计数，点的计数表示每一个canny边缘点法线方向上，
	//到该点距离为R的边缘点的个数，初始化为0
	accum = cvCreateMat( cvCeil(img->rows*idp)+2, cvCeil(img->cols*idp)+2, CV_32SC1 );
	cvZero(accum);
	
	storage = cvCreateMemStorage();
	nz = cvCreateSeq( CV_32SC2, sizeof(CvSeq), sizeof(CvPoint), storage );
	
	//centers用于保存可能的圆心点
	centers = cvCreateSeq( CV_32SC1, sizeof(CvSeq), sizeof(int), storage );
	
	rows = img->rows;
	cols = img->cols;
	arows = accum->rows - 2;
	acols = accum->cols - 2;
	adata = accum->data.i;
	astep = accum->step/sizeof(adata[0]);
	
	//以下这个循环用于获取所有可能的圆边缘点，存储在nz中，同时设置
	//累加器中的值
	for( y = 0; y < rows; y++ )
	{
		const uchar* edges_row = edges->data.ptr + y*edges->step;
		const short* dx_row = (const short*)(dx->data.ptr + y*dx->step);
		const short* dy_row = (const short*)(dy->data.ptr + y*dy->step);
		
		for( x = 0; x < cols; x++ )
		{
			float vx, vy;
			int sx, sy, x0, y0, x1, y1, r, k;
			CvPoint pt;
	
			vx = dx_row[x];
			vy = dy_row[x];
	
			if( !edges_row[x] || (vx == 0 && vy == 0) )
				continue;
	
			float mag = sqrt(vx*vx+vy*vy);
			assert( mag >= 1 );
			
			//sx表示cos, sy表示sin
			sx = cvRound((vx*idp)*ONE/mag);
			sy = cvRound((vy*idp)*ONE/mag);
			
			x0 = cvRound((x*idp)*ONE);
			y0 = cvRound((y*idp)*ONE);
			
			//循环两次表示需要计算两个方向，法线方向和法线的反方向
			for( k = 0; k < 2; k++ )
			{
				//半径方向的水平增量和垂直增量
				x1 = x0 + min_radius * sx;
				y1 = y0 + min_radius * sy;
				
				//在法线方向和反方向上，距离边缘点的距离为输入的最大半径和最小半径范围内找点
				//每找到一个点，该点的累加器计数就加1
				for( r = min_radius; r <= max_radius; x1 += sx, y1 += sy, r++ )
				{
					int x2 = x1 >> SHIFT, y2 = y1 >> SHIFT;
					if( (unsigned)x2 >= (unsigned)acols || (unsigned)y2 >= (unsigned)arows )
						break;
					adata[y2*astep + x2]++;
				}
				//反方向
				sx = -sx; sy = -sy;
			}
			
			//保存可能的圆边缘点
			pt.x = x; pt.y = y;
			cvSeqPush( nz, &pt );
		}
	}

	nz_count = nz->total;
	if( !nz_count )
		return;
	
	//累加器中，计数大于阀值的点，被认为可能的圆心点。因为计算各点计数过程中，距离有误差，所以
	//在与阀值比较时，该点计数先要与4邻域内的各个点的计数比较，最大者才能和阀值比较。可能的圆心
	//点保存在centers中
	for( y = 1; y < arows - 1; y++ )
	{
		for( x = 1; x < acols - 1; x++ )
		{
			int base = y*(acols+2) + x;
			if( adata[base] > acc_threshold &&
			    adata[base] > adata[base-1] && adata[base] > adata[base+1] &&
			    adata[base] > adata[base-acols-2] && adata[base] > adata[base+acols+2] )
			cvSeqPush(centers, &base);
		}
	}

	center_count = centers->total;
	if( !center_count )
		return;

	sort_buf.resize( MAX(center_count,nz_count) );
	
	//链表结构的certers转化成连续存储结构sort_buf
	cvCvtSeqToArray( centers, &sort_buf[0] );
	
	//经过icvHoughSortDescent32s函数后，以sort_buf中元素作为adata数组下标, 
	//adata中的元素降序排列, 即adata[sort_buf[0]]是adata所有元素中最大的, 
	//adata[sort_buf[center_count-1]]是所有元素中最小的
	icvHoughSortDescent32s( &sort_buf[0], center_count, adata );
	
	cvClearSeq( centers );
	
	//经过排序后的元素，重新以链表形式存储到centers中
	cvSeqPushMulti( centers, &sort_buf[0], center_count );
	
	dist_buf = cvCreateMat( 1, nz_count, CV_32FC1 );
	ddata = dist_buf->data.fl;
	
	dr = dp;
	min_dist = MAX( min_dist, dp );
	min_dist *= min_dist;
	
	//对于每一个可能的圆心点，计算所有边缘点到该圆心点的距离。由于centers中的
	//元素已经经过前面排序，所以累加器计数最大的可能圆心点最先进行下面的操作
	for( i = 0; i < centers->total; i++ )
	{
		int ofs = *(int*)cvGetSeqElem( centers, i );
		y = ofs/(acols+2) - 1;
		x = ofs - (y+1)*(acols+2) - 1;
		float cx = (float)(x*dp), cy = (float)(y*dp);
		float start_dist, dist_sum;
		float r_best = 0, c[3];
		int max_count = R_THRESH;
		
		//如果该可能的圆心点和已经确认的圆心点的距离小于阀值，则表示
		//这个圆心点和已经确认的圆心点是同一个点
		for( j = 0; j < circles->total; j++ )
		{
			float* c = (float*)cvGetSeqElem( circles, j );
			if( (c[0] - cx)*(c[0] - cx) + (c[1] - cy)*(c[1] - cy) < min_dist )
				break;
		}

		if( j < circles->total )
			continue;
		
		cvStartReadSeq( nz, &reader );
		
		//求所有边缘点到当前圆心点的距离，符合条件的距离值保存在ddata中
		for( j = k = 0; j < nz_count; j++ )
		{
			CvPoint pt;
			float _dx, _dy, _r2;
			CV_READ_SEQ_ELEM( pt, reader );
			_dx = cx - pt.x; _dy = cy - pt.y;
			_r2 = _dx*_dx + _dy*_dy;
			if(min_radius2 <= _r2 && _r2 <= max_radius2 )
			{
				ddata[k] = _r2;
				sort_buf[k] = k;
				k++;
			}
		}

		int nz_count1 = k, start_idx = nz_count1 - 1;
		if( nz_count1 == 0 )
			continue;
		dist_buf->cols = nz_count1;
		cvPow( dist_buf, dist_buf, 0.5 );
		
		//经过如下处理后，以sort_buf中元素作为ddata数组下标,ddata中的元素降序排列, 
		//即ddata[sort_buf[0]]是ddata所有元素中最大的, ddata[sort_buf[nz_count1-1]]
		//是所有元素中最小的
		icvHoughSortDescent32s( &sort_buf[0], nz_count1, (int*)ddata );
		
		//对所有的距离值做处理，求出最可能圆半径值，max_count为到圆心的距离为最可能半径值的点的个数
		dist_sum = start_dist = ddata[sort_buf[nz_count1-1]];
		for( j = nz_count1 - 2; j >= 0; j-- )
		{
			float d = ddata[sort_buf[j]];		
			if( d > max_radius )
				break;
			
			if( d - start_dist > dr )
			{
				float r_cur = ddata[sort_buf[(j + start_idx)/2]];
				if( (start_idx - j)*r_best >= max_count*r_cur ||
					(r_best < FLT_EPSILON && start_idx - j >= max_count) ) 
				{
					r_best = r_cur;
					max_count = start_idx - j;
				}
				start_dist = d;
				start_idx = j;
				dist_sum = 0;
			}
			dist_sum += d;
		}
		//max_count大于阀值，表示这几个边缘点构成一个圆
		if( max_count > R_THRESH )
		{
			c[0] = cx;
			c[1] = cy;
			c[2] = (float)r_best;
			cvSeqPush( circles, c );
			if( circles->total > circles_max )
			return;
		}
	}
}
/*--------------------------------------------------------------*/
/**
*
*
*/
/*--------------------------------------------------------------*/
void ImageProcess::zhangjiagang_hongbao_duanzao_rgb(std::string filename)
{
	
	
	IplImage* src_color = cvLoadImage(filename.c_str());
	IplImage* src_gary=cvCreateImage(cvGetSize(src_color),src_color->depth,1);  
	IplImage* src_binary=cvCreateImage(cvGetSize(src_color),src_color->depth,1);

	cvCvtColor(src_color,src_gary, CV_BGR2GRAY);

	cvThreshold(src_gary,src_binary,100,255,CV_THRESH_BINARY);

	wait_for_show_image("src_color",src_color);
	wait_for_show_image("src_gary",src_gary);
	wait_for_show_image("src_binary ",src_binary); 
	
	cvReleaseImage(&src_color);
	cvReleaseImage(&src_gary);
	cvReleaseImage(&src_binary);


}
/*--------------------------------------------------------------*/
/**
*
*
*/
/*--------------------------------------------------------------*/
void  ImageProcess::wait_for_show_image(std::string window_name,IplImage* img_t)
{
#if _DEBUG

	bool IsShow=false; 

	if(IsShow){
		cvNamedWindow( window_name.c_str(), 1 );
		cvShowImage(window_name.c_str(), img_t );
		cvWaitKey(10);
		cvWaitKey(0);
	}

	std::string file_name_t="done//"+window_name+".png";
	cvSaveImage(file_name_t.c_str(),img_t);

#endif
}
/*--------------------------------------------------------------*/
/**
*
*
*/
/*--------------------------------------------------------------*/
void ImageProcess::zhangjiagang_hongbao_duanzao(std::string filename)
{
	IplImage* src_color = cvLoadImage(filename.c_str());
#if _DEBUG
	wait_for_show_image("src_color",src_color);
#endif
	IplImage* src_gary=cvCreateImage(cvGetSize(src_color),src_color->depth,1); 
	cvCvtColor(src_color,src_gary, CV_BGR2GRAY);
#if _DEBUG
	wait_for_show_image("src_gary ",src_gary ); 
#endif
	IplImage* src_binary=use_lab2binary(src_color);
#if _DEBUG
	//wait_for_show_image("src_binary ",src_binary); 
#endif
	/*------------------------*/
	CvMemStorage* memory_g=cvCreateMemStorage(0);
	CvSeq* seq=find_max_contour_adjust_binary(memory_g,src_binary);
	{
		CvRect mask_rect = cvBoundingRect(seq,1);
		IplImage* src_color_cut=cvCreateImage(cvSize(mask_rect.width,mask_rect.height),src_color->depth,3);
			cvSetZero(src_color_cut);
		IplImage* src_gary_cut=cvCreateImage(cvSize(mask_rect.width,mask_rect.height),src_color->depth,1);
			cvSetZero(src_gary_cut);
		IplImage* src_binary_cut=cvCreateImage(cvSize(mask_rect.width,mask_rect.height),src_color->depth,1);
		{
			cvSetImageROI(src_binary, mask_rect);
			cvSetImageROI(src_gary, mask_rect);
			cvSetImageROI(src_color, mask_rect);

			cvCopy(src_binary,src_binary_cut);

			if(1){
				cvCopy(src_gary,src_gary_cut,src_binary);
				cvCopy(src_color,src_color_cut,src_binary);
			}

#if _DEBUG
wait_for_show_image("src_binary_cut",src_binary_cut); 
wait_for_show_image("src_gary_cut ",src_gary_cut); 
#endif
		cv::Point3f circle1,circle0;
	if(1){

		process_max_min_rect(memory_g,src_color_cut,src_gary_cut,seq,src_binary_cut,circle1,circle0);
		//draw_duan_jian_result(src_color,seq,circle1,circle0,src_binary);
	}


	/*if(0){
			gary_by_angle(src_color_cut,src_gary_cut,src_binary_cut);	
			IplImage*  canny_result=canny_by_mask(src_color_cut,src_gary_cut,src_binary_cut);
			CvRect cut_1_rect;
			CvRect cut_0_rect;
			IplImage* cut_1=cut_image(canny_result,1,cut_1_rect,NULL);
			IplImage* cut_0=cut_image(canny_result,0,cut_0_rect,NULL);
#if _DEBUG
	wait_for_show_image("cut_1",cut_1);
	wait_for_show_image("cut_0",cut_0);
#endif
Point3f circle1=hough_my_fast(cut_1,
			1,
			20,
			65,
			85,
			cut_1->width);
Point3f circle0=hough_my_fast(cut_0,
			0,
			10,
			60,
			80,
			cut_0->width);
			circle1.x+=cut_1_rect.x;
			circle1.y+=cut_1_rect.y;
			circle0.x+=cut_0_rect.x;
			circle0.y+=cut_0_rect.y;

			
	
	}*/
			cvResetImageROI(src_color);
			cvResetImageROI(src_binary);
			cvResetImageROI(src_gary);
			draw_duan_jian_result(src_color,seq,circle1,circle0,src_binary);
		}
		cvReleaseImage(&src_gary_cut);
		cvReleaseImage(&src_color_cut);
		cvReleaseImage(&src_binary_cut);
		cvReleaseImage(&src_binary_cut);

	}
	/*-----------------------*/
	//cvReleaseImage(&src_gary_mask);
	cvReleaseImage(&src_gary);
	cvReleaseImage(&src_color);
	cvReleaseImage(&src_binary);
	cvReleaseMemStorage(&memory_g);
}
/*--------------------------------------------------------------*/
/**
*
*
*/
/*--------------------------------------------------------------*/
void ImageProcess::draw_duan_jian_result(IplImage* src_color,CvSeq* seq,cv::Point3f circle1,cv::Point3f circle0,IplImage* mask_img)
{
#if _DEBUG
	CvRect mask_rect = cvBoundingRect(seq,1);
	circle1.x=mask_rect.x+circle1.x;
	circle0.x=mask_rect.x+circle0.x;
	//
	circle1.y=mask_rect.y+circle1.y;
	circle0.y=mask_rect.y+circle0.y;
	//
	int lineThickness = 2;
    int lineType = 8;
    int shift = 0;
    int xCoord1 = circle1.x;
    int yCoord1 = circle1.y;
    int radius1 = circle1.z;
    cv::Point2f center1(xCoord1, yCoord1); 
	 
	int xCoord0 = circle0.x;
    int yCoord0 = circle0.y;
    int radius0 = circle0.z;
    cv::Point2f center0(xCoord0, yCoord0); 

    cvCircle(src_color, center1, radius1-2, cv::Scalar(0,255,255), lineThickness, lineType, shift);//黄色
	cvCircle(src_color, center0, radius0-2, cv::Scalar(0,255,255), lineThickness, lineType, shift);

	//cvLine(src_color,center1,center0,Scalar(0),lineThickness);

	double k=1.0*(circle1.y-circle0.y)/(circle1.x-circle0.x);
	double b=circle1.y-k*circle1.x;
	
	double k1k0=-1/k;
	double b1=circle1.y-k1k0*circle1.x;
	double b0=circle0.y-k1k0*circle0.x;

	draw_dash_line(src_color,k1k0,b1,mask_img);
	draw_dash_line(src_color,k1k0,b0,mask_img);
	draw_dash_line(src_color,k,b,mask_img);
	

#endif
#if _DEBUG
	 cvDrawContours(src_color, seq, CV_RGB(0,0,255), CV_RGB(0,0,255), 0, 1, 8, cvPoint(0,0));//蓝色
#endif
#if _DEBUG
	 wait_for_show_image("done",src_color);
#endif
}
/*--------------------------------------------------------------*/
/**
*
*
*/
/*--------------------------------------------------------------*/
void ImageProcess::draw_dash_line(IplImage* src_color,double k,double b,IplImage* mask_img)
{
	int lineThickness=0;
	
	CvRect mask_rect;
	mask_rect.x=0;
	mask_rect.y=0;
	mask_rect.width=src_color->width;
	mask_rect.height=src_color->height;

	cv::Mat * pmatImage = new cv::Mat(src_color,0);

	CvPoint p1=cvPoint(mask_rect.x,mask_rect.x*k+b);
	CvPoint p2=cvPoint(mask_rect.x+mask_rect.width,(mask_rect.x+mask_rect.width)*k+b);

	//Point p1(20,20);                            // start & end points 
	//Point p2(80,50);
	cv::LineIterator it(src_color, p1, p2);            // get a line iterator
	for(int i = 0; i < it.count; i++,it++){
		const int STEP=30;
		double pos=1.0*(i%STEP)/STEP;
		CvPoint p_t=it.pos();
#if 1
		if(	(p_t.x>mask_rect.x)&&
			(p_t.x<mask_rect.x+mask_rect.width)&&
			(p_t.y>mask_rect.y)&&
			(p_t.y<mask_rect.y+mask_rect.height)){
				if(cvGetReal2D(mask_img,p_t.y,p_t.x)>250){
							if(pos<0.6){
								cvDrawCircle(src_color,p_t,1,CV_RGB(255,0,0),lineThickness);
							}else if(pos>=0.6&&pos<=0.75){		
							}else if(pos>0.75&&pos<0.85){
								cvDrawCircle(src_color,p_t,1,CV_RGB(255,0,0),lineThickness);
							}else if(pos>=1){
							}else{		
							}
				}

	}
#endif
	}
}
void ImageProcess::find_circle(IplImage * src_color_cut, IplImage * src_binary_cut_part)
{
}
IplImage* ImageProcess::use_lab2binary(IplImage* src_color_t)
{
	IplImage* src_lab=cvCreateImage(cvGetSize(src_color_t),src_color_t->depth,3);
	IplImage* src_binary=cvCreateImage(cvGetSize(src_color_t),src_color_t->depth,1);
	
	cvZero(src_binary);	
	/*IplImage* src_l=cvCreateImage(cvGetSize(src_color_t),src_color_t->depth,1);
	IplImage* src_a=cvCreateImage(cvGetSize(src_color_t),src_color_t->depth,1);
	IplImage* src_b=cvCreateImage(cvGetSize(src_color_t),src_color_t->depth,1);*/
	
	cvCvtColor(src_color_t,src_lab,CV_BGR2Lab);
	cvSmooth(src_lab,src_lab,CV_BLUR,5,5,0,0);  
	/*cvSplit(src_lab,src_l,src_a,src_b,0);*/
	CvScalar value;	
	const int Delta=5;
	const int Light=80;		
	for(int xi=0;xi<src_color_t->width;xi++){
		for(int yi=0;yi<src_color_t->height;yi++){			
			value= cvGet2D(src_lab, yi, xi);
			double l=value.val[0];
			double a=value.val[1];
			double b=value.val[2];
		
#if 0
			if(
				((a>127-Delta)&&(a<127+Delta)&&(b<127+Delta)&&(b>127-Delta))
				
				
				){
			 //黑 暗色
				CvScalar s_b;
				s_b.val[0]=0;s_b.val[1]=0;s_b.val[2]=0;
				cvSet2D(src_binary,yi,xi,s_b);
				
			}else{
			//亮彩色
				CvScalar s_w;
				s_w.val[0]=255;s_w.val[1]=255;s_w.val[2]=255;
				cvSet2D(src_binary,yi,xi,s_w);
			}
#else
			double new_a=a-255/2.0;
			double new_b=b-255/2.0;

			if(new_a==new_b){
				//灰度系列
				if(l>Light){
					CvScalar s_w;//彩色
						s_w.val[0]=255;s_w.val[1]=255;s_w.val[2]=255;
						cvSet2D(src_binary,yi,xi,s_w);
				}			
			}else{
				//彩色系列
					double arc=atan2(new_b,new_a)*180/CV_PI;
					if((arc>ANGLE_DOWN-Delta)&&(arc<ANGLE_UP+Delta )&&(l>Light)){						
						CvScalar s_w;//彩色
						s_w.val[0]=255;s_w.val[1]=255;s_w.val[2]=255;
						cvSet2D(src_binary,yi,xi,s_w);
					}						
			}
#endif
		}
	}
#if 0
	wait_for_show_image("src_l ",src_l); 
	wait_for_show_image("src_a ",src_a); 
	wait_for_show_image("src_b ",src_b); 
#endif

 	//wait_for_show_image("src_lab ",src_lab); 

	wait_for_show_image("src_binary",src_binary); 

	cvReleaseImage(&src_lab);

	return src_binary;
}
/*--------------------------------------------------------------*/
/**
*
*
*/
/*--------------------------------------------------------------*/
CvSeq* ImageProcess::find_max_contour_adjust_binary(CvMemStorage* memory,IplImage* src_binary_t)
{
   IplImage* src=cvCloneImage(src_binary_t);
      
   CvSeq* Icontour=NULL;
   CvSeq* maxContour =NULL;
   
   cvFindContours(src,memory,&Icontour, sizeof(CvContour),CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE,cvPoint(0,0));

   double area=0;
   double maxArea=0;
   while(Icontour)
   {
       area=fabs(cvContourArea(Icontour,CV_WHOLE_SEQ));
#if _DEBUG&&0 
	   cvDrawContours(des, Icontour,
           CV_RGB(255,255,255), CV_RGB(255, 0,0),
           0, 1, 8, cvPoint(0,0));
#endif   
       if(area>maxArea)
       {           
           maxContour = Icontour;
		   maxArea=area;
       }
       Icontour =Icontour->h_next;
   }
	cvZero(src_binary_t);
	cvDrawContours(src_binary_t, maxContour,
    CV_RGB(255,255,255), CV_RGB(255, 255,255), 0,CV_FILLED );
#if _DEBUG
	wait_for_show_image("src_bin_adjust",src_binary_t);
#endif
#if _DEBUG
	IplImage* des = cvCreateImage(cvGetSize(src), src->depth, 3);
	cvZero(des);
	cv::Point rou = find_central_line(maxContour, des);
	cvReleaseImage(&des);
#endif   

	cvReleaseImage(&src);  

   return maxContour;
}
/*--------------------------------------------------------------*/
/**
*
*
*/
/*--------------------------------------------------------------*/
cv::Point  ImageProcess::find_central_line(CvSeq* maxContour,IplImage* des)
{
	cv::Point pt;
	int thickness_min_box=1;
#if _DEBUG
    cvDrawContours(des, maxContour,
    CV_RGB(255,255,255), CV_RGB(255, 255,255), 0, 1, 8, cvPoint(0,0));
#endif
#if _DEBUG
   //CvRect rect=cvBoundingRect(maxContour,0);
   CvBox2D box=cvMinAreaRect2(maxContour);
  
   //cout<<"长度： "<<box.size.width<<endl<<"宽度： "<<box.size.height;
  
     CvPoint2D32f p4[4];
     cvBoxPoints(box,p4);
     cvLine(des, cvPoint(cvRound(p4[0].x), cvRound(p4[0].y)),
     cvPoint(cvRound(p4[1].x), cvRound(p4[1].y)), CV_RGB(0, 0, 255),thickness_min_box);
       
     cvLine(des, cvPoint(cvRound(p4[1].x), cvRound(p4[1].y)),
     cvPoint(cvRound(p4[2].x), cvRound(p4[2].y)), CV_RGB(0, 0, 255),thickness_min_box);
       
     cvLine(des, cvPoint(cvRound(p4[3].x), cvRound(p4[3].y)),
     cvPoint(cvRound(p4[2].x), cvRound(p4[2].y)), CV_RGB(0,0, 255),thickness_min_box);
       
     cvLine(des, cvPoint(cvRound(p4[3].x), cvRound(p4[3].y)),
     cvPoint(cvRound(p4[0].x), cvRound(p4[0].y)), CV_RGB(0, 0,255),thickness_min_box);
#endif
#if _DEBUG

	  CvRect rect = cvBoundingRect(maxContour,1);
	  cvRectangle(des, cvPoint(rect.x, rect.y), cvPoint(rect.x + rect.width, rect.y + rect.height),CV_RGB(255, 0, 0), 2, 8, 0);
	  	  
		try {  
			CvBox2D ellipse = cvFitEllipse2(maxContour);//最小二乘法的椭圆拟合    
			cvEllipseBox(des,ellipse,CV_RGB(255,255,0),2 ); //在图上画椭圆  

			float k=tan((ellipse.angle-90)*CV_PI/180);
			float k1=-1/k;
			float b=ellipse.center.y-k*ellipse.center.x;
			float b1=box.center.y-k1*box.center.x;
			{
						cv::Point p0,p1;
						p0.x=0;p0.y=k*p0.x+b;
						p1.x=des->width;p1.y=k*p1.x+b;
						cvLine(des,p0,p1,CV_RGB(255,0,255));
			}
			
			{
						cv::Point p0,p1;
						p0.x=0;p0.y=k1*p0.x+b1;
						p1.x=des->width;p1.y=k1*p1.x+b1;
						cvLine(des,p0,p1,CV_RGB(255,0,255));		
			}




		}catch(cv::Exception& exn) {
			printf("%s\n",exn.msg.c_str());
		}
#endif
	  
#if _DEBUG
		wait_for_show_image("max_contour",des);
#endif

	return pt;
}
/*--------------------------------------------------------------*/
/**
*
*
*/
/*--------------------------------------------------------------*/
//void ImageProcess::equalizeHist_by_mask(IplImage* img_gary,CvRect mask_rect)
//{
//	cvSetImageROI(img_gary, mask_rect);//设置ROI  
//	{
//		cvEqualizeHist(img_gary,img_gary);	
//#if _DEBUG
////		 wait_for_show_image("img_eq",img_gary);
//#endif
//	
//	}
//	cvResetImageROI(img_gary);//★释放ROI，否则，只会显示ROI区域
//
//#if _DEBUG
//		 wait_for_show_image("img_eq",img_gary);
//#endif
//}
/*--------------------------------------------------------------*/
/**
*
*
*/
/*--------------------------------------------------------------*/
void ImageProcess::equalizeHist_by_mask(IplImage* img_gary,CvRect mask_rect)
{
	cvSetImageROI(img_gary, mask_rect);//设置ROI  
	{
		cvEqualizeHist(img_gary,img_gary);	
#if _DEBUG
//		 wait_for_show_image("img_eq",img_gary);
#endif
	
	}
	cvResetImageROI(img_gary);//★释放ROI，否则，只会显示ROI区域

#if _DEBUG
		 wait_for_show_image("img_eq",img_gary);
#endif
}
/*--------------------------------------------------------------*/
/**
*
*
*/
/*--------------------------------------------------------------*/
void ImageProcess::equalizeHist_by_mask(IplImage* img_gary,IplImage* mask_img)
{
	CV_Assert(img_gary->nChannels == 1);
    // 计算直方图
    int histogram[256];
	int numberOfPixelUsed =GetHistogram(img_gary, mask_img,histogram);
	int LUT[256];
    LUT[0] = 1.0*histogram[0] /numberOfPixelUsed*255;
    int sum = histogram[0];
    for (int i = 1; i <= 255; ++i)
    {
        sum += histogram[i];

        LUT[i] = 1.0*sum / numberOfPixelUsed * 255;
    }

 // 计算分布函数(也就是变换函数f(x))
	int numberOfPixel = img_gary->width*img_gary->height;
    // 灰度变换
	uchar *dataOfSrc = (uchar*)(img_gary->imageData);
    uchar *dataOfDst = (uchar*)(img_gary->imageData);
	
	uchar *maskData =NULL;
	 
	 if(mask_img==NULL){
		maskData=NULL;
	 }else{
		maskData=(uchar *)(mask_img->imageData);
	 }

	for (int i = 0; i <= numberOfPixel - 1; ++i){
		
		if(maskData!=NULL && maskData[i]==0xff){
	        dataOfDst[i] = LUT[dataOfSrc[i]];	
		}else if(maskData==NULL){
			dataOfDst[i] = LUT[dataOfSrc[i]];	
		}else{
			//无效数据
		}

	}

 

}
/*--------------------------------------------------------------*/
/**
*
*
*/
/*--------------------------------------------------------------*/
int ImageProcess::GetHistogram(const IplImage* img_gary,const IplImage* mask_img,int *histogram)
{
	 memset(histogram, 0, 256 * sizeof(int));
    //计算直方图
	 int pixelCount = img_gary->width*img_gary->height;
	 int number_of_used=0;
	 uchar *imageData = (uchar *)(img_gary->imageData);
	 uchar *maskData =NULL;
	 
	 if(mask_img==NULL){
		maskData=NULL;
	 }else{
		maskData=(uchar *)(mask_img->imageData);
	 }

    for (int i = 0; i <= pixelCount - 1; ++i)
    {
		if(maskData==NULL){
			int gray = imageData[i];
			histogram[gray]++;
			number_of_used++;
		}else	if(maskData!=NULL && maskData[i]==0xff){
			int gray = imageData[i];
			histogram[gray]++;
			number_of_used++;
		}else{
			//无效数据
		}
    }
	return number_of_used;
}
/*--------------------------------------------------------------*/
/**
*
*
*/
/*--------------------------------------------------------------*/
IplImage* ImageProcess::canny_by_mask(IplImage* src_color_t,IplImage* img_gary,IplImage* mask_img)
{

 IplImage* TheImage=cvCloneImage(img_gary);//灰度

	if(mask_img!=NULL){		
		uchar* maskData=(uchar *)(mask_img->imageData);
		const int pixels=img_gary->width* img_gary->height;
		for(int pi=0;pi<pixels;pi++){
			if(maskData[pi]==0x00){
			((uchar *)(TheImage->imageData))[pi]=0x00;//灰度
			}
		}
	
	}
#if _DEBUG
	wait_for_show_image("Canny org",TheImage);
#endif
	int threshold1=10;
	int threshold2=20;
	cvCanny(TheImage,TheImage , threshold1, threshold2);

#if _DEBUG
	wait_for_show_image("Canny result",TheImage);
#endif

#if _DEBUG
 IplImage* src_color_tt=cvCloneImage(src_color_t);
 uchar* cannyData=(uchar *)(TheImage->imageData);
		//const int pixels=img_gary->width* img_gary->height;

		for(int xi=0;xi<img_gary->width;xi++){
			
			for(int yi=0;yi<img_gary->height;yi++){
				
				CvScalar s = cvGet2D(TheImage, yi,xi);
				int canny_t=s.val[0];
					
				if(canny_t>0){
							CvScalar s_w;//高亮白
								s_w.val[0]=0;
									s_w.val[1]=255;
										s_w.val[2]=255;
								cvSet2D(src_color_tt,yi,xi,s_w);
					}			
			}

		}
		wait_for_show_image("Canny color result",src_color_tt);

	cvReleaseImage(&src_color_tt);
#endif

	return  TheImage;
}
/*--------------------------------------------------------------*/
/**
*
*
*/
/*--------------------------------------------------------------*/
void ImageProcess::Laplace_by_mask(IplImage* img_gary,IplImage* mask_img)
{
	IplImage* TheImage=cvCloneImage(img_gary);
	if(mask_img!=NULL){		
		uchar* maskData=(uchar *)(mask_img->imageData);
		const int pixels=img_gary->width* img_gary->height;
		for(int pi=0;pi<pixels;pi++){
			if(maskData[pi]==0x00){
			((uchar *)(TheImage->imageData))[pi]=0x00;
			}
		}
	
	}
#if _DEBUG
	wait_for_show_image("Laplace org",TheImage);
#endif
	int threshold=3;
	cvLaplace(TheImage,TheImage,2*threshold+1);

#if _DEBUG
	wait_for_show_image("Laplace result",TheImage);
#endif
	cvReleaseImage(&TheImage);
}
/*--------------------------------------------------------------*/
/**
*
*
*/
/*--------------------------------------------------------------*/
void ImageProcess::threshold_binary(IplImage* img_gary,IplImage* mask_img)
{
	 uchar *maskData =NULL;
	 if(mask_img==NULL){
		maskData=NULL;
	 }else{
		maskData=(uchar *)(mask_img->imageData);
	 }
	uchar *dataOfSrc = (uchar*)(img_gary->imageData);
    uchar *dataOfDst = (uchar*)(img_gary->imageData);

	int p_num=img_gary->width*img_gary->height;
	const int light_high_threshold=235;
	const int light_low_threshold=100;
	for(int pi=0;pi<p_num;pi++){
		
		if(maskData==NULL){
		//没有掩膜
				dataOfDst[pi]=(dataOfSrc[pi]>light_high_threshold)?255:dataOfSrc[pi];
				dataOfDst[pi]=(dataOfSrc[pi]<light_low_threshold)?0:dataOfSrc[pi];
		}else{
			if(maskData[pi]==0xff){
				dataOfDst[pi]=(dataOfSrc[pi]>light_high_threshold)?255:dataOfSrc[pi];
				dataOfDst[pi]=(dataOfSrc[pi]<light_low_threshold)?0:dataOfSrc[pi];
			}
		
		}
	
	}
	

}
/*--------------------------------------------------------------*/
/**
*
*
*/
/*--------------------------------------------------------------*/

CvPoint ImageProcess::grayCenter(IplImage* TheImage_org,IplImage* mask_img)
{
	IplImage* TheImage=cvCloneImage(TheImage_org);
	if(mask_img!=NULL){		
		uchar* maskData=(uchar *)(mask_img->imageData);
		const int pixels=TheImage_org->width* TheImage_org->height;
		for(int pi=0;pi<pixels;pi++){
			if(maskData[pi]==0x00){
			((uchar *)(TheImage->imageData))[pi]=0x00;
			}
		}
	
	}
	//灰度重心法求质心
	CvPoint Center;
	int i, j;
	CvScalar cs = cvSum(TheImage);
	Center.x = Center.y = 0;
	double x = 0;
	double y = 0;
	for(i = 0;i < TheImage->width;i++)
	{
		for(j = 0; j < TheImage->height;j++)
		{
			CvScalar s = cvGet2D(TheImage, j,i);
			x += i*s.val[0]/cs.val[0];
			y += j*s.val[0]/cs.val[0];
		}
	}
	Center.x = cvRound(x);
	Center.y = cvRound(y);
#if _DEBUG
	cvCircle(TheImage,Center,50,cvScalar(0),2);
	wait_for_show_image("Center",TheImage);
#endif
	cvReleaseImage(&TheImage);
	return Center;
}
/*--------------------------------------------------------------*/
/**
*
*
*/
/*--------------------------------------------------------------*/
IplImage* ImageProcess::gary_by_angle(IplImage* src_color_t,IplImage* src_gary_t,IplImage* mask_img)
{
	
#if 1
	double hist_t[ANGLE_HIST_NUM+1];
	gary_get_by_angle(src_color_t,hist_t);
#endif

	IplImage* src_lab=cvCreateImage(cvGetSize(src_color_t),src_color_t->depth,3);
			
	cvCvtColor(src_color_t,src_lab,CV_BGR2Lab);
	cvSmooth(src_lab,src_lab,CV_BLUR,5,5,0,0);  
	
	CvScalar value;	
	const int Delta=ANGLE_DELTA;
	const int Light=200;		
	for(int xi=0;xi<src_color_t->width;xi++){
		for(int yi=0;yi<src_color_t->height;yi++){			
			value= cvGet2D(src_lab, yi, xi);
			double l=value.val[0];
			double a=value.val[1];
			double b=value.val[2];
		
#if 0
	
#else
			double new_a=a-255/2.0;
			double new_b=b-255/2.0;

			if(new_a==new_b){
				//灰度系列
				if(l>Light){
					CvScalar s_w;//高亮白
						s_w.val[0]=255;
						cvSet2D(src_gary_t,yi,xi,s_w);
				}else{
					CvScalar s_w;//黑
						s_w.val[0]=0;
						cvSet2D(src_gary_t,yi,xi,s_w);
				}			
			}else{
				//彩色系列
				const int a_down=ANGLE_DOWN-Delta;
				const int a_up=ANGLE_UP+Delta;
				const int hist_num=ANGLE_HIST_NUM;
				const double arc=atan2(new_b,new_a)*180/CV_PI;

					if((arc>a_down)&&(arc<a_up)){

						int hist_pos=1.0*(a_up-arc)/(1.0*(a_up-a_down)/hist_num)+1;

						int hist_light=hist_t[hist_pos];
						
						if(hist_light>255) hist_light=255;
							if(hist_light<0) hist_light=0;

						CvScalar s_w;//彩色
						s_w.val[0]=hist_light;
						cvSet2D(src_gary_t,yi,xi,s_w);
					}						
			}
#endif
		}
	}



#if _DEBUG
	wait_for_show_image("color_cut",src_color_t);
	wait_for_show_image("gary_level",src_gary_t);
#endif
	cvReleaseImage(&src_lab);

	return NULL;
}
/*--------------------------------------------------------------*/
/**
*
*
*/
/*--------------------------------------------------------------*/
void ImageProcess::gary_get_by_angle(IplImage* src_color_t,double* hist_gary_level)
{
	IplImage* src_lab=cvCreateImage(cvGetSize(src_color_t),src_color_t->depth,3);
	
	cvCvtColor(src_color_t,src_lab,CV_BGR2Lab);
	cvSmooth(src_lab,src_lab,CV_BLUR,5,5,0,0);

	//double hist_gary_level[1024];
	float hist_gary_level_count[ANGLE_HIST_NUM+1];
	memset(hist_gary_level,0,(ANGLE_HIST_NUM+1)*sizeof(double));
	memset(hist_gary_level_count,0,(ANGLE_HIST_NUM+1)*sizeof(float));

	CvScalar value;	
	const int Delta=ANGLE_DELTA;
	const int Light=200;		
	
	for(int xi=0;xi<src_color_t->width;xi++){
		for(int yi=0;yi<src_color_t->height;yi++){			
			value= cvGet2D(src_lab, yi, xi);
			double l=value.val[0];
			double a=value.val[1];
			double b=value.val[2];		
#if 1
			double new_a=a-255/2.0;
			double new_b=b-255/2.0;

			if(new_a==new_b){
				//灰度系列
				if(l>Light){
					CvScalar s_w;//高亮白
						s_w.val[0]=255;
						//cvSet2D(src_gary_t,yi,xi,s_w);
				}else{
					CvScalar s_w;//黑
						s_w.val[0]=0;
						//cvSet2D(src_gary_t,yi,xi,s_w);
				}			
			}else{
				//彩色系列
				const int a_down=ANGLE_DOWN-Delta;
				const int a_up=ANGLE_UP+Delta;
				const int hist_num=ANGLE_HIST_NUM;
				const double arc=atan2(new_b,new_a)*180/CV_PI;
					if((arc>a_down)&&(arc<a_up)){

						int hist_pos=1.0*(a_up-arc)/(1.0*(a_up-a_down)/hist_num)+1;
						ASSERT(hist_pos>0&&hist_pos<= hist_num);
						hist_gary_level[hist_pos]+=value.val[0];
						hist_gary_level_count[hist_pos]++;
						
					}						
			}
#endif
		}
	}

	for(int i=0;i<ANGLE_HIST_NUM+1;i++){
		//int index=GetMaxValueIndexdouble( hist_gary_level,ANGLE_HIST_NUM+1);
		int index=i;
		if((hist_gary_level[index]>=0)&&
			(hist_gary_level_count[index]>0)){
			//hist_gary_level[index]=1.0*-255/(i+1);	
			hist_gary_level[index]=1.0*hist_gary_level[index]/hist_gary_level_count[index];	
		}



	}

#if _DEBUG
	wait_for_show_image("color_cut",src_color_t);
	//wait_for_show_image("gary_level",src_gary_t);
#endif
	cvReleaseImage(&src_lab);
}
/*--------------------------------------------------------------*/
/**
*
*
*/
/*--------------------------------------------------------------*/
/*------------------------------ ----------------------------------*/
/**
*获得数组最大值得索引的排序
*
*@param [in] data 数组
*@param size  数组长度
*@param [out] sort 数组最大值排序
*@param sort_num 排序数组大小
*
*/
/*----------------------------------------------------------------*/
int ImageProcess::GetMaxValueIndexdouble(
	double* data, 
	double size)
{
	double* data_t=new double[(long long)size];
	memcpy(data_t,data,sizeof(double)*((long long)size));
	/*****寻找最值***************************************************************/
		float max_value=data[0];
		int max_value_i=0;
		/*******************************/
		for (register int i=0;i<size;i++){
			if (data_t[i]>max_value){
				max_value=data_t[i];
				max_value_i=i;
			}
		}
		/*******************************/
	
	/*******************************************************************************/
	delete []data_t;

	return max_value_i;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
IplImage* ImageProcess::cut_image(IplImage* src_color_t,int method,CvRect& cut_t,IplImage* src_binary_t)
{
	
	IplImage* src_t=NULL; 

	if(method==1){

		cut_t.x=0;
		cut_t.y=0;
		cut_t.height=src_color_t->height;
		cut_t.width=cut_t.height;

		src_t=cvCreateImage(cvSize(cut_t.width,cut_t.height),IPL_DEPTH_8U,1);

		cvSetImageROI(src_color_t , cut_t);
		

		cvCopyImage(src_color_t,src_t);

	}else if(method==0){
		cut_t.x=src_color_t->width-src_color_t->height;
		cut_t.y=0;
		cut_t.height=src_color_t->height;
		cut_t.width=cut_t.height;

		src_t=cvCreateImage(cvSize(cut_t.width,cut_t.height),IPL_DEPTH_8U,1);

		cvSetImageROI(src_color_t , cut_t);

		cvCopyImage(src_color_t,src_t);
	}else{
		src_t=cvCreateImage(cvSize(cut_t.width,cut_t.height),IPL_DEPTH_8U,1);
		cvZero(src_t);

		cvSetImageROI(src_color_t , cut_t);
		cvSetImageROI(src_binary_t , cut_t);

		cvCopy(src_color_t,src_t,src_binary_t);

		cvResetImageROI(src_binary_t );
	}


	cvResetImageROI(src_color_t);

	return src_t;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void ImageProcess::hough_image(IplImage* src_color_t,int method)
{
	
	CvSeq* results;
	CvMemStorage* storage = cvCreateMemStorage(0);
	
#if 1
	IplImage* src = cvCloneImage(src_color_t);
	IplImage* dst = cvCreateImage(cvGetSize(src_color_t),IPL_DEPTH_8U,3);

	
	cvZero (dst);
	cvCvtColor(src,dst,CV_GRAY2RGB);

	results =ImageProcess::cvHoughCircles_Binary(  //cvHoughCircles函数需要估计每一个像素梯度的方向，
								      //因此会在内部自动调用cvSobel,而二值边缘图像的处理是比较难的
		src,
		storage,
		CV_HOUGH_GRADIENT,
		1,  //累加器图像的分辨率
		1,//圆心间距
		100,//canny
		37,//累加器		
		10,//小半径 
		100//最大半径
		
		);
#endif
	for( int i = 0; i < results->total; i++ )
	{
		float* p = ( float* )cvGetSeqElem( results, i );
		//霍夫圆变换
		CvPoint pt = cvPoint( cvRound( p[0] ), cvRound( p[1] ) );
		cvCircle(
			dst,
			pt,  //确定圆心
			cvRound( p[2] ),  //确定半径
			CV_RGB( 0xff, 0, 0 )
		);  //画圆函数
	}

	

#if _DEBUG
	std::stringstream stream;
	
	stream<<"hough_";
	stream<<method;
	stream<<".png";
	
	wait_for_show_image(stream.str().c_str(),dst);
#endif
	
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void inc_if_inside(double *** H, int x, int y, int height, int width, int r )
{
  if (x>0 && x<width && y> 0 && y<height)
    H[y][x][r]++;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void hough(cv::Mat &img_data, cv::Mat &dist, double threshold, int minRadius, int maxRadius, double distance, cv::Mat &h_acc, cv::Mat &coins){
  int radiusRange = maxRadius - minRadius;
  int HEIGHT = img_data.rows;
  int WIDTH = img_data.cols;
  int DEPTH = radiusRange;

  double ***H;

  // Allocate memory
  H = new double**[HEIGHT];
  for (int i = 0; i < HEIGHT; ++i) {
    H[i] = new double*[WIDTH];

    for (int j = 0; j < WIDTH; ++j)
      H[i][j] = new double[DEPTH];
  }

    for(int y0 = 0; y0 < HEIGHT; y0++) {
    for(int x0 = 0; x0 < WIDTH; x0++) {
      for(int r = minRadius; r < radiusRange; r++) {
       H[y0][x0][r]=0;;
      }
    }
  }

    for(int y=0;y<img_data.rows;y++)  
  {  
       for(int x=0;x<img_data.cols;x++)  
       {  
        // printf("data point : %f\n", img_data.at<float>(y,x));
            if( (float) img_data.at<float>(y,x) > 250.0 )  //threshold image  
            {   
              for (int r=minRadius; r<radiusRange; r++)
              {

                int x0 = cvRound(x + r * cos(dist.at<float>(y,x)) );
                int x1 = cvRound(x - r * cos(dist.at<float>(y,x)) );
                int y0 = cvRound(y + r * sin(dist.at<float>(y,x)) );
                int y1 = cvRound(y - r * sin(dist.at<float>(y,x)) );


                inc_if_inside(H,x0,y0,HEIGHT, WIDTH, r);
                 // inc_if_inside(H,x0,y1,HEIGHT, WIDTH, r);
                 // inc_if_inside(H,x1,y0,HEIGHT, WIDTH, r);
                inc_if_inside(H,x1,y1,HEIGHT, WIDTH, r);
              }
            }  
       }  
  }  

  //create 2D image by summing values of the radius dimension
  for(int y0 = 0; y0 < HEIGHT; y0++) {
    for(int x0 = 0; x0 < WIDTH; x0++) {
      for(int r = minRadius; r < radiusRange; r++) {
        h_acc.at<float>(y0,x0) +=  H[y0][x0][r];// > 1 ? 255 : 0;
       // printf("h : %d", H[y0][x0][r]);
      }
    }
  }

  std::vector<cv::Point3f> bestCircles;
  
  //compute optimal circles
  for(int y0 = 0; y0 < HEIGHT; y0++) {
    for(int x0 = 0; x0 < WIDTH; x0++) {
      for(int r = minRadius; r < radiusRange; r++) { 
        if(H[y0][x0][r] > threshold){
          cv::Point3f circle(x0, y0, r);
          int i;
          for(i = 0; i < bestCircles.size(); i++) {
            int xCoord = bestCircles[i].x;
            int yCoord = bestCircles[i].y;
            int radius = bestCircles[i].z;            
            if(abs(xCoord - x0) < distance && abs(yCoord - y0) < distance) {           
              if(H[y0][x0][r] > H[yCoord][xCoord][radius]) {
                bestCircles.erase(bestCircles.begin()+i);
                bestCircles.insert(bestCircles.begin(), circle);
              }
              break;
            }
          }
          if(i == bestCircles.size()){
            bestCircles.insert(bestCircles.begin(), circle);
          }
        }
      }
    }
  }

  for(int i = 0; i < bestCircles.size(); i++) {
    int lineThickness = 4;
    int lineType = 10;
    int shift = 0;
    int xCoord = bestCircles[i].x;
    int yCoord = bestCircles[i].y;
    int radius = bestCircles[i].z;
    cv::Point2f center(xCoord, yCoord);      
    circle(coins, center, radius-1, cv::Scalar(255,0,0), lineThickness, lineType, shift);
  }
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
cv::Point3f ImageProcess::hough_my(
	IplImage *img_data,
	IplImage *dist,
	double threshold, int minRadius, int maxRadius, double distance,
	IplImage *h_acc,
	IplImage *coins){
  int radiusRange = maxRadius - minRadius;
  int HEIGHT = img_data->height;
  int WIDTH = img_data->width;
  int DEPTH = radiusRange;

  double ***H;
  // Allocate memory
  H = new double**[HEIGHT];
  for (int i = 0; i < HEIGHT; ++i) {
    H[i] = new double*[WIDTH];

    for (int j = 0; j < WIDTH; ++j)
      H[i][j] = new double[DEPTH];
  }

  for(int y0 = 0; y0 < HEIGHT; y0++) {
    for(int x0 = 0; x0 < WIDTH; x0++) {
      for(int r = minRadius; r <  maxRadius; r++) {
       H[y0][x0][r-minRadius]=0;
      }
    }
  }
    for(int y=0;y<HEIGHT;y++)  
  {  
       for(int x=0;x<WIDTH;x++)  
       {  
		   	double pixel_value_t=cvGetReal2D(img_data,y,x);
        // printf("data point : %f\n", img_data.at<float>(y,x));
            if(pixel_value_t> 250.0 )  //threshold image  
            {   
				  for (int r=minRadius; r< maxRadius; r++)
				  {
					  for(float ai=-180;ai<180;ai+=1){

						int x0 = cvRound(x + r * cos(ai/180*CV_PI));

						int y0 = cvRound(y + r * sin(ai/180*CV_PI));
					
						inc_if_inside(H,x0,y0,HEIGHT, WIDTH, r-minRadius);			  
					  }
				  }
            }  
       }  
  }  

  //create 2D image by summing values of the radius dimension
  for(int y0 = 0; y0 < HEIGHT; y0++) {
    for(int x0 = 0; x0 < WIDTH; x0++) {
      for(int r = minRadius; r < maxRadius; r++) {
		    cvSetReal2D(h_acc,y0,x0,cvGetReal2D(h_acc,y0,x0)+ H[y0][x0][r-minRadius]); 
		  
      }
    }
  }

  std::vector<cv::Point3f> bestCircles;
  
  //compute optimal circles
  for(int y0 = 0; y0 < HEIGHT; y0++) {
    for(int x0 = 0; x0 < WIDTH; x0++) {
      for(int r = minRadius; r < maxRadius; r++) { 
        if(H[y0][x0][r-minRadius] >= threshold){
          cv::Point3f circle(x0, y0, r);
          int i;
          for(i = 0; i < bestCircles.size(); i++) {
            int xCoord = bestCircles[i].x;
            int yCoord = bestCircles[i].y;
            int radius = bestCircles[i].z;            
            if(abs(xCoord - x0) < distance && abs(yCoord - y0) < distance) {           
              if(H[y0][x0][r-minRadius] > H[yCoord][xCoord][radius-minRadius]) {
                bestCircles.erase(bestCircles.begin()+i);
                bestCircles.insert(bestCircles.begin(), circle);
              }
              break;
            }
          }
          if(i == bestCircles.size()){
            bestCircles.insert(bestCircles.begin(), circle);
          }
        }
      }
    }
  }

#if _DEBUG
  for(int i = 0; i < bestCircles.size(); i++) {
    int lineThickness = 2;
    int lineType = 8;
    int shift = 0;
    int xCoord = bestCircles[i].x;
    int yCoord = bestCircles[i].y;
    int radius = bestCircles[i].z;
    cv::Point2f center(xCoord, yCoord);      
    cvCircle(coins, center, radius-2, cv::Scalar(255,0,0), lineThickness, lineType, shift);
  }
#endif

  for (int i = 0; i < HEIGHT; ++i) 
  for (int j = 0; j < WIDTH; ++j)
		delete[] H[i][j];
   
  for (int i = 0; i < HEIGHT; ++i) 
		delete[] H[i];
  
  delete[]H;
  if(bestCircles.size()>0){
    return bestCircles.at(0);
  }else{
	  cv::Point3f null;null.x=0; null.y=0,null.z=0;
   return null;
  }

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
cv::Point3f ImageProcess::hough_my_fast_big(IplImage *img_data)
{
	double threshold=20;
	int min_r=65;
	int max_r=85;
	return	hough_my_fast(img_data,
			1,
			threshold,
			min_r,
			max_r,
 			img_data->width);	
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
cv::Point3f ImageProcess::hough_my_fast_small(IplImage *img_data)
{
	double threshold=10;
	int min_r=60;
	int max_r=66;
	return	hough_my_fast(img_data,
			0,
			threshold,
			min_r,
			max_r,
 			img_data->width);	
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
cv::Point3f ImageProcess::hough_my_fast(IplImage *img_data,
			int method,
			double threshold,
			int minRadius,
			int maxRadius,
			double distance)
{
	IplImage *	dist=cvCreateImage(cvGetSize(img_data),IPL_DEPTH_32F,1);
	IplImage *	coins=cvCreateImage(cvGetSize(img_data),IPL_DEPTH_8U,3);
	IplImage *  h_acc=cvCreateImage(cvGetSize(img_data),IPL_DEPTH_32F,1);
	
	cvZero(dist);
	cvZero(coins);
	cvZero(h_acc);

	cvCvtColor(img_data,coins,CV_GRAY2RGB);


	cv::Point3f xyr=hough_my(
	img_data,
	dist,
	threshold,//10
	minRadius,//15
	maxRadius,//100
	distance,//45
	h_acc,
	coins);
#if _DEBUG
	std::stringstream stream;
	
	stream<<"hough_";
	stream<<method;
	stream<<"_my_result";

	wait_for_show_image(stream.str(),coins);
#endif
 	cvReleaseImage(&dist);
	cvReleaseImage(&coins);
	cvReleaseImage(&h_acc);
	return xyr;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void ImageProcess::process_max_min_rect(CvMemStorage* memory,IplImage* src_color_cut,IplImage* src_gary_cut,CvSeq* seq,IplImage* src_binary_cut,cv::Point3f& circle0,cv::Point3f& circle1)
{
	CvBox2D box=cvMinAreaRect2(seq);//最小外包矩形
	cv::Point cut_center_t=box.center;
	CvBox2D ellipse = cvFitEllipse2(seq);//最小二乘法的椭圆拟合    
	CvRect rect_cut = cvBoundingRect(seq,1);
	float k=0;
	float b=0;

	float A=0;
	float B=0;
	float C=0;
	float CountPart[2]={0,0};

	if(ellipse.angle==0){
		//中垂线，90
		A=1;
		B=0;
		C=-cut_center_t.x;	
	}else{
		k=-1.0/tan((ellipse.angle-90)*CV_PI/180);
		b=cut_center_t.y-k*cut_center_t.x;
		A=-1*k;
		B=1;
		C=-1*b;
	}

	IplImage *src_binary_cut_part0=cvCreateImage(cvGetSize(src_binary_cut),src_binary_cut->depth,1);
	cvZero(src_binary_cut_part0);	
	IplImage *src_binary_cut_part1=cvCreateImage(cvGetSize(src_binary_cut),src_binary_cut->depth,1);
	cvZero(src_binary_cut_part1);	
	{
		int B_W=src_binary_cut->width;
		int B_H=src_binary_cut->height;
		/*-----------------------------------------*/
		for(int xi=0;xi<B_W;xi++){
			for(int yi=0;yi<B_H;yi++){
				float	pixel_value_t=cvGetReal2D(src_binary_cut,yi,xi);
				if(pixel_value_t>250){
					float pos=A*(xi+rect_cut.x)+B*(yi+rect_cut.y)+C;
						if(pos>0){
							//cvSetReal2D(src_binary_cut_part0,yi,xi,255);
							((uchar *)(src_binary_cut_part0->imageData + yi*src_binary_cut_part0->widthStep))[xi]=255;  
							CountPart[0]++;
						}else{
							//cvSetReal2D(src_binary_cut_part1,yi,xi,255);
							((uchar *)(src_binary_cut_part1->imageData + yi*src_binary_cut_part1->widthStep))[xi]=255;  
							CountPart[1]++;
						}
				}
			}
		}
		/*-----------------------------------------*/
#if _DEBUG
		wait_for_show_image("part1",src_binary_cut_part1);
		wait_for_show_image("part0",src_binary_cut_part0);
#endif
#if 1
	gary_by_angle(src_color_cut,src_gary_cut,src_binary_cut);
	IplImage*  canny_result=canny_by_mask(src_color_cut,src_gary_cut,src_binary_cut);

#endif
		/*-----------------------------------------*/
		CvRect cut_1_rect;
		CvRect cut_0_rect;
#if 1			
		cut_image_01(memory,src_color_cut,src_binary_cut_part1,cut_1_rect);
		cut_image_01(memory,src_color_cut,src_binary_cut_part0,cut_0_rect);
#endif
#if _DEBUG
		wait_for_show_image("part11_cut",src_binary_cut_part1);
		wait_for_show_image("part00_cut",src_binary_cut_part0);
#endif
		/*-----------------------------------------*/

	IplImage* cut_1=cut_image(canny_result,-1,cut_1_rect,src_binary_cut_part1);
	IplImage* cut_0=cut_image(canny_result,-1,cut_0_rect,src_binary_cut_part0);
#if _DEBUG
	wait_for_show_image("cut_1",cut_1);
	wait_for_show_image("cut_0",cut_0);
#endif

#if 1
		if(CountPart[1]>CountPart[0]){
				circle1=hough_my_fast_big(cut_1);	
				circle0=hough_my_fast_small(cut_0);
		}else{
				circle1=hough_my_fast_small(cut_1);	
				circle0=hough_my_fast_big(cut_0);				
		}
		circle1.x+=cut_1_rect.x;
			circle1.y+=cut_1_rect.y;
			circle0.x+=cut_0_rect.x;
			circle0.y+=cut_0_rect.y;

			
#endif
		/*-----------------------------------------*/
		cvReleaseImage(&cut_1);
		cvReleaseImage(&cut_0);

	}
	cvReleaseImage(&src_binary_cut_part0);
	cvReleaseImage(&src_binary_cut_part1);

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
 IplImage* ImageProcess::cut_image_01(CvMemStorage* memory,IplImage* src_color_cut,IplImage* src_binary_cut_part,CvRect& cut_t)
{
//#if _DEBUG
//	wait_for_show_image("src_bin_adjust_22b",src_binary_cut_part);
//#endif
   //CvMemStorage* memory=cvCreateMemStorage(0);
   CvSeq* Icontour=NULL;
   CvSeq* maxContour =NULL;
    double area=0;
   double maxArea=0; 
   
   IplImage* src_binary_cut_part_temp=cvCloneImage(src_binary_cut_part);
#if 1
   cvFindContours(src_binary_cut_part_temp,
	   memory,
	   &Icontour,
	   sizeof(CvContour),
	   CV_RETR_EXTERNAL,
	   CV_CHAIN_APPROX_SIMPLE,
	   cvPoint(0,0));

   while(Icontour)
   {
       area=fabs(cvContourArea(Icontour,CV_WHOLE_SEQ));
   
       if(area>maxArea)
       {           
           maxContour = Icontour;
		   maxArea=area;
       }
       Icontour =Icontour->h_next;
   }
	cut_t=cvBoundingRect(maxContour,1);	
#if 1
	//cvZero(src_binary_cut_part);	
	cvDrawContours(src_binary_cut_part, maxContour,
           cv::Scalar(0),cv::Scalar(255),
           0,5);
#endif


#endif
	cvReleaseImage(&src_binary_cut_part_temp);
 	return NULL;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
  void ImageProcess::HYAdaptiveFindThreshold(CvMat *dx, CvMat *dy, double *low, double *high)
 {
	 CvSize size = cvGetSize(dx);
	 IplImage* imge = cvCreateImage(size, IPL_DEPTH_32F, 1);

	 // 计算边缘的强度, 并存于图像中
	 int i, j;
	 short* _dx = 0;
	 short* _dy = 0;
	 float* _image = 0;
	 float maxv = 0;
	 for (i = 0; i < size.height; ++i)
	 {
		 _dx = (short*)(dx->data.ptr + dx->step * i);
		 _dy = (short*)(dy->data.ptr + dy->step * i);
		 _image = (float *)(imge->imageData + imge->widthStep * i);
		 for (j = 0; j < size.width; ++j)
		 {
			 _image[j] = (float)(abs(_dx[j]) + abs(_dy[j]));
			 maxv = maxv < _image[j] ? _image[j] : maxv;
		 }
	 }

	 int hist_size = 255;
	 float range_0[] = { 0, 256 };
	 float* ranges[] = { range_0 };

	 // 计算直方图
	 range_0[1] = maxv;	// 最大值
	 hist_size = (int)(hist_size > maxv ? maxv : hist_size);

	 CvHistogram* hist = cvCreateHist(1, &hist_size, CV_HIST_ARRAY, ranges, 1);
	 cvCalcHist(&imge, hist, 0, 0);

	 double dPercentOfPixelsNotEdges = 0.97;
	 int total = (int)(size.height * size.width * dPercentOfPixelsNotEdges);
	 float sum = 0;
	 int icount = hist->mat.dim[0].size;

	 float *h = (float*)cvPtr1D(hist->bins, 0);
	 for (i = 0; i < icount; ++i)
	 {
		 sum += h[i];
		 if (sum > total)
			 break;
	 }
	 // 计算高低门限
	 *high = (i + 1) * maxv / hist_size;
	 *low = *high * 0.4;
	 cvReleaseImage(&imge);
	 cvReleaseHist(&hist);
 }
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void  ImageProcess::CannyAdaptiveFindThreshold(IplImage* pImg8u,double *low, double *high)
{
	int aperture_size = 3;
	//梯度    
	CvMat* dx = cvCreateMat(pImg8u->height, pImg8u->width, CV_16SC1);
	CvMat* dy = cvCreateMat(pImg8u->height, pImg8u->width, CV_16SC1);

	cvSobel(pImg8u, dx, 1, 0, aperture_size);
	cvSobel(pImg8u, dy, 0, 1, aperture_size);

	// 我们可以在这里自适应计算高低阈值  
	if (*low == -1 && *high == -1)
	{
		HYAdaptiveFindThreshold(dx, dy, low, high);
	}

	
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void ImageProcess::HoughLine(IplImage* pImg, int *pR, int *pTh, int iThreshold,float rho_min,float rho_max,float theta_center,float theta_range)

// src is the src_img one_dimension array
// width is width of src_img, height  is height of src_img
// pR is distance from original_point to the line
// pTh is the angle of the line
// iThreshold value determines whether it is line or not


{
	unsigned char *src = (unsigned char*) pImg->imageData;
	int width = pImg->width;
	int height = pImg->height;
	int *pArray;
	//
	const int iRMax = (int)sqrt(width * width + height * height) + 1;
	// this is diagonal line length
	const int iThMax = 360;
	//  将直线的斜率分成360份，也即一周
	int iTh = 0;
	//
	int iR;
	int iMax = -1;
	int iThMaxIndex = -1;
	int iRMaxIndex = -1;

	pArray = new int[iRMax * iThMax];
	memset(pArray, 0, sizeof(int) * iRMax * iThMax);

	float fRate = (float)(2*M_PI /iThMax);

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			unsigned char* pixel_data=(uchar*) (pImg->imageData + y*pImg->widthStep+x);

			if (*pixel_data == 255)
				//255 stands for foreground picture
			{
					for (iTh = 0; iTh < iThMax; iTh += 1){
								if (std::abs(iTh - theta_center) < theta_range) {
					
									iR = (int)(x * cos(iTh * fRate) + y * sin(iTh * fRate));

										if (iR > rho_min && iR<rho_max){
											pArray[iR / 1 * iThMax + iTh]++;
										}

								}
					}
			}

		
		} // x
	} // y
	  // 这个是累积计算的过程，因为xy已知，当直线垂直于原点和xy点时候有最大的iR


	for (iR = 0; iR < iRMax; iR++)
	{
		for (iTh = 0; iTh < iThMax; iTh++)
		{
			int iCount = pArray[iR * iThMax + iTh];
			if (iCount > iMax)
			{
				iMax = iCount;
				iRMaxIndex = iR;
				iThMaxIndex = iTh;
			}
		}
	}
	// 找最大值的过程
	iThreshold = (iThreshold >= 2) ? iThreshold : 2;
	if (iMax >= iThreshold)
	{
		*pR = iRMaxIndex;
		*pTh = iThMaxIndex;
	}

	delete[]pArray;

	return;
} // end of Hough
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void ImageProcess::Draw_line_on_image(float rho,float theta, CvRect rect_cut, IplImage* iplimg_tmp,cv::Point& pt1,cv::Point& pt2)
{
#if TRUE
		
	theta = theta*M_PI / 180;

	double cos_t = cos(theta);
	double sin_t = sin(theta);
			
	double k;
	double b;

	if (sin_t == 0) {
		
		pt1.x = 0;
		pt1.y = rho;
		
		pt2.x = rect_cut.width;
		pt2.y = rho;

	}else {
 		k = -1*cos_t/sin_t;
		b = rho / sin_t;
		
		pt1.x = 0;
		pt1.y = 0*k+b;
		
		pt2.x = rect_cut.width;
		pt2.y = rect_cut.width*k+b;
	}

	/*pt1.x +=rect_cut.x ;
	pt1.y += rect_cut.y;
	pt2.x += rect_cut.x;
	pt2.y += rect_cut.y;*/
	if (iplimg_tmp != nullptr) {
		cvLine(iplimg_tmp, pt1, pt2, CV_RGB(0, 0, 255), 1, 8);
	}
	

#endif // 0
#if TRUE
	pt1.x = pt1.x+rect_cut.x;
	pt1.y = pt1.y+rect_cut.y;

	pt2.x = pt2.x+rect_cut.x;
	pt2.y = pt2.y+rect_cut.y;
#endif

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
std::vector<float> ImageProcess::crack_get_image_feature_gauss(IplImage * diff_org, std::string file_base,std::string file_name,int CIRCLE,int CHANNEL, int frame_idx, IplImage *image_out,std::vector<float>& delta_out, boolean SAVE_FLAG)
{
#if TRUE

	assert(CIRCLE > 0);
	assert(CHANNEL > 0);


	const std::string four_delta_str = "4delta";
	const std::string diff_str = "diff";

	
	std::string channel_diff_path_str=Base::CRACK_PATH_GetFrameChannelDiff(file_base,CIRCLE,CHANNEL, diff_str,false);

	std::string sub2_path_str = Base::FS_createPath(channel_diff_path_str, four_delta_str,SAVE_FLAG);
	
	const std::string path_diff_out = sub2_path_str+Base::int2str(frame_idx)+".png";

	CvSize diff_size = cvGetSize(diff_org);
	const int SCALE = 1;

	//IplImage *image_out = cvCreateImage(cvSize(diff_size.width*SCALE, diff_size.height*SCALE), IPL_DEPTH_8U, 3);

	const CvScalar color_black = CV_RGB(0, 0, 0);
	const CvScalar color_blue = CV_RGB(0, 0, 255);
	const CvScalar color_yellow = CV_RGB(255, 255, 0);
	const CvScalar color_red_64 = CV_RGB(64, 0, 0);
	const CvScalar color_red_128 = CV_RGB(128, 0, 0);
	const CvScalar color_red_255 = CV_RGB(255, 0, 0);
#endif // TRUE
	
	double Sum_delta[6] = {0};
	double a_col_sum_delta[4] = {0};
	memset(Sum_delta, 0, sizeof(Sum_delta));//统计求和
	std::vector<double> a_col_tmp;
	a_col_tmp.resize(diff_org->height, 0);//每列的暂存数据
	delta_out.resize(diff_org->width, 0);

	for (size_t coli = 0; coli <diff_org->width; coli++)
	{
		memset(a_col_sum_delta, 0, sizeof(a_col_sum_delta));//Col统计结果清零
#if TRUE
		for (size_t rowi = 0; rowi <diff_org->height; rowi++)
		{
			a_col_tmp[rowi] = cvGetReal2D(diff_org, rowi, coli);
		}
		//计算均值方差，统计结果
		double delta_t = 0, avg_t = 0;
		delta_out[coli]=delta_t = Base::Math_GetVarianceValue(a_col_tmp.data(), diff_org->height, avg_t, &delta_t);
#endif // TRUE
#if TRUE
		CvScalar color_fill;
		for (size_t rowi = 0; rowi <diff_org->height; rowi++)
		{
			double diff_value = a_col_tmp[rowi];
			const double range[] = { 1 * delta_t,2 * delta_t,3 * delta_t };
			double test_value = fabs(diff_value - 0);

			if (test_value <= range[0]) {
				a_col_sum_delta[0]++;
				color_fill = color_black;
			}
			else if (test_value > range[0] && test_value <= range[1]) {
				a_col_sum_delta[1]++;
				color_fill = color_blue;
			}
			else if (test_value > range[1] && test_value <= range[2]) {
				a_col_sum_delta[2]++;
				color_fill = color_yellow;
			}
			else if (test_value > range[2]) {
				a_col_sum_delta[3]++;
				color_fill = color_red_255;
			}
			else {
				assert(false);
			}

			cvSet2D(image_out, rowi, coli, color_fill);

		}
#endif // TRUE

		//统计整幅图像
		Sum_delta[0] += a_col_sum_delta[0];
		Sum_delta[1] += a_col_sum_delta[1];
		Sum_delta[2] += a_col_sum_delta[2];
		Sum_delta[3] += a_col_sum_delta[3];
		Sum_delta[4] += delta_t;
		Base::Math_AbsArray(a_col_tmp.data(),a_col_tmp.size() );
		Sum_delta[5] += Base::Math_GetMaxValue(a_col_tmp.data(),a_col_tmp.size());

	}

	std::vector<float> feature_data_t;
#if FALSE
	const float sum_pixel = diff_org->width*diff_org->height;
	assert(sum_pixel == Sum_delta[0] + Sum_delta[1] + Sum_delta[2] + Sum_delta[3]);
	for (size_t i = 1; i <4; i++)
	{
		feature_data_t.push_back(Sum_delta[i] / sum_pixel);
	}
#endif // TRUE

#if 0
	Sum_delta[4] =1.0* Sum_delta[4] / diff_org->width / 255;
	assert(Sum_delta[4] >= 0 && Sum_delta[4] <= 1 + 1E-6);
	feature_data_t.push_back(Sum_delta[4]);
#endif // TRUE

#if TRUE
	int IDX = 5;
	Sum_delta[IDX] = 1.0* Sum_delta[IDX] / diff_org->width / 255;
	feature_data_t.push_back(Sum_delta[IDX]);//最大值求和
	assert(Sum_delta[IDX] >= 0 && Sum_delta[IDX] <= 1 + 1E-6);
#endif // TRUE

	/*if (0)
	{
		//每一列的方差。方差中大于3倍方差 做特征
#if TRUE
	float avg_t=Base::Math_GetAverageValueF(delta_out.data(),delta_out.size());
	float variance_t=0;
	variance_t = Base::Math_GetVarianceValueF(delta_out.data(), delta_out.size(), avg_t, &variance_t);
	vector<float> data_delta_3;
	for (size_t i = 0; i < delta_out.size(); i++){
		float sub = fabs(delta_out.at(i) - avg_t);
		if (sub > 3 * variance_t) {
		 //3 δ之外
			data_delta_3.push_back(delta_out[i]);
		}
			
	}
	//求和
	const float Sum = Base::Math_GetSumF(data_delta_3.data(),data_delta_3.size());

	const float Feature = Sum / 255 / delta_out.size();
	feature_data_t.push_back(Feature);
#endif // TRUE
	}*/




#if TRUE
	if (SAVE_FLAG)
	{
		if (file_base != "") {
				cvSaveImage(path_diff_out.c_str(), image_out);
			}	
	}
	
#endif // TRUE

#if TRUE
	//vector<float> delta_out;
	std::vector<float> histogram;
	const int HISTOGRAM_DIM = diff_org->width;
	std::vector<std::vector<CvPoint>>   point_sets;

	ImageProcess::crack_get_long_crack(diff_org,image_out, 4, point_sets, channel_diff_path_str,CIRCLE, CHANNEL, frame_idx,SAVE_FLAG);
	
	std::vector<float> hist_feature = process_histogram(histogram, point_sets, delta_out, HISTOGRAM_DIM, diff_org->width, diff_org->height);
	std::vector<float> all_feature = Base::CombineVector(feature_data_t, hist_feature);
	
	if (SAVE_FLAG){
			std::vector<float> hist_1920;
			hist_1920.resize(ImageProcess::CRACK_MAX_SIZE,0.0);

			std::copy(histogram.begin(), histogram.end(), hist_1920.begin());

			ImageProcess::DrawHistogram(hist_1920.data(), hist_1920.size(), channel_diff_path_str, CHANNEL, frame_idx, all_feature);
	}
#endif // TRUE


	return all_feature;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
std::vector<float> ImageProcess::crack_get_image_feature_one_line(std::string org)
{
	std::vector<float> feature_one;
	std::vector<float> feature_org;

	IplImage * img_org=cvLoadImage(org.c_str(),0);
	
	for (size_t ci = 0; ci <img_org->width; ci++){
		 float data_t=cvGetReal2D(img_org,0,ci);
		 feature_org.push_back(data_t);
	}

	float avg_org=Base::Math_GetAverageValueF(feature_org.data(), feature_org.size());//[0,255]
	float sigma_org = 0;//[0,255]
	Base::Math_GetVarianceValueF(feature_org.data(), feature_org.size(), avg_org, &sigma_org);
	float line_width = 0;
	
	std::vector<float> feature_adj;
	{
		for (size_t i = 0; i <feature_org.size(); i++){

			float value_col = feature_org.at(i);
			float sub_abs = fabs(avg_org - value_col);
			
			if (sub_abs > 3 * sigma_org) {
				line_width++;
			}else{
				feature_adj.push_back(value_col);
			}

		}
	


	}

	float avg_adj=Base::Math_GetAverageValueF(feature_adj.data(),feature_adj.size());

	float sigma_adj = 0;
		Base::Math_GetVarianceValueF(feature_adj.data(),feature_adj.size(),avg_adj,&sigma_adj);
	


		float f0 = 0;
		float f1 = 0;
		float f2 = 0;

 		if (avg_adj>0)
		{
			f0 = fabs(avg_org - avg_adj) /avg_org;
		}else{
			f0 = 0;
		}

		if (sigma_org>0)
		{
				f1 = sigma_adj / sigma_org;
				
		}else{
			f1 = 0;
		}

		if (true)
		{
				f2 = line_width / CRACK_MAX_WIDTH;
				
		}
		
		

		
		assert(f0 >= 0 && f0 < 1 + 1E-6);
		assert(f1 >= 0 && f1 < 1 + 1E-6);
		assert(f2 >= 0 && f2 < 1 + 1E-6);


		feature_one.push_back(f0);
		feature_one.push_back(f1);
		feature_one.push_back(f2);




	cvReleaseImage(&img_org);
	return feature_one;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void ImageProcess::crack_get_long_crack(IplImage * diff_org,
										IplImage *image_4_delta,
										int delta_idx,
										std::vector<std::vector<CvPoint>>&   point_sets,
										std::string file_base,
										int CIRCLE,
										int CHANNEL,
										int frame_idx,
										boolean SAVE_FLAG)
{
	const int dx4[8] = {-1,-1,-1, 0, 0, 1,1,1};
	const int dy4[8] = {-1, 0, 1,-1, 1,-1,0,1};
	const int WIDTH= image_4_delta->width;
	const int HEIGHT= image_4_delta->height;
	const CvScalar color_black = CV_RGB(0, 0, 0);
	const CvScalar color_blue = CV_RGB(0, 0, 255);
	const CvScalar color_yellow = CV_RGB(255, 255, 0);
	const CvScalar color_red_255 = CV_RGB(255, 0, 0);
	const CvScalar color_XXX = CV_RGB(0, 255, 255);
	CvScalar color_target = CV_RGB(255, 0, 0);
	
	if (delta_idx == 1) {
		color_target = color_black;
	}
	else if (delta_idx == 2) {
		color_target = color_blue;
	}
	else if (delta_idx == 3) {
		color_target = color_yellow;
	}
	else if (delta_idx == 4) {
		color_target = color_red_255;
	}
	else {

	}
	
	IplImage* image_4_delta_out = cvCreateImage(cvGetSize(image_4_delta), IPL_DEPTH_8U, 3);
	IplImage* image_visit = cvCreateImage(cvGetSize(image_4_delta), IPL_DEPTH_8U, 1);
	cvZero(image_4_delta_out);
	cvZero(image_visit);
	point_sets.clear();
	{
#if TRUE		
		for (size_t ci = 0; ci < WIDTH; ci++) {
			for (size_t ri = 0; ri < HEIGHT; ri++) {
				CvScalar visit = cvGet2D(image_visit, ri, ci);
				std::vector<CvPoint>   point_set;
				if (visit.val[0] == 0) {				
						CvScalar value = cvGet2D(image_4_delta, ri, ci);
						visit.val[0] = 255;
						cvSet2D(image_visit,ri, ci, visit);
						
						if (value.val[0]== color_target.val[0]
							&& value.val[1] == color_target.val[1]
							&& value.val[2] == color_target.val[2]
							&& value.val[3] == color_target.val[3]) {
							//采样点
							
							point_set.push_back(cvPoint(ci, ri));
							

								for (register int c = 0; c <point_set.size(); c++)
								{
										for (register int n = 0; n < 8; n++)
										{
												int x_n = point_set[c].x + dx4[n];
												int y_n = point_set[c].y + dy4[n];

												if ((x_n >= 0 && x_n <  WIDTH) && (y_n >= 0 && y_n < HEIGHT))
												{
														//int nindex = y*WIDTH + x;
														CvScalar visit = cvGet2D(image_visit, y_n, x_n);
														CvScalar n_color = cvGet2D(image_4_delta,y_n,x_n);

														if (visit.val[0] == 0) {
																	if ((n_color.val[0] == color_target.val[0])
																			&& (n_color.val[1] == color_target.val[1])
																			&& (n_color.val[2] == color_target.val[2])
																			&& (n_color.val[3] == color_target.val[3])) {
																			point_set.push_back(cvPoint(x_n, y_n));
																	}
																	visit.val[0] = 255;
																	cvSet2D(image_visit, y_n, x_n, visit);										
														}

												}

										}
								}
						}
				}
				if (point_set.size()>0) {
					visit.val[0] = 255;
					point_sets.push_back(point_set);
				}
				
			}
		}
#endif // TRUE

#if TRUE
		std::vector<std::vector<CvPoint>>::iterator it;
		for (it = point_sets.begin(); it != point_sets.end();){
			std::vector<CvPoint> point_set = *it;
			if (point_set.size() == 1)
				it = point_sets.erase(it);    //删除元素，返回值指向已删除元素的下一个位置    
			else
				++it;    //指向下一个位置
		}
#endif // TRUE



#if _DEBUG
		for (size_t si = 0; si < point_sets.size();si++) {
			std::vector<CvPoint> point_set=point_sets.at(si);
			for (size_t pi = 0; pi < point_set.size();pi++) {
				cvSet2D(image_4_delta_out,
					point_set.at(pi).y,
					point_set.at(pi).x,
					color_target);
			}
		}
#endif // _DEBUG
#if _DEBUG&&0
		vector<double> a_col_tmp;
		a_col_tmp.resize(diff_org->height, 0);//每列的暂存数据
		for (size_t coli = 0; coli < diff_org->width; coli++) {
			for (size_t rowi = 0; rowi <diff_org->height; rowi++) {
				a_col_tmp[rowi] = cvGetReal2D(diff_org, rowi, coli);
			}			
			vector<int> sort;
			sort.resize(a_col_tmp.size());
			
			Base::Math_AbsArray(a_col_tmp.data(), a_col_tmp.size());
			Base::Math_GetMaxValueIndex(a_col_tmp.data(), a_col_tmp.size(), sort.data(), sort.size());
			
			for (size_t i = 0; i <sort.size(); i++){
				double data_t = a_col_tmp.at(sort[i]);
				if (	(a_col_tmp.at(sort[0]) == data_t) &&
						(data_t!=0)					
					) {
					cvSet2D(image_4_delta_out, sort[i], coli, color_yellow);
				}
			}

			
		}

#endif // _DEBUG

	cvShowImage("single_delta", image_4_delta_out);
	cvWaitKey(1);
	
			if (SAVE_FLAG) {							
					std::stringstream sub2_path_ss;
					sub2_path_ss << "single_delta";
					std::string sub2_path_str = Base::FS_createPath(file_base, sub2_path_ss.str());
					std::string filesaveimg = sub2_path_str + Base::int2str(frame_idx) + ".png";
					cvSaveImage(filesaveimg.c_str(),image_4_delta_out);		
		
			}

	}

	cvReleaseImage(&image_visit);
	cvReleaseImage(&image_4_delta_out);

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void ImageProcess::VVC2Image(std::vector<std::vector<CvPoint>> vvc, IplImage* img)
{
	/*const int WIDTH = img->width;
	const int HEIGHT = img->height;*/
	cvZero(img);
	for (unsigned long i = 0; i <vvc.size(); i++) {

		std::vector<CvPoint> fps = vvc.at(i);

		const int fps_size = fps.size();


		for (unsigned long pi = 0; pi<fps_size; pi++) {

			cvSetReal2D(img, fps[pi].y, fps[pi].x, 255);

		}



	}
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void ImageProcess::CRACK_get_block_sets(
	IplImage * image_binary,
	int TARGET,
	std::vector<std::vector<CvPoint>>& point_sets,
	boolean SAVE_FLAG)
{
	const int dx4[8] = { -1,-1,-1, 0, 0, 1,1,1 };
	const int dy4[8] = { -1, 0, 1,-1, 1,-1,0,1 };

	const int WIDTH = image_binary->width;
	const int HEIGHT = image_binary->height;
			
	IplImage* image_visit = cvCreateImage(cvGetSize(image_binary), IPL_DEPTH_8U, 1);
	cvZero(image_visit);
	point_sets.clear();
	{
#if TRUE		
		for (size_t ci = 0; ci < WIDTH; ci++) {
			for (size_t ri = 0; ri < HEIGHT; ri++) {
				
				CvScalar visit_start = cvGet2D(image_visit, ri, ci);
				std::vector<CvPoint>   point_set;
				
				if (visit_start.val[0] == 0) {					
					visit_start.val[0] = 255;cvSet2D(image_visit, ri, ci, visit_start);

					CvScalar value = cvGet2D(image_binary, ri, ci);
					
					if (value.val[0] ==TARGET ) {
						//采样点

						point_set.push_back(cvPoint(ci, ri));


								for (register int c = 0; c <point_set.size(); c++)
								{
											for (register int n = 0; n < 8; n++)
											{
														int x_n = point_set[c].x + dx4[n];
														int y_n = point_set[c].y + dy4[n];
														

													if ((x_n >= 0 && x_n <  WIDTH) && (y_n >= 0 && y_n < HEIGHT)){
																	//int nindex = y*WIDTH + x;
																		CvScalar visit = cvGet2D(image_visit, y_n, x_n);
																		CvScalar n_color = cvGet2D(image_binary, y_n, x_n);	

																	if (visit.val[0] == 0) {
																		if (n_color.val[0] == TARGET) {
																			point_set.push_back(cvPoint(x_n, y_n));
																		}
																		visit.val[0] = 255;cvSet2D(image_visit, y_n, x_n, visit);
																	}

													}

											}
								}

					}
				}
				if (point_set.size()>0) {
					point_sets.push_back(point_set);
				}

			}
		}
#endif // TRUE
	}
	cvReleaseImage(&image_visit);


#if _DEBUG
	if(SAVE_FLAG){
		IplImage* img = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1);

		VVC2Image(point_sets, img);

		cvReleaseImage(&img);

	}
#endif // _DEBUG

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
std::vector<float> ImageProcess::CRACK_get_histgram(IplImage * diff, std::vector<std::vector<CvPoint>> point_sets,int PN)
{
#if TRUE	
	const long HISTOGRAM_DIM = 1920 * 1080;
	std::vector<float> histogram;
	
	histogram.resize(HISTOGRAM_DIM, 0);
	
	for (size_t si = 0; si < point_sets.size(); si++) {
		
		std::vector<CvPoint> point_set = point_sets.at(si);
		float one_line_sum = 0;
		unsigned long one_line_idx = 0;
		CRACK_get_block_property(diff,point_set,one_line_sum,one_line_idx,PN);
		histogram[one_line_idx] += one_line_sum;
		
	}
	return histogram;
#endif
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
std::vector<float> ImageProcess::CRACK_get_histgram_feature(std::vector<float> histgram,int WIDTH,int  HEIGHT, int PN)
{
	
	int sort_max[1];
	ImageProcess::GetMaxValueIndex(histgram.data(), histgram.size(),sort_max,1);


	float AREA = CRACK_get_histgram_area(histgram,WIDTH,HEIGHT,PN);
	float MAX_POS =1.0*sort_max[0] / (1920 * 1080);

	std::vector<float> feature;

	feature.push_back(MAX_POS);
	feature.push_back(AREA);

	return feature;

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
std::vector<float> ImageProcess::CRACK_get_block_feature(const std::vector<std::vector<CvPoint>> frame_point_sets, std::vector<std::vector<CvPoint>>& frame_point_sets_out, IplImage * diff, int PN)
{
	std::vector<float> number0;//图块数量，数组
	std::vector<float> number1;
	const int WIDTH = diff->width;
	const int HEIGHT = diff->height;
	float t;
	float n_3_sigma = 0;

	frame_point_sets_out.clear();
	for (unsigned long i = 0; i <frame_point_sets.size(); i++){
		
		std::vector<CvPoint> fps=frame_point_sets.at(i);
		number0.push_back(fps.size());

	}

	const float avg0 = Base::Math_GetAverageValueF(number0.data(), number0.size());
	const float sigma0 = Base::Math_GetVarianceValueF(number0.data(), number0.size(), avg0,&t);


	for (unsigned long i = 0; i < number0.size(); i++){
		float diff_f =number0.at(i)-avg0;
			if (diff_f<3*sigma0){
				number1.push_back(number0.at(i));//保留较小的部分
			}
			else
			{
				n_3_sigma += number0.at(i);//大于3倍方差，
			}
	}

	const float avg1 = Base::Math_GetAverageValueF(number1.data(), number1.size());
	const float sigma1 = Base::Math_GetVarianceValueF(number1.data(), number1.size(), avg1, &t);
	
	float f_1;
	float f_2;
	float f_3;
	//float f_4;
	
	std::vector<float> deltaL;
	for (unsigned long i = 0; i <frame_point_sets.size(); i++) {

		std::vector<CvPoint> fps = frame_point_sets.at(i);
		const int fps_size =fps.size();
		if (fps_size-avg0>3*sigma0){
			frame_point_sets_out.push_back(fps);
			for (unsigned long pi = 0;pi<fps_size; pi++){
				 float data_t=cvGetReal2D(diff, fps[pi].y, fps[pi].x);
				 ASSERT(data_t * PN >= 0);
				 deltaL.push_back(data_t);
			}

		}

	}

#if _DEBUG
	{
		IplImage* img = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1);

		VVC2Image(frame_point_sets_out, img);

		cvReleaseImage(&img);

	}
#endif // _DEBUG


	if (avg0>0)
	{
		f_1 = fabs(avg0 - avg1) / avg0;
	}
	else
	{
		f_1 = 0;
	}

	if (sigma0>0)
	{
		f_2 = sigma1 / sigma0;
	}
	else
	{
		f_2 = 0;
	}

	f_3 = Base::Math_GetAverageValueF(deltaL.data(),deltaL.size())/255.0;

	

	//f_4 =1.0 * n_3_sigma / (WIDTH*HEIGHT);
	
	
	std::vector<float> f;

	f.push_back(f_1);
	f.push_back(f_2);
	f.push_back(fabs(f_3));
	//f.push_back(f_4);

	return f;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void ImageProcess::CRACK_get_block_property(
	IplImage * diff,
	std::vector<CvPoint> point_set,
	float& sum,
	unsigned long& count,
	int PN)
{
	sum = 0;

		for (size_t i = 0; i <point_set.size(); i++){
		
			CvPoint point_t = point_set.at(i);
			int x = point_t.x;//col
			int y = point_t.y;//row
			float data_t=cvGetReal2D(diff, y, x);
			ASSERT(PN*data_t>=0);
			sum += data_t;
		}

		count = point_set.size();

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
float ImageProcess::CRACK_get_histgram_area(std::vector<float> histgram,int WIDTH,int HEIGHT,int PN)
{
	
		const  unsigned long length = histgram.size();

		//step 2 计算面积

		float sum_hist = 0;

		for (unsigned long hi = 0; hi < length; hi++) {
			
			float hist_data = histgram.at(hi);
			
			if (hist_data>0){
					const float single_hist =hist_data *hi;
					sum_hist += single_hist;
			}


		}
		//step 3 计算理论总面积
		
		const float sum_max=1.0*(WIDTH*HEIGHT)*(WIDTH*HEIGHT)*255;

		//step 4 计算特征


		float feature_t = sum_hist / sum_max;

		
		assert(feature_t < 1 + 1E-6);

		return feature_t;


	
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
float GetAreaFeature(std::vector<float> histogram)
{
	const size_t length = histogram.size();
	const float histogram_max = Base::Math_GetMaxValueF(histogram.data(),histogram.size());
	//step 1 直方图修正
	std::vector<float> histogram_scale;

	for (size_t hi = 0; hi < length; hi++) {
		
		float new_value_t = 0;
		if (histogram_max != 0) {
			new_value_t = histogram.at(hi) / histogram_max;
		}else{
			new_value_t = 0;
		}

		histogram_scale.push_back(new_value_t);

	}	
	//step 2 计算面积

	float sum_hist = 0;

	for (size_t hi = 0; hi < length; hi++) {

		float single_hist = 1.0*histogram_scale.at(hi) *(1.0*hi / ImageProcess::CRACK_MAX_SIZE);
		sum_hist += single_hist;

	}
	//step 3 计算理论总面积
	const float m_width = length;
	float sum_max =1.0* (m_width - 1)*m_width / 2.0 / 1920;
	
	//step 4 计算特征


	float feature_t = sum_hist / sum_max;

	assert(sum_max > 0);
	assert(feature_t < 1 + 1E-6);

	return feature_t;


}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
//float GetAllArea(vector<float> histogram)
//{
//	size_t length=histogram.size();
//	float  sum=0;
//	for (size_t hi = 0; hi < length; hi++){
//		sum += 1.0*hi / ImageProcess::CRACK_MAX_SIZE*histogram.at(hi);
//	}
//
//	assert(sum < 1.0+1E-6);
//
//	return sum;
//
//
//}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
std::vector<float> ImageProcess::process_histogram(std::vector<float>& histogram,
												std::vector<std::vector<CvPoint>>& point_sets,
												std::vector<float>& delta_out,
												int HISTOGRAM_DIM,
												int width,
												int height)
{
#if TRUE
	std::vector<float> histogram_count;
	histogram.resize(HISTOGRAM_DIM, 0);
	histogram_count.resize(HISTOGRAM_DIM, 0);
	for (size_t si = 0; si < point_sets.size(); si++) {
		std::vector<CvPoint> point_set = point_sets.at(si);
		float one_line_sum = 0;
		int one_line_idx = 0;
		int count = GetLineProperty(point_set, delta_out, one_line_sum, one_line_idx);
		histogram[one_line_idx] += one_line_sum;
		histogram_count[one_line_idx] += count;
	}
#endif
#if TRUE
	for (size_t hi = 0; hi < HISTOGRAM_DIM; hi++)
	{
		if (histogram_count[hi] == 0) {
			histogram[hi] = 0;
		}
		else {
			histogram[hi] /= (255.0 * width*height);
		}

	}
#endif // TRUE

	std::vector<float> feature;
	int  min_left_pos = 0;
	float min_left_pos_value = 0;


	float * data_hist = histogram.data();
	std::vector<int> sort;

	sort.resize(2, 0);
	ImageProcess::GetMaxValueIndex(histogram.data(), histogram.size(), sort.data(), sort.size());
	const int max_pos = sort.at(0);//噪声峰值 or 左峰
	float max_value = histogram.at(max_pos);//噪声峰值 or 左峰
	int   max_left_pos = 0;
	float   max_left_value=0;

#if 0

	sort.resize(histogram.size() - max_pos, 0);
	ImageProcess::GetMinValueIndex(&data_hist[max_pos], histogram.size() - max_pos, sort.data(), sort.size());
	const int base_pos = max_pos;

	for (size_t i = 0; i <sort.size(); i++) {
		int pos = sort.at(i);

		if (histogram.at(pos + base_pos) > 0) {
			//噪声低谷  or （左峰本身，左锋低谷）
			min_left_pos = pos + base_pos;
			min_left_pos_value = histogram.at(pos + base_pos);
			break;
		}

	}
	sort.resize(2, 0);
	ImageProcess::GetMaxValueIndex(&data_hist[min_left_pos], histogram.size() - min_left_pos, sort.data(), sort.size());
	   max_left_pos = min_left_pos + sort.at(0);//  左峰值，即是伤痕
	float   max_left_value = histogram.at(max_left_pos);
	feature.push_back(1.0* max_left_pos / width);//  左峰值，即是伤痕
	feature.push_back(1.0*histogram.at(max_left_pos));//左峰值，即是伤痕
	if (max_value==0){
		feature.push_back(0);//主峰与次峰的比值
	}else{
		feature.push_back(1.0*max_left_value / max_value);//主峰与次峰的比值
	}
#endif // 0	


	for (int i = histogram.size()-1; i>=max_pos; i--){
		float value_t = histogram.at(i);
		if (value_t > max_value / 2) {
			max_left_pos = i;
			break;
		}
	}

	feature.push_back(1.0*max_pos / ImageProcess::CRACK_MAX_SIZE);//噪声峰值 or 左峰
	feature.push_back(1.0*histogram.at(max_pos));//噪声峰值 or 左峰
	
#if 0
	feature.push_back(1.0*max_left_pos / ImageProcess::CRACK_MAX_SIZE);//噪声峰值 or 左峰
#endif // 0
	feature.push_back(GetAreaFeature(histogram));
	





	return feature;
}
int ImageProcess::GetLineProperty(std::vector<CvPoint> point_set, std::vector<float> delta, float & sum_delta, int & idx)
{
	std::vector<float> x_count;
	std::vector<float>::iterator it;
	sum_delta = 0;

	for (size_t i = 0; i <point_set.size(); i++) {

		int x_p = point_set.at(i).x;
		int y_p = point_set.at(i).y;

		it = find(x_count.begin(), x_count.end(), x_p);

		if (it != x_count.end()) {
			//存在value值

		}
		else {
			//不存在value值
			x_count.push_back(x_p);
		}

		sum_delta += delta.at(x_p);//方差求和

	}

	idx = x_count.size();
	return point_set.size();
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
int ImageProcess::Save_Feature2TXT(std::vector<float> features,
	std::vector<INT32> classifies,
	int feature_dimension,
	std::string _feature_name,
	std::string file_base)
{
	assert(features.size() == classifies.size()*feature_dimension);
#if TRUE
	std::stringstream all_filename;
	all_filename << file_base << "\\" << _feature_name << classifies.size() << ".txt";//样本个数
	std::string output_full_name = all_filename.str();
	std::ofstream outfile;
	outfile.open(output_full_name); //myfile.bat是存放数据的文件名  

	outfile << std::setiosflags(std::ios::scientific);

	if (outfile.is_open()) {

		for (size_t i = 0; i <classifies.size(); i++) {


			for (size_t vi = 0; vi <feature_dimension; vi++) {
				outfile << features.at(i*feature_dimension + vi) << " ";
			}
			outfile << classifies.at(i) << std::endl;
		}

		outfile.close();
		return TRUE;
	}
	else {
		std::cout << "不能打开文件!" << std::endl;
		return FALSE;
	}
#endif // TRUE
	std::cout << "Save Feature Txt at : " << output_full_name << std::endl;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
std::vector<float> ImageProcess::crack_get_image_feature(IplImage *diff_org,std::string file_base,int frame_idx)
{
#if TRUE
	std::stringstream path_diff_out_ss;
	path_diff_out_ss << file_base << "\\"
		<< frame_idx << ".png";
	const std::string path_diff_out = path_diff_out_ss.str();

	CvSize diff_size = cvGetSize(diff_org);
	const int SCALE = 3;
	
	IplImage *image_out = cvCreateImage(cvSize(diff_size.width*SCALE,diff_size.height*SCALE), IPL_DEPTH_8U, 3);
	
	const CvScalar color_black = CV_RGB(0, 0, 0);
	const CvScalar color_blue = CV_RGB(0, 0, 255);
	const CvScalar color_yellow = CV_RGB(255,255, 0);
	const CvScalar color_red_64 = CV_RGB(64, 0, 0);
	const CvScalar color_red_128 = CV_RGB(128, 0, 0);
	const CvScalar color_red_255 = CV_RGB(255, 0, 0);
#endif // TRUE


	double Sum_delta[5];
	double a_col_sum_delta[4];

	memset(Sum_delta, 0, sizeof(Sum_delta));//统计求和
	std::vector<double> a_col_tmp;
	a_col_tmp.resize(diff_org->height, 0);//每列的暂存数据

	for (size_t coli = 0; coli <diff_org->width; coli++)
	{
		memset(a_col_sum_delta, 0, sizeof(a_col_sum_delta));//Col统计结果清零
#if TRUE
		for (size_t rowi = 0; rowi <diff_org->height; rowi++)
		{
			a_col_tmp[rowi] = cvGetReal2D(diff_org, rowi, coli);
		}
		//计算均值方差，统计结果
		double delta_t = 0, avg_t = 0;
		delta_t = Base::Math_GetVarianceValue(a_col_tmp.data(), diff_org->height, avg_t, &delta_t);
#endif // TRUE
#if TRUE
		CvScalar color_fill;
		for (size_t rowi = 0; rowi <diff_org->height; rowi++)
		{
			double diff_value = a_col_tmp[rowi];
			const double range[] = { 1 * delta_t,2 * delta_t,3 * delta_t };
			double test_value = fabs(diff_value - 0);
			
			if (test_value<=range[0]) {
				a_col_sum_delta[0]++;				
				color_fill=color_black;
			}
			else if (test_value > range[0] && test_value<=range[1]) {
				a_col_sum_delta[1]++;
				color_fill=color_blue;
			}
			else if (test_value > range[1] && test_value<=range[2]) {
				a_col_sum_delta[2]++;
				color_fill=color_yellow;
			}
			else if (test_value > range[2]) {
				a_col_sum_delta[3]++;
				color_fill = color_red_255;
			}
			else {
				assert(false);
			}

			for (size_t ri = 0; ri <SCALE; ri++){
				for (size_t ci = 0; ci <SCALE; ci++){
					cvSet2D(image_out, rowi*SCALE+ri, coli*SCALE+ci, color_fill);
				}
			}

		}
#endif // TRUE

		//统计整幅图像
		Sum_delta[0] += a_col_sum_delta[0];
		Sum_delta[1] += a_col_sum_delta[1];
		Sum_delta[2] += a_col_sum_delta[2];
		Sum_delta[3] += a_col_sum_delta[3];
		Sum_delta[4] += delta_t;

	}

	std::vector<float> feature_data_t;
	const float sum_pixel = diff_org->width*diff_org->height;
	assert(sum_pixel == Sum_delta[0] + Sum_delta[1] + Sum_delta[2] + Sum_delta[3]);
	for (size_t i = 1; i <4; i++)
	{
		feature_data_t.push_back(Sum_delta[i]/sum_pixel);
	}
	Sum_delta[4] = Sum_delta[4] / diff_org->width / 255;
	assert(Sum_delta[4] >= 0 && Sum_delta[4] <= 1+1E-6);

	feature_data_t.push_back(Sum_delta[4]);

#if TRUE
	if (file_base != "") {
		cvSaveImage(path_diff_out.c_str(),image_out);
	}

	cvReleaseImage(&image_out);
#endif // TRUE

	return feature_data_t;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void ImageProcess::Svm_Lean_F(std::string _featureFile, std::string _classifyFile, int _method, std::string _path, std::string _module_xml)
{
	  CvMat* mat_feature_t=(CvMat*) cvLoad(_featureFile.c_str());
	  CvMat* mat_classifyFile_t = (CvMat*)cvLoad(_classifyFile.c_str());

	  Svm_Lean_M(mat_feature_t,mat_classifyFile_t, _method, _path,_module_xml);

	  cvRelease((void **)&mat_feature_t);
	  cvRelease((void **)&mat_classifyFile_t);
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
int ImageProcess::Svm_Lean_Predict(CvSVM & _svm, std::vector<float> _feature)
{
	CvMat m;
	cvInitMatHeader(&m, 1, _feature.size(), CV_32FC1, _feature.data());
	int category = _svm.predict(&m);
	return category;

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
int ImageProcess::Svm_Lean_Save_Feature2XML(CvMat * _feature, CvMat * _classify, std::string _filename)
{
	return 0;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
int ImageProcess::Svm_Lean_Save_Feature2TXT(std::vector<float> features,
	std::vector<INT32> classifies,
	int feature_dimension,
	std::string file_base)
{
	return Save_Feature2TXT(features,classifies,feature_dimension,"svm.feature.",file_base);
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
int ImageProcess::Hough_Line_CV_STANDARD_TEST(CvArr * image,
	IplImage *color_dst,
	double rho,
	double theta,
	int threshold)
{
	CvMemStorage *storage = cvCreateMemStorage();
	CvSeq *lines = 0;
			
	lines = cvHoughLines2(image, storage, CV_HOUGH_STANDARD, rho,theta,threshold, 0, 0);

	for (int i = 0; i<lines->total; i++)
	{
		float *line = (float *)cvGetSeqElem(lines, i);
		float rho = line[0];
		float theta = line[1];
		CvPoint pt1, pt2;
		double a = cos(theta);
		double b = sin(theta);
		if (fabs(a)<0.001)
		{
			pt1.x = pt2.x = cvRound(rho);
			pt1.y = 0;
			pt2.y = color_dst->height;
		}
		else if (fabs(b)<0.001)
		{
			pt1.y = pt2.y = cvRound(rho);
			pt1.x = 0;
			pt2.x = color_dst->width;
		}
		else
		{
			pt1.x = 0;
			pt1.y = cvRound(rho / b);
			pt2.x = cvRound(rho / a);
			pt2.y = 0;
		}

		cvLine(color_dst, pt1, pt2, CV_RGB(255, 0, 0), 1, 8);
	}

	cvReleaseMemStorage(&storage);
	return 0;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
int ImageProcess::Hough_Line_CV_PROBABILISTIC_TEST(CvArr * image,
	IplImage *color_dst,
	double rho,
	double theta,
	int threshold,
	double _lineLenMin,
	double _lineGap)
{
	CvMemStorage *storage = cvCreateMemStorage();
	CvSeq *lines = 0;
	lines = cvHoughLines2(image, storage, CV_HOUGH_PROBABILISTIC, rho, theta,threshold, _lineLenMin,_lineGap);
	for (int i = 0; i<lines->total; i++)
	{
		CvPoint *line = (CvPoint *)cvGetSeqElem(lines, i);
		cvLine(color_dst, line[0], line[1], CV_RGB(255, 0, 0), 1, CV_AA);
	}
	cvReleaseMemStorage(&storage);
	return 0;
}
/*----------------------------------------------------------------*/
/**
*@note 查准率
*/
/*----------------------------------------------------------------*/
std::vector<Line2Point> ImageProcess::Hough_Line_CV_PROBABILISTIC_GetLine(
	CvArr * image,
	double rho,
	double theta,
	int threshold,
	double _lineLenMin,
	double _lineGap)
{
	std::vector<Line2Point> line_t;
	CvMemStorage *storage = cvCreateMemStorage();
	CvSeq *lines = 0;
	lines = cvHoughLines2(image, storage, CV_HOUGH_PROBABILISTIC, rho, theta, threshold, _lineLenMin, _lineGap);
	
	for (int i = 0; i<lines->total; i++)
	{
		CvPoint *line = (CvPoint *)cvGetSeqElem(lines, i);
		
		line_t.push_back(Line2Point(line[0], line[1]));
	}
	
	cvReleaseMemStorage(&storage);

	return line_t;

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void ImageProcess::LineCalAngle(std::vector<Line2Point>& _lines)
{
	for (auto& line : _lines)
	{
		line.CalTheta();
	}
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
std::vector<Line2Point> ImageProcess::LineRemove(
					std::vector<Line2Point>&  _lines,
					double _theta_c,
					double _theta_r)
{
		std::vector<Line2Point> lines_t;
		size_t remove_line_num_t = 0;
		
		double theta_min = _theta_c - _theta_r;
		double theta_max = _theta_c + _theta_r;

		for (const auto & line_t : _lines)
		{

			double theta_d = line_t.mTheta_d;

			assert(theta_d > -180 && theta_d <= 180);

			if (theta_d>theta_min &&theta_d<theta_max) {
#if _DEBUG
				remove_line_num_t++;
#endif // _DEBUG
			}
			else
			{
				lines_t.push_back(line_t);
			}
			
		}

	

	return lines_t;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void ImageProcess::LineDraw(std::vector<Line2Point>& _lines,
	IplImage * img_t,
	CvScalar _color,
	int _thickness)
{

	for (auto line_t: _lines)
	{
		cvLine(img_t,line_t.mP1,line_t.mP2,_color, _thickness);
	}

}
/*----------------------------------------------------------------*/
/**
*@note 查准率
*/
/*----------------------------------------------------------------*/
std::vector<Line2Point> ImageProcess::getHoughLines(IplImage * _img_bin,
	IplImage * _img_draw,
	CvScalar _color,
	int _thickness)
{
	const int R_DEGREE = 30;

	std::vector<Line2Point> lines_t = Hough_Line_CV_PROBABILISTIC_GetLine(_img_bin, 1, CV_PI / 180, 15, 6, 5);

	LineCalAngle(lines_t);

	std::vector<Line2Point> lines_remove0_t = LineRemove(lines_t, 90, R_DEGREE);
	std::vector<Line2Point> lines_remove1_t = LineRemove(lines_remove0_t, -90, R_DEGREE);

	if (_img_bin==_img_draw){
		cvZero(_img_draw);
	}

	if (_img_draw != nullptr) {
		LineDraw(lines_remove1_t, _img_draw, _color, _thickness);
	}

	return lines_remove1_t;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void ImageProcess::IMG_RemoveBorder(IplImage * _img_bin, std::vector<int> _row, std::vector<int> _col)
{
	uchar* const ImgData = (uchar *)_img_bin->imageData;
	const int STEP = _img_bin->widthStep / sizeof(uchar);
	const int Width = _img_bin->width;
	const int Height = _img_bin->height;

	ASSERT(Width == STEP);

	const int ROW = _row.size();
	const int COL = _col.size();

	for (size_t rii = 0; rii < ROW; rii++){
		const int ri = _row.at(rii);
		uchar * row_start = &ImgData[ri*STEP];
		memset(row_start,0,STEP);
	}
	
	for (size_t cii = 0; cii <COL ; cii++){
		const int ci = _col.at(cii);
		for (size_t ri = 0; ri <Height; ri++){
				ImgData[ci + ri*STEP] = 0;			
		}
	}

}
/*----------------------------------------------------------------*/
/**
*@note 查准率
*/
/*----------------------------------------------------------------*/
float ImageProcess::Svm_Lean_Precision(int tp, int fp, int fn)
{
	return 1.0f*tp/(tp+fp);
}
/*----------------------------------------------------------------*/
/**
*@note 查全率
*/
/*----------------------------------------------------------------*/
float ImageProcess::Svm_Lean_Recall(int tp, int fp, int fn)
{
	return 1.0f*tp/(tp+fn);
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
float ImageProcess::Svm_Lean_SAVE_Recall_Precision(int tp, int fp, int fn,std::string _full_path)
{
	float recall_t = Svm_Lean_Recall(tp, fp, fn);
	float Precision_t = Svm_Lean_Precision(tp, fp, fn);
	
	std::ofstream outfile;
	outfile.open(_full_path); //myfile.bat是存放数据的文件名  


	outfile << std::setiosflags(std::ios::scientific);

	if (outfile.is_open()) {

		outfile << "tp= " << tp << std::endl;
		outfile << "fp= " << fp << std::endl;
		outfile << "fn= " << fn << std::endl;
		outfile << "p= " << tp+fn << std::endl;


			
				
		outfile << "Precision(查准率)= " << Precision_t << " "
			<< "Recall(查全率)= " << recall_t << " " << std::endl;
		outfile.close();
	}
	else {
		std::cout << "不能打开文件!" << std::endl;

	}

	return 0.0f;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
int ImageProcess::SampleCalPos(int & _tp, int & _fn, int _sample, int _result)
{
	if (_sample==_result)
	{
		_tp++;
	}
	else
	{
		_fn++;
	}
	return 0;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
int ImageProcess::SampleCalNeg(int & _tn, int & _fp, int _sample, int _result)
{
	if (_sample == _result)
	{
		_tn++;
	}
	else
	{
		_fp++;
	}
	return 0;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void ImageProcess::Svm_Lean(std::vector<float> FeatureData,
	int FeatureDim,
	std::vector<INT32> FeatureClassify,
	int method,
	std::string path,
	std::string _module_xml)
{

	const unsigned long samplenum = FeatureClassify.size();//
	const std::string data_file = "svm.data.learn.xml";
	const std::string classify_file = "svm.classifies.learn.xml";
	static CvMat dataMatHeader;//
	static CvMat resMatHeader;//
		
	if (FeatureData.size()==0 || FeatureClassify.size()==0){
		

	}else{

		Svm_Lean_SAVE_VECTOR(FeatureData,FeatureDim,FeatureClassify,data_file,classify_file);

	}

	Svm_Lean_F(data_file, classify_file, method, path,_module_xml);


}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void ImageProcess::Svm_Lean_SAVE_MAT(
	CvMat* _data_mat,
	CvMat* _classify_mat,
	const std::string data_file,
	const std::string classify_file)
{

	cvSave(data_file.c_str(), _data_mat);
	cvSave(classify_file.c_str(), _classify_mat);

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void ImageProcess::Svm_Lean_SAVE_VECTOR(std::vector<float> FeatureData,
	int FeatureDim,
	std::vector<INT32> FeatureClassify,
	const std::string data_file,
	const std::string classify_file)
{

	const unsigned long samplenum = FeatureClassify.size();//
	
	static CvMat dataMatHeader;//
	static CvMat resMatHeader;//

	if (FeatureData.size() == 0 || FeatureClassify.size() == 0) {
	

	}
	else {
		CvMat* res_mat = cvInitMatHeader(&resMatHeader, samplenum, 1, CV_32SC1, FeatureClassify.data());
		CvMat* data_mat = cvInitMatHeader(&dataMatHeader, samplenum, FeatureDim, CV_32FC1, FeatureData.data());

		Svm_Lean_SAVE_MAT(data_mat, res_mat,data_file,classify_file);
		

	}
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void ImageProcess::Svm_Lean_M(CvMat * _data_mat,
	CvMat * _classify_mat,
	int _method, 
	std::string _path,
	std::string _module_xml)
{
#if (CV_MAJOR_VERSION==2)&&(CV_MINOR_VERSION==4)
		
	const int TotalClasses = 2;;
	
	CvSVM svm;
	CvSVMParams params;
	assert(_method >= 0 && _method<=3);

	if (_method == CvSVM::LINEAR) {
		//#ifdef SVM_USE_Linear
		params.svm_type = CvSVM::C_SVC;
		params.kernel_type = CvSVM::LINEAR;
		params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 1e5, FLT_EPSILON);
	}
	else if (_method == CvSVM::RBF) {
		//#ifdef SVM_USE_Gaussian
		params.svm_type = CvSVM::C_SVC;
		params.kernel_type = CvSVM::RBF;
		params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 1000, FLT_EPSILON);
#if 1
		params.gamma = 1.0 / TotalClasses;//增加容错，调小gamma
		params.C = 1;//增加容错，调小C;
#endif // 0
					 //params.gamma = 10;//增加容错，调小gamma
					 //params.C = 8;//增加容错，调小C;

	}
	else if (_method == CvSVM::POLY) {
		//#ifdef SVM_USE_Poly
		params.svm_type = CvSVM::C_SVC;
		params.kernel_type = CvSVM::POLY;
		params.term_crit = cvTermCriteria(CV_TERMCRIT_EPS, 1e5, FLT_EPSILON);
		params.degree = 2;//最高相次
		params.gamma = 1.0 / TotalClasses;//1/n_features
		params.coef0 = 0;
		params.C = 1;//增加容错，调小C 
	}
	else {

	}

	std::cout << "Start SVM Train !" << std::endl;
	svm.train(_data_mat, _classify_mat, NULL, NULL, params);//☆  
	std::cout << "END SVM Train !" << std::endl;
	//☆☆利用训练数据和确定的学习参数,进行SVM学习☆☆☆☆  
	
	Base::FS_checkUserPath_add_divide(_path);

	const	std::string svmsavepath =_path+ _module_xml;
	
	svm.save(svmsavepath.c_str(), 0);

#endif
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void ImageProcess::CuiResize(IplImage * src, IplImage * dst,const int m_step, const int n_step)
{

	const int m = 1.0*src->width / m_step;
	const int n = 1.0*src->height / n_step;
	assert(m==dst->width);
	assert(n==dst->height);
	const float w_step = 1.0*src->width /m;
	const float h_step = 1.0*src->height /n;
	
	std::vector<float> data_statistic;
	std::vector<float> data_statistic_count;	
	data_statistic.resize(m*n, 0);
	data_statistic_count.resize(m*n, 0);
	float*  data_statistic_p = data_statistic.data();
	float*  data_statistic_count_p = data_statistic_count.data();
	memset(data_statistic_p, 0, sizeof(float)*m*n);//清空内存
	memset(data_statistic_count_p, 0, sizeof(float)*m*n);//清空内存

	//统计
	for (size_t iw = 0; iw <src->width; iw++) {
		for (size_t ih = 0; ih <src->height; ih++) {

			const int m_idx = floor(1.0*iw / w_step);
			const int n_idx = floor(1.0*ih / h_step);
			const int idx_mn = m_idx + n_idx*m;
			const float data_pixel_f = cvGetReal2D(src, ih, iw);

			const uchar data_pixel = ((uchar *)(src->imageData + ih*src->widthStep))[iw];
			assert(data_pixel == data_pixel_f);
			assert(data_pixel >= 0 && data_pixel <= 255);

			data_statistic_p[idx_mn] += data_pixel;
			data_statistic_count_p[idx_mn]++;

		}
	}
	//转图像
	for (size_t im = 0; im <m; im++) {
		for (size_t in = 0; in <n; in++) {
			const int idx_mn = im + in*m;
			const float  gray_center = data_statistic_p[idx_mn] = data_statistic_p[idx_mn] / data_statistic_count_p[idx_mn];
#if 0
			uchar* pixel = (uchar*)(image_cut_statistic->imageData + in*image_cut_statistic->widthStep);
			pixel[im] = (uchar)gray_center;
#else
			cvSetReal2D(dst, in, im, gray_center);
#endif // 0
		}
	}
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void ImageProcess::DrawHistogram(float *data, int size,std::string file_base,int CHANNEL,int frame_idx, std::vector<float> feature)
{
	/////////////////////////////////////////////////////////////////	
	double max = GetMaxValue(&data[0],size);
	//////////////////////////////////////////////////////////////////
	const int bin_w = 1;
	const int h_bins = size;
	const int width = h_bins*bin_w;
	const int height = 1000;
	const CvScalar color_white = CV_RGB(255,255,255);
	const CvScalar color_black = CV_RGB(0, 0, 0);
	const CvScalar color_blue = CV_RGB(0, 0, 255);
	const CvScalar color_yellow = CV_RGB(255, 255, 0);
	/*const CvScalar color_red_64 = CV_RGB(64, 0, 0);
	const CvScalar color_red_128 = CV_RGB(128, 0, 0);*/
	const CvScalar color_red_255 = CV_RGB(255, 0, 0);
	CvScalar color_line= color_white;
	IplImage* hist_img = cvCreateImage(cvSize(width, height), 8, 3);
	cvRectangle(hist_img, cvPoint(0, 0), cvPoint(width, height),color_black, -1, 8, 0);
	char  text_buff_t[1024];
	/////////////////////////////////////////////////////////////////////////////////////
	for (int h = 0; h <h_bins; h++) {
		/** 获得直方图中的统计次数，计算显示在图像中的高度 */
		const float SCALE = 255 * 1E5/2;
		float bin_val =data[h]*SCALE;
		int  intensity = bin_val > height?height: bin_val;
		color_line = color_white;
#if FALSE
		if (bin_val <= height) {
			color_line=color_white;
		}else if (bin_val>height && bin_val<=2*height) {
			color_line=color_blue;
		}else if (bin_val>2*height && bin_val <=3*height) {
			color_line=color_yellow;
		}else {
			color_line=color_red_255;
		}
#endif // FALSE		

		/** 获得当前直方图代表的颜色，转换成RGB用于绘制 */
#if FALSE
		//cvRectangle(hist_img, cvPoint(h*bin_w, height), cvPoint((h + 1)*bin_w, height - intensity),CV_RGB(0,255,0), 2);
		//cvRectangle(hist_img, cvPoint(h*bin_w, height), cvPoint((h + 1)*bin_w, height - intensity + 1), color,CV_FILLED);	
#endif 
		cvLine(hist_img,cvPoint(h*bin_w, height), cvPoint(h*bin_w, height - intensity + 1), color_line);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if 0
		CvFont font;
		cvInitFont(&font, CV_FONT_VECTOR0, 0.3, 0.3, 0, 1, 8);
		sprintf(text_buff_t, "%d", h);
		cvPutText(hist_img, text_buff_t, cvPoint(h*bin_w, height), &font, CV_RGB(255, 255, 255));
#endif // 0
		//1,3
#if TRUE
		int max0 = round(feature.at(1)*size);
		//int max1 = round(feature.at(3)*size);
		
		if (max0 == h) {
			color_line= color_red_255;
			cvLine(hist_img, cvPoint(h*bin_w, height), cvPoint(h*bin_w, height - 0.4*intensity + 1), color_line);
		}
		/*if (max1 == h) {
			color_line = color_yellow;
			cvLine(hist_img, cvPoint(h*bin_w, height - 0.4*intensity + 1), cvPoint(h*bin_w, height - 0.8*intensity + 1), color_line);
		}		*/
#endif // 0

		
		

	}
	
	
	std::stringstream sub2_path_ss;
	sub2_path_ss << "histogram";
	
	std::string sub2_path_str = Base::FS_createPath(file_base, sub2_path_ss.str());
	std::string filesaveimg = sub2_path_str + Base::int2str(frame_idx) + ".png";


	cvSaveImage(filesaveimg.c_str(), hist_img);//在"H-S Histogtam"窗口中显示图像
	cvReleaseImage(&hist_img);
#if FALSE
	SaveArray2Disk(data, size, CHANNEL, frame_idx, sub2_path_str);	
#endif

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void ImageProcess::DrawHistogram_fromImage(IplImage * img, std::string file_base, int CIRCLE, int CHANNEL, int frame_idx,int ColIdx,int hist_bar_height,int hist_bar_width)
{
	/////////////////////////////////////////////////////////////////	
	const int size = img->height;
	assert(img->nChannels == 1);
	//////////////////////////////////////////////////////////////////
	const int bin_w = hist_bar_width;
	const int h_bins = size;
	const int HEAD = 20;
	const int width = h_bins*bin_w;
	const int height = hist_bar_height+HEAD;
	const CvScalar color_white = CV_RGB(255, 255, 255);
	const CvScalar color_black = CV_RGB(0, 0, 0);
	const CvScalar color_blue = CV_RGB(0, 0, 255);
	const CvScalar color_yellow = CV_RGB(255, 255, 0);
	const CvScalar color_red_255 = CV_RGB(255, 0, 0);
	CvScalar color_line = color_white;
	IplImage* hist_img = cvCreateImage(cvSize(width, height),IPL_DEPTH_8U, 3);
	cvRectangle(hist_img, cvPoint(0, 0), cvPoint(width, height), color_black, -1, 8, 0);
	char  text_buff_t[1024];
	/////////////////////////////////////////////////////////////////////////////////////
	for (int hi = 0; hi <h_bins; hi++) {
		/** 获得直方图中的统计次数，计算显示在图像中的高度 */
		
		float bin_val = cvGetReal2D(img, hi, 0);
		int  intensity = bin_val > height ? height : bin_val;
		
		color_line = color_white;
		cvLine(hist_img, cvPoint(hi*bin_w, height), cvPoint(hi*bin_w, height - intensity + 1), color_line);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}

#if TRUE
	std::vector<float> data_t;
	//获得数据
	for (size_t ri = 0; ri <size; ri++){
		float bin_val = cvGetReal2D(img, ri, 0);
		data_t.push_back(bin_val);
	}
	
	const float avg_0 =Base::Math_GetAverageValueF(data_t.data(),data_t.size());
	float sigma_0 = 0;	Base::Math_GetVarianceValueF(data_t.data(),data_t.size(),avg_0,&sigma_0);
	const float  avg_0_3sigma = avg_0 + 3 * sigma_0;

	color_line = color_red_255;
	cvLine(hist_img, cvPoint(0, height-avg_0), cvPoint(width, height-avg_0), color_line);
	
	color_line = color_yellow;
	cvLine(hist_img, cvPoint(0, height - avg_0_3sigma), cvPoint(width, height - avg_0_3sigma), color_line);

#endif // TRUE

	
#if TRUE

	for (size_t ri = 0; ri <HEAD; ri++){
		
		for (size_t ci = 0; ci <width; ci++){

			assert(width == data_t.size());
			const int GRAY = data_t.at(ci);
			cvSet2D(hist_img,ri,ci,CV_RGB(GRAY, GRAY, GRAY));
		}

	}

#endif // TRUE


	
#if TRUE
	  std::string ColImageName;
	 
	  ColImageName.append("circle").append(Base::int2str(CIRCLE)).append(".")
		  .append("ch").append(Base::int2str(CHANNEL)).append(".")
		  .append("frame").append(Base::int2str(frame_idx)).append(".")
		  .append("col").append(Base::int2str(ColIdx)).append(".hist.png");

	const std::string ColSavePath_full = file_base + ColImageName;//列号 作为文件名
	cvSaveImage(ColSavePath_full.c_str(), hist_img);//在"H-S Histogtam"窗口中显示图像
#endif
	
	
	cvReleaseImage(&hist_img);


}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void ImageProcess::DrawBox(CvBox2D rect,IplImage * _img,const CvScalar color)
{
	
	const int thickness = 1;
	const int line_type = 8;
	
	CvPoint2D32f Corners[4]; 
	
	cvBoxPoints(rect, Corners); 
	//转换为矩形的四个顶点 /****由于Opencv 没有自带画倾斜矩形，所以画四条矩形边来代替****/ 
	for (int i = 0; i < 3; i++)
		cvLine(_img,
			cvPoint(int(Corners[i].x),int(Corners[i].y)),
			cvPoint(int(Corners[i+1].x),int(Corners[i+1].y)),
			color,
			thickness,
			line_type);
	
	cvLine(_img,
		cvPoint(int(Corners[3].x),int(Corners[3].y)),
		cvPoint(int(Corners[0].x),int(Corners[0].y)),
		color,
		thickness,
		line_type);

	cvEllipseBox(_img, rect, color, thickness, line_type);
	
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void ImageProcess::SaveArray2Disk(float * data, int size,int channel_t,int frame_count, std::string file_base)
{
	std::stringstream ss;
	ss << file_base <<frame_count<<"."<<channel_t<< ".hist.txt";

	std::string myfile_path=ss.str();
	std::ofstream myfile(myfile_path);

	for (size_t i = 0; i < size; i++){

		myfile << i << " " << data[i] << std::endl;

	}

	myfile.close();
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void ImageProcess::Opencv_SaveVector2CvMatrix(std::string file_name,
	std::vector<float> vf)
{
	const int DIM = vf.size();

	CvMat *mat = cvCreateMat(1, DIM, CV_32FC1);

	for (size_t ci = 0; ci <DIM; ci++){
		cvSetReal2D(mat, 0, ci, vf[ci]);
	}

	cvSave(file_name.c_str(), mat);

	cvReleaseMat(&mat);
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
float ImageProcess::GetMaxValue(float* Data, long DataNum)
{
	float *Data_cp = new float[DataNum];
	float  max_value;
	memcpy(Data_cp, Data, sizeof(float)*DataNum);
	std::sort(Data_cp, Data_cp + DataNum, std::greater<float>());
	max_value = Data_cp[0];
	delete[]Data_cp;
	return  max_value;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void ImageProcess::GetMaxValueIndex(float * data, float size, int * sort, int sort_num)
{
#if 1
	float* data_t = new float[(long long)size];
	memcpy(data_t, data, sizeof(float)*((long long)size));
	/*****寻找最值***************************************************************/
	for (int sj = 0; sj<sort_num; sj++) {

		float max_value = data[0];
		int max_value_i = 0;
		/*******************************/
		for (register int i = 0; i<size; i++) {
			if (data_t[i]>max_value) {
				max_value = data_t[i];
				max_value_i = i;
			}
		}
		/*******************************/
		sort[sj] = max_value_i;
		data_t[max_value_i] = FLT_MIN;

	}
	/*******************************************************************************/
	delete[]data_t;
#else

#endif
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void ImageProcess::GetMinValueIndex(float * data, float size, int * sort, int sort_num)
{
	float* data_t = new float[(long long)size];
	memcpy(data_t, data, sizeof(float)*((long long)size));
	/*****寻找最值***************************************************************/
	for (int sj = 0; sj<sort_num; sj++) {

		float min_value = data[0];
		int min_value_i = 0;
		/*******************************/
		for (register int i = 0; i<size; i++) {
			if (data_t[i]<min_value) {
				min_value = data_t[i];
				min_value_i = i;
			}
		}
		/*******************************/
		sort[sj] = min_value_i;
		data_t[min_value_i] = FLT_MAX;

	}
	/*******************************************************************************/
	delete[]data_t;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
int ImageProcess::GetOneColumn(IplImage * image, IplImage * ColData, int IdxCol)
{
	const size_t ci=IdxCol ;

	assert(image->nChannels == ColData->nChannels);

	for (size_t ri = 0; ri < image->height; ri++){
		CvScalar color_data=cvGet2D(image, ri, ci);

		for (size_t dstci = 0; dstci <ColData->width; dstci++){
			cvSet2D(ColData,ri, dstci, color_data);
		}

	}

	return TRUE;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void ImageProcess::VIDEO_GetWidthHeight(std::string video_full_path, int* WIDTH, int* HEIGHT)
{
#if TRUE

	{//init width and height
		CvCapture* capture = cvCaptureFromAVI(video_full_path.c_str());

		//init 
		if (cvGrabFrame(capture)) {

			cv::Mat matimg = cvRetrieveFrame(capture);        // retrieve the captured frame
			IplImage iplimg = matimg.operator _IplImage();
			

			*WIDTH =iplimg.width;
			*HEIGHT =iplimg.height;


		}

		cvReleaseCapture(&capture);
		capture = NULL;
	}
#endif // TRUE
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/