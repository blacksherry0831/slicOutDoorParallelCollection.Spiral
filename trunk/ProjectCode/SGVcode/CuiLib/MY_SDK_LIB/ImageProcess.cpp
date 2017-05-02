#include "StdAfx.h"
#include "ImageProcess.h"
#include "Base.h"
#include "modules.h"

#ifdef min
#undef min
#endif

#define HOUGH_USE_CANNY  TRUE
#define ANGLE_UP (-10)
#define ANGLE_DOWN (-54)
#define ANGLE_DELTA (0)
#define ANGLE_HIST_NUM (11)
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

	//����ɫ�Ҷ�ͼ��

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
				image_Y_sum[i]+=data[i*step+j];  //�ڸ߶ȷ����ۼ�
#endif		
		adjustFloatMatrix2_100000(image_temp->height,image_Y_sum);

#if _DEBUG
		{
			char data_t[1024];
			ofstream outfile;
			outfile.open(Base::comine_str(readfile_name_t,"Matrix_Y_SUM.txt").c_str(),ios::out);
			for(register int i = 0; i <image_temp->height; i++ ){

				float value_t=image_Y_sum[i];
				sprintf(data_t," %f ",value_t);
				outfile<<i;
				outfile<<' ';
				outfile<<data_t;
				outfile<<endl;
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
void ImageProcess::SaveCutImage(IplImage* img_t,CvRect cut_t,string readfile_name_t)
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

			//��̬�ֲ����������������������������̺ͱ�׼��ң���^2Ϊ���

			double     mean_t = mean.val[0];  //��ֵ
			double     stddev_t = std_dev.val[0]; //��׼��

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
			//��̬�ֲ����������������������������̺ͱ�׼��ң���^2Ϊ���
			double     mean_t = mean.val[0];  //��ֵ
			double     stddev_t = std_dev.val[0]; //��׼��
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
    int circles_max = INT_MAX;    //������Բ�ε���������Ϊ�����
   
#if 1
	//canny��Ե�����˫��ֵ�еĸ���ֵ
    int canny_threshold = cvRound(param1);
#endif
    //�ۼ�����ֵ
    int acc_threshold = cvRound(param2);

    img = cvGetMat( img, &stub );
    //ȷ������ͼ���ǻҶ�ͼ��
    if( !CV_IS_MASK_ARR(img))
        CV_Error( CV_StsBadArg, "The source image must be 8-bit, single-channel" );
    //�ڴ�ռ��Ƿ����
    if( !circle_storage )
        CV_Error( CV_StsNullPtr, "NULL destination" );
    //ȷ����������ȷ��
#if 1
    if( dp <= 0 || min_dist <= 0 || canny_threshold <= 0 || acc_threshold <= 0 )
        CV_Error( CV_StsOutOfRange, "dp, min_dist, canny_threshold and acc_threshold must be all positive numbers" );
#endif
    //Բ����С�뾶Ҫ����0
    min_radius = MAX( min_radius, 0 );
    //Բ�����뾶���С��0���������뾶Ϊͼ���ͳ��ȵ����ֵ��
    //������뾶С����С�뾶���������뾶Ϊ��С�뾶���������صĿ��
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
    //ѡ�������㷨���Բ��Ŀǰֻ��2-1����任
    switch( method )
    {
    case CV_HOUGH_GRADIENT:
        //����icvHoughCirclesGradient����
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
    //���Բ
    return result;
}

#define hough_cmp_gt(l1,l2) (aux[l1] > aux[l2])

static CV_IMPLEMENT_QSORT_EX( icvHoughSortDescent32s, int, hough_cmp_gt, const int* )

void ImageProcess::icvHoughCirclesGradient( CvMat* img, float dp, float min_dist,
                         int min_radius, int max_radius,
                         int canny_threshold, int acc_threshold,
                         CvSeq* circles, int circles_max )
{
//������
//img: ����ͼ��
//dp: ʶ�𾫶�,1.0��ʾ����ԭͼ����
//min_dist: Բ�ĵ�λ��ʶ�𾫶�
//min_radius: ����Ҫ�ҵ�Բ����С�뾶
//max_radius������Ҫ�ҵ�Բ�����뾶
//canny_threshold��canny���ӵĸ߷�ֵ
//acc_threshold���ۼ�����ֵ���������ڸķ�ֵ�ĵ㼴����Ϊ�ǿ��ܵ�Բ��
//circles: �����ҵ��ķ�������������Բ
//circles_max: �����Ҫ���ҵ���Բ�ĸ���

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
	
	//canny���������ض�ֵ����Ե��������edges������
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

	//sobel������ˮƽ�ʹ�ֱ����ı�Ե�����ڼ����Ե��ķ��߷���
	dx = cvCreateMat( img->rows, img->cols, CV_16SC1 );
	dy = cvCreateMat( img->rows, img->cols, CV_16SC1 );
	cvSobel( img, dx, 1, 0, 3 );
	cvSobel( img, dy, 0, 1, 3 );
	
	//dp��ʾʶ�𾫶�
	if( dp < 1.f )
		dp = 1.f;
	idp = 1.f/dp;
	
	//accum�����ۼ���������ͼ����ÿһ����ļ�����ͼ����ÿһ���㶼��һ����������ļ�����ʾÿһ��canny��Ե�㷨�߷����ϣ�
	//���õ����ΪR�ı�Ե��ĸ�������ʼ��Ϊ0
	accum = cvCreateMat( cvCeil(img->rows*idp)+2, cvCeil(img->cols*idp)+2, CV_32SC1 );
	cvZero(accum);
	
	storage = cvCreateMemStorage();
	nz = cvCreateSeq( CV_32SC2, sizeof(CvSeq), sizeof(CvPoint), storage );
	
	//centers���ڱ�����ܵ�Բ�ĵ�
	centers = cvCreateSeq( CV_32SC1, sizeof(CvSeq), sizeof(int), storage );
	
	rows = img->rows;
	cols = img->cols;
	arows = accum->rows - 2;
	acols = accum->cols - 2;
	adata = accum->data.i;
	astep = accum->step/sizeof(adata[0]);
	
	//�������ѭ�����ڻ�ȡ���п��ܵ�Բ��Ե�㣬�洢��nz�У�ͬʱ����
	//�ۼ����е�ֵ
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
			
			//sx��ʾcos, sy��ʾsin
			sx = cvRound((vx*idp)*ONE/mag);
			sy = cvRound((vy*idp)*ONE/mag);
			
			x0 = cvRound((x*idp)*ONE);
			y0 = cvRound((y*idp)*ONE);
			
			//ѭ�����α�ʾ��Ҫ�����������򣬷��߷���ͷ��ߵķ�����
			for( k = 0; k < 2; k++ )
			{
				//�뾶�����ˮƽ�����ʹ�ֱ����
				x1 = x0 + min_radius * sx;
				y1 = y0 + min_radius * sy;
				
				//�ڷ��߷���ͷ������ϣ������Ե��ľ���Ϊ��������뾶����С�뾶��Χ���ҵ�
				//ÿ�ҵ�һ���㣬�õ���ۼ��������ͼ�1
				for( r = min_radius; r <= max_radius; x1 += sx, y1 += sy, r++ )
				{
					int x2 = x1 >> SHIFT, y2 = y1 >> SHIFT;
					if( (unsigned)x2 >= (unsigned)acols || (unsigned)y2 >= (unsigned)arows )
						break;
					adata[y2*astep + x2]++;
				}
				//������
				sx = -sx; sy = -sy;
			}
			
			//������ܵ�Բ��Ե��
			pt.x = x; pt.y = y;
			cvSeqPush( nz, &pt );
		}
	}

	nz_count = nz->total;
	if( !nz_count )
		return;
	
	//�ۼ����У��������ڷ�ֵ�ĵ㣬����Ϊ���ܵ�Բ�ĵ㡣��Ϊ���������������У�������������
	//���뷧ֵ�Ƚ�ʱ���õ������Ҫ��4�����ڵĸ�����ļ����Ƚϣ�����߲��ܺͷ�ֵ�Ƚϡ����ܵ�Բ��
	//�㱣����centers��
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
	
	//����ṹ��certersת���������洢�ṹsort_buf
	cvCvtSeqToArray( centers, &sort_buf[0] );
	
	//����icvHoughSortDescent32s��������sort_buf��Ԫ����Ϊadata�����±�, 
	//adata�е�Ԫ�ؽ�������, ��adata[sort_buf[0]]��adata����Ԫ��������, 
	//adata[sort_buf[center_count-1]]������Ԫ������С��
	icvHoughSortDescent32s( &sort_buf[0], center_count, adata );
	
	cvClearSeq( centers );
	
	//����������Ԫ�أ�������������ʽ�洢��centers��
	cvSeqPushMulti( centers, &sort_buf[0], center_count );
	
	dist_buf = cvCreateMat( 1, nz_count, CV_32FC1 );
	ddata = dist_buf->data.fl;
	
	dr = dp;
	min_dist = MAX( min_dist, dp );
	min_dist *= min_dist;
	
	//����ÿһ�����ܵ�Բ�ĵ㣬�������б�Ե�㵽��Բ�ĵ�ľ��롣����centers�е�
	//Ԫ���Ѿ�����ǰ�����������ۼ����������Ŀ���Բ�ĵ����Ƚ�������Ĳ���
	for( i = 0; i < centers->total; i++ )
	{
		int ofs = *(int*)cvGetSeqElem( centers, i );
		y = ofs/(acols+2) - 1;
		x = ofs - (y+1)*(acols+2) - 1;
		float cx = (float)(x*dp), cy = (float)(y*dp);
		float start_dist, dist_sum;
		float r_best = 0, c[3];
		int max_count = R_THRESH;
		
		//����ÿ��ܵ�Բ�ĵ���Ѿ�ȷ�ϵ�Բ�ĵ�ľ���С�ڷ�ֵ�����ʾ
		//���Բ�ĵ���Ѿ�ȷ�ϵ�Բ�ĵ���ͬһ����
		for( j = 0; j < circles->total; j++ )
		{
			float* c = (float*)cvGetSeqElem( circles, j );
			if( (c[0] - cx)*(c[0] - cx) + (c[1] - cy)*(c[1] - cy) < min_dist )
				break;
		}

		if( j < circles->total )
			continue;
		
		cvStartReadSeq( nz, &reader );
		
		//�����б�Ե�㵽��ǰԲ�ĵ�ľ��룬���������ľ���ֵ������ddata��
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
		
		//�������´������sort_buf��Ԫ����Ϊddata�����±�,ddata�е�Ԫ�ؽ�������, 
		//��ddata[sort_buf[0]]��ddata����Ԫ��������, ddata[sort_buf[nz_count1-1]]
		//������Ԫ������С��
		icvHoughSortDescent32s( &sort_buf[0], nz_count1, (int*)ddata );
		
		//�����еľ���ֵ��������������Բ�뾶ֵ��max_countΪ��Բ�ĵľ���Ϊ����ܰ뾶ֵ�ĵ�ĸ���
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
		//max_count���ڷ�ֵ����ʾ�⼸����Ե�㹹��һ��Բ
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
void ImageProcess::zhangjiagang_hongbao_duanzao_rgb(string filename)
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
void  ImageProcess::wait_for_show_image(string window_name,IplImage* img_t)
{
#if _DEBUG
	cvNamedWindow( window_name.c_str(), 1 );
	cvShowImage(window_name.c_str(), img_t );

	string file_name_t=window_name+".png";
	cvSaveImage(file_name_t.c_str(),img_t);
	cvWaitKey(10);
	cvWaitKey(0);
#endif
}
/*--------------------------------------------------------------*/
/**
*
*
*/
/*--------------------------------------------------------------*/
void ImageProcess::zhangjiagang_hongbao_duanzao(string filename)
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
	CvSeq* seq=find_max_contour_adjust_binary(src_binary);
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
			}else{
				cvCopy(src_gary,src_gary_cut);
				cvCopy(src_color,src_color_cut);
			}

#if _DEBUG
wait_for_show_image("src_binary_cut",src_binary_cut); 
wait_for_show_image("src_gary_cut ",src_gary_cut); 
#endif

//	equalizeHist_by_mask(src_gary_cut,src_binary_cut);
//#if _DEBUG
//	wait_for_show_image("src_gary_equalizeHist_cut",src_gary_cut);
//#endif


	gary_by_angle(src_color_cut,src_gary_cut,src_binary_cut);

	canny_by_mask(src_color_cut,src_gary_cut,src_binary_cut);

	//Laplace_by_mask(src_gary_cut,src_binary_cut);
//	threshold_binary(src_gary_cut,src_binary_cut);
//#if _DEBUG
//	wait_for_show_image("src_gary_bin2",src_gary_cut);
//#endif
	//grayCenter(src_gary_cut,src_binary_cut);

			cvResetImageROI(src_color);
			cvResetImageROI(src_binary);
			cvResetImageROI(src_gary);
		}
		cvReleaseImage(&src_gary_cut);
		cvReleaseImage(&src_color_cut);
		cvReleaseImage(&src_binary_cut);

	}


	




	/*-----------------------*/

	//cvReleaseImage(&src_gary_mask);
	cvReleaseImage(&src_gary);
	cvReleaseImage(&src_color);
	cvReleaseImage(&src_binary);
}
/*--------------------------------------------------------------*/
/**
*
*
*/
/*--------------------------------------------------------------*/
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
			 //�� ��ɫ
				CvScalar s_b;
				s_b.val[0]=0;s_b.val[1]=0;s_b.val[2]=0;
				cvSet2D(src_binary,yi,xi,s_b);
				
			}else{
			//����ɫ
				CvScalar s_w;
				s_w.val[0]=255;s_w.val[1]=255;s_w.val[2]=255;
				cvSet2D(src_binary,yi,xi,s_w);
			}
