#pragma once

#include "opencv_stl.h"

class VideoRecord
{
public:
	VideoRecord(void);
	~VideoRecord(void);
private:
	std::string mVideoFullPath;
	std::string mVideoPath;
	std::string mVideoFileFullName;
	CvVideoWriter *mVideoWriter;
	int mWidth;
	int mHeight;
	int mFps;
public:
	int create();
	int writeFrame(IplImage* _img);
	int release();
	int setparam(int _width,int _height,int _fps);
	int setVideoFullPath(std::string _file);
public:
	void SetFileName(int _channel);
};

