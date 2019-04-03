#include "StdAfx.h"
#include "CrackDetection.hpp"
#include <MY_SDK_LIB/Base.h>
#include <MY_SDK_LIB/ImageProcess.h>
#include <MY_SDK_LIB/TimeMeasure.hpp>
#include <MY_SDK_LIB/BlockAnalysis.hpp>
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
#define NOMINMAX
#undef max
#undef min
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
const std::string  CrackDetection::CRACK_OUT("out");
const std::string  CrackDetection::CRACK_IN("in");
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
CrackDetection::CrackDetection(void)
{
	mImgOrgGray=NULL;
	mImgBinary=NULL;
	mImgBinaryLink=NULL;
	mSpiralMatrix=11;
	mNoiseTh=1;
	mSmallBlock = 9;
	mImgBinaryFileName = "binary.png";
	mImgBinaryLinkFileName = "binary_link.png";
	mImgBinarySmoothFileName = "binary_link_smooth.png";
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
CrackDetection::~CrackDetection(void)
{
	cvReleaseImage(&mImgOrgGray);
	cvReleaseImage(&mImgBinary);
	cvReleaseImage(&mImgBinaryLink);
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
void CrackDetection::LoadImg(std::string _file_name)
{
	IplImage* img_t= cvLoadImage(_file_name.c_str());{
	
			CvSize cv_size_t = cvGetSize(img_t);
			
			this->MallocImage(cv_size_t);

			if (img_t->nChannels==3){
				cvCvtColor(img_t,mImgOrgGray,CV_RGB2GRAY);
			}
			else
			{
				assert(0);
			}
	
	
	}cvReleaseImage(&img_t);
	

}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
void CrackDetection::FillImgP2P(IplImage * _img_spiral_matrix, CvPoint _pt1, CvPoint _pt_center, int _matrix_size)
{	
	const unsigned char gray_max = 255;
	

	cvZero(_img_spiral_matrix);

	CvPoint center_p = cvPoint(_matrix_size/2, _matrix_size / 2 );
	CvPoint p_t;
	uchar* const ImgData = (uchar *)_img_spiral_matrix->imageData;
	const int STEP = _img_spiral_matrix->widthStep / sizeof(uchar);

#if TRUE
	int vr_max = std::max(0, _pt1.y);
	int vr_min = std::min(0, _pt1.y);
	int vc = _pt1.x;
	for (int ri = vr_min; ri <= vr_max; ri++)
	{
		 p_t= cvPoint(center_p.x+vc,center_p.y+ri);
		ImgData[p_t.x + p_t.y*STEP] = gray_max;

	}
#endif // TRUE
	
#if TRUE
	int hc_max = std::max(0, _pt1.x);
	int hc_min = std::min(0, _pt1.x);
	int hr = 0;
	for (int ci = hc_min; ci <= hc_max; ci++)
	{
		p_t = cvPoint(center_p.x + ci, center_p.y + hr);
		ImgData[p_t.x + p_t.y*STEP] = gray_max;

	}
#endif // TRUE

}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
int CrackDetection::GetImgNum(IplImage * _img_spiral_matrix, const int _pixel_data)
{
	const int Width=_img_spiral_matrix->width;
	const int Height = _img_spiral_matrix->height;
	uchar* const ImgData = (uchar *)_img_spiral_matrix->imageData;
	const int STEP = _img_spiral_matrix->widthStep / sizeof(uchar);
	int number_t = 0;
	for (size_t ci = 0; ci <Width; ci++){

		for (size_t ri = 0; ri <Height; ri++){

			int data_t = ImgData[ci + ri*STEP];
			
			if (data_t==255) {
				number_t++;
			}else if (data_t==0) {
			
			}else{
				assert(0);
			}

		}

	}
	return  number_t;
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
void CrackDetection::FillMatrixImg2BinaryLink(IplImage * _img_spiral_matrix, IplImage * _img_bin_link,CvPoint _center_p,const int _matrix_size)
{
	const int offset = _matrix_size / 2;
	const int x_start= _center_p.x - offset;
	const int y_start= _center_p.y - offset;
	

	uchar* const ImgDataM = (uchar *)_img_spiral_matrix->imageData;
	const int StepM = _img_spiral_matrix->widthStep / sizeof(uchar);

	uchar* const ImgDataB = (uchar *)_img_bin_link->imageData;
	const int StepB = _img_bin_link->widthStep / sizeof(uchar);


	for (size_t ci = 0; ci < _matrix_size; ci++)
	{
		for (size_t ri = 0; ri <_matrix_size; ri++)
		{

			const unsigned char data_t = ImgDataM[ci+ri*StepM];

			if (data_t) {			
				ImgDataB[(x_start+ci)+(y_start+ri)*StepB] = data_t;
			}


		}

	}

}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
std::vector<CvPoint> CrackDetection::init_spiral_matrix(int _matrix_size)
{
		std::vector<CvPoint> points;
		const int SplitNumber = _matrix_size*_matrix_size;//Seed总数
		const int	OrientationX[4] = { -1,0,1,0 };
		const int	OrientationY[4] = { 0,-1,0,1 };
		int   OrientationStep[4] = { 1,1,2,2 };
		int   CurrentOrientation = 0;
		int   CurrentCircle = 0;
		const float X_Step = 1;//Seed步长
		const float Y_Step = 1;//Seed步长
		int spi = 0;
		float CurrentSeedX = 0;
		float CurrentSeedY = 0;

		points.push_back(cvPoint(CurrentSeedX, CurrentSeedY));

		do {

			//记录当前位置		
#if 1

			if (OrientationStep[CurrentOrientation]>0) {

				//在当前方向可以前进,下一个点可以被记录

			}else {

				CurrentOrientation++;//转到下一个方向
				if (CurrentOrientation == 4) {
					CurrentOrientation = 0;//回到原点，重新开启一轮
					CurrentCircle++;
					OrientationStep[0] = OrientationStep[1] = 2 * CurrentCircle + 1;
					OrientationStep[2] = OrientationStep[3] = 2 * (CurrentCircle + 1);

				}

			}

			if (spi + 1 == SplitNumber) {
				break;
			}

			//计算next 点
			CurrentSeedX = CurrentSeedX + OrientationX[CurrentOrientation] * X_Step;
			CurrentSeedY = CurrentSeedY + OrientationY[CurrentOrientation] * Y_Step;
			
			points.push_back(cvPoint(CurrentSeedX,CurrentSeedY));

			//记录步长
			OrientationStep[CurrentOrientation]--;
			spi++;

#endif	

		} while (spi<SplitNumber);

		return  points;

}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
int CrackDetection::cal_Noise_num(CvPoint _center_pt, std::vector<CvPoint> _spiral_point, IplImage* _img_bin, const int _spiral_matrix)
{
	size_t ci = _center_pt.x;
	size_t ri = _center_pt.y;

	IplImage* img_spiral_matrix = cvCreateImage(cvSize(_spiral_matrix, _spiral_matrix), IPL_DEPTH_8U, 1);{


					const int data_t = cvGetReal2D(this->mImgBinary, ri, ci);

					if (data_t == 255) {

								for (size_t pi = 0; pi < _spiral_point.size(); pi++) {

									const int r_offset = _spiral_point.at(pi).y;
									const int c_offset = _spiral_point.at(pi).x;

											const int ri_offset = ri + r_offset;
											const int ci_offset = ci + c_offset;
											const int data_t = cvGetReal2D(this->mImgBinary, ri_offset, ci_offset);

											if (data_t == 255) {



											}


								}

					}

	}cvReleaseImage(&img_spiral_matrix);

	return 0;
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
void  CrackDetection::Cvt2Binary()
{
	const float threshold = 0.5;
	const float max_value = 255;
	const int threshold_type = CV_THRESH_BINARY;

	cvThreshold(mImgOrgGray, mImgBinary, threshold, 255, threshold_type);//阀值100
	
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
void CrackDetection::Cvt2Binary3Sigma()
{
	const int ONE_COL_WIDTH = 10;
	IplImage *image_one_col = cvCreateImage(cvSize(ONE_COL_WIDTH, mHeight), IPL_DEPTH_8U, 1);
	for (size_t ci = 0; ci <this->mWidth; ci++)
	{
		ImageProcess::GetOneColumn(mImgOrgGray, image_one_col, ci);

		BinaryOneColumn(mImgBinary, image_one_col,ci);
	}
	cvReleaseImage(&image_one_col);

#if _DEBUG
	cvSaveImage(mImgBinaryFileName.c_str(), mImgBinary);
#endif // 0
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
void CrackDetection::Binary2Link()
{
	
	const int SpiralOffset = mSpiralMatrix/2;
	IplImage* img_spiral_matrix = cvCreateImage(cvSize(mSpiralMatrix , mSpiralMatrix), IPL_DEPTH_8U, 1);
	cvZero(this->mImgBinaryLink);
	std::vector<CvPoint> points=this->init_spiral_matrix(mSpiralMatrix);

	assert(mImgBinary->widthStep == mImgBinary->width* mImgBinary->nChannels);
	assert(1==mImgBinary->nChannels);

	uchar* const ImgData = (uchar *)mImgBinary->imageData;
	const int STEP = mImgBinary->widthStep / sizeof(uchar);

	for (size_t ci = SpiralOffset; ci < mWidth-SpiralOffset; ci++){

		for (size_t ri = SpiralOffset; ri < mHeight-SpiralOffset; ri++){

			const unsigned char data_t= ImgData[ci+ri*STEP];

				if (data_t==255){

						for (size_t pi = 1; pi < points.size(); pi++){
					
							const int ri_offset = ri + points.at(pi).y;
							const int ci_offset=ci+points.at(pi).x;
							const int data_t = ImgData[ci_offset + ri_offset*STEP]; 

							if (data_t==255){

								FillImgP2P(img_spiral_matrix, points.at(pi),cvPoint(0,0),mSpiralMatrix);

								int num=this->GetImgNum(img_spiral_matrix, 255);
								
								if (num>=mNoiseTh){
									this->FillMatrixImg2BinaryLink(img_spiral_matrix, this->mImgBinaryLink,cvPoint(ci,ri),mSpiralMatrix);
								}

							}


						}

				}

		}

	}

	cvReleaseImage(&img_spiral_matrix);

#if _DEBUG
	cvSaveImage(mImgBinaryLinkFileName.c_str(),mImgBinaryLink);
#endif 


}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
void CrackDetection::Binary2Smooth()
{

#if _DEBUG
	TimeMeasure tm;
	tm.start(__func__);
#endif // _DEBUG

	this->RemoveConnectivity(this->mImgBinaryLink, mSmallBlock);


#if _DEBUG
	tm.stop();
#endif

#if _DEBUG
	cvSaveImage(mImgBinarySmoothFileName.c_str(), mImgBinaryLink);
#endif 
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
void CrackDetection::RemoveConnectivity(IplImage* _img, int _size_remove)
{
		const int width  = _img->width;
		const int height = _img->height;
		const int sz = width*height;
		std::vector<int> xvecMem(sz,0);
		std::vector<int> yvecMem(sz,0);
		std::vector<int> nlabelsMem(sz, 0);
		uchar* const ImgData = (uchar *)_img->imageData;
		const int STEP = _img->widthStep / sizeof(uchar);


		assert(_img->widthStep == _img->width* _img->nChannels);
#if 0
		const int dx8[8] = {-1, -1,  0,  1, 1, 1, 0, -1};
		const int dy8[8] = { 0, -1, -1, -1, 0, 1, 1,  1};
#else
		const int dx4[4] = { -1,  0,  1,  0 };
		const int dy4[4] = { 0, -1,  0,  1 };
#endif // 0

		int oindex(0);
		int* xvec = xvecMem.data();
		int* yvec = yvecMem.data();
		int* nlabels = nlabelsMem.data();
		const int label_visited(0);
		int adjlabel(0);//adjacent label

		for (register int i = 0; i < sz; i++) 
				nlabels[i] = -1;

		for (register int j = 0; j < height; j++) {
			for (register int k = 0; k < width; k++,nlabels[oindex++] = label_visited) {
					
								const int IDX = j*STEP + k;

								if (ImgData[IDX] == 255) {

									if (nlabels[oindex] == -1)
									{
#if TRUE
										//--------------------
										// Start a new segment
										//--------------------
										xvec[0] = k;
										yvec[0] = j;

										int count(1);
										for (register int c = 0; c < count; c++)
										{
											for (register int n = 0; n < 4; n++)
											{
												int x = xvec[c] + dx4[n];
												int y = yvec[c] + dy4[n];

												if ((x >= 0 && x < width) && (y >= 0 && y < height))
												{
													int nindex = y*STEP + x;

													if (-1 == nlabels[nindex] && 255 == ImgData[nindex])
													{
														xvec[count] = x;
														yvec[count] = y;
														nlabels[nindex] = label_visited;
														count++;
													}
												}

											}
										}



#endif
#if TRUE
										if (count <= _size_remove)
										{
											for (register int c = 0; c < count; c++)
											{
												const int ind = yvec[c] * STEP + xvec[c];
												ImgData[ind] = 0;
											}

										}
#endif

									}



								}else {									
									assert(ImgData[IDX] == 0);//zero 
								}

			}
		}

}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
void CrackDetection::IMG_RemoveBorder(IplImage * _img_bin, std::vector<int> _row, std::vector<int> _col)
{
	ImageProcess::IMG_RemoveBorder(_img_bin, _row, _col);	
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
void CrackDetection::Init_RemoveBorder(const int WIDTH,const int HEIGHT, std::vector<int>& _row, std::vector<int>& _col)
{
	_row.push_back(0);
	_row.push_back(HEIGHT - 1);

	_col.push_back(0);
	_col.push_back(WIDTH - 1);

	if (WIDTH == 960 && HEIGHT == 544) {

		for (size_t ri = HEIGHT - 4; ri < HEIGHT - 1; ri++)
		{
			_col.push_back(ri);
		}

	}
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
void CrackDetection::Binary2Noise(const int _matrix_size_out,const int _matrix_size_in,const int _c_step)
{
	Gray2NoiseFast(this->mImgBinaryLink);
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
void CrackDetection::Binary2NoiseAdj(const int _matrix_size_out, const int _matrix_size_in)
{
	IplImage* const img_t = this->mImgBinaryLink;
	uchar* const ImgData = (uchar *)img_t->imageData;
	const int STEP = img_t->widthStep / sizeof(uchar);

	const int ring_width = _matrix_size_out - _matrix_size_in;

	for (size_t ci = 0; ci < mWidth; ci++) {
		for (size_t ri = 0; ri <mHeight; ri++) {
	
			if ((ri<ring_width) ||
				(ci<ring_width) ||
				(mWidth-ci)<ring_width ||
				(mHeight-ri)<ring_width){

				ImgData[ci + STEP*ri] = 0;

			}
			
		}
	}

}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
void CrackDetection::CopyBinary2Link()
{
	cvCopyImage(this->mImgBinary,this->mImgBinaryLink);
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
std::vector<float> CrackDetection::GetFeature(const int _matrix_size_out, const int _matrix_size_in)
{

	IplImage* const img_t = this->mImgBinaryLink;
	
	return GetFeatureFast(img_t);
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
std::vector<float> CrackDetection::GetCrackImgHoughBlockFeatureBase(IplImage * _img_bin,
	IplImage * _img_block,
	std::vector<Line2Point>& _lines,
	IplImage * _img_output,
	const CvScalar _color,
	const int _thickness)
{
	std::vector<float> f_b_t;
	
	const int hough_thickness = 3;
	const CvScalar hough_color = CV_RGB(255,255,255);
	/*--hough--*/
	_lines = ImageProcess::getHoughLines(_img_bin, _img_block, hough_color, hough_thickness);

	if (_lines.size()==0){

		f_b_t.push_back(0);
		f_b_t.push_back(0);
		f_b_t.push_back(0);

	}else{
		/*--block--*/
		f_b_t = BlockAnalysis::getFeatureBlock(_img_block, _img_output,_color,_thickness);
	}
	
	return f_b_t;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
int CrackDetection::predict_block(std::vector<float> _f, std::vector<float> _th)
{
	const float area = _f.at(0);
	const  float e = _f.at(1);
	const float area_all = _f.at(2);

	const float AREA = _th.at(0);
	const  float E = _th.at(1);
	const float AREA_ALL = _th.at(2);


	if (area_all>AREA_ALL) {

		return POS_CLASS;

	}
	else {

		if (area>AREA && e>E) {
			return POS_CLASS;
		}
		else {
			return NEG_CLASS;
		}

	}
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
int CrackDetection::predict_in_out_block(std::vector<float> _f, std::string _in_out, std::vector<float> _th)
{
	int pos_neg_result = NEG_CLASS;

	if (_in_out.compare(CRACK_IN) == 0)
	{
		pos_neg_result = predict_block(_f, _th);
	}
	else if (_in_out.compare(CRACK_OUT) == 0)
	{
		pos_neg_result = predict_block(_f, _th);
	}
	else
	{
		assert(0);
	}

	return pos_neg_result;
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
void CrackDetection::Gray2NoiseFast(IplImage * _img)
{	
	uchar* const ImgData = (uchar *)_img->imageData;
	const int STEP = _img->widthStep / sizeof(uchar);
	const int Width=_img->width;
	const int Height=_img->height;
	const int border_size = 1;
	const int border_w_max = Width - border_size - 1;
	const int border_h_max = Height - border_size - 1;

#if 1
	const int WidthB= Width - border_size;
	const int HeightB= Height - border_size;
	const int x_offset[8] = {1,1,1,0,-1,-1,-1,0};
	const int y_offset[8] = {-1,0,1,1,1,0,-1,-1};
	
	for (size_t wi = border_size; wi <WidthB; wi++)
	{
		for (size_t hi = border_size; hi <HeightB; hi++)
		{
			int img_data= ImgData[wi + hi*STEP];
			if (img_data==255)
			{
						int isPoint=0;
						for (size_t gi = 0; gi < 8; gi++)
						{
							int gwi = wi + x_offset[gi];
							int ghi = hi + y_offset[gi];
							int img_data_g= ImgData[gwi + ghi*STEP];
							if (img_data_g == 255) {
								isPoint++;
								break;
							}

						}
						if (isPoint==0) {
							ImgData[wi + hi*STEP] = 0;							
						}


			}

		}

	}
#endif

		
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
void CrackDetection::Binary2Smooth_Line(IplImage * _img)
{
	uchar* const ImgData = (uchar *)_img->imageData;
	const int STEP = _img->widthStep / sizeof(uchar);
	const int Width = _img->width;
	const int Height = _img->height;
	const int border_size = 1;
	const int border_w_min = border_size;
	const int border_h_min = 0;
	const int border_w_max = Width - border_size - 1;
	const int border_h_max = Height;
	
	ASSERT(Width==STEP);

#if 1
	const int WidthB = Width;
	const int HeightB = Height - border_size;
	
	const int x_offset[] = { -1,1};
	const int y_offset[] = {0,0 };
	const int SIEZ_OFFSET = sizeof(x_offset) / sizeof(int);
	for (size_t wi = border_w_min; wi <WidthB; wi++)
	{
		for (size_t hi = border_h_min; hi <HeightB; hi++)
		{
			int img_data = ImgData[wi + hi*STEP];
			if (img_data == 255)
			{
				int isPoint = 0;
				for (size_t gi = 0; gi < SIEZ_OFFSET ; gi++)
				{
					int gwi = wi + x_offset[gi];
					int ghi = hi + y_offset[gi];
					int img_data_g = ImgData[gwi + ghi*STEP];
					if (img_data_g == 255) {
						isPoint++;
						break;
					}

				}
				if (isPoint == 0) {
					ImgData[wi + hi*STEP] = 0;
				}


			}

		}

	}
#endif
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
std::vector<float> CrackDetection::GetFeatureFast(const IplImage*  _img_binary)
{
	const IplImage* const img_t = _img_binary;
	const uchar* const ImgData = (uchar *)img_t->imageData;
	const int STEP = img_t->widthStep / sizeof(uchar);

	const int ring_width = 1;
	const int rWIDTH = img_t->width-ring_width;
	const int rHEIGHT = img_t->height-ring_width;
	unsigned int points = 0;

	for (size_t ci = ring_width; ci < rWIDTH; ci++) {
		for (size_t ri = ring_width; ri <rHEIGHT; ri++) {

			if (ImgData[ci + STEP*ri]==255) {
				points++;
			}
			
		}
	}
	
	const float f_t=1.0f*points / (rWIDTH*rHEIGHT);

	std::vector<float> f_v_t;	
	f_v_t.push_back(f_t);

	return f_v_t;
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
std::vector<float> CrackDetection::GetFeatureFastEx(IplImage * _img_binary, int _matrix_size_out, const int _matrix_size_in, const int _c_step)
{
	 Binary2Smooth_Line(_img_binary);
	 	
	 return   GetFeatureFast(_img_binary);
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
void CrackDetection::MallocImage(CvSize _size_t)
{
	this->mHeight = _size_t.height;
	this->mWidth = _size_t.width;

	mImgOrgGray=cvCreateImage(_size_t,IPL_DEPTH_8U,1);
	mImgBinary = cvCreateImage(_size_t, IPL_DEPTH_8U, 1);
	mImgBinaryLink = cvCreateImage(_size_t, IPL_DEPTH_8U, 1);

}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
void CrackDetection::BinaryOneColumn(IplImage * image, IplImage * ColData, int IdxCol)
{
	std::vector<float> feature_one;
	std::vector<float> feature_org;
	const float MIN_V = 0;
	const float MAX_V = 255;

#if TRUE
	for (size_t ri = 0; ri <ColData->height; ri++) {
		float data_t = cvGetReal2D(ColData, ri, 0);
		feature_org.push_back(data_t);
	}
#endif // TRUE

#if TRUE
	const float avg_org = Base::Math_GetAverageValueF(feature_org.data(), feature_org.size());//[0,255]
	float sigma_org = 0;//[0,255]
	Base::Math_GetVarianceValueF(feature_org.data(), feature_org.size(), avg_org, &sigma_org);
#endif // TRUE



#if TRUE
	float line_width = 0;
	for (size_t ri = 0; ri <feature_org.size(); ri++) {

		float value_col = feature_org.at(ri);
		float sub_abs = fabs(avg_org - value_col);

		if (sub_abs > 3 * sigma_org) {
			line_width++;
			//feature
			cvSetReal2D(image, ri, IdxCol, MAX_V);
		}
		else {
			//normal
			cvSetReal2D(image, ri, IdxCol, MIN_V);
		}

	}
#endif // TRUE

	
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
std::vector<float> CrackDetection::getThresholdInCircleBlock()
{
	std::vector<float> th_t;
	th_t.push_back(120);
	th_t.push_back(0.8);
	th_t.push_back(789);
	return th_t;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
std::vector<float> CrackDetection::getThresholdOutCircleBlock()
{
	std::vector<float> th_t;
	th_t.push_back(18);
	th_t.push_back(0.71);
	th_t.push_back(45);
	return th_t;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
std::vector<float> CrackDetection::getThresholdInOutCircleBlock(std::string _in_out)
{
	int pos_neg_result = CrackDetection::NEG_CLASS;
	std::vector<float> f_t;

	if (_in_out.compare(CrackDetection::CRACK_IN) == 0)
	{
		f_t = getThresholdInCircleBlock();
	}
	else if (_in_out.compare(CrackDetection::CRACK_OUT) == 0)
	{
		f_t = getThresholdOutCircleBlock();
	}
	else
	{
		assert(0);
	}

	return f_t;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
std::vector<float> CrackDetection::getThresholdInCircleHoughBlock()
{
	std::vector<float> th_t;
	th_t.push_back(312);
	th_t.push_back(0.77);
	th_t.push_back(1116);
	return th_t;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
std::vector<float> CrackDetection::getThresholdOutCircleHoughBlock()
{
	std::vector<float> th_t;
	th_t.push_back(82);
	th_t.push_back(0.8);
	th_t.push_back(125);
	return th_t;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
std::vector<float> CrackDetection::getThresholdInOutCircleHoughBlock(std::string _in_out)
{
	int pos_neg_result = CrackDetection::NEG_CLASS;
	std::vector<float> f_t;

	if (_in_out.compare(CrackDetection::CRACK_IN) == 0)
	{
		f_t = getThresholdInCircleHoughBlock();
	}
	else if (_in_out.compare(CrackDetection::CRACK_OUT) == 0)
	{
		f_t = getThresholdOutCircleHoughBlock();
	}
	else
	{
		assert(0);
	}

	return f_t;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/