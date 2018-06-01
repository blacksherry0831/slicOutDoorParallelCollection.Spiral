#include "cpp_stl.h"
#include "opencv_stl.h"
/*---------------------------------------*/

/*---------------------------------------*/
#include "MY_SDK_LIB/Base.h"
/*---------------------------------------*/
/**
*
*
*/
/*---------------------------------------*/

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
	




	return 0;


}