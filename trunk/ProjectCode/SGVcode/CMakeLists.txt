#cmake最低版本需求，不加入此行会受到警告信息
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
#项目名称
PROJECT(SGV_cmake)
###############################################################################
INCLUDE(cmake/LinuxPrepare.cmake)

INCLUDE(cmake/SetMFC.cmake)
INCLUDE(cmake/FindOpencv.cmake)
INCLUDE(cmake/setCUDA.cmake)
###############################################################################
INCLUDE_DIRECTORIES( #相关头文件的目录
	${SGV_cmake_SOURCE_DIR}/CuiLib
	${SGV_cmake_SOURCE_DIR}
)
ADD_SUBDIRECTORY(CuiLib)
#ADD_SUBDIRECTORY(arpackpp-master)
###############################################################################
MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM}")  
############配置OPENCV#########################################################
C11_Config()
ConfigOpenCv()
############配置CUDA#########################################################
ConfigCUDA()
#################编译器配置####################################################
MESSAGE(STATUS "###################################")  

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")  
 
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")   
	 ConfigMFC_2()
	 MESSAGE(STATUS "MFC配置")   
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
   
ELSE ()  
   
ENDIF ()  
MESSAGE(STSTUS "###################################")
###############################################################################
#AUX_SOURCE_DIRECTORY(. SRC_LIST) #把当前目录(.)下所有源代码文件和头文件加入变量SRC_LIST
#生成应用程序SpectralClusteringGndVSkyOutDoor (在windows下会自动生成hello.exe)

#find_package(Threads REQUIRED)
#if(THREADS_HAVE_PTHREAD_ARG)
#  target_compile_options(PUBLIC my_app "-pthread")
#endif()
#if(CMAKE_THREAD_LIBS_INIT)
#  target_link_libraries(my_app "${CMAKE_THREAD_LIBS_INIT}")
#endif()
##################################################################################

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")  
	
	ADD_EXECUTABLE(SpectralClusteringGndVSkyOutDoor main.cpp)
	
	find_package(Threads REQUIRED)
	if(THREADS_HAVE_PTHREAD_ARG)
	  target_compile_options(PUBLIC SpectralClusteringGndVSkyOutDoor "-pthread")
	endif()
	
	if(CMAKE_THREAD_LIBS_INIT)
	 
		TARGET_LINK_LIBRARIES(SpectralClusteringGndVSkyOutDoor 
									SGVcode
									"${CMAKE_THREAD_LIBS_INIT}"
						  ) 
		
	ELSE()
	
	TARGET_LINK_LIBRARIES(SpectralClusteringGndVSkyOutDoor 
									SGVcode
						  ) 
	
	endif()
	

	
	
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")   
	ADD_EXECUTABLE(SpectralClusteringGndVSkyOutDoor main.cpp)
	TARGET_LINK_LIBRARIES(SpectralClusteringGndVSkyOutDoor 
									SGVcode
						  )  	
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
   
ELSE ()  
   
ENDIF ()  

##################################################################################