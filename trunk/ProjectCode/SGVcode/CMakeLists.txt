#cmake最低版本需求，不加入此行会受到警告信息
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
#项目名称
PROJECT(SGV_cmake)
###############################################################################
INCLUDE(cmake/LinuxPrepare.cmake)
INCLUDE(cmake/SetMFC.cmake)
INCLUDE(cmake/FindOpencv.cmake)
INCLUDE(cmake/SetBoost.cmake)
INCLUDE(cmake/setCUDA.cmake)
###############################################################################
INCLUDE_DIRECTORIES( #相关头文件的目录
	${SGV_cmake_SOURCE_DIR}/CuiLib
	${SGV_cmake_SOURCE_DIR}
)
###############################################################################

###############################################################################

INCLUDE_DIRECTORIES( #相关头文件的目录
	${SGV_cmake_SOURCE_DIR}/CuiLib/websocketpp-experimental	
)


ADD_SUBDIRECTORY(CuiLib)
ADD_SUBDIRECTORY(main_test)

#ADD_SUBDIRECTORY(arpackpp-master)
###############################################################################
MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM}")  
############配置OPENCV#########################################################
C11_Config()
ConfigOpenCv()
############配置CUDA#########################################################
ConfigCUDA()
#############################################################################
#ConfigBoost()
#################编译器配置####################################################
MESSAGE(STATUS "###################################")  

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")  
 
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")   
	 ConfigMFC_2()
	 MESSAGE(STATUS "MFC config")   
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
############################################################################### 

#FIND_PATH(WEB_SOCKET_PP_ROOT client.hpp ${SGV_cmake_SOURCE_DIR}/CuiLib/websocketpp-experimental/websocketpp)

#message(${WEB_SOCKET_PP_ROOT})
############################################################################### 	
if(CMAKE_CL_64)  
else()
	SET(HW_WIN32_LIB	
		play_sdk.lib
		hwclient.lib
		ijl15l.lib)

	LINK_DIRECTORIES(
		${SGV_cmake_SOURCE_DIR}/CuiLib/HW_SDK_LIB/lib)
	link_libraries(${HW_WIN32_LIB})
endif()

##########################################################
#CMAKE的内建变量，如果是true，就说明编译器的64位的，自然可以编译64bit的程序
if(CMAKE_CL_64)    
	
	SET(LIB_PTHREADS
		pthreadVC2.lib)

   LINK_DIRECTORIES(		
		${SGV_cmake_SOURCE_DIR}/CuiLib/pthreads-w32-2-9-1-release/lib/x64)
else()
	SET(LIB_PTHREADS
		pthreadVC2.lib
		pthreadVCE2.lib
		pthreadVSE2.lib)

   LINK_DIRECTORIES(		
		${SGV_cmake_SOURCE_DIR}/CuiLib/pthreads-w32-2-9-1-release/lib/x86)
endif()

   link_libraries(${LIB_PTHREADS})

############################################################ 
if(CMAKE_CL_64)  
else()
	SET(LIBCURL
		libcurl.lib)

   LINK_DIRECTORIES(		
		${SGV_cmake_SOURCE_DIR}/CuiLib/libcurl/lib)

   link_libraries(${LIBCURL})
endif()

############################################################

   
ADD_EXECUTABLE(EXE_SpectralClusteringGndVSkyOutDoor main.cpp)
TARGET_LINK_LIBRARIES(EXE_SpectralClusteringGndVSkyOutDoor 
									SGVcode
						  )						  

############################################################
################################################
ADD_EXECUTABLE(TEST_GPS main_test/gps_test.cpp)
TARGET_LINK_LIBRARIES(TEST_GPS
	SerialPort)
################################################
ADD_EXECUTABLE(EXE_HW_Hello main_test/hw_hello.cpp)
TARGET_LINK_LIBRARIES(EXE_HW_Hello
	HW_SDK_LIB)  
################################################
################################################
ADD_EXECUTABLE(EXE_DUANZAO main_test/duanzao.cpp)
TARGET_LINK_LIBRARIES(EXE_DUANZAO
						MY_SDK_LIB)
################################################
ADD_EXECUTABLE(TEST_calibration main_test/calibration.cpp)
TARGET_LINK_LIBRARIES(TEST_calibration)
################################################
ADD_EXECUTABLE(TEST_JSON main_test/json_test.cpp)
TARGET_LINK_LIBRARIES(TEST_JSON
								jsoncpp_lib)
################################################
ADD_EXECUTABLE(TEST_HTTP main_test/http_test.cpp)
TARGET_LINK_LIBRARIES(TEST_HTTP)
################################################
################################################
ADD_EXECUTABLE(EXE_HW_AVI2IMAGE main_test/hw_avi2image.cpp)
TARGET_LINK_LIBRARIES(EXE_HW_AVI2IMAGE
	MY_SDK_LIB
	HW_SDK_LIB) 
#################################################
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
   
ELSE ()  
   
ENDIF ()  

##################################################################################