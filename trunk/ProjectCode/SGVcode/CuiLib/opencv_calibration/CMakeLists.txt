############����OPENCV#########################################################
C11_Config()
ConfigOpenCv()
###############################################################################
IF (CMAKE_SYSTEM_NAME MATCHES "Linux")  
	SET(DIR_SRCS 
			checkerboard.cpp
			calibration.cpp
			public.cpp) 
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
	ConfigMFC_2()
	CONFIGPTHREADS()

	SET(DIR_SRCS 
			checkerboard.cpp
			calibration.cpp
			public.cpp) 
ENDIF () 
############################################################ 
############################################################
		
ADD_LIBRARY(OpencvCalibration   STATIC    ${DIR_SRCS}  )

TARGET_LINK_LIBRARIES(OpencvCalibration) 
################################################################################