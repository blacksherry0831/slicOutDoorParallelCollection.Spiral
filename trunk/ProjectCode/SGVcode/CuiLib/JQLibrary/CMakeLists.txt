#cmake最低版本需求，不加入此行会受到警告信息
CMAKE_MINIMUM_REQUIRED(VERSION 3.8.1)
#项目名称
###############################################################################
C11_Config()
CONFIGQT()
###############################################################################
SET(DIR_SRCS    ./src/JQCPUMonitor/jqcpumonitor.cpp
				
				)  
###############################################################################
SET(LINK_LIB 

	)

#include_directories(“./include/JQCPUMonitor/”) 

###############################################################################
IF (CMAKE_SYSTEM_NAME MATCHES "Linux")  
 
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
 ConfigMFC_2()
 ###############################################	
	
	
################################################
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
   
ELSE ()  
   
ENDIF ()  


#ADD_LIBRARY(JQLibrary  STATIC     ${DIR_SRCS}  )
