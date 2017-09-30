#include "stdafx.h"


void print_mat(string name_t,cv::Mat mat_t)
{

	std::cout <<name_t<< " = " << std::endl;
	std::cout << mat_t<<std::endl;

}

void capture_frame() {
	char FileName[80];
	memset(FileName, 0, sizeof FileName);
	string avi_1 = "C:\\Users\\cui-e460\\Desktop\\ch3_cal\\ch3.mp4out.avi";
	CvCapture* capture = cvCaptureFromAVI(avi_1.c_str());
	
	int i = 99;
	while (cvGrabFrame(capture))
	{
		i++;
		if (i % 100 == 0)
		{
			Mat img = cvRetrieveFrame(capture);        // retrieve the captured frame
			sprintf(FileName, "C:\\Users\\cui-e460\\Desktop\\ch3_cal\\002\\%d.png", i);
			imwrite(FileName, img);

		}
	}
	cvReleaseCapture(&capture);
}

void  inv_test()
{
	const int COLS = 1000;
	cv::Mat m1 = cv::Mat::eye(COLS, COLS, CV_32F);	

	m1.at<float>(0, 0) = 0.9;
	m1.at<float>(COLS - 1, COLS - 1) = 0.9;


	if(COLS<11) print_mat("m",m1);

	TimeCountStart();
			cv::Mat m_inv = m1.inv();

			
	TimeCountStop("inv cost time :");

	if(COLS<11) print_mat("m_inv",m_inv);

}


std::vector<std::string> b_split(const std::string &s, string delim)
{
	std::vector<std::string> v;
	std::string::size_type pos1, pos2;
	pos2 = s.find(delim);
	pos1 = 0;
	while (std::string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));

		pos1 = pos2 + delim.size();
		pos2 = s.find(delim, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));

	return v;
}

void parse_line2byte(string linedata,unsigned char*data)
{
	data[0] = 0x00;
	data[1] = 0x00;

	std::vector<std::string>  data_v=b_split(linedata, ",");

	int size_v=data_v.size();

	if (size_v == 19) {

		//unsigned char bit_ascii=  ;
		unsigned char bit = *(data_v[18].c_str());
		
		if (isdigit(bit)) {
			//有效行
			//低位8
			int  bit_0 = atoi(data_v[2].c_str());
			int  bit_1 = atoi(data_v[3].c_str())*2;
			int  bit_2 = atoi(data_v[4].c_str())*4;
			int  bit_3 = atoi(data_v[5].c_str())*8;
			int  bit_4 = atoi(data_v[6].c_str())*16;
			int  bit_5 = atoi(data_v[7].c_str())*32;
			int  bit_6 = atoi(data_v[8].c_str())*64;
			int  bit_7 = atoi(data_v[9].c_str())*128;
			//高8位
			int  bit_8  = atoi(data_v[11].c_str());
			int  bit_9  = atoi(data_v[12].c_str())*2;
			int  bit_10 = atoi(data_v[13].c_str())*4;
			int  bit_11 = atoi(data_v[14].c_str())*8;
			int  bit_12 = atoi(data_v[15].c_str())*16;
			int  bit_13 = atoi(data_v[16].c_str())*32;
			int  bit_14 = atoi(data_v[17].c_str())*64;
			int  bit_15 = atoi(data_v[18].c_str())*128;


			data[0] = bit_7 + bit_6 + bit_5 + bit_4 + bit_3 + bit_2 + bit_1 + bit_0;
			data[1] = bit_15 + bit_14 + bit_13 + bit_12 + bit_11 + bit_10 + bit_9 + bit_8;

				
		}
		

	}

}


std::string read_buffer2str(string file_name) {

	std::ifstream t(file_name);

	if (!t) {
		std::cout << "no file" << file_name<<std::endl;
	}
	std::stringstream buffer;
	buffer << t.rdbuf();
	std::string contents(buffer.str());
	return contents;

}

