#include "stdafx.h"
#include <MY_SDK_LIB/Base.h>
#include <MY_SDK_LIB/ImageProcess.h>
#include <MY_SDK_LIB/BlockAnalysis.hpp>
#include <MY_SDK_LIB/CrackDetection.hpp>
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
#define USE_IMAGE_DEBUG  (0)
/*----------------------------------------------------------------*/
#define IN_RING_CH8_CRACK_BLOCK_SIZE (100)
#define OUT_RING_CH8_CRACK_BLOCK_SIZE (100)
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
const std::string  CRACK_ROLLER_SET= "crack.roller.";
const std::string  CRACK_SAMPLE = "sample";

const std::string  CRACK_IP = "0.0.0.0";
const std::string  CRACK_TIME = "time";
const std::string  CRACK_CHANNEL = "ch";
const std::string  CRACK_CHANNEL_P = "p";
const std::string  CRACK_CHANNEL_N = "n";
const std::string  CRACK_RECALL_PRECISION_TXT = "predict.txt";
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/


/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
std::string GetResultFilePath(std::string _root, std::string _file, std::string _add)
{
	std::string path;
	path.append(_root).append(_add).append(_file);
	return path;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void Save_Feature_Recall_Precision(std::string  _path,
	std::string _path_sub_output,
	std::string _in_out,
	std::string _feature_name,
	int _tp,
	int _fp,
	int _fn,
	const std::vector<float>& _feature,
	const std::vector<int>& _classify)
{
	const int feature_dimension = _feature.size() / _classify.size();

	const std::string block_base_path_t = Base::FS_createPath(_path, _path_sub_output);

	ImageProcess::Save_Feature2TXT(_feature, _classify, feature_dimension, _feature_name, block_base_path_t);

	ImageProcess::Svm_Lean_SAVE_Recall_Precision(_tp,
		_fp,
		_fn,
		GetResultFilePath(block_base_path_t, CRACK_RECALL_PRECISION_TXT, _in_out));
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
std::vector<int> generate_X_ShadowEx(IplImage* _img)
{
	uchar* const ImgData = (uchar *)_img->imageData;
	const int STEP = _img->widthStep / sizeof(uchar);
	const int Width = _img->width;
	const int Height = _img->height;
	
	std::vector<int> shadow_x_t;
	shadow_x_t.resize(Width, 0);

	for (size_t xi = 0; xi < Width; xi++)
	{
		for (size_t yi = 0; yi <Height; yi++)
		{
			unsigned char pixel_data_t = ImgData[xi + yi*STEP];

			if (pixel_data_t == UCHAR_MAX) {
#if 0
				ImgData[xi + (Height - 1)*STEP] = UCHAR_MAX;
#endif // _DEBUG
#if 1
				shadow_x_t[xi] = UCHAR_MAX;
				break;
#endif // 1
			}


		}
	}

	return shadow_x_t;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
std::vector<int>  generate_Y_ShadowEx(IplImage* _img)
{
	uchar* const ImgData = (uchar *)_img->imageData;
	const int STEP = _img->widthStep / sizeof(uchar);
	const int Width = _img->width;
	const int Height = _img->height;

	std::vector<int> shadow_y_t;
	shadow_y_t.resize(Height, 0);

	for (size_t yi = 0; yi <Height; yi++)
	{
		for (size_t xi = 0; xi < Width; xi++)
		{
			unsigned char pixel_data_t = ImgData[xi + yi*STEP];

			if (pixel_data_t == UCHAR_MAX) {
#if 1
				ImgData[yi*STEP] = UCHAR_MAX;
#endif // 1

#if 1
				shadow_y_t[yi] = UCHAR_MAX;
				break;
#endif // 1

			}


		}
	}

	return shadow_y_t;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
std::vector<float> get_Feature_1_2(std::vector<int> _shadow)
{
		                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
	int sum = 0;
	int flag = 0;
	
	std::vector<std::vector<int>> lines_t;
	std::vector<float> feature_t;
	std::vector<int> line_t;

	for (size_t xi = 0; xi <_shadow.size(); xi++)
	{

		unsigned char pixel_data_t = _shadow[xi];

		if (pixel_data_t == UCHAR_MAX) {
			flag = TRUE;
			sum++;
			line_t.push_back(xi);

		}
		else
		{
			if (line_t.size() != 0) {
				lines_t.push_back(line_t);
			}
			flag = FALSE;
			line_t.clear();

		}

	}

#if _DEBUG && 0
	int sum_debug = 0;
	for (size_t li = 0; li < _lines.size(); li++)
	{
		std::vector<int> line_t = _lines.at(li);

		for (size_t pi = 0; pi < line_t.size(); pi++) {
			sum_debug++;

		}

	}
#endif // _DEBUG
	

	
	const int WH = _shadow.size();
	const int SUM = sum;
	const int N = lines_t.size();
	float f1 =0;
	
	
	if (SUM == 0) {
		f1 = 0;
	}else{
		f1 = 1.0*N/SUM;
	}

	const float f2 =1.0 * SUM / WH;	



	ASSERT(WH > 0);
	ASSERT(f1>=0 && f1 <=1.1);
	ASSERT(f2>=0 && f2 <= 1.1);

	feature_t.push_back(f1);
	feature_t.push_back(f2);

	return feature_t;
}

/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
int IsPosSample(std::string _path)
{
	std::string dir_name = Base::FS_getDirName(_path);

	if (dir_name.find("p") == std::string::npos) {
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
std::string getAnotherfullPath(std::string _file_name)
{
	std::string file_full_name_t = _file_name;

	std::string file_name = Base::FS_getFileName(file_full_name_t);
	std::string path_channel = Base::FS_getSuperDirPath(file_full_name_t);
	std::string dir_channel_name = Base::FS_getEndDirName(path_channel);
	std::string path_time = Base::FS_getSuperDirPath(path_channel);

	std::string dir_channel_name_dst;

	if (IsPosSample(path_channel))
	{
		dir_channel_name_dst = Base::replace_str(dir_channel_name, "p", "n");
	}
	else
	{
		dir_channel_name_dst = Base::replace_str(dir_channel_name, "n", "p");
	}

	std::string dst_path = Base::FS_createPath(path_time, dir_channel_name);
	std::string dst_path_full_name = dst_path + file_name;
	return dst_path_full_name;

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void MoveImg2AnotherSide(const std::string _file_full_name_t)
{
	std::string file_full_name_dst_t = getAnotherfullPath(_file_full_name_t);
	Base::FS_MoveFile(_file_full_name_t, file_full_name_dst_t);
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void generate_X_Shadow(IplImage* _img, IplImage* _x_shadow)
{
	uchar* const ImgData = (uchar *)_img->imageData;
	const int STEP = _img->widthStep / sizeof(uchar);
	const int Width = _img->width;
	const int Height = _img->height;

	uchar* const ImgDataDst = (uchar *)_x_shadow->imageData;
	const int StepDst = _x_shadow->widthStep / sizeof(uchar);

	cvZero(_x_shadow);

	for (size_t xi = 0; xi < Width; xi++)
	{
		for (size_t yi = 0; yi <Height; yi++)
		{
			unsigned char pixel_data_t = ImgData[xi + yi*STEP];
			
			if (pixel_data_t == UCHAR_MAX) {
				ImgDataDst[xi] = UCHAR_MAX;
#if _DEBUG
				ImgData[xi + (Height-1)*STEP]=UCHAR_MAX;
#endif // _DEBUG

				break;
			}

			
		}
	}

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
int get_X_Shadow_S_N(IplImage* _x_shadow,std::vector<std::vector<int>> &_lines)
{
	uchar* const ImgData = (uchar *)_x_shadow->imageData;
	const int STEP = _x_shadow->widthStep / sizeof(uchar);
	const int Width = _x_shadow->width;
	const int Height = _x_shadow->height;
	ASSERT(_x_shadow->nChannels==1);

	int sum = 0;
	int flag = 0;

	std::vector<int> line_t;

	for (size_t xi = 0; xi < Width; xi++)
	{
			
			unsigned char pixel_data_t = ImgData[xi];

			if (pixel_data_t == UCHAR_MAX) {
				flag = TRUE;
				sum++;
				line_t.push_back(xi);
				
			}
			else
			{
				if (line_t.size() != 0) {
					_lines.push_back(line_t);
				}
				flag = FALSE;
				line_t.clear();

			}
			
	}

#if _DEBUG
	int sum_debug = 0;
	for (size_t li = 0; li < _lines.size(); li++)
	{
			std::vector<int> line_t=_lines.at(li);
			
			for (size_t pi = 0; pi < line_t.size(); pi++){
				sum_debug++;

			}

	}
#endif // _DEBUG

	return sum;

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
std::vector<float> GetAllFeature(IplImage* _img)
{
	uchar* const ImgData = (uchar *)_img->imageData;
	const int STEP = _img->widthStep / sizeof(uchar);
	const int Width = _img->width;
	const int Height = _img->height;
	ASSERT(_img->nChannels == 1);

	std::vector<int> x_shadow=generate_X_ShadowEx(_img);
	std::vector<int> y_shadow=generate_Y_ShadowEx(_img);

	std::vector<float>  x_f= get_Feature_1_2(x_shadow);
	std::vector<float>  y_f = get_Feature_1_2(y_shadow);

	std::vector<float> feature_t=Base::CombineVector(x_f, y_f);
	
	return feature_t;
}

/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
#if USE_IMAGE_DEBUG

void GetFeature1(IplImage* _img)
{
	uchar* const ImgData = (uchar *)_img->imageData;
	const int STEP = _img->widthStep / sizeof(uchar);
	const int Width = _img->width;
	const int Height = _img->height;

	ASSERT(_img->nChannels==1);

	IplImage* x_shadow = cvCreateImage(cvSize(Width,1),IPL_DEPTH_8U,1);
	{
	
		generate_X_Shadow(_img,x_shadow);
		std::vector<std::vector<int>> lines_t;
		int sum = get_X_Shadow_S_N(x_shadow, lines_t);
		int n = lines_t.size();
		int width = Width;
	
	}
	cvReleaseImage(&x_shadow);
	
}

#endif
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
std::string GetOutPutSavePath(std::string base_path, std::string _sub_path, std::string _add_t, int _pos_neg_sample, int _pos_neg_result)
{
	std::string path_t;

	std::string hough_base_path_t = Base::FS_createPath(base_path, _sub_path);

	if (CrackDetection::POS_CLASS == _pos_neg_sample &&  CrackDetection::POS_CLASS == _pos_neg_result)
	{
		path_t = Base::FS_createPath(hough_base_path_t, _add_t.append("true.pos"));
	}
	else if (CrackDetection::POS_CLASS == _pos_neg_sample && CrackDetection::NEG_CLASS == _pos_neg_result)
	{
		path_t = Base::FS_createPath(hough_base_path_t, _add_t.append("false.neg"));
	}
	else if (CrackDetection::NEG_CLASS == _pos_neg_sample &&  CrackDetection::NEG_CLASS == _pos_neg_result)
	{
		path_t = Base::FS_createPath(hough_base_path_t, _add_t.append("true.neg"));
	}
	else if (CrackDetection::NEG_CLASS == _pos_neg_sample && CrackDetection::POS_CLASS == _pos_neg_result)
	{
		path_t = Base::FS_createPath(hough_base_path_t, _add_t.append("false.pos"));
	}
	else
	{
		assert(0);
	}

	return path_t;

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void SaveImg2OutputPath(IplImage* _img,
	std::string _path,
	std::string _path_sub,
	std::string _file_name,
	int _pos_neg,
	int _pos_neg_result)
{
	std::string add_t;

#ifdef TRUE
	std::string save_path_t = GetOutPutSavePath(_path, _path_sub, add_t, _pos_neg, _pos_neg_result);
	std::string save_full_path_t = save_path_t + _file_name;
	cvSaveImage(save_full_path_t.c_str(), _img);
#endif // TRUE
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
#if 1
std::vector<float> GetCrackImgFeature(std::string _img_file)
{
	std::cout << _img_file << std::endl;

	std::vector<float> f_t;
	


	IplImage* img_t= cvLoadImage(_img_file.c_str(), CV_LOAD_IMAGE_ANYCOLOR);
	{
	
			f_t=GetAllFeature(img_t);
	
	}
	cvReleaseImage(&img_t);




	return f_t;
}
#endif
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
int predict_hough_lines(std::vector<Line2Point> _lines)
{
	int pos_neg_result_t = CrackDetection::NEG_CLASS;

	if (_lines.size() == 0)
	{
		pos_neg_result_t = CrackDetection::NEG_CLASS;
	}
	else
	{
		pos_neg_result_t = CrackDetection::POS_CLASS;
	}

	return pos_neg_result_t;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
int predict_in_out_block_output(IplImage* _img,
	std::string _path,
	std::string _path_sub,
	std::string _file_name,
	std::vector<float> _f,
	std::string _in_out,
	int _pos_neg,
	std::vector<float> _th)
{
	int pos_neg_result = CrackDetection::predict_in_out_block(_f, _in_out, _th);

	SaveImg2OutputPath(_img, _path, _path_sub, _file_name, _pos_neg, pos_neg_result);

	return pos_neg_result;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
std::string GetHoughOutPutSavePath(std::string base_path,std::string _add_t, int _pos_neg_sample,int _pos_neg_result )
{
	
	std::string hough_base_path_t =  ("hough.analyze.output");

	return GetOutPutSavePath(base_path, hough_base_path_t, "", _pos_neg_sample, _pos_neg_result);

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
std::string GetBlockOutPutSavePath(std::string base_path, std::string _add_t, int _pos_neg_sample, int _pos_neg_result)
{
	
	std::string hough_base_path_t =  ("block.analyze.output");

	assert(_pos_neg_sample== CrackDetection::POS_CLASS || _pos_neg_sample == CrackDetection::NEG_CLASS);
	assert(_pos_neg_result == CrackDetection::POS_CLASS || _pos_neg_result == CrackDetection::NEG_CLASS);

	std::string path_t = GetOutPutSavePath(base_path, hough_base_path_t, _add_t, _pos_neg_sample, _pos_neg_result);

	return path_t;

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
std::vector<float> GetCrackImgHoughFeature(std::string _img_file,std::string _path,int _pos_neg)
{
	std::cout << _img_file << std::endl;
	
	std::vector<float> f_t(2,0);
	std::string file_name_t = Base::FS_getFileName(_img_file);
	
	const int R_DEGREE=45;

	IplImage* img_t = cvLoadImage(_img_file.c_str(), CV_LOAD_IMAGE_ANYCOLOR);
	IplImage* img_color_t = cvLoadImage(_img_file.c_str(), CV_LOAD_IMAGE_COLOR);
	
	{
				
		std::vector<Line2Point> lines_t = ImageProcess::getHoughLines(img_t, img_color_t);
		
		int pos_neg_result= CrackDetection::NEG_CLASS;
		
		if (lines_t.size()==0)
		{
			pos_neg_result = CrackDetection::NEG_CLASS;
		}
		else
		{
			pos_neg_result = CrackDetection::POS_CLASS;
		}

		std::string hough_base_path_t = ("hough.analyze.output");
		std::string file_name_t = Base::FS_getFileName(_img_file);

		SaveImg2OutputPath(img_color_t,
								_path,
								hough_base_path_t,
								file_name_t,
								_pos_neg,
								pos_neg_result);

	}
	cvReleaseImage(&img_t);
	cvReleaseImage(&img_color_t);
	
	return f_t;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
std::vector<float> GetCrackImgHoughBlockFeatureFAST(const std::string _file_name,
	std::string _in_out,
	std::string _path,
	std::string _path_sub,
	IplImage* _img_bin,
	IplImage* _img_block,
	IplImage* _img_output,
	int _pos_neg,
	std::vector<float> _th)
{	
	std::vector<Line2Point> lines_t;	

	std::vector<float> f_b_t= CrackDetection::GetCrackImgHoughBlockFeatureBase(_img_bin, _img_block, lines_t, _img_output);

	do{
		/*--hough--*/
		const int pos_neg_hough_result = predict_hough_lines(lines_t);
		SaveImg2OutputPath(_img_block, _path, _path_sub + ".p.hough", _file_name, _pos_neg, pos_neg_hough_result);
		/*--block--*/
		const int pos_neg_block_result = CrackDetection::predict_in_out_block(f_b_t, _in_out, _th);
		SaveImg2OutputPath(_img_output, _path, _path_sub + ".p.block", _file_name, _pos_neg, pos_neg_block_result);	
	
	} while (0);
					
	return f_b_t;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
std::vector<float> GetCrackImgHoughBlockFeature(std::string _img_file,
	std::string _in_out,
	std::string _path,
	std::string _path_sub,
	int _pos_neg,
	std::vector<float> _th)
{
	std::cout << _img_file << std::endl;

	std::vector<float> f_t;
	
	const std::string file_name_t = Base::FS_getFileName(_img_file);

	IplImage* img_bin_t = cvLoadImage(_img_file.c_str(), CV_LOAD_IMAGE_ANYCOLOR);
	IplImage* img_block_t = cvCreateImage(cvGetSize(img_bin_t), IPL_DEPTH_8U, 1);
	IplImage* img_output_t = cvCreateImage(cvGetSize(img_bin_t), IPL_DEPTH_8U, 3);
	
	cvZero(img_block_t);

#if 0
	cvZero(img_output_t);
#else
	cvCvtColor(img_bin_t, img_output_t,CV_GRAY2BGR);
#endif // 0
	
		f_t=GetCrackImgHoughBlockFeatureFAST(file_name_t,
			_in_out,
			_path,
			_path_sub,
			img_bin_t,
			img_block_t,
			img_output_t,
			_pos_neg,_th);
		
	cvReleaseImage(&img_bin_t);
	cvReleaseImage(&img_block_t);
	cvReleaseImage(&img_output_t);
	
	return f_t;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
int predict_in_out_hough(std::vector<float> _f,
	std::string _in_out,
	std::vector<float> _th)
{
	int pos_neg_result = CrackDetection::NEG_CLASS;

	return pos_neg_result;
}

/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
int predict_in_out_hough_output(IplImage* _img,
	std::string _path,
	std::string _path_sub,
	std::string _file_name,
	std::vector<float> _f,
	std::string _in_out,
	int _pos_neg,
	std::vector<float> _th)
{
	int pos_neg_result = CrackDetection::predict_in_out_block(_f, _in_out, _th);

	SaveImg2OutputPath(_img, _path, _path_sub, _file_name, _pos_neg, pos_neg_result);

	return pos_neg_result;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
std::vector<float> getFeatureHough(IplImage* _img_bin, IplImage* _img_draw)
{
	std::vector<Line2Point> lines = ImageProcess::getHoughLines(_img_bin, _img_draw);

	std::vector<float> f_t;

	f_t.push_back(lines.size());

	return f_t;
}

/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
std::vector<float> GetCrackImgBlockFeature(std::string _img_file,
	std::string _in_out,
	std::string _path,
	std::string _path_sub,
	int _pos_neg,
	std::vector<float> _th)
{
	std::cout << _img_file << std::endl;

	std::vector<float> f_t;
	std::string file_name_t = Base::FS_getFileName(_img_file);
		
	IplImage* img_bin_t = cvLoadImage(_img_file.c_str(), CV_LOAD_IMAGE_ANYCOLOR);
	IplImage* img_color_t = cvCreateImage(cvGetSize(img_bin_t),IPL_DEPTH_8U,3);

	cvCvtColor(img_bin_t, img_color_t,CV_GRAY2BGR);

	f_t = BlockAnalysis::getFeatureBlock(img_bin_t,img_color_t);

	predict_in_out_block_output(img_color_t, _path, _path_sub, file_name_t, f_t, _in_out, _pos_neg,_th);

	cvReleaseImage(&img_bin_t);
	cvReleaseImage(&img_color_t);

	return f_t;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
std::vector<float> GetCrackImgBlockHoughFeature(std::string _img_file,
	std::string _in_out,
	std::string _path,
	std::string _path_sub,
	int _pos_neg,
	std::vector<float> _th)
{
	std::cout << _img_file << std::endl;

	std::vector<float> f_t;
	std::string file_name_t = Base::FS_getFileName(_img_file);

	IplImage* img_bin_t = cvLoadImage(_img_file.c_str(), CV_LOAD_IMAGE_ANYCOLOR);
	IplImage* img_color_t = cvCreateImage(cvGetSize(img_bin_t), IPL_DEPTH_8U, 3);

	cvCvtColor(img_bin_t, img_color_t, CV_GRAY2BGR);

#if TRUE
	//block
	std::string path_block_sub="block.az.output.00";
	f_t = BlockAnalysis::getFeatureBlock(img_bin_t, img_color_t);
	int	block_result=predict_in_out_block_output(img_color_t, _path, _path_sub, file_name_t, f_t, _in_out, _pos_neg, _th);
#endif // TRUE

#if TRUE
	//hough
	if (block_result== CrackDetection::POS_CLASS)
	{
		//有伤痕，跳过

	}
	else if (block_result== CrackDetection::NEG_CLASS)
	{
		//无伤痕，团块分析

		ImageProcess::getHoughLines(img_bin_t,img_color_t);

	}
	else
	{

	}
#endif // TRUE
	
#if TRUE
	//block

#endif // TRUE



	cvReleaseImage(&img_bin_t);
	cvReleaseImage(&img_color_t);

	return f_t;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/


/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
std::vector<float> doOneImg_block_hough_block(std::string _img_file,
	std::string _in_out,
	std::string _path,
	std::string _path_sub_output,
	int _pos_neg,
	std::vector<float> _th)
{
	std::cout << _img_file << std::endl;

	std::vector<float> f_t;
	std::string file_name_t = Base::FS_getFileName(_img_file);

	const int R_DEGREE = 45;

	IplImage* img_bin_t = cvLoadImage(_img_file.c_str(), CV_LOAD_IMAGE_ANYCOLOR);
	IplImage* img_color_t = cvCreateImage(cvGetSize(img_bin_t), IPL_DEPTH_8U, 3);

	cvCvtColor(img_bin_t, img_color_t, CV_GRAY2BGR);

	{

		f_t= BlockAnalysis::getFeatureBlock(img_bin_t, img_color_t);

		const int pos_neg_result_t= predict_in_out_block_output(img_color_t, _path, _path_sub_output, file_name_t, f_t, _in_out, _pos_neg,_th);

	}
	cvReleaseImage(&img_bin_t);
	cvReleaseImage(&img_color_t);

	return f_t;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void print_tree_level(int _level,std::string _path)
{

	while (_level-->0) {
		std::cout << "-" ;
	}

	std::cout << _path << std::endl;

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void print_tree()
{
		print_tree_level(0, "root");
			print_tree_level(1, "crack.roller.*");
				print_tree_level(2, "sample");
					print_tree_level(3, "in | out");
						print_tree_level(4, "ipAddr.*");
							print_tree_level(5, "time.*");
								print_tree_level(6, "ch*n.*");
								print_tree_level(6, "ch*p.*");
	
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void FS_getDirs4Dirs(std::vector<std::string> _in_dirs,int _idx,std::string _filter, std::vector<std::string> &_out)
{
	std::string  in_dir_t = _in_dirs.at(_idx);
	std::cout << in_dir_t << std::endl;
	Base::FS_getDirs(in_dir_t,_filter, _out);
}

/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void get_sample_data(std::string  _path, 
	std::string _in_out, 
	std::vector<std::string>& _pos_imgs,
	std::vector<std::string>& _neg_imgs)
{
	
	std::vector<std::string> sets_t;

	Base::FS_getDirs(_path, CRACK_ROLLER_SET, sets_t);

	for (size_t seti = 0; seti < sets_t.size(); seti++) {

		std::vector<std::string> samples_t;
		FS_getDirs4Dirs(sets_t, seti, CRACK_SAMPLE, samples_t);

		if (samples_t.size() == 1) {
			std::vector<std::string> ins_outs_t;
			FS_getDirs4Dirs(samples_t, 0, _in_out, ins_outs_t);

			if (ins_outs_t.size() == 1) {

				std::vector<std::string> ip_addrs_t;
				FS_getDirs4Dirs(ins_outs_t, 0, "", ip_addrs_t);

				for (size_t ipi = 0; ipi < ip_addrs_t.size(); ipi++) {
					std::vector<std::string> ip_times_t;
					FS_getDirs4Dirs(ip_addrs_t, ipi, "", ip_times_t);

					for (size_t ti = 0; ti <ip_times_t.size(); ti++) {
						std::vector<std::string> chs_t;
						FS_getDirs4Dirs(ip_times_t, ti, CRACK_CHANNEL, chs_t);

						for (size_t chi = 0; chi < chs_t.size(); chi++) {
							std::string ch_path_t = chs_t.at(chi);

							std::vector<std::string> imgs_t;
							Base::FS_getFiles(ch_path_t, ".png", imgs_t);

							if (IsPosSample(ch_path_t))
							{
								Base::STD_push_back(_pos_imgs,imgs_t);
							}
							else
							{
							
								Base::STD_push_back(_neg_imgs,imgs_t);
							}


						}


					}

				}

			}


		}


	}

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void generateFeatureData(std::string  _path, 
	std::string _in_out,
	const std::string feature_file,
	const std::string classify_file)
{
	
	std::vector<std::string> pos_imgs_t;
	std::vector<std::string> neg_imgs_t;

	get_sample_data(_path,_in_out,pos_imgs_t,neg_imgs_t);
	
	int feature_dimension = 0;
	
	std::vector<float>	feature_t;
	std::vector<int>	classify_t;
	
	for (size_t pi = 0; pi <pos_imgs_t.size(); pi++)
	{
			std::vector<float> f_p_t=GetCrackImgFeature(pos_imgs_t.at(pi));
			Base::STD_push_back(feature_t, f_p_t);
			classify_t.push_back(CrackDetection::POS_CLASS);
			feature_dimension =f_p_t.size();
	}

	for (size_t ni = 0; ni < neg_imgs_t.size(); ni++)
	{
			 std::vector<float> f_n_t=GetCrackImgFeature(neg_imgs_t.at(ni));
			 Base::STD_push_back(feature_t, f_n_t);
			 classify_t.push_back(CrackDetection::NEG_CLASS);
			 feature_dimension = f_n_t.size();
	}

	ImageProcess::Svm_Lean_Save_Feature2TXT(feature_t,classify_t,feature_dimension,_path);

	ImageProcess::Svm_Lean_SAVE_VECTOR (feature_t, feature_dimension,classify_t,
		GetResultFilePath(_path, feature_file, _in_out),
		GetResultFilePath(_path, classify_file, _in_out));
	
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void generateHoughData(std::string  _path,
	std::string _in_out,
	std::string _path_sub_output,
	const std::string feature_file,
	const std::string classify_file)
{
	
	std::vector<std::string> pos_imgs_t;
	std::vector<std::string> neg_imgs_t;

	get_sample_data(_path, _in_out, pos_imgs_t, neg_imgs_t);

	int feature_dimension = 0;

	std::vector<float>	feature_t;
	std::vector<int>	classify_t;

	for (size_t pi = 0; pi <pos_imgs_t.size(); pi++)
	{
		std::vector<float> f_p_t = GetCrackImgHoughFeature(pos_imgs_t.at(pi),_path, CrackDetection::POS_CLASS);
		Base::STD_push_back(feature_t, f_p_t);
		classify_t.push_back(CrackDetection::POS_CLASS);
		feature_dimension = f_p_t.size();
	}

	for (size_t ni = 0; ni < neg_imgs_t.size(); ni++)
	{
		std::vector<float> f_n_t = GetCrackImgHoughFeature(neg_imgs_t.at(ni),_path, CrackDetection::NEG_CLASS);
		Base::STD_push_back(feature_t, f_n_t);
		classify_t.push_back(CrackDetection::NEG_CLASS);
		feature_dimension = f_n_t.size();
	}

#if TRUE

	ImageProcess::Svm_Lean_Save_Feature2TXT(feature_t, classify_t, feature_dimension, _path);

	ImageProcess::Svm_Lean_SAVE_VECTOR(feature_t, feature_dimension, classify_t,
		GetResultFilePath(_path, feature_file, _in_out),
		GetResultFilePath(_path, classify_file, _in_out));

#endif // TRUE



}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void generateHoughBlockData(std::string  _path,
	std::string _in_out,
	std::string _path_sub_output,
	const std::string feature_file,
	const std::string classify_file,
	std::vector<float> _th)
{
	int tp_t = 0, fp_t = 0, fn_t = 0, tn_t = 0;

	std::vector<std::string> pos_imgs_t;
	std::vector<std::string> neg_imgs_t;

	get_sample_data(_path, _in_out, pos_imgs_t, neg_imgs_t);

	

	std::vector<float>	feature_t;
	std::vector<int>	classify_t;

	for (size_t pi = 0; pi <pos_imgs_t.size(); pi++)
	{
		std::vector<float> f_p_t = GetCrackImgHoughBlockFeature(pos_imgs_t.at(pi),_in_out, _path,_path_sub_output, CrackDetection::POS_CLASS,_th);
				
		if (0 != Base::Math_GetSumF(f_p_t.data(), f_p_t.size())) {
				Base::STD_push_back(feature_t, f_p_t);
				classify_t.push_back(CrackDetection::POS_CLASS);
		}
		
		const int pos_neg_block_result = CrackDetection::predict_in_out_block(f_p_t, _in_out, _th);
		ImageProcess::SampleCalPos(tp_t, fn_t, CrackDetection::POS_CLASS, pos_neg_block_result);


	}

	for (size_t ni = 0; ni < neg_imgs_t.size(); ni++)
	{
		std::vector<float> f_n_t = GetCrackImgHoughBlockFeature(neg_imgs_t.at(ni), _in_out, _path,  _path_sub_output, CrackDetection::NEG_CLASS,_th);
		
		if (0 != Base::Math_GetSumF(f_n_t.data(), f_n_t.size())) {
			Base::STD_push_back(feature_t, f_n_t);
			classify_t.push_back(CrackDetection::NEG_CLASS);
		}
		
		const int pos_neg_block_result = CrackDetection::predict_in_out_block(f_n_t, _in_out, _th);

		ImageProcess::SampleCalNeg(tn_t, fp_t, CrackDetection::NEG_CLASS, pos_neg_block_result);

	}


	Save_Feature_Recall_Precision(_path,
		_path_sub_output,
		_in_out,
		"hough.block.feature",
		tp_t,
		fp_t,
		fn_t,
		feature_t,
		classify_t);


}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void generateBlockData(std::string  _path,
	std::string _in_out,
	std::string _path_sub_output,
	const std::string feature_file,
	const std::string classify_file,
	std::vector<float> _th)
{
	int tp_t = 0, fp_t = 0, fn_t = 0, tn_t = 0;
	std::vector<std::string> pos_imgs_t;
	std::vector<std::string> neg_imgs_t;

	get_sample_data(_path, _in_out, pos_imgs_t, neg_imgs_t);

	int feature_dimension = 0;

	std::vector<float>	feature_t;
	std::vector<int>	classify_t;

	for (size_t pi = 0; pi <pos_imgs_t.size(); pi++)
	{
		std::vector<float> f_p_t = GetCrackImgBlockFeature(pos_imgs_t.at(pi),_in_out, _path, _path_sub_output, CrackDetection::POS_CLASS,_th);
		Base::STD_push_back(feature_t, f_p_t);
		classify_t.push_back(CrackDetection::POS_CLASS);
		feature_dimension = f_p_t.size();

		int result = CrackDetection::predict_in_out_block(f_p_t, _in_out, _th);
		
		ImageProcess::SampleCalPos(tp_t, fn_t, CrackDetection::POS_CLASS, result);
		
	}

	for (size_t ni = 0; ni < neg_imgs_t.size(); ni++)
	{
		std::vector<float> f_n_t = GetCrackImgBlockFeature(neg_imgs_t.at(ni),_in_out, _path, _path_sub_output, CrackDetection::NEG_CLASS,_th);
		Base::STD_push_back(feature_t, f_n_t);
		classify_t.push_back(CrackDetection::NEG_CLASS);
		feature_dimension = f_n_t.size();

		int result = CrackDetection::predict_in_out_block(f_n_t, _in_out, _th);
		
		ImageProcess::SampleCalNeg(tn_t, fp_t, CrackDetection::NEG_CLASS, result);
	}

	std::string block_base_path_t = Base::FS_createPath(_path, _path_sub_output);

	ImageProcess::Save_Feature2TXT(feature_t, classify_t, feature_dimension, "block.feature", block_base_path_t);

	ImageProcess::Svm_Lean_SAVE_Recall_Precision(tp_t,
		fp_t,
		fn_t,
		GetResultFilePath(block_base_path_t, CRACK_RECALL_PRECISION_TXT, _in_out));
	
}

/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void generateBlockHoughData(std::string  _path,
	std::string _in_out,
	std::string _path_sub_output,
	const std::string feature_file,
	const std::string classify_file,
	std::vector<float> _th)
{
	int tp_t = 0, fp_t = 0, fn_t = 0, tn_t = 0;
	std::vector<std::string> pos_imgs_t;
	std::vector<std::string> neg_imgs_t;

	get_sample_data(_path, _in_out, pos_imgs_t, neg_imgs_t);

	int feature_dimension = 0;

	std::vector<float>	feature_t;
	std::vector<int>	classify_t;

	for (size_t pi = 0; pi <pos_imgs_t.size(); pi++)
	{
		std::vector<float> f_p_t = GetCrackImgBlockHoughFeature(pos_imgs_t.at(pi), _in_out, _path, _path_sub_output, CrackDetection::POS_CLASS, _th);
		Base::STD_push_back(feature_t, f_p_t);
		classify_t.push_back(CrackDetection::POS_CLASS);
		feature_dimension = f_p_t.size();

		int result = CrackDetection::predict_in_out_block(f_p_t, _in_out, _th);
		ImageProcess::SampleCalPos(tp_t, fn_t, CrackDetection::POS_CLASS, result);


	}

	for (size_t ni = 0; ni < neg_imgs_t.size(); ni++)
	{
		std::vector<float> f_n_t = GetCrackImgBlockHoughFeature(neg_imgs_t.at(ni), _in_out, _path, _path_sub_output, CrackDetection::NEG_CLASS, _th);
		Base::STD_push_back(feature_t, f_n_t);
		classify_t.push_back(CrackDetection::NEG_CLASS);
		feature_dimension = f_n_t.size();

		int result = CrackDetection::predict_in_out_block(f_n_t, _in_out, _th);
		ImageProcess::SampleCalNeg(tn_t, fp_t, CrackDetection::NEG_CLASS, result);

	}


	Save_Feature_Recall_Precision(_path,
		_path_sub_output,
		_in_out,
		"block.hough.feature",
		tp_t,
		fp_t,
		fn_t,
		feature_t,
		classify_t);


}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void generate_Block_hough_block_Data(std::string  _path,
	std::string _in_out,
	std::string _path_sub_output,
	const std::string feature_file,
	const std::string classify_file,
	std::vector<float> _th)
{
	
	std::vector<std::string> pos_imgs_t;
	std::vector<std::string> neg_imgs_t;

	get_sample_data(_path, _in_out, pos_imgs_t, neg_imgs_t);

	int feature_dimension = 0;

	std::vector<float>	feature_t;
	std::vector<int>	classify_t;

	for (size_t pi = 0; pi <pos_imgs_t.size(); pi++)
	{
		std::vector<float> f_p_t = doOneImg_block_hough_block(pos_imgs_t.at(pi), _in_out, _path,_path_sub_output, CrackDetection::POS_CLASS,_th);
		Base::STD_push_back(feature_t, f_p_t);
		classify_t.push_back(CrackDetection::POS_CLASS);
		feature_dimension = f_p_t.size();
	}

	for (size_t ni = 0; ni < neg_imgs_t.size(); ni++)
	{
		std::vector<float> f_n_t = doOneImg_block_hough_block(neg_imgs_t.at(ni), _in_out, _path,_path_sub_output, CrackDetection::NEG_CLASS,_th);
		Base::STD_push_back(feature_t, f_n_t);
		classify_t.push_back(CrackDetection::NEG_CLASS);
		feature_dimension = f_n_t.size();
	}

	std::string hough_base_path_t = Base::FS_createPath(_path, _path_sub_output);

	ImageProcess::Save_Feature2TXT(feature_t, classify_t, feature_dimension, _path_sub_output, hough_base_path_t);

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void run_svm(std::string  _path,
	std::string _in_out,
	const std::string feature_file,
	const std::string classify_file,
	std::string _module_xml)
{
	
	ImageProcess::Svm_Lean_F(GetResultFilePath(_path, feature_file, _in_out),
							 GetResultFilePath(_path, classify_file, _in_out),
							CvSVM::RBF,
							_path,
							GetResultFilePath("",_module_xml, _in_out));

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void run_svm_test(
	std::string  _path_tran_root,
	std::string  _path_test_root,
	std::string _in_out,
	std::string _module_xml)
{
	std::string path_fp = Base::FS_createPath(_path_test_root, "fp");
	std::string path_fn = Base::FS_createPath(_path_test_root, "fn");

	CvSVM svm;
	
	svm.load(GetResultFilePath(_path_tran_root,_module_xml,_in_out).c_str());

	std::vector<std::string> pos_imgs_t;
	std::vector<std::string> neg_imgs_t;

	get_sample_data(_path_test_root, _in_out, pos_imgs_t, neg_imgs_t);

	int feature_size = 0;
	int tp_t = 0, fp_t = 0, fn_t = 0,tn_t=0;
	for (size_t pi = 0; pi <pos_imgs_t.size(); pi++)
	{
		std::string file_full_name_t = pos_imgs_t.at(pi);
		std::vector<float> f_p_t = GetCrackImgFeature(file_full_name_t);
		feature_size = f_p_t.size();
		int result=ImageProcess::Svm_Lean_Predict(svm, f_p_t);
		ImageProcess::SampleCalPos(tp_t, fn_t, CrackDetection::POS_CLASS, result);
	}

	for (size_t ni = 0; ni < neg_imgs_t.size(); ni++)
	{
		std::string file_full_name_t = neg_imgs_t.at(ni);
		std::vector<float> f_n_t = GetCrackImgFeature(file_full_name_t);
		feature_size = f_n_t.size();
		int result=ImageProcess::Svm_Lean_Predict(svm, f_n_t);
		ImageProcess::SampleCalNeg(tn_t, fp_t, CrackDetection::NEG_CLASS, result);
	}

	ImageProcess::Svm_Lean_SAVE_Recall_Precision(tp_t, 
		fp_t,
		fn_t,
		GetResultFilePath(_path_test_root, CRACK_RECALL_PRECISION_TXT, _in_out));


}

/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void run_svm_classify(std::string  _path_tran_root,
	std::string  _path_classify_root,
	std::string _in_out,
	std::string _module_xml)
{
	const int POS_CLASS = 1;
	const int NEG_CLASS = -1;

	CvSVM svm;

	svm.load(GetResultFilePath(_path_tran_root, _module_xml, _in_out).c_str());

	std::vector<std::string> pos_imgs_t;
	std::vector<std::string> neg_imgs_t;

	get_sample_data(_path_classify_root, _in_out, pos_imgs_t, neg_imgs_t);
		
	for (size_t pi = 0; pi <pos_imgs_t.size(); pi++)
	{
		std::string file_full_name_t = pos_imgs_t.at(pi);	
		std::vector<float> f_p_t = GetCrackImgFeature(file_full_name_t);		
		int result = ImageProcess::Svm_Lean_Predict(svm, f_p_t);
		if (result == POS_CLASS) {

		}
		else if (result == NEG_CLASS) {
			  MoveImg2AnotherSide(file_full_name_t);
		}
		else {
			ASSERT(0);
		}
	}

	for (size_t ni = 0; ni < neg_imgs_t.size(); ni++)
	{
		const std::string file_full_name_t = neg_imgs_t.at(ni);
		std::vector<float> f_n_t = GetCrackImgFeature(file_full_name_t);
		int result = ImageProcess::Svm_Lean_Predict(svm, f_n_t);
		
		if (result == POS_CLASS) {
			MoveImg2AnotherSide(file_full_name_t);
		}
		else if (result == NEG_CLASS) {
			
		}
		else {
			ASSERT(0);
		}
	}

	


}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
int print_do_dataset_list()
{
	int test_num;
	std::cout << "Please Input IN/OUT Dataset:" << std::endl;
	
	std::cout << "1 dataset [in]  ! " << std::endl;
	std::cout << "2 dataset [out]  ! " << std::endl;

	std::cin >> test_num;

	return test_num;

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
int print_do_task_list()
{
	int test_num=0;

	std::cout << "Please Input Execute Item:" << std::endl;
	
	std::cout << "1 [svm]  ! " << std::endl;
	
	std::cout << "2 [hough]  ! " << std::endl;
	
	std::cout << "3 [hough-->block]   ! " << std::endl;

	std::cout << "4 [block--> hough-->block]   ! " << std::endl;

	std::cout << "5 [block  Analysis]   ! " << std::endl;

	std::cout << "6 [exchange pos/neg sample]  ! " << std::endl;
	
	std::cout << "7 [recovery pos/neg sample]  ! " << std::endl;
	
	std::cin >> test_num;

	return test_num;

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void print_do_svm_task_list() 
{
	std::cout << "Please Input Execute Item:" << std::endl;
	std::cout << "1 do  date gen ! " << std::endl;
	
	std::cout << "3 do [svm]  date gen ! " << std::endl;

	std::cout << "5 do [svm][recall-precision]  date gen ! " << std::endl;

	std::cout << "7 do [svm][classify]  date gen ! " << std::endl;


}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void do_svm_task(int _task,
	std::string _tran_root,
	std::string _test_root,
	std::string _classify_root,
	std::string _feature,
	std::string _classify,
	std::string _module,
	std::string _in_out)
{
	print_do_svm_task_list();

	if (_task == 1) {
		generateFeatureData(_tran_root, _in_out, _feature, _classify);
	}
	else if (_task == 2) {
		
	}
	else if (_task == 3) {
		run_svm(_tran_root, _in_out, _feature, _classify, _module);
	}
	else if (_task == 4) {
		
	}
	else if (_task == 5) {
		run_svm_test(_tran_root, _test_root,_in_out, _module);
	}
	else if (_task == 6) {
		
	}
	else if (_task == 7) {
		run_svm_classify(_tran_root, _classify_root, _in_out, _module);
	}
	else if (_task == 8) {
	
	}
	else {

	}
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void exchange_P_N(std:: string _path_root,std::string _in_out)
{

	std::vector<std::string> pos_imgs_t;
	std::vector<std::string> neg_imgs_t;

	get_sample_data(_path_root, _in_out, pos_imgs_t, neg_imgs_t);

	int feature_size = 0;

	std::vector<float>	feature_t;
	std::vector<int>	classify_t;

	for (size_t pi = 0; pi < pos_imgs_t.size(); pi++)
	{
		const std::string file_full_name_t = pos_imgs_t.at(pi);
		MoveImg2AnotherSide(file_full_name_t);
	}

	for (size_t ni = 0; ni <neg_imgs_t.size(); ni++)
	{
		const std::string file_full_name_t = neg_imgs_t.at(ni);
		MoveImg2AnotherSide(file_full_name_t);
	}
	
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
std::string recoveryFile(std::string _root_path,std::string _in_out,const std::string _img_t,int _pos_neg)
{
	std::string path_t;
	const std::string file_full_name_t = Base::FS_getFileName(_img_t);

	std::vector<std::string>  img_property_t= Base::split(file_full_name_t, ".");

	std::string ip_addr_t=img_property_t.at(0)+"."+img_property_t.at(1)+"."+img_property_t.at(2)+"."+img_property_t.at(3);
	std::string time_t= img_property_t.at(4) + "." + img_property_t.at(5);
	std::string channel_t=img_property_t.at(6);
	
	std::string path_crack_roller = Base::FS_createPath(_root_path, CRACK_ROLLER_SET +Base::TIME_getCurrentTime("%Y%m%d"));
	std::string path_sample = Base::FS_createPath(path_crack_roller, CRACK_SAMPLE);
	std::string path_in_out = Base::FS_createPath(path_sample, _in_out);
	std::string path_ip = Base::FS_createPath(path_in_out, ip_addr_t);
	std::string path_time = Base::FS_createPath(path_ip, time_t);
	
	std::string p_n;
	if (_pos_neg == CrackDetection::POS_CLASS) {
		p_n =CRACK_CHANNEL_P ;
	}else if (_pos_neg == CrackDetection::NEG_CLASS) {
		p_n = CRACK_CHANNEL_N;
	}else {
		ASSERT(0);
	}

	std::string path_channel = Base::FS_createPath(path_time, channel_t+p_n);

	std::string img_save_full_path = path_channel + file_full_name_t;


	Base::FS_copyFileSTL(_img_t,img_save_full_path);
	
	return path_t;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void recovery_P_N(std::string _path_root, std::string _in_out)
{

	std::vector<std::string> pos_imgs_t;
	std::vector<std::string> neg_imgs_t;

	std::string POS="pos";
	std::string NEG="neg";
	
	std::string path_root_pos=Base::FS_createPath(_path_root,POS,0);
	std::string path_root_neg= Base::FS_createPath(_path_root, NEG, 0);

	Base::FS_getFiles(path_root_pos, ".png", pos_imgs_t);

	Base::FS_getFiles(path_root_neg, ".png", neg_imgs_t);

	for (size_t pi = 0; pi < pos_imgs_t.size(); pi++)
	{
		recoveryFile(_path_root,_in_out, pos_imgs_t.at(pi), CrackDetection::POS_CLASS);
	}
	
	for (size_t ni = 0; ni <neg_imgs_t.size(); ni++)
	{
		recoveryFile(_path_root, _in_out, neg_imgs_t.at(ni), CrackDetection::NEG_CLASS);
	}

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void do_task(std::string _tran_root,
	std::string _test_root,
	std::string _classify_root,
	std::string _feature,
	std::string _classify,
	std::string _module,
	std::string _in_out,
	std::string _output_sub_path,
	const int _task,
	std::vector<float> _th)
{

	if (_task == 1) {
		generateHoughData(_tran_root, _in_out, _output_sub_path, _feature, _classify);
	}
	else if (_task == 2) {
		generateBlockData(_tran_root, _in_out, _output_sub_path, _feature, _classify,_th);
	}
	else if (_task == 3) {
		generateBlockHoughData(_tran_root, _in_out, _output_sub_path, _feature, _classify, _th);
	}
	else if (_task == 4) {
		generateHoughBlockData(_tran_root, _in_out, _output_sub_path, _feature, _classify, _th);
	}
	else if (_task == 5) {

	}
	else if (_task == 6) {

	}
	else if (_task == 7) {

	}
	else if (_task == 8) {

	}
	else {

	}
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void do_hough_task(std::string _tran_root,
	std::string _test_root,
	std::string _classify_root,
	std::string _feature,
	std::string _classify,
	std::string _module,
	std::string _in_out)
{	
	std::string output_sub_path_t = ("hough.analyze.output");
	std::vector<float> th_t;

	do_task(_tran_root,
		_test_root,
		_classify_root,
		_feature,
		_classify,
		_module,
		_in_out,
		output_sub_path_t,
		1,th_t);	
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void do_block_task(std::string _tran_root,
	std::string _test_root,
	std::string _classify_root,
	std::string _feature,
	std::string _classify,
	std::string _module,
	std::string _in_out)
{
	
	std::string output_sub_path_t = ("block.analyze.output");
	std::vector<float> th_t;
	
	if (_in_out == CrackDetection::CRACK_IN){
		th_t = CrackDetection::getThresholdInCircleBlock();
	}else if (_in_out == CrackDetection::CRACK_OUT) {
		assert(0);
	}else{
		assert(0);
	}
	
	do_task(_tran_root,
		_test_root,
		_classify_root,
		_feature,
		_classify,
		_module,
		_in_out,
		output_sub_path_t,
		2,
		th_t);

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void do_hough_block(std::string _tran_root,
	std::string _test_root,
	std::string _classify_root,
	std::string _feature,
	std::string _classify,
	std::string _module,
	std::string _in_out)
{
	int _task = 4;
	std::string output_sub_path_t;
	std::vector<float> th_t;
	if (_task == 1) {

		output_sub_path_t = ("hough.analyze.output");

	}
	else if (_task == 2) {

		output_sub_path_t = ("block.analyze.output.00");
		th_t = CrackDetection::getThresholdOutCircleBlock();
		
	}
	else if (_task == 3) {
		output_sub_path_t = ("block.hough.block.output");
	}
	else if (_task == 4) {
		output_sub_path_t = ("hough.block.output");
		th_t = CrackDetection::getThresholdInOutCircleHoughBlock(_in_out);
	}
	else {

	}

	do_task(_tran_root,
		_test_root,
		_classify_root,
		_feature,
		_classify,
		_module,
		_in_out,
		output_sub_path_t,
		_task,
		th_t);

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void do_block_hough_block(std::string _tran_root,
	std::string _test_root,
	std::string _classify_root,
	std::string _feature,
	std::string _classify,
	std::string _module,
	std::string _in_out)
{
	int _task = 4;
	std::string output_sub_path_t;
	std::vector<float> th_t;
	if (_task == 1) {		
	
		output_sub_path_t = ("hough.analyze.output");
	
	}else if (_task == 2) {
	
		output_sub_path_t = ("block.analyze.output.00");
		th_t = CrackDetection::getThresholdOutCircleBlock();
	
	}else if(_task ==3){
	
		output_sub_path_t = ("block.hough.analyze.output");
	
	}else if (_task == 4){
		output_sub_path_t = ("hough.block.analyze.output");
	}else{

	}

	do_task(_tran_root,
		_test_root,
		_classify_root,
		_feature,
		_classify,
		_module,
		_in_out,
		output_sub_path_t,
		_task,
		th_t);
	
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
int main(int argc, char *argv[])
{
	int EXEC_RESULT = 0;
	std::string  path_tran_root;
	std::string  path_test_root;
	std::string  path_classify_root;

	const std::string  feature = "svm.feature.learn.crack.xml";
	const std::string  classify = "svm.classifies.learn.crack.xml";
	const std::string  module = "svm.module.learn.crack.xml";
	
	const std::string  path_tran_root_out = "X:\\ImageDataBase\\crack.roller.dataset.analyze.out\\crack.roller.dataset.out.hf.tran.01\\";//训练集
	const std::string  path_tran_root_in = "X:\\ImageDataBase\\crack.roller.dataset.analyze.in\\crack.roller.dataset.in.hf.tran\\";//训练集


	const std::string  path_test_root_in = "";
	const std::string  path_classify_root_in = "";

	const std::string  path_test_root_out = "";
	const std::string  path_classify_root_out = "";

		
	print_tree();
#if TRUE
	int in_out = print_do_dataset_list();
	std::string in_out_dataset="none";
	if (in_out==1){
		in_out_dataset = CrackDetection::CRACK_IN;
		path_tran_root = path_tran_root_in;

	}else if (in_out == 2) {
		in_out_dataset = CrackDetection::CRACK_OUT;
		path_tran_root = path_tran_root_out;

	}else{
		exit(0);
	}
#endif // TRUE
		
	int test_num = print_do_task_list();
	
	if (test_num == 1) {
		do_svm_task(test_num,
			path_tran_root,
			path_test_root,
			path_classify_root,
			feature,
			classify,
			module,
			in_out_dataset);
	}
	else if (test_num == 2) {
		do_hough_task(path_tran_root,
			path_test_root,
			path_classify_root,
			feature,
			classify,
			module,
			in_out_dataset);
	}
	else if (test_num == 3) {

		do_hough_block(path_tran_root,
			path_test_root,
			path_classify_root,
			feature,
			classify,
			module,
			in_out_dataset);


	}
	else if (test_num == 4) {

		do_block_hough_block(path_tran_root,
			path_test_root,
			path_classify_root,
			feature,
			classify,
			module,
			in_out_dataset);

	}
	else if (test_num == 5) {

		do_block_task(path_tran_root,
			path_test_root,
			path_classify_root,
			feature,
			classify,
			module,
			in_out_dataset);

	}
	else if (test_num == 6) {

#if TRUE
		assert(0);
		exchange_P_N(path_tran_root, in_out_dataset);
#endif // 0

	}
	else if (test_num == 7) {

		recovery_P_N("X:\\ImageDataBase\\second\\",in_out_dataset);

	}
	else if (test_num == 8) {
		
	}
	else {

	}
	

	return  EXEC_RESULT;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/