#include "StdAfx.h"
#include "VideoRecord.h"
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
VideoRecord::VideoRecord(void)
{
	mVideoWriter = nullptr;
	this->setparam(1920, 1080, 5);
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
VideoRecord::~VideoRecord(void)
{
	this->release();
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
int VideoRecord::create(void)
{
	mVideoWriter = cvCreateVideoWriter(mVideoFullPath.c_str(),
		//CV_FOURCC_PROMPT,
		CV_FOURCC('D', 'I', 'V', 'X'),
		mFps,
		cvSize(mWidth, mHeight));

	if (mVideoWriter == nullptr) {
		std::cout << "create viveo writer [fail] :" << std::endl;
		return FALSE;
	}else {
		std::cout << "create viveo writer [success] :"  << std::endl;
		return TRUE;
	}
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
int VideoRecord::writeFrame(IplImage* _img)
{
	assert(_img == nullptr);
	assert(_img->width!=mWidth);
	assert(_img->height != mHeight);

	assert(0);

	cvWriteFrame(mVideoWriter, _img);

	return 0;
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
int VideoRecord::release()
{
	cvReleaseVideoWriter(&mVideoWriter);
	mVideoWriter = nullptr;
	return TRUE;
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
int VideoRecord::setparam(int _width, int _height, int _fps)
{
	this->mWidth = _width;
	this->mHeight = _height;
	this->mFps = _fps;
	return 0;
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
int VideoRecord::setVideoFullPath(std::string _file)
{
	this->mVideoFullPath = _file;
	return 0;
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
void VideoRecord::SetFileName(int _channel)
{

}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/