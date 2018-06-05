#include "cpp_stl.h"
#include "opencv_stl.h"
/*---------------------------------------*/

/*---------------------------------------*/
#include "MY_SDK_LIB/Base.h"
#include "Harr/CuiHarrTransformLet.h"
#include "Harr/HarrTransformUser.hpp"
/*---------------------------------------*/
/**
*
*
*/
/*---------------------------------------*/
float GetThresholdTter(float * _data,int _size)
{


	float max_v=Base::Math_GetMaxValueF(_data,_size);
	float min_v=Base::Math_GetMinValueF(_data, _size);
	float Threshold = (max_v + min_v)/2.0;
	float threshold_new = 0;
	int times=0;
	std::vector<float> max_datas;
	std::vector<float> min_datas;
	
	for (;times<1000;times++) {
		
			max_datas.clear();
			min_datas.clear();
#if TRUE
		
		
		for (size_t i = 0; i <_size; i++)
		{

			if (_data[i] > Threshold) {
				max_datas.push_back(_data[i]);
			}else {
				min_datas.push_back(_data[i]);
			}

		}

		max_v = Base::Math_GetAverageValueF(max_datas.data(),max_datas.size());
		min_v = Base::Math_GetAverageValueF(min_datas.data(), min_datas.size());

		threshold_new = (max_v + min_v)/2.0;

		if (fabs(threshold_new - Threshold) < 0.5){
			break;
		}
		else
		{
			Threshold = threshold_new;
		}


#endif // TRUE
		
	
	} 


	return threshold_new;
 
}
/*---------------------------------------*/
/**
*
*
*/
/*---------------------------------------*/
CvRect getDetectionArea(IplImage* img_org)
{
	CvRect rect_detection;
	ASSERT(img_org!=NULL);
	ASSERT(img_org->nChannels==1);

	const int HEIGHT = img_org->height;
	const int WIDTH  = img_org->width;

	std::vector<std::vector<float>> datas;	

#if TRUE
	
	datas.resize(HEIGHT);
	
	for (size_t wi = 0; wi <WIDTH; wi++){

		for (size_t hi = 0; hi < HEIGHT; hi++){
			
			float pixel_data_t = cvGetReal2D(img_org, hi, wi);

			datas[hi].push_back(pixel_data_t);

		}
	}

#endif // TRUE

	std::vector<float> avg_h;
#if TRUE
	avg_h.resize(HEIGHT);

	for (size_t hi = 0; hi <HEIGHT; hi++){
		std::vector<float> data_row = datas.at(hi);
		float avg= Base::Math_GetAverageValueF(data_row.data(),data_row.size());
		avg_h[hi] = avg;
	}
#endif // TRUE

	float threshold= GetThresholdTter(avg_h.data(),avg_h.size());
	vector<float> pos;
	for (size_t hi = 0; hi <HEIGHT; hi++) {

		if (avg_h[hi] >= threshold) {
			pos.push_back(hi);
		}

	}

	float pos_max = Base::Math_GetMaxValueF(pos.data(), pos.size());
	float pos_min = Base::Math_GetMinValueF(pos.data(), pos.size());



	rect_detection.x = 0;
	rect_detection.y = pos_min;

	rect_detection.height = pos_max-pos_min;
	rect_detection.width = WIDTH;
	
	ASSERT(rect_detection.width  > 64);
	ASSERT(rect_detection.height > 64);

	do
	{


		if (rect_detection.width%4!=0)
		{
			rect_detection.width--;
		}

		if (rect_detection.height%4!=0)
		{
			rect_detection.height--;
		}


		if ((rect_detection.width % 4 == 0) &&
			(rect_detection.height % 4 == 0)) {
			
			break;
		
		}

	} while (TRUE);


	return rect_detection;

}
/*---------------------------------------*/
/**
*
*
*/
/*---------------------------------------*/
Energy_Harr processHarrEnergy(IplImage* _img)
{


	CuiHarrTransformLet Harr_Origin;
	Energy_Harr harr_origin_energy;

	UINT32* Origin_img =(UINT32*) _img->imageData;
	
	const int img_width=_img->width;
	const int img_height=_img->height;

	ASSERT(img_width % 4  == 0);
	ASSERT(img_height % 4 == 0);
	ASSERT(_img->widthStep == _img->width*_img->nChannels);

	Harr_Origin.CuiGetImageData(Origin_img, img_width, img_height);
	Harr_Origin.CuiWaveletTrans(1);//0.4ms
	Harr_Origin.CuiCalculateEnergyALL();//2.9ms

	Harr_Origin.CuiSaveImg();


	Energy_Harr harr_energy = Harr_Origin.harr_energy;
	
	return harr_energy;


}
/*---------------------------------------*/
/**
*
*
*/
/*---------------------------------------*/
std::vector<float> processHarrEnergyUser(IplImage* _img)
{

	std::vector<float> feature;

	for (size_t i = 1; i <=100; i++)
	{
		HarrTransformUser Harr_Origin;
	
		Harr_Origin.setImageData(_img);

		float data_t=Harr_Origin.CalculateEnergy(i);
		
		feature.push_back(data_t);

	}

	


	

	return feature;


}
/*---------------------------------------*/
/**
*
*
*/
/*---------------------------------------*/
void file(std::string finalpath) 
{

ofstream outfile;
outfile.open(finalpath.c_str(), ios::out);
{
		outfile << endl;
		outfile <<"";
}
	

outfile.close();

}