void statistics255(std::vector<unsigned char> image_data)
{
	vector<float> data255;

	data255.resize(256,0);


	for (int i = 0; i < image_data.size(); i++) {
	
		unsigned char idx = image_data.at(i);
		data255[idx]= data255[idx]+1;
	
	}


	std::ofstream out_yuv_row("E:\\bt1120-yuv\\statistics255.txt");
	
	for (size_t i = 0; i <data255.size(); i++){

	

		out_yuv_row << i << "," << 1.0*data255.at(i)/image_data.size() << std::endl;


	}
	out_yuv_row.flush();
	out_yuv_row.close();


}

void findff0000xy(std::vector<unsigned char> image_data)
{
	unsigned char *data_b = image_data.data();
	std::ofstream out_yuv_row("E:\\bt1120-yuv\\ff0000xy.txt");


	for (int i = 0; i < image_data.size(); i++) {

		if (
			data_b[i] == 0xff
			&& data_b[i + 1] == 0xff

			&& data_b[i + 2] == 0x00
			&& data_b[i + 3] == 0x00

			&& data_b[i + 4] == 0x00
			&& data_b[i + 5] == 0x00
			) {
				out_yuv_row <<std::hex <<
					(int) data_b[i]<< (int)data_b[i + 1]<<
					(int) data_b[i + 2]<< (int)data_b[i + 3] <<
					(int) data_b[i + 4] << (int)data_b[i + 5] <<
					(int) data_b[i + 6] << (int)data_b[i + 7] <<std::endl;

				
		}

	}
	out_yuv_row.flush();
	out_yuv_row.close();


}

void readyuvrowData(std::string file_name_t,std::string file_name_out_t)
{
	ifstream fileTEXT(file_name_t);
	
	
	
	string line_new;
	
	std::vector<unsigned char> image_data;
	//image_data.resize(1920*1080*2*10);
	
	image_data.reserve(1920 * 1080 * 2 * 10);

	int i = 0;
	unsigned char data[2];

	do {
	
		std::istream& flag=std::getline(fileTEXT, line_new);
		parse_line2byte(line_new,data);
		
		image_data.push_back(data[1]);//Cr Cb
		image_data.push_back(data[0]);//Y


	
		if (i++ % 50000==0) {
			std::cout <<"read line :"<< i<<std::endl;
		}

		if (flag.eof()){
			break;
		}
	
	}while (1);


	unsigned char *data_b=image_data.data();

	boolean write_flag = false;
	std::ofstream out_yuv_row(file_name_out_t);
#if 1
	for (int i = 0; i < image_data.size(); i++) {
		
		
		

		if (
			data_b[i] == 0xff
			&& data_b[i + 1] == 0xff

			&& data_b[i + 2] == 0x00
			&& data_b[i + 3] == 0x00

			&& data_b[i + 4] == 0x00
			&& data_b[i + 5] == 0x00

			//&& data_b[i + 6] == 0xAB
			&& data_b[i + 6] == 0xeb
			/*&& data_b[i + 7] == 0xAB*/

			) {
			write_flag =true;
		}
		
		
		if (
			data_b[i] == 0xff
			 &&data_b[i + 1] == 0xff

			&& data_b[i + 2] == 0x00
			&& data_b[i + 3] == 0x00

			&& data_b[i + 4] == 0x00
			&& data_b[i + 5] == 0x00

			&& data_b[i + 6] == 0x80
			&& data_b[i + 7] == 0x80

			) {

			if (1
#if 0
				//&& data_b[i + 2 + 1920*2] == 0xff
				&& data_b[i + 2 + 1920*2 + 1] == 0xff

				//&& data_b[i + 2 + 1920*2 + 2] == 0x00
				&& data_b[i + 2 + 1920*2 + 3] == 0x00

				//&& data_b[i + 2 + 1920*2 + 4] == 0x9d
				&& data_b[i + 2 + 1920*2 + 5] == 0x9d

#endif // 0)
				) {			
								if ((i + 8 + 1920*2+1 < image_data.size())
									&&write_flag) {
										out_yuv_row.write((char*) &data_b[i + 8], 720*2);			
								}			
			}

		}

	}
#endif
#if 0
	
				out_yuv_row.write((char *)data_b, image_data.size());
	
#endif
	out_yuv_row.flush();
	out_yuv_row.close();

	findff0000xy(image_data);
	statistics255(image_data);
}


