############≈‰÷√OPENCV#########################################################
C11_Config()
###############################################################################
FIND_PACKAGE(Boost)
###############################################################################
ConfigOpenCv()
###############################################################################
IF (CMAKE_SYSTEM_NAME MATCHES "Linux")  
 
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
####################################### 
 ConfigMFC_2()
 CONFIGCURLHTTP()
 CONFIGPTHREADS()
 
 SET(BASE_LINK_LIB
		MY_SDK_LIB
		)


	
	
	SET(DIR_SRCS   
			StompCommandConstants.cpp
			StompFrame.cpp
		 )
	ADD_LIBRARY(StompLib   STATIC    ${DIR_SRCS}  )
	
	TARGET_LINK_LIBRARIES(StompLib ${BASE_LINK_LIB})

########################################

ELSEIF (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
   
ELSE ()  
   
ENDIF ()  
###############################################################################