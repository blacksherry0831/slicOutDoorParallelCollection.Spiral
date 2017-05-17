#cmake最低版本需求，不加入此行会受到警告信息
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
###############################################################################
#MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM}")
C11_Config()  
ConfigOpenCv()
###############################################################################

SET(BASE_LINK_LIB 
	         
	)

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")  
 
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
	
ConfigMFC_2() 
###############################################################################
SET(pthreads_SRCS

	)

ADD_LIBRARY(pthreads   STATIC    ${pthreads_SRCS})

SET_TARGET_PROPERTIES(pthreads PROPERTIES LINKER_LANGUAGE CXX)
###############################################################################

ELSEIF (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
   
ELSE ()  
   
ENDIF ()  
