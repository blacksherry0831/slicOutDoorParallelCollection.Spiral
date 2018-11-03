#include "StdAfx.h"
#include "CrackDetection.hpp"
#include <MY_SDK_LIB/Base.h>
#include <MY_SDK_LIB/ImageProcess.h>
#include <MY_SDK_LIB/TimeMeasure.hpp>
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
void CrackDetection::Binary2Noise(const int _matrix_size_out,const int _matrix_size_in,const int _c_step)
{
	Gray2NoiseFast(this->mImgBinaryLink,_matrix_size_out,_matrix_size_in,_c_step);
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
float CrackDetection::GetFeature(const int _matrix_size_out, const int _matrix_size_in)
{

	IplImage* const img_t = this->mImgBinaryLink;
	
	return GetFeatureFast(img_t,_matrix_size_out,_matrix_size_in);
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
void CrackDetection::Gray2NoiseFast(IplImage * _img, const int _matrix_size_out, const int _matrix_size_in, const int _c_step)
{	
	uchar* const ImgData = (uchar *)_img->imageData;
	const int STEP = _img->widthStep / sizeof(uchar);
	const int Width=_img->width;
	const int Height=_img->height;
	const int center_c = _matrix_size_out / 2;
	const int center_r = _matrix_size_out / 2;
	const int matrix_hsize_out = _matrix_size_out / 2;
	const int matrix_hsize_in = _matrix_size_in / 2;
	const int center_c_max = Width - center_c;
	const int center_r_max = Height - center_r;

	for (size_t ci = center_c; ci < center_c_max; ci += _c_step) {
		for (size_t ri = center_r; ri <center_r_max; ri += _c_step) {

			const int rect_out_x = ci - matrix_hsize_out;
			const int rect_out_right = rect_out_x + _matrix_size_out;
			const int rect_out_y = ri - matrix_hsize_out;
			const int rect_out_bottom = rect_out_y + _matrix_size_out;

			const int rect_in_x = ci - matrix_hsize_in;
			const int rect_in_y = ri - matrix_hsize_in;
			const int rect_in_right = rect_in_x + _matrix_size_in;
			const int rect_in_bottom = rect_in_y + _matrix_size_in;

			int out_pixel_size = 0;
#if TRUE

			for (size_t cio = rect_out_x; cio < rect_out_right; cio++) {
				for (size_t rio = rect_out_y; rio <rect_out_bottom; rio++) {

					if (cio >= rect_in_x  &&
						cio<rect_in_right &&
						rio >= rect_in_y &&
						rio<rect_in_bottom) {
						//in
						//ImgData[cio + STEP*rio] = 128;

					}
					else {
						//ring
						if (ImgData[cio + STEP*rio]) {
							//this point is not null,
							out_pixel_size++;
						}
						//ImgData[cio + STEP*rio] = 255;
					}

				}

			}
#endif 

#if TRUE
			if (out_pixel_size == 0) {
				//out ring no 
				for (size_t cii = rect_in_x; cii < rect_in_right; cii++) {
					for (size_t rii = rect_in_y; rii <rect_in_bottom; rii++) {
						ImgData[cii + STEP*rii] = 0;
					}
				}

			}
#endif // TRUE

		}
	}
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
float CrackDetection::GetFeatureFast(const IplImage*  _img_binary, const int _matrix_size_out, const int _matrix_size_in)
{
	const IplImage* const img_t = _img_binary;
	const uchar* const ImgData = (uchar *)img_t->imageData;
	const int STEP = img_t->widthStep / sizeof(uchar);

	const int ring_width = _matrix_size_out - _matrix_size_in;
	const int rWIDTH = img_t->width-ring_width;
	const int rHEIGHT = img_t->height-ring_width;
	unsigned int points = 0;

	for (size_t ci = ring_width; ci < rWIDTH; ci++) {
		for (size_t ri = ring_width; ri <rHEIGHT; ri++) {

			if (ImgData[ci + STEP*ri]) {
				points++;
			}

		}
	}

	return 1.0f*points / (rWIDTH*rHEIGHT);
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
float CrackDetection::GetFeatureFastEx(IplImage * _img_binary, int _matrix_size_out, const int _matrix_size_in, const int _c_step)
{
//	TimeMeasure tm;
//	tm.start(__func__);

	 Gray2NoiseFast(_img_binary, _matrix_size_out, _matrix_size_in, _c_step);
	 const float feature_t = GetFeatureFast(_img_binary,_matrix_size_out,_matrix_size_in);
	
//	tm.stop();
	 return feature_t;
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
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/