void cal_yuv_raw_data(std::string file_name_t)
{

	ifstream fileTEXT(file_name_t);



	string line_new;

	std::vector<unsigned char> image_data;
	//image_data.resize(1920*1080*2*10);

	image_data.reserve(1920 * 1080 * 2 * 10);

	int i = 0;
	unsigned char data[2];

	do {

		std::istream& flag = std::getline(fileTEXT, line_new);
		parse_line2byte(line_new, data);

		image_data.push_back(data[1]);//Cr Cb
		image_data.push_back(data[0]);//Y



		if (i++ % 50000 == 0) {
			std::cout << "read line :" << i << std::endl;
		}

		if (flag.eof()) {
			break;
		}

	} while (1);


	unsigned char *data_b = image_data.data();


	std::ofstream out_yuv_row("E:\\bt1120-yuv\\uyvy.20m3.process2.raw");
#if TRUE
	
	 i = 0;
	do {
	
		//find header
		if (data_b[i] == 0xff
			&&data_b[i + 1] == 0xff

			&& data_b[i + 2] == 0x00
			&& data_b[i + 3] == 0x00

			&& data_b[i + 4] == 0x80
			&& data_b[i + 5] == 0x80

			&& data_b[i + 6] == 0x00
			&& data_b[i + 7] == 0x00) {



		
		}

	} while (i < image_data.size());

#endif //

#if 0
	for (int i = 0; i < image_data.size(); i++) {
		if (
			//data_b[i] == 0xff
			data_b[i + 1] == 0xff

			//&& data_b[i + 2] == 0x00
			&& data_b[i + 3] == 0x00

			//&& data_b[i + 4] == 0x80
			&& data_b[i + 5] == 0x80

			/*	&& data_b[i + 6] == 0x00
			&& data_b[i + 7] == 0x00*/

			) {

			if (1
#if 1
				//&& data_b[i + 2 + 1920*2] == 0xff
				&& data_b[i + 2 + 1920 * 2 + 1] == 0xff

				//&& data_b[i + 2 + 1920*2 + 2] == 0x00
				&& data_b[i + 2 + 1920 * 2 + 3] == 0x00

				//&& data_b[i + 2 + 1920*2 + 4] == 0x9d
				&& data_b[i + 2 + 1920 * 2 + 5] == 0x9d

#endif // 0)
				) {
				if (i + 8 + 1920 * 2 + 1 < image_data.size()) {
					out_yuv_row.write((char*)&data_b[i + 8], 1920 * 2);
				}
			}

		}

	}
#endif
#if 0

	out_yuv_row.write((char *)data_b, image_data.size());

#endif
	out_yuv_row.flush();
	out_yuv_row.close();


	statistics255(image_data);

}


int main( int argc, char** argv )
{
#if 0
	inv_test();//矩阵求逆
#endif // 0


#if 0
	capture_frame(); //从文件截图
#endif

#if 0
	//yuv-bitt1120
	//readyuvrowData("E:\\bt1120-yuv\\20Mxx.TXT");
	readyuvrowData("E:\\bt1120-yuv\\2017年9月21日.TXT", "E:\\bt1120-yuv\\uyvy.2017年9月21日.process.raw");
	//cal_yuv_raw_data("E:\\bt1120-yuv\\20M0914.TXT");
#endif // 1
	


    return 0;
}
