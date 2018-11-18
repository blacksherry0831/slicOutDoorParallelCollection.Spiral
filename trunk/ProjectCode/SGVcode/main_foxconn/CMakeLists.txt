#cmake最低版本需求，不加入此行会受到警告信息
CMAKE_MINIMUM_REQUIRED(VERSION 3.8.1)
#项目名称
###############################################################################
C11_Config()
CONFIGQT()
CONFIGPTHREADS()

if(NEED_OPENCV)
	ConfigOpenCv()	
endif(NEED_OPENCV)

option (CBB_BUILDING_FOXCONN
        "视觉导航 Automatic Guiding Vehicle" OFF) 

###############################################################################
IF (CMAKE_SYSTEM_NAME MATCHES "Linux")  
 
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
 ConfigMFC_2()
 ###############################################	
	if(NEED_OPENCV)		

		if(CBB_BUILDING_FOXCONN)
				
				ADD_EXECUTABLE(TEST_FOXCONN main_foxconn.cpp)
				TARGET_LINK_LIBRARIES(TEST_FOXCONN
										MY_SDK_LIB
										QT_THREAD_MODULE
										Harr
										SGVBaseCode)
													
		endif(CBB_BUILDING_FOXCONN)		

	endif(NEED_OPENCV)
################################################
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
   
ELSE ()  
   
ENDIF ()  
