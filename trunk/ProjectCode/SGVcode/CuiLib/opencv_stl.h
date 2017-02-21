#ifndef OpenCV_STL
#define OpenCV_STL          

//#include <vld.h>
/*********************************************************/
//OPENCV
#include <cv.h>
#include <highgui.h>

#if (CV_MAJOR_VERSION==2)&&(CV_MINOR_VERSION==4)
#include <opencv2/ml/ml.hpp>
#endif
#if (CV_MAJOR_VERSION==3)
#include <opencv2/ml.hpp> 
#endif

#include <opencv2/opencv.hpp>
#if _MSC_VER
#include <opencv2/gpu/gpu.hpp>
#endif
using namespace cv;
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
//C++
#include<stdio.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
//#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cfloat>
#include <cmath>
#include <ios>
#include <fstream>
#include <stdexcept>
#include <cstdio> 
#include<stdio.h>
#include<string.h>
#if _MSC_VER
#include <mbctype.h>  
#include <io.h>
#endif
//#if _MSC_VER
//#include<atlstr.h>
//#endif
#if linux||__linux__||__linux||__GNUC__
#include<memory>
#include <tr1/memory>
#include <dirent.h>
#include <unistd.h>
#endif


#ifndef TimeCountClock_START

#endif 
#ifndef TimeCountClock_END

#endif

#ifndef TimeCountClockTest
//#define TimeCountClockTest(P,M)  {double dur;clock_t start,end;start = clock();	{P} end = clock();dur = (double)(end - start);printf("%s(S):%0.3f\n",M,(dur/CLOCKS_PER_SEC));}

#define TimeCountStart()	double dur=0;clock_t start,end;start = clock();

#define TimeCountStop(MESSAGE_TIME_END)		end = clock();dur = (double)(end - start);printf("%s(S):%0.3f\n",MESSAGE_TIME_END,(dur/CLOCKS_PER_SEC));
#endif



using namespace std;
/*********************************************************/
#endif









