#cmake最低版本需求，不加入此行会受到警告信息
CMAKE_MINIMUM_REQUIRED(VERSION 3.8.1)
###############################################################################
#MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM}")
C11_Config()  
ConfigOpenCv()
###############################################################################

###############################################################################

SET(BASE_LINK_LIB 
	         
	)

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")  
 
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
	
ConfigMFC_2() 
CONFIGCURLHTTP()
###############################################################################
SET(LIBCURL_SRCS
		CHttpClient.cpp
	)

ADD_LIBRARY(libcurl_http   STATIC    ${LIBCURL_SRCS})

TARGET_LINK_LIBRARIES(libcurl_http ${BASE_LINK_LIB})

SET_TARGET_PROPERTIES(libcurl_http PROPERTIES LINKER_LANGUAGE CXX)
###############################################################################
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
   
ELSE ()  
   
ENDIF ()  
