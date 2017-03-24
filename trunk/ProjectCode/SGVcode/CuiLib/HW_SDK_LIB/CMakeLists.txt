#cmake最低版本需求，不加入此行会受到警告信息
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
###############################################################################
#MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM}")
C11_Config()  
ConfigOpenCv()
###############################################################################

SET(BASE_LINK_LIB 
	         tinyxml2
			 SGVcode
			 SerialPort
	)

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")  
 
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
	
	ConfigMFC_2() 
   
	#SET(HW_WIN32	
	#play_sdk.lib
	#hwclient.lib
	#ijl15l.lib)

	#LINK_DIRECTORIES(
	#		lib
	#)

   #link_libraries(${HW_WIN32})

ELSEIF (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
   
ELSE ()  
   
ENDIF ()  
###############################################################################
SET(HW_SDK_SRCS
	live_video.cpp
	live_set.cpp
	ipc.cpp
	dc_save.cpp)

ADD_LIBRARY(HW_SDK_LIB   STATIC    ${HW_SDK_SRCS})

TARGET_LINK_LIBRARIES(HW_SDK_LIB ${BASE_LINK_LIB})
###############################################################################