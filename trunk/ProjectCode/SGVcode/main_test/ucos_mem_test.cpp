#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <cctype>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <iostream>



#include "ucos/os.h"

unsigned char    CommTxPart[32][1920*1080];

int main( int argc, char** argv )
{
	
	
	OS_MEM  os_mem_t;
	OS_ERR  os_err_t;
	OSMemCreate(&os_mem_t,"memory",CommTxPart,100,32,&os_err_t);

	if (os_err_t != OS_ERR_NONE)
		std::cout << "create error" <<std::endl;

	do {
		unsigned char *data=(unsigned char *) OSMemGet(&os_mem_t, &os_err_t);

		if (os_err_t != OS_ERR_NONE)
			std::cout << "create error" << std::endl;

#if defined(_WIN32) || defined(_WIN64) || defined( _MSC_VER)
		_sleep(1);
#endif

	
	} while (true);
	

	
    return 0;
}
