#cmake最低版本需求，不加入此行会受到警告信息
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
###############################################################################
#MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM}")
C11_Config()  
ConfigOpenCv()
###############################################################################

SET(BASE_LINK_LIB 
	         tinyxml2
	)

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")  
 
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
	
	ConfigMFC_2() 

ELSEIF (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
   
ELSE ()  
   
ENDIF ()  
###############################################################################
SET(LIBCURL_SRCS

	)

ADD_LIBRARY(libcurl   STATIC    ${LIBCURL_SRCS})

TARGET_LINK_LIBRARIES(libcurl ${BASE_LINK_LIB})
###############################################################################