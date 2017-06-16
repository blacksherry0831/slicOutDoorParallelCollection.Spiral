#cmake最低版本需求，不加入此行会受到警告信息
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
#项目名称
###############################################################################
FIND_PACKAGE(Boost)
###############################################################################
ConfigOpenCv()
ConfigCUDA()
###############################################################################
  

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")  
 
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
 ConfigMFC_2()	
 CONFIGPTHREADS()
 CONFIGCURLHTTP()
###############################################
IF(Boost_FOUND)
	ConfigBoost()
	####################################
	ADD_EXECUTABLE(TEST_WEB_SOCKET_PP websocket_test.cpp)
	TARGET_LINK_LIBRARIES(TEST_WEB_SOCKET_PP
					)
	####################################

	ADD_EXECUTABLE(TEST_AGV_CAR agv_car_test.cpp)
	TARGET_LINK_LIBRARIES(TEST_AGV_CAR	
		libcurl_http
		SerialPort
		Comm2ServerLib) 
	####################################

ELSE(Boost_FOUND)

   MESSAGE(FATAL_ERROR ”Boost library not found”)

ENDIF(Boost_FOUND)




################################################

################################################

################################################


ELSEIF (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
   
ELSE ()  
   
ENDIF ()  
