#ifndef OpenCV_STL
#define OpenCV_STL          

//#include <vld.h>
/*********************************************************/
//OPENCV
#include <cv.h>
#include <highgui.h>

#if (CV_MAJOR_VERSION==2)&&(CV_MINOR_VERSION==4)
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/ml/ml.hpp>
#endif
#if (CV_MAJOR_VERSION==3)
#include <opencv2/ml.hpp> 
#endif

#include <opencv2/opencv.hpp>
#if _MSC_VER
#include <opencv2/gpu/gpu.hpp>
#endif

/*********************************************************/
#ifndef cvCopyImage
#define cvCopyImage( src, dst ) cvCopy( src, dst, 0 )
#endif 

#ifndef cvCvtPixToPlane
#define cvCvtPixToPlane cvSplit
#endif

#ifndef cvmMul
#define cvmMul( src1, src2, dst ) cvMatMulAdd( src1, src2, 0, dst )
#endif

#ifndef cvGetHistValue_1D
#define cvGetHistValue_1D( hist, idx0 ) ((float*)cvPtr1D( (hist)->bins, (idx0), 0))
#endif

#ifndef cvQueryHistValue_1D
#define cvQueryHistValue_1D( hist, idx0 ) ((float)cvGetReal1D( (hist)->bins, (idx0)))
#endif


/*********************************************************/

#include "cpp_stl.h"

#endif









