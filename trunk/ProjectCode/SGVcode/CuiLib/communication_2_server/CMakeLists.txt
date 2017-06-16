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
	libcurl_http)

IF(Boost_FOUND)
	
	ConfigBoost()
	SET(DIR_SRCS   
			Comm2Server.cpp		 
		 )
	ADD_LIBRARY(Comm2ServerLib   STATIC    ${DIR_SRCS}  )
	
	TARGET_LINK_LIBRARIES(Comm2ServerLib ${BASE_LINK_LIB})

ELSE(Boost_FOUND)

   MESSAGE(FATAL_ERROR °±Boost library not found°±)

ENDIF(Boost_FOUND)
########################################

ELSEIF (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
   
ELSE ()  
   
ENDIF ()  
###############################################################################