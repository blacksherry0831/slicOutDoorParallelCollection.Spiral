#cmake最低版本需求，不加入此行会受到警告信息
CMAKE_MINIMUM_REQUIRED(VERSION 3.8.1)
###############################################################################
#MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM}")
C11_Config()  
ConfigOpenCv()
CONFIGQT()
###############################################################################

###############################################################################


IF (CMAKE_SYSTEM_NAME MATCHES "Linux")  
 
 SET(HW_SDK_SRCS
		hw_dev.cpp)
		
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
	
	ConfigMFC_2() 
    CONFIGPTHREADS()

	SET(HW_SDK_SRCS
		hw_nvr.cpp
		live_video_base.cpp
		live_set.cpp
		ipc.cpp
		dc_save.cpp
		hw_dev.cpp)
		
	SET(BASE_LINK_LIB 
		 tinyxml2
		 SerialPortModule
		 MY_SDK_LIB
		 pthreads)

	if(NEED_CUDA)

		SET(HW_SDK_SRCS
				${HW_SDK_SRCS}
				live_video.cpp)
			
		SET(BASE_LINK_LIB 
				${BASE_LINK_LIB}
				SGVcode)	
	else()
			 
	endif()

ELSEIF (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
   
ELSE ()  
   
ENDIF ()  
###############################################################################


	
ADD_LIBRARY(HW_SDK_LIB   STATIC    ${HW_SDK_SRCS})

TARGET_LINK_LIBRARIES(HW_SDK_LIB ${BASE_LINK_LIB})
###############################################################################