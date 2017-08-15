#cmake最低版本需求，不加入此行会受到警告信息
CMAKE_MINIMUM_REQUIRED(VERSION 3.8.1)
#项目名称
PROJECT(SGV_cmake)
###############################################################################
option (CBB_BUILDING_AGV_CAR
        "视觉导航 Automatic Guiding Vehicle" ON) 

option (CBB_BUILDING_HOT_FORGING
        "热锻工件定位" ON)

option (CBB_BUILDING_CRACK_DETECTION
        "工业视觉探伤" ON)
#########################################
if (CBB_BUILDING_AGV_CAR)

	SET(NEED_HTTP	ON)
	SET(NEED_JSON	ON)
	SET(NEED_WEBSOCKETPP	ON)
	SET(NEED_OPENCV		ON)
	SET(NEED_CUDA		ON)

endif (CBB_BUILDING_AGV_CAR)
##############################
if (CBB_BUILDING_HOT_FORGING)

	
	SET(NEED_OPENCV		ON)
	

endif (CBB_BUILDING_HOT_FORGING)
##############################
if (CBB_BUILDING_CRACK_DETECTION)


	SET(NEED_OPENCV		ON)
	SET(NEED_QT		ON)

endif (CBB_BUILDING_CRACK_DETECTION)
###############################################################################
if(NEED_WEBSOCKETPP)
SET(NEED_BOOST  ON)
endif(NEED_WEBSOCKETPP)
###############################################################################
function(CONFIGPTHREADS)

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
  
endfunction(CONFIGPTHREADS)
#############################################################################
macro(CONFIGCURLHTTP)

	if(NEED_HTTP)

		if(CMAKE_CL_64)  

		else()

			SET(LIBCURL
			libcurl.lib
			)

		  
			
		   LINK_DIRECTORIES(		
				${SGV_cmake_SOURCE_DIR}/CuiLib/libcurl/lib)

		   link_libraries(${LIBCURL})

		endif()

	endif(NEED_HTTP)
  
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
ADD_SUBDIRECTORY(main_gui_crack)
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

	
	################################################
	if(NEED_CUDA AND NEED_OPENCV)

	ADD_EXECUTABLE(EXE_SpectralClusteringGndVSkyOutDoor main.cpp)
	TARGET_LINK_LIBRARIES(EXE_SpectralClusteringGndVSkyOutDoor 
										SGVcode
							  )
	endif()	
	################################################
	if(CBB_BUILDING_AGV_CAR)

		ADD_EXECUTABLE(TEST_GPS main_test/gps_test.cpp)
		TARGET_LINK_LIBRARIES(TEST_GPS
			SerialPort)

		ADD_EXECUTABLE(EXE_HW_Hello main_test/hw_hello.cpp)
		TARGET_LINK_LIBRARIES(EXE_HW_Hello
			HW_SDK_LIB)

	endif(CBB_BUILDING_AGV_CAR)
	################################################
	ADD_EXECUTABLE(EXE_HW_BASE main_test/hw_base.cpp)
	TARGET_LINK_LIBRARIES(EXE_HW_BASE
		HW_SDK_LIB)
	################################################
	if (CBB_BUILDING_HOT_FORGING)

		ADD_EXECUTABLE(EXE_DUANZAO main_test/duanzao.cpp)
		TARGET_LINK_LIBRARIES(EXE_DUANZAO
								MY_SDK_LIB)
	
	endif(CBB_BUILDING_HOT_FORGING)	
	################################################
	if(NEED_OPENCV)
	
		ADD_EXECUTABLE(TEST_calibration main_test/calibration.cpp)
		TARGET_LINK_LIBRARIES(TEST_calibration)
	
		ADD_EXECUTABLE(TEST_opencv main_test/opencv_test.cpp)
		TARGET_LINK_LIBRARIES(TEST_opencv)
	
	endif(NEED_OPENCV)	
	################################################
	if (NEED_JSON)

		ADD_EXECUTABLE(TEST_JSON main_test/json_test.cpp)
		TARGET_LINK_LIBRARIES(TEST_JSON
										jsoncpp_lib)
 
	 endif(NEED_JSON)
	################################################
	if (NEED_HTTP)

		ADD_EXECUTABLE(TEST_HTTP main_test/http_test.cpp)
		TARGET_LINK_LIBRARIES(TEST_HTTP)	
 
	 endif (NEED_HTTP)
	################################################
	if(NEED_OPENCV)

		ADD_EXECUTABLE(EXE_HW_AVI2IMAGE main_test/hw_avi2image.cpp)
		TARGET_LINK_LIBRARIES(EXE_HW_AVI2IMAGE
		MY_SDK_LIB
		HW_SDK_LIB) 
		
	endif() 
	#################################################
	if(CBB_BUILDING_CRACK_DETECTION)

		ADD_EXECUTABLE(TEST_BE_1105 main_test/be1105_test.cpp)
		TARGET_LINK_LIBRARIES(TEST_BE_1105
			SerialPort) 
		
	endif() 
	#################################################
	if(NEED_OPENCV)
		ADD_EXECUTABLE(EXE_HOWELL_CALIBRATION main_test/howell_calibration.cpp)
			TARGET_LINK_LIBRARIES(EXE_HOWELL_CALIBRATION
								OpencvCalibration
								HW_SDK_LIB) 	
	endif(NEED_OPENCV)
	#################################################
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
   
ELSE ()  
   
ENDIF ()  

##################################################################################