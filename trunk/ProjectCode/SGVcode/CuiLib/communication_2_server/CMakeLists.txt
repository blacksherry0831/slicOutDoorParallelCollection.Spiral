############≈‰÷√OPENCV#########################################################
C11_Config()
###############################################################################
FIND_PACKAGE(Boost)
###############################################################################
ConfigOpenCv()
CONFIGQT()
###############################################################################
IF (CMAKE_SYSTEM_NAME MATCHES "Linux")  
 
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")

####################################### 
 ConfigMFC_2()
 CONFIGCURLHTTP()
 CONFIGPTHREADS()
 
 SET(BASE_LINK_LIB
		MY_SDK_LIB
		StompLib
		jsoncpp_lib
		SerialPort
		libcurl_http
		pthreads)

IF(Boost_FOUND)
	
	ConfigBoost()
	SET(DIR_SRCS   
			Comm2Server.cpp
			CommData.cpp	
			ResponseData.cpp
			DriveCarByCmd.cpp 
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