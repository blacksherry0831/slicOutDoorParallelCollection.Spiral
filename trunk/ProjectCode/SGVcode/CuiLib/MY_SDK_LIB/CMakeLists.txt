#cmake最低版本需求，不加入此行会受到警告信息
CMAKE_MINIMUM_REQUIRED(VERSION 3.8.1)
###############################################################################

###############################################################################

###############################################################################

C11_Config()  
ConfigOpenCv()


IF (CMAKE_SYSTEM_NAME MATCHES "Linux")  
 
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
	ConfigMFC_2()
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
   
ELSE ()  
   
ENDIF ()  
###############################################################################
SET(MY_SDK_SRCS
	Base.cpp
	PathCfg.cpp
	LatLng.cpp
	Line2Point.cpp
	BlockAnalysis.cpp
	BlockProperty.cpp
	OpencvCamera.cpp
	ImageProcess.cpp
	OutCircleFeature.cpp
	CrackDetection.cpp
	TimeMeasure.cpp
	VideoRecord.cpp)
ADD_LIBRARY(MY_SDK_LIB   STATIC    ${MY_SDK_SRCS})
###############################################################################
	
##############################################################################

##############################################################################
