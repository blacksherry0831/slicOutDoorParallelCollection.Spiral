#cmake最低版本需求，不加入此行会受到警告信息
CMAKE_MINIMUM_REQUIRED(VERSION 3.8.1)
#项目名称
###############################################################################
if(NEED_BOOST)
	FIND_PACKAGE(Boost)
	IF(Boost_FOUND)

	ELSE()
		MESSAGE(FATAL_ERROR ”Boost library not found”)
	ENDIF(Boost_FOUND)
endif(NEED_BOOST)
###############################################################################

CONFIGPTHREADS()

if(NEED_OPENCV)
	ConfigOpenCv()	
endif(NEED_OPENCV)

if(NEED_CUDA)
	ConfigCUDA()	
endif(NEED_CUDA)

if(NEED_HTTP)
	 CONFIGCURLHTTP()
endif(NEED_HTTP)
###############################################################################
  

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")  
 
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
 ConfigMFC_2()
 ###############################################
IF(Boost_FOUND)

	ConfigBoost()
	
	if(NEED_WEBSOCKETPP)
		ADD_EXECUTABLE(TEST_WEB_SOCKET_PP websocket_test.cpp)
		TARGET_LINK_LIBRARIES(TEST_WEB_SOCKET_PP
					)
	endif(NEED_WEBSOCKETPP)
	####################################

	if(NEED_WEBSOCKETPP AND NEED_HTTP AND NEED_BOOST)
		ADD_EXECUTABLE(TEST_AGV_CAR agv_car_test.cpp)
		TARGET_LINK_LIBRARIES(TEST_AGV_CAR	
			libcurl_http
			SerialPort
			Comm2ServerLib) 		
	endif()
	####################################
	
	if(NEED_OPENCV)
	
	endif(NEED_OPENCV)
	####################################
ENDIF(Boost_FOUND)
################################################


ELSEIF (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
   
ELSE ()  
   
ENDIF ()  
