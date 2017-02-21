SET(WindowsLib
		opencv_calib3d249d.lib
		opencv_contrib249d.lib
		opencv_core249d.lib
		opencv_features2d249d.lib
		opencv_flann249d.lib
		opencv_gpu249d.lib
		opencv_highgui249d.lib
		opencv_imgproc249d.lib
		opencv_legacy249d.lib
		opencv_ml249d.lib
		opencv_objdetect249d.lib
		opencv_ts249d.lib
		opencv_video249d.lib
		opencv_ocl249d.lib
	 )

function (ConfigMFC )  


IF (CMAKE_SYSTEM_NAME MATCHES "Windows")  
   SET(CMAKE_MFC_FLAG 2)	
ENDIF()  
  
endfunction () 


macro(ConfigMFC_2)
IF (CMAKE_SYSTEM_NAME MATCHES "Windows")  
	if(MSVC_VERSION)
		 SET(CMAKE_MFC_FLAG 2)	
		 link_libraries(${WindowsLib})
    endif(MSVC_VERSION)
    
ENDIF() 
endmacro()