/*---------------------------------------*/
/**
*
*
*/
/*---------------------------------------*/
int main(int argc, char *argv[])
{
	

	std::string  file_base = "X:\\MyProjectTemp\\Project\\yjkj\\FOXCONN\\0524";
	std::vector<std::string> files;
	std::string ext = "bmp";
	Base::FS_getFiles(file_base,ext, files);
	const string WINDOW_NAME = "TEST";
	cvNamedWindow(WINDOW_NAME.c_str(),CV_WINDOW_NORMAL);

	ofstream outfile;
	outfile.open("harr.txt", std::ios::out);

#if 0
	outfile << "fileName" << " "
		<<"IDX"<<" "
		<< "LL" << " "
		<< "LH" << " "
		<< "HL" << " "
		<< "HH" << " "
		<< std::endl;
#endif // 0


	for (size_t i = 0; i <files.size(); i++)
	{
	
		IplImage* img_org_t = cvLoadImage(files.at(i).c_str());
		std::string baseName = Base::base_name(files.at(i));
		int idx = Base::get_number(baseName);
		cui_GeneralImgProcess::ConvertImg3ChTo4Ch(&img_org_t);

		IplImage* img_gray_t = cvCreateImage(cvGetSize(img_org_t),IPL_DEPTH_8U,1);
		cvCvtColor(img_org_t, img_gray_t, CV_RGBA2GRAY);

		{
			CvRect rect_detection = getDetectionArea(img_gray_t);			
			
			IplImage* img_gray_detection = cvCreateImage(cvSize(rect_detection.width,rect_detection.height),IPL_DEPTH_8U,4);

			{
				cvSetImageROI(img_org_t, rect_detection);
			
				cvCopyImage(img_org_t,img_gray_detection);

				cvResetImageROI(img_org_t);

#if 0
				EnergyHarr eh= processHarrEnergy(img_gray_detection);
				


				outfile << files.at(i)<< " "
					<<idx<<" "
					<< eh.Energy_LL << " "
					<< eh.Energy_LH << " "
					<< eh.Energy_HL<< " "
					<< eh.Energy_HH << " "
					<< std::endl;

#endif // 0		

#if TRUE
			std::vector<float> feature_t = processHarrEnergyUser(img_gray_detection);
			
			outfile << files.at(i) << " "
					<< idx << " ";
				
				for (size_t i = 0; i <feature_t.size(); i++)
				{
					outfile << feature_t.at(i) << " ";
					
				}

			outfile << std::endl;
#endif


			
			}

			

			cvSaveImage((Base::int2str(idx)+".png").c_str(),img_gray_detection);
		
			
			cvReleaseImage(&img_gray_detection);

#if TRUE
			
			cvRectangleR(img_org_t, rect_detection, cvScalar(255, 0, 0), 2);
			cvShowImage(WINDOW_NAME.c_str(),img_org_t);
			cvWaitKey(10);

#endif		
		}
		cvReleaseImage(&img_org_t);
		cvReleaseImage(&img_gray_t);

	}


	outfile.close();


	return 0;


}