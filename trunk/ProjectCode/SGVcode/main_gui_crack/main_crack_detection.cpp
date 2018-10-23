#include "stdafx.h"
#include <MY_SDK_LIB/Base.h>
#include <MY_SDK_LIB/ImageProcess.h>
#include <MY_SDK_LIB/OutCircleFeature.hpp>
#include <MY_SDK_LIB/CrackDetection.hpp>
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void main_crack_detection() 
{


	
	

	CrackDetection cd;
	std::string file_full_name = "X:\\ImageDataBase\\Crack\\Test\\SVM_tst.jpg";

	cd.LoadImg(file_full_name);

	cd.Cvt2Binary3Sigma();


	cd.Binary2Link();
	
	cd.Binary2Smooth();

	 
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
int main(int argc, char *argv[])
{
	int EXEC_RESULT = 0;
	int test_num = 0;
#if  0
	std::cin >> test_num;
#endif
	main_crack_detection();

//	std::cin.get();		
	return  EXEC_RESULT;
}

/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/