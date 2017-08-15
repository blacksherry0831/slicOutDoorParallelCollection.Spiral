#include "stdafx.h"
#include "cpp_stl.h"
#include "opencv_stl.h"
#include "opencv_calibration/public.h"

#include "pt_mutex.h"

int main( int argc, char** argv )
{


#if TRUE	
	
	global_thread_synch = true;
	GlobalStatic::CAL_ROUTINE = TRUE;

	int ret = 0;
	pthread_t pt_handle;
	ret = pthread_create(&pt_handle,NULL, &thread_opencv_calibration,NULL);
	if (ret != 0)
	{
		std::cout << "Create pthread error!" << std::endl;
		ASSERT(ret != 0);
	}

	ret = pthread_join(pt_handle, NULL);

#endif
	std::cout << "thread done !";
	char str[1024];
	std::cin.getline(str, 5);


}