#else
			double new_a=a-255/2.0;
			double new_b=b-255/2.0;

			if(new_a==new_b){
				//�Ҷ�ϵ��
				if(l>Light){
					CvScalar s_w;//��ɫ
						s_w.val[0]=255;s_w.val[1]=255;s_w.val[2]=255;
						cvSet2D(src_binary,yi,xi,s_w);
				}			
			}else{
				//��ɫϵ��
					double arc=atan2(new_b,new_a)*180/CV_PI;
					if((arc>ANGLE_DOWN-Delta)&&(arc<ANGLE_UP+Delta )&&(l>Light)){						
						CvScalar s_w;//��ɫ
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
CvSeq* ImageProcess::find_max_contour_adjust_binary(IplImage* src_binary_t)
{
   IplImage* src=cvCloneImage(src_binary_t);
#if _DEBUG
   IplImage* des=cvCreateImage(cvGetSize(src),src->depth,3);
   cvZero(des);
#endif

   
   CvMemStorage* memory=cvCreateMemStorage(0);
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
#if _DEBUG
    cvDrawContours(des, maxContour,
    CV_RGB(255,255,255), CV_RGB(255, 255,255), 0, 1, 8, cvPoint(0,0));
   //CvRect rect=cvBoundingRect(maxContour,0);
   CvBox2D box=cvMinAreaRect2(maxContour);
   cout<<"���ȣ� "<<box.size.width<<endl<<"��ȣ� "<<box.size.height;
   //cvRectangle(src,cvPoint((rect.x-rect.height/2),(rect.y-rect.width/2)),cvPoint((rect.x+rect.height/2),(rect.y+rect.width/2)),cvScalar(255,255,255),1,8,0);
  // cvRectangle(src,cvPoint((rect.x-rect.height/2),(rect.y-rect.width/2)),cvPoint((rect.x-rect.height/2),(rect.y-rect.width/2)),cvScalar(255,255,255),2,8,0);
  // cvDrawCircle(src,cvPoint(box.center.x,box.center.y),box.size.height,cvScalar(255,255,255),2,8,0);
   
 
     CvPoint2D32f p4[4];
     cvBoxPoints(box,p4);
     cvLine(des, cvPoint(cvRound(p4[0].x), cvRound(p4[0].y)),
     cvPoint(cvRound(p4[1].x), cvRound(p4[1].y)), CV_RGB(0, 0, 255),2);
       
     cvLine(des, cvPoint(cvRound(p4[1].x), cvRound(p4[1].y)),
     cvPoint(cvRound(p4[2].x), cvRound(p4[2].y)), CV_RGB(0, 0, 255),2);
       
     cvLine(des, cvPoint(cvRound(p4[3].x), cvRound(p4[3].y)),
     cvPoint(cvRound(p4[2].x), cvRound(p4[2].y)), CV_RGB(0,0, 255),2);
       
     cvLine(des, cvPoint(cvRound(p4[3].x), cvRound(p4[3].y)),
     cvPoint(cvRound(p4[0].x), cvRound(p4[0].y)), CV_RGB(0, 0,255),2);
       

	  CvRect rect = cvBoundingRect(maxContour,1);
	  cvRectangle(des, cvPoint(rect.x, rect.y), cvPoint(rect.x + rect.width, rect.y + rect.height),CV_RGB(255, 0, 0), 2, 8, 0);
	  	  
		try {  
			CvBox2D ellipse = cvFitEllipse2(maxContour);//��С���˷�����Բ���    
			cvEllipseBox(des,ellipse,CV_RGB(255,255,0),2 ); //��ͼ�ϻ���Բ  
		}catch(cv::Exception& exn) {
			printf("%s\n",exn.msg.c_str());
		}
	  
	  

		wait_for_show_image("max_contour",des);
		cvReleaseImage(&des);

#endif
   
    
	cvZero(src_binary_t);
	cvDrawContours(src_binary_t, maxContour,
    CV_RGB(255,255,255), CV_RGB(255, 255,255), 0,CV_FILLED );

	wait_for_show_image("src_bin_adjust",src_binary_t);
	cvReleaseImage(&src);  

   return maxContour;
}
/*--------------------------------------------------------------*/
/**
*
*
*/
/*--------------------------------------------------------------*/
//void ImageProcess::equalizeHist_by_mask(IplImage* img_gary,CvRect mask_rect)
//{
//	cvSetImageROI(img_gary, mask_rect);//����ROI  
//	{
//		cvEqualizeHist(img_gary,img_gary);	
//#if _DEBUG
////		 wait_for_show_image("img_eq",img_gary);
//#endif
//	
//	}
//	cvResetImageROI(img_gary);//���ͷ�ROI������ֻ����ʾROI����
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
	cvSetImageROI(img_gary, mask_rect);//����ROI  
	{
		cvEqualizeHist(img_gary,img_gary);	
#if _DEBUG
//		 wait_for_show_image("img_eq",img_gary);
#endif
	
	}
	cvResetImageROI(img_gary);//���ͷ�ROI������ֻ����ʾROI����

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
    // ����ֱ��ͼ
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

 // ����ֲ�����(Ҳ���Ǳ任����f(x))
	int numberOfPixel = img_gary->width*img_gary->height;
    // �Ҷȱ任
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
			//��Ч����
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
    //����ֱ��ͼ
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
			//��Ч����
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
void ImageProcess::canny_by_mask(IplImage* src_color_t,IplImage* img_gary,IplImage* mask_img)
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
	wait_for_show_image("Canny org",TheImage);
#endif
	int threshold1=10;
	int threshold2=20;
	cvCanny(TheImage,TheImage , threshold1, threshold2);

#if _DEBUG
	wait_for_show_image("Canny result",TheImage);
#endif
#if _DEBUG
		
		uchar* cannyData=(uchar *)(TheImage->imageData);
		//const int pixels=img_gary->width* img_gary->height;

		for(int xi=0;xi<img_gary->width;xi++){
			
			for(int yi=0;yi<img_gary->height;yi++){
				
				CvScalar s = cvGet2D(TheImage, yi,xi);
				int canny_t=s.val[0];
					
				if(canny_t>0){
							CvScalar s_w;//������
								s_w.val[0]=0;
									s_w.val[1]=255;
										s_w.val[2]=255;
								cvSet2D(src_color_t,yi,xi,s_w);
					}			
			}

		}
		wait_for_show_image("Canny color result",src_color_t);
#endif
	cvReleaseImage(&TheImage);
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
		//û����Ĥ
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
	//�Ҷ����ķ�������
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
void ImageProcess::gary_by_angle(IplImage* src_color_t,IplImage* src_gary_t,IplImage* mask_img)
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
				//�Ҷ�ϵ��
				if(l>Light){
					CvScalar s_w;//������
						s_w.val[0]=255;
						cvSet2D(src_gary_t,yi,xi,s_w);
				}else{
					CvScalar s_w;//��
						s_w.val[0]=0;
						cvSet2D(src_gary_t,yi,xi,s_w);
				}			
			}else{
				//��ɫϵ��
				const int a_down=ANGLE_DOWN-Delta;
				const int a_up=ANGLE_UP+Delta;
				const int hist_num=ANGLE_HIST_NUM;
				const double arc=atan2(new_b,new_a)*180/CV_PI;

					if((arc>a_down)&&(arc<a_up)){

						int hist_pos=1.0*(a_up-arc)/(1.0*(a_up-a_down)/hist_num)+1;

						int hist_light=hist_t[hist_pos];
						
						if(hist_light>255) hist_light=255;
							if(hist_light<0) hist_light=0;

						CvScalar s_w;//��ɫ
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
				//�Ҷ�ϵ��
				if(l>Light){
					CvScalar s_w;//������
						s_w.val[0]=255;
						//cvSet2D(src_gary_t,yi,xi,s_w);
				}else{
					CvScalar s_w;//��
						s_w.val[0]=0;
						//cvSet2D(src_gary_t,yi,xi,s_w);
				}			
			}else{
				//��ɫϵ��
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
*����������ֵ������������
*
*@param [in] data ����
*@param size  ���鳤��
*@param [out] sort �������ֵ����
*@param sort_num ���������С
*
*/
/*----------------------------------------------------------------*/
int ImageProcess::GetMaxValueIndexdouble(
	double* data, 
	double size)
{
	double* data_t=new double[(LONGLONG)size];
	memcpy(data_t,data,sizeof(double)*((LONGLONG)size));
	/*****Ѱ����ֵ***************************************************************/
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