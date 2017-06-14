#cmake最低版本需求，不加入此行会受到警告信息
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
#项目名称
PROJECT(SGV_cmake)
###############################################################################
function (CONFIGPTHREADS)

#CMAKE的内建变量，如果是true，就说明编译器的64位的，自然可以编译64bit的程序

INCLUDE_DIRECTORIES( #相关头文件的目录
		${SGV_cmake_SOURCE_DIR}/CuiLib/pthreads-w32-2-9-1-release/include
	)

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
  
endfunction (CONFIGPTHREADS)
#############################################################################
macro(CONFIGCURLHTTP)

if(CMAKE_CL_64)  

else()

	SET(LIBCURL
	libcurl.lib
	)

  
	
   LINK_DIRECTORIES(		
		${SGV_cmake_SOURCE_DIR}/CuiLib/libcurl/lib)

   link_libraries(${LIBCURL})

endif()
  
endmacro(CONFIGCURLHTTP)
#############################################################################
#############################################################################
function (CONFIG_HOWELL_CAMERA)

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

  
endfunction (CONFIG_HOWELL_CAMERA)
###############################################################################
macro(CONFIG_ALL_MODULE)
	MESSAGE(STSTUS "###################################") 
			IF (CMAKE_SYSTEM_NAME MATCHES "Linux")  
 
			ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows") 
						#############################
						ConfigMFC_2()
						#############################	
						CONFIG_HOWELL_CAMERA()
						#############################
						CONFIGPTHREADS()
						############################# 
						CONFIGCURLHTTP()
						#############################
						IF(Boost_FOUND)
							#ConfigBoost()

						ELSE(Boost_FOUND)

						   MESSAGE(FATAL_ERROR ”Boost library not found”)

						ENDIF(Boost_FOUND)
						#############################
			ELSEIF (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
   
			ELSE ()  
   
			ENDIF ()  
	MESSAGE(STSTUS "###################################")  
endmacro(CONFIG_ALL_MODULE)
###############################################################################
function (CONFIG_LOCAL_MODULE)
	
	#############################################
	INCLUDE_DIRECTORIES( #相关头文件的目录
					${SGV_cmake_SOURCE_DIR}/CuiLib
					${SGV_cmake_SOURCE_DIR}
					) 
	#############################################
	INCLUDE_DIRECTORIES( #相关头文件的目录
					${SGV_cmake_SOURCE_DIR}/CuiLib/websocketpp-experimental
					)
	#############################################

endfunction (CONFIG_LOCAL_MODULE)
##################################################################################
##################################################################################
##################################################################################
##################################################################################
INCLUDE(cmake/LinuxPrepare.cmake)
INCLUDE(cmake/SetMFC.cmake)
INCLUDE(cmake/FindOpencv.cmake)
INCLUDE(cmake/SetBoost.cmake)
INCLUDE(cmake/setCUDA.cmake)
###############################################################################
#放在最前面
CONFIG_LOCAL_MODULE()
###############################################################################
ADD_SUBDIRECTORY(CuiLib)
ADD_SUBDIRECTORY(main_test)
###############################################################################
MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM}")  
############配置OPENCV#########################################################
C11_Config()
ConfigOpenCv()
############配置CUDA#########################################################
ConfigCUDA()
#############################################################################
FIND_PACKAGE(Boost)
#################编译器配置####################################################
CONFIG_ALL_MODULE()
##################################################################################
##################################################################################
##################################################################################
##################################################################################
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

############################################################   
ADD_EXECUTABLE(EXE_SpectralClusteringGndVSkyOutDoor main.cpp)
TARGET_LINK_LIBRARIES(EXE_SpectralClusteringGndVSkyOutDoor 
									SGVcode
						  )
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
################################################
#ADD_EXECUTABLE(TEST_AGV_CAR main_test/agv_car_test.cpp)
#TARGET_LINK_LIBRARIES(TEST_AGV_CAR
#	Comm2ServerLib
#	SerialPort) 
#################################################
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
   
ELSE ()  
   
ENDIF ()  

##################################################################################