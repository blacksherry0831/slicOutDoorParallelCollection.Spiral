############≈‰÷√OPENCV#########################################################
C11_Config()
ConfigOpenCv()
###############################################################################
IF (CMAKE_SYSTEM_NAME MATCHES "Linux")  

ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
	ConfigMFC_2()
	CONFIGPTHREADS()

	SET(DIR_SRCS   
			public.cpp) 
ENDIF () 
############################################################ 
############################################################
		
ADD_LIBRARY(OpencvCalibration   STATIC    ${DIR_SRCS}  )

TARGET_LINK_LIBRARIES(OpencvCalibration) 
################################################################################